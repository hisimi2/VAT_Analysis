/********************************************************************
	created:	2015/07/24
	created:	24:7:2015   15:17
	filename: 	SLT_Handler\Include\DEF_COMMON.h
	file path:	SLT_Handler\Include
	file base:	DEF_COMMON
	file ext:	h
	author:		나명진
	
	purpose:	모든 프로젝트에서 공통적으로 사용하는 매크로 모음
*********************************************************************/
#pragma  once

#include <vector>
#include <afx.h>
#include <afxmt.h>
#include <initializer_list>
#include <string>
#include <algorithm>
// 20230801 hyb WMX3 리커넥트 기능
enum _eCommReconnect : int
{
	eCommIdle = 0,
	eCommReconnectProcessing_WMX,
	eCommReconnectProcessed_WMX,
	eCommReconnectProcessing_MotorPicker,
	eCommReconnectProcessed_MotorPicker,
	eCommReconnectOK,

	eSLTMC_Disconnect=99,
};





#define DEF_TESTER_COMM_SPEC_ONE_SOCKET	(1)	 // 20230824 hyb DUT별 소켓 통신

#define EACHSOCKET	(eEachTesterSocket::eMaxTesterForYanmade)
#define EACHSTATION	(2)


enum _eSetTesterInformation		// 테스터로 부터 Regstaraition 수신 후 Tester Information 설정 후 셋팅 진행.
{
	_TesterInformationIdle = 0,
	_TesterInformationRegReceiveOK,
	_TesterInformationReq,
	_TesterInformationOK,
	_TesterInformationNeedResetSend,
	_TesterInformationNeedResetOK,
	_TesterScriptInformationNeedResetSend,
	_TesterScriptInformationNeedResetOK,
	//Watt 패키지 추가 사항
	_TesterScriptHeartBeatNeedResetSend,
	_TesterScriptHeartBeatNeedResetOK,
	_TesterAPLInfoSend,
	_TesterAPLInfoSendResetOK,
};


#define MAX_THREAD_SCAN_COUNT		(1)
#define DEF_SOCKET_CLEAN_DEVICE      "_SC_CLR"
#define DEF_2D_READ_ERR_DEVICE       _T("2D_READ_ERROR")
#define DEF_HANDLER_ERR_DEVICE		 _T("HE")
#define DEF_TEST_TIME_OUT_ERR_DEVICE _T("TEST_TIME_OUT")
#define DEF_TEST_RESULT_TP101		 _T("TP101")
#define DEF_TEST_RESULT_TP100        _T("TP100")
#define DEF_TEST_RESULT_TP103        _T("TP103")
#define DEF_NEW_LOAD_DEVICE          _T("99")
#define DEF_TRAY_PICK_ERR_DEVICE	 _T("20")
#define DEF_PASS_DEVICE				_T("PASS")
#define DEF_FAIL_DEVICE				_T("FAIL")
#define DEF_TEST_ALARM_DEVICE		 _T("TEST_ALARM")
#define DEF_TEST_RESULT_WRONG_APN _T("WRONG_APN")


#define DEF_BIN_SOLUTION_FAIL_SW_BIN		 _T("2")
#define DEF_BIN_SOLUTION_FINAL_REJECT_BIN	 _T("final reject bin#")

#define DEF_IO_CHECK_MODE			(0)
#define DEF_TEST_PP_PICKER_DOWN_CHECK_IGNORE_MODE			(0)

#define DEF_COMPANY_AMKOR            _T("AMKOR")

//TWSL421 PKG
#define DEF_PROJECT_LOWLAND				0
#define DEF_PROJECT_CAMPBELTOWN			1
#define DEF_PROJECT_WATT				2
#define DEF_PROJECT_CARPENTERS			3

#define DEF_TESTER_SINGLE				0		   //Single Tester
#define DEF_TESTER_MULTI				1		   //Multi Tester
#define DEF_TESTER_AGENT				2          //Agent Tester


#define DEF_EQUIPMENT_UNIT1				1			//1호기
#define DEF_EQUIPMENT_UNIT2_3			2			//2호기 ~ 3호기

#define DEF_EQUIPMENT_UNIT4_N			4			//4호기 ~ n호기

#define DEF_USE							_T("1")
#define DEF_NOT_USE					_T("0")

#define	LOG_MAX_BUFF						2500

#define MAX_TESTERID_CNT				8

#define DEBUG_SERVER					0

#define EACH_LINE_STATION_CNT			4

#define DEF_STATION_8PARA					0
#define DEF_STATION_4PARA					1

#define DEF_TESTER_HEAP_POINTER_DELETE ((void *)0xdddddddd)
#define DEF_TESTER_POINTER_DELETE ((void *)0xcdcdcdcd)
//====================================================
enum eSortingType{
    eSortType_SwBin = 0,
    eSortType_ErrStr,
};
//====================================================

//====================================================
enum eBoardDirection {
	eBoardDir_Forward = 0,
	eBoardDir_Backward
};


enum eNoInterLock{
	eNoInterLock_OFF = 0,
	eNoInterLock_ON,
};
//====================================================




//==============================================================================
// MACRO CONST
//==============================================================================
#define DEF_VERSION_NAME                _T("Demo")
#define SZ_DRIVER_FILE_NAME             _T("TW_LS_DRIVER.INI")
#define SZ_CUST_DVC_PGM_FILE_NAME       _T("TW_LS_SETUPLIST.INI")
#define SZ_SLT_SYS_FILE_NAME	        _T("SLT_SYSTEM.INI")	// Last Version
#define SZ_TEACHING_FILE_NAME           _T("Teaching.dat")   // sys_data/cok_name/Teaching.dat
#define SZ_HANDLER_RCP_FOLDER           _T("HandlerRecipe")
#define SZ_TRAY_FOLDER			        _T("TrayFiles")
#define SZ_CATEGORY_FOLDER		        _T("Category")
#define SZ_SLT_PGM_FOLDER		        _T("SLT_PGM")
#define SZ_DM_DEVICE_DATA               _T("SLT_DEVICE_DATA_MEM")  // Device Data Map File
#define SZ_DIR_PATH_OEE_DATA            _T("D:\\Techwing\\OEE")
#define SZ_DIR_PATH_JAMRATE             _T("D:\\techwing\\JamRate")
#define SZ_DIR_PATH_SUMMARY             _T("D:\\Techwing\\Summary")
#define SZ_DIR_PATH_TEST_LOG_DATA		_T("D:\\ProductCSV")
#define SZ_DIR_PATH_TCPIP_LOG_DATA	    _T("D:\\TechWing\\Logs\\TCPIP")
#define SZ_FTP_FILE_NAME				_T("FTP_PATH.ini")
#define MAC_FILE_COPY_PATH_CSV          _T("D:\\TesterData")
#define MAC_FILE_COPY_PATH_LOG          _T("D:\\TesterLogData")
#define MAC_FILE_UPDATE_TMP_PATH		_T("D:\\TesterLogZipData")
#define SZ_FILE_PATH                    _T("FILE_PATH.INI")
#define SZ_VAT_PKG_FILE_NAME			_T("VisionAT.dat")
#define BACKLASH_LOG_PATH				_T("D:\\techwing\\LOG\\")
#define SZ_CAPUTRE_PATH					_T("D:\\techwing\\Capture")
#define SZ_DIR_PATH_HANDLER_LOG			_T("D:\\TechWing\\HandlerLog")
#define SZ_DIR_PATH_HANDLER_LOG_BACKUP  _T("D:\\TechWing\\HandlerLog_BackUp")
#define SZ_DIR_PATH_HANDLER_VERSION		_T("D:\\TechWing\\Version_Date.INI")

#define SZ_2D_MARK_FOLDER				_T("2D_Mark")
#define SZ_2D_VALIDATE					_T("2dValidate")

#define NEXT_10			 Sleep(10)

#define DEF_MSG_TYPE_REGIST				(1)	 // 2021011	SSJ MSG Type별 Pasing
#define DEF_MSG_TYPE_INFORMATION		(2)	 // 2021011	SSJ MSG Type별 Pasing
#define DEF_MSG_TYPE_TEST_SCRIPT		(3)	 
#define DEF_MSG_TYPE_TEST_MODE			(4)	 
//#include <thread>
#ifndef NEXT
	//#define NEXT std::this_thread::yield();
	#define NEXT			 Sleep(1);
#endif 

#define MOTOR_INPOSITION_um			(5.0)	// 마이크로 미터

#define Z_MOTOR_PICK_RETRY_MAX_POS_um		(750.0)   //마이크로 미터

// 2D KEYENCE DEFINE
#define		ENQ		                            (0x05)
#define		ETX		                            (0x03)
#define		STX		                            (0x02)
#define		ACK		                            (0x06)
#define		NAK		                            (0x15)
#define		MAX_MSG	                            (1024)
#define		CR		                            (0x0D)
#define		READCMD	                            (0x80)

#define EJECTOR_CHANGE_MIN_TIME                 (50)
#define MIN_VAC_BLOW_TIME					    (50)
#define CYLINDER_ACT_MIN_TIME					(50)

#define MOTOR_TIME_OUT_LONG						(120.0)
#define MOTOR_TIME_OUT_MIDDLE					(60.0)
#define MOTOR_TIME_OUT_SHORT					(30.0)

#define TIME_OUT_LONG							(120.0)
#define TIME_OUT_MIDDLE							(30.0)
#define TIME_OUT_SHORT							(5.0)

#define TIME_MAIN_POWER_RECOVERY_PRE_DELAY_SEC	(5.0)

#define VIBRATOR_WAIT_TIME						(0.2)

#define SIMUL_SLEEP_TIME						(10)

#define VISION_TIME_OUT							(10.0)
#define VISION_TIME_OUT_LONG					(VISION_TIME_OUT*12)

#define DEF_SENSOR_DISP_TM						(500)

#define DEF_DESABLE								(-1)
#define DEF_ON									( 1)
#define DEF_OFF									( 0)
enum eOnOffState {
	eOnOffState_Unknown = -1,
	eOff,
	eOn
};
struct stOnOffState {
	eOnOffState state;
	char msg[ MAX_PATH ];

	operator int() const {
		return (int)state;
	}

	operator eOnOffState() const {
		return state;
	}
};

#define DEF_LOCK								( 1)
#define DEF_UNLOCK								( 0)

#define DEF_OPEN								( 1)
#define DEF_CLOSE								( 0)

#define DEF_CLAMP								(1)
#define DEF_UNCLAMP								(0)
#define DEF_CLAMP_UNK							(-1)
enum eClampCmd {
	eClamp_Unknown = -1,
	eUnclamp,
	eClamp
};

#define DEF_EXIST								(1)
#define DEF_NOT_EXIST							(0)
enum eExistState {
	eExist_Unknown = -1,
	eNot_Exist,
	eExist
};

#define DEF_WORKABLE_RESERVATION				(2)
#define DEF_WORKABLE							(1)
#define DEF_UNWORKABLE							(0)

#define DEF_WORK_RESERVED						(1)
#define DEF_WORK_FREE							(0)

// Under Stacker P-Trun
#define DEF_PTURN_HOME							(0)
#define DEF_PTURN_TURN							(1)
#define DEF_PTURN_UNK							(-1)

// Forward/Backward type
#define DEF_BACK								(0)
#define DEF_FORW								(1)
#define DEF_UNK									(-1)
enum eForwBackCmd {
	eForwBack_Unknown = -1,
	eBack,
	eForw
};

// Up/Down
#define DEF_DOWN								(1)
#define DEF_UP									(0)
#define DEF_UPDOWN								(2)
enum eUpDownCmd {
	eUp,
	eDown,
	eUpDown
};

enum eCleanOffset
{
	eCleanOffsetNotUse = 0,
	eCleanOffsetUse,
};

enum eTesterLotStatus
{
	eTester_Lot_Start = 0,
	eTester_Lot_On_Going,
	eTester_Lot_End,
};

enum ePickerSafetySensorCheckType {
	eSafetySensorCheck =0,
	ePlaceSafetySensorCheck,
	eMaxPickerSafetySensorCheckType,
};


#define DEF_PUSH_UP								(0)
#define DEF_PUSH_DOWN							(1)

#define DEF_LEFT                                (0)
#define DEF_RIGHT                               (1)

// REAR/FRONT
#define DEF_FRONT								(1)
#define DEF_REAR								(0)

#define DEF_DISABLE								(0)
#define DEF_ENABLE								(1)

#define DEF_SITE_OFF_RESERVATION				(2)
#define DEF_SITE_ON                             (1)
#define DEF_SITE_OFF                            (0)
#define DEF_SITE_DISABLE                        (-1)

#define DEF_SITE_OFF_YIELD                      (3)
#define DEF_SITE_OFF_ERRORCODE					(4)
#define DEF_SITE_SOCKET_RETEST_OK				(5)
#define DEF_SITE_SOCKET_HOLD					(6)
#define DEF_SITE_SOCKET_LOCK                    (7)
#define DEF_SITE_OFF_RESERVATION_YIELD			(8)

#define DEF_LOTEND_UNK                          (-1)
#define DEF_LOTEND_TRAYEND                      (0)
#define DEF_LOTEND_TRAYFEED                     (1)
#define DEF_LOTEND_TRAYSKIP                     (2)

#define DEF_AUTO_HEIGHT_CAL_UNK					(-1)
#define DEF_AUTO_HEIGHT_CAL_SAVE				(1)
#define DEF_AUTO_HEIGHT_CAL_CANCEL   			(0)

#define DEF_ZAXIS_PICKER_HEIGHT_CAL_UNK			(-1)
#define DEF_ZAXIS_PICKER_HEIGHT_CAL_SAVE		(1)
#define DEF_ZAXIS_PICKER_HEIGHT_CAL_CANCEL   	(0)

//#define DEF_ZAXIS_MAX_SPEED						(655000.0)
#define DEF_ZAXIS_MAX_SPEED						(65535)
#define DEF_ZAXIS_INIT_SPEED						(200)

#define MAX_WIN_X_SIZE							(1280)
#define MAX_WIN_Y_SIZE							(1024)


#define MAX_SETPLATE							(7)
#define MAX_LOAD_TABLE							(4)	// 20230808 hyb loader table 갯수 4개
#define MAX_TEST_PP								(16)

#define MAX_IGNORE_INDEX_REC_COUNT				(3)

#define MAX_STRING_LENGTH						(255)

#define MAX_CYLINDER_COUNT						(80)

#define LOAD_TABLE_MOVE_OFFSET_Y				(150000.0)

// #define MAX_TEST_SITE_X							(4)
// #define MAX_TEST_SITE_Y							(2)
// #define MAX_TEST_SITE							(16)

// #define MAX_TEST_SITE_64PARA_X				    (32)
// #define MAX_TEST_SITE_64PARA_Y					(1)
// #define MAX_TEST_SITE_64PARA					(64)	//(_1STATION_IN_MAX_SITE*eMaxPressUnitCount)
// #define _1STATION_IN_MAX_SITE					(4)

//#define TRAY_THICKNESS_DEFAULT					(6350.0) // 6.35 mm

//////////////////////////////////////////////////////////////////
///////////////Interlock 절대 위치 정의 Define -> Global 변수로 이동 

#define INTERLOCK_LOADING_TABLE_Y_FRAM_IMPACT_LOC_START		(-1557000.0)  
#define INTERLOCK_LOADING_TABLE_Y_FRAM_IMPACT_LOC_END		(-1857000.0)
#define INTERLCOK_LOADING_TABLE_Y_STACKER_IMPACT_LOC		(-2155000.0)

// #define MIN_TRAY_PP_WORK_ENABLE_POS_um				(-50000)
// //Tray Load Hand
// #define INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS		(292000.0) //Tray Load X Feeder Left 간섭 구간.
// #define INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS		(741500.0) //Tray Load X Feeder Right 간섭 구간.
// #define INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT		(292000.0) //Tray Load X Feeder Left 간섭 구간.
// #define INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT		(741500.0) //Tray Load X Feeder Right 간섭 구간.
// 
// //Tray Unload Hand
// #define INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER			(17300.0) //Tray Unload Y Transfer 간섭 구간.
// #define INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS		(324000.0) //Tray Unload X Feeder Left 간섭 구간.
// #define INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS		(781500.0) //Tray Unload X Feeder Right 간섭 구간.
// #define INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT		(324000.0) //Tray Load X Feeder Left 간섭 구간.
// #define INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT		(781500.0) //Tray Load X Feeder Right 간섭 구간.
// 
// //Test Hand
// #define INTERLOCK_TESTPP_1_X_STATION_START_POS					(-285000.0) //-136000.0 * 2
// #define INTERLOCK_TESTPP_2_X_STATION_START_POS					(282000.0) //136000.0 * 2
// #define INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT		(182500.0 * 2) //Table Front 위치
// #define INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR		(66300 * 2) //Table Rear 위치 //81350.0
// #define INTERLOCK_TESTPP_X_SAFETY_POS							(133000.0 *2) //282 //133000.0 *2
// 
// #define INTERLOCK_HAND_LENGTH							(274000.0)
// #define INTERLOCK_STATION_GAP							(10000.0)
// #define INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD		(-1300000.0) // Test PP Load Y축 Station 1 간섭 시작 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD		(-712000.0) // Test PP Load Y축 Station 1 간섭 종료 위치. // -722
// #define INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD		(-930000.0) // Test PP Load Y축 Station 2 간섭 시작 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD		(-369000.0) // Test PP Load Y축 Station 2 간섭 종료 위치. // -379
// #define INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD		(-580000.0) // Test PP Load Y축 Station 3 간섭 시작 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD		(-33000.0) // Test PP Load Y축 Station 3 간섭 종료 위치. // -43
// #define INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD		(-247000.0) // Test PP Load Y축 Station 4 간섭 종료 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD		(40000.0) // Test PP Load Y축 Station 4 간섭 종료 위치. 
// 
// #define INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD	(-1300000.0) // Test PP Unload Y축 Station 1 간섭 시작 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD		(-959000.0) // Test PP Unload Y축 Station 1 간섭 종료 위치. //-969
// #define INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD	(-1214000.0) // Test PP Unload Y축 Station 2 간섭 시작 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD		(-609000.0) // Test PP Unload Y축 Station 2 간섭 종료 위치. //-619
// #define INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD	(-870000.0) // Test PP Unload Y축 Station 3 간섭 시작 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD		(-273000.0) // Test PP Unload Y축 Station 3 간섭 종료 위치. //-283
// #define INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD	(-529000.0) // Test PP Unload Y축 Station 4 간섭 종료 위치. 
// #define INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD		(40000.0) // Test PP Unload Y축 Station 4 간섭 종료 위치. 
// 
// #define INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT			(-375000.0)
// #define INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT			(375000.0)
// 
// //Loading Table
// #define INTERLOCK_LOADING_TBL_POS_BOUNDARY			(-1700000.0)
// 
// //Press
// #define INTERLOCK_PRESS_Z_SAFETY_POS				(0.0)	//Press Z Safety Position
// 
// //feeder
// #define INTERLOCK_FEEDER_SAFETY_POS_ENC				(-300000.0) //Tray Feeder 안전 위치 확인(Safety Sensor 감지 되는 위치).
// #define INTERLOCK_FEEDER_POS_BOUNDARY				(-148000.0) //Tray Feeder Unload Y축 간섭 시작 위치.


//////////////////////////////////////////////////////////////////


#define Move_100mm_Offset					(100000.0)

// #define MIN_TRAY_PP_WORK_ENABLE_POS_um		(-30000)
// #define MAX_TRAY_PP_WORK_ENABLE_POS_um		(1)

//#define TRANSFER_HEAD_DIST_DEFAULT				(180000.0) // um
#define TEST_PP_Y_AVOID_DIST_DEFAULT            (140000.0) // um
//#define TRAY_PP_MOTOR_SPD_STD_GAP_DIST          (300000.0) // um
#define LOAD_TABLE_IMPACT_DIST					(25000.0)   // um // 기준점이 될 위치
#define LOAD_TABLE_IMPACT_MOVE_DIST             (12500.0)	// um // 실제로 움직일 거리
#define LOAD_TABLE_IMPACT_COUNT					(3)
#define TEST_PP_Y_SAFETY_DIST					(100000.0) // um

#define LOADER_TABLE_REVERSE_SPIN_DEGREE		(-180000.0)	// Loader table rotator 180도 회전
#define LOADER_TABLE_STACKER_IMPACT_MOVE		(100000.0)

#define DEF_REQ_UNK                             (-1)
#define DEF_REQ_CANCEL                          (0)
#define DEF_REQ_OK                              (1)

// default:: cleaning contact count
#define DEFAULT_SOC_CLEAN_CONTACT_ONE_ACTION    (3)

#define MAX_BINCATE_LIST                        (256)

#define MAX_2D_DEVICE_LIST					(256)
//#define DEF_DVC_HOR                             (0)
//#define DEF_DVC_VER                             (1)

#define TARGET_RESULUTION                       (1)

#define DEF_VAT_FIND_HOLE_RETRY_COUNT           (7)

#define MAX_VISION_RESULT_DATA_COUNT			(4)

#define DEF_VAT_SCALE_FACTOR_CAL_OK_RANGE		(0.5)
#define DEF_VAT_PICKER_ALIGN_OK_RANGE			(200) // Picker Align Spec 0.200 mm.
#define DEF_VAT_PICKER_ALIGN_CAL_OK_RANGE		(200) // Picker Align 보정 시 Spec 0.200 mm.
#define DEF_VAT_ARRANGE_CAL_INPUT_ARM_2XPICKER	(0)
#define DEF_VAT_ARRANGE_CAL_MIN_COUNT			(2) // Picker Align 최소 보정시도 횟수.
#define DEF_VAT_ARRANGE_CAL_MAX_COUNT			(7) // Picker Align 최대 보정시도 횟수.

#define DEF_MAX_TOKENIZE_2D_MARK				(10)

//==============================================================================
// SLT GUI <-> MC Remote Command
//==============================================================================
#define     GUI_RCMD_REQ_PAUSE							(1000)
#define     GUI_RCMD_REQ_SHOW_DLG						(1001)
#define     GUI_RCMD_REQ_FINALIZE_PGM					(1002)
#define     GUI_RCMD_REQ_YIELD_MONITOR_CLEAR			(1003)
#define     GUI_RCMD_REQ_BIN_CATEGORY_REFRESH			(1004)
#define     GUI_RCMD_REQ_PACKAGE_CONVERSION				(1005)
#define     GUI_RCMD_REQ_USER_OFFSET_REFESH				(1006)
#define     GUI_RCMD_REQ_SITE_VISION_SET_CHNG			(1007)
#define     GUI_RCMD_REQ_2D_VISION_SET_CHNG				(1008)
#define		GUI_RCMD_REQ_AUTOGRR_CANCEL					(1009)
#define     GUI_RCMD_REQ_VIBRATOR_CHANGE				(1010)
#define     GUI_RCMD_REQ_TEACHING_RELOAD				(1011)
#define     GUI_RCMD_REQ_DATA_ASSIGN_SHOW				(1012)
#define     GUI_RCMD_REQ_MAKE_SUMMARY    				(1013)

#define     GUI_RCMD_REQ_TESTPP_OFFSET_SHOW				(1015)
#define     GUI_RCMD_REQ_YIELD_MONITOR_CLEAR_EACH		(1016)
#define		GUI_RCMD_REQ_DEGAULT_BIN_CATEGORY_REFRESH	(1017)
#define     GUI_RCMD_REQ_TESTER_ONLINE_MODE_CHANGE		(1018)
#define     GUI_RCMD_REQ_MODULE_SITE_ON_IS_POSSIBLE		(1019)   
#define     GUI_RCMD_REQ_TRAYPP_SAFETY_POS_MOVE			(1020)
#define     GUI_RCMD_REQ_MAKE_SUMMARY_END				(1021)
#define     GUI_RCMD_REQ_MULTI_ORIGIN					(1022)
#define     GUI_RCMD_REQ_TESTER_BATCH_FILE_EXCUTE		(1023)
#define     GUI_RCMD_REQ_LOTINFO_SHOW					(1024)
#define     GUI_RCMD_REQ_LOAD_SUPPLY					(1025)
#define     GUI_RCMD_REQ_QA_QTY_SHOW					(1026)
#define     GUI_RCMD_REQ_DOOR_STATUS_VIEW				(1027)
//==============================================================================
// WindowMessage Define
//==============================================================================
#define WM_UPDATE_MAIN_DISPLAY	(WM_USER + 1100)

//==============================================================================
// MACRO FUNCTION
//==============================================================================
#define GETBIT(x, y) (((x)>>(y)) & (0x01))
#define SETBIT(x, y) (x) |= ((0x01)<<(y))
#define CLRBIT(x, y) (x) &= ~((0x01)<<(y))
#define SAFE_DELETE(p)	if(p){ delete (p); (p) = nullptr; }
#define SAFE_DELETE_ARRAY(p)	if(p){ delete[] (p); (p) = nullptr; }

#define MAX_SLOT_COUNT_UPPER_STACKER   (30)
#define MAX_SLOT_COUNT_UNDER_STACKER   (5)


// Robot Device Pick/Place 정보 찾기 위해 사용
#define VIRTUAL_POCKET_Y_MAX			(100 )
#define VIRTUAL_POCKET_X_MAX			(100 )

#define VIRTUAL_POCKET_X_START			(30 )
#define VIRTUAL_POCKET_Y_START			(30 )
//==============================================================================
// ENUM
//==============================================================================
enum _eMotorIntertlock
{
	eMotorInterlock_Operation = 0,
	eMotorInterlock_ZAxisNoChkXYTMotioning,									// Z축 MoveInterlock 일부 해제[Z축이 이동 하려고 할 때 hand, x,y,t 가 모셔닝 중이면 동작 제한 해제] 
	eMotorInterlock_ForceMove,
	eMotorInterlock_All_Ignore,
};

enum _eApplyBacklashCondition
{
	eApplyBacklashCondition_NoApply = -1,	// 일반 Move인데, Backlash미적용일떄
	eApplyBacklashCondition_Normal = 0,		// 일반 Move일때, ( Backlash적용 )
	eApplyBacklashCondition_BacklashTest,	// Backlash Test모드일때 ( Backlash적용 )
	eApplyBacklashCondition_Backlash		// Backlash 측정모드일때 ( Backlash미적용 )
};


enum eDeviceDataStage
{
	eDeviceStage_Undefine = -1,
 	eDeviceStage_STACKER_CONV_1=0,
 	eDeviceStage_STACKER_CONV_2,
 	eDeviceStage_STACKER_CONV_3,
 	eDeviceStage_STACKER_CONV_4,
 	eDeviceStage_STACKER_CONV_5,
	eDeviceStage_STACKER_CONV_6,
	eDeviceStage_STACKER_CONV_7,
	eDeviceStage_STACKER_BTM_1,
	eDeviceStage_STACKER_BTM_2,
	eDeviceStage_STACKER_BTM_3,
	eDeviceStage_STACKER_BTM_4,
	eDeviceStage_STACKER_BTM_5,
	eDeviceStage_STACKER_BTM_6,
	eDeviceStage_STACKER_BTM_7,
	eDeviceStage_TRAY_PP_1,
	eDeviceStage_TRAY_PP_2,
	eDeviceStage_TRAY_FEEDER,
	eDeviceStage_STACKER_CONV_3_TRANSFER_AREA,
	eDeviceStage_LD_TBL_1,
	eDeviceStage_LD_TBL_2,
 	eDeviceStage_LD_TBL_3,
 	eDeviceStage_LD_TBL_4,
	eDeviceStage_TEST_PP_1,
	eDeviceStage_TEST_PP_2,
	eDeviceStage_TEST_SITE_1,
	eDeviceStage_TEST_SITE_2,
	eDeviceStage_TEST_SITE_3,
	eDeviceStage_TEST_SITE_4,
	eDeviceStage_TEST_SITE_5,
	eDeviceStage_TEST_SITE_6,
	eDeviceStage_TEST_SITE_7,
	eDeviceStage_TEST_SITE_8,

	eDeviceStage_Transfer,

	eDeviceStage_TEST_PP_1_ART_BUFF,
	eDeviceStage_TEST_PP_2_ART_BUFF,

	eDeviceStage_TEST_PP_1_CLEAN_BUFF,
	eDeviceStage_TEST_PP_2_CLEAN_BUFF,

	eDeviceStage_TRAY_BUFF,

	eDeviceDataStageCountMax,
};

enum eAutoLotCmd{
	eAutoLotCmd_None = 0,
	eAutoLotCmd_Initial,
	eAutoLotCmd_Start,
	eAutoLotCmd_Pause,     // 장비의 Stop 버튼
	eAutoLotCmd_Resume,    // 장비의 Start 버튼
	eAutoLotCmd_Retry,
	eAutoLotCmd_Skip,
	eAutoLotCmd_CleanOut,
	eAutoLotCmd_AlarmPause,
};

enum eAutoLotState{
	eAutoLotState_InitNeed=0,
	eAutoLotState_Initialize,
	eAutoLotState_Idle,		    // Idle
	eAutoLotState_Executing,	// Auto Running
	eAutoLotState_Pause,		// Pause
	eAutoLotState_Resume,		// Resume
	eAutoLotState_AlarmStop,
	eAutoLotState_NormalComplete,
	eAutoLotState_CleanOut,
	eAutoLotState_Unknown = 99,
};

