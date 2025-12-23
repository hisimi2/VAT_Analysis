#pragma once

#include "DefineBbxmnp.h"

// CBbxmnpWnd
class AFX_EXT_CLASS CBbxmnpWnd : public CWnd
{
	DECLARE_DYNAMIC(CBbxmnpWnd)

protected:
	DECLARE_MESSAGE_MAP()

public:
	CBbxmnpWnd();
	virtual ~CBbxmnpWnd();

	int Initialize( CWnd* pCwnd,  char* szFilePath, FN_PARENT_CALLBACK fnAlarmCallBack );
	void Finialize();
	int ReConnection();
	int GetNumOfBoard();
	int GetNumOfAxis();
	int GetBoardNumber(_eBbxmnpAxis eAxis);

	int SetCommSpeed(int nBoardNum, int nCommSpeed);
	int GetCommSpeed(int nBoardNum, int* pnCommSpeed);
	int CyclicBegin(int nBoardNum);
	int CyclicEnd(int nBoardNum);
	int SysComm(int nBoardNum);

	// 파일 경로 (C:\default_MN.prm)
	int MotionParamLoadToFile();	// Cyclic Beging을 한 후에 호출해야 함 
	int MotionParamSaveToFile();

	// General Function
	int GetCommErrNum(int nBoardNum, int* pnErrNum);
	int CommErrNumClear(int nBoardNum);
	int GetCyclicSpeed(int nBoardNum, int* pnTime);
	int ErrGetCode(int nBoardNum, int* pnCode);					// 메뉴얼에 없는 함수
	int ErrGetString(int nBoardNum, int nCode, char* szString);	// 메뉴얼에 없는 함수

