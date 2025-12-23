#include "StdAfx.h"
#include "BaseAutoSeqManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBaseAutoSeqManager::CBaseAutoSeqManager()
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;
	m_nLast_Excuting_Step = 0;

}


CBaseAutoSeqManager::~CBaseAutoSeqManager()
{
}


void CBaseAutoSeqManager::ChangeMainState( eAutoLotState nAutoRunMainStep, int nAutoRunSubStep/*=0*/ )
{
	m_csAutoSeqCmdStatus.Lock();
	m_nAutoRunBackUpMainStep = m_nAutoRunMainStep;
	m_nAutoRunMainStep = nAutoRunMainStep;
	ChangeSubState( nAutoRunSubStep );
	m_csAutoSeqCmdStatus.Unlock();

}

void CBaseAutoSeqManager::ChangeSubState( int nAutoRunSubStep )
{
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;
	m_nAutoRunSubStep = nAutoRunSubStep;
}

eAutoLotState CBaseAutoSeqManager::GetCurrentLotState()
{
	return m_nAutoRunMainStep;
}

// CString CBaseAutoSeqManager::GetLotCmdToString( eAutoLotCmd eLotCmd )
// {
// 	CString strLotCmd = _T( "" );
// 	switch( eLotCmd )
// 	{
// 	case eAutoLotCmd_Initial: { strLotCmd = _T( "Initial" ); } break;
// 	case eAutoLotCmd_Start: { strLotCmd = _T( "Start" ); } break;
// 	case eAutoLotCmd_Pause: { strLotCmd = _T( "Pause" ); } break;
// 	case eAutoLotCmd_Resume: { strLotCmd = _T( "Resume" ); } break;	
// 	case eAutoLotCmd_Retry: { strLotCmd = _T( "Retry" ); } break;
// 	case eAutoLotCmd_Skip: { strLotCmd = _T( "Skip" ); } break;
// 	case eAutoLotCmd_CleanOut: { strLotCmd = _T( "CleanOut" ); } break;
// 	case eAutoLotCmd_AlarmPause: { strLotCmd = _T( "AlarmPause" ); } break;
// 	default: { strLotCmd = _T( "None" ); } break;
// 	}
// 
// 	return strLotCmd;
// }

CString CBaseAutoSeqManager::GetLotStateToString( eAutoLotState nState )
{
	CString strLotStatus = _T( "" );
	switch( nState )
	{
	case eAutoLotState_InitNeed: { strLotStatus = _T( "InitNeed" ); } break;
	case eAutoLotState_Initialize: { strLotStatus = _T( "Initialize" ); } break;
	case eAutoLotState_Idle: { strLotStatus = _T( "Idle" ); } break;
	case eAutoLotState_Executing: { strLotStatus = _T( "Executing" ); } break;
	case eAutoLotState_Pause: { strLotStatus = _T( "Pause" ); } break;
	case eAutoLotState_Resume: { strLotStatus = _T( "Resume" ); } break;
	case eAutoLotState_AlarmStop: { strLotStatus = _T( "AlarmStop" ); } break;
	case eAutoLotState_NormalComplete: { strLotStatus = _T( "NormalComplete" ); } break;
	case eAutoLotState_CleanOut: { strLotStatus = _T( "CleanOut" ); } break;
	default: { strLotStatus = _T( "Unknown" ); } break;
	}

	return strLotStatus;
}

CString CBaseAutoSeqManager::GetLotStateToString()
{
	return GetLotStateToString( GetCurrentLotState() );
}


