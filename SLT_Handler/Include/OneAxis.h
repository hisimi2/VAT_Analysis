#pragma once

#include "DEF_MOTION.h"
#include "DefineBbxmnp.h"
#include "DEF_ERROR_CODE.h"
class AFX_EXT_CLASS OneAxis;

struct ST_MOTION_INIT_PARAM
{
	double dInPosWidth;
	int nPulsePerRound;
	int nMoveDirection;
	int nSlaveNumber;
};

enum eAxisServoConfiguration {
	RestartEngine = 0x01,				// Engine 재시작
	ReconnectCommunication = 0x02,		// 통신 재시작
	BasicConfiguration = 0x04,			// Motor를 이동시키기 위해 필요한 기본적인 설정들
	ExtendedConfiguration = 0x08,		// 필요하지만, Motor 이동에 반드시 필요한 설정은 아님

	Default = BasicConfiguration | ExtendedConfiguration,
	Full = RestartEngine | ReconnectCommunication | BasicConfiguration | ExtendedConfiguration,
};

enum eEncoderType{
	eEncoderType_4Ch=0, eEncoderType_4Ch_NoPulseDir, eMaxEncoderTypeCount,
};
enum eMotorOrgFlag {
	eMotorOrgFlag_ING=0,eMotorOrgFlag_OK=1, eMotorOrgFlag_NG=-1,
};

typedef enum eMotorType
{
	eMotorType_Servo = 0,
	eMotorType_Step,
}MOTOR_TYPE;

typedef enum eMotorSyncType
{
	eMotorSyncType_None = 0,
	eMotorSyncType_Master,
	eMotorSyncType_Slave,
}MOTOR_SYNC_TYPE;

typedef int (OneAxis::*fnEvent)();

typedef struct AFX_EXT_CLASS _stSpline
{
	double dSpeed;
	double dAcc;
	double dDec;
	double dTarget[2];
}SPLINE_MOVE_CMD;

enum _eTorqueType : int
{
	CURRENT_ACTUAL_TORQUE = 0,
	MAX_ACTUAL_TORQUE,
	CURRENT_PEAK_TORQUE,
	MAX_PEAK_TORQUE,
	MAX_ITEM,
};

typedef struct AFX_EXT_CLASS _tagEventMap
{
	int nID;
	fnEvent pfn;
}EVENT_MAP;


class AFX_EXT_CLASS OneAxis : public CObject
{
public:
	OneAxis();
	virtual ~OneAxis(void);

	static EVENT_MAP	EventMap[];

	virtual ERR_CODE InitBoard() { return -1/*ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE*/; };

	virtual void    SetAxisInitInfo(
		char* szAxisName,
		int num,
		int nLogicalNum,
		double scale,
		int Zr_direction,
		int mmc_encoder_dir,
		int move_dir,
		int a_nEncoderType,
		int mmc_coordinate_dir,
		int pulse_dir,
		int double_chk_pulse,
		MOTOR_TYPE _motor_type,
		MOTOR_SYNC_TYPE _sync_type,
		int nslaveno,
		BOOL bDegreeCtrl,
		int nPulsePerRound,     // Pulses per 1 round
		double dHwInPosWidth);  // Hardware In-position width value
	BOOL			GetDegreeAxis() { return m_bDegeeCtrl; }
	const char*     GetAxisName(){ return m_szAxisName; };
	inline int		GetAxisNo() const { return m_nAxisNumPhy; }
	inline int		GetAxisLogicNo() const { return m_nAxisNumLogical; }
	inline double	GetSwLimitPosi() const  { return m_dSwLimit_Posi; }
	inline double	GetSwLimitNega() const  { return m_dSwLimit_Nega; }
	inline double	GetDesignScaleFactor() const { return m_dDesignScaleFactor; }

	virtual int		Move(double a_dPos_um, double a_dSpeed_umps = 0, int a_nAccel = 0, int a_nDccel = 0, BOOL bApplyBacklash = FALSE, _eMotorIntertlock nInterlockType = _eMotorIntertlock::eMotorInterlock_Operation) { return 0; };
	virtual int		MoveWhile(double a_dPos, double a_dSpeed = 0, int a_nAccel = 0, int a_nDccel = 0) { return 0; };
	virtual int     R_MoveWhile(double a_dPos, double a_dSpeed) { return 0; };
	//	virtual int     V_Move(double a_dSpeed, int a_nAccel){return 0;};
	virtual int 	V_Move(double a_dSpeed, int a_nAccel) { return 0; };
	virtual int     V_MoveCheckInterlock(double speed, int accel = 0) { return 0; };
	virtual int     V_MoveStop() = 0;
	virtual int		R_Move(double a_dPos, double a_dSpeed = 0) { return 0; };
	virtual int     RepeatMove(double a_dPos, double a_dSpeed, int a_nAcc, int a_nDcc) { return FALSE; };
	virtual int		JogMove(int nDir, double dSpeed_umps, int nAccDec_msec, BOOL bPcdBacklashMode = FALSE) { return 0; };

