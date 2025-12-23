#pragma once
class CTaskTrayFeeder : public CBaseTaskManager
{
public:
	enum eWorkSite {
		eWorkSite_NotAvaliable = 0,
		eWorkSite_Conveyor,
		eWorkSite_Feeder
	};

	enum eDI {
		xTRAY_FEEDER_GRIP_FOR_LEFT_SEN,
		xTRAY_FEEDER_GRIP_BACK_LEFT_SEN,
		xTRAY_FEEDER_UP_LEFT_SEN,
		xTRAY_FEEDER_DOWN_LEFT_SEN,
		xTRAY_FEEDER_GRIP_FOR_RIGHT_SEN,
		xTRAY_FEEDER_GRIP_BACK_RIGHT_SEN,
		xTRAY_FEEDER_UP_RIGHT_SEN,
		xTRAY_FEEDER_DOWN_RIGHT_SEN,
		xTRAY_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN,
		xTRAY_FEEDER_SAFETY_SEN,
		xTRAY_FEEDER_EXIST_SEN,
		xMAX_COUNT,
	};

	enum eDO {
		yTRAY_FEEDER_GRIP_FOR,
		yTRAY_FEEDER_GRIP_BACK,
		yTRAY_FEEDER_UP,
		yTRAY_FEEDER_DOWN,
		yTRAY_FEEDER_ALIGN_X_FOR,
		yTRAY_FEEDER_ALIGN_X_BACK,

		yMAX_COUNT,
	};

	enum eMotor {
		eAxisAll = -1,
		eAxisY = 0,

		eAxisMax,
	};

	enum eFeederWorkDir {
		eFeederWorkDirTopLeft = 0,
		eFeederWorkDirBottomLeft,
	};

	enum eFeederStepLast {
		eFeederStepLast_First = -1,
		eFeederStepLast_Seconde = -2,
	};
	// base
	CTaskTrayFeeder();
	~CTaskTrayFeeder() = default;

	int Initialize( char* szDriverFileName );
	void Finalize();

	void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	// common
	void MakeLog( LPCTSTR fmt, ... );

	// Motor
	int do_MovePos( eMotor motor_idx, eLocIdxFeeder nLocStage, int nTrayRowStep = 0, uint32_t nSeqSpdRate = 100 );
	int do_MovePos_Pcd_backlash(double dAbsPosition, eMotor nAxisIdx, int nSeqSpdRate = 5, double dPosOffset = 0.0, BOOL bIsApplyBcklsh = FALSE);
	int	ChkMotorPos( eMotor motor_idx, eLocIdxFeeder nLocStage, int nTrayRowStep = 0 );
	int WhereisMotorPos();
	// cylinder
	int			do_cyl_ClampUnclamp( eClampCmd nClampUnclamp );
	stErrCode	chk_cyl_ClampUnclamp( eClampCmd nClampUnclamp );
	stErrCode	get_cyl_ClampUnclamp( eClampCmd nClampUnclamp );
	int			do_cyl_UpDown( eUpDownCmd nUpDown );
	stErrCode	chk_cyl_UpDown( eUpDownCmd nUpDown );
	stErrCode	get_cyl_UpDown( eUpDownCmd nUpDown );
	int			do_cyl_align_ForBack( eForwBackCmd nForBack );
	stErrCode	chk_cyl_align_ForBack( eForwBackCmd nForBack );
	stErrCode	get_cyl_align_ForBack( eForwBackCmd nForBack );


	bool GetTrayExist_TransferArea_Data();
	// tray exist
	bool GetTrayExist_Data();
	bool IsEmptyDevice();
	void SetTrayDeviceExistRowStep(int nTrayDeviceExistRow, int nHandYOffset, int nWorkDir);
	int  GetTrayDeviceExistRowStep();

	void SetIsTrayLastRow(BOOL bSet);
	BOOL GetIsTrayLastRow();

	void SetClearStep();

	int ChkTrayExist_TransferArea_Sen( eExistState nExist );
	int GetTrayExist_TransferArea_Sen( eExistState nExist );

	int GetTrayExist_TransferArea_DvcMode(int nExist);

	int GetSafetySensor();
	int ChkSafetySensor(eExistState nExist);
	int GetTrayExistSensor();
	int ChkTrayExistSensor(eExistState nExist);
	int GetTrayExistSensor_DvcMode(int nExist);

	BOOL ChkFeederFinalRow(int nCurStep);

	eWorkSite WhereIsWorkSite();
	bool	is_safety_state();
	bool	reserve_is_safety_state();
	double	GetTeachPos(eMotor motor_idx, eLocIdxFeeder nLocStage);
	// Motor
	OneAxis* GetAxis( eMotor nAxisIdx );

	int GetDiExtNum(CTaskTrayFeeder::eDI internal_io_num)
	{
		return m_vX[internal_io_num];
	};

	int GetDoExtNum(CTaskTrayFeeder::eDO internal_io_num)
	{
		return m_vY[internal_io_num];
	};

	enum MotorPos {
		ePosY_None = -1,

		ePosY_StartPos = 0,
		ePosY_LoadPos,
		ePosY_UnloadPos,
	};

//Feeder Status
public:
	int GetFeederStatus();

	//Cmd : Pick & Place
	//Status : Cylinder Up/Down, Gripper Clamp/Unclamp
	enum FeederStatus
	{
		eFeederStatus_Up_Clamp = 0,
		eFeederStatus_Up_Unclamp,
		eFeederStatus_Down_Clamp,
		eFeederStatus_Down_Unclamp,
	};


public:
	int m_bInitialized;
	int m_nTrayDeviceExistRowStep;
	BOOL m_bTrayLastRow;
	int m_nPitchType;

	

private:
	enum eThreadNum : int
	{
		eTHREAD_1,
	};

	// Physical Information
	std::vector<int> m_vX;
	std::vector<int> m_vY;
	std::vector<OneAxis*> m_vMotor;

	CMap<int, int, CString, CString> m_mapDI;
	CMap<int, int, CString, CString> m_mapDO;
	CMap<int, int, CString, CString> m_mapMotor;

	// common
	void	ThreadProc1();
	

	// DIO
	stErrCode		ChkDI( eDI nIndex, eOnOffState bValue = eOn );
	stErrCode		ChkDI( std::initializer_list<std::pair<eDI, eOnOffState>> nIndexList );
	stOnOffState	GetDI( eDI nIndex );
	stErrCode		GetDI( std::initializer_list<std::pair<eDI, eOnOffState>> nIndexList );
	stOnOffState	GetDO( eDO nIndex );
	stErrCode		GetDO( std::initializer_list<std::pair<eDO, eOnOffState>> nIndexList );
	int				SetDO( eDO nIndex, eOnOffState eOnOff );
	int				SetDO( std::initializer_list<std::pair<eDO, eOnOffState>> nIndexList );

//VAT
public:
	int do_VATMovePos(double dTargetPos, double dOffset = 0.0);
	int ChkVATMotorPos(double dTPos);
};

