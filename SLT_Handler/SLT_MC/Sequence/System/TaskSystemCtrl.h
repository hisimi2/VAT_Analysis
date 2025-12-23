#pragma once

#include "LotInfo.h"
#include <deque>
#include <vector>
#include "DEF_COMMON.h"


class CBaseTaskManager;

class CTaskSystemCtrl : public CBaseTaskManager
{
public:
	enum eDI{
		//Main Air
		MAIN_AIR_PRESS_TEST_SEN = 0,
		MAIN_AIR_STACKER_CTRAY_SEN,

		//Ionizer
		IONIZER_1_ALARM,
		IONIZER_1_STATE,
		IONIZER_2_ALARM,
		IONIZER_2_STATE,
		IONIZER_3_ALARM,
		IONIZER_3_STATE,
		IONIZER_4_ALARM,
		IONIZER_4_STATE,
		IONIZER_5_ALARM,
		IONIZER_5_STATE,
		IONIZER_6_ALARM,
		IONIZER_6_STATE,
		IONIZER_7_ALARM,
		IONIZER_7_STATE,
		IONIZER_8_ALARM,
		IONIZER_8_STATE,
		PRESS1_IONIZER_ALARM,
		PRESS1_IONIZER_STATE,
		PRESS2_IONIZER_ALARM,
		PRESS2_IONIZER_STATE,
		PRESS3_IONIZER_ALARM,
		PRESS3_IONIZER_STATE,
		PRESS4_IONIZER_ALARM,
		PRESS4_IONIZER_STATE,
		PRESS5_IONIZER_ALARM,
		PRESS5_IONIZER_STATE,
		PRESS6_IONIZER_ALARM,
		PRESS6_IONIZER_STATE,
		PRESS7_IONIZER_ALARM,
		PRESS7_IONIZER_STATE,
		PRESS8_IONIZER_ALARM,
		PRESS8_IONIZER_STATE,

		//Servo CP
		TEST1_PRESS1_SERVO_CP_TRIP_ALARM_SEN,
		LOAD_UNLOAD_TRANSFER_SERVO_CP_TRIP_SEN,
		TABLE_SERVO_CP_TRIP_SEN,
		TEST2_PRESS2_CP_TRIP_SEN,

		//Front Op Switch
		START_SW_SEN1,
		START_SW_SEN2,
		STOP_SW_SEN,
		COVER_SW_SEN,
		SOUND_SW_SEN,
		LAMP_SW_SEN,
		TRAY_FEEDER_STEP_SW,
		TRAY_LOADER_HAND_STEP_SW,
		TRAY_UNLOADER_HAND_STEP_SW,

		//door
		FRONT_LEFT_DOORLOCK_SEN,
		FRONT_RIGHT_DOORLOCK_SEN,
		HINGED_RIGHT_DOORLOCK_SEN,
		SLIDE_RIGHT_F1_DOORLOCK_SEN,
		SLIDE_RIGHT_R2_DOORLOCK_SEN,
		REAR_RIGHT_DOORLOCK_SEN,
		REAR_LEFT_DOORLOCK_SEN,
		SLIDE_LEFT_R2_DOORLOCK_SEN,
		SLIDE_LEFT_F1_DOORLOCK_SEN,
		HINGED_LEFT_DOORLOCK_SEN,
		FRONT_LEFT2_DOORLOCK_SEN,
		ALL_DOOR_LOCK_SEN,

		//EMO
		REAR_LEFT_EMO_SW_SEN,
		FRONT_LEFT_EMO_SW_SEN,
		FRONT_RIGHT_EMO_SW_SEN,
		REAR_RIGHT_EMO_SW_SEN,

		MIDDLE_LEFT_EMO_SW_SEN,   //4호기 부터 적용
		MIDDLE_RIGHT_EMO_SW_SEN,   //4호기 부터 적용

		//POWER CP
		MAIN_POWER_ON_SEN1,
		MAIN_POWER_ON_SEN2,
		POWER_BOX_CP_TRIP_ALARM_SEN,
		SERVO_POWER_ON_SEN1,


		REAR_LEFT_START_SW_SEN1,
		REAR_LEFT_START_SW_SEN2,
		REAR_LEFT_STOP_SW_SEN,
		REAR_LEFT_COVER_SW_SEN,
		REAR_LEFT_SOUND_SW_SEN,
		REAR_LEFT_LAMP_SW_SEN,
		SELECT_FRONT_SW_SEN,
		SELECT_REAR_SW_SEN,