enum eDlgProcCmd{
    eDlgProcCmd_Position = 0,
    eDlgProcCmd_IoMonitorDIO,
    eDlgProcCmd_IoMonitorAIO,
    eDlgProcCmd_MotorMonitor,
    eDlgProcCmd_Alignment,
    eDlgProcCmd_SignalTower,
    eDlgProcCmd_TesterIfTest,
	eDlgProcCmd_SocketContactCount,
	eDlgProcCmd_ServoMotor,
	eDlgProcCmd_TesterInfo,
	eDlgProcCmd_LotInfoSave,
	eDlgProcCmd_VAT,
	eDlgProcCmd_TrayView,
	eDlgProcCmd_BatchFileErr,
	eDlgProcCmd_BarcodeSetting,
};

enum eHandlerLotMode{
    eHandlerLotMode_Idle =0,
    eHandlerLotMode_OnGoing,
};

enum eTesterIF_Mode {
	eTesterIF_Mode_Normal = 0,
	eTesterIF_Mode_Grr,
	eTesterIF_Mode_LoopTest,
	eTesterIF_Mode_Audit,
};

enum eSystemLang{
	eSysLang_Eng = 0, eSysLang_Kor, eSysLang_Chi, eSysLang_Jap, eMaxSysLangCount,
};

enum eAutoRetestBin{
	eARB_NotTest = 0,
	eARB_1stTestEnd ,
	eARB_2stTestEnd ,     // Once Fail
	eARB_3stTestEnd ,     // Twice Fail
	eMax_ARB_BinCount,
};

enum eSensor_TYPE{
	eSensor_TypeA = 0,
	eSensor_TypeB,
};

enum eBIN_COLOR
{
	eBin_NotTest = -1,
	eBinFail = 0,
	eBinGood,	
	eBinError,
	eMaxBinColor,
};

enum eDVC_INFO
{
	eDvc_Cleaning = 0,
	eDVC_Normal,
	eDVC_None,
};

enum eAUTO_RETEST_TEST_PP
{
    eAutoRetest_Aonly = 0,
    eAutoRetest_AB,
    eAutoRetest_AAB,
    eAutoRetest_AA,
};

enum eTEMP_MODE
{
	eTEMP_AMBIENT = 0,
	eTEMP_HOT,
	eMaxTempMode,
};

enum eRUN_MODE
{
	eRUN_MODE_ONLINE  = 0,
	eRUN_MODE_OFFLINE,
	eRUN_MODE_DEMO,
	eMaxRunMode,
};

// 0:Device Mode, 1:NoDeviceTrayExist, 2:NoDeviceNoTray
enum _eDeviceMode
{
	eDeviceMode=0,
	eNoDeviceTrayExist,
	eNoDeviceNoTray,
	eMaxDeviceMode,
};

enum eCONTACT_MODE
{
	eCONTACT_DIRECT = 0,
	eCONTACT_DROP,
	eMaxContactMode,
};

enum eLOT_START_MODE
{
	eStart_Normal = 0,
	eStart_RC,
	eMaxStartMode,
};

enum eUSER_LEVEL
{
	eUSERLEVEL_OPERATOR = 0,
	eUSERLEVEL_TECHNICIAN,
	eUSERLEVEL_ENGINEER,
	eUSERLEVEL_SUPERVISOR,
	eMaxUserLevel,
};

enum eTRAYASSIGN_PATTERN
{
	eTrayAssign_LoadToEmpty1  = 0,
	eTrayAssign_LoadToEmpty2,
	eMaxTrayPattern,
};

enum eSYSTEM_OPTION
{
	eOPTION_NOT_USE = 0,
	eOPTION_USE
};
 
enum eALARM_LEVEL
{
	eAlarm_Lv0 = 0,
	eAlarm_Lv1,
	eAlarm_Lv2,
	eAlarm_Lv3,
	eAlarm_All,
	eAlarm_Period,
};

enum eBaudRate
{
	eBaudRate9600   = 0,
	eBaudRate19200,
	eBaudRate38400,
	eBaudRate115200,
};

enum eBinColor
{
	eFailBin = 0,
	eGoodBin,
	eOtherFailBin,
};

// Define 순서 바꿀때, Auto Test Site Seq Flow 변경 되므로 순서 변경 주의!!
enum eStartMode
{
	eStartMD_Initial = 0,
	eStartMD_Retest,
	eStartMD_AutoGRR,
	eStartMD_QA,
	eStartMD_AutoLoop,
	eStartMD_InitContinue,
	eStartMD_RetestContinue,
	eStartMD_AutoGRR_Continue,
    eStartMD_QA_Continue,	
	eStartMD_AutoLoop_Continue,
	eMaxStartMD,
};

enum eTaskAutoTestSiteAreaLoc{
	eATS_DN_STATION_L1 = 0,
	eATS_DN_STATION_L2,
	eATS_DN_STATION_L3,
	eATS_DN_STATION_L4,
// 	eATS_UP_STATION_L1,
// 	eATS_UP_STATION_L2,
// 	eATS_UP_STATION_L3,
// 	eATS_UP_STATION_L4,

	eMaxATS_Area,
};

enum eSltPgm_ItemList
{
    eSltPgm_ItemNew = 0,
    eSltPgm_ItemModify,
};

enum eJamRateDataType{
	eJamRateDataType_Current=0,
	eJamRateDataType_First,
	eJamRateDataType_Last,
};

enum eTesterInfoEach {
	eTesterInfoEach_HW_Version = 0,
	eTesterInfoEach_PPO_FW_Version, 
	eTesterInfoEach_HID_FW_Version,
	eTesterInfoEach_SW_Version,
	eTesterInfoEach_Test_Script,
	eMaxTesterInfoEach,
};

 enum eTesterInfoSingleDual {
 	eTesterInfo_TesterID = 0,
 	eTesterInfo_SocketID,
 	eTesterInfo_SW_Version,
 	eTesterInfo_IP,
 	eMaxTesterInfoSingleDual,
 };
// rev tester info CTesterStatus(m_sInfo) paring version history

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 
// enum eTesterInfo {
// 	eTesterInfo_HW_Version = 0,
// 	eTesterInfo_PPO_FW_Version, 
// 	eTesterInfo_HID_FW_Version,
// 	eTesterInfo_SW_Version,
// 	eTesterInfo_Test_Script,
// 	eMaxTesterInfo,
// };
// #else
// 
// enum eTesterInfo {
// 	eTesterInfo_TesterID = 0,
// 	eTesterInfo_SocketID,
// 	eTesterInfo_SW_Version,
// 	eTesterInfo_IP,
// 
// 	eMaxTesterInfo,
// };
// 
// #endif

enum eAgentInfo {
	eAgentInfo_TesterID = 0,
	eAgentInfo_StationID,
	eAgentInfo_SW_Version,
	eAgentInfo_IP,

	eMaxAgentInfo,
};

enum eInOutJamType{
	eJamType_Input=0,
	eJamType_Output,
	eMaxJamType,
};

enum e2dPocketDirection{
	e2dPocketDirection_REAR_LEFT=0,
	e2dPocketDirection_REAR_RIGHT,
	e2dPocketDirection_FRONT_LEFT,
	e2dPocketDirection_FRONT_RIGHT,
};

enum eLoadTablePocketY {
	eLoadTablePocketY1 = 0,
	eLoadTablePocketY2,
	eLoadTablePocketY3,
	eLoadTablePocketY4,
	eMaxLoadTablePocketY,
};

enum eMultiOrgType{
	eMultiOrgType_PressUp=0,
	eMultiOrgType_PressDown,
	eMultiOrgType_TestPp,
	eMultiOrgType_LoadTable,
	eMultiOrgType_TrayPP,
	eMultiOrgType_Module1,
	eMultiOrgType_Module2,
	eMultiOrgType_ModuleAll,
	eMultiOrgType_EqpUpAll,
	eMultiOrgType_AxisAll,
	eMaxMultiOrgType,
};

// Pwr = Power
// Tm  = Time
// Dly = Delay
// enum eSltPgm_ItemList
// {
//     eSltPgmType_BootTmOut = 0,
//     eSltPgmType_PreCooling,
//     eSltPgmType_FailCodeDigNo,
//     eSltPgmType_AcPwrOnDly,
//     eSltPgmType_AcPwrOff,
//     eSltPgmType_AtxPowerOn,
//     eSltPgmType_MinPassTm,
//     eSltPgmType_BaudRate,
//     eSltPgmType_HangReboot,
//     eSltPgmType_BiosBoot,
//     eSltPgmType_BiosS3,
//     eSltPgmType_TempMonitor,
//     eSltPgmType_ExtentionPwrAGP,
//     eSltPgmType_PassStr,
//     eSltPgmType_FailCodeHead,    
//     eSltPgmType_StartLine,
//     eSltPgmType_EndLine,
//     eSltPgmType_MaxItem,
// };

enum eVisionPC{
	eVision_AllUnit = -1,
	eVision_BarCode=0,
	eVision_TestSiteUp,
	eVision_TestSiteDown,
	eMaxVisionPC,

	
	eVision_TestHand1=1,
	eVision_TestHand2,
	eVision_VAT_PC1,
	eVision_VAT_PC2
};


enum eRetestYieldFunc{
    eRetestYieldFunc_Stop = 0,
    eRetestYieldFunc_Warring, 
};

enum eTesterOption {
	eTester_Single = 0,
	eTester_Dual,
	eTester_Agent,
	eTester_EachSocket	// 각 소켓별로 패킷 송수신 하는 방식 for yanmade
};


enum eJamRateData{
    eJamRateData_LoadCnt=0,
    eJamRateData_SortCnt,
    eJamRateData_PassBinCnt,
    eJamRateData_FailBinCnt,
    eJamRateData_Output1Cnt,
    eJamRateData_Output2Cnt,
    eJamRateData_Output3Cnt,
    eJamRateData_Output4Cnt,
    eMaxJamRateData,
};

enum eJamRateCode{
	eJamRateCode_Normal=0,
	eJamRateCode_RT1,
	eJamRateCode_RT2,
	eJamRateCode_RT3,
	eJamRateCode_RT4,
	eJamRateCode_RT5,
	eMaxJamRateCode,
};

enum eFileTransType{
	eFileTransType_CSV=0,
	eFileTransType_LOG,
	eMaxFileTransType,
};

enum eVisionPos{
	eVisionPos_TestSite=0,
	eVisionPos_2dBarcode,
	eMaxVisionPos,
};

enum ePressWorkType{
	ePressWorkType_Contact=0,
	ePressWorkType_Safety,
	ePressWorkType_ChkVision,
	ePressWorkType_Test,
	ePressWorkType_SocClnContact,
	eMaxePressWorkType,

	ePressWorkType_Safety_EmptySite,
	ePressWorkType_Safety_CleanEnd,
	ePressWorkType_Safety_TestEnd,
};


//===========================================================================================
//  RBOBOT TEACHING - POSITION (LOCATION에 대한 각 축의 위치)
//===========================================================================================
//┌────────┬───────┬───────┬───────┬───────┬───────┬───────┐
//│        │ POS 1 │ POS 2 │ POS 3 │ POS 4 │ POS 5 │ POS 6 │
//├────────┼───────┼───────┼───────┼───────┼───────┼───────┼
//│ LOC 1  │	   │       │       │       │       │       │
//│ LOC 2  │	   │       │       │       │       │       │
//│ LOC 3  │	   │       │       │       │       │       │
//│ ...    │	   │       │       │       │       │       │
//│ LOC n  │	   │       │       │       │       │       │
//└────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//===========================================================================================
#define MAX_POS_IDX_COUNT			         (15)

//===========================================================================================
// ※ POS_IDX와 LOC_IDX는 Teaching Table의 Row와 Col이 매칭되므로 수정 시 주의가 필요 함.
//===========================================================================================
// PRESS_UNIT						         
enum ePOS_IDX_PRESS_UNIT{
	POS_IDX_PRESS_UNIT_Z = 0,
	POS_IDX_PRESS_UNIT_Z_MAX,
};

// TEST_PP							         
enum ePOS_IDX_TEST_PP{
	POS_IDX_TEST_PP_X1	   = 0,
	POS_IDX_TEST_PP_Y1	     ,
	POS_IDX_TEST_PP_Z1_PICK   ,
	POS_IDX_TEST_PP_Z1_PLACE  ,
	POS_IDX_TEST_PP_Z1_SAFETY ,
	POS_IDX_TEST_PP_X1_PITCH,
	POS_IDX_TEST_PP_Y1_PITCH,
	POS_IDX_TEST_PP_X2,
	POS_IDX_TEST_PP_Y2,
	POS_IDX_TEST_PP_Z2_PICK,
	POS_IDX_TEST_PP_Z2_PLACE,
	POS_IDX_TEST_PP_Z2_SAFETY,
	POS_IDX_TEST_PP_X2_PITCH,
	POS_IDX_TEST_PP_Y2_PITCH,
	POS_IDX_TEST_PP_MAX ,
};

// LD_TBL				
enum ePOS_IDX_LD_TBL{
	//POS_IDX_LD_TBL_X=0,
	POS_IDX_LD_TBL_Y=0,
	POS_IDX_LD_TBL_ROTATE,
	POS_IDX_LD_TBL_MAX,
};

// TRAY_PP				
enum ePOS_IDX_TRAY_PP{
	POS_IDX_TRAY_PP_X	   = 0,
	POS_IDX_TRAY_PP_Y	      ,	
	POS_IDX_TRAY_PP_Z_PICK    , 	
	POS_IDX_TRAY_PP_Z_PLACE   ,	
	POS_IDX_TRAY_PP_Z_SAFETY  ,	
	POS_IDX_TRAY_PP_X_PITCH   ,	
	POS_IDX_TRAY_PP_Y_PITCH   ,	
	POS_IDX_TRAY_PP_MAX       ,
};

enum ePOS_IDX_TRAY_LOAD_PP {
	POS_IDX_TRAY_LOAD_PP_X = 0,
	POS_IDX_TRAY_LOAD_PP_Z_PICK,
	POS_IDX_TRAY_LOAD_PP_Z_PLACE,
	POS_IDX_TRAY_LOAD_PP_Z_SAFETY,
	POS_IDX_TRAY_LOAD_PP_X_PITCH,
	POS_IDX_TRAY_LOAD_PP_Y_PITCH,
	POS_IDX_TRAY_LOAD_PP_MAX,
};

// FEEDER
enum ePOS_IDX_FEEDER {
	POS_IDX_FEEDER_Y = 0,
	POS_IDX_STOPPER_Y,
	POS_IDX_FEEDER_MAX,
};

// TRANSFER
enum ePOS_IDX_TRANSFER{
	POS_IDX_TRANSFER_X			= 0,
	POS_IDX_TRANSFER_MAX           ,
};


//===========================================================================================
//  RBOBOT TEACHING - LOCATION (하나의 의미를 갖는 위치)
//===========================================================================================
#define MAX_LOC_IDX_COUNT                              (22)		// Max Location Point

// PRESS_UNIT
enum eLocIdxPressUnit{
	LOC_IDX_PRESS_UNIT_SAFETY		            = 0 ,//(0)
	LOC_IDX_PRESS_UNIT_CONTACT		                ,//(1)
	MAX_PRESS_UNIT_LOC_COUNT,
};

// TEST_PP
enum eLocIdxTestPP{
	LOC_IDX_TEST_PP_Undefined = -1,

	LOC_IDX_STATION_1 = 0,
	LOC_IDX_STATION_2,
	LOC_IDX_STATION_3,
	LOC_IDX_STATION_4,

	LOC_IDX_TEST_PP_TBL_1_LD,
	LOC_IDX_TEST_PP_TBL_2_LD,
	LOC_IDX_TEST_PP_TBL_3_LD,
	LOC_IDX_TEST_PP_TBL_4_LD,

	LOC_IDX_TEST_PP_TBL_1_UD,
	LOC_IDX_TEST_PP_TBL_2_UD,
	LOC_IDX_TEST_PP_TBL_3_UD,
	LOC_IDX_TEST_PP_TBL_4_UD,

	LOC_IDX_CLEAN_TBL,

	LOC_IDX_TEST_PP_HOME,	
	MAX_TEST_PP_LOC_COUNT,

	//Teaching Table에는 없으며, LOC_IDX_TEST_PP_TBL_3_LD 위치로 사용.
	LOC_IDX_TEST_PP_LOAD_TBL_BUFF = 99,
};

// LD_TBL
enum eLocIdxLoadingTbl{
	LOC_IDX_TBL_LD_SIDE= 0,//(0)
	LOC_IDX_LD_TBL_TEST_SIDE_1,//(1)
	LOC_IDX_LD_TBL_TEST_SIDE_2,
	LOC_IDX_LD_TBL_TEST_SIDE_3,
	LOC_IDX_LD_TBL_TEST_SIDE_4,
	LOC_IDX_LD_TBL_TEST_SIDE_2D,
	MAX_LD_TBL_LOC_COUNT,
};

enum eLocIdxTrayLoadPP {
	LOC_IDX_TRAY_LOAD_PP_FEEDER=0,
	LOC_IDX_TRAY_LOAD_PP_CONV_3,
	LOC_IDX_TRAY_LOAD_PP_TBL_1,
	LOC_IDX_TRAY_LOAD_PP_TBL_2,
	LOC_IDX_TRAY_LOAD_PP_TBL_3,
	LOC_IDX_TRAY_LOAD_PP_TBL_4,
	LOC_IDX_TRAY_LOAD_PP_SAFETY,
};

enum eLocIdxTrayUnLoadPP {
	LOC_IDX_TRAY_UNLOAD_PP_CONV_4 =0,
	LOC_IDX_TRAY_UNLOAD_PP_CONV_5,
	LOC_IDX_TRAY_UNLOAD_PP_CONV_6,
	LOC_IDX_TRAY_UNLOAD_PP_CONV_7,
	LOC_IDX_TRAY_UNLOAD_PP_TBL_1,
	LOC_IDX_TRAY_UNLOAD_PP_TBL_2,
	LOC_IDX_TRAY_UNLOAD_PP_TBL_3,
	LOC_IDX_TRAY_UNLOAD_PP_TBL_4,
	LOC_IDX_TRAY_UNLOAD_PP_SAFETY,
};

enum eLocIdxTrayLoadPPManual {
	LOC_IDX_TRAY_LOAD_PP_MANUAL_FEEDER = 0,
	LOC_IDX_TRAY_LOAD_PP_MANUAL_CONV_3,
	LOC_IDX_TRAY_LOAD_PP_MANUAL_TBL_2,
	LOC_IDX_TRAY_LOAD_PP_MANUAL_TBL_3,
	LOC_IDX_TRAY_LOAD_PP_MANUAL_SAFETY,
};

enum eLocIdxTrayUnLoadPPManual {
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_4 = 0,
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_5,
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_6,
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_7,
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_TBL_1,
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_TBL_4,
	LOC_IDX_TRAY_UNLOAD_PP_MANUAL_SAFETY,
};

enum eLocIdxTrayPP {
	LOC_IDX_TRAY_PP_Undefined = -1,
	LOC_IDX_TRAY_PP_TBL_1 = 0,
	LOC_IDX_TRAY_PP_TBL_2,
	LOC_IDX_TRAY_PP_TBL_3,
	LOC_IDX_TRAY_PP_TBL_4,

	LOC_IDX_TRAY_PP_CONV_1,
	LOC_IDX_TRAY_PP_CONV_2,
	LOC_IDX_TRAY_PP_CONV_3,
	LOC_IDX_TRAY_PP_CONV_4,
	LOC_IDX_TRAY_PP_CONV_5,
	LOC_IDX_TRAY_PP_CONV_6,
	LOC_IDX_TRAY_PP_CONV_7,

	LOC_IDX_TRAY_PP_FEEDER,

	LOC_IDX_TRAY_PP_SAFETY,

	MAX_TRAY_PP_LOC_COUNT,

	LOC_IDX_TRAY_PP_TBL_MAX_COUNT = 4,
};


enum eLocIdxFeeder {
	LOC_IDX_FEEDER_LOAD_PICK_START_POS=0,
	LOC_IDX_FEEDER_TRAY_PICK_POS,
	LOC_IDX_FEEDER_TRAY_PLACE_POS,
	
	LOC_IDX_FEEDER_SAFETY,

	MAX_TRAY_FEEDER_LOC_COUNT

};

// TRANSFER
enum eLocIdxTransfer{
	LOC_IDX_TRANSFER_CONV1,
	LOC_IDX_TRANSFER_CONV2,
	LOC_IDX_TRANSFER_CONV3,
	LOC_IDX_TRANSFER_CONV4,
	LOC_IDX_TRANSFER_CONV5,
	LOC_IDX_TRANSFER_CONV6,
	LOC_IDX_TRANSFER_CONV7,
	LOC_IDX_TRANSFER_SAFETY,

	MAX_TRANSFER_LOC_COUNT,
};

enum eMANU_OPER_IDX_POS{
	eMANU_OPER_PRESS_DN_1 = 0,
	eMANU_OPER_PRESS_DN_2,
	eMANU_OPER_PRESS_DN_3,
	eMANU_OPER_PRESS_DN_4,
	eMANU_OPER_PRESS_DN_5,
	eMANU_OPER_PRESS_DN_6,
	eMANU_OPER_PRESS_DN_7,
	eMANU_OPER_PRESS_DN_8,
	eMANU_OPER_TEST_PP1,
	eMANU_OPER_TEST_PP2,
	eMANU_OPER_LD_TBL1,
	eMANU_OPER_LD_TBL2,
	eMANU_OPER_LD_TBL3,
	eMANU_OPER_LD_TBL4,
	eMANU_OPER_TRAY_PP1_LD,
	eMANU_OPER_TRAY_PP2_UD,
	eMANU_OPER_TRANSFER,
	eMANU_OPER_FEEDER,
	eMANU_STACKER_1,
	eMANU_STACKER_2,
	eMANU_STACKER_3,
	eMANU_STACKER_4,
	eMANU_STACKER_5,
	eMANU_STACKER_6,
	eMANU_STACKER_7,
	eMAX_MANU_OPER_CNT,
};

enum eFileErrorCode
{
	eFileNotExist = 1,
	eFileNotAccess = 2,
	eFileNotData = 3,
};

/*
 *	device 정보; empty, partial, full 조사 (의미 부여)
 */
enum eDeviceFullEmptyCheckCode
{
	eEmpty = -1,
	ePatial = 0,
	eFull
};

enum eDeviceHistoryStage
{
	HISTORY_LOC_IDX_TRAY_PP_SHT_1 = 0,
	HISTORY_LOC_IDX_TRAY_PP_SHT_2,
	HISTORY_LOC_IDX_TRAY_PP_CONV_1,
	HISTORY_LOC_IDX_TRAY_PP_CONV_2,
	HISTORY_LOC_IDX_TRAY_PP_CONV_3,
	HISTORY_LOC_IDX_TRAY_PP_CONV_4,
	HISTORY_LOC_IDX_TRAY_PP_CONV_5,
	HISTORY_LOC_IDX_TRAY_PP_CONV_6,
	HISTORY_LOC_IDX_TRAY_PP_CONV_7,

	HISTORY_LOC_IDX_TEST_PP1_LD_TBL_1 = 10,
	HISTORY_LOC_IDX_TEST_PP1_LD_TBL_2,
	HISTORY_LOC_IDX_TEST_PP1_LD_TBL_3,
	HISTORY_LOC_IDX_TEST_PP1_LD_TBL_4,
	HISTORY_LOC_IDX_TEST_PP1_UD_TBL_1,
	HISTORY_LOC_IDX_TEST_PP1_UD_TBL_2,
	HISTORY_LOC_IDX_TEST_PP1_UD_TBL_3,
	HISTORY_LOC_IDX_TEST_PP1_UD_TBL_4,

	HISTORY_LOC_IDX_TEST_PP1_STATION_DN_1,
	HISTORY_LOC_IDX_TEST_PP1_STATION_DN_2,
	HISTORY_LOC_IDX_TEST_PP1_STATION_DN_3,
	HISTORY_LOC_IDX_TEST_PP1_STATION_DN_4,

	HISTORY_LOC_IDX_TEST_PP1_CLEAN_BUFFER,

	HISTORY_LOC_IDX_TEST_PP2_LD_TBL_1 = 30,
	HISTORY_LOC_IDX_TEST_PP2_LD_TBL_2,
	HISTORY_LOC_IDX_TEST_PP2_LD_TBL_3,
	HISTORY_LOC_IDX_TEST_PP2_LD_TBL_4,
	HISTORY_LOC_IDX_TEST_PP2_UD_TBL_1,
	HISTORY_LOC_IDX_TEST_PP2_UD_TBL_2,
	HISTORY_LOC_IDX_TEST_PP2_UD_TBL_3,
	HISTORY_LOC_IDX_TEST_PP2_UD_TBL_4,

	HISTORY_LOC_IDX_TEST_PP2_STATION_DN_1,
	HISTORY_LOC_IDX_TEST_PP2_STATION_DN_2,
	HISTORY_LOC_IDX_TEST_PP2_STATION_DN_3,
	HISTORY_LOC_IDX_TEST_PP2_STATION_DN_4,

	HISTORY_LOC_IDX_TEST_PP2_CLEAN_BUFFER,
};

//---------------------------------------------
// Test Site쌍을 이루도록 구성
//---------------------------------------------
//┌──────┬─────┬─────┬─────┬─────┐
//│      │ 1,2 │ 3,4 │ 5,6 │ 7,8 │
//├─ UP ─┼─────┼─────┼─────┼─────┤
//│      │  1  │  2  │  3  │  4  │
//├──────┼─────┼─────┼─────┼─────┤
//│      │ 1,2 │ 3,4 │ 5,6 │ 7,8 │
//├─DOWN─┼─────┼─────┼─────┼─────┤
//│      │  5  │  6  │  7  │  8  │
//└──────┴─────┴─────┴─────┴─────┘

//==============================================================================
//
//==============================================================================
// 1Station의 Site 구성 갯수
#define STATION_MAX_X_SITE (4)
#define STATION_MAX_Y_SITE (2)
#define STATION_MAX_PARA   (STATION_MAX_X_SITE*STATION_MAX_Y_SITE)
#define STATION_MALRIN_Y_SITE (1)
#define STATION_T_PUT_X_SITE (2)

#define STATION_1_SITE		(0)
#define STATION_2_SITE		(1)
#define STATION_3_SITE		(2)
#define STATION_4_SITE		(3)
#define STATION_5_SITE		(4)
#define STATION_6_SITE		(5)
#define STATION_7_SITE		(6)
#define STATION_8_SITE		(7)

#define STATION_1_PARA		(1)
#define STATION_2_PARA		(2)
#define STATION_3_PARA		(3)
#define STATION_4_PARA		(4)
#define STATION_5_PARA		(5)
#define STATION_6_PARA		(6)
#define STATION_7_PARA		(7)
#define STATION_8_PARA		(8)

// 단위 : um
#define DEF_TRAY_PP_JIG_HAND_X_PITCH			(40000)
#define DEF_TRAY_PP_JIG_HAND_Y_PITCH			(60000)
#define DEF_TRAY_PP_MAX_X_PITCH_WIDE			(46000)
#define DEF_TRAY_PP_MAX_X_PITCH_NARROW			(11000)
#define DEF_TRAY_PP_MAX_Y_PITCH_WIDE			(46000)
#define DEF_TRAY_PP_MAX_Y_PITCH_NARROW			(11000)

#define DEF_TEST_PP_MAX_X_PITCH_WIDE			(46000)
#define DEF_TEST_PP_MAX_X_PITCH_NARROW			(22000)
#define DEF_TEST_PP_MAX_Y_PITCH_WIDE			(60000)
#define DEF_TEST_PP_MAX_Y_PITCH_NARROW			(11000)

#define DEF_TEST_PP_CLEAN_TABLE_PITCH_X			(26000)
#define DEF_TEST_PP_CLEAN_TABLE_PITCH_Y			(23000)

#define DEF_TEST_PP_Y_PITCH                     (42000)

#define MAX_ROBOT_COUNT                     (100)
// TEST HAND ( enum _eTestPPId )
#define MAX_TEST_PP_CNT							(2)
#define MAX_TEST_PP_PKR_CNT_X                   (4)  // hand picker x 열 수
#define MAX_TEST_PP_PKR_CNT_Y                   (2)  // hand picker y 열 수
#define MAX_TEST_PP_PKR_CNT						(MAX_TEST_PP_PKR_CNT_X*MAX_TEST_PP_PKR_CNT_Y)
#define NAX_TEST_PP_HEAD_COUNT					(2)
#define TEST_PP_SAFETY_MOVE_Z_OFFSET			(-200.0)	// test hand z축 안전위치 올릴 경우 사람이 셋팅한것보다 조금 더 올린다.
#define STATION_CENTER_TO_LR_HW_DIST			(130000.0) // sation의 site 중심 기준으로 좌우측 base gap이 남는 기구 계산 값.
#define TEST_PP_Z_SAFETY_SEN_DOG				(35000.0)	// Z축 안전센서 dog의 길이

// LD TABLE
#define MAX_LD_TBL_CNT                      (4)
#define MAX_LD_TBL_POCKET_CNT_X             (4)
#define MAX_LD_TBL_POCKET_CNT_Y             (4)
#define RID_INIT_IDLE_RESERVE               (-10)	// load side로 보냄 ( auto test에서 놓아준 경우 or init s/w)
#define RID_NOT_RESERVE                     (-1)	// test site로 보냄 ( tray가 놓아준 경우 )

// TRAY HAND
#define MAX_TRAY_PP_CNT                     (2)
#define MAX_TRAY_PP_CNT_X                   (4)
#define MAX_TRAY_PP_CNT_Y                   (2)

// TRANSFER
#define MAX_TRANSFER_CNT                    (1)

