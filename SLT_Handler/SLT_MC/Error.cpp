#include "StdAfx.h"
#include "Error.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CError::CError(void)
{
	m_pMainFrameWnd = nullptr;
	nCurrentJamCount = 0;
}


CError::~CError(void)
{
}

int CError::Initialize(CWnd* pMainFrameWnd)
{
	m_pMainFrameWnd = pMainFrameWnd;
	
	g_DB.SELECT_ERROR_MSG_TBL( m_AlarmMsgList );
	
	//InitDuplicateJamTime();
	return 0;
}

void CError::Finalize()
{

}

void CError::AlarmReport(int nAlid, CParamList &AlarmData, CBaseTaskManager* pSender/*=nullptr*/,std::vector<CPoint>& vFailPicker)
{
    m_csMsg.Lock();
	_tERROR_MSG* e = GetErrMsgPtr(nAlid);
	if( e == nullptr)
	{
		// add new alid
		_tERROR_MSG cErrMsgNew;
		cErrMsgNew.nALCD = 2;
		cErrMsgNew.nALED = 1;
		cErrMsgNew.nError_Level = 1;		
		sprintf_s(cErrMsgNew.szPart, sizeof(cErrMsgNew.szPart), "SYSTEM");
		sprintf_s(cErrMsgNew.szALID, sizeof(cErrMsgNew.szALID), "%d", nAlid);
		sprintf_s(cErrMsgNew.szEQP_ALID, sizeof(cErrMsgNew.szEQP_ALID), "%d", nAlid);
		cErrMsgNew.nPosition = 0;
		g_DB.UPDATE_ERROR_MSG_TBL(nAlid, cErrMsgNew);

		// 백업 alarm status
		std::vector<int> vAlarmIdList_Set;
		for each( auto item in m_AlarmMsgList ) {
			_tERROR_MSG* pItem = item.second.get();
			if( pItem != nullptr ) {
				if( pItem->bAlarmState == TRUE ) {
					vAlarmIdList_Set.push_back( atoi( pItem->szALID ) );
				}
			}
		}

		// alarm status 원복
		m_AlarmMsgList.clear();
		g_DB.SELECT_ERROR_MSG_TBL(m_AlarmMsgList);
		for each( auto item in m_AlarmMsgList ) {
			_tERROR_MSG* pItem = item.second.get();
			if( pItem != nullptr ) {
				for( std::vector<int>::iterator iter = vAlarmIdList_Set.begin(); iter != vAlarmIdList_Set.end(); iter++ )
				{
					if( atoi( pItem->szALID ) == *iter )
					{
						pItem->bAlarmState = TRUE;
						vAlarmIdList_Set.erase( iter );
						break;
					}
				}
			}
		}
		e = GetErrMsgPtr(nAlid);
	}
	BOOL bIsDupErr = FALSE;
	BOOL bAddErr = FALSE;
	if( e->bAlarmState != TRUE )
	{
		// ========================================================================================================
		// duplicate jam search, 중복 Jam 관련 처리 부분.
		_tDuplicateJamTime* pItem = nullptr;
		int nDuplicateJamCnt = m_DuplicateJam.m_fnGetCount();
		if(nDuplicateJamCnt > 0){
			for(int i=0; i< nDuplicateJamCnt ; i++){
				pItem = nullptr;
				pItem = (_tDuplicateJamTime*)m_DuplicateJam.m_fnGetData(i);
				if( pItem != nullptr ){
					double dSpanTime = (double)GetTickCount64() - (pItem->dOccurdTime);
					// 5분 이상이 지난 뒤 발생 한 경우 갱신한다.
					if(dSpanTime > 300000.0 && atoi(pItem->szALID) == nAlid && e->nError_Level >= eErrLevel_Warn)
					{
						m_DuplicateJam.m_fnDeleteData(i);
						_tDuplicateJamTime* pDuplItem = new _tDuplicateJamTime;
						sprintf_s(pDuplItem->szALID, sizeof(pDuplItem->szALID), "%d", nAlid);
						pDuplItem->dOccurdTime = (double)GetTickCount64();
						m_DuplicateJam.m_fnAddData(pDuplItem);
						bAddErr = TRUE;
						break;
					}else if(dSpanTime < 300000.0 && atoi(pItem->szALID) == nAlid && e->nError_Level >= eErrLevel_Warn){
						bIsDupErr =TRUE;
						break;
					}else if(i == nDuplicateJamCnt-1 && bIsDupErr == FALSE && bAddErr == FALSE && e->nError_Level >= eErrLevel_Warn){
						_tDuplicateJamTime* pDuplItem = new _tDuplicateJamTime;
						sprintf_s(pDuplItem->szALID, sizeof(pDuplItem->szALID), "%d", nAlid);
						pDuplItem->dOccurdTime = (double)GetTickCount64();
						m_DuplicateJam.m_fnAddData(pDuplItem);
					}
				}
			}
		}else{
			_tDuplicateJamTime* pDuplItem = new _tDuplicateJamTime;
			sprintf_s(pDuplItem->szALID, sizeof(pDuplItem->szALID), "%d", nAlid);
			pDuplItem->dOccurdTime = (double)GetTickCount64();
			m_DuplicateJam.m_fnAddData(pDuplItem);
		}
		
		//========================================================================================================

		ST_ALARM_RPT_OBJ tAlarmObj;
		tAlarmObj.nAlid = nAlid;
		tAlarmObj.pSender = pSender;
		tAlarmObj.bDuplicateAlarm = bIsDupErr;
		for(int i = 0; i < (int)vFailPicker.size(); i++){
			CPoint Picker = vFailPicker[i];
			tAlarmObj.vFailPicker.push_back(Picker);
		}
		vFailPicker.clear();
		HWND hWnd = m_pMainFrameWnd->GetSafeHwnd();
		::SendMessage(hWnd, UM_ALARM_REPORT, (WPARAM)&tAlarmObj, (LPARAM)&AlarmData);

		int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

		if( e->nError_Level > eErrLevel_Warn ){
			g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_JAM_CALL);
		}else/* if(e->nError_Level <= eErrLevel_Warn)*/{
			g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_REQUEST);
		}
		if(bIsDupErr == FALSE){
			if(e->nError_Level >= eErrLevel_Jam 
				&& CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) 
				//&& nRunMode == eRUN_MODE_ONLINE)
			{
				nCurrentJamCount = g_DMCont.m_dmEQP.GN(NDM0_CurLotJamCount);
				nCurrentJamCount++;
				g_DMCont.m_dmEQP.SN(NDM0_CurLotJamCount, nCurrentJamCount);
			}
		}

		e->bAlarmState = TRUE;
	}

	if (e != nullptr)
	{
		CString strAlarmLog;
		strAlarmLog.Format(_T("ALID = %d, PART = %s, KOREA = %s, ENGLISH = %s, CHINA = %s, JAPAN = %s, ERROR_LEVEL = %d, ALID = %s, ALCD = %d, ALED = %d, MBTYPE = %d, POSITION = %d"),
			nAlid, e->szPart, e->strKorea, e->strEnglish, e->strChina, e->strJapan, e->nError_Level, e->szALID, e->nALCD, e->nALED, e->nMB_Type, e->nPosition);
		MakeLog(strAlarmLog);
	}

    m_csMsg.Unlock();
}