	// Z Axis Motor

	virtual void    bbwmnpAxisInfo(int nSubZAxisNum, double scale, int Zr_direction/*=1*/, int pulse_dir/*=1*/, int move_dir/*=1*/) { return; };

	virtual int     V_Move(_eJogDir eJogDir, int nAxisCnt, int* anAxes, double speed, int accel /*=0*/) { return 0; };
	virtual int		V_Move(_eJogDir eJogDir, _eBbxmnpAxis eAxis, double speed, int accel = 0) { return 0; };
	virtual int		Move(std::vector<CPoint> vPicker, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, int nBoardType = eBoardDir_Forward, BOOL bNoUseOffset = FALSE /*BOOL bVatOffsetUse = FALSE*/) { return 0; };
	virtual int		Move(_eBbxmnpAxis eAxis, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, int nBoardType = eBoardDir_Forward, BOOL bNoUseOffset = FALSE) { return 0; };
	virtual int		Move_Inc(std::vector<CPoint> vPicker, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL No_Check_safety = FALSE) { return 0; };
	virtual int		MoveWhile(std::vector<CPoint> vPicker,double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL bNoUseOffset = FALSE) { return 0; };
	virtual int		MoveWhile(_eBbxmnpAxis eAxis, double pos, double speed = 0, int accel = 0, int dccel = 0,BOOL No_Check_safety = FALSE) { return 0; };
	virtual int		R_MoveWhile(std::vector<CPoint> vPicker, double pos, double speed = 0, int accel = 0) { return 0; };

	virtual bool    IsHomeSensorOn(_eBbxmnpAxis eAxis) { return false; };
	virtual BOOL	GetServoState(_eBbxmnpAxis eAxis) { return FALSE; };
	virtual BOOL	GetOriginFlag() { return FALSE;};

	virtual double  GetCurrentPos(double &dPos, _eBbxmnpAxis eAxis) { return 0; };
	virtual int	   GetCurrenPosAvg(double &dPos) { return 0; };

	virtual int		ServoOn(_eBbxmnpAxis eAxis) { return 0; };
	virtual int		ServoOff(_eBbxmnpAxis eAxis) { return 0; };
	virtual int		V_MoveStop(_eBbxmnpAxis eAxis) { return 0; };

	virtual bool    IsHomeSensorOn() { return false; };
	virtual void	SetInitMotorParam(double r_vel, int r_acc, int r_dcc, double o_vel, int o_acc, double pos, double neg,double scalefactor) {};

	virtual int		AxisDone(std::vector<CPoint>& vPicker, std::vector<CPoint>& vErrPicker) { return false; };
	//////////////////////////////////////////////////////////////////////////

	virtual int		Origin(){return 0;};
	virtual int     OriginWhile(){return 0;};
/*	virtual BOOL	IsOrgCompleted(){ return FALSE; };*/
	virtual BOOL			IsOrgCompleted(ERR_CODE *pError)
	{
		if (pError) *pError = ERR_MOTOR_CMN_ORIGIN_FAILURE; return FALSE;
	}

	virtual int		OriginStart(){return 0;};		//Switch 형식의 오리진 시작
	virtual int     OriginStop();
	virtual int 	GetOrgFlag() { return m_nOri_Flag; };

	virtual int		MotionDone(){return 0;};
	virtual int     AxisDone(){return 0;};

	virtual int     ServoOn(){return 0;};
	virtual int     ServoOff(){return 0;};
	virtual BOOL    CheckPowerOffStat(){return FALSE;};

	virtual void    SetCommandPos(double a_dPos){};
	virtual int		GetCommandPos(double &dPos){return 0;};

	virtual void    SetCurrentPos(double pos){};
	//virtual double	GetCurrentPos(){return 0;};
	virtual int  GetCurrentPos(double &dPos) { return 0; };

	virtual void    Set_disable(BOOL bDisable){};
	virtual bool    IsDisable(){return false;};
	virtual bool	InSequence(){ return false; };
	virtual bool	InMotion(){ return false; };
	virtual bool	InPosition(){ return false; };
	virtual bool	IsAxisDone(){ return false; };

	virtual int     GetaxisdoneORfree(){return 0;};
	virtual BOOL    GetServoStat(){return FALSE;};
	virtual BOOL    GetPositiveSensor(){return FALSE;};
	virtual BOOL    GetNegativeSensor(){return FALSE;};
	virtual double  GetVelocity(){return FALSE;};
	virtual int     GetAcc(){return FALSE;};
	virtual int     GetDcc(){return FALSE;};
	virtual double  GetSpeed(){return FALSE;};
	virtual BOOL    GetAmpFaultStat(){return FALSE;};
	virtual double	GetErrorValue(){ return 1; };
	virtual bool    IsAmpFault() { return false; };