#define MAX_PICKER_X_CNT					(4)
#define MAX_PICKER_Y_CNT					(2)
#define MAX_PICKER_Z_AXIS_CNT			MAX_PICKER_X_CNT* MAX_PICKER_Y_CNT
// PRESS HAND
#define PRESS_CONTACT_DIST_OFFSET           (-5000.0)	// 2단동작 거리
#define PRESS_MARLIN_CONTACT_DIST_OFFSET           (-15000.0)	// 2단동작 거리
#define PRESS_TPUT_CONTACT_DIST_OFFSET           (-15000.0)	// 2단동작 거리

#define PRESS_CLEAN_CONTACT_DIST_MIN_OFFSET           (5000.0)	// 2단동작 거리
#define PRESS_CLEAN_CONTACT_DIST_MAX_OFFSET           (20000.0)	// 2단동작 거리

#define PRESS_SOCKET_CLEAN_CONTACT_SPEED           (70)	// 2단동작 속도
#define PRESS_MIN_CONTACT_SPEED           (5)	// 2단동작 속도
#define PRESS_MAX_CONTACT_SPEED           (15)//5	// 2단동작 속도

#define DEF_AGNET_QUERY_CYCLE_TIME 			(1000.0)
//==============================================================================
// RID  : Robot 고유 번호 임.
// RIDX : Robot Index, 그룹 내에서의 Index
//==============================================================================
#define RID_PRESS_UNIT_DN_L_BASE            (0)
#define RID_PRESS_UNIT_DN_L_(x)             (RID_PRESS_UNIT_DN_L_BASE+x)
#define RID_PRESS_UNIT_UP_L_BASE            (8)
#define RID_PRESS_UNIT_UP_L_(x)             (RID_PRESS_UNIT_UP_L_BASE+x)
#define RID_PRESS_UNIT_(x)	                (RID_PRESS_UNIT_DN_L_BASE+x)

#define RIDX_PRESS_UNIT_1	                (0)
#define RIDX_PRESS_UNIT_2	                (1)
#define RIDX_PRESS_UNIT_3	                (2)
#define RIDX_PRESS_UNIT_4	                (3)
#define RIDX_PRESS_UNIT_5	                (4)
#define RIDX_PRESS_UNIT_6	                (5)
#define RIDX_PRESS_UNIT_7	                (6)
#define RIDX_PRESS_UNIT_8	                (7)


#define RID_TEST_PP_BASE                    (20)
#define RID_TEST_PP_(x)		                (RID_TEST_PP_BASE+x)
#define RIDX_TEST_PP_1                      (0)
#define RIDX_TEST_PP_2                      (1)

#define RID_LD_TBL_BASE						(30)
#define RID_LD_TBL_(x)		                (RID_LD_TBL_BASE+x)
#define RIDX_LD_TBL_1                       (0)
#define RIDX_LD_TBL_2                       (1)
#define RIDX_LD_TBL_3                       (2)
#define RIDX_LD_TBL_4                       (3)

#define RID_TRAY_PP_BASE					(40)
#define RID_TRAY_PP_(x)		                (RID_TRAY_PP_BASE+x)
#define RIDX_TRAY_PP_1                      (0)
#define RIDX_TRAY_PP_2                      (1)

#define RID_FEEDER_BASE						(50)
#define RID_FEEDER_(x)						(RID_FEEDER_BASE+x)
#define RIDX_FEEDER							(0)
#define RIDX_FEEDER_END_STOPPER				(1)

#define RID_TRANSFER_BASE					(60)
#define RID_TRANSFER_(x)	                (RID_TRANSFER_BASE+x)
#define RIDX_TRANSFER                       (0)

//==============================================================================
// Motor 축번호
//==============================================================================
// 1~3 , 4~9호기 프로그램 이원화로 변경 [3/25/2025 donghyun.shin]
enum eAxisNo {

	eAXIS_TEST_PP_1_X_PITCH = 0,
	eAXIS_TEST_PP_1_X2_PITCH,
	eAXIS_TEST_PP_1_Y_PITCH,
	eAXIS_TEST_PP_1_Y2_PITCH,
	eAXIS_TEST_PP_1_X,
	eAXIS_TEST_PP_1_X2,
	eAXIS_TEST_PP_1_Y,

	eAXIS_PRESS_UNIT_DN1,
	eAXIS_PRESS_UNIT_DN2,
	eAXIS_PRESS_UNIT_DN3,
	eAXIS_PRESS_UNIT_DN4,

	eAXIS_TRAY_PP_1_X,
	eAXIS_TRAY_PP_1_X_PITCH,
	eAXIS_TRAY_PP_1_Y_PITCH,

	eAXIS_TRAY_PP_2_X,
	eAXIS_TRAY_PP_2_Y,
	eAXIS_TRAY_PP_2_X_PITCH,
	eAXIS_TRAY_PP_2_Y_PITCH,

	eAXIS_TRANSFER_X,

	eAXIS_TRAY_FEEDER_Y,
	//eAXIS_TRAY_FEEDER_END_STOPPER,

	eAXIS_LOAD_TABLE_1_ROTATE,
	eAXIS_LOAD_TABLE_2_ROTATE,
	eAXIS_LOAD_TABLE_3_ROTATE,
	eAXIS_LOAD_TABLE_4_ROTATE,

	eAXIS_LOAD_TABLE_1_Y,
	eAXIS_LOAD_TABLE_2_Y,
	eAXIS_LOAD_TABLE_3_Y,
	eAXIS_LOAD_TABLE_4_Y,

	eAXIS_TEST_PP_2_Y,
	eAXIS_TEST_PP_2_X,
	eAXIS_TEST_PP_2_X2,
	eAXIS_TEST_PP_2_X_PITCH,
	eAXIS_TEST_PP_2_X2_PITCH,
	eAXIS_TEST_PP_2_Y_PITCH,
	eAXIS_TEST_PP_2_Y2_PITCH,


	eAXIS_PRESS_UNIT_DN5,
	eAXIS_PRESS_UNIT_DN6,
	eAXIS_PRESS_UNIT_DN7,
	eAXIS_PRESS_UNIT_DN8,

	eAXIS_TRAY_PP_1_Z,
	eAXIS_TEST_PP_1_Z,
	eAXIS_TEST_PP_1_Z2,
	eAXIS_TRAY_PP_2_Z,
	eAXIS_TEST_PP_2_Z,
	eAXIS_TEST_PP_2_Z2,

	eMAX_AXIS_COUNT
};

//===================================================================================COK/BASIC/Category/SLT Program Data
#define MAX_ONE_AXIS_FORM				(10)	// Ex) 0=X, 1=Y, 2=Z, 3=X-pitch, 4=Y-Pitch .....


//==============================================================================
//
//==============================================================================
enum eAXIS_FORM
{
	eAxis_X=0,
	eAxis_Y,
	eAxis_X2,
	eAxis_Z,
	eAxis_X_Pitch,
	eAxis_Y_Pitch,
};

enum eAXIS_FORM_LOADING_TBL
{
	eAxis_Tbl_Y = 0,
	eAxis_Tbl_Rotate
};

enum eAXIS_FORM_FEEDER
{
	eAxis_Feeder_Y = 0
};

enum eAXIS_FORM_TESTPP
{
	eAxis_TestPP_X1 = 0,
	eAxis_TestPP_X2,
	eAxis_TestPP_Y,
	eAxis_TestPP_Z1,
	eAxis_TestPP_Z2,
	eAxis_TestPP_X1_Pitch,
	eAxis_TestPP_X2_Pitch,
	eAxis_TestPP_Y1_Pitch,
	eAxis_TestPP_Y2_Pitch,
};


enum eSIGNAL_TOWER
{
	eSIGNAL_RUNNING = 0,
	eSIGNAL_JAM_CALL,
	eSIGNAL_CYCLE_STOP,
	eSIGNAL_FINISH,
	eSIGNAL_RESET,
	eSIGNAL_PAUSE,
	eSIGNAL_REQUEST,
	eSIGNAL_TEMP_WAITING,
	eSIGNAL_TEST_RUNNING,
	eSIGNAL_START,
	eSIGNAL_TEMP_STABILIZE,
	eSIGNAL_MAX,

	eSIGNAL_PRIORTY = 0,
	eSIGNAL_RED,
	eSIGNAL_YELLOW,
	eSIGNAL_GREEN,
	eSIGNAL_SOUND,
	eSIGNAL_SUB_MAX,
};

enum eLAMP_ACTION{
	eLAMP_NO_ACTION = -1,
	eLAMP_OFF = 0,
	eLAMP_ON,
	eLAMP_BLINK,
};

enum eBUZ_ACTION{
	eBUZ_NO_ACTION = -1, 
	eBUZ_OFF = 0,
	eBUZ_ON_1,
	eBUZ_ON_2,
	eBUZ_ON_3,
	eBUZ_ON_4,
};

enum eErrLevel{
	eErrLevel_Log=0, eErrLevel_Message, eErrLevel_Warn, eErrLevel_Jam, eErrLevel_Critical, eErrLevel_All
};


#define MAX_CYLINDER_RETRY_COUNT        (1)
#define MAX_MOTOR_RETRY_COUNT           (2)
#define MAX_VISION_RETRY_COUNT			(5)


enum eSeqCmdAck{
	eSeqCmdAck_OK = 0,
	eSeqCmdAck_InvalidCmd,
	eSeqCmdAck_InvalidParam
};


//==============================================================================
//
//==============================================================================
#define CMD_EVT_BASE				(0x0000)
#define CMD_EVT_CMD_END				(CMD_EVT_BASE + 1)
#define CMD_EVT_ALARM				(CMD_EVT_BASE + 2)

enum _eTransferIdx
{
	eTransfer =0,
	eMaxTransferCount,
};

enum _eLoadTblIdx{
	eLoadTbl_1=0,
	eLoadTbl_2,
	eLoadTbl_3,
	eLoadTbl_4,
	eMaxTblCount,

	eMaxLoadTblCount =4,//KSJ imsi
};

enum _eTrayPPIdx {
	eTrayPP_1_LD = 0,
	eTrayPP_2_UD,
	eMaxTrayPPCount,//KSJ imsi
};



enum _ePressUnit{
	ePressUnit_Down_1 = 0,
	ePressUnit_Down_2,
	ePressUnit_Down_3,
	ePressUnit_Down_4,
	ePressUnit_Down_5,
	ePressUnit_Down_6,
	ePressUnit_Down_7,
	ePressUnit_Down_8,
	eMaxPressUnitCount,

	ePressRegul_Left=0,
	ePressRegul_Right,
	eMaxPressRegul,
};

enum _eDualTesterNum {
	ePressUnit_Down_1_1 = 0,
	ePressUnit_Down_1_2,
	ePressUnit_Down_2_1,
	ePressUnit_Down_2_2,
	ePressUnit_Down_3_1,
	ePressUnit_Down_3_2,
	ePressUnit_Down_4_1,
	ePressUnit_Down_4_2,
	ePressUnit_Down_5_1,
	ePressUnit_Down_5_2,
	ePressUnit_Down_6_1,
	ePressUnit_Down_6_2,
	ePressUnit_Down_7_1,
	ePressUnit_Down_7_2,
	ePressUnit_Down_8_1,
	ePressUnit_Down_8_2,
	ePressUnit_Up_1_1,
	ePressUnit_Up_1_2,
	ePressUnit_Up_2_1,
	ePressUnit_Up_2_2,
	ePressUnit_Up_3_1,
	ePressUnit_Up_3_2,
	ePressUnit_Up_4_1,
	ePressUnit_Up_4_2,
	ePressUnit_Up_5_1,
	ePressUnit_Up_5_2,
	ePressUnit_Up_6_1,
	ePressUnit_Up_6_2,
	ePressUnit_Up_7_1,
	ePressUnit_Up_7_2,
	ePressUnit_Up_8_1,
	ePressUnit_Up_8_2,

};

#define MAX_PARA (eMaxPressUnitCount * STATION_MAX_X_SITE * STATION_MAX_Y_SITE)
#define HALF_MAX_PARA (eMaxPressUnitCount * STATION_MAX_X_SITE)
#define INLINE_8PARA (STATION_MAX_X_SITE * STATION_MAX_Y_SITE)

#define MAX_TESTER_SINGLE 8
#define MAX_TESTER_DUAL   16
#define MAX_TESTER_AGENT  8
#define MAX_TESTER_EACH_SOCKET	64
#define MAX_TESTER_1EA_SOCKET_CNT 4

enum _eTestPPIdx{
	eTestPP_1 = 0,
	eTestPP_2,
	eMaxTestPPCount, 
};

enum eVacBlowState{
	eVacBlwSt_Unknown = -1,
	eVacBlwSt_Off = 0,
	eVacBlwSt_VacOn,
	eVacBlwSt_BlwOn,
};

#define DEF_PICK                 (0)
#define DEF_PLACE                (1)
#define DEF_EXCH                 (2)

#define DEF_TABLE_TYPE_LOAD_UNLOAD    (0)
#define DEF_TABLE_TYPE_BUFFER         (1)
#define DEF_TABLE_TYPE_CLEAN          (2)

enum ePPCmd {
	ePPCmd_Unknown = -1,
	ePick,
	ePlace,
	eExch
};

enum eAxisActionSelectType
{
	eDlgActionPick = 0,
	eDlgActionPlace,
	eDlgActionExchange,
	eDlgActionOnlyMove,
	eMaxDlgActionCount,
};

enum eDeviceInfo{
	eDvcEmpty = 0,
	eDvcExist,
};

enum eTestSiteInfo{
	eTestSite_Empty = 0,
	eTestSite_DvcLoading,
	eTestSite_ContactStart,
	eTestSite_Contacting,
	eTestSite_TestStart,
	eTestSite_Testing,
	eTestSite_TestEnd,
	eTestSite_TestTimeOut,
	eTestSite_CleanLoading,
    eTestSite_CleanEnd,

	eMaxTestSiteTestCount,

	eTestSite_All_Station_Test = 99,
};


enum eEqpCtrlMode{
	eEqpCtrlMode_Manual=0,eEqpCtrlMode_Auto
};

enum _eUsePicker {
	_eUsePicker_Load = 0,
	_eUsePicker_Unload
};

enum _eExchangeType {
	_eExchangeType_ULP_LP = 0,		// Exchange Unload picker with Load picker
	_eExchangeType_UP_UP			// Exchange Unload picker with Unload picker
};

enum _eVibratortionCyl{
	eCyl_Up =0,
	eCyl_Down
};

enum _eVibratorMode{
	eVibMode_Unloading_Finsih = 0,
	eVibMode_Setplate_Down
};


enum eLoadTbl_Usage {
	eLoadType = 0,
	eUnloadType
};
//==============================================================================
// MACRO CONST : JOG SPEED
//==============================================================================
#define JOG_SPD_LOW_UMPS			(500)   // um/sec
#define JOG_SPD_MID_UMPS			(5000)   // um/sec
#define JOG_SPD_HIG_UMPS			(10000)  // um/sec
#define TEACH_MOVE_SPD_LOW			(30000)
#define TEACH_MOVE_SPD_MIDDLE		(80000)
#define TEACH_MOVE_ACC_LOW			(50)
#define TEACH_MOVE_DEC_LOW			(50)

enum eSeqCmdId{
	eSeqCmd_None = -1,
	eSeqCmd_LoadTbl_Initial,
	eSeqCmd_LoadTbl_MovePos,
	eSeqCmd_LoadTbl_Avoid_MovePos,

	eSeqCmd_PressUnit_AutoHeightCal,
	eSeqCmd_PressUnit_Contact,
	eSeqCmd_PressUnit_Initial,
	eSeqCmd_PressUnit_MovePos,
	eSeqCmd_PressUnit_OfflineTest,
    eSeqCmd_PressUnit_OnlineTest,
	eSeqCmd_PressUnit_TestSiteVisionChk,
    eSeqCmd_PressUnit_SocketCleanContact,
	eSeqCmd_PressUnit_OnlineLoopTest,


	eSeqCmd_Stacker_Initial,
	eSeqCmd_Stacker_Load,
	eSeqCmd_Stacker_TrayDn,
	eSeqCmd_Stacker_TrayUp,
	eSeqCmd_Stacker_Unload,
	eSeqCmd_Stacker_TrayOut,
	eSeqCmd_Stacker_Vibration,
	eSeqCmd_Stacker_FullTray,

	eSeqCmd_TestPP_Initial,
	eSeqCmd_TestPP_MoveExchange,
	eSeqCmd_TestPP_MovePickPlace,
	eSeqCmd_TestPP_MovePos,
    eSeqCmd_TestPP_MoveAlreadyPos,
	eSeqCmd_TestPP_MoveSamePlaceStation,
	eSeqCmd_TestPP_MoveSamePickPlaceLoadTable,
	eSeqCmd_TestPPCmd_PickerHeightCal,

	eSeqCmd_TestPP_FindPickerHole,
	eSeqCmd_TestPP_VatAF,
	eSeqCmd_TestPP_VatAtWCamTch,
	eSeqCmd_TestPP_VatPkrAlign,
	eSeqCmd_TestPP_AutoVacZ,
	eSeqCmd_TestPP_PocketTch,
	eSeqCmd_TestPP_MovePikPlace_TestSite_64Para, 

	eSeqCmd_Transfer_Initial,

 	eSeqCmd_Transfer_MovePos,
 	eSeqCmd_Transfer_MovePickPlace,

	eSeqCmd_TrayPP_EmptyCheck,
	eSeqCmd_TrayPP_Initial,
	eSeqCmd_TrayPP_MovePos,
	eSeqCmd_TrayPP_MovePickPlace,
	eSeqCmd_TrayPP_FindPickerHole,
	eSeqCmd_TrayPP_VatAF,
	eSeqCmd_TrayPP_VatAtWCamTch,
	eSeqCmd_TrayPP_VatPkrAlign,
	eSeqCmd_TrayPP_VatPocketTch,
	eSeqCmd_TrayPP_AutoVacZ,
	eSeqCmd_TrayPP_PickerHeightCal,

	eSeqCmd_TrayLoadPP_Initial,
	eSeqCmd_TrayLoadPP_MovePickPlace,
	eSeqCmd_TrayLoadPP_MovePos,
	eSeqCmd_TrayLoadPP_PickerHeightCal,

	eSeqCmd_TrayUnloadPP_Initial,
	eSeqCmd_TrayUnloadPP_MovePickPlace,
	eSeqCmd_TrayUnloadPP_MovePos,
	eSeqCmd_TrayUnloadPP_PickerHeightCal,
	eSeqCmd_TrayUnloadPP_VatPcd,
	eSeqCmd_TrayUnloadPP_VatBacklash,

	eSeqCmd_UnderStacker_Fwd,
	eSeqCmd_UnderStacker_PTurn,

	eSeqCmd_Feeder_Initial,
	eSeqCmd_Feeder_MovePos,
	eSeqCmd_Feeder_MovePickPlace,

	eSeqCmd_Vibrator,
	eSeqCmd_MultiOrg,

	eSeqCmd_SystemOrigin,
	eSeqCmd_SystemJogMove,

	eMaxSeqCmdIdCount,
};

enum eCylManualOper{
	eCylManualOper_None=-1,
	eCylManualOper_Only_One_Action_and_No_Check_Sensor=0,
	eCylManualOper_Only_One_Action_and_Check_Sensor,
	eCylManualOper_Continue_Action_and_No_Check_Sensor,
	eCylManualOper_Continue_Action_and_Check_Sensor,
	eCylManualOper_Time_Check_Action_Once_and_Sensor_Check,
	eMaxCylManualOperCount,
};

enum eCylManualCmd{
	eCylManualCmd_None=-1,
	eCylManualCmd_Picker_EachUp = 0,
	eCylManualCmd_Picker_EachDown,
	eCylManualCmd_Picker_AllUp,
	eCylManualCmd_Picker_AllDown,
	eCylManualCmd_EachClamp,
	eCylManualCmd_EachUnclamp,
	eCylManualCmd_EachFwd,
	eCylManualCmd_EachBak,
	eCylManualCmd_EachUp,
	eCylManualCmd_EachDown,
	eCylManualCmd_EachHor,
	eCylManualCmd_EachVer,
	eCylManualCmd_AllUp,
	eCylManualCmd_AllDown,
	eCylManualCmd_Sub_EachFwd,
	eCylManualCmd_Sub_EachBak,
	eMaxCylManualCmdCount,
};

enum eDeviceLoadSts {
	eDeviceLoadSts_None = 0,
	eDeviceLoadSts_LoadReady, eDeviceLoadSts_Loading, eDeviceLoadSts_LoadComp, eDeviceLoadSts_OneCycleComplete,
	eDeviceLoadSts_UnloadReady, eDeviceLoadSts_Unloading, eDeviceLoadSts_UnloadComp,
};

enum eSteplateUsage {eSteplateUsage_None = -1, eSteplateUsage_Load = 0, eSteplateUsage_Unload };

enum eContactCylinderSize
{
	eContactCylinderSize_32mm = 0,
	eContactCylinderSize_40mm,
	eContactCylinderSize_55mm,
	eMaxContactCylinderSizeCount,
};

enum eConveyorMoveDir
{
	eConveyorMove_FWD,	// Forward
	eConveyorMove_BKWD	// Backward
};

enum eStacker_Idx
{
	eSTACKER_01,
	eSTACKER_02,
	eSTACKER_03,
	eSTACKER_04,
	eSTACKER_05,
	eSTACKER_06,
	eSTACKER_07,

	eMaxStackerCount,
	eMaxULStackerCount = 4,
};

enum eStacker_Type {
	eStackerType_Undefined = -1,
	eStackerType_Load=0,
	eStackerType_Unload,
	eStackerType_Empty_Unload,
	eStackerType_Empty_Load,
};

enum eStacker_Conveyor_State {
	Blocked,                // C/V가 구동되는 상태
	ReadyToLoad_Tray,			// Tray를 Stage에 투입이 가능 한 상태
	ReadyToUnload_Tray,			// Tray를 Stage로 Unload가 가능 한 상태
	PickEnable_Device,				// Tray에 있는 Device를 Pick 가능한 상태
	PlaceEnable_Device,			// Tray에 Device를 Place 가능한 상태
};

enum eErrorMessagePosition {
	ePos_Stacker_1 = 1,
	ePos_Stacker_2,
	ePos_Stacker_3,
	ePos_Stacker_4,
	ePos_Stacker_5,
	ePos_Stacker_6,
	ePos_Stacker_7,
	ePos_LoadTbl_1,
	ePos_LoadTbl_2,
	ePos_LoadTbl_3,
	ePos_LoadTbl_4,
	ePos_TrayPP_1,
	ePos_TrayPP_2,
	ePos_Feeder,
	ePos_CleanBuf_1,
	ePos_CleanBuf_2,
	ePos_TestPP_1,
	ePos_TestPP_2,
	ePos_Station_1,
	ePos_Station_2,
	ePos_Station_3,
	ePos_Station_4,
	ePos_Station_5,
	ePos_Station_6,
	ePos_Station_7,
	ePos_Station_8,
	ePos_Transfer,
	ePos_MaxPosition,
};

enum eVATPosition {
	eVATPos_Stacker_1 = 1,
	eVATPos_Stacker_2,
	eVATPos_Stacker_3,
	eVATPos_Stacker_4,
	eVATPos_Stacker_5,
	eVATPos_Stacker_6,
	eVATPos_Stacker_7,
	eVATPos_CleanBuf_1,
	eVATPos_CleanBuf_2,
	eVATPos_ARTBuf_1,
	eVATPos_ARTBuf_2,
	eVATPos_TrayLoadTbl_1,
	eVATPos_TrayLoadTbl_2,
	eVATPos_Station_1,
	eVATPos_Station_2,
	eVATPos_Station_3,
	eVATPos_Station_4,
	eVATPos_Station_5,
	eVATPos_Station_6,
	eVATPos_Station_7,
	eVATPos_Station_8,
	eVATPos_Station_9,
	eVATPos_Station_10,
	eVATPos_Station_11,
	eVATPos_Station_12,
	eVATPos_Station_13,
	eVATPos_Station_14,
	eVATPos_Station_15,
	eVATPos_Station_16,
	eVATPos_TrayLowerCam,
	eVATPos_TestLowerCam1,
	eVATPos_TestLowerCam2,
	eVATPos_TestLoadTbl_1,
	eVATPos_TestLoadTbl_2,
	eVATPos_MaxPosition,
};

enum eGrrMode
{
	eNormal_Mode = 0,
	ePressContinueTest,
	ePressReContact,
};


enum eHandlerMode
{
	eFT_Nomarl_Mode = 0,
	eLoop_Mode = 0,
	eGRR_Mode,
	eAudit_Mode,
};

enum eDualTester
{
	eRearTester = 0,
	eFrontTester = 1,
	eMaxTester,
};
enum eEachTesterSocket
{	
	eSocket1 = 0,
	eSocket2,
	eSocket3,
	eSocket4,
	eSocket5,
	eSocket6,
	eSocket7,
	eSocket8,
	eMaxTesterForYanmade // test site는 PC1개당 4socket을 관리 하지 않지만 소켓별로 pccket을 송신 해야 되는 방식임.
};

enum eAgentDevStatusInfo {
	eStatus_Agent_Not_Ready = 0,
	eStatus_Agent_Idle,
	eStatus_Agent_Running,
	eStatus_Agent_Wait_Complete,
	eStatus_Agent_Test_Completed,
	eStatus_Agent_Time_Out,
	eStatus_Agent_Error,
	eStatus_Agent_Offline,
	eMaxStatus_Agent_Count,
};

enum eStationNumber {
	eStationNumber_1,
	eStationNumber_2,
	eStationNumber_3,
	eStationNumber_4,
	eStationNumber_5,
	eStationNumber_6,
	eStationNumber_7,
	eStationNumber_8,
	eStationNumber_9,
	eStationNumber_10,
	eStationNumber_11,
	eStationNumber_12,
	eStationNumber_13,
	eStationNumber_14,
	eStationNumber_15,
	eStationNumber_16,
	eStationNumberMax,
};

enum eYieldKind {
	eYield = 0,
	e1stFail,
};

enum eQAStatus
{
	eQAStatus_Load =0,     // QA 투입 수량
	eQAStatus_LoadEnd,     // QA 추가 투입 수량 확인
	eQAStatus_EmptyLoad,   // QA 투입하고 남은 수량 (Test 진행 하지 않고, Station 거쳐서 Unload 한다.)
	eQAStatus_End,         // Load Empty 까지 마무리 된 상황 종료
};
// enum e2DBarcodeMarkType {
// 	e2DMarkType_Left = 0,
// 	e2DMarkType_Right,
// };

// enum  e2DBarcodeMarkYpe
// {
// 	e2DMarkType_1 = 0,
// 	e2DMarkType_2,
// 	e2DMarkType_3,
// 	e2DMarkTypeMax,
// };



//==============================================================================
// STRUCTURE : Teaching Table에서 Col에 대한 정보를 갖음.
//==============================================================================
typedef struct _ST_LOCATION_HEAD{
	char aszPosName[MAX_POS_IDX_COUNT][128];   // Position의 명칭
	int anPosAxisNo[MAX_POS_IDX_COUNT];       // Position에 해당하는 Motor 축번호 (Logical AxisNo)
	int nPosCount;

	_ST_LOCATION_HEAD()
	{
		memset(this, 0x00, sizeof(_ST_LOCATION_HEAD));
		
		for(int i=0; i<MAX_POS_IDX_COUNT; i++){
			anPosAxisNo[i] = -1;
		}
	};
}ST_LOCATION_HEAD;

typedef struct _ST_SOC_CLEAN_DATA{
    unsigned int nRevDataCnt;
    unsigned int nPassBinCnt;

    _ST_SOC_CLEAN_DATA()
    {
        memset(this, 0x00, sizeof(_ST_SOC_CLEAN_DATA));
    };
}ST_SOC_CLEAN_DATA;

//==============================================================================
// STRUCTURE : Teaching Table에서 Row에 해당 함.
//==============================================================================
typedef struct _ST_LOCATION{
	char   szName[128];
	double dPos[MAX_POS_IDX_COUNT];

	_ST_LOCATION()
	{
		memset(this, 0x00, sizeof(_ST_LOCATION));
	};
}ST_LOCATION;

//==============================================================================
// STRUCTURE : 한개의 Robot Unit이 갖는 Teaching Table
//==============================================================================
typedef struct _ST_ROBOT_TEACHING {
	int               nRobotID;
	char              szRobotName[128];
	ST_LOCATION_HEAD  stLocHead;
	ST_LOCATION       aLoc[MAX_LOC_IDX_COUNT];

	_ST_ROBOT_TEACHING()
	{
		memset(this, 0x00, sizeof(_ST_ROBOT_TEACHING));
	};
}ST_ROBOT_TEACHING;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_SIGNAL_TOWER
{
	int nTower[eSIGNAL_MAX][eSIGNAL_SUB_MAX];

	_ST_SLT_SIGNAL_TOWER()
	{
		memset(this, 0x00, sizeof(_ST_SLT_SIGNAL_TOWER));
	};
}ST_SLT_SIGNAL_TOWER;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SPD_ONE_AXIS
{
	int nSPD;
	int nACC;
	int nDCC;

	_ST_SPD_ONE_AXIS()
	{
		memset(this, 0x00, sizeof(_ST_SPD_ONE_AXIS));
	};
}ST_SPD_ONE_AXIS;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SPD_AXIS
{
	ST_SPD_ONE_AXIS aAxisForm[MAX_ONE_AXIS_FORM];

	_ST_SPD_AXIS()
	{
		memset(this, 0x00, sizeof(_ST_SPD_AXIS));
	};
}ST_SPD_AXIS;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_ARM_TIME
{
	int nVacuumTm;
	int nBlowTm;
	int nRetryCnt;
	int nCylDownWaitTm;
	double dBlowPos;
	double dCleanOffset;
	int nPick_1Stage_Offset;
	int nPick_1Stage_Speed;
	int nLinear_Offset;
	double dPickRetryOffset;
	double dPickerWaitTm;

	_ST_ARM_TIME()
	{
		memset(this, 0x00, sizeof(_ST_ARM_TIME));
	};
}ST_ARM_TIME;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_TRAY_PLATE
{
	int	nStart_PitchX;
	int	nStart_PitchY;
	int	nPitchX;
	int	nPitchY;
	int	nCnt_X;
	int	nCnt_Y;
	int nThickness;

	_ST_TRAY_PLATE()
	{
		memset(this, 0x00, sizeof(_ST_TRAY_PLATE));
	};
}ST_TRAY_PLATE;