	// Motion interface I/O Configure and Control Function
	int AxSetServoOn(_eBbxmnpAxis eAxis, int nState);
	int AxGetServoOn(_eBbxmnpAxis eAxis, int* pnState);
	int AxGetServoInp(_eBbxmnpAxis eAxis, int* pnInp);
	int AxSetServoReset(_eBbxmnpAxis eAxis, int nState);	// Servo Alarm Reset (nState=0:reset, 1:set)
	int AxGetServoReset(_eBbxmnpAxis eAxis, int* pnState);
	int AxSetServoInpLevel(_eBbxmnpAxis eAxis, int nLevel);
	int AxGetServoInpLevel(_eBbxmnpAxis eAxis, int* pnLevel);
	int AxSetServoInpEnable(_eBbxmnpAxis eAxis, int nEnable);
	int AxGetServoInpEnable(_eBbxmnpAxis eAxis, int* pnEnable);
	int AxSetServoAlarmLevel(_eBbxmnpAxis eAxis, int nLevel);
	int AxGetServoAlarmLevel(_eBbxmnpAxis eAxis, int* pnLevel);
	int AxSetServoAlarmAction(_eBbxmnpAxis eAxis, int nAction);
	int AxGetServoAlarmAction(_eBbxmnpAxis eAxis, int* pnAction);
	int AxSetOrgLevel(_eBbxmnpAxis eAxis, int nLevel);
	int AxGetOrgLevel(_eBbxmnpAxis eAxis, int* pnLevel);
	int AxSetSoftLimitPos(_eBbxmnpAxis eAxis, double dLimitP, double dLimitN);
	int	AxGetSoftLimitPos(_eBbxmnpAxis eAxis, double* pdLimitP, double* pdLimitN);
	//Action: The reacting method of soft limit
	//Action =0, INT
	//Action =1, Immediately stop
	//Action =2, slow down then stop
	//Action =3, Change Velocity
	int AxSetSoftLimitAction(_eBbxmnpAxis eAxis, int nAction);
	int AxGetSoftLimitAction(_eBbxmnpAxis eAxis, int* pnAction);
	int AxSetSoftLimitEnable(_eBbxmnpAxis eAxis, int nEnable);
	int AxGetSoftLimitEnable(_eBbxmnpAxis eAxis, int* pnEnable);
	//Command pulse signal output mode
	//0 OUT/DIR OUT Falling edge, DIR+ is high level
	//1 OUT/DIR OUT Rising edge,  DIR+ is high level
	//2 OUT/DIR OUT Falling edge, DIR+ is low level
	//3 OUT/DIR OUT Rising edge,  DIR+ is low level
	//4 CW/CCW Falling edge(Default)
	//5 CW/CCW Rising edge
	//6 CW/CCW Falling edge Inverse
	//7 CW/CCW Rising  edge Inverse
	//8 Two Phase Mode Pulse Phase lead than Dir
	//9 Two Phase Mode Dir   Phase lead than Pulse
	int AxSetPulseType(_eBbxmnpAxis eAxis, int nType);
	int AxGetPulseType(_eBbxmnpAxis eAxis, int* pnType);
	int AxSetEncType(_eBbxmnpAxis eAxis, int nType);
	int AxGetEncType(_eBbxmnpAxis eAxis, int* pnType);
	int AxSetEncDir(_eBbxmnpAxis eAxis, int nDir);
	int AxGetEncDir(_eBbxmnpAxis eAxis, int* pnDir);
	int AxSetEncCount(_eBbxmnpAxis eAxis, int nCount);
	int AxGetEncCount(_eBbxmnpAxis eAxis, int* pnCount);
	int AxSetMaxVel(_eBbxmnpAxis eAxis, double dVel);
	int AxGetMaxVel(_eBbxmnpAxis eAxis, double* pdVel);
	int AxSetInitJogVel(_eBbxmnpAxis eAxis, double dVel);
	int AxGetInitJogVel(_eBbxmnpAxis eAxis, double* pdVel);
	int AxSetInitVel(_eBbxmnpAxis eAxis, double dVel);
	int AxGetInitVel(_eBbxmnpAxis eAxis, double* pdVel);
	int AxSetJogVelProf(_eBbxmnpAxis eAxis, int nProfileType, double dVel, double dAccTime);
	int AxGetJogVelProf(_eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdAccTime);
	int AxSetVelProf(_eBbxmnpAxis eAxis, int nProfileType, double dVel, double dAccTime, double dDecTime);
	int AxGetVelProf(_eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdAccTime, double* pdDecTime);
	int AxSetDecelType(_eBbxmnpAxis eAxis, int nType);
	int AxGetDecelType(_eBbxmnpAxis eAxis, int* pnType);
	int AxSetFilterEnable(_eBbxmnpAxis eAxis, int nEnable);
	int AxGetFilterEnable(_eBbxmnpAxis eAxis, int* pnEnable);

	// Home-Return Function
	int AxHomeSetResetPos(_eBbxmnpAxis eAxis, int nResetPos);
	int AxHomeGetResetPos(_eBbxmnpAxis eAxis, int* pnResetPos);
	int AxHomeSetType(_eBbxmnpAxis eAxis, int nType);
	int AxHomeGetType(_eBbxmnpAxis eAxis, int* pnType);
	//home_dir = 0, CW direction (Default)
	//home_dir = 1, CCW direction
	int AxHomeSetDir(_eBbxmnpAxis eAxis, int nDir);
	int	AxHomeGetDir(_eBbxmnpAxis eAxis, int* pnDir);
	int AxHomeSetEscapePul(_eBbxmnpAxis eAxis, double dEscape);
	int AxHomeGetEscapePul(_eBbxmnpAxis eAxis, double* pdEscape);
	int AxHomeSetShiftDist(_eBbxmnpAxis eAxis, double dShift);
	int	AxHomeGetShiftDist(_eBbxmnpAxis eAxis, double* pdShift);
	int AxHomeSetInitVel(_eBbxmnpAxis eAxis, double dVel);
	int	AxHomeGetInitVel(_eBbxmnpAxis eAxis, double* pdVel);
	//Home search drive Vel setting one axis
	//nType=0 Constant
	//nType=1 T-Curve (Default)
	//nType=2 S-Curve
	//dVel    pps ( 0 ~ 6553500 )
	//dRevVel pps ( 0 ~ 6553500 )
	//0 <= Tacc < 60000  [ms]
	int AxHomeSetVelProf(_eBbxmnpAxis eAxis, int nProfileType, double dVel, double dRevVel, double dAccTime);
	int AxHomeGetVelProf(_eBbxmnpAxis eAxis, int* pnProfileType, double* pdVel, double* pdRevVel, double* pdAccTime);
	int AxHomeMove(_eBbxmnpAxis eAxis);
	int MxHomeMove(int nAxisCnt, int naAxis[]);
	int AxHomeCheckDone(_eBbxmnpAxis eAxis, int* pnDone);
	int AxHomeSetStatus(_eBbxmnpAxis eAxis, int nStatus);
	int AxHomeGetStatus(_eBbxmnpAxis eAxis, int* pnStatus);