	virtual int		SetStop(){ return 1; };
	virtual int		SetSlowStop(){ return 1; };
	virtual int		SetSlowStopEach(_eBbxmnpAxis eAxis) { return 1; };
	virtual void    SetStopRate(double stop_dcc){};
	virtual bool    SetSWLimit(double dPosLimit, double dNegLimit, BOOL bDisable = FALSE) { return true; };
	virtual void    EnableSWLimit(BOOL bEnable){};
	virtual void    SetVelAcc(double r_vel, int r_acc, double o_vel, int o_acc, double pos, double neg);
	virtual void    SetVelAcc(double r_vel, int r_acc, int r_dcc, double o_vel, int o_acc, double pos, double neg);
	virtual void    SetHome(int active){};
	virtual void    SetPulseDirection(int zdir, int pdir, int mdir, BOOL bEnType = TRUE){};
	virtual void    Set_Reset_Level(BOOL bType){};
	virtual int     sethomelevel(BOOL bStatus){return FALSE;};

	virtual BOOL    ClearStatus(){return FALSE;};
	virtual int     AlarmReset(){return 0;};
	virtual void    FramesClear(){};	
	virtual void    ClearOrgFlag();
	virtual int     _source(){return FALSE;};	
	
	virtual double  LimitValueCheck(BOOL Posi_Nega);
	virtual void    MotorStrageCheck(ST_MOTORSTATUS* lpstMotorData){};
	virtual BOOL    AmpFaultCheckFunction(){return FALSE;};
	virtual void    SaveMotorMoveLog(double dTargetPos, double dMovePos, int nLogMode, double dEncoder){};

	virtual void	ShowSimulationWindow(BOOL bShow){};
	virtual int		TimeCheck(int time);	

	virtual void	SetScaleFactor(double scale);
	virtual double  GetScaleFactor(){return 0.0;};

	virtual double  GetServoPulse(){return 2500.0;};
    virtual void    SetServoPulse(double dServoPulse);

	virtual void SetPartNo(int nPartNo) { m_nPartNo = nPartNo; }
	virtual int GetPartNo() { return m_nPartNo; }

	//동작 상태
	virtual bool	IsPositionDone(){return false;};	//위치 체크용
	virtual int		GetAxisStatus(){return 0;};
	virtual int		GetNoControlAxis(){return 0;};	//제어하고 있는 축수
	virtual double	GetTargetPos(bool bMM=false){return 0;};
	virtual int		SetEmgStop(){return 0;};
	virtual int     ChkMotorUsable();
	virtual BOOL	IsMotorUsable(int &nErr); //Motor Interlock에서 사용.
	//Backlash
	virtual double	GetBacklashOffset() { return 0.0; };
	virtual double	GetBacklashOffset_TestPP_X1() { return 0.0; };
	
	virtual double	GetBacklashDataAboutSpeed( double speed_umps ) { return 0.0; };
	virtual void	SetSpeedBacklash( int nSpdIdx, double dValue ) {};
	virtual void	SetParameterBacklash(int nDistanceIdx, int nSpdIdx, double dValue) {};
	virtual double	GetLastBacklashOffset() { return m_dMem_BacklashOffset; };

	virtual double  GetEncoderPulse() { return 0.0; };

	virtual bool ApplyConfiguration(eAxisServoConfiguration Configuration, BOOL bUseStartEngine = FALSE) { return true; }
	virtual bool ApplyPlviModuleConfigration(bool bSet) { return true; }
	virtual int GetTorque(BOOL bMaxTorque, double &dTorquePesentage) { return 0; }
	virtual int ReadActualPeakTorqueData(int nAxisNo, _eTorqueType eType) { return 0; }
	virtual double GetReadActualTorque_PDO(int nAxisNo) { return 0.0; }

	virtual ERR_CODE     CheckMotorUsableForVMove();
	virtual ERR_CODE     CheckMotorUsable();



public:
	void SetInterlock(FN_MOTOR_MOV_INTERLOCK_CALLBACK fnMovInterLock, FN_MOTOR_ORG_INTERLOCK_CALLBACK fnOrgInterLock, FN_MOTOR_JOG_INTERLOCK_CALLBACK fnJogInterLock )
	{ 
		m_cbInterLock_Move = fnMovInterLock; 
		m_cbInterLock_Org = fnOrgInterLock;
		m_cbInterLock_Jog = fnJogInterLock;
	};


