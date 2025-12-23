// BbxmnpWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BbxmnpWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBbxmnpWnd

IMPLEMENT_DYNAMIC(CBbxmnpWnd, CWnd)

CBbxmnpWnd::CBbxmnpWnd()
{
	m_fnCallback = NULL;
	m_pOwnCWnd	 = NULL;
	g_pBbxmnpDrv = new CBbxmnpDrv;
}

CBbxmnpWnd::~CBbxmnpWnd()
{
	if (g_pBbxmnpDrv != nullptr)
	{
		delete g_pBbxmnpDrv;
		g_pBbxmnpDrv = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CBbxmnpWnd, CWnd)
END_MESSAGE_MAP()

int CBbxmnpWnd::Initialize(CWnd* pCwnd, char* szFilePath, FN_PARENT_CALLBACK fnAlarmCallBack )
{
	int nRet = 0;
	m_pOwnCWnd = pCwnd;
	m_fnCallback = fnAlarmCallBack;
	nRet = g_pBbxmnpDrv->Initialize(this, szFilePath, CBbxmnpWnd::cbOnAlarmReport);

	return nRet;
}

void CBbxmnpWnd::Finialize()
{
	g_pBbxmnpDrv->Finalize();
}

int CBbxmnpWnd::ReConnection()
{
	int nRet = 0;
	nRet = g_pBbxmnpDrv->Reconnection();
	return nRet;
}



int CBbxmnpWnd::GetNumOfBoard()
{
	return g_pBbxmnpDrv->GetNumOfBoard();
}

int CBbxmnpWnd::GetNumOfAxis()
{
	return g_pBbxmnpDrv->GetNumOfAxis();
}

int CBbxmnpWnd::GetBoardNumber( _eBbxmnpAxis eAxis )
{
	return g_pBbxmnpDrv->GetBoardNumber(eAxis);
}

int CBbxmnpWnd::SetCommSpeed( int nBoardNum, int nCommSpeed )
{
	return g_pBbxmnpDrv->SetCommSpeed(nBoardNum, nCommSpeed);
}

int CBbxmnpWnd::GetCommSpeed( int nBoardNum, int* pnCommSpeed )
{
	return g_pBbxmnpDrv->GetCommSpeed(nBoardNum, pnCommSpeed);
}

int CBbxmnpWnd::CyclicBegin( int nBoardNum )
{
	return g_pBbxmnpDrv->CyclicBegin(nBoardNum);
}

int CBbxmnpWnd::CyclicEnd( int nBoardNum )
{
	return g_pBbxmnpDrv->CyclicEnd(nBoardNum);
}

int CBbxmnpWnd::SysComm(int nBoardNum)
{
	return g_pBbxmnpDrv->SysComm(nBoardNum);
}

int CBbxmnpWnd::MotionParamLoadToFile()
{
	return g_pBbxmnpDrv->MotionParamLoadToFile();
}

int CBbxmnpWnd::MotionParamSaveToFile()
{
	return g_pBbxmnpDrv->MotionParamSaveToFile();
}

int CBbxmnpWnd::GetCommErrNum( int nBoardNum, int* pnErrNum )
{
	return g_pBbxmnpDrv->GetCommErrNum(nBoardNum, pnErrNum);
}

int CBbxmnpWnd::CommErrNumClear( int nBoardNum )
{
	return g_pBbxmnpDrv->CommErrNumClear(nBoardNum);
}

int CBbxmnpWnd::GetCyclicSpeed( int nBoardNum, int* pnTime )
{
	return g_pBbxmnpDrv->GetCyclicSpeed(nBoardNum, pnTime);
}

int CBbxmnpWnd::ErrGetCode( int nBoardNum, int* pnCode )
{
	return g_pBbxmnpDrv->ErrGetCode(nBoardNum, pnCode);
}

int CBbxmnpWnd::ErrGetString( int nBoardNum, int nCode, char* szString )
{
	return g_pBbxmnpDrv->ErrGetString(nBoardNum, nCode, szString);
}

int CBbxmnpWnd::AxSetServoOn( _eBbxmnpAxis eAxis, int nState )
{
	return g_pBbxmnpDrv->AxSetServoOn(eAxis, nState);
}

int CBbxmnpWnd::AxGetServoOn( _eBbxmnpAxis eAxis, int* pnState )
{
	return g_pBbxmnpDrv->AxGetServoOn(eAxis, pnState);
}

int CBbxmnpWnd::AxGetServoInp( _eBbxmnpAxis eAxis, int* pnInp )
{
	return g_pBbxmnpDrv->AxGetServoInp(eAxis, pnInp);
}

int CBbxmnpWnd::AxSetServoReset( _eBbxmnpAxis eAxis, int nState )
{
	return g_pBbxmnpDrv->AxSetServoReset(eAxis, nState);
}

int CBbxmnpWnd::AxGetServoReset( _eBbxmnpAxis eAxis, int* pnState )
{
	return g_pBbxmnpDrv->AxGetServoReset(eAxis, pnState);
}

int CBbxmnpWnd::AxSetServoInpLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	return g_pBbxmnpDrv->AxSetServoInpLevel(eAxis, nLevel);
}

