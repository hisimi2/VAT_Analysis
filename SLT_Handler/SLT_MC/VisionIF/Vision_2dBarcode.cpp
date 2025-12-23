#include "StdAfx.h"
#include "Vision_2dBarcode.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

const int DEFAULT_2DID_VEC_CAPA = 60000;

CVision_2dBarcode::CVision_2dBarcode(void)
{
	m_v2dBarcode.clear();
	m_v2dBarcode.reserve(DEFAULT_2DID_VEC_CAPA);
}


CVision_2dBarcode::~CVision_2dBarcode(void)
{
}


void CVision_2dBarcode::ResetData()
{
	m_v2dBarcode.clear();
	g_DB.DELETE_BARCODE_2D_ALL();
}

void CVision_2dBarcode::Push2dData( std::vector<ST_2D_BARCODE_INFO>& vBarcode )
{
	if( !vBarcode.empty() )
	{
		m_cs.Lock();
		for(int i=0; i<(int)vBarcode.size(); i++)
		{
			m_v2dBarcode.push_back(vBarcode[i]);
		}	
		m_cs.Unlock();
		AddRecode_DB(vBarcode);

	}
}
/*=============================================================================
  AUTHOR   : checking Duplicate 2D ID 
  NAME     : // By ksj [160531] 2d barcode data
  PARAMS   : sz2dData = read barcode information
  RETURN   : FALSE = NOT duplication
             TRUE  = Duplication
  COMMENTS : vision이 읽은 2d barcode에 대한 중복 검사를 실행한다.
==============================================================================*/
struct Equal2dBarcode
{
	bool operator () (const ST_2D_BARCODE_INFO & rhs) const
	{
		return (strcmp(szBarcode,rhs.szBarcode) == 0);
	}

	Equal2dBarcode(const char * n) : szBarcode(n) {}

private:

	const char * szBarcode;
};

bool CVision_2dBarcode::IsDupData(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<ST_2D_BARCODE_INFO>& v2dBarcodeBakForCancel, std::vector<CPoint>& v2dDuplicateCoordi)
{

	m_cs.Lock();
// 	if(m_v2dBarcode.empty() ) {
// 		m_cs.Unlock();
// 		return false; 
// 	}

	int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	std::vector<std::pair<_tVisionReqData, CPoint>> vReq2d;
	std::vector<ST_2D_BARCODE_INFO> v2DBARCODE;
	ST_2D_BARCODE_INFO t2dBarcode;

	ST_DD_DEVICE tDeviceData;
	CString strErr = _T("");

	std::vector<CPoint> vLoadPocket; vLoadPocket.clear();

	for (int nY = 0; nY < nPocketCntY; nY++) {
		for (int nX = 0; nX < nPocketCntX; nX++) {
			char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
			if(setting == 'L')
				vLoadPocket.push_back(CPoint(nX, nY));
		}
	}

/*	int nPocketNum = 0;*/

	for(int i=0; i < (int)vLoadPocket.size(); i++ )
	{
// 		int nPocketX = i;
// 		int nPocketY = 0;
// 		g_pBarcode->ChangePocketNum(nCamNo, nPocketX, nPocketY, nPocketNum);

		tDeviceData.clear();
		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1+nTableIdx, vLoadPocket[i].x, vLoadPocket[i].y, &tDeviceData);
		strErr.Format("%s", tDeviceData.szErrString);
		strErr.MakeUpper();

		if(tDeviceData.sExist == eDvcExist && strcmp(strErr, DEF_2D_READ_ERR_DEVICE) != 0 && strcmp(strErr, DEF_TEST_RESULT_WRONG_APN) != 0 || g_TaskLoadTbl[nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual){
			std::pair<_tVisionReqData, CPoint> p;
			//p.first  = vReqData[ x + nPocketX*(y- nPocketY / 2) ];
			p.first = vReqData[i];
			p.second = CPoint(vLoadPocket[i].x, vLoadPocket[i].y);
			vReq2d.push_back(p);

			//nSiteNo
			//nIndexArmNo
			//szDataValue
			//szReadTime
			t2dBarcode.clear();
			//t2dBarcode.nSiteNo = x + nPocketX*(y - nPocketY / 2);
			t2dBarcode.nSiteNo = i;
			t2dBarcode.nIndexArmNo = nTableIdx / 2;
			strcpy_s(t2dBarcode.szBarcode, sizeof(t2dBarcode.szBarcode), vReqData[i].szDataValue);
			SYSTEMTIME t;
			::GetLocalTime(&t);
			CString strTime;
			MakeTimeToString(t,strTime);
			sprintf_s(t2dBarcode.szReadTime, "%s", strTime.GetString() );
			v2DBARCODE.push_back(t2dBarcode);
		}
	}

	
	// Table 내에 중복이 있는지 점검한다.
	for(int nOrg=0; nOrg < (int)vReq2d.size(); nOrg++){
		for(int nCmp=0; nCmp < (int)vReq2d.size(); nCmp++){
			if( nOrg == nCmp ) { continue; } // same pos isn't compare
			if(strcmp(vReq2d[nOrg].first.szDataValue, vReq2d[nCmp].first.szDataValue) == 0){
				v2dDuplicateCoordi.push_back(vReq2d[nCmp].second);
				v2dDuplicateCoordi.push_back(vReq2d[nOrg].second);
				m_cs.Unlock();
				return TRUE;
			}
		}
	}
	
	BOOL bChkDuplicate = FALSE;
	for(std::vector<std::pair<_tVisionReqData, CPoint>>::iterator itRevData= vReq2d.begin(); itRevData != vReq2d.end(); itRevData++){
		std::vector<ST_2D_BARCODE_INFO>::iterator it;
		it = find_if(m_v2dBarcode.begin(), m_v2dBarcode.end(), Equal2dBarcode(itRevData->first.szDataValue) );
		if(it != m_v2dBarcode.end()){
			bChkDuplicate = TRUE;
			// coordinate
			v2dDuplicateCoordi.push_back(itRevData->second);
		}
	}

	if(bChkDuplicate == TRUE){
		m_cs.Unlock();
		return TRUE;
	}

	//Data 중복이 없으면 m_v2dBarcode에 Data 저장.
	Push2dData(v2DBARCODE);
	v2dBarcodeBakForCancel = v2DBARCODE;
	m_cs.Unlock();
	return FALSE;
}