struct CPointF
{
	enum {
		XY,XYZ ,Y,X, XZ,
	};
	double x;
	double y;
	double z;
	CPointF() { memset(this, 0x00, sizeof(CPointF)); }
	CPointF(double x, double y) { this->x=x; this->y=y; z=0; }
	CPointF(double x, double y, double z) { this->x=x; this->y=y; this->z=z; }

	CString ToStr(int flag, LPCSTR fmt="%f"){
		CString vfmt,v;
		if( flag == XY || flag == XZ) {
			vfmt.Format("%s,%s", fmt,fmt);
			v.Format(vfmt,x,y);
		}
		else if (flag == Y || flag == X)
		{
			vfmt.Format("%s",  fmt);
			v.Format(vfmt, y);
		}
		else 
		{
			vfmt.Format("%s,%s,%s", fmt,fmt,fmt);
			v.Format(vfmt,x,y,z);
		}
		return v;
	}
};

typedef struct _ST_FW_VERSION {
	int nIndex;
	std::string sVersion;

	_ST_FW_VERSION()
	{
		memset(this, 0x00, sizeof(_ST_FW_VERSION));
	};
	void clear()
	{
		memset(this, 0x00, sizeof(_ST_FW_VERSION));
	}

}ST_FW_VERSION;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_BASIC_INFO
{
	int			 nFTRTQC;				// enum eStartMode
	int			 nVision;				// 0:Use	 1:Not Use
	int			 nStartMode;			// 0:Normal  1:RT
	int			 nRunMode;				// 0:Online  1:OffLine 2:Demo
	BOOL		 bContinueFailMode;		// 0: Off    1:On
	BOOL		 bYieldControlMode;		// 0: Off    1:On
	BOOL		 bContactMode;			// 0: Direct 1:Drop
	BOOL		 bTempMode;				// 0:Ambient 1:Hot
	BOOL		 bAutoSkipMode;			// 0:No-use  1:Use
	double		 dYield_Percent;		// Yield Limit Percent
	int			 nYield_Cnt;			// Yield Control Window count
	int			 nContiFailCnt;			// Yield Contniue Fail Count
	int			 nContiResetCnt;		// Yield Continue Reset Count
	int			 nSiteOnOff[eMaxPressUnitCount][4][2];
// 	int			 nSocketOnOff[eMaxPressUnitCount];	// Site Enable/Disable		-1:Disable 0:Off 1:On
// 	int			 nSocketOnOff64Para[eMaxPressUnitCount][4];	// Site Enable/Disable		-1:Disable 0:Off 1:On
	int			 nOffLineTestTm;		// Offline using Test Time (ms)
	int			 nDropHeight;			// Drop Mode Height        (um)
	int			 nDropTimer;			// Drop Mode DropTime      (ms)
	int			 nContactForcePins;
	double		 dContactcForceGF;

	char		 szSltPgmName[512];
	char		 szCategoryName[512];

	char		 szLotNo[MAX_PATH];
	char		 szScheduleNo[MAX_PATH];
	char		 szOperID[MAX_PATH];
	char		 szDeviceName[MAX_PATH];
	char		 szPgmName[MAX_PATH];
	char		 szCustomerName[MAX_PATH];

	int          nDvcDimensionX;
	int          nDvcDimensionY;

	int          nDeviceMode;         // 0:DeviceExist, 1:No-Device,Exist Tray, 2:No-Device,No-Tray
	BOOL         bAutoRetestSiteUse;  // 0:AutoRetest Nouse, 1:AutoRetestUse
    int          nAutoRetestKind;     // enum eAUTO_RETEST_TEST_PP
	BOOL		 bOffLineBinRandom;
	BOOL		 bOffLineYieldCut;
	double		 dOffLineYieldCutPer;
	BOOL		 bAlwayBin1Sorting;

	// Vibrator
	int			nVibUdDvcActCount;
	double		dVibActDelayTime;
	int         nVibActCnt;

	// Yield Monitor
	BOOL		bYieldMonitor;			// 0:Off      1:On
	int			nSampleQuantity;		// Sample Quantity Value
	int			nPresetCounter;			// Preset Counter Value
	double		dLowYield;				// Low Yield Value
	double      dVariance;				// Variance Yield
	int			nContinueFailureCnt;	// Consecutive Failure Count
	BOOL		bEventType;				// 0:Handler Stop 1:Warning Message Only
	BOOL		bRetestYield;			// 0:Disable  1:Enable
	int			nContinueFailureCnt_Station; // Consecutive Failure Count Station
	int			n1stFailCnt;

	// option
    BOOL        bLoadTblArrived;	 // 0:Disable  1:Enable
	BOOL		bLoadTblAfterMoveArrived; // 0:Disable  1:Enable
	BOOL        bImpactTbl;			 // 0:Disable  1:Enable
    BOOL        bMD_Autokill;		 // 0:Disable  1:Enable
	BOOL        bMD_VisionExist_AAB; // 0:Disable  1:Enable
//	BOOL        bMD_Dvc_HorVer;      // 0:horizontal  1:vertical
	BOOL        bMD_TestPP_DevExist_Check; // 0:Disable  1:Enable
	BOOL        bMD_TestPP_After_DevExist_Check; // 0:Disable  1:Enable
	BOOL        bMD_TestPP_DevExist_Check_Sensor; // 0:Disable  1:Enable
	BOOL        bMD_TestPP_ArtDevice_PickPlace;	// 0:Disable  1:Enable
	BOOL        bMD_TestPP_PreVacuum;	// 0:disable, 1:Enable
	BOOL        bMD_TrayPP_PreVacuum;
	BOOL        bMD_Consecutive_Fail_AutoKill;// 0:Disable  1:Enable
	BOOL        bMD_Consecutive_Fail_AutoKill_Station;// 0:Disable  1:Enable
	BOOL		bMD_TestPP_PNP_SiteArrived_Sen;
	BOOL		bMD_TestPP_PNP_SiteMoveArrived_Sen;
	BOOL		bMD_TestppStationFR_PNP;
	BOOL		bMD_LoadHandAlreadyPick;
	BOOL        bMD_TestSite_Retouch;
	double      dTestSite_Retouch_Dis;
    // socket cleaning
    BOOL        bMD_SocketClean ;           // 0:Off 1:On
    BOOL        bMD_CleanLoadTblArrivedSen; // 0:Off 1:On
    BOOL        bMD_CleanInitStart;
    BOOL        bMD_CleanInitRetestStart;
    BOOL        bMD_CleanIntervalCnt;
    BOOL        bMD_CleanLowYield;
    BOOL        bMD_EventType;             //0:Handler Stop 1:Warning Message Only
	int         nCleanIntervalCnt;
    int         nCleanContacTm;             // ms
    int         nCleanContacCnt;
    int         nCleanPiecesCnt;
    int         nCleanAlarmCnt;       
    double      dCleanLowYield;
	double      dContactDelayTime;
	BOOL        bMD_1stFailAutokill;		 // 0:Disable  1:Enable

	BOOL        b2DTestSiteMove;  // 2D Test Site Move
	int			nBarcodeRetryCnt;
	int			nNumberOfLoadTable;

	int         nAutoSkipCount; // AutoSkip Count

	BOOL        bRetestSkip_HW1;
	BOOL        bRetestSkip_HW2;
	BOOL        bRetestSkip_HW3;
	BOOL        bRetestSkip_HW4;

	//Speed Press Unit Option
	BOOL		bPressDeviceChk;	// 0 : Disable 1 : Enable
	int			nPressDeviceChkOpt;	// 0 : All Socket 1 : Only Socket Open
	int			nPressVibratorChkOpt; // 1: USE , 0: NOT_USE
	int			nPressVibratorRetryCnt;
	double		dPressVibratorDelayTime;

	//Alway Vibrator
	int			nPressAlwayVibratorChkOpt; // 1: USE , 0: NOT_USE
	int			nPressAlwayVibratorRetryCnt;
	double		dPressAlwayVibratorDelayTime;

	int			nLoadTableVibratorChkOpt; // 1: USE , 0: NOT_USE
	int			nLoadTableVibratorRetryCnt;
	double		dLoadTableVibratorDelayTime;

	int			m_nNumberOfLoadTable; // 0 : 4Unit, 1: 8Unit

	double      dLoadTableWaitTime;

	int			nPressBlowPosChkOpt;

	int			nCleanOffsetOption;

	double		dContactPos0ffset;

	int			nVibrationOption;
	int			nContactLogOption;
	int		    nSocketCleanFunctionOption;
	int			nContact2stSpeed;
	double		dCleaning2ndZOffset;

	double		dSocketCleanBlowPos;
	double		dSocketCleanBlowTime;
	// GRR OPTION
	int			nAutoGrrTestCount;
	int			nAutoGrrUnitCount;
	BOOL		bGRR_Device_swap;
	BOOL		bGRR_Audit_loop;
	int			nGRRSiteOnOff[ eMaxPressUnitCount ][ STATION_MAX_Y_SITE ][ STATION_MAX_X_SITE ];
	int			nAutoGrrMode;
	int         nGrrSotDelay;

	char		 szMasterMacPath[MAX_PATH];
	BOOL         bCBCheckAlloff;
	BOOL		 bCBDataCheckOnOff;
	char		 szObsidianPath[MAX_PATH];
	char		 szObsidianCreatePath[MAX_PATH];
	char		 szDeleteLogPath[MAX_PATH];

	// QA OPTION
	int			nAutoQADefaultCount;
	// LOOP OPTION
	int			nAutoLoopTestCount;
	int         nLoopSotDelay;
	int         nAutoLoopmode;
	int			nAutoLoopUnitCount;
	int         nAutoLoopRePickPlaceCount;
	int			nAutoLoopReContactCount;
	// Audit Option
	int			nAuditSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	int			nAutoAuditTestCount;
	int			nAutoAuditUnitCount;
	int			nAutoAuditMode;
	int			nAuditSotDelay;

	int			nDivision;

	int			nBoardType;
	_ST_BASIC_INFO()
	{
		memset(this, 0x00, sizeof(_ST_BASIC_INFO));
	};

	//Load Table GUI Change Data Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameLoadTable(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		std::pair<CString,std::pair<double,double>> p;

		if(this->bLoadTblArrived != stAfterBasicData.bLoadTblArrived)
		{
			p.first = "Arrived";
			p.second.first = this->bLoadTblArrived;
			p.second.second = stAfterBasicData.bLoadTblArrived;

			vData.push_back(p);
		}
		if (this->bLoadTblAfterMoveArrived != stAfterBasicData.bLoadTblAfterMoveArrived)
		{
			p.first = "After Move Arrived";
			p.second.first = this->bLoadTblAfterMoveArrived;
			p.second.second = stAfterBasicData.bLoadTblAfterMoveArrived;

			vData.push_back(p);
		}
		if(this->bImpactTbl != stAfterBasicData.bImpactTbl)
		{
			p.first = "Impact Load Table";
			p.second.first = this->bImpactTbl;
			p.second.second = stAfterBasicData.bImpactTbl;

			vData.push_back(p);
		}
// 		if(this->bMD_Dvc_HorVer != stAfterBasicData.bMD_Dvc_HorVer){
// 			p.first = "Device Hor Ver";
// 			p.second.first = this->b2dBarcode;
// 			p.second.second = stAfterBasicData.b2dBarcode;
// 
// 			vData.push_back(p);
// 		}
	}

	// Tray Setup GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameTrayFile(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		std::pair<CString,std::pair<double,double>> p;

		if(this->bTempMode != stAfterBasicData.bTempMode)
		{ 
			p.first = "Plate Form Temp";
			p.second.first = this->bTempMode;
			p.second.second = stAfterBasicData.bTempMode;

			vData.push_back(p);
		}
	}
	// Device Setup GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameDeviceSetup(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString,std::pair<CString,CString>>>& vData)
	{
		std::pair<CString,std::pair<CString,CString>> p;

		if(strcmp(this->szCustomerName,stAfterBasicData.szCustomerName) != 0)
		{
			p.first = "Customer";
			p.second.first = this->szCustomerName;
			p.second.second = stAfterBasicData.szCustomerName;

			vData.push_back(p);
		}
		if(strcmp(this->szDeviceName,stAfterBasicData.szDeviceName) != 0)
		{
			p.first = "Device Name";
			p.second.first = this->szDeviceName;
			p.second.second = stAfterBasicData.szDeviceName;

			vData.push_back(p);
		}
		if(strcmp(this->szPgmName,stAfterBasicData.szPgmName) != 0)
		{
			p.first = "Program Name";
			p.second.first = this->szPgmName;
			p.second.second = stAfterBasicData.szPgmName;

			vData.push_back(p);
		}
		if(strcmp(this->szSltPgmName,stAfterBasicData.szSltPgmName) != 0)
		{
			p.first = "SLT Program";
			p.second.first = this->szSltPgmName;
			p.second.second = stAfterBasicData.szSltPgmName;

			vData.push_back(p);
		}
		if(strcmp(this->szCategoryName,stAfterBasicData.szCategoryName) != 0)
		{
			p.first = "Category";
			p.second.first = this->szCategoryName;
			p.second.second = stAfterBasicData.szCategoryName;

			vData.push_back(p);
		}
	}

	// Socket Cleaning GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameSocketCleaning(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>>& vData)
	{
		std::pair<std::pair<CString,int>,std::pair<double,double>> p;

		if(this->bMD_SocketClean != stAfterBasicData.bMD_SocketClean)
		{
			p.first.first = "Cleaning";
			p.first.second = 1;
			p.second.first = this->bMD_SocketClean;
			p.second.second = stAfterBasicData.bMD_SocketClean;

			vData.push_back(p);
		}
		if(this->bMD_CleanLoadTblArrivedSen != stAfterBasicData.bMD_CleanLoadTblArrivedSen)
		{
			p.first.first = "LoadingTable arrival check sensor";
			p.first.second = 1;
			p.second.first = this->bMD_CleanLoadTblArrivedSen;
			p.second.second = stAfterBasicData.bMD_CleanLoadTblArrivedSen;

			vData.push_back(p);
		}
		if(this->nCleanContacTm != stAfterBasicData.nCleanContacTm)
		{
			p.first.first = "Contact Time";
			p.first.second = 0;
			p.second.first = this->nCleanContacTm / 1000;
			p.second.second = stAfterBasicData.nCleanContacTm / 1000;

			vData.push_back(p);
		}
		if(this->nCleanContacCnt != stAfterBasicData.nCleanContacCnt)
		{
			p.first.first = "Contact Count";
			p.first.second = 0;
			p.second.first = this->nCleanContacCnt;
			p.second.second = stAfterBasicData.nCleanContacCnt;

			vData.push_back(p);
		}
		if(this->nCleanAlarmCnt != stAfterBasicData.nCleanAlarmCnt)
		{
			p.first.first = "Alarm Count";
			p.first.second = 0;
			p.second.first = this->nCleanAlarmCnt;
			p.second.second = stAfterBasicData.nCleanAlarmCnt;

			vData.push_back(p);
		}
		if(this->nCleanPiecesCnt != stAfterBasicData.nCleanPiecesCnt)
		{
			p.first.first = "Number of pieces";
			p.first.second = 0;
			p.second.first = this->nCleanPiecesCnt;
			p.second.second = stAfterBasicData.nCleanPiecesCnt;

			vData.push_back(p);
		}
		if(this->nCleanIntervalCnt != stAfterBasicData.nCleanIntervalCnt)
		{
			p.first.first = "Interval";
			p.first.second = 0;
			p.second.first = this->nCleanIntervalCnt;
			p.second.second = stAfterBasicData.nCleanIntervalCnt;

			vData.push_back(p);
		}
		if(this->bMD_CleanInitStart != stAfterBasicData.bMD_CleanInitStart)
		{
			p.first.first = "Initial Start";
			p.first.second = 1;
			p.second.first = this->bMD_CleanInitStart;
			p.second.second = stAfterBasicData.bMD_CleanInitStart;

			vData.push_back(p);
		}
		if(this->bMD_CleanInitRetestStart != stAfterBasicData.bMD_CleanInitRetestStart)
		{
			p.first.first = "Initial Retest Start";
			p.first.second = 1;
			p.second.first = this->bMD_CleanInitRetestStart;
			p.second.second = stAfterBasicData.bMD_CleanInitRetestStart;

			vData.push_back(p);
		}
		if(this->bMD_CleanIntervalCnt != stAfterBasicData.bMD_CleanIntervalCnt)
		{
			p.first.first = "Interval";
			p.first.second = 1;
			p.second.first = this->bMD_CleanIntervalCnt;
			p.second.second = stAfterBasicData.bMD_CleanIntervalCnt;

			vData.push_back(p);
		}
		if(this->bMD_CleanLowYield != stAfterBasicData.bMD_CleanLowYield)
		{
			p.first.first = "Low Yield";
			p.first.second = 1;
			p.second.first = this->bMD_CleanLowYield;
			p.second.second = stAfterBasicData.bMD_CleanLowYield;

			vData.push_back(p);
		}
		if(this->dCleanLowYield != stAfterBasicData.dCleanLowYield)
		{
			p.first.first = "Low Yield";
			p.first.second = 0;
			p.second.first = this->dCleanLowYield;
			p.second.second = stAfterBasicData.dCleanLowYield;

			vData.push_back(p);
		}
		if(this->bMD_EventType != stAfterBasicData.bMD_EventType)
		{
			p.first.first = "Event Type";
			p.first.second = 1;
			p.second.first = this->bMD_EventType;
			p.second.second = stAfterBasicData.bMD_EventType;

			vData.push_back(p);
		}		
	}


	// Yield Monitoring GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameYieldMonitor(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>>& vData)
	{
		std::pair<std::pair<CString,int>,std::pair<double,double>> p;

		if(this->bYieldMonitor != stAfterBasicData.bYieldMonitor)
		{
			p.first.first = "Yield Monitoring";
			p.first.second = 1;
			p.second.first = this->bYieldMonitor;
			p.second.second = stAfterBasicData.bYieldMonitor;

			vData.push_back(p);
		}
		if(this->bEventType != stAfterBasicData.bEventType)
		{
			p.first.first = "Event Type";
			p.first.second = 1;
			p.second.first = this->bEventType;
			p.second.second = stAfterBasicData.bEventType;

			vData.push_back(p);
		}
		if(this->bRetestYield != stAfterBasicData.bRetestYield)
		{
			p.first.first = "Re-Test Mode Yield Monitoring";
			p.first.second = 1;
			p.second.first = this->bRetestYield;
			p.second.second = stAfterBasicData.bRetestYield;

			vData.push_back(p);
		}
		if(this->nContinueFailureCnt != stAfterBasicData.nContinueFailureCnt)
		{
			p.first.first = "Consecutive Failure Count (Each Site)";
			p.first.second = 0;
			p.second.first = this->nContinueFailureCnt;
			p.second.second = stAfterBasicData.nContinueFailureCnt;

			vData.push_back(p);
		}

		if (this->nContinueFailureCnt_Station != stAfterBasicData.nContinueFailureCnt_Station)
		{
			p.first.first = "Consecutive Failure Count (Station)";
			p.first.second = 0;
			p.second.first = this->nContinueFailureCnt_Station;
			p.second.second = stAfterBasicData.nContinueFailureCnt_Station;

			vData.push_back(p);
		}

		if(this->nSampleQuantity != stAfterBasicData.nSampleQuantity)
		{
			p.first.first = "Sampling Quantity";
			p.first.second = 0;
			p.second.first = this->nSampleQuantity;
			p.second.second = stAfterBasicData.nSampleQuantity;

			vData.push_back(p);
		}
		if(this->nPresetCounter != stAfterBasicData.nPresetCounter)
		{
			p.first.first = "Preset Counter";
			p.first.second = 0;
			p.second.first = this->nPresetCounter;
			p.second.second = stAfterBasicData.nPresetCounter;

			vData.push_back(p);
		}
		if(this->dLowYield != stAfterBasicData.dLowYield)
		{
			p.first.first = "Low Yield";
			p.first.second = 0;
			p.second.first = this->dLowYield;
			p.second.second = stAfterBasicData.dLowYield;

			vData.push_back(p);
		}
		if(this->dVariance != stAfterBasicData.dVariance){
			p.first.first = "Variance";
			p.first.second = 0;
			p.second.first = this->dVariance;
			p.second.second = stAfterBasicData.dVariance;
		}
		if(this->bMD_Autokill != stAfterBasicData.bMD_Autokill)
		{
			p.first.first = "Auto Socket Off";
			p.first.second = 1;
			p.second.first = this->bMD_Autokill;
			p.second.second = stAfterBasicData.bMD_Autokill;

			vData.push_back(p);
		}

		if (this->bMD_Consecutive_Fail_AutoKill != stAfterBasicData.bMD_Consecutive_Fail_AutoKill)
		{
			p.first.first = "Consecutive Fail AutoKill (Each Site)";
			p.first.second = 1;
			p.second.first = this->bMD_Consecutive_Fail_AutoKill_Station;
			p.second.second = stAfterBasicData.bMD_Consecutive_Fail_AutoKill_Station;

			vData.push_back(p);
		}
		
		if (this->bMD_Consecutive_Fail_AutoKill_Station!= stAfterBasicData.bMD_Consecutive_Fail_AutoKill_Station)
		{
			p.first.first = "Consecutive Fail AutoKill (Station)";
			p.first.second = 1;
			p.second.first = this->bMD_Consecutive_Fail_AutoKill_Station;
			p.second.second = stAfterBasicData.bMD_Consecutive_Fail_AutoKill_Station;

			vData.push_back(p);
		}
		if (this->bMD_1stFailAutokill != stAfterBasicData.bMD_1stFailAutokill)
		{
			p.first.first = "1st Fail AutoKill";
			p.first.second = 1;
			p.second.first = this->bMD_1stFailAutokill;
			p.second.second = stAfterBasicData.bMD_1stFailAutokill;

			vData.push_back(p);
		}
	}

	// Contact Control GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameYieldSet(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>>& vData)
	{
		std::pair<std::pair<CString,int>,std::pair<double,double>> p;

		if(this->bContactMode != stAfterBasicData.bContactMode)
		{
			p.first.first = "Contact Mode";
			p.first.second = 1;
			p.second.first = this->bContactMode;
			p.second.second = stAfterBasicData.bContactMode;

			vData.push_back(p);
		}
		if(this->nDropHeight != stAfterBasicData.nDropHeight)
		{
			p.first.first = "Drop Height";
			p.first.second = 0;
			p.second.first = (double)this->nDropHeight / 1000;
			p.second.second = (double)stAfterBasicData.nDropHeight / 1000;

			vData.push_back(p);
		}
		if(this->nDropTimer != stAfterBasicData.nDropTimer)
		{
			p.first.first = "Drop Time";
			p.first.second = 0;
			p.second.first = (double)this->nDropTimer / 1000;
			p.second.second = (double)stAfterBasicData.nDropTimer / 1000;

			vData.push_back(p);
		}
		if(this->nContactForcePins != stAfterBasicData.nContactForcePins)
		{
			p.first.first = "Pins(Balls)";
			p.first.second = 0;
			p.second.first = (double)this->nContactForcePins;
			p.second.second = (double)stAfterBasicData.nContactForcePins;

			vData.push_back(p);
		}
		if(this->dContactcForceGF != stAfterBasicData.dContactcForceGF)
		{
			p.first.first = "Force Per Pin";
			p.first.second = 0;
			p.second.first = (double)this->dContactcForceGF;
			p.second.second = (double)stAfterBasicData.dContactcForceGF;

			vData.push_back(p);
		}																							

		if(this->nDvcDimensionX != stAfterBasicData.nDvcDimensionX)
		{
			p.first.first = "Dimension X";
			p.first.second = 0;
			p.second.first = (double)this->nDvcDimensionX / 1000;
			p.second.second = (double)stAfterBasicData.nDvcDimensionX / 1000;

			vData.push_back(p);
		}
		if(this->nDvcDimensionY != stAfterBasicData.nDvcDimensionY)
		{
			p.first.first = "Dimension Y";
			p.first.second = 0;
			p.second.first = (double)this->nDvcDimensionY / 1000;
			p.second.second = (double)stAfterBasicData.nDvcDimensionY/ 1000;

			vData.push_back(p);
		}
	}
	
	CString SiteOnOffName(int nSiteOnOff)
	{
		CString strSiteOnOffName = _T("");
		switch (nSiteOnOff)
		{
		case DEF_SITE_OFF: {strSiteOnOffName = "Off";}break;
		case DEF_SITE_ON: {strSiteOnOffName = "On";}break;
		case DEF_SITE_OFF_RESERVATION: {strSiteOnOffName = "Reservation";}break;
		case DEF_SITE_DISABLE: {strSiteOnOffName = "Disable";}break;
		case DEF_SITE_OFF_YIELD: {strSiteOnOffName = "Yield Off";}break;
		case DEF_SITE_OFF_ERRORCODE: {strSiteOnOffName = "ErrorCode Off";}break;
		case DEF_SITE_SOCKET_RETEST_OK: {strSiteOnOffName = "Retest OK";}break;
		case DEF_SITE_SOCKET_HOLD: {strSiteOnOffName = "Hold";}break;
		case DEF_SITE_SOCKET_LOCK: {strSiteOnOffName = "Lock";}break;
		case DEF_SITE_OFF_RESERVATION_YIELD: {strSiteOnOffName = "Reservation Yield";}break;
		default:
			break;
		}
		return strSiteOnOffName;
	}


	// Site Setting GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameSiteSet(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString,std::pair<CString,CString>>>& vData)
	{
		std::pair<CString,std::pair<CString,CString>> p;

		CString strSite =_T("");
		for(int nIdx=0; nIdx<eMaxPressUnitCount; nIdx++)
		{
			for(int nX =0; nX < 2/*STATION_MAX_X_SITE*/; nX++)
			{
				for(int nY =0; nY < 4/*STATION_MAX_Y_SITE*/; nY++)
				{
					strSite.Format(_T("Station%d_Site[%d][%d]"),nIdx+1,nY,nX);

					if(this->nSiteOnOff[nIdx][nY][nX] != stAfterBasicData.nSiteOnOff[nIdx][nY][nX])
					{
						p.first = "Site Enable/Disable " + strSite;

						p.second.first = SiteOnOffName(this->nSiteOnOff[nIdx][nY][nX]);
						p.second.second = SiteOnOffName(stAfterBasicData.nSiteOnOff[nIdx][nY][nX]);
// 						if( this->nSiteOnOff[nIdx][nY][nX] == DEF_ON )
// 						{
// 							p.second.first = "On";
// 							p.second.second = "Off";
// 						}
// 						else
// 						{
// 							p.second.first = "Off";
// 							p.second.second = "On";
// 						}

						vData.push_back(p);
					}
				}
			}
		}

		if(this->nRunMode != stAfterBasicData.nRunMode)
		{
			CString strBeforeRunMode =_T("");
			CString strAfterRunMode =_T("");

			// 0:Online  1:OffLine 2:Demo
			p.first = "Run Mode";

			if(this->nRunMode == 0)
			{
				strBeforeRunMode =_T("On Line");
			}
			else if(this->nRunMode == 1)
			{
				strBeforeRunMode =_T("Off Line");
			}
			else if(this->nRunMode ==2)
			{
				strBeforeRunMode =_T("Demo");
			}

			if(stAfterBasicData.nRunMode == 0)
			{
				strAfterRunMode =_T("On Line");
			}
			else if(stAfterBasicData.nRunMode == 1)
			{
				strAfterRunMode =_T("Off Line");
			}
			else if(stAfterBasicData.nRunMode ==2)
			{
				strAfterRunMode =_T("Demo");
			}

			p.second.first = strBeforeRunMode;
			p.second.second = strAfterRunMode;

			vData.push_back(p);
		}
		
		if(this->nDeviceMode != stAfterBasicData.nDeviceMode)
		{
			CString strBeforeDeviceMode =_T("");
			CString strAfterDeviceMode =_T("");

			// 0:DeviceExist, 1:No-Device,Exist Tray, 2:No-Device,No-Tray
			p.first = "Device Mode";

			if(this->nDeviceMode == 0)
			{
				strBeforeDeviceMode =_T("Device");
			}
			else if(this->nDeviceMode == 1)
			{
				strBeforeDeviceMode =_T("No-Device,Exist");
			}
			else if(this->nDeviceMode ==2)
			{
				strBeforeDeviceMode =_T("No-Device,No-Tray");
			}

			if(stAfterBasicData.nDeviceMode == 0)
			{
				strAfterDeviceMode =_T("DeviceExist");
			}
			else if(stAfterBasicData.nDeviceMode == 1)
			{
				strAfterDeviceMode =_T("No-Device,Exist");
			}
			else if(stAfterBasicData.nDeviceMode ==2)
			{
				strAfterDeviceMode =_T("No-Device,No-Tray");
			}

			p.second.first = strBeforeDeviceMode;
			p.second.second = strAfterDeviceMode;

			vData.push_back(p);
		}
		
		if(this->nOffLineTestTm != stAfterBasicData.nOffLineTestTm)//   / 1000
		{
			p.first = "Test Time";
			p.second.first.Format(_T("%.2f"),(double) this->nOffLineTestTm / 1000);
			p.second.second.Format(_T("%.2f"),(double) stAfterBasicData.nOffLineTestTm/ 1000); 

			vData.push_back(p);
		}		

		if (this->bAutoRetestSiteUse != stAfterBasicData.bAutoRetestSiteUse)
		{
			p.first = "Auto ReTest Use";
			if(this->bAutoRetestSiteUse == 0)
			{
				p.second.first =_T("NoUse");
				p.second.second =_T("Use");
			}
			else
			{
				p.second.first =_T("Use");
				p.second.second =_T("NoUse");
			}

			vData.push_back(p);
		}

		if(stAfterBasicData.bAutoRetestSiteUse == DEF_ON)
		{
			if(this->nAutoRetestKind != stAfterBasicData.nAutoRetestKind)
			{
				CString strBeforeKind =_T("");
				CString strAfterKind = _T("");

				p.first = "Test PP AutoRetest Kind";
				
				if(this->nAutoRetestKind == 0)
				{
					strBeforeKind =_T("A Only");
				}
				else if(this->nAutoRetestKind == 1)
				{
					strBeforeKind =_T("AB");
				}
				else if(this->nAutoRetestKind == 2)
				{
					strBeforeKind =_T("AAB");
				}
				else if(this->nAutoRetestKind == 3)
				{
					strBeforeKind =_T("AA");
				}

				if(stAfterBasicData.nAutoRetestKind == 0)
				{
					strAfterKind =_T("A Only");
				}
				else if(stAfterBasicData.nAutoRetestKind == 1)
				{
					strAfterKind =_T("AB");
				}
				else if(stAfterBasicData.nAutoRetestKind == 2)
				{
					strAfterKind =_T("AAB");
				}
				else if(stAfterBasicData.nAutoRetestKind == 3)
				{
					strAfterKind =_T("AA");
				}
				p.second.first = strBeforeKind;
				p.second.second = strAfterKind;

				vData.push_back(p);
			}

			if(this->bRetestSkip_HW1 != stAfterBasicData.bRetestSkip_HW1){
				p.first = "AutoRetest Skip HW Bin1";

				if(this->bRetestSkip_HW1 == 0){
					p.second.first =_T("NoUse");
					p.second.second =_T("Use");
				}else{
					p.second.first =_T("Use");
					p.second.second =_T("NoUse");
				}
				vData.push_back(p);
			}

			if(this->bRetestSkip_HW2 != stAfterBasicData.bRetestSkip_HW2){
				p.first = "AutoRetest Skip HW Bin2";

				if(this->bRetestSkip_HW2 == 0){
					p.second.first =_T("NoUse");
					p.second.second =_T("Use");
				}else{
					p.second.first =_T("Use");
					p.second.second =_T("NoUse");
				}
				vData.push_back(p);
			}

			if(this->bRetestSkip_HW3 != stAfterBasicData.bRetestSkip_HW3){
				p.first = "AutoRetest Skip HW Bin3";

				if(this->bRetestSkip_HW3 == 0){
					p.second.first =_T("NoUse");
					p.second.second =_T("Use");
				}else{
					p.second.first =_T("Use");
					p.second.second =_T("NoUse");
				}
				vData.push_back(p);
			}

			if(this->bRetestSkip_HW4 != stAfterBasicData.bRetestSkip_HW4){
				p.first = "AutoRetest Skip HW Bin4";
				
				if(this->bRetestSkip_HW4 == 0){
					p.second.first =_T("NoUse");
					p.second.second =_T("Use");
				}else{
					p.second.first =_T("Use");
					p.second.second =_T("NoUse");
				}
				vData.push_back(p);
			}
		}

		if (this->bOffLineBinRandom != stAfterBasicData.bOffLineBinRandom)
		{
			p.first = "OffLineBinRandom";
			if (this->bOffLineBinRandom == 0)
			{
				p.second.first = _T("NoUse");
				p.second.second = _T("Use");
			}
			else
			{
				p.second.first = _T("Use");
				p.second.second = _T("NoUse");
			}

			vData.push_back(p);
		}
		if (this->bOffLineYieldCut != stAfterBasicData.bOffLineYieldCut)
		{
			p.first = "OffLineBinRandom";
			if (this->bOffLineYieldCut == 0)
			{
				p.second.first = _T("NoUse");
				p.second.second = _T("Use");
			}
			else
			{
				p.second.first = _T("Use");
				p.second.second = _T("NoUse");
			}

			vData.push_back(p);
		}
		if (this->bAlwayBin1Sorting != stAfterBasicData.bAlwayBin1Sorting)
		{
			p.first = "bAlwayBin1Sorting";
			if (this->bAlwayBin1Sorting == 0)
			{
				p.second.first = _T("NoUse");
				p.second.second = _T("Use");
			}
			else
			{
				p.second.first = _T("Use");
				p.second.second = _T("NoUse");
			}

			vData.push_back(p);
		}

		if (this->dOffLineYieldCutPer != stAfterBasicData.dOffLineYieldCutPer)
		{
			p.first = "OffLine Yield Cut Percent";
			p.second.first.Format(_T("%.2f"), (double) this->dOffLineYieldCutPer);
			p.second.second.Format(_T("%.2f"), (double)stAfterBasicData.dOffLineYieldCutPer);

			vData.push_back(p);
		}
	}

	// GRR Site Setting GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameGRRSiteSet( _ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString, std::pair<CString, CString>>>& vData )
	{
		std::pair<CString, std::pair<CString, CString>> p;

		if( this->nAutoGrrUnitCount != stAfterBasicData.nAutoGrrUnitCount )
		{
			p.first = "Auto GRR Parameter - Unit Count";
			p.second.first.Format( _T( "%d" ), this->nAutoGrrUnitCount );
			p.second.second.Format( _T( "%d" ), stAfterBasicData.nAutoGrrUnitCount );
			vData.push_back(p);
		}

		if( this->nAutoGrrTestCount != stAfterBasicData.nAutoGrrTestCount )
		{
			p.first = "Auto GRR Parameter - Test Count";
			p.second.first.Format( _T( "%d" ), this->nAutoGrrTestCount );
			p.second.second.Format( _T( "%d" ), stAfterBasicData.nAutoGrrTestCount );
			vData.push_back(p);
		}

		//if (this->nAutoGrrMode != stAfterBasicData.nAutoGrrMode)
		//{
		//	p.first = "Auto GRR Parameter - Grr Mode";
		//	p.second.first.Format(_T("%d"), this->nAutoGrrMode);
		//	p.second.second.Format(_T("%d"), stAfterBasicData.nAutoGrrMode);
		//	vData.push_back(p);
		//}

		if( this->bGRR_Device_swap != stAfterBasicData.bGRR_Device_swap ) {
			p.first = "Auto GRR Mode - Device Swap";

			if( this->bGRR_Device_swap == 0 ) {
				p.second.first = _T( "NoUse" );
				p.second.second = _T( "Use" );
			}
			else {
				p.second.first = _T( "Use" );
				p.second.second = _T( "NoUse" );
			}
			vData.push_back( p );
		}

		if( this->bGRR_Audit_loop != stAfterBasicData.bGRR_Audit_loop ) {
			p.first = "Auto GRR Mode - Audit Loop";

			if( this->bGRR_Audit_loop == 0 ) {
				p.second.first = _T( "NoUse" );
				p.second.second = _T( "Use" );
			}
			else {
				p.second.first = _T( "Use" );
				p.second.second = _T( "NoUse" );
			}
			vData.push_back( p );
		}

		if (this->nAutoGrrMode != stAfterBasicData.nAutoGrrMode) {
			p.first = "Auto GRR Mode";

			switch (this->nAutoGrrMode)
			{
			case eNormal_Mode: 
			{
				p.second.first.Format(_T("Normal"));
			}break;
			case ePressContinueTest:
			{
				p.second.first.Format(_T("ContinueTest"));
			}break;
			case ePressReContact:
			{
				p.second.first.Format(_T("ReContact"));
			}break;
			}

			switch (stAfterBasicData.nAutoGrrMode)
			{
			case eNormal_Mode:
			{
				p.second.second.Format(_T("Normal"));
			}break;
			case ePressContinueTest:
			{
				p.second.second.Format(_T("ContinueTest"));
			}break;
			case ePressReContact:
			{
				p.second.second.Format(_T("ReContact"));
			}break;
			}
			vData.push_back(p);
		}

		if (this->nGrrSotDelay != stAfterBasicData.nGrrSotDelay) {
			p.first = "GRR SOT Delay";
			p.second.first.Format(_T("%.2f"), this->nGrrSotDelay/ 1000.0 );
			p.second.second.Format(_T("%.2f"), stAfterBasicData.nGrrSotDelay / 1000.0);

			vData.push_back(p);
		}


		CString strSite = _T( "" );
		for( int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++ )
		{
			for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ )
			{
				for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ )
				{
					strSite.Format( _T( "Station%d_Site[%d][%d]" ), nIdx + 1, nY, nX );

					if( this->nGRRSiteOnOff[ nIdx ][ nY ][ nX ] != stAfterBasicData.nGRRSiteOnOff[ nIdx ][ nY ][ nX ] )
					{
						p.first = "Site Enable/Disable " + strSite;
						if( this->nSiteOnOff[ nIdx ][ nY ][ nX ] == DEF_ON )
						{
							p.second.first = "On";
							p.second.second = "Off";
						}
						else
						{
							p.second.first = "Off";
							p.second.second = "On";
						}

						vData.push_back( p );
					}
				}
			}
		}
	}

	void IsSameQASiteSet(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString, std::pair<CString, CString>>>& vData)
	{
		std::pair<CString, std::pair<CString, CString>> p;

		if (this->nAutoQADefaultCount != stAfterBasicData.nAutoQADefaultCount)
		{
			p.first = "Auto QA Parameter - Default Count";
			p.second.first.Format(_T("%d"), this->nAutoQADefaultCount);
			p.second.second.Format(_T("%d"), stAfterBasicData.nAutoQADefaultCount);
			vData.push_back(p);
		}
	}


	// GRR Site Setting GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameLOOPSiteSet(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString, std::pair<CString, CString>>>& vData)
	{
		std::pair<CString, std::pair<CString, CString>> p;

		if (this->nAutoLoopUnitCount != stAfterBasicData.nAutoLoopUnitCount)
		{
			p.first = "Auto LOOP Parameter - Unit Count";
			p.second.first.Format(_T("%d"), this->nAutoLoopUnitCount);
			p.second.second.Format(_T("%d"), stAfterBasicData.nAutoLoopUnitCount);
			vData.push_back(p);
		}

		if (this->nAutoLoopTestCount != stAfterBasicData.nAutoLoopTestCount)
		{
			p.first = "Auto LOOP Parameter - Test Count";
			p.second.first.Format(_T("%d"), this->nAutoLoopTestCount);
			p.second.second.Format(_T("%d"), stAfterBasicData.nAutoLoopTestCount);
			vData.push_back(p);
		}


		if (this->nAutoLoopmode != stAfterBasicData.nAutoLoopmode) {
			p.first = "Auto Loop Mode";

			switch (this->nAutoLoopmode)
			{
			case eNormal_Mode:
			{
				p.second.first.Format(_T("Normal"));
			}break;
			case ePressContinueTest:
			{
				p.second.first.Format(_T("ContinueTest"));
			}break;
			case ePressReContact:
			{
				p.second.first.Format(_T("ReContact"));
			}break;
			}

			switch (stAfterBasicData.nAutoLoopmode)
			{
			case eNormal_Mode:
			{
				p.second.second.Format(_T("Normal"));
			}break;
			case ePressContinueTest:
			{
				p.second.second.Format(_T("ContinueTest"));
			}break;
			case ePressReContact:
			{
				p.second.second.Format(_T("ReContact"));
			}break;
			}
			vData.push_back(p);
		}

		if (this->nLoopSotDelay != stAfterBasicData.nLoopSotDelay) {
			p.first = "Loop SOT Delay";
			p.second.first.Format(_T("%.2f"), this->nLoopSotDelay / 1000.0);
			p.second.second.Format(_T("%.2f"), stAfterBasicData.nLoopSotDelay / 1000.0);

			vData.push_back(p);
		}

	}

	void IsSameAuditSiteSet(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString, std::pair<CString, CString>>>& vData)
	{
		std::pair<CString, std::pair<CString, CString>> p;

		if (this->nAutoAuditUnitCount != stAfterBasicData.nAutoAuditUnitCount)
		{
			p.first = "Auto Audit Parameter - Unit Count";
			p.second.first.Format(_T("%d"), this->nAutoAuditUnitCount);
			p.second.second.Format(_T("%d"), stAfterBasicData.nAutoAuditUnitCount);
			vData.push_back(p);
		}

		if (this->nAutoAuditTestCount != stAfterBasicData.nAutoAuditTestCount)
		{
			p.first = "Auto Audit Parameter - Test Count";
			p.second.first.Format(_T("%d"), this->nAutoAuditTestCount);
			p.second.second.Format(_T("%d"), stAfterBasicData.nAutoAuditTestCount);
			vData.push_back(p);
		}

		if (this->nAuditSotDelay != stAfterBasicData.nAuditSotDelay) {
			p.first = "Audit SOT Delay";
			p.second.first.Format(_T("%.2f"), this->nAuditSotDelay / 1000.0);
			p.second.second.Format(_T("%.2f"), stAfterBasicData.nAuditSotDelay / 1000.0);

			vData.push_back(p);
		}


		CString strSite = _T("");
		for (int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++)
		{
			for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
			{
				for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
				{
					strSite.Format(_T("Station%d_Site[%d][%d]"), nIdx + 1, nY, nX);

					if (this->nAuditSiteOnOff[nIdx][nY][nX] != stAfterBasicData.nAuditSiteOnOff[nIdx][nY][nX])
					{
						p.first = "Site Enable/Disable " + strSite;
						if (this->nSiteOnOff[nIdx][nY][nX] == DEF_ON)
						{
							p.second.first = "On";
							p.second.second = "Off";
						}
						else
						{
							p.second.first = "Off";
							p.second.second = "On";
						}

						vData.push_back(p);
					}
				}
			}
		}
	}

	// Lot Information GUI Data Change Log
	// vData.first.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second = 1  check box or radio button
	void IsSameLotInfo(_ST_BASIC_INFO stAfterBasicData, std::vector<std::pair<CString,std::pair<CString,CString>>>& vData)
	{
		std::pair<CString,std::pair<CString,CString>> p;

		if(this->nFTRTQC != stAfterBasicData.nFTRTQC)
		{
			// Mode FT RC QC
			CString strBeforeData =_T("");
			CString strAfterData  =_T("");

			switch(this->nFTRTQC)
			{
				case eStartMD_Initial:       { strBeforeData = _T("FT");          } break;
				case eStartMD_Retest:        { strBeforeData = _T("RT");	      } break;
				case eStartMD_QA:            { strBeforeData = _T("QA");          } break;
				case eStartMD_AutoGRR:       { strBeforeData = _T("GRR");         } break;
				case eStartMD_AutoLoop: { strBeforeData = _T("LOOP"); } break;
				case eStartMD_AutoGRR_Continue:   { strBeforeData = _T("GRR Continue"); } break;
				case eStartMD_RetestContinue:{ strBeforeData = _T("RT Continue"); } break;
				case eStartMD_InitContinue:  { strBeforeData = _T("FT Continue"); } break;
				case eStartMD_QA_Continue:   { strBeforeData = _T("QA Continue"); } break;		
				case eStartMD_AutoLoop_Continue: { strBeforeData = _T("LOOP Continue"); } break;

			}
			switch(stAfterBasicData.nFTRTQC)
			{
				case eStartMD_Initial:       { strAfterData = _T("FT");          } break;
				case eStartMD_Retest:        { strAfterData = _T("RT");	      } break;
				case eStartMD_QA:            { strAfterData = _T("QA");          } break;
				case eStartMD_AutoGRR:       { strAfterData = _T("GRR");         } break;
				case eStartMD_AutoLoop: { strBeforeData = _T("LOOP"); } break;
				case eStartMD_AutoGRR_Continue:   { strAfterData = _T("GRR Continue"); } break;
				case eStartMD_RetestContinue:{ strAfterData = _T("RT Continue"); } break;
				case eStartMD_InitContinue:  { strAfterData = _T("FT Continue"); } break;
				case eStartMD_QA_Continue:   { strAfterData = _T("QA Continue"); } break;		
				case eStartMD_AutoLoop_Continue: { strBeforeData = _T("LOOP Continue"); } break;
			}
			p.first ="Mode";
			p.second.first = strBeforeData;
			p.second.second = strAfterData;

			vData.push_back(p);
		}
		if(this->nStartMode != stAfterBasicData.nStartMode)
		{
			CString strBeforeData =_T("");
			CString strAfterData  =_T("");

			switch(this->nStartMode)
			{
				case 0: strBeforeData =_T("Normal");  break;
				case 1: strBeforeData =_T("RC Mode"); break;
			}
			switch(stAfterBasicData.nStartMode)
			{
				case 0: strAfterData =_T("Normal");  break;
				case 1: strAfterData =_T("RC Mode"); break;
			}

			p.first ="Start Mode";
			p.second.first = strBeforeData; 
			p.second.second = strAfterData;

			vData.push_back(p);
		}
		if(this->nVision != stAfterBasicData.nVision)
		{
			CString strBeforeData =_T("");
			CString strAfterData  =_T("");

			switch(this->nVision)
			{
				case 0: strBeforeData =_T("Use");  break;
				case 1: strBeforeData =_T("No Use"); break;
			}
			switch(stAfterBasicData.nVision)
			{
				case 0: strAfterData =_T("Use");  break;
				case 1: strAfterData =_T("No Use"); break;
			}

			p.first ="VIsion";
			p.second.first = strBeforeData; 
			p.second.second = strAfterData;

			vData.push_back(p);
		}
		if(strcmp(this->szLotNo,stAfterBasicData.szLotNo) != 0)
		{
			p.first = "Lot No";
			p.second.first = this->szLotNo;
			p.second.second = stAfterBasicData.szLotNo;

			vData.push_back(p);
		}
		if(strcmp(this->szScheduleNo,stAfterBasicData.szScheduleNo) != 0)
		{
			p.first = "Schedule No";
			p.second.first = this->szScheduleNo;
			p.second.second = stAfterBasicData.szScheduleNo;

			vData.push_back(p);
		}
		if(strcmp(this->szOperID,stAfterBasicData.szOperID) != 0)
		{
			p.first = "Operator ID";
			p.second.first = this->szOperID;
			p.second.second = stAfterBasicData.szOperID;

			vData.push_back(p);
		}
	}
}ST_BASIC_INFO;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_COK_INFO{
	ST_SPD_AXIS		 aTrayPP_SPD[eMaxTrayPPCount];		// Ex) aTrayPP_SPD.aAxisForm[0].nACC = 50;
	ST_SPD_AXIS		 aTestPP_SPD[eMaxTestPPCount];		// Ex) aTestPP_SPD[RID_TEST_PP_1].aAxisForm[eAxis_X].nACC = 50;
	ST_SPD_AXIS		 aLoadTbl_SPD[MAX_LD_TBL_CNT];		// Ex) aLoadTbl_SPD[RID_LD_TBL_1].aAxisForm[eAxis_X].nACC = 50;
	ST_SPD_AXIS		 aPressUnit_SPD[eMaxPressUnitCount];// Ex) aPressUnit_SPD[RID_PRESS_UNIT_1].aAxisForm[eAxis_Z].nACC = 50;
	ST_SPD_AXIS		 aTransfer;							// Ex) aTransfer.aAxisForm[eAxis_X].nACC = 50;
	ST_SPD_AXIS		 aFeeder_SPD;

	ST_ARM_TIME      aTrayPP_Tm[eMaxTrayPPCount];		// Ex) aTrayPP_Tm.nVacuumTm = 5000; (ms)
	ST_ARM_TIME      aTestPP_Tm[eMaxTestPPCount];		// Ex) aTestPP_Tm[RID_TEST_PP_1].nVacuumTm = 5000; (ms)
	ST_ARM_TIME      aPress_Tm[eMaxPressUnitCount];     // Ex) aPress_Tm[1].nBlowTm = 5000; (ms)
	ST_ARM_TIME		 aTranser_Tm;

	ST_TRAY_PLATE    aTrayForm;						    // Ex) aTrayForm.nStart_PitchX = 10000; (10mm)
	ST_TRAY_PLATE    aPlateForm;						// Ex) aPlateForm.nStart_PitchX = 10000; (10mm)

	int  nLoadTbl_Xpitch;
	int  nLoadTbl_Ypitch;
    int  nLoadTbl_XCnt;
    int  nLoadTbl_YCnt;
	int  nLoadTbl_SPD_Barcode_Limit;
	
	int  nTestPP_ST_XPitch;
	int	 nTestPP_ST_YPitch;

	int  nLoadTbl_Xpitch_Ver;
	int  nLoadTbl_Ypitch_Ver;

    int  nBuffTbl_Xpitch;
    int  nBuffTbl_Ypitch;
    int  nBuffTbl_XCnt;
    int  nBuffTbl_YCnt;

	int  nTestBuffTbl_Xpitch;
	int  nTestBuffTbl_Ypitch;
	int  nTestBuffTbl_XCnt;
	int  nTestBuffTbl_YCnt;

	int  nTestClean_Xpitch;
	int  nTestClean_Ypitch;
	int  nTestClean_XCnt;
	int  nTestClean_YCnt;

	int  nTestSite_Xpitch;
	int  nTestSite_Ypitch;
	int  nTestSite_XCnt;
	int  nTestSite_YCnt;

	int  nTrayPPLtPitchX;
	int  nTrayPPLtPitchY;

	int nTestPP_Xpitch;
	int nTestPP_Ypitch;

	int nStacker_Main_WaitTm;
	int nStacker_Sub_WaitTm;
	int nStacker_Clamp_WaitTm;
	int nStacker_Conveyor_StopWaitTm;
	int nStacker_Conveyor_Load_StopWaitTm;
	int nSlowUnloadStackerOption;

	//Feeder
	int nFeeder_UpDn_WaitTm;
	int nFeeder_Clamp_WaitTm;
	double dFeeder_StepOffset;
	int	nFeeder_AlignWait;

	//2d Barcode Validate
	BOOL b2dBarcodeUse;

	BOOL b2dDuplicateOptUse;

	BOOL b2dLengthOptUse;
	int n2dDeviceLength;

	BOOL b2dMarkOptUse;
	int n2dMarkStartIdx;
	char sz2dDeviceName[MAX_PATH];
	char sz2dMarkList[MAX_PATH];

	//Press Auto Height Cal Data
	double dPressHeightCalData[eMaxPressUnitCount];

	// Tray PP GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameTrayPP(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		std::pair<CString, std::pair<double, double>> p;


		for (int i = 0; i <= RIDX_TRAY_PP_2; i++) {
			for (int j = 0; j < eAxis_Y_Pitch; j++)
			{		
				CString strAxis = _T("");

				switch (j)
				{
				case eAxis_X: strAxis = _T("Tray PP X"); break;
				case eAxis_Y: strAxis = _T("Tray PP Y"); break;
				case eAxis_Z: strAxis = _T("Tray PP Z"); break;
				case eAxis_X_Pitch: strAxis = _T("Tray PP X_Pitch"); break;
				case eAxis_Y_Pitch: strAxis = _T("Tray PP Y_Pitch"); break;
				}

				if (this->aTrayPP_SPD[i].aAxisForm[j].nSPD != stAfterCokData.aTrayPP_SPD[i].aAxisForm[j].nSPD)
				{
					p.first = strAxis + " - Speed";
					p.second.first = this->aTrayPP_SPD[i].aAxisForm[j].nSPD;
					p.second.second = stAfterCokData.aTrayPP_SPD[i].aAxisForm[j].nSPD;

					vData.push_back(p);
				}

				if (this->aTrayPP_SPD[i].aAxisForm[j].nACC != stAfterCokData.aTrayPP_SPD[i].aAxisForm[j].nACC)
				{
					p.first = strAxis + " - Accel";
					p.second.first = this->aTrayPP_SPD[i].aAxisForm[j].nACC;
					p.second.second = stAfterCokData.aTrayPP_SPD[i].aAxisForm[j].nACC;

					vData.push_back(p);
				}

				if (this->aTrayPP_SPD[i].aAxisForm[j].nDCC != stAfterCokData.aTrayPP_SPD[i].aAxisForm[j].nDCC)
				{
					p.first = strAxis + " - Decel";
					p.second.first = this->aTrayPP_SPD[i].aAxisForm[j].nDCC;
					p.second.second = stAfterCokData.aTrayPP_SPD[i].aAxisForm[j].nDCC;

					vData.push_back(p);
				}
			}
		}
		if( this->aTrayPP_Tm[eTrayPP_1_LD].nBlowTm != stAfterCokData.aTrayPP_Tm[eTrayPP_1_LD].nBlowTm)
		{
			p.first = "Load Tray Blow Delay Time";
			p.second.first = this->aTrayPP_Tm[eTrayPP_1_LD].nBlowTm / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_1_LD].nBlowTm / 1000;

			vData.push_back(p);
		}
		if(this->aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm != stAfterCokData.aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm)
		{
			p.first = "TrayVacuum Delay Time";
			p.second.first = this->aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm / 1000;

			vData.push_back(p);
		}
		if(this->aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt != stAfterCokData.aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt)
		{
			p.first = "Retry Count";
			p.second.first = this->aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt;

			vData.push_back(p);
		}
		if(this->aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm != stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm)
		{
			p.first = "Cylinder Down Wait Time";
			p.second.first = this->aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm / 1000;

			vData.push_back(p);
		}
		//////
		if (this->aTrayPP_Tm[eTrayPP_2_UD].nBlowTm != stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nBlowTm)
		{
			p.first = "Load Tray Blow Delay Time";
			p.second.first = this->aTrayPP_Tm[eTrayPP_2_UD].nBlowTm / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nBlowTm / 1000;

			vData.push_back(p);
		}
		if (this->aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm != stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm)
		{
			p.first = "TrayVacuum Delay Time";
			p.second.first = this->aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm / 1000;

			vData.push_back(p);
		}
		if (this->aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt != stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt)
		{
			p.first = "Retry Count";
			p.second.first = this->aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt;

			vData.push_back(p);
		}
		if (this->aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm != stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm)
		{
			p.first = "Cylinder Down Wait Time";
			p.second.first = this->aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm / 1000;

			vData.push_back(p);
		}

		if (this->aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset != stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset)
		{
			p.first = "Linear Offset";
			p.second.first = this->aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset / 1000;
			p.second.second = stAfterCokData.aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset / 1000;

			vData.push_back(p);
		}


	}

	// Test PP GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameTestPP(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		std::pair<CString, std::pair<double, double>> p;
		for(int i = 0; i <= RIDX_TEST_PP_2; i++)
		{
			CString strTestPP = _T("");

			switch(i)
			{
				case RIDX_TEST_PP_1: strTestPP =_T("Test PP #1"); break;
				case RIDX_TEST_PP_2: strTestPP =_T("Test PP #2"); break;
			}
		
			for(int j = 0; j<= eAxis_TestPP_Y2_Pitch; j++)
			{
				CString strAxis = _T("");

				switch(j)
				{
					case eAxis_TestPP_X1		: strAxis = _T(" X1"); break;
					case eAxis_TestPP_X2		: strAxis = _T(" X2"); break;
					case eAxis_TestPP_Y			: strAxis = _T(" Y");  break;
					case eAxis_TestPP_Z1		: strAxis = _T(" Z1"); break;
					case eAxis_TestPP_Z2		: strAxis = _T(" Z2"); break;
					case eAxis_TestPP_X1_Pitch	: strAxis = _T(" X1 Pitch"); break;
					case eAxis_TestPP_X2_Pitch	: strAxis = _T(" X2 Pitch"); break;
					case eAxis_TestPP_Y1_Pitch	: strAxis = _T(" Y1 Pitch"); break;
					case eAxis_TestPP_Y2_Pitch	: strAxis = _T(" Y2 Pitch"); break;
				}

				if(this->aTestPP_SPD[i].aAxisForm[j].nSPD != stAfterCokData.aTestPP_SPD[i].aAxisForm[j].nSPD)
				{
					p.first = strTestPP + strAxis + " - Speed";
					p.second.first = this->aTestPP_SPD[i].aAxisForm[j].nSPD;
					p.second.second = stAfterCokData.aTestPP_SPD[i].aAxisForm[j].nSPD;

					vData.push_back(p);
				}
				if(this->aTestPP_SPD[i].aAxisForm[j].nACC != stAfterCokData.aTestPP_SPD[i].aAxisForm[j].nACC)
				{
					p.first = strTestPP + strAxis + " - Accel";
					p.second.first = this->aTestPP_SPD[i].aAxisForm[j].nACC;
					p.second.second = stAfterCokData.aTestPP_SPD[i].aAxisForm[j].nACC;

					vData.push_back(p);
				}
				if(this->aTestPP_SPD[i].aAxisForm[j].nDCC != stAfterCokData.aTestPP_SPD[i].aAxisForm[j].nDCC)
				{
					p.first = strTestPP + strAxis + " - Decel";
					p.second.first = this->aTestPP_SPD[i].aAxisForm[j].nDCC;
					p.second.second = stAfterCokData.aTestPP_SPD[i].aAxisForm[j].nDCC;

					vData.push_back(p);
				}
			}

			CString strNum;
			strNum.Format(_T("%d"),(i+1));

			if(this->aTestPP_Tm[i].nVacuumTm != stAfterCokData.aTestPP_Tm[i].nVacuumTm)
			{
				p.first = "Vacuum #" + strNum + " Time";
				p.second.first = (double)this->aTestPP_Tm[i].nVacuumTm / 1000;
				p.second.second = (double)stAfterCokData.aTestPP_Tm[i].nVacuumTm / 1000;

				vData.push_back(p);
			}
			if(this->aTestPP_Tm[i].nBlowTm != stAfterCokData.aTestPP_Tm[i].nBlowTm)
			{
				p.first = "Blow #"+ strNum + " Time";
				p.second.first = (double)this->aTestPP_Tm[i].nBlowTm / 1000;
				p.second.second = (double)stAfterCokData.aTestPP_Tm[i].nBlowTm / 1000;

				vData.push_back(p);
			}
			if(this->aTestPP_Tm[i].nRetryCnt != stAfterCokData.aTestPP_Tm[i].nRetryCnt)
			{
				p.first = "PP #" + strNum + " Retry Count";
				p.second.first = this->aTestPP_Tm[i].nRetryCnt;
				p.second.second = stAfterCokData.aTestPP_Tm[i].nRetryCnt;

				vData.push_back(p);
			}
			if (this->aTestPP_Tm[i].dPickRetryOffset != stAfterCokData.aTestPP_Tm[i].dPickRetryOffset)
			{
				p.first = "PP #" + strNum + " Pick Retry Offset";
				p.second.first = this->aTestPP_Tm[i].dPickRetryOffset;
				p.second.second = stAfterCokData.aTestPP_Tm[i].dPickRetryOffset;

				vData.push_back(p);
			}
			if (this->aTestPP_Tm[i].dPickerWaitTm != stAfterCokData.aTestPP_Tm[i].dPickerWaitTm)
			{
				p.first = "PP #" + strNum + " Picker Wait Time";
				p.second.first = this->aTestPP_Tm[i].dPickerWaitTm;
				p.second.second = stAfterCokData.aTestPP_Tm[i].dPickerWaitTm;

				vData.push_back(p);
			}
		}

	}

	// Load Table GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameLoadTable(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		std::pair<CString,std::pair<double,double>> p;

		for(int i=0; i<eMaxTblCount; i++)
		{
			CString strNum =_T("");
			strNum.Format(_T("%d"),(i + 1));

			if(this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD)
			{
				p.first = "Tbl #" + strNum + " Y - Speed";
				p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD;
				p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD;

				vData.push_back(p);
			}
			if(this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC)
			{
				p.first = "Tbl #" + strNum + " Y - Accel";
				p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC;
				p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC;

				vData.push_back(p);
			}
			if(this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC)
			{
				p.first = "Tbl #" + strNum + " Y - Decel";
				p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC;
				p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC;

				vData.push_back(p);
			}


			if (this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD)
			{
				p.first = "Tbl #" + strNum + " Rotate - Speed";
				p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD;
				p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD;

				vData.push_back(p);
			}
			if (this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC)
			{
				p.first = "Tbl #" + strNum + " Rotate - Accel";
				p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC;
				p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC;

				vData.push_back(p);
			}
			if (this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC)
			{
				p.first = "Tbl #" + strNum + " Rotate - Decel";
				p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC;
				p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC;

				vData.push_back(p);
			}
			//nLoadTbl_SPD_Barcode_Limit

// 			if(i==RIDX_LD_TBL_1 || i== RIDX_LD_TBL_2)
// 			{
// 				if(this->aLoadTbl_SPD[i].aAxisForm[eAxis_X].nSPD != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nSPD)
// 				{
// 					p.first = "Tbl #" + strNum + " X - Speed";
// 					p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_X].nSPD;
// 					p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nSPD;
// 
// 					vData.push_back(p);
// 				}
// 				if(this->aLoadTbl_SPD[i].aAxisForm[eAxis_X].nACC != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nACC)
// 				{
// 					p.first = "Tbl #" + strNum + " X - Accel";
// 					p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_X].nACC;
// 					p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nACC;
// 
// 					vData.push_back(p);
// 				}
// 				if(this->aLoadTbl_SPD[i].aAxisForm[eAxis_X].nDCC != stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nDCC)
// 				{
// 					p.first = "Tbl #" + strNum + " X - Decel";
// 					p.second.first = this->aLoadTbl_SPD[i].aAxisForm[eAxis_X].nDCC;
// 					p.second.second = stAfterCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nDCC;
// 
// 					vData.push_back(p);
// 				}
// 			}
		}

		if (this->nLoadTbl_SPD_Barcode_Limit != stAfterCokData.nLoadTbl_SPD_Barcode_Limit)
		{
			p.first = "Tbl SPD Barcode Limit";
			p.second.first = this->nLoadTbl_SPD_Barcode_Limit;
			p.second.second = stAfterCokData.nLoadTbl_SPD_Barcode_Limit;

			vData.push_back(p);
		}
	}

	// Feeder GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameFeeder(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString, std::pair<double, double>>>& vData)
	{
		std::pair<CString, std::pair<double, double>> p;

		CString strName = "Feeder Y";
		if (this->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD != stAfterCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD)
		{
			p.first = strName + " - Speed";
			p.second.first = this->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD;
			p.second.second = stAfterCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD;

			vData.push_back(p);
		}
		if (this->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC != stAfterCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC)
		{
			p.first = strName + " - Accel";
			p.second.first = this->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC;
			p.second.second = stAfterCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC;

			vData.push_back(p);
		}
		if (this->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC != stAfterCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC)
		{
			p.first = strName + " - Decel";
			p.second.first = this->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC;
			p.second.second = stAfterCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC;

			vData.push_back(p);
		}
		if (this->nFeeder_UpDn_WaitTm != stAfterCokData.nFeeder_UpDn_WaitTm)
		{
			p.first = strName + " - Feeder Up/Down wait";
			p.second.first = this->nFeeder_UpDn_WaitTm;
			p.second.second = stAfterCokData.nFeeder_UpDn_WaitTm;

			vData.push_back(p);
		}
		if (this->nFeeder_Clamp_WaitTm != stAfterCokData.nFeeder_Clamp_WaitTm)
		{
			p.first = strName + " - Feeder Clamp wait";
			p.second.first = this->nFeeder_Clamp_WaitTm;
			p.second.second = stAfterCokData.nFeeder_Clamp_WaitTm;

			vData.push_back(p);
		}
		if (this->nFeeder_AlignWait != stAfterCokData.nFeeder_AlignWait)
		{
			p.first = strName + " - Feeder_Align Wait";
			p.second.first = this->nFeeder_AlignWait;
			p.second.second = stAfterCokData.nFeeder_AlignWait;

			vData.push_back(p);
		}
		if (this->nStacker_Main_WaitTm != stAfterCokData.nStacker_Main_WaitTm)
		{
			p.first = strName + " - Stacker Main Cylinder wait";
			p.second.first = this->nStacker_Main_WaitTm;
			p.second.second = stAfterCokData.nStacker_Main_WaitTm;

			vData.push_back(p);
		}
		if (this->dFeeder_StepOffset != stAfterCokData.dFeeder_StepOffset)
		{
			p.first = strName + " - Feeder Step Offset";
			p.second.first = this->dFeeder_StepOffset;
			p.second.second = stAfterCokData.dFeeder_StepOffset;

			vData.push_back(p);
		}
	}



	// Press Unit GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSamePressUnit(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		//int nDownNum = 1;
		//int nUpNum = 1;

		for(int i=0; i<= RIDX_PRESS_UNIT_8; i++)
		{
			CString strData = _T("");
			std::pair<CString,std::pair<double,double>> p;
			strData.Format("[#%d]", i+1);

			if(this->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD != stAfterCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD)
			{
				p.first = strData+" Speed";
				p.second.first = this->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD;
				p.second.second = stAfterCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD;

				vData.push_back(p);
			}
			if(this->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC != stAfterCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC)
			{
				p.first = strData+" Accel";
				p.second.first = this->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC;
				p.second.second = stAfterCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC;

				vData.push_back(p);
			}
			if(this->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC != stAfterCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC)
			{
				p.first = strData+" Decel";
				p.second.first = this->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC;
				p.second.second = stAfterCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC;

				vData.push_back(p);
			}
			if(this->aPress_Tm[i].nBlowTm != stAfterCokData.aPress_Tm[i].nBlowTm){
				p.first = strData+" Blow Time";
				p.second.first = this->aPress_Tm[i].nBlowTm;
				p.second.second = stAfterCokData.aPress_Tm[i].nBlowTm;
			}
			if (this->aPress_Tm[i].dBlowPos != stAfterCokData.aPress_Tm[i].dBlowPos) {
				p.first = strData + " Blow Pos";
				p.second.first = this->aPress_Tm[i].nBlowTm;
				p.second.second = stAfterCokData.aPress_Tm[i].nBlowTm;
			}
			if (this->aPress_Tm[i].dCleanOffset != stAfterCokData.aPress_Tm[i].dCleanOffset) {
				p.first = strData + " Clean Offset";
				p.second.first = this->aPress_Tm[i].nBlowTm;
				p.second.second = stAfterCokData.aPress_Tm[i].nBlowTm;
			}
		}
	}

	// Transfer GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameTransfer(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		//CString strLeftRight =_T("");
		std::pair<CString,std::pair<double,double>> p;

		CString strName ="Transfer X";
		if(this->aTransfer.aAxisForm[eAxis_X].nSPD != stAfterCokData.aTransfer.aAxisForm[eAxis_X].nSPD)
		{
			p.first = strName + " - Speed";
			p.second.first = this->aTransfer.aAxisForm[ eAxis_X ].nSPD;
			p.second.second = stAfterCokData.aTransfer.aAxisForm[ eAxis_X ].nSPD;

			vData.push_back(p);
		}
		if(this->aTransfer.aAxisForm[eAxis_X].nACC != stAfterCokData.aTransfer.aAxisForm[eAxis_X].nACC)
		{
			p.first = strName + " - Accel";
			p.second.first = this->aTransfer.aAxisForm[ eAxis_X ].nACC;
			p.second.second = stAfterCokData.aTransfer.aAxisForm[ eAxis_X ].nACC;

			vData.push_back(p);
		}
		if(this->aTransfer.aAxisForm[eAxis_X].nDCC != stAfterCokData.aTransfer.aAxisForm[eAxis_X].nDCC)
		{
			p.first = strName + " - Decel";
			p.second.first = this->aTransfer.aAxisForm[eAxis_X].nDCC;
			p.second.second = stAfterCokData.aTransfer.aAxisForm[eAxis_X].nDCC;

			vData.push_back(p);
		}
		if (this->nStacker_Clamp_WaitTm != stAfterCokData.nStacker_Clamp_WaitTm)
		{
			p.first = strName + " - Stacker Clamp wait";
			p.second.first = this->nStacker_Clamp_WaitTm;
			p.second.second = stAfterCokData.nStacker_Clamp_WaitTm;

			vData.push_back(p);
		}
		if (this->nStacker_Sub_WaitTm != stAfterCokData.nStacker_Sub_WaitTm)
		{
			p.first = strName + " - Stacker Sub Cylinder wait";
			p.second.first = this->nStacker_Sub_WaitTm;
			p.second.second = stAfterCokData.nStacker_Sub_WaitTm;

			vData.push_back(p);
		}
		if (this->nStacker_Main_WaitTm != stAfterCokData.nStacker_Main_WaitTm)
		{
			p.first = strName + " - Stacker Main Cylinder wait";
			p.second.first = this->nStacker_Main_WaitTm;
			p.second.second = stAfterCokData.nStacker_Main_WaitTm;

			vData.push_back(p);
		}
		if (this->nStacker_Conveyor_StopWaitTm != stAfterCokData.nStacker_Conveyor_StopWaitTm)
		{
			p.first = strName + " - Conveyor unload stop wait";
			p.second.first = this->nStacker_Conveyor_StopWaitTm;
			p.second.second = stAfterCokData.nStacker_Conveyor_StopWaitTm;

			vData.push_back(p);
		}
		if (this->nStacker_Conveyor_Load_StopWaitTm != stAfterCokData.nStacker_Conveyor_Load_StopWaitTm)
		{
			p.first = strName + " - Conveyor load stop wait";
			p.second.first = this->nStacker_Conveyor_Load_StopWaitTm;
			p.second.second = stAfterCokData.nStacker_Conveyor_Load_StopWaitTm;

			vData.push_back(p);
		}
		if (this->nSlowUnloadStackerOption != stAfterCokData.nSlowUnloadStackerOption)
		{
			p.first = strName + " - Slow Unload Stacker Option";
			p.second.first = this->nSlowUnloadStackerOption;
			p.second.second = stAfterCokData.nSlowUnloadStackerOption;

			vData.push_back(p);
		}

		

	}

	// TrayFileSetup GUI Data Change Log
	// vData.first = Data Name, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameTrayFile(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString,std::pair<double,double>>>& vData)
	{
		std::pair<CString,std::pair<double,double>> p;

		if(this->aTrayForm.nCnt_X != stAfterCokData.aTrayForm.nCnt_X)
		{
			p.first = "Tray X - Division ";
			p.second.first = this->aTrayForm.nCnt_X;
			p.second.second = stAfterCokData.aTrayForm.nCnt_X;

			vData.push_back(p);
		}
		if(this->aTrayForm.nCnt_Y != stAfterCokData.aTrayForm.nCnt_Y)
		{
			p.first = "Tray Y - Division ";
			p.second.first = this->aTrayForm.nCnt_Y;
			p.second.second = stAfterCokData.aTrayForm.nCnt_Y;

			vData.push_back(p);
		}
		if(this->aTrayForm.nPitchX != stAfterCokData.aTrayForm.nPitchX)
		{
			p.first = "Tray X - Pitch ";
			p.second.first = this->aTrayForm.nPitchX / 1000;
			p.second.second = stAfterCokData.aTrayForm.nPitchX / 1000;

			vData.push_back(p);
		}
		if(this->aTrayForm.nPitchY != stAfterCokData.aTrayForm.nPitchY)
		{
			p.first = "Tray Y - Pitch ";
			p.second.first = this->aTrayForm.nPitchY / 1000;
			p.second.second = stAfterCokData.aTrayForm.nPitchY / 1000;

			vData.push_back(p);
		}
		if(this->aTrayForm.nStart_PitchX != stAfterCokData.aTrayForm.nStart_PitchX)
		{
			p.first = "Tray X - Start ";
			p.second.first = this->aTrayForm.nStart_PitchX / 1000;
			p.second.second = stAfterCokData.aTrayForm.nStart_PitchX / 1000;

			vData.push_back(p);
		}
		if(this->aTrayForm.nStart_PitchY != stAfterCokData.aTrayForm.nStart_PitchY)
		{
			p.first = "Tray Y - Start ";
			p.second.first = this->aTrayForm.nStart_PitchY / 1000;
			p.second.second = stAfterCokData.aTrayForm.nStart_PitchY / 1000;

			vData.push_back(p);
		}
		if(this->aTrayForm.nThickness != stAfterCokData.aTrayForm.nThickness)
		{
			p.first = "Tray Thickness ";
			p.second.first = this->aTrayForm.nThickness / 1000;
			p.second.second = stAfterCokData.aTrayForm.nThickness / 1000;

			vData.push_back(p);
		}

		if(this->aPlateForm.nCnt_X != stAfterCokData.aPlateForm.nCnt_X)
		{
			p.first = "Plate X - Division ";
			p.second.first = this->aPlateForm.nCnt_X;
			p.second.second = stAfterCokData.aPlateForm.nCnt_X;

			vData.push_back(p);
		}
		if(this->aPlateForm.nCnt_Y != stAfterCokData.aPlateForm.nCnt_Y)
		{
			p.first = "Plate Y - Division ";
			p.second.first = this->aPlateForm.nCnt_Y;
			p.second.second = stAfterCokData.aPlateForm.nCnt_Y;

			vData.push_back(p);
		}
		if(this->aPlateForm.nPitchX != stAfterCokData.aPlateForm.nPitchX)
		{
			p.first = "Plate X - Pitch ";
			p.second.first = this->aPlateForm.nPitchX / 1000;
			p.second.second = stAfterCokData.aPlateForm.nPitchX / 1000;

			vData.push_back(p);
		}
		if(this->aPlateForm.nPitchY != stAfterCokData.aPlateForm.nPitchY)
		{
			p.first = "Plate Y - Pitch ";
			p.second.first = this->aPlateForm.nPitchY / 1000;
			p.second.second = stAfterCokData.aPlateForm.nPitchY / 1000;

			vData.push_back(p);
		}
		if(this->aPlateForm.nStart_PitchX != stAfterCokData.aPlateForm.nStart_PitchX)
		{
			p.first = "Plate X - Start ";
			p.second.first = this->aPlateForm.nStart_PitchX / 1000;
			p.second.second = stAfterCokData.aPlateForm.nStart_PitchX / 1000;

			vData.push_back(p);
		}
		if(this->aPlateForm.nStart_PitchY != stAfterCokData.aPlateForm.nStart_PitchY)
		{
			p.first = "Plate Y - Start ";
			p.second.first = this->aPlateForm.nStart_PitchY / 1000;
			p.second.second = stAfterCokData.aPlateForm.nStart_PitchY / 1000;

			vData.push_back(p);
		}

		if(this->nLoadTbl_XCnt != stAfterCokData.nLoadTbl_XCnt)
		{
			p.first = "Load Table X - Division ";
			p.second.first = this->nLoadTbl_XCnt;
			p.second.second = stAfterCokData.nLoadTbl_XCnt;

			vData.push_back(p);
		}
		if(this->nLoadTbl_YCnt != stAfterCokData.nLoadTbl_YCnt)
		{
			p.first = "Load Table Y - Division ";
			p.second.first = this->nLoadTbl_YCnt;
			p.second.second = stAfterCokData.nLoadTbl_YCnt;

			vData.push_back(p);
		}
		if(this->nTestPP_ST_XPitch != stAfterCokData.nTestPP_ST_XPitch)
		{
			p.first = "TestPP ST X - Pitch ";
			p.second.first = this->nTestPP_ST_XPitch / 1000;
			p.second.second = stAfterCokData.nTestPP_ST_XPitch / 1000;

			vData.push_back(p);
		}
		if(this->nTestPP_ST_YPitch != stAfterCokData.nTestPP_ST_YPitch)
		{
			p.first = "TestPP ST Y - Pitch ";
			p.second.first = this->nTestPP_ST_YPitch / 1000;
			p.second.second = stAfterCokData.nTestPP_ST_YPitch / 1000;

			vData.push_back(p);
		}
		if(this->nLoadTbl_Xpitch_Ver != stAfterCokData.nLoadTbl_Xpitch_Ver){
			p.first = "Load Table X - Pitch Vertical ";
			p.second.first = this->nLoadTbl_Xpitch_Ver / 1000;
			p.second.second = stAfterCokData.nLoadTbl_Xpitch_Ver / 1000;

			vData.push_back(p);
		}
		if(this->nLoadTbl_Ypitch_Ver != stAfterCokData.nLoadTbl_Ypitch_Ver){
			p.first = "Load Table Y - Pitch Vertical ";
			p.second.first = this->nLoadTbl_Ypitch_Ver / 1000;
			p.second.second = stAfterCokData.nLoadTbl_Ypitch_Ver / 1000;

			vData.push_back(p);
		}

		if (this->nLoadTbl_Xpitch != stAfterCokData.nLoadTbl_Xpitch)
		{
			p.first = "Load Table X - Pitch ";
			p.second.first = this->nLoadTbl_Xpitch / 1000;
			p.second.second = stAfterCokData.nLoadTbl_Xpitch / 1000;

			vData.push_back(p);
		}
		if (this->nLoadTbl_Ypitch != stAfterCokData.nLoadTbl_Ypitch)
		{
			p.first = "Load Table Y - Pitch ";
			p.second.first = this->nLoadTbl_Ypitch / 1000;
			p.second.second = stAfterCokData.nLoadTbl_Ypitch / 1000;

			vData.push_back(p);
		}

		if(this->nBuffTbl_XCnt != stAfterCokData.nBuffTbl_XCnt)
		{
			p.first = "Buffer Table X - Division ";
			p.second.first = this->nBuffTbl_XCnt;
			p.second.second = stAfterCokData.nBuffTbl_XCnt;

			vData.push_back(p);
		}
		if(this->nBuffTbl_YCnt != stAfterCokData.nBuffTbl_YCnt)
		{
			p.first = "Buffer Table Y - Division ";
			p.second.first = this->nBuffTbl_YCnt;
			p.second.second = stAfterCokData.nBuffTbl_YCnt;

			vData.push_back(p);
		}
		if(this->nBuffTbl_Xpitch != stAfterCokData.nBuffTbl_Xpitch)
		{
			p.first = "Buffer Table X - Pitch ";
			p.second.first = this->nBuffTbl_Xpitch / 1000;
			p.second.second = stAfterCokData.nBuffTbl_Xpitch / 1000;

			vData.push_back(p);
		}
		if(this->nBuffTbl_Ypitch != stAfterCokData.nBuffTbl_Ypitch)
		{
			p.first = "Buffer Table Y - Pitch ";
			p.second.first = this->nBuffTbl_Ypitch / 1000;
			p.second.second = stAfterCokData.nBuffTbl_Ypitch / 1000;

			vData.push_back(p);
		}
		if(this->nTestBuffTbl_Xpitch != stAfterCokData.nTestBuffTbl_Xpitch)
		{
			p.first = "Test Buffer Table X - Pitch ";
			p.second.first = this->nTestBuffTbl_Xpitch / 1000;
			p.second.second = stAfterCokData.nTestBuffTbl_Xpitch / 1000;

			vData.push_back(p);
		}
		if(this->nTestBuffTbl_Ypitch != stAfterCokData.nTestBuffTbl_Ypitch)
		{
			p.first = "Test Buffer Table Y - Pitch ";
			p.second.first = this->nTestBuffTbl_Ypitch / 1000;
			p.second.second = stAfterCokData.nTestBuffTbl_Ypitch / 1000;

			vData.push_back(p);
		}
		if(this->nTestBuffTbl_XCnt != stAfterCokData.nTestBuffTbl_XCnt)
		{
			p.first = "Test Buffer Table X - Division ";
			p.second.first = this->nTestBuffTbl_XCnt;
			p.second.second = stAfterCokData.nTestBuffTbl_XCnt;

			vData.push_back(p);
		}
		if(this->nTestBuffTbl_YCnt != stAfterCokData.nTestBuffTbl_YCnt)
		{
			p.first = "Buffer Table Y - Division ";
			p.second.first = this->nTestBuffTbl_YCnt;
			p.second.second = stAfterCokData.nTestBuffTbl_YCnt;

			vData.push_back(p);
		}

		if(this->nTestClean_XCnt != stAfterCokData.nTestClean_XCnt)
		{
			p.first = "Test Clean X - Division ";
			p.second.first = this->nTestClean_XCnt;
			p.second.second = stAfterCokData.nTestClean_XCnt;

			vData.push_back(p);
		}
		if(this->nTestClean_YCnt != stAfterCokData.nTestClean_YCnt)
		{
			p.first = "Test Clean Y - Division ";
			p.second.first = this->nTestClean_YCnt;
			p.second.second = stAfterCokData.nTestClean_YCnt;

			vData.push_back(p);
		}
		if(this->nTestClean_Xpitch != stAfterCokData.nTestClean_Xpitch)
		{
			p.first = "Test Clean X - Pitch ";
			p.second.first = this->nTestClean_Xpitch / 1000;
			p.second.second = stAfterCokData.nTestClean_Xpitch/ 1000;

			vData.push_back(p);
		}
		if(this->nTestClean_Ypitch != stAfterCokData.nTestClean_Ypitch)
		{
			p.first = "Test Clean Y - Pitch ";
			p.second.first = this->nTestClean_Ypitch / 1000;
			p.second.second = stAfterCokData.nTestClean_Ypitch/ 1000;

			vData.push_back(p);
		}

		if(this->nTestSite_XCnt != stAfterCokData.nTestSite_XCnt)
		{
			p.first = "Test Site X - Division ";
			p.second.first = this->nTestSite_XCnt;
			p.second.second = stAfterCokData.nTestSite_XCnt;

			vData.push_back(p);
		}
		if(this->nTestSite_YCnt != stAfterCokData.nTestSite_YCnt)
		{
			p.first = "Test Site Y - Division ";
			p.second.first = this->nTestSite_YCnt;
			p.second.second = stAfterCokData.nTestSite_YCnt;

			vData.push_back(p);
		}
		if(this->nTestSite_Xpitch != stAfterCokData.nTestSite_Xpitch)
		{
			p.first = "Test Site X - Pitch ";
			p.second.first = this->nTestSite_Xpitch / 1000;
			p.second.second = stAfterCokData.nTestSite_Xpitch/ 1000;

			vData.push_back(p);
		}
		if(this->nTestSite_Ypitch != stAfterCokData.nTestSite_Ypitch)
		{
			p.first = "Test Site Y - Pitch ";
			p.second.first = this->nTestSite_Ypitch / 1000;
			p.second.second = stAfterCokData.nTestSite_Ypitch/ 1000;

			vData.push_back(p);
		}
	}

	void IsSame2dValidateOptUse(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString, std::pair<double, double>>>& vData)
	{
		std::pair<CString, std::pair<double, double>> p;

		//Barcode Option Use/No Use
		if (this->b2dBarcodeUse != stAfterCokData.b2dBarcodeUse)
		{
			p.first = "2D Barcode Use";
			p.second.first = this->b2dBarcodeUse;
			p.second.second = stAfterCokData.b2dBarcodeUse;

			vData.push_back(p);
		}

		//Duplication
		if (this->b2dDuplicateOptUse != stAfterCokData.b2dDuplicateOptUse)
		{
			p.first = "2D Barcode Duplication Opt Use";
			p.second.first = this->b2dDuplicateOptUse;
			p.second.second = stAfterCokData.b2dDuplicateOptUse;

			vData.push_back(p);
		}

		//Length
		if (this->b2dLengthOptUse != stAfterCokData.b2dLengthOptUse)
		{
			p.first = "2D Barcode Length Opt Use";
			p.second.first = this->b2dLengthOptUse;
			p.second.second = stAfterCokData.b2dLengthOptUse;

			vData.push_back(p);
		}

		if (this->n2dDeviceLength != stAfterCokData.n2dDeviceLength)
		{
			p.first = "2D Barcode Length";
			p.second.first = (double)(this->n2dDeviceLength);
			p.second.second = (double)(stAfterCokData.n2dDeviceLength);

			vData.push_back(p);
		}

		//Mark
		if (this->b2dMarkOptUse != stAfterCokData.b2dMarkOptUse)
		{
			p.first = "2D Barcode Mark Opt Use";
			p.second.first = this->b2dMarkOptUse;
			p.second.second = stAfterCokData.b2dMarkOptUse;

			vData.push_back(p);
		}

		if (this->n2dMarkStartIdx != stAfterCokData.n2dMarkStartIdx)
		{
			p.first = "2D Barcode Mark Start Idx ";
			p.second.first = (double)(this->n2dMarkStartIdx);
			p.second.second = (double)(stAfterCokData.n2dMarkStartIdx);

			vData.push_back(p);
		}
	}

	//Device Name & Mark List
	void IsSame2DMark(_ST_COK_INFO stAfterCokData, std::vector<std::pair<CString, std::pair<CString, CString>>> &vData)
	{
		std::pair<CString, std::pair<CString, CString>> p;

		//DeviceName
		if (strcmp(this->sz2dDeviceName, stAfterCokData.sz2dDeviceName) != 0)
		{
			p.first = "2D Device Name";
			p.second.first = (LPCTSTR)(this->sz2dDeviceName);
			p.second.second = (LPCTSTR)(stAfterCokData.sz2dDeviceName);

			vData.push_back(p);
		}


		//Mark List
		if (strcmp(this->sz2dMarkList, stAfterCokData.sz2dMarkList) != 0)
		{
			p.first = "2D Mark List";
			p.second.first = (LPCTSTR)(this->sz2dMarkList);
			p.second.second = (LPCTSTR)(stAfterCokData.sz2dMarkList);

			vData.push_back(p);
		}
	}


	//Press Auto Height Cal Data
	void IsSamePressHeightCalData(_ST_COK_INFO stAfterCokData, int nPressIdx, std::vector<std::pair<CString, std::pair<double, double>>>& vData)
	{
		std::pair<CString, std::pair<double, double>> p;

		if (this->dPressHeightCalData[nPressIdx] != stAfterCokData.dPressHeightCalData[nPressIdx])
		{
			CString strTitle = _T("");
			strTitle.Format("Press #%d", nPressIdx + 1);
			p.first = strTitle;
			p.second.first = this->dPressHeightCalData[nPressIdx];
			p.second.second = stAfterCokData.dPressHeightCalData[nPressIdx];

			vData.push_back(p);
		}
	}

}ST_COK_INFO;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_CATE_SUB_INFO
{
	char szSWBin[16];	        // Software bin number Ex)0 or 9999 or 999 or 1 ...
	char szHWBin[16];	        // Hardware bin number, Stacker Num Ex)1 or 2 or 4 ...
	char szCode[5];	        // Test Bin Code       Ex) PASS or E1 or E97 ....
	char szDesc[32];           // Description
	char szPassFail[4];		// 0: Fail, 1 : Pass
	char szHWBin_Tester[5];	        // Watt Pkg HW Bin EX) 3,4,7

	_ST_CATE_SUB_INFO()
	{
		memset(this, 0x00, sizeof(_ST_CATE_SUB_INFO));
	};
}ST_CATE_SUB_INFO;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_CATE_INFO{

	int nIndexCount;
	ST_CATE_SUB_INFO aCateInfo[MAX_BINCATE_LIST];	 

	_ST_CATE_INFO()
	{
		memset(this, 0x00, sizeof(_ST_CATE_INFO));
	};

	//Category Setup GUI Data Change Log
	// vData.first.first = Data Name , vData.first.second = Standard Data, vData.second.first = BeforeData vData.second.second = AfterData
	void IsSameCategorySetup(_ST_CATE_INFO* stAfterCateData, std::vector<std::pair<CString,std::pair<CString,CString>>> & vData, int nSortType)
	{
		//int nModifyCount =0;
		for(int i=0; i<this->nIndexCount; i++)
		{
			//nModifyCount = 0; //이전 데이터가 새로 저장하는 데이터에 있는지 확인하여 Log남기기위한 Count
			BOOL bChkDelData = FALSE;

			for(int j=0; j<stAfterCateData->nIndexCount; j++)
			{
				std::pair<CString,std::pair<CString,CString>> p;
				char szBefore[256] = {0,};
				char szAfter[256] = {0,};
				char szStandardData[256] ={0,};

				if(nSortType == eSortType_SwBin){
					sprintf_s(szBefore, sizeof(szBefore), this->aCateInfo[i].szSWBin);
					sprintf_s(szAfter, sizeof(szAfter), stAfterCateData->aCateInfo[j].szSWBin);
					sprintf_s(szStandardData, sizeof(szStandardData), this->aCateInfo[i].szSWBin);
				}else {
					sprintf_s(szBefore, sizeof(szBefore), this->aCateInfo[i].szDesc);
					sprintf_s(szAfter, sizeof(szAfter), stAfterCateData->aCateInfo[j].szDesc);
					sprintf_s(szStandardData, sizeof(szStandardData), this->aCateInfo[i].szDesc);
				}

				if(strcmp(szBefore, szAfter) == 0)
				{
					if(bChkDelData == FALSE) { bChkDelData = TRUE; }

					if( strcmp(this->aCateInfo[i].szSWBin, stAfterCateData->aCateInfo[j].szSWBin)  != 0
						|| strcmp(this->aCateInfo[i].szDesc, stAfterCateData->aCateInfo[j].szDesc)  != 0
						|| strcmp(this->aCateInfo[i].szHWBin, stAfterCateData->aCateInfo[j].szHWBin)  != 0
// 						|| strcmp(this->aCateInfo[i].szCode, stAfterCateData->aCateInfo[j].szCode)    != 0
						|| strcmp(this->aCateInfo[i].szPassFail, stAfterCateData->aCateInfo[j].szPassFail) != 0 )
					{
						CString strBeforData =_T(""),  strAfterData = _T(""); 
						strBeforData.Format(_T("%s, %s, %s, %s"),this->aCateInfo[i].szSWBin,this->aCateInfo[i].szHWBin/*,this->aCateInfo[i].szCode*/,this->aCateInfo[i].szDesc,this->aCateInfo[i].szPassFail);
						p.second.first = strBeforData;
						strAfterData.Format(_T("%s, %s, %s, %s"), stAfterCateData->aCateInfo[j].szSWBin, stAfterCateData->aCateInfo[j].szHWBin/*,stAfterCateData->aCateInfo[j].szCode*/, stAfterCateData->aCateInfo[j].szDesc, stAfterCateData->aCateInfo[j].szPassFail);
						p.second.second =strAfterData;
						vData.push_back(p);
					}
				}
			}
			
			if(bChkDelData == FALSE)
			{
				CString strBeforData = _T("");
				std::pair<CString,std::pair<CString,CString>> p;
				p.first = "Delete";				
				strBeforData.Format(_T("%s, %s, %s, %s"),this->aCateInfo[i].szSWBin,this->aCateInfo[i].szHWBin/*,this->aCateInfo[i].szCode*/,this->aCateInfo[i].szDesc,this->aCateInfo[i].szPassFail);
				p.second.second = strBeforData;
				vData.push_back(p);
			}
		}
		for(int j=0; j<stAfterCateData->nIndexCount; j++)
		{
			for(int i=0; i<this->nIndexCount; i++)
			{
				std::pair<CString,std::pair<CString,CString>> p;
				char szBefore[256] = {0,};
				char szAfter[256] = {0,};

				if(nSortType == eSortType_SwBin){
					sprintf_s(szBefore, sizeof(szBefore), this->aCateInfo[i].szSWBin);
					sprintf_s(szAfter, sizeof(szAfter), stAfterCateData->aCateInfo[j].szSWBin);
				}else {
					sprintf_s(szBefore, sizeof(szBefore), this->aCateInfo[i].szDesc);
					sprintf_s(szAfter, sizeof(szAfter), stAfterCateData->aCateInfo[j].szDesc);
				}

				if(strcmp(szBefore, szAfter) == 0) {
					break;
				}

				if(i == this->nIndexCount -1)
				{
					p.first = "ADD";
					CString strAfterData =_T(""); 
					strAfterData.Format(_T("%s, %s, %s, %s"), stAfterCateData->aCateInfo[j].szSWBin, stAfterCateData->aCateInfo[j].szHWBin/*,stAfterCateData->aCateInfo[j].szCode*/, stAfterCateData->aCateInfo[j].szDesc, stAfterCateData->aCateInfo[j].szPassFail);
					p.second.second =strAfterData;

					vData.push_back(p);
				}
			}
		}
	}
}ST_CATE_INFO;