int CBbxmnpWnd::AxGetServoInpLevel( _eBbxmnpAxis eAxis, int* pnLevel )
{
	return g_pBbxmnpDrv->AxGetServoInpLevel(eAxis, pnLevel);
}

int CBbxmnpWnd::AxSetServoInpEnable( _eBbxmnpAxis eAxis, int nEnable )
{
	return g_pBbxmnpDrv->AxSetServoInpEnable(eAxis, nEnable);
}

int CBbxmnpWnd::AxGetServoInpEnable( _eBbxmnpAxis eAxis, int* pnEnable )
{
	return g_pBbxmnpDrv->AxGetServoInpEnable(eAxis, pnEnable);
}

int CBbxmnpWnd::AxSetServoAlarmLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	return g_pBbxmnpDrv->AxSetServoAlarmLevel(eAxis, nLevel);
}

int CBbxmnpWnd::AxGetServoAlarmLevel( _eBbxmnpAxis eAxis, int* pnLevel )
{
	return g_pBbxmnpDrv->AxGetServoAlarmLevel(eAxis, pnLevel);
}

int CBbxmnpWnd::AxSetServoAlarmAction( _eBbxmnpAxis eAxis, int nAction )
{
	return g_pBbxmnpDrv->AxSetServoAlarmAction(eAxis, nAction);
}

int CBbxmnpWnd::AxGetServoAlarmAction( _eBbxmnpAxis eAxis, int* pnAction )
{
	return g_pBbxmnpDrv->AxGetServoAlarmAction(eAxis, pnAction);
}

int CBbxmnpWnd::AxSetOrgLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	return g_pBbxmnpDrv->AxSetOrgLevel(eAxis, nLevel);
}

int CBbxmnpWnd::AxGetOrgLevel( _eBbxmnpAxis eAxis, int* pnLevel )
{
	return g_pBbxmnpDrv->AxGetOrgLevel(eAxis, pnLevel);
}

int CBbxmnpWnd::AxSetSoftLimitPos( _eBbxmnpAxis eAxis, double dLimitP, double dLimitN )
{
	return g_pBbxmnpDrv->AxSetSoftLimitPos(eAxis, dLimitP, dLimitN);
}

int CBbxmnpWnd::AxGetSoftLimitPos( _eBbxmnpAxis eAxis, double* pdLimitP, double* pdLimitN )
{
	return g_pBbxmnpDrv->AxGetSoftLimitPos(eAxis, pdLimitP, pdLimitN);
}

int CBbxmnpWnd::AxSetSoftLimitAction( _eBbxmnpAxis eAxis, int nAction )
{
	return g_pBbxmnpDrv->AxSetSoftLimitAction(eAxis, nAction);
}

