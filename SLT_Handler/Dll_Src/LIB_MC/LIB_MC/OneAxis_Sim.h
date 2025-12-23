#pragma once

#include "oneaxis.h"
#include "DEF_MOTION.h"
#include "Logger.h"
#include "TimerTickCheck.h"
#include "ConfigData.h"

class CMotionSimData
{
public:
	CMotionSimData()
	{
		dCurPos = 0.0;
		dCurSpeed = 0.0;
		nCurAcc    = 10;
		nCurDcc    = 10;
		dStop_Acc = 0.0;
		bSwLimitStopEnable = FALSE;
//		dInposRange = 10; // um
		dInposRange = (MOTOR_INPOSITION_um); // um
		bInpos = FALSE;
		bDone = FALSE;
		bServoOn = FALSE;
		bPowerOn = FALSE;
		bSen_Org = FALSE;
		bSen_PosiLimit = FALSE;
		bSen_NegaLimit = FALSE;
		bSequence = FALSE;
		bAmpFault = FALSE;
		bMotion = FALSE;
	};
	~CMotionSimData()
	{
	};

	bool GetBitSts(int nIdx)
	{
		bool bValue = FALSE;
		switch( nIdx )
		{
		case 0: bValue = bInpos;         break;
		case 1: bValue = bDone;          break;
		case 2: bValue = bServoOn;       break;
		case 3: bValue = bPowerOn;       break;
		case 4: bValue = bSen_Org;       break;
		case 5: bValue = bSen_PosiLimit; break;
		case 6: bValue = bSen_NegaLimit; break;
		case 7: bValue = bSequence;      break;
		case 8: bValue = bAmpFault;      break;
		case 9: bValue = bMotion;        break;
		}
		return bValue;
	}
	void SetBitSts(int nIdx, bool bValue)
	{
		switch( nIdx )
		{
		case 0: bInpos          = bValue; break;
		case 1: bDone           = bValue; break;
		case 2: bServoOn        = bValue; break;
		case 3: bPowerOn        = bValue; break;
		case 4: bSen_Org        = bValue; break;
		case 5: bSen_PosiLimit  = bValue; break;
		case 6: bSen_NegaLimit  = bValue; break;
		case 7: bSequence       = bValue; break;
		case 8: bAmpFault       = bValue; break;
		case 9: bMotion         = bValue; break;
		}
	}

	double dCurPos;         // um
	double dCurSpeed;       // um/sec
	int    nCurAcc;            // msec
	int    nCurDcc;            // msec
	double dStop_Acc;
	BOOL   bSwLimitStopEnable;
	double dInposRange;

	bool   bInpos;
	bool   bDone;
	bool   bServoOn;
	bool   bPowerOn;
	bool   bSen_Org;
	bool   bSen_PosiLimit;
	bool   bSen_NegaLimit;
	bool   bSequence;
	bool   bAmpFault;
	bool   bMotion;
};

class OneAxis_Sim :	public OneAxis
{
public:
	OneAxis_Sim(void);
	OneAxis_Sim(int num, double scale, int Zr_direction=1, int pulse_dir=1, int move_dir=1);
	~OneAxis_Sim(void);

	static ERR_CODE InitBoard();
	int			Move(std::vector<CPoint> vPicker, double move_pos, double speed, int accel, int dccel, int nBoardType = eBoardDir_Forward, BOOL bNoUseOffset = FALSE);
	int			Move(_eBbxmnpAxis eAxis, double move_pos, double speed, int accel, int dccel, int nBoardType = eBoardDir_Forward, BOOL bNoUseOffset = FALSE);

	ERR_CODE		Move(double a_dMove_pos, double a_dSpeed=0, int a_nAccel=0, int a_nDccel = 0, BOOL bApplyBacklash = FALSE, _eMotorIntertlock nInterlockType = _eMotorIntertlock::eMotorInterlock_Operation);	// 20190402 ihc.
	//ERR_CODE		SplineMove(std::vector<_stSpline> SplineMoveCmd);
	ERR_CODE		NoInterlock_Move( double _pos_um, double _spd_umps, int _acc, int _dec, BOOL bApplyBacklash = FALSE, BOOL bAutoRunning = FALSE, _eApplyBacklashCondition eApplyBacklashCondition = _eApplyBacklashCondition::eApplyBacklashCondition_Normal, _eMotorIntertlock nInterlockType = _eMotorIntertlock::eMotorInterlock_Operation);	// 20190405 ihc. backlash test 추가
	ERR_CODE     V_Move(double a_dSpeed, int a_nAccel);
	ERR_CODE     V_MoveStop();
	ERR_CODE		R_Move(double a_dMove_pos, double a_dSpeed=0, BOOL bPcdBacklashMode = FALSE);			// 20190402 ihc.
	ERR_CODE		JogMove( int nDir, double dSpeed_umps, int nAccDec, BOOL bPcdBacklashMode = FALSE );	// 20190402 ihc.

	ERR_CODE		Origin();
	ERR_CODE     OriginWhile();
	BOOL	IsOrgCompleted(ERR_CODE *pError);