typedef struct _ST_2D_MARK_INFO {
	char szDeviceName[MAX_PATH];

	std::vector<CString> v2dMarkList;

	_ST_2D_MARK_INFO()
	{
		memset(this, 0x00, sizeof(_ST_2D_MARK_INFO));
	};

	void Clear()
	{
		memset(this, 0x00, sizeof(_ST_2D_MARK_INFO));
	}

	void IsSameBarcodeMarkSetUp(_ST_2D_MARK_INFO stAfter2DMarkData, std::vector<std::pair<CString, std::pair<CString, CString>>> &vData)
	{
		std::pair<CString, std::pair<CString, CString>> p;

		BOOL bIsMarkSame = FALSE;

		if (this->v2dMarkList.size() == stAfter2DMarkData.v2dMarkList.size()) {
			std::vector<CString> vAfter2DMarkList = stAfter2DMarkData.v2dMarkList;
			std::vector<CString> vBefore2DMarkList = this->v2dMarkList;

			//벡터 정렬하여 비교.
			std::sort(vAfter2DMarkList.begin(), vAfter2DMarkList.end());
			std::sort(vBefore2DMarkList.begin(), vBefore2DMarkList.end());

			if (vAfter2DMarkList == vBefore2DMarkList) {
				bIsMarkSame = TRUE;
			}
		}

		if (bIsMarkSame != TRUE) {
			CString strDeviceName = _T("");
			strDeviceName.Format("%s", this->szDeviceName);
			
			p.first = strDeviceName; //2D Device Name

			CString strTemp = _T("");
			for (int idx = 0; idx < this->v2dMarkList.size(); idx++) {
				strTemp += this->v2dMarkList[idx];
				strTemp += ",";
			}
			p.second.first = strTemp; //Before Mark List


			strTemp = _T("");
			for (int idx = 0; idx < stAfter2DMarkData.v2dMarkList.size(); idx++) {
				strTemp += stAfter2DMarkData.v2dMarkList[idx];
				strTemp += ",";
			}
			p.second.second = strTemp; //After Mark List

			vData.push_back(p);
		}

	}
}ST_2D_MARK_INFO;