int CBbxmnpWnd::AxGetSoftLimitAction( _eBbxmnpAxis eAxis, int* pnAction )
{
	return g_pBbxmnpDrv->AxGetSoftLimitAction(eAxis, pnAction);
}

int CBbxmnpWnd::AxSetSoftLimitEnable( _eBbxmnpAxis eAxis, int nEnable )
{
	return g_pBbxmnpDrv->AxSetSoftLimitEnable(eAxis, nEnable);
}

int CBbxmnpWnd::AxGetSoftLimitEnable( _eBbxmnpAxis eAxis, int* pnEnable )
{
	return g_pBbxmnpDrv->AxGetSoftLimitEnable(eAxis, pnEnable);
}

int CBbxmnpWnd::AxSetPulseType( _eBbxmnpAxis eAxis, int nType )
{
	return g_pBbxmnpDrv->AxSetPulseType(eAxis, nType);
}

int CBbxmnpWnd::AxGetPulseType( _eBbxmnpAxis eAxis, int* pnType )
{
	return g_pBbxmnpDrv->AxGetPulseType(eAxis, pnType);
}

int CBbxmnpWnd::AxSetEncType( _eBbxmnpAxis eAxis, int nType )
{
	return g_pBbxmnpDrv->AxSetEncType(eAxis, nType);
}

int CBbxmnpWnd::AxGetEncType( _eBbxmnpAxis eAxis, int* pnType )
{
	return g_pBbxmnpDrv->AxGetEncType(eAxis, pnType);
}

int CBbxmnpWnd::AxSetEncDir( _eBbxmnpAxis eAxis, int nDir )
{
	return g_pBbxmnpDrv->AxSetEncDir(eAxis, nDir);
}

int CBbxmnpWnd::AxGetEncDir( _eBbxmnpAxis eAxis, int* pnDir )
{
	return g_pBbxmnpDrv->AxGetEncDir(eAxis, pnDir);
}

int CBbxmnpWnd::AxSetEncCount( _eBbxmnpAxis eAxis, int nCount )
{
	return g_pBbxmnpDrv->AxSetEncCount(eAxis, nCount);
}

int CBbxmnpWnd::AxGetEncCount( _eBbxmnpAxis eAxis, int* pnCount )
{
	return g_pBbxmnpDrv->AxGetEncCount(eAxis, pnCount);
}

int CBbxmnpWnd::AxSetMaxVel( _eBbxmnpAxis eAxis, double dVel )
{
	return g_pBbxmnpDrv->AxSetMaxVel(eAxis, dVel);
}

int CBbxmnpWnd::AxGetMaxVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return g_pBbxmnpDrv->AxGetMaxVel(eAxis, pdVel);
}

int CBbxmnpWnd::AxSetInitJogVel( _eBbxmnpAxis eAxis, double dVel )
{
	return g_pBbxmnpDrv->AxSetInitJogVel(eAxis, dVel);
}

int CBbxmnpWnd::AxGetInitJogVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return g_pBbxmnpDrv->AxGetInitJogVel(eAxis, pdVel);
}

int CBbxmnpWnd::AxSetInitVel( _eBbxmnpAxis eAxis, double dVel )
{
	return g_pBbxmnpDrv->AxSetInitVel(eAxis, dVel);
}

int CBbxmnpWnd::AxGetInitVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return g_pBbxmnpDrv->AxGetInitVel(eAxis, pdVel);
}

int CBbxmnpWnd::AxSetJogVelProf( _eBbxmnpAxis eAxis, int nProfileType, double dVel, double dAccTime )
{
	return g_pBbxmnpDrv->AxSetJogVelProf(eAxis, nProfileType, dVel, dAccTime);
}

int CBbxmnpWnd::AxGetJogVelProf( _eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdAccTime )
{
	return g_pBbxmnpDrv->AxGetJogVelProf(eAxis, pnProfileType, pdVel, pdAccTime);
}

