#pragma once

#include "OneAxis.h"
#include "DEF_MOTION.h"
#include "Logger.h"
#include "TimerTickCheck.h"
#include "ConfigData.h"
#include "DEF_ERROR_CODE.h"
#include <iostream>
#include <cstring>
using namespace std;

//#define MAX_MMC_AXIS_ONE_BD			(8)
//#define MMC_MULTIPLICATION          (4.0) // mmc 4체배 임.
//#define MMC_ACC_TIME_UNIT			(2)	  // half type mmc board(default) acc/dec time 1은 2 msec에 해당 함.

// Hardware Limit Switch 감지 시의 감속시간(sec). 0.1의 경우 0.1초 내로 감속을 끝낸다.
#define	DEF_SOFTSERVO_LIMIT_SWITCH_HIT_DECEL_TIME_SEC			0.1

#define ACC_INTERLOCK_MIN			(50)  // [msec], 최소 이값 이상이어야 함.
#define DEC_INTERLOCK_MIN			(50)  // [msec], 최소 이값 이상이어야 함.

#define DEF_SOFTSERVO_MASTER_ID		(1)

#define DEF_ZAXIS_PIKCER_CNT		(6)
#define DEF_IO_MODULE_CNT			(2)
// struct ST_MOTION_INIT_PARAM
// {
// 	double dInPosWidth;
// 	int nPulsePerRound;
// 	int nMoveDirection;
// 	int nSlaveNumber;
// };

class AFX_EXT_CLASS OneAxis_SoftServoEtherCAT : public OneAxis
{
public:
	bool ApplyConfiguration(eAxisServoConfiguration Configuration, BOOL bUseStartEngine = FALSE);
	bool ApplyPlviModuleConfigration(bool bSet);
	int  SetPlviCFG_REG(bool bUse/*0=dis,1=enable*/, double dTrigerDistence_um=0.0/*start-end dist*/, double dStartOffset_um = 0.0/*현위치에서 빼야할 거리*/, int nImageCount = 1800/*촬영 이미지 횟수*/);
	int GetTorque(BOOL bMaxTorque, double &dPesentage);

	int  GetPlviTriggerCount();

	static int  GetOnlineAxisCount();

	static bool InitBoard(std::map<int, ST_MOTION_INIT_PARAM>& AxisMap);
	static bool Close();
	static bool StartCommunication();

	double GetReadActualTorque_PDO(int nAxisNo);
	int ReadActualPeakTorqueData(int nAxisNo, _eTorqueType eType);
	int ConvertToValidData_Torque(unsigned char Data[], int nSizeArr, _eTorqueType eType);
	int binaryToDecimal(string strValue);
	string decimalToBinary(int Navlue);

	static void OneAxis_SoftServoEtherCAT::Motion_Error_Check(void);
	static UINT SoftServoEtherCAT_Error_Polling(LPVOID lpVoid);
	static void OneAxis_SoftServoEtherCAT::ComThreadCtrl(BOOL bRunStop);
	void		ClearOrgFlag();
	static BOOL ChkCommunication();
	static int m_nConnectSuccess;
	CLogger m_MtLog;
	
public:
	OneAxis_SoftServoEtherCAT(void);
	~OneAxis_SoftServoEtherCAT(void);

	int		SetHomeLevel(BOOL bStatus);
	double	IsVel(){return m_dCmdVelocity;};
	int		IsAcc(){return m_nCmdAcc;};
	bool		InSequence();
	bool		InMotion();	
	bool		InPosition();
	bool		IsAxisDone();

	double	GetVelocity();
	int		GetAcc();
	int		GetDcc();
	bool	IsAmpFault();
	double	GetErrorValue();

	void	SetCurrentPos(double pos);

	//////////////////////////////////////////////////////////////////////////

	/*=============================================================================
	RETURN   : 
	1         = 해당 축 사용 안함 or Board open fail
	dCalcPos  = 위치의 Encoder 값	[um]
	==============================================================================*/
//	ERR_CODE	GetCurrentPos(double &dPos);
	int		GetCurrentPos(double &dPos);
	ERR_CODE	GetCommandPos(double &dPos);
	void	SetCommandPos(double posit);
	void	SetErrLimit(double pos);

	void	FramesClear();
	ERR_CODE		ClearStatus();

	ERR_CODE		Move(double a_dPos_um, double speed_umps=0, int accel=0, int dccel = 0, BOOL bApplyBacklash = FALSE, _eMotorIntertlock nInterlockType = _eMotorIntertlock::eMotorInterlock_Operation/*, BOOL bAutoRunning = FALSE*/);	// ihc. bPcdBacklashMode, bIsBacklashDataTest 추가
	ERR_CODE		NoInterlock_Move( double _pos_um, double _spd_umps, int _acc, int _dec, BOOL bAutoRunning = FALSE, _eMotorIntertlock nInterlockType = _eMotorIntertlock::eMotorInterlock_Operation);	// 20190405 ihc. bIsBacklashDataTest추가
	ERR_CODE		MoveWhile(double a_dPos_um, double speed_umps =0, int accel=0, int dccel = 0);
	ERR_CODE		R_Move( double a_dPos_um, double speed_umps = 0, int nAcc_msec = 40, int nDec_msec = 40, BOOL bPcdBacklashMode = FALSE);	// ihc. bPcdBacklashMode추가
	ERR_CODE		Origin();
	ERR_CODE		GetOriginCheck(ERR_CODE *pError = NULL);
	ERR_CODE     OriginWhile();
	BOOL		 IsOrgCompletedInMotionController();
	ERR_CODE		MotionDone();