int CBaseAutoSeqManager::OnLotCommand( eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam )
{
	switch( eLotCmd )
	{
	case eAutoLotCmd_Initial:
		{
			if( m_nAutoRunMainStep == eAutoLotState_Initialize )
				return ERR_NO_ERROR;

			if( m_nAutoRunMainStep == eAutoLotState_InitNeed )
			{
				ChangeMainState( eAutoLotState_Initialize );
				return ERR_NO_ERROR;
			}
		}break;	
	case eAutoLotCmd_Skip:
	case eAutoLotCmd_Pause:
	case eAutoLotCmd_AlarmPause:
		{
			if( m_nAutoRunMainStep == eAutoLotState_Initialize
				|| m_nAutoRunMainStep == eAutoLotState_InitNeed
				|| m_nAutoRunMainStep == eAutoLotState_Idle )
			{
				ChangeMainState( eAutoLotState_InitNeed );
				return ERR_NO_ERROR;
			}

			if( m_nAutoRunMainStep == eAutoLotState_Pause
				|| m_nAutoRunMainStep == eAutoLotState_AlarmStop )
				return ERR_NO_ERROR;			
			
			if( m_nAutoRunMainStep == eAutoLotState_Executing
					 || m_nAutoRunMainStep == eAutoLotState_Resume
					 || m_nAutoRunMainStep == eAutoLotState_CleanOut
					 || m_nAutoRunMainStep == eAutoLotState_NormalComplete )
			{
				//if( eLotCmd == eAutoLotCmd_AlarmPause )
				//	ChangeMainState( eAutoLotState_AlarmStop );
				//else
					ChangeMainState( eAutoLotState_Pause );
				return ERR_NO_ERROR;
			}
		}break;
	case eAutoLotCmd_Retry:
	case eAutoLotCmd_Resume:    // 장비의 Start 버튼
		{
			if( m_nAutoRunMainStep == eAutoLotState_Executing
				|| m_nAutoRunMainStep == eAutoLotState_Resume
				|| m_nAutoRunMainStep == eAutoLotState_CleanOut
				|| m_nAutoRunMainStep == eAutoLotState_NormalComplete )
			{
				return ERR_NO_ERROR;
			}

			if( m_nAutoRunMainStep == eAutoLotState_Pause
				|| m_nAutoRunMainStep == eAutoLotState_AlarmStop
				|| m_nAutoRunMainStep == eAutoLotState_Idle )
			{
				ChangeMainState( eAutoLotState_Resume );
				return ERR_NO_ERROR;
			}
		}break;
	case eAutoLotCmd_CleanOut:
		{
			if( m_nAutoRunMainStep == eAutoLotState_CleanOut
				|| m_nAutoRunMainStep == eAutoLotState_NormalComplete )
				return ERR_NO_ERROR;

			if( m_nAutoRunMainStep == eAutoLotState_Executing
				|| m_nAutoRunMainStep == eAutoLotState_Pause )
			{
				ChangeMainState( eAutoLotState_CleanOut );
				return ERR_NO_ERROR;
			}
		}break;
	}

	return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
}

CString CBaseAutoSeqManager::GetExcuteStateStr()
{
	CString strExcuteStsName;
	
	if( m_nAutoRunMainStep == eAutoLotState_Executing )
		m_mapExcuteState.Lookup( m_nAutoRunSubStep, strExcuteStsName );

	return strExcuteStsName;
}

int CBaseAutoSeqManager::GetExcuteState()
{
	if( m_nAutoRunMainStep != eAutoLotState_Executing )
		return -1;

	return m_nAutoRunSubStep;
}

void CBaseAutoSeqManager::AutoRunning()
{
	m_csAutoSeqCmdStatus.Lock();
	switch( m_nAutoRunMainStep )
	{
	case eAutoLotState_InitNeed: { AutoState_InitNeed(); m_nLast_Excuting_Step = 0;} break;
	case eAutoLotState_Initialize: { AutoState_Initialize(); } break;
	case eAutoLotState_Idle: { AutoState_Idle(); } break;
	case eAutoLotState_Executing: { AutoState_Execute(); } break;
	case eAutoLotState_Pause: { AutoState_Pause(); } break;	
	case eAutoLotState_Resume: { AutoState_Resume(); } break;
	case eAutoLotState_AlarmStop: { AutoState_AlarmStop(); } break;
	case eAutoLotState_NormalComplete: { AutoState_NormalComplete(); } break;
	case eAutoLotState_CleanOut: { AutoState_CleanOut(); } break;
	}
	m_csAutoSeqCmdStatus.Unlock();
}

void CBaseAutoSeqManager::ChangeMainStateByRspAlarm()
{
	switch( m_nAutoRunMainStep )
	{
		case eAutoLotState_InitNeed:
		case eAutoLotState_Initialize:
		case eAutoLotState_Idle:
			{
				ChangeMainState( eAutoLotState_InitNeed );
			}break;
		case eAutoLotState_Resume:
		case eAutoLotState_Executing:
		case eAutoLotState_CleanOut:
		case eAutoLotState_NormalComplete:
			{
				ChangeMainState( eAutoLotState_AlarmStop );
			}break;
	}
}