void CVision_2dBarcode::DelData( char* sz2dData )
{
	m_cs.Lock();
	if( !m_v2dBarcode.empty() ){
		vector<ST_2D_BARCODE_INFO>::reverse_iterator rit;
		rit = find_if(m_v2dBarcode.rbegin(), m_v2dBarcode.rend(), Equal2dBarcode(sz2dData) );
		if( rit != m_v2dBarcode.rend() ){
			vector<ST_2D_BARCODE_INFO>::iterator it = (++rit).base();
			g_DB.DELETE_BARCODE_2D(sz2dData, it->szReadTime);
			m_v2dBarcode.erase(it);
		}
	}
	m_cs.Unlock();
}

int CVision_2dBarcode::LoadData()
{
	m_v2dBarcode.clear();
	g_DB.SELECT_BARCODE_2D_ALL(m_v2dBarcode);
	return TRUE;
}

void CVision_2dBarcode::SaveHistoryData()
{
}


int CVision_2dBarcode::IsMarkValidate(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<std::pair<CPoint, CString>> &v2DErrorData)
{
	//Amkor && WATT PKG
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	if (strcmp(szCompany, DEF_COMPANY_AMKOR) != 0 /*|| (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0 &&nProjectOption != DEF_PROJECT_WATT)*/) {
		return TRUE;
	}

	m_cs.Lock();

	//사용자 입력 Mark Get

	char szMark[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_MARK_LIST, szMark, sizeof(szMark));

	CString str2dMark = _T("");
	str2dMark.Format("%s", szMark);

	//검사 할것 없으면 return TRUE
	if (str2dMark == _T("")) {
		m_cs.Unlock();
		return TRUE;
	}
		

	//Parsing 후, Mark만 저장.
	std::vector<CString> v2dMark;
	v2dMark.clear();
	int nNextToken = 0;
	CString tkn = _T("");
	for (int i = 0; i < DEF_MAX_TOKENIZE_2D_MARK; i++) {
		tkn = str2dMark.Tokenize(_T(","), nNextToken);
		tkn.TrimLeft();
		tkn.TrimRight();
		v2dMark.push_back(tkn);

		if (str2dMark.GetLength() <= nNextToken || nNextToken == (-1))
			break;
	}


	//2d 촬영 Data와 Mark 비교.
	int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	ST_DD_DEVICE tDeviceData;
	CString strErr = _T("");

	std::vector<CPoint> vTblPocket; vTblPocket.clear();

	for (int nY = 0; nY < nPocketCntY; nY++) {
		for (int nX = 0; nX < nPocketCntX; nX++) {
			char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
			if (setting == 'L')
				vTblPocket.push_back(CPoint(nX, nY));
		}
	}

	int nInvalidateCnt = 0;
	int nPocketNum = 0;
	for (int i = 0; i < (int)vTblPocket.size(); i++)
	{
		tDeviceData.clear();
		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);
		strErr.Format("%s", tDeviceData.szErrString);
		strErr.MakeUpper();

		if (tDeviceData.sExist == eDvcExist && (strcmp(strErr, DEF_2D_READ_ERR_DEVICE) != 0 && strcmp(strErr, DEF_TEST_RESULT_WRONG_APN) != 0 && strcmp(vReqData[i].szDataValue, _T("ERROR")) != 0) || g_TaskLoadTbl[nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual) {
			CString str2dData = _T("");
			str2dData.Format("%s", vReqData[i].szDataValue);


			//Data 비교.
			int nErrCnt = 0;
			int nStartIdx = g_DMCont.m_dmHandlerOpt.GN(NDM11_2D_MARK_START_INDEX);
			for (int i = 0; i < v2dMark.size(); i++) {
				//위치 유효한지 확인.
				//시작 위치 + mark 길이 <= 2d read한 길이보다 작거나 같으면 Parsing 가능
				int n2dMarkLength = v2dMark[i].GetLength();
				int nRead2dMarkLength = str2dData.GetLength();
				if ((nStartIdx - 1) + v2dMark[i].GetLength() <= str2dData.GetLength()) {
					//2d 촬영 data를 작업자가 설정한 Data 위치 부터 Mark 길이만큼 Parsing.
					CString strCompareChar = str2dData.Mid((nStartIdx - 1), v2dMark[i].GetLength());
					if (strCompareChar != v2dMark[i] && v2dMark[i] != _T("")) { //입력 mark와 다르면 반환.
						nErrCnt++;
					}
				}
				else {
					nErrCnt++;
				}
			}
			//이전 비교문
// 			CString strCompareData = str2dData.Right(2); //끝에서 2자리 비교
// 			int nErrCnt = 0;
// 			for (int i = 0; i < v2dMark.size(); i++) {
// 				if (strCompareData != v2dMark[i] && v2dMark[i] != _T("")) { //입력 mark와 다르면 반환.
// 					nErrCnt++;
// 					
// 				}
// 			}

			//입력 2d Mark와 맞는게 없음
			if (nErrCnt  == v2dMark.size()) {
				nInvalidateCnt++; //맞지 않는 Device Count.

				std::pair<CPoint, CString> p;
				p.first = CPoint(vTblPocket[i].x, vTblPocket[i].y);
				p.second = str2dData;
				v2DErrorData.push_back(p);
			}

		}
	}

	if (nInvalidateCnt > 0) {
		m_cs.Unlock();
		return FALSE;
	}

	m_cs.Unlock();

	return TRUE;
}