	/*=============================================================================
	RETURN   :
	0       = 움직이는 상태
	1       = 해당 축 동작 완료 or 해당 축 사용 안함 or Board open fail
	-1      = 해당 축 Timeover or Amp fault
	other	= interlock error code
	==============================================================================*/
	ERR_CODE		AxisDone();
	ERR_CODE		ChkPremovePos();
	ERR_CODE		V_MoveStop();
//	ERR_CODE		V_Move(double speed_umps, int accel=0);
	ERR_CODE        V_Move(int nDir, double dSpeed_umps, int nAccDec_msec);
	ERR_CODE		V_MoveCheckInterlock(double speed, int accel = 0);
	ERR_CODE		R_MoveWhile(double pos_um, double speed=0);
	ERR_CODE		RepeatMove(double move_pos_um, double rspeed=0,int racc=30,int rdcc=30);
	ERR_CODE		JogMove( int nDir, double dSpeed_umps, int nAccDec, BOOL bPcdBacklashMode = FALSE);
	BOOL	IsOrgCompleted(ERR_CODE *pError);
	/*
	RETURN: 1 : mmc board init fail or 해당 축 disable
	speed : 저장 된 속도 값( um / sec )
	*/
	double	GetSpeed();
	double  GetManualMoveSpeed_High();
	double  GetManualMoveSpeed_Middle();
	double  GetManualMoveSpeed_Low();

	bool	IsHomeSensorOn();
	bool	IsPositiveSensorOn();
	bool	IsNegativeSensorOn();
	BOOL	GetServoStat();
	bool	IsServoOn();
	bool		IsAxisDoneOrFree();

	ERR_CODE		AlarmReset();
	ERR_CODE		ServoOff();
	ERR_CODE		ServoOn();
	
	void	SetAxisInitInfo(char* szAxisName, int num, double scale
							 , int Zr_direction, int mmc_encoder_dir, int move_dir
							 , int a_nEncoderType, int mmc_coordinate_dir, int pulse_dir, int double_chk_pulse,
							 MOTOR_TYPE _motor_type, MOTOR_SYNC_TYPE _sync_type, int nslaveno, BOOL bDegreeCtrl);

	void    SetPulseDirection(int zdir, int pdir, int mdir, BOOL bEnType=TRUE);
	void    Set_Reset_Level(BOOL bType=FALSE);

	double	SetScaleFactor(int nRobotNo);
	void    SetScaleFactor(double Pit,double Dec,double needRevP);
	void	SetScaleFactor(double dScaleFactor);
	double  GetScaleFactor();
	double	GetMotorInposition();

	void	SetStopRate(double stop_dcc);
	ERR_CODE		SetStop();
	ERR_CODE		SetSlowStop();
	void	Set_disable(BOOL bDisable=true);
	bool	IsDisable();
	int		_source();

	BOOL	CheckPowerOffStat();
	void	MotorStrageCheck(ST_MOTORSTATUS* lpstMotorData);
	void    LogMtStrage(int nAxisNumPhy, double dPos_um=0.0, double dSpeed=0.0);
	BOOL	AmpFaultCheckFunction();
	void	SaveMotorMoveLog(double dTargetPos, double dMovePos, int nLogMode);		
											
	bool	SetSWLimit(double dPosLimit_um, double dNegLimit_um, BOOL bDisable = FALSE);	
	void    EnableSWLimit(BOOL bEnable);
	void	GetAxisName(char* pcAxisName, int nBuffSize);								

	//동작 상태
	bool    IsMotorStop();
	bool	IsPositionDone();	//위치 체크용
	int		GetAxisStatus();
	ERR_CODE		OriginStart();		//Switch 형식의 오리진 시작
	int		GetNoControlAxis();	//제어하고 있는 축수
	double	GetTargetPos(bool bMM=false);
	ERR_CODE		SetEmergencyStop();
	
	ERR_CODE SetAmpFaultLevel(BOOL bStatus);
	ERR_CODE SetAmpInpositionLevel(BOOL bStatus);

	ERR_CODE SetAmpNegativeLimitLevel(BOOL bStatus);

	ERR_CODE SetAmpPositiveLimitLevel(BOOL bStatus);
	bool CheckLimitClose();

	//Pcd
	void SetScaleFactorOffset(double dOffset);
	double GetScaleFactorOffset();

	//Backlash
	//double	GetBacklashOffset(double dMovePos);
	void SetParameterBacklash(int nDistanceIdx, int nSpdIdx, double dValue);
	void SetSpeedBacklash(int nSpdIdx, double dValue);
	double GetBacklashParameterData(double dMoveDistance, double dspeed_umps);
	double GetBacklashDataAboutSpeed(double dspeed_umps);
	double  GetBacklashOffset_TestPpX(double dMovePos, double dPPS);
 	double	GetBacklashOffset(double dMovePos, double dPPS);
	double  GetBacklashOffset_TestPP_X1(double dMovePos, double dPPS);
// 	double	GetBacklashDataAboutSpeed(double speed_umps);
// 	double	GetBacklashParameterData(double dMoveDistance, double speed_umps);

private:
	void	SetHome(int active);
	void    SetPositiveLimit(int nAction);
	void    SetNegativeLimit(int nAction);

	ERR_CODE NormalizeError(int Error); // softservo error -> ERR_CODE

	double get_manual_move_speed();
protected:	
	CTimerTickCheck		m_tm;

	public:
		static void set_sync_axis_map_setting(int nMaster, int nSlave);
		static void set_sync_control_setting(BOOL nAction);	// TRUE : 사용, FALSE : 해제
		static ERR_CODE SplineMove(std::vector<_stSpline> SplineMoveCmd, int nAxisNumPhy1, int nAxisNumPhy2);
};