BOOL CError::Get_Jam_Alarm_Set_State()
{
	BOOL bALST = FALSE;
	_tERROR_MSG* pItem = nullptr;
	for each( auto item in m_AlarmMsgList ) {
		pItem = item.second.get();
		if( pItem != nullptr ) {
			if( pItem->nError_Level >= eErrLevel_Jam && pItem->bAlarmState == TRUE ) {
				bALST = TRUE;
				break;
			}
		}
	}
	return bALST;
}

BOOL CError::IsSetAlarm(int nAlid)
{
	BOOL bALST = FALSE;
	_tERROR_MSG* pItem = GetErrMsgPtr(nAlid);
	if( pItem != nullptr ){
		bALST = pItem->bAlarmState;
	}
	return bALST;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : TRUE  = Start 불가능
             FALSE = Start 가능 알람
  COMMENTS : Start가 불가능한 알람이 떠있는지 확인 함수.
==============================================================================*/
BOOL CError::IsImpossibleStartAlarm()
{
	for each( auto item in m_AlarmMsgList ) {
		_tERROR_MSG* pCurItem = item.second.get();
		if( pCurItem != nullptr && pCurItem->bAlarmState == TRUE && pCurItem->nMB_Type == TWMB_SKIP_PASSWORD ) {
			return TRUE;
		}
	}
	return FALSE;
}

_tERROR_MSG* CError::GetErrMsgPtr(int nAlid)
{
	_tERROR_MSG* pItem = nullptr;
	pItem = m_AlarmMsgList[ nAlid ].get();
	return pItem;
}

void CError::GetErrMsgInfo(int nAlid, _tERROR_MSG& tErrMsg)
{
	_tERROR_MSG* pItem = nullptr;
	pItem = m_AlarmMsgList[ nAlid ].get();
	if( pItem != nullptr ) {
		tErrMsg = *pItem;
	}
}


int CError::RequestUserSelectionModal(int nAlid, int nBtnType)
{
	int nRes = 0;
	_tERROR_MSG e;
	GetErrMsgInfo(nAlid, e);
	CStringList rMsgList;
	GetDisplayMsg(nAlid, rMsgList);
	CString msg1, msg2;
	msg1 = rMsgList.GetHead();     // 영어
	if( rMsgList.GetCount()>1){    // 현지언어
		msg2 = rMsgList.GetTail();
	}

	char *pszFilePath = nullptr;
	pszFilePath = "D:\\techwing\\LOG\\VAT_ALARM.log";
	CLogger m_AlarmLogger;
	m_AlarmLogger.SetFileName(pszFilePath);
	//Amkor용 한글 Log
	int nLang = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_LANGUAGE);
	if(nLang == eSysLang_Kor)
		m_AlarmLogger.LOG("[ALARM][NUM=%d] %s",nAlid, msg2);
	else
		m_AlarmLogger.LOG("[ALARM][NUM=%d] %s",nAlid, msg1);
	//int nAlaramIdx = atoi(e.szEQP_ALID);

	switch (nBtnType)
	{
	case 0:
	{
		nRes = g_LibCommCtrl.MyMsg2("WARNNING", e.szEQP_ALID, e.szPart, msg1.GetBuffer(0), msg2.GetBuffer(0), M_ICONINFORMATION, M_YESNO, "YES", "NO");
	} break;
	case 1:
	{
		nRes = g_LibCommCtrl.MyMsg2("WARNNING", e.szEQP_ALID, e.szPart, msg1.GetBuffer(0), msg2.GetBuffer(0), M_ICONINFORMATION, M_OK, "OK");
	} break;
	case 2:
	{
		nRes = g_LibCommCtrl.MyMsg2("WARNNING", e.szEQP_ALID, e.szPart, msg1.GetBuffer(0), msg2.GetBuffer(0), M_ICONINFORMATION, M_YESNO, "RETRY", "EXIT");
	} break;
	case 3:
	{
		nRes = g_LibCommCtrl.MyMsg2("WARNNING", e.szEQP_ALID, e.szPart, msg1.GetBuffer(0), msg2.GetBuffer(0), M_ICONINFORMATION, M_YESNO, "NEXT", "EXIT");
	} break;
	default:
		break;
	}

	msg1.ReleaseBuffer();
	msg2.ReleaseBuffer();
	
	switch (nRes)
	{
	case IDOK:			m_AlarmLogger.LOG("[USER SELECT] OK", msg2); break;
	case IDCANCEL:		m_AlarmLogger.LOG("[USER SELECT] CANCEL", msg2); break;
	case IDABORT:		m_AlarmLogger.LOG("[USER SELECT] ABORT", msg2); break;
	case IDRETRY:		m_AlarmLogger.LOG("[USER SELECT] RETRY", msg2); break;		
	case IDYES:			m_AlarmLogger.LOG("[USER SELECT] YES", msg2); break;
	case IDNO:			m_AlarmLogger.LOG("[USER SELECT] NO", msg2); break;

	default:

		break;
	}
	return nRes;
}