	FN_MOTOR_MOV_INTERLOCK_CALLBACK m_cbInterLock_Move;     // 정상인 경우 0, Error의 경우 음수 값
	FN_MOTOR_ORG_INTERLOCK_CALLBACK m_cbInterLock_Org;      // 정상인 경우 0, Error의 경우 음수 값
	FN_MOTOR_JOG_INTERLOCK_CALLBACK m_cbInterLock_Jog;      // 정상인 경우 0, Error의 경우 음수 값

	virtual bool CheckLimitClose() { return false; };
	virtual double  GetMtMonCurPos() { return 0.0; };
protected:
	int				m_nAxisNumPhy;						// 0 base, 물리적 축번호
	int				m_nSubZAxisNumPhy;						// 0 base, Motor Z축 Number
	double			m_dSwLimit_Posi, m_dSwLimit_Nega;	// [um]
	double			m_dSwLimit_Posi_um, m_dSwLimit_Nega_um;
	double			m_dScaleFactor;						// [um/pulse]
	double			m_dDesignScaleFactor;				// [um/pulse]
	double          m_dServo_Pulse;

	double			m_dTarget_pos;						// [um]
	double			m_dLast_pos;						// [um]
	double			m_dCmdVelocity, m_dCmdVelocity_Ori; // [um/sec]
	int				m_nCmdAcc, m_nCmdAcc_Ori;			// [msec]
	int				m_nCmdDcc;							// [msec]

	int				m_nDir_Ori;							// -1:역, 1:정
	int				m_nDir_Pulse;						// -1:역, 1:정
	int				m_nDir_Move;						// -1:역, 1:정
	int				m_nLastOri_Step;						
	clock_t			m_clockStart, m_clockFinish;
	int             m_nEncoderType;						// enum eEncoderType


	char			m_szAxisName[128];
	double			m_dPosLimitPulse;					// [pulse]
	double			m_dNegLimitPulse;					// [pulse]

	BOOL			m_bMotorDisable;					// 0:disable, 1:enable
	BOOL			m_bResetLevel;						// 0:LOW, 1:HIGH

	double			m_dActTime;
	int				m_nOri_Flag;						// 1:OK, 0: Org ing, -1:Not Org
	BOOL            m_bOri_Stop; 


protected:
	//Backlash
	std::vector<double>		m_vBacklashOffset_XY;
	double					m_dBacklashParameterOffset_XY[eDistance_MAX][eSpeed_MAX];

	double					m_dDistanceRate;
	double					m_dMem_BacklashOffset;
	double					m_dBackDisplay_MtEncoder;

	BOOL					m_bBacklashOffset_Apply_Flag;
	double					m_dBefore_BacklashPos;
	double					m_dAfter_BacklashPos;

	BOOL					m_OriMemFlag;




protected:
		int				m_nAxisNumLogical;					// 0 base, 프로그램에서 사용하는 논리적인 번호 ( 연속됨 )
		int				m_nPartNo;							// PartNo저장

		double			m_dTarget_pos_um;						// [um]

		int				m_nMMC_Encoder_dir;					// 0: 정, 1: 역
		int				m_nMMC_Coordinate_dir;				// 0: 정, 1: 역

		int				m_nDir_Pluse;						// -1:역, 1:정

		/*================================================================
		origin 진행 시 2중으로 home check 진행 할때 사용하는 um 값.
		1차 origin 진행 후 해당 um 값 만큼 origin 반대 방향으로 이동.
		이동 후 다시 origin 진행 하여 2중 check 진행 한다.
		================================================================*/
		int				m_nDouble_chk_um;
		bool			m_bDoubleCheckFlag;					// 두번쨰 Check 중일 때 true;

		int				m_nOri_Step;

		MOTOR_TYPE		m_eMotorType;


		MOTOR_SYNC_TYPE  m_eMotorSyncType;						// 0 : no use, 1: master, 2, slave
		int				 m_nSlaveNo;							// m_eMotorSyncType = 1, 2 경우 사용
		BOOL			 m_bDegeeCtrl;							// 단위 um이 아닌 º이다.
		double			m_dPulsePerRound;						// 1바퀴 이동 당 필요 Pulse 수. 20191214 ihc.(각도계산위해 저장)
		double          m_dHwInPosWidth;                        // Hardware In-position width. Servo driver에 설정.

																// Backlash 20191204 ihc
		int  m_nMotorSpdRateIdx;
		bool m_bApplyBacklash;
		double m_arrBackLashOffset[11];		// 5% ~ 100% 속도별 Backlash Offset저장
		double m_arrPrevBackLashOffset[11];	// 5% ~ 100% 속도별 이전 Backlash Offset저장
		double m_dPrevMovePos;
		double m_dBacklashMoveDist;

		// Pcd
		double          m_dScaleFactorOffset;

		// EziServo
		//double			m_dPosTable[eMaxOperation];				// position table

		// module trigger state
		bool m_bPlviModuleServo;

public:
	CString         m_strServoMaker;
	
};