//==============================================================================
// STRUCTURE : Bin Result Jamrate data
//==============================================================================
typedef struct _ST_BIN_SORT_INFO{
	char szHWBin[4];	        // Hardware bin number, Stacker Num Ex)1 or 2 or 4 ...
//	char szCode[32];	        // Test Bin Code       Ex) PASS or E1 or E97 ....
	char szDesc[32];           // Description
	char szPassFail[4];		// 0: Fail, 1 : Pass
	int  nCount[16];

	_ST_BIN_SORT_INFO()
	{
		memset(this, 0x00, sizeof(_ST_BIN_SORT_INFO));
	};
}ST_BIN_SORT_INFO;

typedef struct _ST_PRODUCTION_SUM_INFO{
	int nBinCnt[eMaxJamRateData];	// enum eJamRateData{
	int nSiteTestCnt[eMaxPressUnitCount];
	int nTotalGdPart[eMaxPressUnitCount];
	int nInputJamCnt;
	int nOutputJamCnt;
	int nCvDamageCnt;
	char szLotStartTm[128];
	char szLotEndTm[128];
	char szLotName[32];

	_ST_PRODUCTION_SUM_INFO()
	{
		memset(this, 0x00, sizeof(_ST_PRODUCTION_SUM_INFO));
	};
}ST_PRODUCTION_SUM_INFO;