	// Velocity mode And Single Axis Position Motion Configure
	int AxJogMove(_eBbxmnpAxis eAxis, int nDir);
	//nAbsMode = 0 Driving the relative coordinates
	//nAbsMode = 1 Driving the absolute coordinate
	int AxPosMove(_eBbxmnpAxis eAxis, int nAbsMode, double dPos);
	int AxCheckDone(_eBbxmnpAxis eAxis, int* pnDone);
	int AxStop(_eBbxmnpAxis eAxis);
	int AxEStop(_eBbxmnpAxis eAxis);
	// Velocity mode And Multi Axis Point to Point Motion Configure
	int MxJogMove(int nAxisCnt, int naAxis[], int naDir[]);
	int MxPosMove(int nAxisCnt, int nAbsMode, int naAxis[], double faDist[]);
	int MxCheckDone(int nAxisCnt, int naAxis[], int* pnDone);
	int MxStop(int nAxisCnt, int naAxis[]);
	int MxEStop(int nAxisCnt, int naAxis[]);
	// Motion I/O Monitoring FUNCTIONS
	int AxGetMechanical(_eBbxmnpAxis eAxis, unsigned int* pnMechanical);
	int AxGetMotion(_eBbxmnpAxis eAxis, unsigned int* pnMotion);
	int AxGetErrStatus(_eBbxmnpAxis eAxis, unsigned int* pnErrStatus);
	int AxGetCmdVel(_eBbxmnpAxis eAxis, double* pdVel);
	int AxSetCmdPos(_eBbxmnpAxis eAxis, double dPos);
	int	AxGetCmdPos(_eBbxmnpAxis eAxis, double* pdPos);
	int AxSetActPos(_eBbxmnpAxis eAxis, double dPos);
	int	AxGetActPos(_eBbxmnpAxis eAxis, double* pdPos);
	int AxSetPosError(_eBbxmnpAxis eAxis, double dErrPos);
	int	AxGetPosError(_eBbxmnpAxis eAxis, double* pdErrPos);
	int AxSetSyncType(_eBbxmnpAxis eAxis, int nType);
	int AxGetSyncType(_eBbxmnpAxis eAxis, int* pnType);
	int AxSetSyncAction(_eBbxmnpAxis eAxis, int nMaskAxes, int nCondition);
	int AxGetSyncAction(_eBbxmnpAxis eAxis, int nMaskAxes, int* pnCondition);
	void OnReceiveAlarm(WPARAM wParam, LPARAM lParam);
	BOOL GetBoardCommSts(int *pBoardConnectSts);

	int AxGetHomeSensorStatus(int nCardNum, int nSlaveNum, int *nORG);

	FN_PARENT_CALLBACK m_fnCallback;
	CWnd*			   m_pOwnCWnd;
private:
	static void cbOnAlarmReport(CWnd* pWnd, LPARAM lParam, WPARAM wParam);
	
};


