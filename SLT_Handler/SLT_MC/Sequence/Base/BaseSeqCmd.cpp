#include "StdAfx.h"
#include "BaseSeqCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;

CBaseSeqCmd::CBaseSeqCmd(void)
{
	CBaseSeqCmd::MakeCmdSerial(this);
	m_pSender               = NULL;
	m_pReceive              = NULL;
	m_nCmdId                = 0;
	m_eCmdStatus            = eSeqCmdStatus_Ready;
	m_nStep                 = 0;
	m_nStep_New             = -1;
	m_nStep_Pre             = -1;
	m_nLastError            = 0;
	m_nLast_Executing_Step  = 0;
	memset(m_szCmdName, 0x00, sizeof(m_szCmdName));
	m_nWorkCmdStage = -1;

	m_nCmdSeqMode = 0;

	//VAT 관련 변수
// 	m_bIsSimul = g_IO.GetDriverType() == _DIO_TYPE_SIMUL_;
// 	m_nActionCnt = 0;

	InitializeCriticalSection( m_csSeqCmdStatus );
}

CBaseSeqCmd::CBaseSeqCmd(const CBaseSeqCmd& src)
{
	*this = src;
}


CBaseSeqCmd::~CBaseSeqCmd(void)
{
}

CBaseSeqCmd& CBaseSeqCmd::operator = (const CBaseSeqCmd& src)
{
	if( this == &src ){
		return *this;
	}

	m_pSender                  = src.m_pSender;
	m_pReceive                 = src.m_pReceive;
	m_nSerialNo                = src.m_nSerialNo;
	m_nCmdId                   = src.m_nCmdId;
	m_ParamList                = src.m_ParamList;
	m_nLastError               = src.m_nLastError;
	m_LastErrorData            = src.m_LastErrorData;
	m_eCmdStatus               = src.m_eCmdStatus;
	m_nLast_Executing_Step     = src.m_nLast_Executing_Step;
	m_nStep                    = src.m_nStep;
	m_nStep_New                = src.m_nStep_New;
	m_nStep_Pre                = src.m_nStep_Pre;
	memcpy(m_szCmdName, src.m_szCmdName, sizeof(m_szCmdName));
	m_nWorkCmdStage            = src.m_nWorkCmdStage;

	m_nCmdSeqMode = src.m_nCmdSeqMode;

	return *this;
}


BOOL CBaseSeqCmd::Running()
{
	BOOL bComplete = FALSE;

	switch( m_eCmdStatus )
	{
	case eSeqCmdStatus_Queue:			{ SetCmdStatus(eSeqCmdStatus_Setup);	} break;
	case eSeqCmdStatus_Setup:			{ bComplete = State_Setup();		    } break;
	case eSeqCmdStatus_Executing:		{ bComplete = State_Executing();	    } break;
	case eSeqCmdStatus_Recovery:		{ bComplete = State_Recovery();			} break;
	case eSeqCmdStatus_Pause:			{ bComplete = State_Pause();		    } break;
	case eSeqCmdStatus_NormalComplete:	{ bComplete = State_NormalComplete();	} break;
	case eSeqCmdStatus_Alarm:			{ bComplete = State_Alarm();			} break;
	case eSeqCmdStatus_AlarmComplete:	{ bComplete = State_AlarmComplete();	} break;
	case eSeqCmdStatus_Aborting:		{ bComplete = State_Aborting();			} break;
	case eSeqCmdStatus_AbortComplete:	{ bComplete = State_AbortComplete();	} break;
	}

	return bComplete;
}

void CBaseSeqCmd::ReportCmdEnd(eSeqCmdStatus a_eEndCode/*=CBaseSeqCmd::eNormalComplete*/)
{
	SetCmdStatus(a_eEndCode);
	m_pSender->OnCommandRsp(this, CMD_EVT_CMD_END, (WPARAM)a_eEndCode, (LPARAM)NULL);
}