	ERR_CODE		MotionDone();
	ERR_CODE     AxisDone();
	int			 AxisDone(std::vector<CPoint>& vPicker, std::vector<CPoint>& vErrPicker);
	ERR_CODE	ChkPremovePos();

	ERR_CODE     ServoOn();
	ERR_CODE     ServoOff();
	BOOL    CheckPowerOffStat();

	void    SetCommandPos(double posit);
	ERR_CODE  GetCommandPos(double &dPos);

	void    SetCurrentPos(double pos);
	ERR_CODE  GetCurrentPos(double &dPos);

	double  GetCurrentPos(double &dPos, _eBbxmnpAxis eAxis);

	int	 GetCurrenPosAvg(double &dPos);

	void    Set_disable(BOOL bDisable);
	bool		InSequence();
	bool		InMotion();
	bool		InPosition();
	bool		IsAxisDone();

	bool     IsAxisDoneOrFree();
	BOOL    GetServoStat();
	bool	IsServoOn();
	bool    IsHomeSensorOn();
	bool	IsHomeSensorOn(_eBbxmnpAxis eAxis);
	bool    IsPositiveSensorOn();
	bool    IsNegativeSensorOn();
	double  GetVelocity();
	int     GetAcc();
	int     GetDcc();
	double  GetSpeed();
	bool    IsAmpFault();
	double	GetErrorValue();

	ERR_CODE		SetStop();
	ERR_CODE		SetSlowStop();
	void    SetStopRate(double stop_dcc);
	bool    SetSWLimit(double dPosLimit, double dNegLimit, BOOL bDisable = FALSE);
	void    EnableSWLimit(BOOL bEnable);

	ERR_CODE     AlarmReset();

	void    MotorStrageCheck(ST_MOTORSTATUS* lpstMotorData);
	void    LogMtStrage(int nAxisNumPhy, double dPos=0.0, double dSpeed=0.0);
	BOOL    AmpFaultCheckFunction();
	void	SaveMotorMoveLog(double dTargetPos, double dMovePos, int nLogMode);	

	//동작 상태
	bool	IsPositionDone();	//위치 체크용
	bool	IsMotorStop();		//모터 동작 체크용
	int		GetAxisStatus();
	ERR_CODE		OriginStart();		//Switch 형식의 오리진 시작
	int		GetNoControlAxis();	//제어하고 있는 축수
	double	GetTargetPos(bool bMM=false);
	ERR_CODE		SetEmergencyStop();

// 	// 20190405 ihc. copy from MS100
// 	// Back Lash
// 	void SetApplyBacklash(bool bApplyBacklash);
// 	bool GetApplyBacklash();
// 	void SetBackLashOffset(double dBackLashOffset);
// 	double GetBacklashOffset(double dMovePos);
	virtual double	GetBacklashOffset(double dMovePos, double speed_umps);
	virtual double  GetBacklashOffset_TestPP_X1(double dMovePos, double dPPS);
	virtual double  GetBacklashOffset_TestPpX(double dMovePos, double dPPS);
	virtual double	GetBacklashDataAboutSpeed(double speed_umps);
	virtual double  GetBacklashParameterData(double dMoveDistance, double dspeed_umps);
	virtual void	SetSpeedBacklash(int nSpdIdx, double dValue);
	virtual void	SetParameterBacklash(int nDistanceIdx, int nSpdIdx, double dValue);
	virtual double	GetLastBacklashOffset() { return m_dMem_BacklashOffset; };

	// Pcd
	void SetScaleFactorOffset(double dOffset);
	double GetScaleFactorOffset();

	bool m_bApplyBacklash;
	double m_dBackLashOffset;
	double m_dPrevMovePos;
	double m_dPrevBackLashOffset;

	double  GetScaleFactor();

// simulation routine
public:
	static UINT ThreadProc(LPVOID lpVoid);
	void MotionSimulProc();
	
	void Run_Sim_Idle();
	void Run_Sim_Stop();
	void Run_Sim_EStop();
	void Run_Sim_Move();
	void Run_Sim_VMove();
	void Run_Sim_Alarm();
	void ChangeSimulStatus(int nNewState);

	CEvent m_evtTerminatedThread;
	BOOL m_bSopThread;
	int m_nMotionSimulState;
	int m_nMotorSimulStep;
	double m_dOrg_Func_Call_Time_msec;
	double m_dMove_Func_Call_Time_msec;

	double			m_dWork_Velocity, m_dWork_Velocity_Ori; // [um/sec]
	int				m_nWork_Acc, m_nWork_Acc_Ori;			// [msec]
	int				m_nWork_Dcc;							// [msec]

	CLogger m_MtLog;

	enum eMotionSimulState{
		eMoSimSt_Idle = 0,
		eMoSimSt_Stop,
		eMoSimSt_EStop,
		eMoSimSt_Move,
		eMoSimSt_VMove,
		eMoSimSt_Alarm,
	};

public:
	static ERR_CODE InitBoard(void* lpVoid);
	static ERR_CODE SplineMove(std::vector<_stSpline> SplineMoveCmd, int nAxisNumPhy1, int nAxisNumPhy2);

	CMotionSimData m_SimMotorState;
};