int CVision_2dBarcode::IsShortLen(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<CPoint> &v2DErrorData)
{
	m_cs.Lock();

	std::string strOrg = _T("");
// 	std::pair<std::string, CPoint> pOrg;
// 	//초기화
// 	pOrg.first = _T("");
// 	pOrg.second = CPoint(-1, -1);


	std::vector<std::pair<std::string, CPoint>> vRecvData;
	std::pair<std::string, CPoint> p;
	//std::vector<std::pair<_tVisionReqData, CPoint>> vReq2d;
	int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	ST_DD_DEVICE tDeviceData;
	CString strErr = _T("");
	

	std::vector<CPoint> vLoadPocket; vLoadPocket.clear();

	for (int nY = 0; nY < nPocketCntY; nY++) {
		for (int nX = 0; nX < nPocketCntX; nX++) {
			char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
			if (setting == 'L')
				vLoadPocket.push_back(CPoint(nX, nY));
		}
	}


	int nPocketNum = 0;
	for (int i = 0; i < (int)vLoadPocket.size()/*nPocketCntX*/; i++)
	{
		//Cam 1개당 Barcode 4개.
		tDeviceData.clear();
		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nTableIdx, vLoadPocket[i].x, vLoadPocket[i].y, &tDeviceData);
		strErr.Format("%s", tDeviceData.szErrString);
		strErr.MakeUpper();

		if (tDeviceData.sExist == eDvcExist && strcmp(strErr, DEF_2D_READ_ERR_DEVICE) != 0 && strcmp(strErr, DEF_TEST_RESULT_WRONG_APN) != 0 || g_TaskLoadTbl[nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual) {
			p.first = vReqData[i].szDataValue;
			p.second = CPoint(vLoadPocket[i].x, vLoadPocket[i].y);
			vRecvData.push_back(p);




// 			if (strOrg.empty()) {
// 				strOrg = vReqData[i].szDataValue;
//  			}
			//가장 긴놈 저장
			if (strOrg.length() < p.first.length() ) {
				strOrg = vReqData[i].szDataValue;
			}

		}
	}
	
	BOOL bIsShort = FALSE;
	for (std::vector<std::pair<std::string, CPoint>>::iterator iter = vRecvData.begin(); iter != vRecvData.end(); iter++) { //checking exception all data
		if (strOrg.length() != (*iter).first.length()) {
			bIsShort = TRUE;
			v2DErrorData.push_back((*iter).second);			
		}																																																												
	}
	
	if (bIsShort) {
		m_cs.Unlock();
		return TRUE;
	}
// 	for(std::vector<std::string>::iterator iter=vRecData.begin(); iter!=vRecData.end(); iter++){ //checking exception all data
// 		if( strOrg.length() != (*iter).length()){
// 			m_cs.Unlock();
// 			return TRUE;
// 		}
// 	}

	m_cs.Unlock();

	return FALSE;
}