int CBbxmnpWnd::AxSetVelProf( _eBbxmnpAxis eAxis, int nProfileType, double dVel, double dAccTime, double dDecTime )
{
	return g_pBbxmnpDrv->AxSetVelProf(eAxis, nProfileType, dVel, dAccTime, dDecTime);
}

int CBbxmnpWnd::AxGetVelProf( _eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdAccTime, double* pdDecTime )
{
	return g_pBbxmnpDrv->AxGetVelProf(eAxis, pnProfileType, pdVel, pdAccTime, pdDecTime);
}

int CBbxmnpWnd::AxSetDecelType( _eBbxmnpAxis eAxis, int nType )
{
	return g_pBbxmnpDrv->AxSetDecelType(eAxis, nType);
}

int CBbxmnpWnd::AxGetDecelType( _eBbxmnpAxis eAxis, int* pnType )
{
	return g_pBbxmnpDrv->AxGetDecelType(eAxis, pnType);
}

int CBbxmnpWnd::AxSetFilterEnable( _eBbxmnpAxis eAxis, int nEnable )
{
	return g_pBbxmnpDrv->AxSetFilterEnable(eAxis, nEnable);
}

int CBbxmnpWnd::AxGetFilterEnable( _eBbxmnpAxis eAxis, int* pnEnable )
{
	return g_pBbxmnpDrv->AxGetFilterEnable(eAxis, pnEnable);
}

int CBbxmnpWnd::AxHomeSetResetPos( _eBbxmnpAxis eAxis, int nResetPos )
{
	return g_pBbxmnpDrv->AxHomeSetResetPos(eAxis, nResetPos);
}

int CBbxmnpWnd::AxHomeGetResetPos( _eBbxmnpAxis eAxis, int* pnResetPos )
{
	return g_pBbxmnpDrv->AxHomeGetResetPos(eAxis, pnResetPos);
}

int CBbxmnpWnd::AxHomeSetType( _eBbxmnpAxis eAxis, int nType )
{
	return g_pBbxmnpDrv->AxHomeSetType(eAxis, nType);
}

int CBbxmnpWnd::AxHomeGetType( _eBbxmnpAxis eAxis, int* pnType )
{
	return g_pBbxmnpDrv->AxHomeGetType(eAxis, pnType);
}

int CBbxmnpWnd::AxHomeSetDir( _eBbxmnpAxis eAxis, int nDir )
{
	return g_pBbxmnpDrv->AxHomeSetDir(eAxis, nDir);
}

int CBbxmnpWnd::AxHomeGetDir( _eBbxmnpAxis eAxis, int* pnDir )
{
	return g_pBbxmnpDrv->AxHomeGetDir(eAxis, pnDir);
}

int CBbxmnpWnd::AxHomeSetEscapePul( _eBbxmnpAxis eAxis, double dEscape )
{
	return g_pBbxmnpDrv->AxHomeSetEscapePul(eAxis, dEscape);
}

int CBbxmnpWnd::AxHomeGetEscapePul( _eBbxmnpAxis eAxis, double* pdEscape )
{
	return g_pBbxmnpDrv->AxHomeGetEscapePul(eAxis, pdEscape);
}

int CBbxmnpWnd::AxHomeSetShiftDist( _eBbxmnpAxis eAxis, double dShift )
{
	return g_pBbxmnpDrv->AxHomeSetShiftDist(eAxis, dShift);
}

int CBbxmnpWnd::AxHomeGetShiftDist( _eBbxmnpAxis eAxis, double* pdShift )
{
	return g_pBbxmnpDrv->AxHomeGetShiftDist(eAxis, pdShift);
}

int CBbxmnpWnd::AxHomeSetInitVel( _eBbxmnpAxis eAxis, double dVel )
{
	return g_pBbxmnpDrv->AxHomeSetInitVel(eAxis, dVel);
}

int CBbxmnpWnd::AxHomeGetInitVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return g_pBbxmnpDrv->AxHomeGetInitVel(eAxis, pdVel);
}

