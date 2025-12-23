#include "StdAfx.h"
#include "BbxmnpDrv.h"

CBbxmnpDrv::CBbxmnpDrv(void)
{
	m_fnAlarm		= NULL;
	m_pOwnCWnd		= NULL;
	ZeroMemory(m_nArrChkErrComBoard, sizeof(m_nArrChkErrComBoard));
}


CBbxmnpDrv::~CBbxmnpDrv(void)
{

}

int CBbxmnpDrv::Initialize( CWnd* pCwnd, char* szFilePath, FN_PARENT_CALLBACK fnAlarmCallBack )
{
	int nRet = 0;
	m_pOwnCWnd = pCwnd;
	memset( m_nArrChkErrComBoard, 0, sizeof( m_nArrChkErrComBoard ) );
	
	m_fnAlarm = fnAlarmCallBack;

	nRet = m_motionManager.Initialize(szFilePath);
	if (nRet != 0)
	{
		AfxMessageBox("Bbxmnp(Picker Motor) Motion Information Loading Fail...");
		return nRet;
	}

	int nBoardCnt = 0;
	nRet = nmiSysLoad(emAUTO, &nBoardCnt);
	
	if (nRet < 1)
	{
		AfxMessageBox("Bbxmnp(Picker Motor) Drive SysLoad Fail...");
		return nRet;
	}

	int nAxesCnt = 0, nCnt = 0;
	int nNumOfBoard = m_motionManager.GetNumOfBoard();
	int nBoardNum = m_motionManager[eBMAxis_LOAD_TRAY_Picker02].m_nBoardNum;
	ASSERT( nNumOfBoard > 0 );

	int  nFailCnt = 0;
	int  nRes = 0;
	//BOOL bReChk = FALSE;
	int  nChkLoop = 0;
	int  nTotalCnt = 0;
	for (int i = 0; i < nNumOfBoard; i++)
	{
		for( nChkLoop = 0; nChkLoop < 2; nChkLoop++ )
		{
			nFailCnt = 0;		
				
			// Set Communication speed(20Mbps)
			nRes = nmiSetCommSpeed(i, emCOMM_SPEED20);
			Sleep(1);
		
			if (nRes != TMC_RV_OK)
			{
				nFailCnt++;
			}

			// Start communication of Motion board system
			nRes = nmiSysComm(i);
			if (nRes != TMC_RV_OK)
			{
				nFailCnt++;
			}
			Sleep(1);
			// Start cyclic communication
			nRes = nmiCyclicBegin(i);
			if (nRes != TMC_RV_OK)
			{
				nFailCnt++;
			}
			Sleep(1);
			if( nFailCnt == 0 )	// board 문제 없음
			{
				break;
			}
		}	

		nTotalCnt += nFailCnt;
	}

	nmiConParamLoad();
// 	nmiDLogSetLevel(2);
// 	nmiDLogSetEnable(1);



	//nmiConParamSave();

	// Check Board (실제 Board에 연결된 Slave의 개수를 받아와서 사용할 축의 개수와 비교)
	for (int i = 0; i < nNumOfBoard; i++)
	{
		nCnt = 0;
		nmiGnGetAxesNumAll(i, &nCnt);
		Sleep(1);
		nAxesCnt += nCnt;
	}

	for (int i= 0; i < m_motionManager.GetNumOfAxes(); i++)	{
		AxSetPulseType(_eBbxmnpAxis(i), m_motionManager[_eBbxmnpAxis(i)].m_nPulseType);
		Sleep(1);
	}

	for (int i= 0; i < m_motionManager.GetNumOfAxes(); i++)	{
		AxSetEncCount(_eBbxmnpAxis(i), m_motionManager[_eBbxmnpAxis(i)].m_nEncoderExist);
		Sleep(1);
	}

	for (int i = 0; i < m_motionManager.GetNumOfAxes(); i++) {
		AxSetEncDir(_eBbxmnpAxis(i), 1);
		Sleep(1);
	}
	

	if (nAxesCnt != m_motionManager.GetNumOfAxes())
	{
		nmiSysUnload();
		AfxMessageBox("The system information does not match... Picker Motor ");
		return -10;
	}

	if (nTotalCnt > 0)
	{
		AfxMessageBox("Start Communication Fail Picker Motor!");
		nRet = -99;
	}

	CreateThread(0);
	CreateThread(1);

	return nRet;
}