// system lang 설정이 영문이면 한개만 리턴하고, 영문이 아니면 두번째에 시스템 설정 언어를 리턴한다. (첫번째는 무조건 영어)
void CError::GetDisplayMsg(int nAlid, CStringList &rMsgList)
{
	_tERROR_MSG e;
	GetErrMsgInfo(nAlid, e);

	rMsgList.AddTail(e.strEnglish);

	int nLang = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_LANGUAGE);
	if( nLang == eSysLang_Kor ){
		rMsgList.AddTail(e.strKorea);
	}else if( nLang == eSysLang_Chi ){
		rMsgList.AddTail(e.strChina);
	}else if( nLang == eSysLang_Jap ){
		rMsgList.AddTail(e.strJapan);
	}
}

void CError::InitDuplicateJamTime()
{
	_tERROR_MSG* pItem = nullptr;
	for each( auto item in m_AlarmMsgList ) {
		_tDuplicateJamTime* pDuplItem = new _tDuplicateJamTime;
		pItem = item.second.get();
		if( pItem != nullptr ) {
			strcpy_s( pDuplItem->szALID, sizeof( pDuplItem->szALID ), pItem->szALID );
			pDuplItem->dOccurdTime = (double)GetTickCount64();

			m_DuplicateJam.m_fnAddData( pDuplItem );
		}
	}
}