typedef struct _ST_LOT_SUMMARY_INFO{
	int nHWBinNo[4]; // unload plate data
	int nInputJamCnt;
	int nOutputJamCnt;
	int nLoadingCnt;
	int nSortingCnt;

	_ST_LOT_SUMMARY_INFO()
	{
		memset(this, 0x00, sizeof(_ST_LOT_SUMMARY_INFO));
	};
}ST_LOT_SUMMARY_INFO;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_PGM_TYPE_LG{
	int  nBootTmOut;
	int  nTestTmOut;
	int  nAcPowerOnDelayTm;
	int  nAcPowerOffDelayTm;
	int  nAtxPowerOnDelayTm;
	int  nBaudRate;
	char szFailCodeHeader[128];
	int  nFailCodeDigitalNo;
	char szType[128];

	_ST_SLT_PGM_TYPE_LG()
	{
		memset(this, 0x00, sizeof(_ST_SLT_PGM_TYPE_LG));
	};
}ST_SLT_PGM_TYPE_LG;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_PGM_TYPE_ADVDSystem{
	int  nBootTmOut;
	int  nTestTmOut;
	int  nAcPowerOnDelayTm;
	int  nAcPowerOffDelayTm;
	int  nAtxPowerOnDelayTm;
	int  nBaudRate;
	int  nSendStartDelayTm;
	BOOL bMD_ExternalPowerUse;
	int  nExternalPowerDelayTm;

	char szType[128];

	_ST_SLT_PGM_TYPE_ADVDSystem()
	{
		memset(this, 0x00, sizeof(_ST_SLT_PGM_TYPE_ADVDSystem));
	};
}ST_SLT_PGM_TYPE_ADVDSystem;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_PGM_TYPE_ECD{
	int  nBootTmOut;
	int  nTestTmOut;
	int  nAcPowerOnDelayTm;
	int  nAcPowerOffDelayTm;
	int  nAtxPowerOnDelayTm;
	int  nBaudRate;
	int  nSendStartPreCoolingTm;
	char szPassString[128];
	char szFailCodeHeader[128];
	int  nFailCodeDigitalNo;
	char szType[128];

	_ST_SLT_PGM_TYPE_ECD()
	{
		memset(this, 0x00, sizeof(_ST_SLT_PGM_TYPE_ECD));
	};
}ST_SLT_PGM_TYPE_ECD;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_PGM_TYPE_VGA{
	int  nBootTmOut1;
	int  nBootTmOut2;
	int  nTestTmOut;
	int  nAcPowerOnDelayTm;
	int  nAcPowerOffDelayTm;
	int  nAtxPowerOnDelayTm;
	int  nBaudRate;
	int  nPreCoolingTm;
	char szPassString[128];
	char szFailCodeHeader[128];
	int  nFailCodeDigitalNo;
	char szStartLine[32];
	char szEndLine[32];
	BOOL bMd_AgpUse;
	int  nAgpDelayTm;
	BOOL bMd_BtsHangReboot;
	BOOL bMd_BiosBootUse;
	int  nBiosBootCnt;
	int  nBiosTmOut;
	BOOL bMd_BiosS3Use;
	int  nBiosS3Cnt;
	int  nBiosS3TmOut;
	int  nBiosS3DelayTm;
	BOOL bMd_TempeUse;
	int  nSetTemp;
	int  nTempGuardBand;
	int  nMinPassTm;
	char szType[128];

	_ST_SLT_PGM_TYPE_VGA()
	{
		memset(this, 0x00, sizeof(_ST_SLT_PGM_TYPE_VGA));
	};
}ST_SLT_PGM_TYPE_VGA;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_PGM{
    // Use/NoUse Mode
    BOOL bMD_BootTmOut	    ;
    BOOL bMD_PreCooling     ;
    BOOL bMD_AcPwrOnDly     ;
    BOOL bMD_AcPwrOff       ;
    BOOL bMD_AtxPowerOn     ;
    BOOL bMD_PassStr        ;
    BOOL bMD_FailCodeHead   ;
    BOOL bMD_FailCodeDig    ;
    BOOL bMD_StartLine      ;
    BOOL bMD_EndLine        ;
    BOOL bMD_MinPassTm      ;
    BOOL bMD_BaudRate       ;
    BOOL bMD_HangReboot     ;
    BOOL bMD_BiosBoot       ;
    BOOL bMD_BiosS3         ;
    BOOL bMD_TempMonitor    ;
    BOOL bMD_ExtentionPwrAGP;
	BOOL bMD_SiteOffTestTimeOut;
	BOOL bMD_SiteOffDefaultBin;
	BOOL bMD_Sort_as_DefaultBin;

    int  nBootTmOut;
    int  nTestTmOut;
    int  nAcPowerOnDelayTm;
    int  nAcPowerOffDelayTm;
    int  nAtxPowerOnDelayTm;
    int  nBaudRate;
    int  nPreCoolingTm;
    char szPassString[128];
    char szFailCodeHeader[128];
    int  nFailCodeDigitalNo;
    char szStartLine[32];
    char szEndLine[32];    
    int  nAgpDelayTm;    
    int  nBiosBootCnt;
    int  nBiosTmOut;    
    int  nBiosS3Cnt;
    int  nBiosS3TmOut;
    int  nBiosS3DelayTm;    
    int  nSetTemp;
    int  nTempGuardBand;
    int  nMinPassTm;
	int  nSotDelayTm;
	int  nNotTestingTmOut;

    _ST_SLT_PGM()
    {
        memset(this, 0x00, sizeof(_ST_SLT_PGM));
    };

	// TrayFileSetup GUI Data Change Log
	// vData.first.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	// vData.first.second  = 1  Check box or Radio Button
	void IsSameTestPgmSet(_ST_SLT_PGM stAfterPgmType, std::vector<std::pair<std::pair<CString,int>,std::pair<CString,CString>>>& vData)
	{
		std::pair<std::pair<CString,int>,std::pair<CString,CString>> p;

		if(this->nAcPowerOffDelayTm != stAfterPgmType.nAcPowerOffDelayTm)
		{
			p.first.first = "AC Power Off";
			p.second.first.Format(_T("%.2f"), (double)this->nAcPowerOffDelayTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nAcPowerOffDelayTm / 1000);

			vData.push_back(p);
		}
		if(this->nAcPowerOnDelayTm != stAfterPgmType.nAcPowerOnDelayTm)
		{
			p.first.first = "AC Power On Deley";
			p.second.first.Format(_T("%.2f"), (double)this->nAcPowerOnDelayTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nAcPowerOnDelayTm / 1000);

			vData.push_back(p);

		}
		if(this->nAtxPowerOnDelayTm != stAfterPgmType.nAtxPowerOnDelayTm)
		{
			p.first.first = "ATX Power On";
			p.second.first.Format(_T("%.2f"), (double)this->nAtxPowerOnDelayTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nAtxPowerOnDelayTm / 1000);

			vData.push_back(p);
		}
		if(this->nBootTmOut != stAfterPgmType.nBootTmOut)
		{
			p.first.first = "Boot Time Out";
			p.second.first.Format(_T("%.2f"), (double)this->nBootTmOut / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nBootTmOut / 1000);

			vData.push_back(p);
		}
		if(this->nFailCodeDigitalNo != stAfterPgmType.nFailCodeDigitalNo)
		{
			p.first.first = "Fail Code Digital";
			p.second.first.Format(_T("%.2f"), (double)this->nFailCodeDigitalNo / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nFailCodeDigitalNo / 1000);

			vData.push_back(p);
		}
		if(this->nTestTmOut != stAfterPgmType.nTestTmOut)
		{
			p.first.first = "Test Time Out";
			p.second.first.Format(_T("%.2f"), (double)this->nTestTmOut / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nTestTmOut / 1000);

			vData.push_back(p);
		}
		if (this->nNotTestingTmOut != stAfterPgmType.nNotTestingTmOut)
		{
			p.first.first = "Not Testing Time Out";
			p.second.first.Format(_T("%.2f"), (double)this->nNotTestingTmOut / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nNotTestingTmOut / 1000);

			vData.push_back(p);
		}

		
		if(this->nPreCoolingTm != stAfterPgmType.nPreCoolingTm)
		{
			p.first.first = "Pre-Cooling";
			p.second.first.Format(_T("%.2f"), (double)this->nPreCoolingTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nPreCoolingTm / 1000);

			vData.push_back(p);
		}
		if(this->nMinPassTm != stAfterPgmType.nMinPassTm)
		{
			p.first.first = "Minimum Pass Time";
			p.second.first.Format(_T("%.2f"), (double)this->nMinPassTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nMinPassTm / 1000);

			vData.push_back(p);
		}
		if(this->nBiosBootCnt != stAfterPgmType.nBiosBootCnt)
		{
			p.first.first = "BIOS Boot Count";
			p.second.first.Format(_T("%.2f"), (double)this->nBiosBootCnt);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nBiosBootCnt);

			vData.push_back(p);
		}
		if(this->nBiosTmOut != stAfterPgmType.nBiosTmOut)
		{
			p.first.first = "BIOS Boot Timeout";
			p.second.first.Format(_T("%.2f"), (double)this->nBiosTmOut / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nBiosTmOut / 1000);

			vData.push_back(p);
		}
		if(this->nBiosS3Cnt != stAfterPgmType.nBiosS3Cnt)
		{
			p.first.first = "BIOS S3 Count";
			p.second.first.Format(_T("%.2f"), (double)this->nBiosS3Cnt);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nBiosS3Cnt);

			vData.push_back(p);
		}
		if(this->nBiosS3TmOut != stAfterPgmType.nBiosS3TmOut)
		{
			p.first.first = "BIOS S3 Timeout";
			p.second.first.Format(_T("%.2f"), (double)this->nBiosS3TmOut / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nBiosS3TmOut / 1000);

			vData.push_back(p);
		}
		if(this->nBiosS3DelayTm != stAfterPgmType.nBiosS3DelayTm)
		{
			p.first.first = "BIOS S3 Delay";
			p.second.first.Format(_T("%.2f"), (double)this->nBiosS3DelayTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nBiosS3DelayTm / 1000);

			vData.push_back(p);
		}
		if(this->nSetTemp != stAfterPgmType.nSetTemp)
		{
			p.first.first = "SetTemp";
			p.second.first.Format(_T("%.2f"), (double)this->nSetTemp);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nSetTemp);

			vData.push_back(p);
		}
		if(this->nTempGuardBand != stAfterPgmType.nTempGuardBand)
		{
			p.first.first = "Guard Band";
			p.second.first.Format(_T("%.2f"), (double)this->nTempGuardBand);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nTempGuardBand);

			vData.push_back(p);
		}
		if(this->nAgpDelayTm != stAfterPgmType.nAgpDelayTm)
		{
			p.first.first = "Delay Time";
			p.second.first.Format(_T("%.2f"), (double)this->nAgpDelayTm / 1000);
			p.second.second.Format(_T("%.2f"), (double)stAfterPgmType.nAgpDelayTm / 1000);

			vData.push_back(p);
		}
		if(this->nBaudRate != stAfterPgmType.nBaudRate)
		{
			p.first.first = "Baud Rate";
			p.first.second = 2;
			p.second.first.Format(_T("%d"), this->nBaudRate);
			p.second.second.Format(_T("%d"), stAfterPgmType.nBaudRate);

			vData.push_back(p);
		}

		if(this->bMD_BootTmOut != stAfterPgmType.bMD_BootTmOut)
		{
			p.first.first = "Boot Time Out";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_BootTmOut);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_BootTmOut);

			vData.push_back(p);
		}
		if(this->bMD_PreCooling != stAfterPgmType.bMD_PreCooling)
		{
			p.first.first = "Pre - Cooling";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_PreCooling);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_PreCooling);

			vData.push_back(p);
		}
		if(this->bMD_AcPwrOnDly != stAfterPgmType.bMD_AcPwrOnDly)
		{
			p.first.first = "AC Power on Delay";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_AcPwrOnDly);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_AcPwrOnDly);

			vData.push_back(p);
		}
		if(this->bMD_AcPwrOff != stAfterPgmType.bMD_AcPwrOff)
		{
			p.first.first = "AC Power off";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_AcPwrOff);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_AcPwrOff);

			vData.push_back(p);
		}
		if(this->bMD_AtxPowerOn != stAfterPgmType.bMD_AtxPowerOn)
		{
			p.first.first = "ATX Power on";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_AtxPowerOn);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_AtxPowerOn);

			vData.push_back(p);
		}
		if(this->bMD_PassStr != stAfterPgmType.bMD_PassStr)
		{
			p.first.first = "Pass String";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_PassStr);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_PassStr);

			vData.push_back(p);
		}
		if(this->bMD_FailCodeHead != stAfterPgmType.bMD_FailCodeHead)
		{
			p.first.first = "Fail Code Header";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_FailCodeHead);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_FailCodeHead);

			vData.push_back(p);
		}
		if(this->bMD_FailCodeDig != stAfterPgmType.bMD_FailCodeDig)
		{
			p.first.first = "Fail Code Digital";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_FailCodeDig);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_FailCodeDig);

			vData.push_back(p);
		}
		if(this->bMD_StartLine != stAfterPgmType.bMD_StartLine)
		{
			p.first.first = "Start Line";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_StartLine);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_StartLine);

			vData.push_back(p);
		}
		if(this->bMD_EndLine != stAfterPgmType.bMD_EndLine)
		{
			p.first.first = "End Line";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_EndLine);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_EndLine);

			vData.push_back(p);
		}
		if(this->bMD_MinPassTm != stAfterPgmType.bMD_MinPassTm)
		{
			p.first.first = "Minimum Pass Time";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_MinPassTm);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_MinPassTm);

			vData.push_back(p);
		}
		if(this->bMD_BaudRate != stAfterPgmType.bMD_BaudRate)
		{
			p.first.first = "Baud Rate";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_BaudRate);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_BaudRate);

			vData.push_back(p);
		}
		if(this->bMD_HangReboot != stAfterPgmType.bMD_HangReboot)
		{
			p.first.first = "Hang Reboot";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_HangReboot);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_HangReboot);

			vData.push_back(p);
		}
		if(this->bMD_BiosBoot != stAfterPgmType.bMD_BiosBoot)
		{
			p.first.first = "BIOS Boot";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_BiosBoot);
			p.second.second.Format(_T("%d"),stAfterPgmType.bMD_BiosBoot);

			vData.push_back(p);
		}
		if(this->bMD_BiosS3 != stAfterPgmType.bMD_BiosS3)
		{
			p.first.first = "BIOS S3";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_BiosS3);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_BiosS3);

			vData.push_back(p);
		}
		if(this->bMD_TempMonitor != stAfterPgmType.bMD_TempMonitor)
		{
			p.first.first = "Temperature Monitor";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_TempMonitor);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_TempMonitor);

			vData.push_back(p);
		}
		if(this->bMD_ExtentionPwrAGP != stAfterPgmType.bMD_ExtentionPwrAGP)
		{
			p.first.first = "Extension Power (AGP)";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), this->bMD_ExtentionPwrAGP);
			p.second.second.Format(_T("%d"),  stAfterPgmType.bMD_ExtentionPwrAGP);

			vData.push_back(p);
		}
		if(strcmp(this->szFailCodeHeader,stAfterPgmType.szFailCodeHeader) != 0)
		{
			p.first.first = "Fail Code Header";
			p.first.second = 0;
			p.second.first = this->szFailCodeHeader;
			p.second.second = stAfterPgmType.szFailCodeHeader;

			vData.push_back(p);
		}
		if(strcmp(this->szPassString,stAfterPgmType.szPassString) != 0)
		{
			p.first.first = "Pass String";
			p.first.second = 0;
			p.second.first = this->szPassString;
			p.second.second = stAfterPgmType.szPassString;

			vData.push_back(p);
		}
		if(strcmp(this->szStartLine,stAfterPgmType.szStartLine) != 0)
		{
			p.first.first = "Start Line";
			p.first.second = 0;
			p.second.first = this->szStartLine;
			p.second.second = stAfterPgmType.szStartLine;

			vData.push_back(p);
		}
		if(strcmp(this->szEndLine,stAfterPgmType.szEndLine) != 0)
		{
			p.first.first = "End Line";
			p.first.second = 0;
			p.second.first = this->szEndLine;
			p.second.second = stAfterPgmType.szEndLine;

			vData.push_back(p);
		}
		if (this->bMD_SiteOffTestTimeOut != stAfterPgmType.bMD_SiteOffTestTimeOut)
		{
			p.first.first = "Site Off test time out";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), bMD_SiteOffTestTimeOut);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_SiteOffTestTimeOut);

			vData.push_back(p);
		}

		if (this->bMD_SiteOffDefaultBin != stAfterPgmType.bMD_SiteOffDefaultBin)
		{
			p.first.first = "Site Off Default Bin";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), bMD_SiteOffDefaultBin);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_SiteOffDefaultBin);

			vData.push_back(p);
		}

		if (this->bMD_Sort_as_DefaultBin != stAfterPgmType.bMD_Sort_as_DefaultBin)
		{
			p.first.first = "Site Off Sort As Default Bin";
			p.first.second = 1;
			p.second.first.Format(_T("%d"), bMD_Sort_as_DefaultBin);
			p.second.second.Format(_T("%d"), stAfterPgmType.bMD_Sort_as_DefaultBin);

			vData.push_back(p);
		}
		
	}
}ST_SLT_PGM;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_SLT_SYSTEM
{
	char            szLastPackageName[256];
	ST_COK_INFO     lpstDataCOK;
	ST_BASIC_INFO   lpstDataBasic;
	//	ST_SLT_PGM_INFO lpstDataSltPgm;
	ST_CATE_INFO    lpstDataCategory;
    ST_SLT_PGM      lpstDataSltPgm;

	_ST_SLT_SYSTEM()
	{
		memset(this, 0x00, sizeof(_ST_SLT_SYSTEM));
	};
}ST_SLT_SYSTEM;

//==============================================================================
// STRUCTURE :
//==============================================================================
typedef struct _ST_TRANSFER_TEACHING_DATA{
	int nLocIdx;
	double dPos_X;
	double dPos_ZL_DN;
	double dPos_ZL_UP;
	double dPos_ZR_DN;
	double dPos_ZR_UP;
	_ST_TRANSFER_TEACHING_DATA()
	{
		memset(this, 0x00, sizeof(ST_TRANSFER_TEACHING_DATA));
	}
}ST_TRANSFER_TEACHING_DATA;

typedef struct _ST_ALARM_RPT_OBJ{
	int nAlid;                 // AlarmId
	void* pSender;             // CBaseTaskManager* , User Selection을 전달하기 위해 필요.
	std::vector<CPoint> vFailPicker;
	BOOL bDuplicateAlarm;

	_ST_ALARM_RPT_OBJ()
	{
		nAlid = 0;
		pSender = NULL;
		vFailPicker.clear();
		bDuplicateAlarm = false;
	}
}ST_ALARM_RPT_OBJ, *LPST_ALARM_RPT_OBJ;


//==============================================================================
// STRUCTURE :
//==============================================================================
class TRobotName : public CObject
{
public:
	TRobotName()
	{
		nRobotId = 0;
		memset(szName, 0x00, sizeof(szName));
	};
	~TRobotName(){};

	int  nRobotId;
	char szName[128];
};

//==============================================================================
// STRUCTURE :
//==============================================================================
class _tDuplicateJamTime : public CObject
{
public:
	_tDuplicateJamTime()
	{
		dOccurdTime = 0.0;
		memset(szALID, 0x00, sizeof(szALID));
	};
	~_tDuplicateJamTime(){};

	char   szALID[32];
	double dOccurdTime;	
};

//==============================================================================
//
//==============================================================================
class CPickPoint
{
public:
	CPickPoint(int nStageLoc, std::vector<CPoint> vPicker, std::vector<CPoint> vPocket)
		: m_nStageLoc(nStageLoc), m_vPicker(vPicker), m_vPocket(vPocket)
	{
	};
	CPickPoint()
	{
		m_nStageLoc = 0;
		m_vPocket.clear();
		m_vPicker.clear();
	};
	CPickPoint(const CPickPoint& src)
	{
		m_nStageLoc = 0;
		m_vPocket   = src.m_vPocket;
		m_vPicker   = src.m_vPicker;
	}
	CPickPoint& operator = (const CPickPoint& src)
	{
		if( this == &src ){
			return *this;
		}
		m_nStageLoc = src.m_nStageLoc;
		m_vPocket   = src.m_vPocket;
		m_vPicker   = src.m_vPicker;
		return *this;
	}
	~CPickPoint(){};

	int                 m_nStageLoc;
	std::vector<CPoint> m_vPocket;
	std::vector<CPoint> m_vPicker;
};


typedef struct _St_PickerHeightJigPos
{
	double dJigPosX;
	double dJigPosY;
	double dJigPosXPitch;
	double dJigPosYPitch;

	_St_PickerHeightJigPos() { clear(); };
	void clear() { ZeroMemory(this, sizeof(_St_PickerHeightJigPos)); };
}St_PickerHeightJigPos;