void CBbxmnpDrv::Finalize()
{
	int nNumOfBoard = m_motionManager.GetNumOfBoard();

	//nmiConParamSave();

	for (int i = 0; i < nNumOfBoard; i++)
	{
		nmiCyclicEnd(i);

		//nmiReset(i);
	}

	nmiSysUnload();
}

int CBbxmnpDrv::Reconnection()
{
	int nRet = 0;
	int nBoardCnt = 0;
	nRet = nmiSysLoad(emAUTO, &nBoardCnt);

	if (nRet < 1)
	{
		return nRet;
	}

	int nAxesCnt = 0, nCnt = 0;
	int nNumOfBoard = m_motionManager.GetNumOfBoard();
	int nBoardNum = m_motionManager[eBMAxis_LOAD_TRAY_Picker02].m_nBoardNum;
	ASSERT(nNumOfBoard > 0);

	int  nFailCnt = 0;
	int  nRes = 0;
	//BOOL bReChk = FALSE;
	int  nChkLoop = 0;
	int  nTotalCnt = 0;
	for (int i = 0; i < nNumOfBoard; i++)
	{
		for (nChkLoop = 0; nChkLoop < 2; nChkLoop++)
		{
			nFailCnt = 0;

			// Set Communication speed(20Mbps)
			nRes = nmiSetCommSpeed(i, emCOMM_SPEED20);
			Sleep(1);

			if (nRes != TMC_RV_OK)
			{
				nFailCnt++;
			}

			// Start communication of Motion board system
			nRes = nmiSysComm(i);
			if (nRes != TMC_RV_OK)
			{
				nFailCnt++;
			}
			Sleep(1);
			// Start cyclic communication
			nRes = nmiCyclicBegin(i);
			if (nRes != TMC_RV_OK)
			{
				nFailCnt++;
			}
			Sleep(1);
			if (nFailCnt == 0)	// board 문제 없음
			{
				break;
			}
		}

		nTotalCnt += nFailCnt;
	}

	nmiConParamLoad();

	// Check Board (실제 Board에 연결된 Slave의 개수를 받아와서 사용할 축의 개수와 비교)
	for (int i = 0; i < nNumOfBoard; i++)
	{
		nCnt = 0;
		nmiGnGetAxesNumAll(i, &nCnt);
		Sleep(1);
		nAxesCnt += nCnt;
	}

	for (int i = 0; i < m_motionManager.GetNumOfAxes(); i++) {
		AxSetPulseType(_eBbxmnpAxis(i), m_motionManager[_eBbxmnpAxis(i)].m_nPulseType);
		Sleep(1);
	}

	for (int i = 0; i < m_motionManager.GetNumOfAxes(); i++) {
		AxSetEncCount(_eBbxmnpAxis(i), m_motionManager[_eBbxmnpAxis(i)].m_nEncoderExist);
		Sleep(1);
	}

	for (int i = 0; i < m_motionManager.GetNumOfAxes(); i++) {
		AxSetEncDir(_eBbxmnpAxis(i), 1);
		Sleep(1);
	}


	if (nAxesCnt != m_motionManager.GetNumOfAxes())
	{
		nmiSysUnload();
		AfxMessageBox("The system information does not match... Picker Motor ");
		return -10;
	}

	if (nTotalCnt > 0)
	{
		AfxMessageBox("Start Communication Fail Picker Motor!");
		nRet = -99;
	}

	return nRet;
}

int CBbxmnpDrv::GetNumOfBoard()
{
	return m_motionManager.GetNumOfBoard();
}

int CBbxmnpDrv::GetNumOfAxis()
{
	return m_motionManager.GetNumOfAxes();
}

int CBbxmnpDrv::GetBoardNumber( _eBbxmnpAxis eAxis )
{
	return m_motionManager[eAxis].m_nBoardNum;
}

int CBbxmnpDrv::SetCommSpeed( int nBoardNum, int nCommSpeed )
{
	int nRet = nmiSetCommSpeed(nBoardNum, nCommSpeed);
	return nRet;
}

int CBbxmnpDrv::GetCommSpeed( int nBoardNum, int* pnCommSpeed )
{
	int nRet = nmiGetCommSpeed(nBoardNum, pnCommSpeed);
	return nRet;
}

int CBbxmnpDrv::CyclicBegin( int nBoardNum )
{
	int nRet = nmiCyclicBegin(nBoardNum);
	return nRet;
}

