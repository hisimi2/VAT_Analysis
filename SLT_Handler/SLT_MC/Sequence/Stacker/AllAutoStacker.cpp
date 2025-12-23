#include "StdAfx.h"
#include "AllAutoStacker.h"


CAllAutoStacker::CAllAutoStacker()
{
	m_bLoadTraySupplyEnd = false;
	m_bOneCycleCleanOut = false;
	m_nLotEndTrayEndFeed = DEF_LOTEND_UNK;
	m_nOffLineIgnore = 0;
	m_n2dUseIgnore = 0;
	m_n2dMarkUseIgnore = 0;
	m_n2dDuplicateIgnore = 0;
	m_nVisionOffIgnore = 0;
	m_nDeviceModeIgnore = 0;
	m_nRetestSkipIgnore = 0;
	m_nOnlineTesterMode = DEF_REQ_UNK;
	m_nPressArriveSensorIgnore = DEF_REQ_UNK;
	m_nAutoRetestIgnore = DEF_REQ_UNK;
	m_nDisConnectionStationIgnore = DEF_REQ_UNK;
	m_nCBModifyTimeCheck = 0;
	m_nCBDataOnOff = 0;
	m_nCBFileNotExist = 0;

	m_nGRRModeIgnore = DEF_REQ_UNK;
	m_nLoopModeIgnore = DEF_REQ_UNK;
	m_nAuditModeIgnore = DEF_REQ_UNK;
	m_nQAModeIgnore = DEF_REQ_UNK;
	m_nTesterInfoIgnore = DEF_REQ_UNK;
	m_nSwitchTrayFeederIgnore = DEF_REQ_UNK;
}


CAllAutoStacker::~CAllAutoStacker()
{
}

bool CAllAutoStacker::GetLoadTraySupplyEnd()
{
	return m_bLoadTraySupplyEnd;
}

void CAllAutoStacker::ChkLoadTraySupplyEnd()
{
	int load_enable_stacker_count = GetLoadEnableStackerCount();

	if( load_enable_stacker_count == 0
		&& g_Error.IsSetAlarm( ERR_LOT_MSG_REQ_ADD_LOAD_TRAY ) == FALSE
		&& m_bLoadTraySupplyEnd == false ) {
		// 사용자가 supply end 할 수 있도록 알람 떠야 함.
		CParamList AlarmData;
		g_Error.AlarmReport( ERR_LOT_MSG_REQ_ADD_LOAD_TRAY, AlarmData, &g_TaskSystemCtrl );
	}
}

int CAllAutoStacker::GetLoadEnableStackerCount()
{
	int load_enable_stacker_count = std::count_if( g_load_stackers.begin(), g_load_stackers.end()
												   , [] ( CTaskAutoStacker* _stacker ) -> bool
	{
		// stacker에 트레이 있으면, 바로 return true;
		// stacker에 트레이 없지만, eStacker_Conveyor_State::ReadyToUnload 아니라면 load 가능한 device 있음. return true;
		// Load Stacker 조건 
		int stacker_idx = _stacker->GetStackerIndex();

		if(g_TaskStacker[stacker_idx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
			return true;

		if( g_TaskStacker[ stacker_idx ].GetEmptyTray() == false )
			return true;

		if( _stacker->GetConveyorState() == PickEnable_Device )
			return true;

		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
		if( nDeviceMode == eNoDeviceNoTray && g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == false )
		{
			return true;
		}

		if (nDeviceMode == eNoDeviceNoTray)
		{
			if (g_TaskStacker[stacker_idx].GetExistTray_FrontPos_Sen(DEF_NOT_EXIST) != ERR_NO_ERROR)
			{
				return true;
			}
		}

		bool bBtmExist = g_DMCont.m_dmDevice.GetTrayExist( eDeviceStage_STACKER_BTM_1 + stacker_idx ) == DEF_EXIST;
		bool bConvExist = g_DMCont.m_dmDevice.GetTrayExist( eDeviceStage_STACKER_CONV_1 + stacker_idx ) == DEF_EXIST;
		if( bBtmExist || bConvExist ) {
			return true;
		}

		// Tray Feeder 조건 
		bool feeder_tray_empty_device = g_TaskTrayFeeder.IsEmptyDevice();
		if (feeder_tray_empty_device == false)
			return true;

		return false;
	} );

	return load_enable_stacker_count;
}


int CAllAutoStacker::GetDevicePickEnableStackerCount()
{
	int pick_enable_stacker_count = std::count_if( g_load_stackers.begin(), g_load_stackers.end()
												   , [] ( CTaskAutoStacker* _stacker ) -> bool
	{
		if( _stacker->GetConveyorState() == PickEnable_Device )
			return true;

		return false;
	} );

	return pick_enable_stacker_count;
}

int CAllAutoStacker::GetSeqCmdStatusAllPart()
{
	int status = CBaseSeqCmd::eSeqCmdStatus_Ready;
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		int status = g_TaskStacker[ i ].GetSeqCmdStatus();
		if( status != CBaseSeqCmd::eSeqCmdStatus_Ready )
			return status;
	}

	return status;
}

int CAllAutoStacker::GetCurrentLotState()
{
	int nCurrentState = eAutoLotState_Unknown;
	for( int i = 0; i < eMaxStackerCount - 1; i++ ) {
		int nState = g_TaskAutoStacker[ i ].GetCurrentLotState();
		int nStateNext = g_TaskAutoStacker[ i + 1 ].GetCurrentLotState();
		if( nState != nStateNext ) {
			return eAutoLotState_Unknown;
		}
		nCurrentState = nState;
	}

	return nCurrentState;
}

int CAllAutoStacker::OnLotCommand( eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam )
{
	int nErr = ERR_NO_ERROR;
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		nErr |= g_TaskAutoStacker[ i ].OnLotCommand( eLotCmd, wParam, lParam );
	}

	return nErr;
}