int CBbxmnpWnd::AxHomeSetVelProf( _eBbxmnpAxis eAxis, int nProfileType, double dVel, double dRevVel, double dAccTime )
{
	return g_pBbxmnpDrv->AxHomeSetVelProf(eAxis, nProfileType, dVel, dRevVel, dAccTime);
}

int CBbxmnpWnd::AxHomeGetVelProf( _eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdRevVel, double* pdAccTime )
{
	return g_pBbxmnpDrv->AxHomeGetVelProf(eAxis, pnProfileType, pdVel, pdRevVel, pdAccTime);
}

int CBbxmnpWnd::AxHomeMove( _eBbxmnpAxis eAxis )
{
	return g_pBbxmnpDrv->AxHomeMove(eAxis);
}

int CBbxmnpWnd::MxHomeMove( int nAxisCnt, int naAxis[] )
{
	return g_pBbxmnpDrv->MxHomeMove(nAxisCnt, naAxis);
}

int CBbxmnpWnd::AxHomeCheckDone( _eBbxmnpAxis eAxis, int* pnDone )
{
	return g_pBbxmnpDrv->AxHomeCheckDone(eAxis, pnDone);
}

int CBbxmnpWnd::AxHomeSetStatus( _eBbxmnpAxis eAxis, int nStatus )
{
	return g_pBbxmnpDrv->AxHomeSetStatus(eAxis, nStatus);
}

int CBbxmnpWnd::AxHomeGetStatus( _eBbxmnpAxis eAxis, int* pnStatus )
{
	return g_pBbxmnpDrv->AxHomeGetStatus(eAxis, pnStatus);
}

int CBbxmnpWnd::AxJogMove( _eBbxmnpAxis eAxis, int nDir )
{
	return g_pBbxmnpDrv->AxJogMove(eAxis, nDir);
}

int CBbxmnpWnd::AxPosMove( _eBbxmnpAxis eAxis, int nAbsMode, double dPos )
{
	return g_pBbxmnpDrv->AxPosMove(eAxis, nAbsMode, dPos);
}

int CBbxmnpWnd::AxCheckDone( _eBbxmnpAxis eAxis, int* pnDone )
{
	return g_pBbxmnpDrv->AxCheckDone(eAxis, pnDone);
}

int CBbxmnpWnd::AxStop( _eBbxmnpAxis eAxis )
{
	return g_pBbxmnpDrv->AxStop(eAxis);
}

int CBbxmnpWnd::AxEStop( _eBbxmnpAxis eAxis )
{
	return g_pBbxmnpDrv->AxEStop(eAxis);
}

int CBbxmnpWnd::MxJogMove( int nAxisCnt, int naAxis[], int naDir[] )
{
	return g_pBbxmnpDrv->MxJogMove(nAxisCnt, naAxis, naDir);
}

int CBbxmnpWnd::MxPosMove( int nAxisCnt, int nAbsMode, int naAxis[], double faDist[] )
{
	return g_pBbxmnpDrv->MxPosMove(nAxisCnt, nAbsMode, naAxis, faDist);
}

int CBbxmnpWnd::MxCheckDone( int nAxisCnt, int naAxis[], int* pnDone )
{
	return g_pBbxmnpDrv->MxCheckDone(nAxisCnt, naAxis, pnDone);
}

int CBbxmnpWnd::MxStop( int nAxisCnt, int naAxis[] )
{
	return g_pBbxmnpDrv->MxStop(nAxisCnt, naAxis);
}

int CBbxmnpWnd::MxEStop( int nAxisCnt, int naAxis[] )
{
	return g_pBbxmnpDrv->MxEStop(nAxisCnt, naAxis);
}

int CBbxmnpWnd::AxGetMechanical( _eBbxmnpAxis eAxis, unsigned int* pnMechanical )
{
	return g_pBbxmnpDrv->AxGetMechanical(eAxis, pnMechanical);
}

int CBbxmnpWnd::AxGetMotion( _eBbxmnpAxis eAxis, unsigned int* pnMotion )
{
	return g_pBbxmnpDrv->AxGetMotion(eAxis, pnMotion);
}