int CBbxmnpDrv::CyclicEnd( int nBoardNum )
{
	int nRet = nmiCyclicEnd(nBoardNum);
	return nRet;
}

int CBbxmnpDrv::SysComm(int nBoardNum)
{
	int nRet = nmiSysComm(nBoardNum);
	return nRet;
}

int CBbxmnpDrv::MotionParamLoadToFile()
{
	int nRet = nmiConParamLoad();
	return nRet;
}

int CBbxmnpDrv::MotionParamSaveToFile()
{
	int nRet = nmiConParamSave();
	return nRet;
}

int CBbxmnpDrv::GetCommErrNum( int nBoardNum, int* pnErrNum )
{
	int nRet = nmiGetCommErrNum(nBoardNum, pnErrNum);
	return nRet;
}

int CBbxmnpDrv::CommErrNumClear( int nBoardNum )
{
	int nRet = nmiCommErrNumClear(nBoardNum);
	return nRet;
}

int CBbxmnpDrv::GetCyclicSpeed( int nBoardNum, int* pnTime )
{
	int nRet = nmiGetCyclicSpeed(nBoardNum, pnTime);
	return nRet;
}

int CBbxmnpDrv::ErrGetCode( int nBoardNum, int* pnCode )
{
	int nRet = nmiErrGetCode(nBoardNum, pnCode);
	return nRet;
}

int CBbxmnpDrv::ErrGetString( int nBoardNum, int nCode, char* szString )
{
	int nRet = nmiErrGetString(nBoardNum, nCode, szString);
	return nRet;
}

int CBbxmnpDrv::AxSetServoOn( _eBbxmnpAxis eAxis, int nState )
{
	int nRet = nmiAxSetServoOn(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nState);
	return nRet;
}

int CBbxmnpDrv::AxGetServoOn( _eBbxmnpAxis eAxis, int* pnState )
{
	int nRet = nmiAxGetServoOn(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnState);
	return nRet;
}

int CBbxmnpDrv::AxGetServoInp( _eBbxmnpAxis eAxis, int* pnInp )
{
	int nRet = nmiAxGetServoInp(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnInp);
	return nRet;
}

int CBbxmnpDrv::AxSetServoReset( _eBbxmnpAxis eAxis, int nState )
{
	int nRet = nmiAxSetServoReset(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nState);
	return nRet;
}

int CBbxmnpDrv::AxGetServoReset( _eBbxmnpAxis eAxis, int* pnState )
{
	int nRet = nmiAxGetServoReset(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnState);
	return nRet;
}

int CBbxmnpDrv::AxSetServoInpLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	int nRet = nmiAxSetServoInpLevel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nLevel);
	return nRet;
}

int CBbxmnpDrv::AxGetServoInpLevel( _eBbxmnpAxis eAxis, int* pnLevel )
{
	int nRet = nmiAxGetServoInpLevel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnLevel);
	return nRet;
}

int CBbxmnpDrv::AxSetServoInpEnable( _eBbxmnpAxis eAxis, int nEnable )
{
	int nRet = nmiAxSetServoInpEnable(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nEnable);
	return nRet;
}

int CBbxmnpDrv::AxGetServoInpEnable( _eBbxmnpAxis eAxis, int* pnEnable )
{
	int nRet = nmiAxGetServoInpEnable(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnEnable);
	return nRet;
}

int CBbxmnpDrv::AxSetServoAlarmLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	int nRet = nmiAxSetServoAlarmLevel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nLevel);
	return nRet;
}

int CBbxmnpDrv::AxGetServoAlarmLevel( _eBbxmnpAxis eAxis, int* pnLevel )
{
	int nRet = nmiAxGetServoAlarmLevel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnLevel);
	return nRet;
}

int CBbxmnpDrv::AxSetServoAlarmAction( _eBbxmnpAxis eAxis, int nAction )
{
	int nRet = nmiAxSetServoAlarmAction(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nAction);
	return nRet;
}

int CBbxmnpDrv::AxGetServoAlarmAction( _eBbxmnpAxis eAxis, int* pnAction )
{
	int nRet = nmiAxGetServoAlarmAction(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnAction);
	return nRet;
}

int CBbxmnpDrv::AxSetOrgLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	int nRet = nmiAxSetOrgLevel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nLevel);
	return nRet;
}

int CBbxmnpDrv::AxGetOrgLevel( _eBbxmnpAxis eAxis, int* pnLevel )
{
	int nRet = nmiAxGetOrgLevel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnLevel);
	return nRet;
}