// 상태를 alarm으로 변경 후 Sender에게 Alarm이 발생되었음을 보고 한다.
void CBaseSeqCmd::ReportAlarm(int nAlid, CParamList AlarmData, std::vector<CPoint> &vPocket_Picker)
{
	if( m_nCmdSeqMode == eSeqMode_Manual ){
		SetCmdStatus(eSeqCmdStatus_Aborting);
	}else{
		SetCmdStatus(eSeqCmdStatus_Alarm);
	}
	m_LastErrorData.m_fnClear();
	m_LastErrorData = AlarmData;
	m_nLastError = nAlid;

	m_pSender->OnCommandRsp(this, CMD_EVT_ALARM, (WPARAM)nAlid, (LPARAM)&AlarmData, vPocket_Picker);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-10-09]
  NAME     : 
  PARAMS   : 
  RETURN   : enum eSeqCmdStatus 
  COMMENTS : 
==============================================================================*/
int CBaseSeqCmd::GetCmdStatus() 
{ 
	int nSts = 0;
	m_csSeqCmdStatus.Lock();
	nSts = m_eCmdStatus;
	m_csSeqCmdStatus.Unlock();
	return nSts; 
}

int CBaseSeqCmd::GetLastError() 
{ 
	return m_nLastError; 
}

TCHAR * CBaseSeqCmd::GetCmdName()
{
	return m_szCmdName;
}

int CBaseSeqCmd::GetWorkCmdStage()
{
	return m_nWorkCmdStage;
}

void CBaseSeqCmd::MakeCmdSerial(CBaseSeqCmd* pSelf)
{
	static int nCmdSerialNo = 0;
	if( nCmdSerialNo > 99999 ){
		nCmdSerialNo = 0;
	}
	pSelf->m_nSerialNo = nCmdSerialNo;
}

int CBaseSeqCmd::SetCmdStatus(eSeqCmdStatus a_eNewStatus, int nNewStep/*=0*/) 
{ 
	m_csSeqCmdStatus.Lock();
	//m_nStep      = nNewStep;
	m_nStep_New  = nNewStep;
	m_eCmdStatus = a_eNewStatus;
	m_csSeqCmdStatus.Unlock();
	return 0;
}

//------------------------------------------------------------------->>

/*
* 명령의 상태에 해당 될 때 함수들이 호출 됩니다. 
* 함수들은 상태가 바꾸거나, return 값을 TRUE로 하기 전까지 계속 호출 됩니다.
* Retur의 의미는 명령의 종료 입니다. CBaseTaskManager에서 호출 함.
*/

BOOL CBaseSeqCmd::State_Setup()
{
	SetCmdStatus(eSeqCmdStatus_Executing);
	return _seq_cmd_not_end_;
}

BOOL CBaseSeqCmd::State_Executing()
{
	return _seq_cmd_not_end_;
}
BOOL CBaseSeqCmd::State_Pause()
{
	return _seq_cmd_not_end_;
}
BOOL CBaseSeqCmd::State_Recovery()
{
	return _seq_cmd_not_end_;
}
BOOL CBaseSeqCmd::State_NormalComplete()
{
	return _seq_cmd_end_;
}
BOOL CBaseSeqCmd::State_Alarm()
{
	return _seq_cmd_not_end_;
}

BOOL CBaseSeqCmd::State_AlarmComplete()
{
	return _seq_cmd_end_;
}

BOOL CBaseSeqCmd::State_Aborting()
{
	//SetCmdStatus(eSeqCmdStatus_AbortComplete);
	return _seq_cmd_not_end_;
}
BOOL CBaseSeqCmd::State_AbortComplete()
{
	return _seq_cmd_end_;
}
// <<-------------------------------------------------------------------

// VAT 사용 함수.
// void CBaseSeqCmd::_Simul_SeqControl(int control_time)
// {
// 	if (m_bIsSimul)
// 		Sleep(control_time);
// }
// 
// BOOL CBaseSeqCmd::_Simul_IsRemainSeqCycle()
// {
// 	if (m_bIsSimul && m_nActionCnt < VAT_SIM_MAX_ACTION_COUNT) {
// 		m_nActionCnt++;
// 		return TRUE;
// 	}
// 
// 	return FALSE;
// }
// 
// void CBaseSeqCmd::_Simul_ClearParam()
// {
// 	if (m_bIsSimul)
// 		m_nActionCnt = 0;
// }