int CVision_2dBarcode::IsSameLength(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<CPoint>& v2DLengthFailPocket, std::vector<_ST_2DID_SAME_LENGTH>& v2DSameLength)
{
	m_cs.Lock();

	std::string strOrg = _T("");
	// 	std::pair<std::string, CPoint> pOrg;
	// 	//초기화
	// 	pOrg.first = _T("");
	// 	pOrg.second = CPoint(-1, -1);


	std::vector<std::string> vRecData;

	std::vector<std::pair<std::string, CPoint>> vRecvData;
	std::pair<std::string, CPoint> p;
	//std::vector<std::pair<_tVisionReqData, CPoint>> vReq2d;
	int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
	int n2DIDLength = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_2DID_LENGTH);

	if (n2DIDLength <= 0)
	{
		m_cs.Unlock();
		return TRUE;
	}
	

	ST_DD_DEVICE tDeviceData;
	CString strErr = _T("");


	std::vector<CPoint> vLoadPocket; vLoadPocket.clear();

	for (int nY = 0; nY < nPocketCntY; nY++) {
		for (int nX = 0; nX < nPocketCntX; nX++) {
			char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
			if (setting == 'L')
				vLoadPocket.push_back(CPoint(nX, nY));
		}
	}


	int nPocketNum = 0;
	for (int i = 0; i < (int)vLoadPocket.size()/*nPocketCntX*/; i++)
	{
		//Cam 1개당 Barcode 4개.
		// 		if (i > 3) {
		// 			nCamNo += 1;
		// 		}
		// 
		// 		int nPocketX = i % 4;
		// 		int nPocketY = 0;
		//		g_pBarcode->ChangePocketNum(nCamNo, nPocketX, nPocketY, nPocketNum);

		tDeviceData.clear();
		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nTableIdx, vLoadPocket[i].x, vLoadPocket[i].y, &tDeviceData);
		strErr.Format("%s", tDeviceData.szErrString);
		strErr.MakeUpper();

		if (tDeviceData.sExist == eDvcExist && strcmp(strErr, DEF_2D_READ_ERR_DEVICE) != 0 && strcmp(strErr, DEF_TEST_RESULT_WRONG_APN) != 0 || g_TaskLoadTbl[nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual)
		{
			if (n2DIDLength != strlen(vReqData[i].szDataValue))
			{
				_ST_2DID_SAME_LENGTH st2DSameLength;
				CPoint cPoint;
				st2DSameLength.nX = cPoint.x = vLoadPocket[i].x;
				st2DSameLength.nY = cPoint.y = vLoadPocket[i].y;
				st2DSameLength.strBarcode = vReqData[i].szDataValue;
				v2DLengthFailPocket.push_back(cPoint);
				v2DSameLength.push_back(st2DSameLength);
			}
		}
	}
	m_cs.Unlock();

	return FALSE;
}


bool CVision_2dBarcode::IsFullError(std::vector<_tVisionReqData>& vReqData)
{
	bool bRet = false;
	int nErrCnt = 0;
	for (int i = 0; i < (int)vReqData.size();/*8*/ i++)
	{
		if (strcmp(vReqData[i].szDataValue, _T("ERROR")) == 0)
		{
			nErrCnt++;
		}
	}

	if (nErrCnt == vReqData.size())
		bRet = true;
	
	return bRet;
}

void CVision_2dBarcode::AddRecode_DB( std::vector<ST_2D_BARCODE_INFO> &vec )
{
	for(std::vector<ST_2D_BARCODE_INFO>::size_type i = 0; i<vec.size(); i++){
		g_DB.INSERT_BARCODE_2D(vec[i]);
	}
}


void CVision_2dBarcode::MakeTimeToString( SYSTEMTIME t, CString& strTime )
{
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds );
}



void CVision_2dBarcode::DelData(std::vector<ST_2D_BARCODE_INFO> vReqData)
{
	for(std::vector<ST_2D_BARCODE_INFO>::iterator it=vReqData.begin(); it!=vReqData.end(); it++){
		DelData(it->szBarcode);
	}
}