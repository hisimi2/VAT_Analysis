#pragma once

class CAllAutoStacker
{
private:
	bool m_bLoadTraySupplyEnd;
	bool m_bOneCycleCleanOut;
	int m_nLotEndTrayEndFeed;
	int m_nOffLineIgnore;
	int m_n2dUseIgnore;
	int m_n2dMarkUseIgnore;
	int m_n2dDuplicateIgnore;
	int m_nVisionOffIgnore;
	int m_nDeviceModeIgnore;
	int m_nRetestSkipIgnore;
	int m_nOnlineTesterMode;
	int m_nPressArriveSensorIgnore;
	int m_nDisConnectionStationIgnore;
	int m_nAutoRetestIgnore;
	int m_nCBModifyTimeCheck;
	int m_nCBDataOnOff;
	int m_nCBFileNotExist;

	int m_nGRRModeIgnore;
	int m_nLoopModeIgnore;
	int m_nAuditModeIgnore;
	int m_nQAModeIgnore;
	int m_nSwitchTrayFeederIgnore;
	
	int m_nTesterInfoIgnore;

public:
	CAllAutoStacker();
	~CAllAutoStacker();

	int OnLotCommand( eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam );

	bool GetLoadTraySupplyEnd();
	void ChkLoadTraySupplyEnd();
	void SetLoadTraySupplyEnd( bool bTraySupplyEnd ) { m_bLoadTraySupplyEnd = bTraySupplyEnd; };

	int GetLoadEnableStackerCount();
	int GetDevicePickEnableStackerCount();

	int GetSeqCmdStatusAllPart();
	int GetCurrentLotState();

	bool GetOneCycleCleanOut() { return m_bOneCycleCleanOut; }
	void SetOneCycleCleanOut( bool _bOneCycleCleanOut ) { m_bOneCycleCleanOut = _bOneCycleCleanOut; }

	int  GetTrayEndFeed() { return m_nLotEndTrayEndFeed; }//DEF_LOTEND_UNK
	void SetTrayEndFeed( int nEndFeed ) { m_nLotEndTrayEndFeed = nEndFeed; };

	BOOL GetOffLineUseIgnore() { return m_nOffLineIgnore; }
	void SetOffLineUseIgnore( int nOffLineUseIgnore ) { m_nOffLineIgnore = nOffLineUseIgnore; };

	BOOL Get2dUseIgnore() { return m_n2dUseIgnore; }
	void Set2dUseIgnore( int n2dDUseIgnore ) { m_n2dUseIgnore = n2dDUseIgnore; };

	BOOL Get2dMarkUseIgnore() { return m_n2dMarkUseIgnore; }
	void Set2dMarkUseIgnore(int n2dMarkUseIgnore) { m_n2dMarkUseIgnore = n2dMarkUseIgnore; };

	BOOL Get2dDuplicateIgnore() { return m_n2dDuplicateIgnore; }
	void Set2dDuplicateIgnore( int  n2dDuplicateIgnore ) { m_n2dDuplicateIgnore = n2dDuplicateIgnore; };

	BOOL GetVisionOffIgnore() { return m_nVisionOffIgnore; }
	void SetVisionOffIgnore( int nVisionOffIgnore ) { m_nVisionOffIgnore = nVisionOffIgnore; };

	BOOL GetDeviceModeIgnore() { return m_nDeviceModeIgnore; }
	void SetDeviceModeIgnore( int nDeviceModeIgnore ) { m_nDeviceModeIgnore = nDeviceModeIgnore; };

	BOOL GetRetestSkipIgnore() { return m_nRetestSkipIgnore; }
	void SetRetestSkipIgnore( int nRetestSkipIgnore ) { m_nRetestSkipIgnore = nRetestSkipIgnore; };

	BOOL GetOnlineTesterMode() { return m_nOnlineTesterMode; }
	void SetOnlineTesterMode(int nOnlineTesterMode) { m_nOnlineTesterMode = nOnlineTesterMode; };

	BOOL GetPressArriveSensorIgnore() { return m_nPressArriveSensorIgnore; }
	void SetPressArriveSensorIgnore(int nPressArriveSensorIgnore) { m_nPressArriveSensorIgnore = nPressArriveSensorIgnore; };

	BOOL GetDisConnectionStation() { return m_nDisConnectionStationIgnore; }
	void SetDisConnectionStation(int nDisConnectionStationIgnore) { m_nDisConnectionStationIgnore = nDisConnectionStationIgnore; };