int CError::GetCurrentLotJamCount()
{
	return nCurrentJamCount;
}

void CError::ResetCurrentLotJamCount()
{
	nCurrentJamCount = 0;
}

void CError::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_ALARM), Debug, "", __LINE__, NULL, tmpMsg);
}


//Vision Data 수신 Error 발생 시 Message 내용에 Error Code 및 운영 절차를 추가하여 Alarm을 발생시킨다.
int CError::CheckVisionError(int nErrorDiscription)
{
	int nErrorCode = ERR_STATION1_VISION_CMD_RESP_DATA_IS_WRONG;

	switch (nErrorDiscription)
	{
	case ERRORCODE_INSPECTION_SQ_WAIT:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_INSPECTION_SQ_WAIT;
	}break;
	case ERRORCODE_MEASURE_TYPE:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_MEASURE_TYPE;
	}break;
	case ERRORCODE_INPECTION_SQ_START:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_INPECTION_SQ_START;
	}break;
	case ERRORCODE_CAMERA_RESET_START:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_CAMERA_RESET_START;
	}break;
	case ERRORCODE_CAMERA_RESET_WAIT:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_CAMERA_RESET_WAIT;
	}break;
	case ERRORCODE_PRE_GRAB_SET:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_PRE_GRAB_SET;
	}break;
	case ERRORCODE_GRAB_FAIL:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_GRAB_FAIL;
	}break;
	case ERRORCODE_GRAB_WAIT:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_GRAB_WAIT;
	}break;
	case ERRORCODE_RECEIVEDATA:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_RECEIVEDATA;
	}break;
	case ERRORCODE_INSPECTION:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_INSPECTION;
	}break;
	case ERRORCODE_INSPECTION_TIMEOUT:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_INSPECTION_TIMEOUT;
	}break;
	case ERRORCODE_RESULT_SEND:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_RESULT_SEND;
	}break;
	case ERRORCODE_AF_PRE_START:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_AF_PRE_START;
	}break;
	case ERRORCODE_AF_WAIT:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_AF_WAIT;
	}break;
	case ERRORCODE_AF_RESULT:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_AF_RESULT;
	}break;
	case ERRORCODE_AF_FORCED_STOP:
	{
		nErrorCode = ERR_STATION1_VISION_ERRCODE_AF_FORCED_STOP;
	}break;
	default:
	break;
	}
	return nErrorCode;
}


// VAT측정 시, Alarm 활성화 상태 확인 함수. [10/1/2025 dohyeong.kim]
BOOL CError::IsVATAlarmActivate()
{
	for each(auto item in m_AlarmMsgList) {
		_tERROR_MSG* pCurItem = item.second.get();
		if (pCurItem != nullptr && pCurItem->bAlarmState == TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}