int CBbxmnpDrv::AxSetSoftLimitPos( _eBbxmnpAxis eAxis, double dLimitP, double dLimitN )
{
	int nRet = nmiAxSetSoftLimitPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dLimitP, dLimitN);
	return nRet;
}

int CBbxmnpDrv::AxGetSoftLimitPos( _eBbxmnpAxis eAxis, double* pdLimitP, double* pdLimitN )
{
	int nRet = nmiAxGetSoftLimitPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdLimitP, pdLimitN);
	return nRet;
}

int CBbxmnpDrv::AxSetSoftLimitAction( _eBbxmnpAxis eAxis, int nAction )
{
	int nRet = nmiAxSetSoftLimitAction(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nAction);
	return nRet;
}

int CBbxmnpDrv::AxGetSoftLimitAction( _eBbxmnpAxis eAxis, int* pnAction )
{
	int nRet = nmiAxGetSoftLimitAction(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnAction);
	return nRet;
}

int CBbxmnpDrv::AxSetSoftLimitEnable( _eBbxmnpAxis eAxis, int nEnable )
{
	int nRet = nmiAxSetSoftLimitEnable(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nEnable);
	return nRet;
}

int CBbxmnpDrv::AxGetSoftLimitEnable( _eBbxmnpAxis eAxis, int* pnEnable )
{
	int nRet = nmiAxGetSoftLimitEnable(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnEnable);
	return nRet;
}

int CBbxmnpDrv::AxSetPulseType( _eBbxmnpAxis eAxis, int nType )
{
	int nRet = nmiAxSetPulseType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nType);
	return nRet;
}

int CBbxmnpDrv::AxGetPulseType( _eBbxmnpAxis eAxis, int* pnType )
{
	int nRet = nmiAxGetPulseType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnType);
	return nRet;
}

int CBbxmnpDrv::AxSetEncType( _eBbxmnpAxis eAxis, int nType )
{
	int nRet = nmiAxSetEncType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nType);
	return nRet;
}

int CBbxmnpDrv::AxGetEncType( _eBbxmnpAxis eAxis, int* pnType )
{
	int nRet = nmiAxGetEncType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnType);
	return nRet;
}

int CBbxmnpDrv::AxSetEncDir( _eBbxmnpAxis eAxis, int nDir )
{
	int nRet = nmiAxSetEncDir(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nDir);
	return nRet;
}

int CBbxmnpDrv::AxGetEncDir( _eBbxmnpAxis eAxis, int* pnDir )
{
	int nRet = nmiAxGetEncDir(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnDir);
	return nRet;
}

int CBbxmnpDrv::AxSetEncCount( _eBbxmnpAxis eAxis, int nCount )
{
	int nRet = nmiAxSetEncCount(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nCount);
	return nRet;
}

int CBbxmnpDrv::AxGetEncCount( _eBbxmnpAxis eAxis, int* pnCount )
{
	int nRet = nmiAxGetEncCount(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnCount);
	return nRet;
}

int CBbxmnpDrv::AxSetMaxVel( _eBbxmnpAxis eAxis, double dVel )
{
	int nRet = nmiAxSetMaxVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dVel);
	return nRet;
}

int CBbxmnpDrv::AxGetMaxVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	int nRet = nmiAxGetMaxVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdVel);
	return nRet;
}

int CBbxmnpDrv::AxSetInitJogVel( _eBbxmnpAxis eAxis, double dVel )
{
	int nRet = nmiAxSetInitJogVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dVel);
	return nRet;
}

int CBbxmnpDrv::AxGetInitJogVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	int nRet = nmiAxGetInitJogVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdVel);
	return nRet;
}

int CBbxmnpDrv::AxSetInitVel( _eBbxmnpAxis eAxis, double dVel )
{
	int nRet = nmiAxSetInitVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dVel);
	return nRet;
}

int CBbxmnpDrv::AxGetInitVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return nmiAxGetInitVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdVel);
}

int CBbxmnpDrv::AxSetJogVelProf( _eBbxmnpAxis eAxis, int nProfileType, double dVel, double dAccTime )
{
	return nmiAxSetJogVelProf(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nProfileType, dVel, dAccTime);
}

int CBbxmnpDrv::AxGetJogVelProf( _eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdAccTime )
{
	return nmiAxGetJogVelProf(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnProfileType, pdVel, pdAccTime);
}