	BOOL GetAutoRetestIgnore() { return m_nAutoRetestIgnore; }
	void SetAutoRetestIgnore(int nAutoRetestIgnore) { m_nAutoRetestIgnore = nAutoRetestIgnore; };	

	BOOL GetCBModifyTimeCheckIgnore() { return m_nCBModifyTimeCheck; }
	void SetCBModifyTimeCheckIgnore(int nCBModifyTimeCheck) { m_nCBModifyTimeCheck = nCBModifyTimeCheck; };

	BOOL GetCBDataOnOffIgnore() { return m_nCBDataOnOff; }
	void SetCBDataOnOffIgnore(int nCBDataOnOff) { m_nCBDataOnOff = nCBDataOnOff; }

	BOOL GetCBFileNotExistIgnore() { return m_nCBFileNotExist; }
	void SetCBFileNotExistIgnore(int nCBFileNotExist) { m_nCBFileNotExist = nCBFileNotExist; };

	BOOL GetGRRModeIgnore() { return m_nGRRModeIgnore; }
	void SetGRRModeIgnore(int nGRRModeIgnore) { m_nGRRModeIgnore = nGRRModeIgnore; };

	BOOL GetLoopModeIgnore() { return m_nLoopModeIgnore; }
	void SetLoopModeIgnore(int nLoopModeIgnore) { m_nLoopModeIgnore = nLoopModeIgnore; };

	BOOL GetAuditModeIgnore() { return m_nAuditModeIgnore; }
	void SetAuditModeIgnore(int nAuditModeIgnore) { m_nAuditModeIgnore = nAuditModeIgnore; };

	BOOL GetQAModeIgnore() { return m_nQAModeIgnore; }
	void SetQAModeIgnore(int nQAModeIgnore) { m_nQAModeIgnore = nQAModeIgnore; };

	BOOL GetSwitchTrayFeederIgnore() { return m_nSwitchTrayFeederIgnore; }
	void SetSwitchTrayFeederIgnore(int nSwitchTrayFeederIgnore) { m_nSwitchTrayFeederIgnore = nSwitchTrayFeederIgnore; };

// 	BOOL GetTesterInfoIgnore() { return m_nTesterInfoIgnore; }
// 	void SetTesterInfoIgnore(int nTesterInfoIgnore) { m_nTesterInfoIgnore = nTesterInfoIgnore; }
};


typedef struct _ST_CONVEYOR_WORK_AREA
{
private:
	bool bWorking;
	CCriticalSection cs;
	CBaseTaskManager* m_reserved_task;

public:
	_ST_CONVEYOR_WORK_AREA() {
		bWorking = false;
		m_reserved_task = nullptr;
		::InitializeCriticalSection( cs );
	}
	~_ST_CONVEYOR_WORK_AREA() {
		::DeleteCriticalSection( cs );
	}

	bool is_work_enable_state( CBaseTaskManager* _this );
	void release_work_area( CBaseTaskManager* _this );
	bool reserve_work_area( CBaseTaskManager* _this );
	CBaseTaskManager* get_reserved_task();

}ST_CONVEYOR_WORK_AREA;


typedef struct _ST_TRANSFER_FEEDER_WORK_AREA
{
private:
	bool bWorking;
	CCriticalSection cs;
	CBaseTaskManager* m_reserved_task;

public:
	_ST_TRANSFER_FEEDER_WORK_AREA() {
		bWorking = false;
		m_reserved_task = nullptr;
		::InitializeCriticalSection(cs);
	}
	~_ST_TRANSFER_FEEDER_WORK_AREA() {
		::DeleteCriticalSection(cs);
	}

	bool is_work_enable_state(CBaseTaskManager* _this);
	void release_work_area(CBaseTaskManager* _this);
	bool reserve_work_area(CBaseTaskManager* _this);
	CBaseTaskManager* get_reserved_task();

}ST_TRANSFER_FEEDER_WORK_AREA;

typedef struct _ST_TESTPP_TABLE_WORK_AREA
{
private:
	bool bWorking;
	CCriticalSection cs;
	CBaseTaskManager* m_reserved_task;

public:
	_ST_TESTPP_TABLE_WORK_AREA() {
		bWorking = false;
		m_reserved_task = nullptr;
		::InitializeCriticalSection(cs);
	}
	~_ST_TESTPP_TABLE_WORK_AREA() {
		::DeleteCriticalSection(cs);
	}

	void release_work_area(CBaseTaskManager* _this);
	bool reserve_work_area(CBaseTaskManager* _this);
	CBaseTaskManager* get_reserved_task();

}ST_TESTPP_TABLE_WORK_AREA;