#pragma once

class CBaseTaskManager;

class CTaskTransfer : public CBaseTaskManager
{
public:
	enum eThreadNum : int
	{
		eTHREAD_1,
	};

	enum eDI : int
	{
		xTRANSFER_LEFT_CLAMP_SEN,
		xTRANSFER_LEFT_UNCLAMP_SEN,
		xTRANSFER_RIGHT_CLAMP_SEN,
		xTRANSFER_RIGHT_UNCLAMP_SEN,
		xTRANSFER_TRAY_EXIST_SEN,    // B접 센서
		xTRANSFER_UP_SEN,
		xTRANSFER_DOWN_SEN,

		MAX_DI_COUNT
	};

	enum eDO : int
	{
		yTRANSFER_CLAMP_SOL,
		yTRANSFER_UNCLAMP_SOL,
		yTRANSFER_UP_SOL,
		yTRANSFER_DOWN_SOL,

		MAX_DO_COUNT
	};

	enum eAxis : int
	{
		AxisAll = -1,
		AXIS_TRANSFER_X,

		eMaxAxisCount,
	};

	CTaskTransfer();
	~CTaskTransfer();

	int Initialize( TCHAR * szDriverFileName );
	void Finalize();

	void ThreadProc1();

	int OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList = NULL );

	virtual void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	double GetTeachPos( eLocIdxTransfer nLocStage, CTaskTransfer::eAxis _axis );

	void MakeLog( LPCTSTR fmt, ... );

	// Motor
	int do_MovePos( eAxis eAxisNo, eLocIdxTransfer nLocation, double dOffset_um = 0.0, int nSeqSpdRate = 100 );
	/*======================================
	return :
	0 = 동작 중.
	1 = 정지. ( AxisDone )
	======================================*/
	int ChkAxisDone( eAxis eAxisNo );

	OneAxis* GetAxis( int nAxisIdx );
	int GetAxisCount();

	// DIO (Cylinder)
	int		ChkDI( int nIndex, BOOL bValue = TRUE );
	BOOL	GetDI( int nIndex );
	int		GetDI( int nIndex, BOOL bValue );

	int do_cyl_ClampUnclamp( int nClampUnclamp );
	int chk_cyl_ClampUnclamp( int nClampUnclamp );
	int get_cyl_ClampUnclamp( int nClampUnclamp );

	int do_cyl_UpDown( int nUpDown );
	int chk_cyl_UpDown( int nUpDown );
	int get_cyl_UpDown( int nUpDown );

	int ChkTrayExist( int nExist );
	int GetTrayExist( int nExist );

	bool is_safety_state();

	int GetTrayExist_DvcMode(int nExist);

	// Physical Information
	std::vector< OneAxis* > m_vMotor;       // Motor Pointer list

	std::vector<int>                   m_vX;           // DI id list
	std::vector<int>                   m_vY;           // DO id list

	CMap<int, int, CString, CString>      m_mapDI;
	CMap<int, int, CString, CString>      m_mapDO;
	CMap<int, int, CString, CString>      m_mapMotor;

	bool m_bInitialized;
};