int CBbxmnpDrv::AxSetVelProf( _eBbxmnpAxis eAxis, int nProfileType, double dVel, double dAccTime, double dDecTime )
{
	return nmiAxSetVelProf(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nProfileType, dVel, dAccTime, dDecTime);
}

int CBbxmnpDrv::AxGetVelProf( _eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdAccTime, double* pdDecTime )
{
	return nmiAxGetVelProf(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnProfileType, pdVel, pdAccTime, pdDecTime);
}

int CBbxmnpDrv::AxSetDecelType( _eBbxmnpAxis eAxis, int nType )
{
	return nmiAxSetDecelType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nType);
}

int CBbxmnpDrv::AxGetDecelType( _eBbxmnpAxis eAxis, int* pnType )
{
	return nmiAxGetDecelType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnType);
}

int CBbxmnpDrv::AxSetFilterEnable( _eBbxmnpAxis eAxis, int nEnable )
{
	return nmiAxSetFilterEnable(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nEnable);
}

int CBbxmnpDrv::AxGetFilterEnable( _eBbxmnpAxis eAxis, int* pnEnable )
{
	return nmiAxGetFilterEnable(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnEnable);
}

int CBbxmnpDrv::AxHomeSetResetPos( _eBbxmnpAxis eAxis, int nResetPos )
{
	return nmiAxHomeSetResetPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nResetPos);
}

int CBbxmnpDrv::AxHomeGetResetPos( _eBbxmnpAxis eAxis, int* pnResetPos )
{
	return nmiAxHomeGetResetPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnResetPos);
}

int CBbxmnpDrv::AxHomeSetType( _eBbxmnpAxis eAxis, int nType )
{
	return nmiAxHomeSetType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nType);
}

int CBbxmnpDrv::AxHomeGetType( _eBbxmnpAxis eAxis, int* pnType )
{
	return nmiAxHomeGetType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnType);
}

int CBbxmnpDrv::AxHomeSetDir( _eBbxmnpAxis eAxis, int nDir )
{
	return nmiAxHomeSetDir(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nDir);
}

int CBbxmnpDrv::AxHomeGetDir( _eBbxmnpAxis eAxis, int* pnDir )
{
	return nmiAxHomeGetDir(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnDir);
}

int CBbxmnpDrv::AxHomeSetEscapePul( _eBbxmnpAxis eAxis, double dEscape )
{
	return nmiAxHomeSetEscapePul(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dEscape);
}

int CBbxmnpDrv::AxHomeGetEscapePul( _eBbxmnpAxis eAxis, double* pdEscape )
{
	return nmiAxHomeGetEscapePul(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdEscape);
}

int CBbxmnpDrv::AxHomeSetShiftDist( _eBbxmnpAxis eAxis, double dShift )
{
	return nmiAxHomeSetShiftDist(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dShift);
}

int CBbxmnpDrv::AxHomeGetShiftDist( _eBbxmnpAxis eAxis, double* pdShift )
{
	return nmiAxHomeGetShiftDist(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdShift);
}

int CBbxmnpDrv::AxHomeSetInitVel( _eBbxmnpAxis eAxis, double dVel )
{
	return nmiAxHomeSetInitVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dVel);
}

int CBbxmnpDrv::AxHomeGetInitVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	return nmiAxHomeGetInitVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdVel);
}

int CBbxmnpDrv::AxHomeSetVelProf( _eBbxmnpAxis eAxis, int nProfileType, double dVel, double dRevVel, double dAccTime )
{
	return nmiAxHomeSetVelProf(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nProfileType, dVel, dRevVel, dAccTime);
}

int CBbxmnpDrv::AxHomeGetVelProf( _eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdRevVel, double* pdAccTime )
{
	return nmiAxHomeGetVelProf(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnProfileType, pdVel, pdRevVel, pdAccTime);
}

int CBbxmnpDrv::AxHomeMove( _eBbxmnpAxis eAxis )
{
	return nmiAxHomeMove(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum);
}

int CBbxmnpDrv::MxHomeMove( int nAxisCnt, int naAxis[] )
{
	_eBbxmnpAxis eAxis = _eBbxmnpAxis(naAxis[0]);
	int nBoardNum = m_motionManager[eAxis].m_nBoardNum;

	// 실제 Axis number로 변환
	int* naAxisNumber = new int[nAxisCnt];
	for (int i = 0; i < nAxisCnt; i++)
	{
		eAxis = _eBbxmnpAxis(naAxis[i]);
		naAxisNumber[i] = m_motionManager[eAxis].m_nAxisNum;
	}

	int nRet = nmiMxHomeMove(nBoardNum, nAxisCnt, naAxisNumber);

	delete [] naAxisNumber;
	return nRet;
}