//TRNASFER & UNLOADTRAYPP
bool _ST_CONVEYOR_WORK_AREA::is_work_enable_state( CBaseTaskManager* _this )
{
	bool bTransfer_Safety = _this == &g_TaskAutoTransfer ? true : g_TaskTransfer.is_safety_state();	
	//bool bTrayfeeder = _this == &g_TaskAutoTrayFeeder ? true : g_TaskTrayFeeder.is_safety_state();
	bool bTrayPP_Safety2 = _this == &g_TaskAutoTrayUnloadPP ? true : g_TaskTrayUnloadPP.is_safety_state();

	if( bTransfer_Safety == true /*&& bTrayfeeder == true*/ && bTrayPP_Safety2 == true && bWorking == false ) {
		return true;
	}

	return false;
}

void _ST_CONVEYOR_WORK_AREA::release_work_area( CBaseTaskManager* _this )
{
	cs.Lock();

	if( m_reserved_task == _this ) {
		m_reserved_task = nullptr;
		bWorking = false;
	}


	cs.Unlock();
}

bool _ST_CONVEYOR_WORK_AREA::reserve_work_area( CBaseTaskManager* _this )
{
	cs.Lock();

	bool bRet = false;


	if( is_work_enable_state( _this ) == true ) {
		m_reserved_task = _this;
		bWorking = true;
		bRet = true;
	}
	else if( bWorking == true && m_reserved_task == _this ) {
		bRet = true;
	}

	cs.Unlock();

	return bRet;
}

CBaseTaskManager * ST_CONVEYOR_WORK_AREA::get_reserved_task()
{
	return m_reserved_task;
}



//TRNASFER & FEEDER
bool _ST_TRANSFER_FEEDER_WORK_AREA::is_work_enable_state(CBaseTaskManager* _this)
{
	bool bTransfer_Safety = _this == &g_TaskAutoTransfer ? g_TaskTrayFeeder.reserve_is_safety_state() : g_TaskTransfer.is_safety_state();

	if (bTransfer_Safety == true && bWorking == false) {
		return true;
	}

	return false;
}

void _ST_TRANSFER_FEEDER_WORK_AREA::release_work_area(CBaseTaskManager* _this)
{
	cs.Lock();

	if (m_reserved_task == _this) {
		m_reserved_task = nullptr;
		bWorking = false;
	}


	cs.Unlock();
}

bool _ST_TRANSFER_FEEDER_WORK_AREA::reserve_work_area(CBaseTaskManager* _this)
{
	cs.Lock();

	bool bRet = false;


	if (is_work_enable_state(_this) == true) {
		m_reserved_task = _this;
		bWorking = true;
		bRet = true;
	}
	else if (bWorking == true && m_reserved_task == _this) {
		bRet = true;
	}

	cs.Unlock();

	return bRet;
}

CBaseTaskManager * _ST_TRANSFER_FEEDER_WORK_AREA::get_reserved_task()
{
	return m_reserved_task;
}




//TESTPP & TABLE
void _ST_TESTPP_TABLE_WORK_AREA::release_work_area(CBaseTaskManager* _this)
{
	cs.Lock();

	if (m_reserved_task == _this) {
		m_reserved_task = nullptr;
		bWorking = false;
	}


	cs.Unlock();
}

bool _ST_TESTPP_TABLE_WORK_AREA::reserve_work_area(CBaseTaskManager* _this)
{
	cs.Lock();

	bool bRet = false;


	if (m_reserved_task == nullptr) {
		m_reserved_task = _this;
		bWorking = true;
		bRet = true;
	}
	else if (bWorking == true && m_reserved_task == _this) {
		bRet = true;
	}

	cs.Unlock();

	return bRet;
}

CBaseTaskManager * _ST_TESTPP_TABLE_WORK_AREA::get_reserved_task()
{
	return m_reserved_task;
}