//==============================================================================
// STRUCTURE :
//==============================================================================
enum eTestPpPicker{
// 	eTestPpPicker_RL=0,
// 	eTestPpPicker_RR,
// 	eTestPpPicker_FL,
// 	eTestPpPicker_FR,
	eMaxTestPpPicker=8,
};
enum eTestPpOffSet{
	eTestPpOffSet_X = 0,
	eTestPpOffSet_Y,
	eTestPpOffSet_Z_Pick,
	eTestPpOffSet_Z_Place,
	eMaxTestPpOffSet,
};
// typedef struct _ST_USER_OFFSET_PICKER{
// 	int nOffSet[eMaxTestPpOffSet];  // um 0:X, 1:Y 2:Z
// 	_ST_USER_OFFSET_PICKER()
// 	{
// 		memset(this, 0x00, sizeof(_ST_USER_OFFSET_PICKER));
// 	}
// }ST_USER_OFFSET_PICKER;
// typedef struct _ST_USER_OFFSET_SITE{
// 	ST_USER_OFFSET_PICKER stPicker[eMaxTestPpPicker];
// 	_ST_USER_OFFSET_SITE()
// 	{
// 		memset(this, 0x00, sizeof(_ST_USER_OFFSET_SITE));
// 	}
// }ST_USER_OFFSET_SITE;
// 
// typedef struct _ST_USER_OFFSET_TESTPP{
// 	ST_USER_OFFSET_SITE stTestSite[eMaxATS_Area];
// 	_ST_USER_OFFSET_TESTPP()
// 	{
// 		memset(this, 0x00, sizeof(_ST_USER_OFFSET_TESTPP));
// 	}
// }ST_USER_OFFSET_TESTPP;
// 
// typedef struct _ST_USER_OFFSET_TRAYPP{
// 	int stPickerX[eMaxLoadTblCount][MAX_TRAY_PP_CNT_X];
// 
// 	_ST_USER_OFFSET_TRAYPP()
// 	{
// 		memset(this, 0x00, sizeof(_ST_USER_OFFSET_TRAYPP));
// 	}
// }ST_USER_OFFSET_TRAYPP;

typedef struct _ST_USER_OFFSET_SITE_MP
{
	int nOffSet[eMaxTestPpOffSet]; //front site offset
	_ST_USER_OFFSET_SITE_MP()
	{
		memset(this, 0x00, sizeof(_ST_USER_OFFSET_SITE_MP));
	}
}ST_USER_OFFSET_SITE_MP;

typedef struct _ST_USER_OFFSET_TESTPP_MP 
{
	ST_USER_OFFSET_SITE_MP stTestSite[eMaxATS_Area];
	_ST_USER_OFFSET_TESTPP_MP()
	{
		memset(this, 0x00, sizeof(_ST_USER_OFFSET_TESTPP_MP));
	}
}ST_USER_OFFSET_TESTPP_MP;
 
// =================================================================================================== Vision Auto Teaching
enum eSpeed
{
	eSpeed_5 = 0,
	eSpeed_10,
	eSpeed_20,
	eSpeed_30,
	eSpeed_40,
	eSpeed_50,
	eSpeed_60,
	eSpeed_70,
	eSpeed_80,
	eSpeed_90,
	eSpeed_100,
	eSpeed_MAX,
};

enum eLocation 
{
	eLD_Table =0,
	eBuffer_Table,
	eClean_Table,
	eStation_DN1 ,
	eStation_DN2,
	eStation_DN3,
	eStation_DN4,
	eStation_UP1,
	eStation_UP2,
	eStation_UP3,
	eStation_UP4,

	eLocBTP_MAX,
};

enum eRow 
{
	eRow_1 = 0,
	eRow_2 ,
	eRow_3 ,
	eRow_4 ,

	eRowMAX ,
};


enum eDistance
{
	eDistance_10 = 0,
	eDistance_20,
	eDistance_30,
	eDistance_40,
	eDistance_50,
	eDistance_60,
	eDistance_70,
	eDistance_80,
	eDistance_90,
	eDistance_100,
	eDistance_110,
	eDistance_120,
	eDistance_130,
	eDistance_140,
	eDistance_150,
	eDistance_MAX,
};
/*
// enum eTestPPAxis
// {
// 	VAT_TestPP_X = 0,
// 	VAT_TestPP_Y,
// 	VAT_TestPP_Z,
// 	VAT_TestPP_AXIS_MAX,
// };


enum _eVAT3P_MEASURE_LOCATION
{
	VAT3P_MEASURE_NONE = -1,

	VAT3P_MEASURE_LEFT_TOP = 0,
	VAT3P_MEASURE_RIGHT_TOP,
	VAT3P_MEASURE_LEFT_BOTTOM,
	VAT3P_MEASURE_LOC_MAX,
};
enum _eVAT3P_MEASURE_KIND
{
	VAT3P_MEASURE_KIND_GRADIENT = 0,
	VAT3P_MEASURE_KIND_SCALE,
	VAT3P_MEASURE_KIND_TRAY_PITCH,
	VAT3P_MEASURE_KIND_MAX
};

enum _eSeqHandPitch
{
	VAT_PITCH_WIDE = 0,
	VAT_PITCH_NARW,
	VAT_PITCH_MAX
};

enum _eVAT_OPTION
{
	VAT_OPTION_MODE_INIT = 0,
	VAT_OPTION_MODE_PM,

	VAT_OPTION_POINT_2P = 0,
	VAT_OPTION_POINT_3P,

	VAT_OPTION_SPEED_50_PERCENT = 0,
	VAT_OPTION_SPEED_HANDLER_SETTING,

	VAT_OPTION_VISION_OK_SCALE_FACTOR = 0,
	VAT_OPTION_VISION_OK_USER_VALUE,

	VAT_OPTION_APPLY_GRADIENT_NONE = 0,
	VAT_OPTION_APPLY_GRADIENT_2P_OFFSET,
	VAT_OPTION_APPLY_GRADIENT_4P_OFFSET,

	VAT_OPTION_PLACE_MODE_NORMAL = 0,
	VAT_OPTION_PLACE_MODE_EACH_PICKER,
};

*/

enum _eVAT
{
	VAT_TRAY_LOAD_PP = 0,
	VAT_TRAY_UNLOAD_PP,
	VAT_TEST_PP1_LOAD,
	VAT_TEST_PP1_UNLOAD,
	VAT_TEST_PP2_LOAD,
	VAT_TEST_PP2_UNLOAD,
	VAT_MAX_ARM,

// 	VAT_X = 0,
// 	VAT_Y,
// 	VAT_Z,
// 	VAT_MAX_DIRECTION,
// 
// 	VAT_MAX_AXIS_XY = 2,
// 	VAT_MAX_AXIS_XYZ = 3,
// 
// 	VAT_MANUAL_CAM_CENTER = 0,
// 	VAT_CAM_CENTER,
// 	VAT_MAX_CAM_CENTER,
// 
// 	VAT_JIG_Z = 0,
// 	VAT_TARGET_Z,
// 	VAT_MAX_Z,
// 
// 	VAT_DESIGN_CAM_CENTER = 0,
// 	VAT_MEASURE_CAM_CENTER,
// 	VAT_MAX_CAM_CENTER_COUNT,
// 
// 	VAT_SEQ_MODE_CAM_CAL = 0,
// 	VAT_SEQ_MODE_PICKER_ALIGN,
// 
// 	VAT_MEASURE_HOLE_CENTER = 0,
// 	VAT_MEASURE_DEVICE_CENTER,
// 	VAT_MEASURE_HOLE_FOCUS,
// 	VAT_MEASURE_DEVICE_FOCUS,
};
enum _eMultiOriginStatus
{
	eMulti_Origin_None = 0,
	eMulti_Origin_Start,
	eMulti_Origin_End,
};

enum _eNumberOfLoadTable
{
	eNumberOfLoadTable_4Unit = 0,
	eNumberOfLoadTable_8Unit,
};


enum _ePickerHeightCalStatus
{
	ePickerHeightCal_Start =0,
	ePickerHeightCal_End,
};
enum _eVATProgressStatus
{
	eVATProgressStatus_IDLE = 0,
	eVATProgressStatus_Reserved,
	eVATProgressStatus_Running,
	eVATProgressStatus_Complete,
	eVATProgressStatus_NormalComplete,
};

/*
typedef struct _ST_VAT_3POINT{
	// 0: X, 1: Y, 2:Z
	double dCameraPosition[VAT_MAX_ARM][VAT_MAX_CAM_CENTER][VAT_MAX_DIRECTION];

	double dStd3PEdgePosTray[MAX_TRAY_PP_LOC_COUNT-1][VAT_MAX_DIRECTION-1];	                // TrayPP의 기구적 위치 값을 저장 [ C-Tray 의 Edge, Table(0,0)의 Cetner 값 ]
	double dStd3PEdgePosTest[eMaxTestPPCount][MAX_TEST_PP_LOC_COUNT-1][VAT_MAX_DIRECTION];	// TestPP의 기구적 위치 값을 저장 [ Site(0,0) 및 Table(0,0)의 Center 값 ]

	double dTest3P_MeasureLoc[eMaxTestPPCount][MAX_TEST_PP_LOC_COUNT-1][VAT_MAX_DIRECTION];	      // 찾은 위치
	double dTray3P_MeasureLoc[MAX_TRAY_PP_LOC_COUNT-1][VAT3P_MEASURE_LOC_MAX][VAT_MAX_DIRECTION]; // 찾은 위치

	double dTray3P_Gradient[MAX_TRAY_PP_LOC_COUNT-1][2];			// 기울기
	double dTray3P_ScalefactorRate[MAX_TRAY_PP_LOC_COUNT-1][2];	// 실제 Scale Factor 비율 (타겟거리에서 비율만큼의 이동거리를 더 가야함)
//	double d3PointTrayPitchOffset[_eMAXVAT3P_TRAYPP_LOCATION][2];					// 실제 Hand Pitch Offset

	_ST_VAT_3POINT() {
		memset(this, 0, sizeof(_ST_VAT_3POINT));
	}
}ST_VAT_3POINT;

// By jhRyu [180315] Vision Auto Teaching <Start>
typedef struct _St_VAT_Info
{
	double dCameraPosition[ VAT_MAX_CAM_CENTER ][ 2 ];
	double dCameraZPosition[ VAT_MAX_Z ];
	double dPitchJigBase[ VAT_MAX_AXIS_XY ];
	double dPickerAlignWidePitch[ 2 ];
	double dPickerAlignNarrowPitch[ 2 ];
	int		nAutoTeachingMode;
	_St_VAT_Info() {
		memset( this, 0x00, sizeof( _St_VAT_Info ) );
	}
} St_VAT_Info;

typedef struct _St_VAT_PickerInfo
{
	std::pair<double, double> pairPickerAlignWide[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ]; // Wide Pitch 상태에서 Picker 틀어짐.
	std::pair<double, double> pairPickerAlignNarrow[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ]; // Narrow Pitch 상태에서 picker 틀어짐.

	double dMeasureScaleFactor[ VAT_MAX_AXIS_XY ]; // X,Y Pitch 모터에 대한 Scale Factor 측정치.

	_St_VAT_PickerInfo() {
		for (int i = 0; i < VAT_MAX_AXIS_XY; i++)
			dMeasureScaleFactor[i] = 0;
		//memset( this, 0x00, sizeof( _St_VAT_PickerInfo ) );
	}
} St_VAT_PickerInfo;

typedef struct _St_VAT_Option
{
	// Teaching Option.
	int nTeachingHandSpeed; // 50% or Handler Setting Speed.
	int nVisionOKRangeType; // Vision Offset 허용 범위. Scale Factor or User Value.
	double dVisionUserOKRange;
	int nStdPicker[ 2 ]; // 기준피커 정보 ( [0] : X, [1] : Y )
	double dVisionWaitTime; // Vision Measure 전 Wait Time.
	int nApplyPickerAlignOffset; // Picker Align 측정 결과 보정 여부.

	int nPitchScaleFactorCalOption;

	_St_VAT_Option() {
		memset( this, 0x00, sizeof( _St_VAT_Option ) );
	}
} St_VAT_Option;
*/
typedef struct _ST_TRAY_ASSIGN{
	
	int nLoad1Top;
	int nLoad1Bottom;
	int nLoad2Top;
	int nLoad2Bottom;
	int nUnLoad1Top;
	int nUnLoad1Bottom;
	int nUnLoad2Top;
	int nUnLoad2Bottom;
	int nUnLoad3Top;
	int nUnLoad3Bottom;
	int nUnLoad4Top;
	int nUnLoad4Bottom;
	int nUnLoad5Top;
	int nUnLoad5Bottom;

	_ST_TRAY_ASSIGN()
	{
		memset(this, 0x00, sizeof(_ST_TRAY_ASSIGN));
	};

}ST_TRAY_ASSIGN;

typedef struct _ST_Set_DefaultBin {
	int nIndexCount;

	std::vector<CString> vDefaultBin;

	_ST_Set_DefaultBin() {
		clear();
	}

	void clear() {
		vDefaultBin.clear();
		nIndexCount = 0;
	}
}ST_Set_DefaultBin;

typedef struct _ST_2DID_SAME_LENGTH {

	int nX;
	int nY;
	CString strBarcode;

	_ST_2DID_SAME_LENGTH()
	{
		clear();
	};
	void clear() {
		strBarcode.Empty();
		nX = 0;
		nY = 0;
	}

}ST_2DID_SAME_LENGTH;

enum  FTP_Type{
	eFTP_Type_Server = 0,
	eFTP_Type_Network
};

/*TRobotName에 들어있는 nRobotId*/
enum TRobotNameRobotID
{
	tRobotId_PRESS_DN1_SITE1 = 0,
	tRobotId_PRESS_DN2_SITE2,
	tRobotId_PRESS_DN3_SITE3,
	tRobotId_PRESS_DN4_SITE4,
	tRobotId_PRESS_DN5_SITE5,
	tRobotId_PRESS_DN6_SITE6,
	tRobotId_PRESS_DN7_SITE7,
	tRobotId_PRESS_DN8_SITE8,
	tRobotId_TESTPP_1 = 20,
	tRobotId_TESTPP_2,
// 	tRobotId_TESTPP_3,
// 	tRobotId_TESTPP_4,
	tRobotId_SHUTTLE_1 = 30,
	tRobotId_SHUTTLE_2,
 	tRobotId_SHUTTLE_3,
 	tRobotId_SHUTTLE_4,
	tRobotId_TRAYPP_1 = 40,
	tRobotId_TRAYPP_2,
	tRobotId_FEEDER = 50,
	tRobotId_TRANSFER = 60,
};

enum eServoMotornAxisNo
{
	eAxisNo_PRESS_DN1_SITE1,
	eAxisNo_PRESS_DN2_SITE2,
	eAxisNo_PRESS_DN3_SITE3,
	eAxisNo_PRESS_DN4_SITE4,
	eAxisNo_PRESS_DN5_SITE5,
	eAxisNo_PRESS_DN6_SITE6,
	eAxisNo_PRESS_DN7_SITE7,
	eAxisNo_PRESS_DN8_SITE8

};

enum eCTrayHandTablePitchType
{
	_ePitchTypeX_1x=0,
	_ePitchTypeX_OneByOne,

	_ePitchTypeY_1x=0,
	_ePitchTypeY_OneByOne,
};

enum eVisionResultData
{
	eResData_DataCnt = 0,
	eResData_ResultStatus,
	eResData_Error_Discription,
	eResData_ResultPos,
};

enum eCompanyProjectList
{
	eAmkor_Maui = 0,
	eUSI_Maui,
	eAmkor_Marlin,
	eUSI_Marlin,
	eUSI_Mussel,
	eLuxshare_Mussel,
};



template< class T, int _col=0, int _row =0 >
struct stMatrix
{
private:
	typedef std::vector< T > row;
	std::vector< row > row_list;

public:

	stMatrix() {
		row_list.clear();
		for( int row = 0; row < _row; row++ ) 
		{
			std::vector< T > item;
			for( int col = 0; col < _col; col++ )
			{
				item.push_back( T() );
			}
			add_row( item );
		}
	}
	~stMatrix() {}

	stMatrix( const stMatrix& _src )
	{
		*this = _src;
	}
	const stMatrix& operator = ( const stMatrix& _src )
	{
		return *this;
	}



	void add_row( std::vector<T> l ) {
		row_list.push_back( l );
	}

	void add_row( T* _arr, int _size ) {
		std::vector< T > temp;
		for( int i = 0; i < _size; i++ ) {
			temp.push_back( _arr[ i ] );
		}

		add_row( temp );
	}

	int max_row_cnt() {
		return row_list.size();
	}

	int max_col_cnt() {
		if( row_list.size() > 0 ) {
			return row_list[ 0 ].size();
		}

		return 0;
	}

	int get_item_cnt( T _item ) {
		int total_cnt = 0;
		std::for_each( row_list.begin(), row_list.end()
					   , [&] ( row _r ) 
		{
			total_cnt += std::count( _r.begin(), _r.end(), _item );
		} );

		return total_cnt;
	}

	row operator[]( int row ) {
		return row_list[ row ];
	}
};


typedef struct _ST_SOCKETID_INFO {
	CString strSocketID;
	int nTD;
	int nTotalTD;
	int nLimit;
	int nStation;
	int nSite;

	_ST_SOCKETID_INFO() {
		clear();
	}

	void clear() {
		strSocketID.Empty();
		nTD = 0;
		nTotalTD = 0;
		nLimit = 0;
		nStation = -1;
		nSite = -1;
	}

}ST_SOCKEID_INFO;

typedef struct _ST_SOCKET_CONTACT_COUNT {
	int nIndexCount;
	
	std::vector<ST_SOCKEID_INFO> vSocketCountactInfo;

	_ST_SOCKET_CONTACT_COUNT() {
		clear();
	}

	void clear() {
		nIndexCount = 0;
		vSocketCountactInfo.clear();
	}
}ST_SOCKET_CONTACT_COUNT;

typedef struct _ST_CYLINDER_SPEC {
	double dMin;
	double dMax;
	_ST_CYLINDER_SPEC()
	{
		memset(this, 0x00, sizeof(_ST_CYLINDER_SPEC));
	};
}ST_CYLINDER_SPEC;

typedef struct _ST_CYLINDER_TIME_DATA {
	CString strUnitName;
	double dSpecMin;
	double dSpecMax;
	double dValue;

	_ST_CYLINDER_TIME_DATA() {
		clear();
	}

	void clear() {
		strUnitName.Empty();
		dSpecMin = 0.0;
		dSpecMax = 0.0;
		dValue = 0.0;
	}

}ST_CYLINDER_TIME_DATA;

enum eDeviceType
{
	_eDeviceType_Normal = 0,
	_eDeviceType_Cleaning,
};

enum eTrayPickDataOption
{
	eDeviceData_Nothing = 0,
	eDeviceData_Erase,
	eDeviceData_EraseAndAutoSkip,
};



enum eVibratorMode 
{
	CHECK_VIBRATOR_NOT_USE =0,
	CHECK_VIBRATOR_USE ,
};

enum eContactPosMode
{
	CHECK_CONTACT_POS_NOT_USE = 0,
	CHECK_CONTACT_POS_USE,
};

enum eCleanOffsetMode
{
	CHECK_CLEAN_OFFSET_NOT_USE = 0,
	CHECK_CLEAN_OFFSET_USE,
};


enum eOriginMode
{
	eMultiOriginExecuting = 0,
	eMultiOriginStop,
	eDlgProcCmd_PressIonizerOnOff,
};

enum eVibratorUnit
{
	ePressVibratorUnit = 0,
	eLoadTableVibratorUnit,
};

enum AgentCommandKind
{
	eCommandKind_Start = 0,
	eCommandKind_Status,
	eCommandKind_Config,
	eCommandKind_Max,
};

enum eLoadTblLeftRight
{
	eLoadTblLeft = 0,
	eLoadTblRight,
	eMaxLoadTblLeftRight,
};


enum eCylinderNumner
{									
	eEmpty_Stacker1_Main_UP = 1,
	eEmpty_Stacker1_Main_Down,
	eEmpty_Stacker1_Sub_UP,
	eEmpty_Stacker1_Sub_Down,
	eLoader_Stacker_Main_Up,
	eLoader_Stacker_Main_Down,
	eLoader_Stacker_Sub_Up,
	eLoader_Stacker_Sub_Down,
	eEmpty_Stacker2_Main_UP,
	eEmpty_Stacker2_Main_Down,
	eEmpty_Stacker2_Sub_UP,
	eEmpty_Stacker2_Sub_Down,
	eUnloader_Stacker1_Main_Up,
	eUnloader_Stacker1_Main_Down,
	eUnloader_Stacker1_Sub_Up	,
	eUnloader_Stacker1_Sub_Down	,
	eUnloader_Stacker2_Main_Up	,
	eUnloader_Stacker2_Main_Down,
	eUnloader_Stacker2_Sub_Up	,
	eUnloader_Stacker2_Sub_Down	,
	eUnloader_Stacker3_Main_Up	,
	eUnloader_Stacker3_Main_Down	,
	eUnloader_Stacker3_Sub_Up	,
	eUnloader_Stacker3_Sub_Down	,
	eUnloader_Stacker4_Main_Up	,
	eUnloader_Stacker4_Main_Down	,
	eUnloader_Stacker4_Sub_Up	,
	eUnloader_Stacker4_Sub_Down	,
	eTransfer_Z_axis_Up				,
	eTransfer_Z_axis_Down			,
	eCentering_Clamp1_Lock			,
	eCentering_Clamp1_Unlock		,
	eCentering_Clamp2_Lock			,
	eCentering_Clamp2_Unlock		,
	eCentering_Clamp3_Lock			,
	eCentering_Clamp3_Unlock		,
	eCentering_Clamp4_Lock			,
	eCentering_Clamp4_Unlock		,
	eCentering_Clamp5_Lock			,
	eCentering_Clamp5_Unlock		,
	eCentering_Clamp6_Lock			,
	eCentering_Clamp6_Unlock		,
	eCentering_Clamp7_Lock			,
	eCentering_Clamp7_Unlock		,
	e2D_Barcode_Left_Left			,
	e2D_Barcode_Left_Right			,
	e2D_Barcode_Right_Left			,
	e2D_Barcode_Right_Right			,
	eAnti_Vibration_1_Up			,
	eAnti_Vibration_1_Down			,
	eAnti_Vibration_2_Up			,
	eAnti_Vibration_2_Down			,
	eAnti_Vibration_3_Up			,
	eAnti_Vibration_3_Down			,
	eAnti_Vibration_4_Up			,
	eAnti_Vibration_4_Down			,
	eAnti_Vibration_5_Up			,
	eAnti_Vibration_5_Down			,
	eAnti_Vibration_6_Up			,
	eAnti_Vibration_6_Down			,
	eAnti_Vibration_7_Up			,
	eAnti_Vibration_7_Down			,
	eAnti_Vibration_8_Up			,
	eAnti_Vibration_8_Down			,
	eAnti_Vibration_9_Up			,
	eAnti_Vibration_9_Down			,
	eAnti_Vibration_10_Up			,
	eAnti_Vibration_10_Down			,
	eAnti_Vibration_11_Up			,
	eAnti_Vibration_11_Down			,
	eAnti_Vibration_12_Up			,
	eAnti_Vibration_12_Down			,
	eAnti_Vibration_13_Up			,
	eAnti_Vibration_13_Down			,
	eAnti_Vibration_14_Up			,
	eAnti_Vibration_14_Down			,
	eAnti_Vibration_15_Up			,
	eAnti_Vibration_15_Down			,
	eAnti_Vibration_16_Up			,
	eAnti_Vibration_16_Down			,

};

enum eTestPPType {
	eTestPPLoadType = 0,
	eTestPPUnLoadType = 1,
	eMaxTestPPTypeCount,
};

enum eTestPPTypeIdx {
	eTestPP_1_LoadType = 0,
	eTestPP_1_UnloadType,
	eTestPP_2_LoadType,
	eTestPP_2_UnloadType,
};


// Color
#define YELLOW_DUCK		RGB( 255, 254, 224  )
#define DUCK1			RGB(  50,  80,  150 )
#define BBLACK			RGB(   0,   0,   0	)	// XTColorRef.h 중복
#define WWHITE			RGB( 255, 255, 255	)	// XTColorRef.h 중복
#define	RED				RGB( 255,   0,	0	)
#define GREEN			RGB(   0, 255,   0	)
#define BLUE			RGB(   0,   0, 255	)
#define YELLOW			RGB( 255, 255,   0	)
#define MAGENTA			RGB( 255,   0, 255	)
#define CYAN			RGB(   0, 255, 255	)
#define CLOUDBLUE		RGB( 128, 184, 223  )
#define LLIGHTBLUE		RGB( 140, 140, 255	)
#define DDARKBLUE		RGB(   0,   0, 130	)
#define LLIGHTGREEN		RGB( 140, 255, 130	)
#define DDARKGREEN		RGB(   0, 130,   0	)
#define LLIGHTCYAN		RGB( 150, 230, 230	)
#define	DDARKCYAN		RGB(   0, 130, 130	)
#define LLIGHTRED		RGB( 255, 140, 140	)
#define DDARKRED		RGB( 128,   0,   0	)
#define LLIGHTMAGENTA	RGB( 255, 150, 255	)
#define DDARKMAGENTA	RGB( 150,  70, 150	)
#define LLIGHTBROWN		RGB( 255, 200, 150	)
#define DDARKBROWN		RGB( 230, 150,  50	)
#define LLIGHTGRAY		RGB( 210, 210, 210	)
#define DDARKGRAY		RGB(  50,  50,  50	)
#define LLIGHTYELLOW	RGB( 255, 255, 170	)
#define DDARKYELLOW		RGB( 140, 140, 110	)
#define BACK_COLOR		RGB(  61,	5, 165	)
#define DARKBLUE		RGB(   0,   0, 150  )
#define SKYBLUE			RGB(   0, 255, 255  )
#define DARKRED			RGB( 150,   0,   0  )
#define DARKGREEN		RGB(   0, 150,   0  )
#define DARKYELLOW		RGB( 150, 150,   0  )
#define NOMAL			RGB( 221, 221, 221  )
#define BACKGROUND		RGB( 255, 251, 240  )
#define ORANGE			RGB( 255, 187,   0  )
#define DISABLE_BACK_COLOR		RGB( 240, 240, 240	)
#define DISABLE_FORE_COLOR		RGB( 109, 109, 109	)
#define DISABLE_CELL_COLOR		RGB(200, 200, 200)

//VAT 사용
#define TAB_BACKGROUND_COLOR		RGB(240, 240, 240)
#define TAB_UNSELECT_FORE_COLOR		RGB(200, 200, 200)
#define SHEET_READ_ONLY_COLOR		RGB(110, 180, 180)

#define PROGRESS_BACK_COLOR			RGB(220, 240, 220) //연녹색
#define PROGRESS_BAR_COLOR			RGB(40, 200, 80) //Light Green

#define STATUS_BLINK_COLOR			RGB(90, 90, 180)
#define PICKER_ALIGN_PITCH_TYPE		RGB(144, 238, 144)

struct stErrCode {
	int ErrCode = 0;
	std::string msg = "";
	operator int() const {
		return ErrCode;
	};
	const bool operator != (const int& src) {
		return (this->ErrCode != src);
	};
	const bool operator == (const int& src) {
		return (this->ErrCode == src);
	};
};


enum eTransferState {
	eTransferState_Blocked=0,
	eTransferState_ReadyToLoad,
	eTransferState_ReadyToUnload,
};

typedef struct _ST_STATION_TO_IDENTITYNO{
	int nStationNo;
	int nIdentityNo;

		_ST_STATION_TO_IDENTITYNO() {
		clear();
	}

	void clear() {
		nStationNo = 0;
		nIdentityNo = 0;

	}

}ST_STATION_TO_IDENTITYNO;

enum eTesterStationToIdentityNo {
	eTesterStationNo = 0,
	eIdentityNo,
};

enum _eDoorSensor		
{	
	eFRONT_LEFT_DOORLOCK_SEN = 0,
	eFRONT_RIGHT_DOORLOCK_SEN	   ,
	eHINGED_RIGHT_DOORLOCK_SEN	   ,
	eSLIDE_RIGHT_F1_DOORLOCK_SEN   ,
	eSLIDE_RIGHT_R2_DOORLOCK_SEN   ,
	eREAR_RIGHT_DOORLOCK_SEN	   ,
	eREAR_LEFT_DOORLOCK_SEN		   ,
	eSLIDE_LEFT_R2_DOORLOCK_SEN	   ,
	eSLIDE_LEFT_F1_DOORLOCK_SEN	   ,
	eHINGED_LEFT_DOORLOCK_SEN	   ,
	eFRONT_LEFT2_DOORLOCK		   ,
	eMaxDoorSensor,
};

enum _eCw_CCw
{
	_eCW = 0,    //시계 방향 으로 회전
	_eCCW,  //시계 반대 방향 으로 회전
};

enum _eCheckTesterNullPointerMode
{
	_eCheckTesterNullPointerInstance = 0, // *Tester 객체 체크
	_eCheckTesterSockNullPointer,         // m_Sock 체크
	_eCheckTesterTotalNullPointer,        // *Tester,m_Sock 둘다 객체 체크
};

enum _eTestMode
{
	_eTestModeNull = -1,			// Test Mode Null
	_eTestModeSLT1 =0,				// FT MODE
	_eTestModeSLTQA1 =1,			// QA MODE
	_eMaxTestMode,					// Max Tester Mode
};