int CBbxmnpDrv::AxHomeCheckDone( _eBbxmnpAxis eAxis, int* pnDone )
{
	return nmiAxHomeCheckDone(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnDone);
}

int CBbxmnpDrv::AxHomeSetStatus( _eBbxmnpAxis eAxis, int nStatus )
{
	return nmiAxHomeSetStatus(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nStatus);
}

int CBbxmnpDrv::AxHomeGetStatus( _eBbxmnpAxis eAxis, int* pnStatus )
{
	return nmiAxHomeGetStatus(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnStatus);
}

int CBbxmnpDrv::AxJogMove( _eBbxmnpAxis eAxis, int nDir )
{
	return nmiAxJogMove(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nDir);
}

int CBbxmnpDrv::AxPosMove( _eBbxmnpAxis eAxis, int nAbsMode, double dPos )
{
	return nmiAxPosMove(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nAbsMode, dPos);
}

int CBbxmnpDrv::AxCheckDone( _eBbxmnpAxis eAxis, int* pnDone )
{
	return nmiAxCheckDone(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnDone);
}

int CBbxmnpDrv::AxStop( _eBbxmnpAxis eAxis )
{
	return nmiAxStop(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum);
}

int CBbxmnpDrv::AxEStop( _eBbxmnpAxis eAxis )
{
	return nmiAxEStop(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum);
}

int CBbxmnpDrv::MxJogMove( int nAxisCnt, int naAxis[], int naDir[] )
{
	_eBbxmnpAxis eAxis = _eBbxmnpAxis(naAxis[0]);
	int nBoardNum = m_motionManager[eAxis].m_nBoardNum;

	// 실제 Axis number로 변환
	int* naAxisNumber = new int[nAxisCnt];
	for (int i = 0; i < nAxisCnt; i++)
	{
		eAxis = _eBbxmnpAxis(naAxis[i]);
		naAxisNumber[i] = m_motionManager[eAxis].m_nAxisNum;
	}

	int nRet = nmiMxJogMove(nBoardNum, nAxisCnt, naAxisNumber, naDir);

	delete [] naAxisNumber;
	return nRet;
}

int CBbxmnpDrv::MxPosMove( int nAxisCnt, int nAbsMode, int naAxis[], double faDist[] )
{
	_eBbxmnpAxis eAxis;
	int nBoardNum = 0;
	int naAxisCnt = 0;
	// 실제 Axis number로 변환
	int* naAxisNumber = new int[nAxisCnt];
	double* faPos = new double[nAxisCnt];
	for (int i = 0; i < MAX_PICKER_CNT_EACH_SLAVE; i++)
	{
		if(naAxis[i] >= 0)
		{
			eAxis = _eBbxmnpAxis(naAxis[i]);
			naAxisNumber[naAxisCnt] = m_motionManager[eAxis].m_nAxisNum;
			faPos[naAxisCnt] = faDist[i];
			nBoardNum = m_motionManager[eAxis].m_nBoardNum;
			naAxisCnt++;
		}	
	}

	ASSERT(naAxisCnt == nAxisCnt);

	int nRet = nmiMxPosMove(nBoardNum, nAxisCnt, nAbsMode, naAxisNumber, faPos);

	delete [] naAxisNumber;
	delete[] faPos;
	return nRet;
}

int CBbxmnpDrv::MxCheckDone( int nAxisCnt, int naAxis[], int* pnDone )
{
	_eBbxmnpAxis eAxis = _eBbxmnpAxis(naAxis[0]);
	int nBoardNum = m_motionManager[eAxis].m_nBoardNum;

	// 실제 Axis number로 변환
	int* naAxisNumber = new int[nAxisCnt];
	for (int i = 0; i < nAxisCnt; i++)
	{
		eAxis = _eBbxmnpAxis(naAxis[i]);
		naAxisNumber[i] = m_motionManager[eAxis].m_nAxisNum;
	}

	int nRet = nmiMxCheckDone(nBoardNum, nAxisCnt, naAxisNumber, pnDone);

	delete [] naAxisNumber;
	return nRet;
}