		REAR_RIGHT_START_SW_SEN1,
		REAR_RIGHT_START_SW_SEN2,
		REAR_RIGHT_STOP_SW_SEN,
		REAR_RIGHT_COVER_SW_SEN,
		TEST_HAND1_STEP_SW,
		TEST_HAND2_STEP_SW,

		AC_FAN_CP_TRIP_SEN,

        xMAX_COUNT,
	};

	enum eDO{
		//Front OP Switch
		START_SW_LAMP = 0,
		STOP_SW_LAMP,
		COVER_SW_LAMP,
		SOUND_SW_LAMP,
		LAMP_SW_LAMP,
		TRAY_FEEDER_STEP_SW_LAMP,
		TRAY_LOADER_HAND_STEP_SW_LAMP,
		TRAY_UNLOADER_HAND_STEP_SW_LAMP,

		//door
		FRONT_LEFT_DOORLOCK,
		FRONT_RIGHT_DOORLOCK,
		HINGED_RIGHT_DOORLOCK,
		SLIDE_RIGHT_F1_DOORLOCK,
		SLIDE_RIGHT_R2_DOORLOCK,
		REAR_RIGHT_DOORLOCK,
		REAR_LEFT_DOORLOCK,
		SLIDE_LEFT_R2_DOORLOCK,
		SLIDE_LEFT_F1_DOORLOCK,
		HINGED_LEFT_DOORLOCK,

		FRONT_LAMP_ON,
		TEST_SITE_LAMP_ON,
		COVER_RESET,
		SERVO_POWER_ON,

		REAR_LEFT_START_SW_LAMP,
		REAR_LEFT_STOP_SW_LAMP,
		REAR_LEFT_COVER_SW_LAMP,
		REAR_LEFT_SOUND_SW_LAMP,
		REAR_LEFT_LAMP_SW_LAMP,
		REAR_RIGHT_START_SW_LAMP,
		REAR_RIGHT_STOP_SW_LAMP,
		REAR_RIGHT_COVER_SW_LAMP,
		REAR_RIGHT_SOUND_SW_LAMP,
		REAR_RIGHT_LAMP_SW_LAMP,

		TEST_HAND1_STEP_SW_LAMP,
		TEST_HAND2_STEP_SW_LAMP,

		TOWER_RED,
		TOWER_YELLOW,
		TOWER_GREEN,

		BUZZER1,
		BUZZER2,
		BUZZER3,
		BUZZER4,
		
		yMAX_COUNT,
	};

	enum _eMainPowerRecoveryState
	{
		eIdle = 0,				// 아무 동작도 안하고 있을 때
		eMainPowerDown,			// Main Power Up -> Down으로 변경되었을 때
		eMainPowerInRecovery_WMX,	// Main Power 복구 후 (Down -> Up) 의 처리 중일 때
		eMainPowerInRecovery_MotorPicker,	// Main Power 복구 후 (Down -> Up) 의 처리 중일 때

		eCPTripOff,			// CP Trip On-> Off으로 변경되었을 때
		eCPTripOn,			// CP Trip Off-> On으로 변경되었을 때
	};


	int ChkComInterlock();

public:
	CTaskSystemCtrl(void);
	~CTaskSystemCtrl(void);