int CBbxmnpWnd::AxGetErrStatus( _eBbxmnpAxis eAxis, unsigned int* pnErrStatus )
{
	return g_pBbxmnpDrv->AxGetErrStatus(eAxis, pnErrStatus);
}

int CBbxmnpWnd::AxGetCmdVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return g_pBbxmnpDrv->AxGetCmdVel(eAxis, pdVel);
}

int CBbxmnpWnd::AxSetCmdPos( _eBbxmnpAxis eAxis, double dPos )
{
	return g_pBbxmnpDrv->AxSetCmdPos(eAxis, dPos);
}

int CBbxmnpWnd::AxGetCmdPos( _eBbxmnpAxis eAxis, double* pdPos )
{
	return g_pBbxmnpDrv->AxGetCmdPos(eAxis, pdPos);
}

int CBbxmnpWnd::AxSetActPos( _eBbxmnpAxis eAxis, double dPos )
{
	return g_pBbxmnpDrv->AxSetActPos(eAxis, dPos);
}

int CBbxmnpWnd::AxGetActPos( _eBbxmnpAxis eAxis, double* pdPos )
{
	return g_pBbxmnpDrv->AxGetActPos(eAxis, pdPos);
}

int CBbxmnpWnd::AxSetPosError( _eBbxmnpAxis eAxis, double dErrPos )
{
	return g_pBbxmnpDrv->AxSetPosError(eAxis, dErrPos);
}

int CBbxmnpWnd::AxGetPosError( _eBbxmnpAxis eAxis, double* pdErrPos )
{
	return g_pBbxmnpDrv->AxGetPosError(eAxis, pdErrPos);
}

int CBbxmnpWnd::AxSetSyncType( _eBbxmnpAxis eAxis, int nType )
{
	return g_pBbxmnpDrv->AxSetSyncType(eAxis, nType);
}

int CBbxmnpWnd::AxGetSyncType( _eBbxmnpAxis eAxis, int* pnType )
{
	return g_pBbxmnpDrv->AxGetSyncType(eAxis, pnType);
}

int CBbxmnpWnd::AxSetSyncAction( _eBbxmnpAxis eAxis, int nMaskAxes, int nCondition )
{
	return g_pBbxmnpDrv->AxSetSyncAction(eAxis, nMaskAxes, nCondition);
}

int CBbxmnpWnd::AxGetSyncAction( _eBbxmnpAxis eAxis, int nMaskAxes, int* pnCondition )
{
	return g_pBbxmnpDrv->AxGetSyncAction(eAxis, nMaskAxes, pnCondition);
}

/*=============================================================================
  DESCRIPT : motionnet board 통신 알람 상태 확인
  KEY WORD :  // By hyb [2018-01-17]
  ARGUMENT : pBoardConnectSts 상태를 가져올 data
  RETURN   : TRUE : 정상 FALSE : 비정상
  ==============================================================================*/
BOOL CBbxmnpWnd::GetBoardCommSts(int *pBoardConnectSts)
{
	BOOL bRetVAl = FALSE;	// 반환값 확인

	bRetVAl = g_pBbxmnpDrv->CheckCommunicationAlarm(pBoardConnectSts);

	return bRetVAl;
}

void CBbxmnpWnd::OnReceiveAlarm(WPARAM wParam, LPARAM lParam)
{
	m_fnCallback( m_pOwnCWnd, wParam, lParam );
}

void CBbxmnpWnd::cbOnAlarmReport(CWnd* pWnd, LPARAM lParam, WPARAM wParam)
{
	CBbxmnpWnd* pSelf = (CBbxmnpWnd*)pWnd;
	pSelf->OnReceiveAlarm(wParam, lParam);
}

int CBbxmnpWnd::AxGetHomeSensorStatus(int nCardNum, int nSlaveNum, int *nORG)
{
	return g_pBbxmnpDrv->AxGetHomeSensorStatus(nCardNum, nSlaveNum, nORG);
}