int CBbxmnpDrv::MxStop( int nAxisCnt, int naAxis[] )
{
	_eBbxmnpAxis eAxis = _eBbxmnpAxis(naAxis[0]);
	int nBoardNum = m_motionManager[eAxis].m_nBoardNum;

	// 실제 Axis number로 변환
	int* naAxisNumber = new int[nAxisCnt];
	for (int i = 0; i < nAxisCnt; i++)
	{
		eAxis = _eBbxmnpAxis(naAxis[i]);
		naAxisNumber[i] = m_motionManager[eAxis].m_nAxisNum;
	}

	int nRet = nmiMxStop(nBoardNum, nAxisCnt, naAxisNumber);

	delete [] naAxisNumber;
	return nRet;
}

int CBbxmnpDrv::MxEStop( int nAxisCnt, int naAxis[] )
{
	_eBbxmnpAxis eAxis = _eBbxmnpAxis(naAxis[0]);
	int nBoardNum = m_motionManager[eAxis].m_nBoardNum;

	// 실제 Axis number로 변환
	int* naAxisNumber = new int[nAxisCnt];
	for (int i = 0; i < nAxisCnt; i++)
	{
		eAxis = _eBbxmnpAxis(naAxis[i]);
		naAxisNumber[i] = m_motionManager[eAxis].m_nAxisNum;
	}

	int nRet = nmiMxEStop(nBoardNum, nAxisCnt, naAxisNumber);

	delete [] naAxisNumber;
	return nRet;
}

int CBbxmnpDrv::AxGetMechanical( _eBbxmnpAxis eAxis, unsigned int* pnMechanical )
{
	int nRet = nmiAxGetMechanical(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnMechanical);
	return nRet;
}

int CBbxmnpDrv::AxGetMotion( _eBbxmnpAxis eAxis, unsigned int* pnMotion )
{
	int nRet = nmiAxGetMotion(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnMotion);
	return nRet;
}

int CBbxmnpDrv::AxGetErrStatus( _eBbxmnpAxis eAxis, unsigned int* pnErrStatus )
{
	int nRet = nmiAxGetErrStatus(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnErrStatus);
	return nRet;
}

int CBbxmnpDrv::AxGetCmdVel( _eBbxmnpAxis eAxis, double* pdVel )
{
	int nRet = nmiAxGetCmdVel(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdVel);
	return nRet;
}

int CBbxmnpDrv::AxSetCmdPos( _eBbxmnpAxis eAxis, double dPos )
{
	int nRet = nmiAxSetCmdPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dPos);
	return nRet;
}

int CBbxmnpDrv::AxGetCmdPos( _eBbxmnpAxis eAxis, double* pdPos )
{
	int nRet = nmiAxGetCmdPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdPos);
	return nRet;
}

int CBbxmnpDrv::AxSetActPos( _eBbxmnpAxis eAxis, double dPos )
{
	int nRet = nmiAxSetActPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dPos);
	return nRet;
}

int CBbxmnpDrv::AxGetActPos( _eBbxmnpAxis eAxis, double* pdPos )
{
	int nRet = nmiAxGetActPos(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdPos);
	return nRet;
}

int CBbxmnpDrv::AxSetPosError( _eBbxmnpAxis eAxis, double dErrPos )
{
	int nRet = nmiAxSetPosError(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, dErrPos);
	return nRet;
}

int CBbxmnpDrv::AxGetPosError( _eBbxmnpAxis eAxis, double* pdErrPos )
{
	int nRet = nmiAxGetPosError(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pdErrPos);
	return nRet;
}

int CBbxmnpDrv::AxSetSyncType( _eBbxmnpAxis eAxis, int nType )
{
	int nRet = nmiAxSetSyncType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nType);
	return nRet;
}

int CBbxmnpDrv::AxGetSyncType( _eBbxmnpAxis eAxis, int* pnType )
{
	int nRet = nmiAxGetSyncType(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, pnType);
	return nRet;
}

int CBbxmnpDrv::AxSetSyncAction( _eBbxmnpAxis eAxis, int nMaskAxes, int nCondition )
{
	int nRet = nmiAxSetSyncAction(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nMaskAxes, nCondition);
	return nRet;
}

int CBbxmnpDrv::AxGetSyncAction( _eBbxmnpAxis eAxis, int nMaskAxes, int* pnCondition )
{
	int nRet = nmiAxGetSyncAction(m_motionManager[eAxis].m_nBoardNum, m_motionManager[eAxis].m_nAxisNum, nMaskAxes, pnCondition);
	return nRet;
}