	int Initialize(char* szDriverFileName);
	void Finalize();

	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Add_Load_Tray_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_OneCycle_Cmd(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_AutoGrr_OneCycle_Cmd(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
    void OnRcv_Req_Stacker_For_Back_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
    void OnRcv_Req_Tray_End_Feed(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_AutoHeightCal_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_ZPickerAxisHeightCal_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	
	void OnRcv_Req_2D_Duplicate_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Lot_End(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_TrayPp_Device_Loss(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_2D_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_2D_Mark_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	
	void OnRcv_Req_OffLine_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_OnLineMode_Use_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_TestPp_Device_Loss(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Vision_Off_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Device_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Retest_Skip_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Test_Time_Out_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam, int nStationID);
	void OnRcv_Req_Press_Arrive_Sensor_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Remain_AutoSkipDevice_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_DisConnectionStation_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_SocketCleaning_Dvc_Clear_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Auto_Retest_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_CB_Modify_Time_Check_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_CB_Data_OnOff_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_CB_File_Not_Exist_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_GRR_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_LOOP_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_AUDIT_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_QA_Mode_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	void OnRcv_Req_Switch_Tray_Feeder_Ack(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	
		
	int  GetAutoHeightCalSelect();
	void SetAutoHeightCalSelect(int nSelectType);

	int  GetZPickerAxisHeightCalSelect();
	void SetZPickerAxisHeightCalSelect(int nSelectType);

	void ActionTowerAndSound(int kinds);
	void DeActionTowerAndSound(int kinds);

	int OnLotCommand(eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam);
	int CheckOnlineTesterInfo();
	int CheckEqpConditionForStart();
	int CheckEqpConditionForFirstStart();
    int CheckEqpSocketSite();
	int SetSignalTowerDataFromSM();
	int CheckEqpConditionForMultiOrg();
	int Check2dMarkDeviceName();

	int OnPanelButtonStart();
	int OnPanelButtonStop();

	// Lot ���� ����
	int GetCurrentLotState();
	void SetCurrentLotState(int nLotState);

    // Socket Cleaning
	BOOL ModuleSiteOffCheck(int nIdx);

	//Manual ���� ����
	int CheckManualable(BOOL bManualMode=FALSE);

	CString CheckCategoryInterlock();
	//Default Bin ������ Error String �� Category File ���ִ��� Ȯ�� 
	BOOL CompareDefaultBinToCategory();

	CTimerTickCheck m_tcUPH;        // EQP all time add
	CTimerTickCheck m_tcStop;       // EQP stop time

	CTimerTickCheck m_tcCycleUPH;        // EQP all time add

	double          m_dTotalStopTm; // sec
	double			m_dTotalRunTm;
	int   CalcUPH();	// calc UPH
	void  UpdateStopRunTm();
	void  ResetUPHtm();

	void UpdateCycleUPH();

	CTimerTickCheck m_tIndexTime[eMaxPressUnitCount];
	double m_dBestTestTime;	// best short pass bin test time history
	void SetAutoGrrUse(BOOL bSet) { m_bAutoGrrUse = bSet; }
	BOOL GetAutoGrrUse() { return m_bAutoGrrUse; }
	void SetAutoGrrCancel( bool bSet );
	BOOL GetAutoGrrCancel();

	void SetAutoLoopUse(BOOL bSet) { m_bAutoLoopUse = bSet; }
	BOOL GetAutoLoopUse() { return m_bAutoLoopUse; }
	void SetAutoLoopCancel(bool bSet);
	BOOL GetAutoLoopCancel();

	void SetAutoQAUse(BOOL bSet) { m_bAutoQAUse = bSet; }
	BOOL GetAutoQAUse() { return m_bAutoQAUse; }

	int ChkAllSiteOffGrr();

	void LotInfoDataClear();
	BOOL IsLotInfoEmpty();
	void MakeLog(LPCTSTR fmt, ...);
	CString m_strLastDuplicate;
	//void CalcYieldVarianceMonitoring();
	//void YieldVarianceClear();
	//void YieldVarianceClear(int nIdx, int nX, int nY);
	//void AddYieldVariance(int nIdx, BOOL bPassFailData, int nX, int nY);

	void CalcYieldSampleMonitoring(int nPressIndex, std::vector<CPoint> &vAlarmSet);
	void YieldSampleClear();
	void YieldSampleClear(int nIdx, int nX, int nY);
	void AddYieldSample(int nIdx, BOOL bPassFailData, int nX, int nY);

	void CalcYieldContiFailMonitoring(int nPressIndex, std::vector<CPoint> &vAlarmSet);
	void YieldContiFailClear();
	void YieldContiFailClear(int nIdx, int nX, int nY);
	void AddYieldContiFail(int nIdx, BOOL bPassFailData, int nX, int nY);
	void CalcYieldContiFailMonitoring_Station(int nPressIndex, std::vector<CPoint> &vAlarmSet);
	

	void CalcYield1stFailMonitoring(int nPressIndex, std::vector<CPoint> &vAlarmSet);
	void AddYield1stFailSample(int nIdx, BOOL bPassFailData, int nX, int nY, int nTestCnt = -1);
	void Yield1stFailClear();
	void Yield1stFailClear(int nIdx, int nX, int nY);

	void SetErrTestPpIdx(int nIdx, int nData){ m_nErrTestPpIdx[nIdx] = nData;}
	int  GetErrTestPpIdx(int nIdx){ return m_nErrTestPpIdx[nIdx]; }

	int	 GetErrTestPPType(int nIdx);

	static void FtpErrorCallBack(CString strErrMsg);

	void SetUseUnloadSetplate(ST_CATE_INFO BinCategory);

	bool IsOneCycleModeOn();
	void SetOneCycleMode( int _OnOff );

	void SetCleanDeviceSkipOneCycleMode(int _OnOff);
	bool IsCleanDeviceSkipOneCycleModeOn();


	BOOL m_2DBarcodeUse;

	BOOL ChkVisionPosResult(CString strPosResult, CString& strErrMsg, std::vector<CPoint> vPicker, CPoint cTarget, std::vector<CPoint>& vFailPocket, BOOL *bVisionErrSkip = NULL);

	int CheckAutoRetestRuleSite(int nStationNo, int nSiteX, int nSiteY);

	bool checkUphMode();
	CCriticalSection m_csHeartBeat;

	CCriticalSection m_csDailyReport;
	void UpdateDailyReport();
	void AddDailyReportData(ST_DAILY_REPORT Data);

	BOOL IsDefaultBin(ST_DD_DEVICE stDvcData);
	
	BOOL IsRunningFinish();

	BOOL CheckTesterDisConnection(int& nIdx, std::vector<int> vDisConnectionStationIdx);
	BOOL IsPossibleStationOff(int nPressUnit);
	void SetDisConnectionStationIdx(int nPressUnit);
	std::vector<int> GetDisConnectionStationIdx();
	void ClearDisConnectionStationIdx();

	CString GetHistoryStageName(int nCmdStage);

private:
    int m_nCleanIntervalCnt[eMaxPressUnitCount];
    ST_SOC_CLEAN_DATA m_stTestedBin[eMaxPressUnitCount];
	int m_nRecSortCnt;

	BOOL m_bAutoGrrUse;			// Auto GRR �������
	BOOL m_bAutoLoopUse;
	BOOL m_bAutoQAUse;

	//Site SocketInterlock
	int  m_nSiteSocketOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][ STATION_MAX_X_SITE ];
	void GetSiteSocketStatusFromDM();
	BOOL ChkAllStationOFF();
	int ChkSocketOnOffInterlock(CParamList &AlarmData);
	int ChkSocketOnOffInterlock_GRR(CParamList &AlarmData);
	int ChkSocketOnOffInterlock_AUDIT(CParamList &AlarmData);
	BOOL Chk8StationSocketOff(std::vector<int> vDown, std::vector<int> vUp);
	BOOL ChkEachOneMoreSocketOn(std::vector<int> vDown, std::vector<int> vUp);
	BOOL ChkRetestTwoMoreStaionOn(std::vector<int> vDown, std::vector<int> vUp);
	void ChkErrEachOneMoreSocketOn(std::vector<int> vDown, std::vector<int> vUp, std::vector<int> &vSocket);
	void ChkEachOneMoreSocketState(int _Downstation,int _Upstation, int& _Socket_number);
	void ChkRetestTwoMoreStaionState( int _index, std::vector<int> _vDown, std::vector<int> _vUp, int& _DownStation, int& _UpStation);
	void ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vDown, std::vector<int> _vUp, int &_Socket );

	//std::deque<BOOL>  m_dqVarPF_Yield[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	//std::vector<BOOL> m_vVarPF_Yield_PreSet[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];

	std::deque<BOOL>  m_dqSamplePF_Yield[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	std::vector<BOOL> m_vSamplePF_Yield_PreSet[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];

	std::deque<BOOL>  m_dqContiFail[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];

	std::deque<BOOL>  m_dq1stFail_Yield[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	

	std::vector<int> m_vModuleStationNo[eMaxTestPPCount];

	std::deque<ST_DAILY_REPORT> m_dqDailyData;
	std::vector<int> m_vDisConnectionStationIdx;

protected:
	void ThreadProc1();
	void ThreadProc2();
	void ThreadProc3();

	int ReinitializeAxis(eAxisServoConfiguration Configuration);
	void Switch_Action();
	
	//Site Socket Interlock
	
	//==============================================================================
	// system i/o
	//==============================================================================
	// emo
	void _Check_Sig_EMO();
	std::vector<int>					m_vEMO_Sig;
	CTimerTickCheck                     m_tcEMO;

	// main power
	void _Check_Sig_MainPow();
	std::vector<int>                    m_vMainPow_Sig;
	CTimerTickCheck                     m_tcMainPow;

	// servo power
	void _Check_Sig_ServoPow();
	std::vector<int>                    m_vServoPow_Sig;
	CTimerTickCheck                     m_tcServoPow;

	// ion
	void _Check_Sig_Ion_Alarm();
	std::vector<int>                    m_vIonAlarm_Sig;
	CTimerTickCheck                     m_tcIonAlarm;
	// ion fan
	void _Check_Sig_Ion_Fan_Alarm();
	std::vector<int>                    m_vIonFanAlarm_Sig;
	CTimerTickCheck                     m_tcIonFanAlarm;

	// CP Trip
	void _Check_Sig_CPTrip();
	std::vector<int>                    m_vCPTrip_Sig;
	CTimerTickCheck                     m_tcCPTrip;

    // Down Door
    void _Check_Sig_DownDoor();
    std::vector<int>                    m_vDownDoor_Sig;
    CTimerTickCheck                     m_tcDownDoor;

	// Regulator
	void _Check_Regulator();
	CTimerTickCheck						m_tcChkMainAIRPressProblemForEP_Reg;
	CTimerTickCheck                     m_tcRegulator;
	int                                 m_nRetryRegulator;

	//// Main Air Ejector
	// Press & Test main air
	void _Check_PressTestMainAir();
	std::vector<int>                    m_vMainAir_PressTest_Sig;
	CTimerTickCheck                     m_tcMainPressTestAir;

	// Stacker & Ctray main air
	void _Check_StackerCtrayMainAir();
	std::vector<int>                    m_vMainAir_StackerCtray_Sig;
	CTimerTickCheck                     m_tcMainStackerCtrayAir;

	// Down Door
	void _Check_Sig_VisionBoardReset();
	std::vector<int>                    m_vVisionBoardReset_Sig;
	CTimerTickCheck                     m_VisionBoardReset;

	CTimerTickCheck                     m_tcMainPowerRecoveryWait;
	
	void _Check_Switch_Mode();
	BOOL								m_bFrontMode;

	//Handler OS Version Ȯ��.
	void _Check_Recent_OS_Version();

	void _Check_HddRemainSize();
	//Heart Beat
	void _Check_Online_Heart_Beat();	

	void _Check_Online_ReqTesterInformation();
	void _Check_Online_ReqTesterTestMode();
	void _Check_Online_ReqTesterAPLInfo();

	void System_Signal_AlarmReport(std::vector<int>& a_vSignal, BOOL a_bSigStateForAlarm, int a_nAlarmId, BOOL bType = eSensor_TypeA);
	// EMO


	void On_Emo_Switch_On(CParamList* pDataList);
	void On_Emo_Switch_Off(CParamList* pDataList);
	// Door Lock
	void On_DoorLock_Switch_On(CParamList* pDataList);
	void On_DoorLock_Switch_Off(CParamList* pDataList);
	// Main Power
	void On_MainPower_Switch_On(CParamList* pDataList);
	void On_MainPower_Switch_Off(CParamList* pDataList);
	// Site Slide
	void On_SiteSlide_On(CParamList* pDataList);
	void On_SiteSlide_Off(CParamList* pDataList);
	// Start
	void On_Start_Switch_On(CParamList* pDataList);
	// Stop
	void On_Stop_Switch_On(CParamList* pDataList);
	// Cover
	void On_Cover_Switch_On(CParamList* pDataList);
	void On_Cover_Switch_Off(CParamList* pDataList);
	// Sound Switch
	void On_Sound_Switch_Down(CParamList* pDataList);
	void On_Sound_Switch_Up(CParamList* pDataList);
	// Lamp
	void On_Lamp_Switch_On(CParamList* pDataList);
	void On_Lamp_Switch_Off(CParamList* pDataList);
	// Servo Power
	void On_ServoPower_Switch_On(CParamList* pDataList);
	void On_ServoPower_Switch_Off(CParamList* pDataList);
	// ION Alarm
	void On_IonAlarm_On(CParamList* pDataList);
	void On_IonAlarm_Off(CParamList* pDataList);
	// ION Fan Alarm
	void On_IonFanAlarm_On(CParamList* pDataList);
	void On_IonFanAlarm_Off(CParamList* pDataList);
	// Under stacker load
	void On_UndStkLoad_Switch_On(CParamList* pDataList);
	void On_UndStkLoad_Switch_Off(CParamList* pDataList);
	// Under stacker unload
	void On_UndStkUnload_Switch_On(CParamList* pDataList);
	void On_UndStkUnload_Switch_Off(CParamList* pDataList);

	void _Check_HandlerTesterModePopup();

	//==============================================================================
	// Signal Tower
	//==============================================================================
	POSITION GetTowerActionCheck(int kinds);
	void DeleteActionTower();
	void ActTower();
	void do_Buzzer(int nBuzAction);	

private:
	CCriticalSection m_csTowerAction;
	CCriticalSection m_csSiteKill;
	CList <int, int&> m_intList;

	CCriticalSection m_csYieldSample;
	CCriticalSection m_csYieldContiFail;
	CCriticalSection m_csYield1stFail;

	CTimerTickCheck  m_tcNoDeviceCheck;

	//==============================================================================
	// Lot Handling
	//==============================================================================
public:
	// Lot Info
	int			AddLotInfo(CLotInfo& a_LotInfo);
	int			DelLotInfo(CString strLotNo);
	CLotInfo*	GetLotInfo(CString strLotNo);
	void		ClearLotInfo();
	int			GetLotInfoCount();
	void		LotDataClear();
	void		LotTesterModeText(CString& strHandlerMode, CString& strMode);
	int			GetTestModeStringToint(CString strTestMode);
	CString		GetTestModeintToString(int nTestMode);
	CString		GetLotCmdString(int nLotCmd);

	// Make Summary
	int			MakeSummary(BOOL bIsGrrMode = FALSE);
	CString		CheckMakeSummary();
	CString		ScopesInterlock();
	int			MakeSummaryGetTrayEndFeed() { return m_nLotEndTrayEndFeed; }//DEF_LOTEND_UNK
	void		MakeSummarySetTrayEndFeed(int nEndFeed){ m_nLotEndTrayEndFeed = nEndFeed; };	
	int			SummaryNetworkDriveInterlock();
	int			SummaryNetworkDriveInterlockCheck(char* szFilePath);

	// Socket ID
	BOOL		AddSocketIDContactCnt(CString strSocketID,int nStationNum =0);
	BOOL		MakeSocketIDContactCnt();
	void		ClearSocketIDContactCnt();
	int			GetSocketIDContactCnt();

	// Site Setting
	void		LoadSiteOnOffData();
	void		SetSiteOnOffData(int nStation, std::vector<CPoint> vPocket, int nSiteOnOff);
	void		SetSiteOnOffData(int nStation, std::vector<CPoint>vPocket, std::vector<int> vSiteOnOff);
	void		ChangeSiteOffData();

	// Socket Clean
	int			SocketCleanInterlock();
	int			SocketCleanDevCnt(int nIdx);

	// Motor Reconnection
	int			GetWMX3ReconnectSts();
	void		SetWMX3ReconnectSts(int nSts);
	int			GetMainPower();
	int			GetCPTrip();
	int			ReConnectSoftServoDriver(eAxisServoConfiguration Configuration);

	// QA Mode
	void		SetQAStatus(eQAStatus newStatus);
	int			GetQAStatus();
	void		ChkFeederTrayEnd(BOOL bTrayEmptyCheck);
	int			GetQASampleCount();
	int			CalcQaSampleAddLoadCount(int nQaSampleCnt, int nCurPassCnt);
	void		UpdateQaSampleLoadCount(int nQaSampleAddLoadCount);
	void		QaProcessLoad(int nQaSampleCnt);
	void		QaProcessLoadEnd(int nQaSampleCnt);
	void		QaProcessLoadEmpty();
	// Motor Jog move ���� �Լ�
	public:
		int MoveJog(int nAxis, int nMoveDir, double nMoveSpeed/*um/s*/);
		int StopJog();
		int MoveInitial(int nAxis);

protected:
	CList<CLotInfo*,CLotInfo*>  m_LotInfoList;
	CCriticalSection m_csLotInfo;



	//==============================================================================
	//
	//==============================================================================
private:
	std::vector<int>                   m_vX;           // DI id list
	std::vector<int>                   m_vY;           // DO id list

	CMap<int,int,CString,CString>      m_mapDI;
	CMap<int,int,CString,CString>      m_mapDO;

	ST_SLT_SIGNAL_TOWER  m_stTowerSetting;
	int m_nAutoHeightCalSelect;
	int m_nZPickerAxisHeightCalSelect;
	int m_nErrTestPpIdx[eMaxTestPPCount];
	int m_nLotEndTrayEndFeed;
	ST_SOCKET_CONTACT_COUNT			   m_SocketCountCnt;
	int  m_nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	int m_nWM3ReconnectSts;

};

