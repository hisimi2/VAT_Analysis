#include "stdafx.h"
#include "StackerCmd_FullTray.h"


CStackerCmd_FullTray::CStackerCmd_FullTray()
{
	m_nCmdId = eSeqCmd_Stacker_FullTray;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
	m_nLast_Do_Step = 0;
}

CStackerCmd_FullTray::~CStackerCmd_FullTray()
{
}

int CStackerCmd_FullTray::CheckParam()
{
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_FullTray::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_FullTray::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_FullTray::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_FullTray::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_FullTray::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_Setup()
{
	m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_Executing()
{
	// check alarm
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case eExcutingStep_Start: // check interlock
	{
		m_nLast_Do_Step = m_nStep;
		m_nStep = eExcutingStep_FullTrayCheck;
	}break;

	case eExcutingStep_FullTrayCheck:
	{
		pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check stacker full  (STR)", m_szCmdName, m_nStep);
		if (pTask->ChkFullTray(DEF_ON) != true) {
			int nAlid = ERR_STACKER_1_TRAY_FULL + pTask->GetIndex();
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
			ReportAlarm(nAlid, AlarmData);
			break;
		}
		pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check stacker full (END)", m_szCmdName, m_nStep);

		m_nLast_Do_Step = m_nStep;
		m_nStep = eExcutingStep_End;
	}break;
	case eExcutingStep_End:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
	}break;
	}
	return CBaseSeqCmd::State_Executing();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_Pause()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]"), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		m_nStep++;
	}break;
	case 1:
	{
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_Recovery()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]"), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		m_swTimeout.ResetStartTime();
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step);
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}
	return CBaseSeqCmd::State_Recovery();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_NormalComplete()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_Alarm()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]"), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
	{
		m_nStep++;
	}break;
	case 1:
	{
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}
	return CBaseSeqCmd::State_Alarm();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_Aborting()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [ABORT] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		m_nStep++;
	}break;
	case 1:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}
	return CBaseSeqCmd::State_Aborting();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_FullTray::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}