void CBbxmnpDrv::ThreadProc1()
{
	int nMaxBoardCnt					 = 0;
	int nInxBoard						 = 0;
	unsigned int nLowFlag = 0;
	unsigned int nHeightFlag = 0;
	unsigned int nStatus = 0;
	int		 nGetErrNum				 = 0;
	BOOL bChkAlarm						 = FALSE;
	int nRet = 0;
	nMaxBoardCnt = m_motionManager.GetNumOfBoard(); // board 수량 확인 
	while( GetThreadAliveFlag(0) )
	{
		Sleep(50);
		bChkAlarm= FALSE;
		
		for( nInxBoard = 0; nInxBoard < nMaxBoardCnt; nInxBoard++ )
		{
			nLowFlag = 0;
			nRet = nmiGetCyclicErrFlagAll( nInxBoard, &nHeightFlag, &nLowFlag ); // 모든 슬래이브 모듈의 사이클릭 통신 에러 Flag 값을 반환한다.
			nRet = nmiGetCyclicStatus( nInxBoard, &nStatus); //사이클릭 통신 상태를 반환한다
			if( nLowFlag > 0 )	// 통신 이상
			{
				m_nArrChkErrComBoard[nInxBoard] = 1;
				bChkAlarm = TRUE;
			}
			else if( nStatus != 0x01) // 통신 이상
			{
				bChkAlarm = TRUE;
			}
			else
			{
				m_nArrChkErrComBoard[nInxBoard] = 0;
			}
		}
		if( bChkAlarm == TRUE )		
		{
			// 알람 코드 추가
			for( nInxBoard = 0; nInxBoard < nMaxBoardCnt; nInxBoard++ )
			{
				if( m_nArrChkErrComBoard[nInxBoard] == 1 )
				{
					nmiGetCommErrNum(nInxBoard, &nGetErrNum);

					nmiCommErrNumClear(nInxBoard);
					//Sleep(1);

// 					Sleep(1);
// 					nRet = nmiCyclicErrFlagClearAll( nInxBoard );
// 					Sleep(1);
// 					nRet = nmiReset(nInxBoard);
// 					Sleep(1);
// 					nRet = nmiSysComm(nInxBoard);
// 					Sleep(1);
// 					nRet = nmiCyclicBegin(nInxBoard);
// 					Sleep(1);
// 					nRet = nmiConParamLoad();
// 					Sleep(1);
				}
			}
		}
	}
}

void CBbxmnpDrv::ThreadProc2()
{
	int nMaxBoardCnt = 0;
	int nInxBoard = 0;
	int nHighFlag = 0, nLowFlag = 0;// high 
	unsigned int nChkErrCommunicationFlg = 0;

	while( GetThreadAliveFlag(1) )
	{
		Sleep(5);
		
		if( CheckCommunicationAlarm() == TRUE )
		{
			m_fnAlarm(m_pOwnCWnd, NULL, NULL );							// Error Report
		}		
	}
}

BOOL CBbxmnpDrv::CheckCommunicationAlarm( int *pBoardConnectSts )
{
	int			 nSearch			 =  0;
	const    int nBoardMaxCnt		 =  m_motionManager.GetNumOfBoard();
	unsigned int nSize				 =  sizeof( int ) * nBoardMaxCnt;
	BOOL		 bReturnAlarm		 =  0;

	for( nSearch = 0; nSearch < nBoardMaxCnt; nSearch++ )
	{
		if( m_nArrChkErrComBoard[nSearch] == 1 )
		{
			bReturnAlarm = TRUE;
		}
	}
	
	if( pBoardConnectSts != NULL )
	{
		memcpy_s( pBoardConnectSts, nSize, m_nArrChkErrComBoard, nSize );	// copy
	}	

	return bReturnAlarm;
}

void CBbxmnpDrv::SetParentCallbackAlarmReport( FN_PARENT_CALLBACK cbFunc )
{
	m_fnAlarm = cbFunc;
}

int CBbxmnpDrv::AxGetHomeSensorStatus(int nCardNum, int nSlaveNum, int *nORG)
{
	if( nORG == NULL )
		return -1;
	int nRet = nmiGnGetOrgStatus(nCardNum, nSlaveNum, nORG);
	return nRet;
}
