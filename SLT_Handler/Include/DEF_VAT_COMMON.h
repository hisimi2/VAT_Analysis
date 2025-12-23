#pragma once
#include <unordered_map>
///////////////////////////////////////////////////////////////////////////
//Vision 없이 Auto Seq 검증 
// TRUE : Vision OS 없이 Auto 가능.
// FALSE : Vision OS 연동.
#define VAT_VISION_OS_BY_PASS		(FALSE)

//////////////////////////////////////////////////////////////////////////
//MACRO//
#define COPY_VAT_ARRAY(dest, src) \
    memcpy_s( \
        (dest), sizeof(dest), \
        (src), sizeof(src) \
    )

//Picker Align
#define SET_BIT_VAT_PICKER_INFO(hand, pitch, picker) \
    ((WPARAM)(((hand & 0xFF) << 16) | ((pitch & 0xFF) << 8) | (picker & 0xFF)))

#define GET_BIT_VAT_HAND(bit)      (((bit) >> 16) & 0xFF)
#define GET_BIT_VAT_PITCH(bit)     (((bit) >> 8) & 0xFF)
#define GET_BIT_VAT_PICKER(bit)    ((bit) & 0xFF)


//Picker Index -> (x,y)
#define VAT_PICKER_X(idx)  ((idx) % (4))
#define VAT_PICKER_Y(idx)  ((idx) / (4))


//DEFINE//
//simulation
#define VAT_SIM_MAX_ACTION_COUNT			(3)
#define VAT_SIM_CONTROL_TIME_SHORT			(500)
#define VAT_SIM_CONTROL_TIME_MIDLE			(1000)
#define VAT_SIM_CONTROL_TIME_LONG			(2000)

//GUI ALARM
#define ERR_VAT_TYPE_YESNO		  (0)
#define ERR_VAT_TYPE_OK			  (1)
#define ERR_VAT_TYPE_RETRYEXIT	  (2)
#define ERR_VAT_TYPE_NEXTEXIT	  (3)


//common
#define VAT_STRING_SIZE			 (256)

#define VAT_MOTOR_SPEED						(10)

#define VAT_MAX_HAND_CNT_X			(4)
#define VAT_MAX_HAND_CNT_Y			(2)



//Sequence
#define VAT_PRE_POS_100mm_OFFSET	(100000)
#define VAT_PRE_POS_150mm_OFFSET	(150000) //현재 사용 안함.

#define VAT_PRE_FAST_Z_POS_OFFSET	(2000)
#define VAt_Z_HEIGHT_CAL_PRE_POS_OFFSET	(2000)

//[Z Find Pos Offset]
#define VAT_PICKER_ALIGN_Z_FIND_MOVE_OFFSET		(100.0) //0.1mm

//[Picker Align]
#define VAT_PICKER_X_OFFSET_SPEC	(150) // 0.15mm
#define VAT_PICKER_Y_OFFSET_SPEC	(150) // 0.15mm

#define VAT_HAND_MINUMUM_X_PITCH	(11000)
#define VAT_HAND_MINUMUM_Y_PITCH	(11000)

//[Vision Auto Teaching]
#define VAT_PI	(3.141592653589793)

//[Z Auto Teaching]
#define VAT_Z_TEACHING_MOVE_OFFSET_FAST			(70.0)
#define VAT_Z_TEACHING_MOVE_OFFSET_SLOW			(20.0)




//User Message
#define UM_VAT_SEQ_MSG_MEASURE_COMPLETE	  (WM_USER + 5000)

#define UM_VAT_SEQ_MSG_UPDATE_HAND_STATUS		(WM_USER + 100)
#define UM_VAT_SEQ_MSG_UPDATE_PROGRESS			(WM_USER + 101)
#define UM_VAT_SEQ_MSG_UPDATE_VISION_STATUS		(WM_USER + 102)
#define UM_VAT_SEQ_MSG_UPDATE_VISION_OFFSET		(WM_USER + 103)
#define UM_VAT_SEQ_MSG_UPDATE_VISION_RETRY_CNT	(WM_USER + 104)
#define UM_VAT_SEQ_MSG_UPDATE_PICKER_STATUS		(WM_USER + 105)

#define UM_VAT_SEQ_MSG_UPDATE_SAVE_DATA			(WM_USER + 200)


//common
enum _eVAT_SEQ_MODE
{
	VAT_SEQ_MODE_Z_FOCUS = 0,
	VAT_SEQ_MODE_CAM_CAL,
	VAT_SEQ_MODE_PICKER_ALIGN,	
	VAT_SEQ_MODE_VAT,
	VAT_SEQ_MODE_Z_TEACHING,
	VAT_SEQ_MODE_MAX,

	//	VAT_SEQ_MODE_DATA_SAVE = 999,
};

enum _eVAT_SEQ_MODE_TYPE
{
	VAT_MODE_TYPE_NONE = -1,

	VAT_MODE_TYPE_HAND = 0,
	VAT_MODE_TYPE_PICKER,
	VAT_MODE_TYPE_PART,

	VAT_MODE_TYPE_MAX,
};

//임시
enum _eTemp
{
	VAT_HAND_TESTPP1 = 99,
	VAT_HAND_TESTPP2,
};


enum _eVAT_COMMON_HAND
{
	VAT_HAND_LOAD_TRAYPP = 0,
	VAT_HAND_UNLOAD_TRAYPP,
	VAT_HAND_LOAD_TESTPP1,
	VAT_HAND_UNLOAD_TESTPP1,
	VAT_HAND_LOAD_TESTPP2,
	VAT_HAND_UNLOAD_TESTPP2,
	VAT_HAND_MAX,
};


enum _eVAT_COMMON_HAND_PART
{
	//Tray PP(Load, Unload)
	VAT_IDX_TRAY_PP_TBL_1 = 0,
	VAT_IDX_TRAY_PP_TBL_2,
	VAT_IDX_TRAY_PP_TBL_3,
	VAT_IDX_TRAY_PP_TBL_4,

	VAT_IDX_TRAY_PP_CONV_1,
	VAT_IDX_TRAY_PP_CONV_2,
	VAT_IDX_TRAY_PP_CONV_3,
	VAT_IDX_TRAY_PP_CONV_4,
	VAT_IDX_TRAY_PP_CONV_5,
	VAT_IDX_TRAY_PP_CONV_6,
	VAT_IDX_TRAY_PP_CONV_7,

	VAT_IDX_TRAY_PP_FEEDER,

	VAT_MAX_TRAY_PP_LOC_COUNT,

	//Test PP
	VAT_IDX_STATION_1 = 0,
	VAT_IDX_STATION_2,
	VAT_IDX_STATION_3,
	VAT_IDX_STATION_4,

	VAT_IDX_TEST_PP_TBL_1_LD,
	VAT_IDX_TEST_PP_TBL_2_LD,
	VAT_IDX_TEST_PP_TBL_3_LD,
	VAT_IDX_TEST_PP_TBL_4_LD,

	VAT_IDX_TEST_PP_TBL_1_UD,
	VAT_IDX_TEST_PP_TBL_2_UD,
	VAT_IDX_TEST_PP_TBL_3_UD,
	VAT_IDX_TEST_PP_TBL_4_UD,

	VAT_IDX_CLEAN_TBL,

	VAT_MAX_TEST_PP_LOC_COUNT,

};


//[VAT를 진행 하기 위한 Part Index 관리]
//TWSL421의 경우 Hand만 VAT진행 하는 것이 아닌, 독립된 Part도 VAT를 진행.
//독립적인 Unit이 여러개의 Teaching Point가 존재하기 때문.
//ex) Table의 경우 Test Site 4가지 teaching 위치가 존재함.
enum _eVAT_COMMON_PART_ONLY
{
	VAT_PART_FEEDER = 0,
	VAT_PART_TABLE_1,
	VAT_PART_TABLE_2,
	VAT_PART_TABLE_3,
	VAT_PART_TABLE_4,

	VAT_PART_MAX,

	VAT_PART_NONE = -1,
};

//Hand에 관련된 Part가 아닌, Part 독립적인 Position을 의미함.
enum _eVAT_PART_ONLY_POS
{
	//Feeder Base Position
// 	VAT_FEEDER_LOAD_PICK_START = 0,
// 	VAT_FEEDER_PICK,
// 	VAT_FEEDER_PLACE,
	VAT_FEEDER_C_TRAY_JIG = 0,
	VAT_MAX_FEEDER_LOC_COUNT,

	//Table Base Position
	VAT_TBL_LOAD_SIDE = 0,
	VAT_TBL_TEST_SIDE_1,
	VAT_TBL_TEST_SIDE_2,
	VAT_TBL_TEST_SIDE_3,
	VAT_TBL_TEST_SIDE_4,
	VAT_MAX_TBL_LOC_COUNT,
};


//Common Cmd 내에서 Part가 아닌 다른 Unit 측정 시, 사용
//_eVAT_COMMON_PART에 해당 하는 것과 Index 중첩하여 사용 하지 말것.
enum _eVAT_COMMON_NON_PART
{
	VAT_NON_PART_PLATAE_JIG = 100,
};



enum _eVAT_AXIS
{
	//[Hand]
	VAT_X = 0,
	VAT_Y,
	VAT_Z,
	VAT_PITCH_X,
	VAT_PITCH_Y,
	VAT_MAX_AXIS,

 	VAT_MAX_AXIS_XYZPITCH = 5, //현재 사용 X
 	VAT_MAX_AXIS_XY = 2,       
	VAT_MAX_AXIS_X_Y_XPITCH_YPICTH = 4, //현재 VAT에서 사용 X
	VAT_MAX_AXIS_XYZ = 3,

	//[Part] - feeder/table
	VAT_PART_Y = 0,
	VAT_PART_ROTATE,
	VAT_MAX_AXIS_YROTATE = 2, //(table)
	VAT_MAX_AXIS_Y = 1,       //(feeder)
};


enum _eVAT_COMMON_TABLE_OWNER
{
	VAT_TABLE_OWNER_NONE = -1,
	VAT_TABLE_1_OWNER = 0,
	VAT_TABLE_2_OWNER,
	VAT_TABLE_3_OWNER,
	VAT_TABLE_4_OWNER,
	VAT_TABLE_OWNER_MAX,
};


enum _eVAT_PITCH_TYPE
{
	VAT_PITCH_TYPE_WIDE = 0,
	VAT_PITCH_TYPE_NARROW,

	VAT_PITCH_TYPE_MAX,
};

//Picker Align Data 정보 Type
enum _eVAT_PICKER_ALIGN_INFO_TYPE
{
	VAT_DATA_INFO_TYPE_WIDE = 0,
	VAT_DATA_INFO_TYPE_NARROW,
	VAT_DATA_INFO_TYPE_WIDE_TO_NARROW,

	VAT_DATA_INFO_TYPE_MAX,
};

enum _eVAT_DIRECTION
{
	VAT_DIRECTION_PLUSE = 0,
	VAT_DIRECTION_MINUS,
};


//Vision Auto Teaching Mode 측정 Type
enum _eVAT_MEASURE_TYPE
{
	VAT_MEASURE_TYPE_BOTH = 0,
	VAT_MEASURE_TYPE_HAND_ONLY,
	VAT_MEASURE_TYPE_UNIT_ONLY,

	VAT_MEASURE_TYPE_NONE = -1,
};

/*===============================================================*/
//COMMENT : Hand 또는 Part의 측정 상태
/*============================================================*/
enum _eVAT_MeasureStatus//_eVAT_COMMON_SELECTED_PART_STAUS
{
	//VAT_STATUS_PREPARE = 0,
	VAT_STATUS_MEASURE_INIT = 0,		//Unit 선택 되지 않음.
	VAT_STATUS_MEASURE_READY,			//선택된 Unit 측정 전 상태.
	VAT_STATUS_MEASURE_WAIT,			//선택된 Unit 측정 대기 상태.(다른 Hand에서 측정 중일 경우)
	VAT_STATUS_MEASURE_ONGOING,			//선택된 Unit 측정 진행 중 상태.	
	VAT_STATUS_MEASURE_COMPLETE,		//선택된 Unit 측정 완료 상태.	

	VAT_STATUS_DATA_SAVE_COMPLETE,

	VAT_STATUS_MAX,
};


enum _eVAT_Picker_No
{
	VAT_PICKER_01 = 0,
	VAT_PICKER_02,
	VAT_PICKER_03,
	VAT_PICKER_04,
	VAT_PICKER_05,
	VAT_PICKER_06,
	VAT_PICKER_07,
	VAT_PICKER_08,

	VAT_PICKER_MAX,
};

//////////////////////////////////////////////////Data
enum _eVAT_MODE_EACH_DATA_TYPE
{
	//Z Auto Focus
	eZFocus_PlateJigTargetPos = 0,
	eZFocus_PlateJigVacPos,
	eZFocus_PlateJigLowerOffset,
	eZFocusType_Max,

	//Cam Center Calibration
	eCamCenCal_BotCamCenterPos = 0,
	eCamCenCal_TopCamToPkrDis,
	eCamCenCalType_Max,

	//Picker Align
	ePickerAlign_SelectedPkr = 0,
	ePickerAlign_EachPickerOffset,
	ePickerAlign_EachPickerPos,
	ePickerAlign_AvgOffset,
	ePickerAlign_ZFocusPos,
	ePickerAlign_PreSettingPos,
	ePickerAlign_Max,

	//얘는 삭제 필요 사항.
	ePickerAlign_BeforePickerSetPos = 0,
	ePickerAlign_AfterPickerSetPos,


	//Vision Auto Teaching
	eVisionAutoTeaching_HandBasePos = 0, //일단 하나밖에 없어서 임시
	eVisionAutoTeaching_PartBasePos,


	//Z Auto Teaching
	eZAutoTeaching_ZTeachBasePos = 0,
	eZAutoTeaching_PickPlaceOffset,
	eZAutoTeaching_PickerFlatnessOffset,
	eZAutoTeaching_PickerVacPos,

	//Design Value
	eDesignValue_ZAutoFocus = 0,
	eDesignValue_CamCenterCalibration,
	eDesignValue_PickerAlign,
	eDesignValue_ZAutoTeaching,
	eDesignValue_VisionAutoTeaching,


	//Option
	eVatOption_PkrAutoFocus = 0,

	eModeEachDataType_None = -1,

};


//////////////////////////////////////////////////Data
// enum _eVAT_PART_ONLY_TARGET_POSITION {
// 	eVat_Table_LoadSide = 0,
// 	
// 	
// 	
// 	
// 	
// 	eVAT_Feeder_PickPos = 99, //지금 사용 안함.
// };




//====================================================================Design Value

// [1] 수정 불가능한 Design Value
enum _eVAT_DesignValue_Unmodifiable {
	//Tray Pp (LD/UD)	
	VAT_TRAY_PP_BASE_TO_TABLE = 0,
	VAT_TRAY_PP_BASE_TO_FEEDER_OR_STACKER,

	//Test Pp (LD/UD)
	VAT_TEST_PP_BASE_TO_TABLE = 0,
	VAT_TEST_PP_BASE_TO_STATION,
	VAT_TEST_PP_BASE_TO_CLEAN_TABLE,


	VAT_MAX_BASE_TO_UNIT = 3,
};




// [2] 작업자가 수정 가능한 Design Value
enum _eVAT_DesignValue_Modifiable {
	//1. Z Auto Focus
	//2. Cam Center Calibration
	//3. Picker Align
	//4. Vision Auto Teaching
	//5. Z Auto Teaching
	VAT_TABLE_HEIGHT = 0, //VAT_TABLE_HEIGHT
	VAT_TABLE_POCKET_Z,
	VAT_CLEAN_TABLE_HEIGHT,
	VAT_CLEAN_TABLE_POCKET_Z,
	VAT_C_TRAY_HEIGHT,
	VAT_C_TRAY_POCKET_Z,
	VAT_STATION_HEIGHT,
	VAT_STATION_SOCKET_Z,
	VAT_C_TRAY_JIG_HEIGHT,
	VAT_DEVICE_HEIGHT,

	VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING,

};

//=======================================================================

enum _eVAT_Z_PickPlace_Offset {
	//===============================================Type
	VAT_Z_PICK_OFFSET = 0,
	VAT_Z_PLACE_OFFSET,
	VAT_Z_OFFSET_TYPE_MAX,

	//===============================================Unit
	VAT_LD_TRAY_PP_UNIT_TABLE = 0,	//LD Tray PP
	VAT_LD_TRAY_PP_UNIT_FEEDER,		//LD Tray PP
	VAT_LD_TRAY_PP_UNIT_MAX,		//LD Tray PP	

	VAT_UD_TRAY_PP_UNIT_TABLE = 0,	//UD Tray PP
	VAT_UD_TRAY_PP_UNIT_C_TRAY,		//UD Tray PP
	VAT_UD_TRAY_PP_UNIT_MAX,		//UD Tray PP

	VAT_LD_TEST_PP_UNIT_TABLE = 0,		//LD Test PP
	VAT_LD_TEST_PP_UNIT_STATION,		//LD Test PP
	VAT_LD_TEST_PP_UNIT_CLEAN_TABLE,	//LD Test PP
	VAT_LD_TEST_PP_UNIT_MAX,			//LD Test PP

	VAT_UD_TEST_PP_UNIT_TABLE = 0,		//LD Test PP
	VAT_UD_TEST_PP_UNIT_STATION,		//LD Test PP
	VAT_UD_TEST_PP_UNIT_MAX,			//LD Test PP

	VAT_Z_OFFSET_UNIT_MIN = 2,
	VAT_Z_OFFSET_UNIT_MAX = 3,
};

enum _eVAT_Data_Save_Case
{
	eVat_Normal,
	eVat_Abort,
};


enum _eVAT_TEST_HAND_TYPE
{
	VAT_TESTPP_LOAD = 0,
	VAT_TESTPP_UNLOAD,
	VAT_TESTPP_HAND_TYPE,
};

enum _eVAT_INTERLOCK
{
	eVAT_INTERLOCK_PASS = 0,
	eVAT_INTERLOCK_FAIL,
};

/*===============================================*/
//COMMENT : GUI 
/*===============================================*/
enum _eVAT_TAB_TYPE
{
	eVatTabType_CellChange = 0,
	eVatTabType_CellNotChange,
};

enum _eVAT_SPREAD_SHEET_OPER_INDEX
{
	eSSOper_ENCODER_READ = 0,
	eSSOper_MOTOR_MOVING,
	eSSOper_DATA_RELOAD,
	eSSOper_DATA_SAVE,
	eSSOper_MAKE_BASE_POS,
	eSSOper_MAX,
};

enum _eVAT_SPREAD_SHEET_TYPE
{
	eVAT_SSTYPE_HAND = 0,			
	eVAT_SSTYPE_PICKER,				
	eVAT_SSTYPE_PART,
	eVAT_SSTYPE_PART_ONLY,

	//기존 Hand/Part Type이 아닌, 일반 Spread Sheet.
	eVAT_SSTYPE_NORMAL = 99, 
	eVAT_SSTYPE_ENGINEER,    //Operate Btn의 경우, Engineer Type Sheet만 동작 가능함.
};

enum  _eVAT_SPREAD_SHEET_ACCESS_TYPE
{
	eVAT_SSACCESSTYPE_RW = 0, //Read & Write
	eVAT_SSACCESSTYPE_RO,     //Read Only
};

enum _eVAT_DATA_SHOW_TYPE
{
	eVAT_SSDISPLAYTYPE_DATA = 0,
	eVAT_SSDISPLAYTYPE_DIFFERENCE,
};

//현재 사용 안함
enum _eVAT_TAB_SELCHANGE
{
	eVatTabType_Data =0,
	eVatTabType_ValueDiff, //이전 Data와 값 차이.
};



///////////AUTO & CMD
//Common

#define DEF_VAT_MOTOR_SPEED_RATE	(10) //10%
#define DEF_VAT_MOTOR_ACC_RATE		(10)
#define DEF_VAT_MOTOR_DCC_RATE		(10)


// enum _eVAT_HAND_AXIS
// {
// 	VAT_HAND_NO_USE = -1,
// 
// 	VAT_HAND_AXIS_X = 0,
// 	VAT_HAND_AXIS_Y,
// 	VAT_HAND_AXIS_Z,
// 	VAT_HAND_AXIS_XP,
// 	VAT_HAND_AXIS_YP,
// 
// 	VAT_HAND_MAX_AXIS,
// 
// };


/*===============================================*/
//COMMENT : Z Auto Focus
/*===============================================*/
//#define DEF_VAT

/*===============================================*/
//COMMENT : Cam Center Calibration
/*===============================================*/



/*===============================================*/
//COMMENT : Picker Align
/*===============================================*/


/*===============================================*/
//COMMENT : Vision Auto Teaching
/*===============================================*/
enum _eVAT_Vision_Auto_Teaching_Table_Target
{
	eTableTarget_None = -1,

	eTableTarget_1 = 0,
	eTableTarget_2,
	eTableTargetMax,

	eTableTarget_VAT = 99,
};



/*===============================================*/
//COMMENT : Z Auto Teaching
/*===============================================*/
enum _eVAT_Z_Auto_Teaching_Speed {
	eZSpeed_None = -1,
	eZSpeed_Fast = 0,
	eZSpeed_Slow
};

//===========================================================[STRUCTURE]

//Hand X,Y,Z 정보 - Auto 및 Cmd에서 사용.
typedef struct _ST_VAT_HAND_XYZ_INFO
{
	int x_axis_no;
	int y_axis_no;
	int z_axis_no;

	_ST_VAT_HAND_XYZ_INFO(int param1, int param2, int param3) :
		x_axis_no(param1),
		y_axis_no(param2),
		z_axis_no(param3)
	{}

	_ST_VAT_HAND_XYZ_INFO() { clear(); };
	void clear() {
		x_axis_no = -1; y_axis_no = -1;  z_axis_no = -1;
	};
}ST_VAT_HAND_XYZ_INFO;

static const std::unordered_map<int, ST_VAT_HAND_XYZ_INFO> cHandXYZParamMap = {
	{ VAT_HAND_LOAD_TRAYPP,{ eAXIS_TRAY_PP_1_X,  eAXIS_TRAY_FEEDER_Y,  eAXIS_TRAY_PP_1_Z } },
	{ VAT_HAND_UNLOAD_TRAYPP,{ eAXIS_TRAY_PP_2_X,  eAXIS_TRAY_PP_2_Y,  eAXIS_TRAY_PP_2_Z } },
	{ VAT_HAND_LOAD_TESTPP1,{ eAXIS_TEST_PP_1_X,  eAXIS_TEST_PP_1_Y,  eAXIS_TEST_PP_1_Z } },
	{ VAT_HAND_UNLOAD_TESTPP1,{ eAXIS_TEST_PP_1_X2,  eAXIS_TEST_PP_1_Y,  eAXIS_TEST_PP_1_Z2 } },
	{ VAT_HAND_LOAD_TESTPP2,{ eAXIS_TEST_PP_2_X,  eAXIS_TEST_PP_2_Y,  eAXIS_TEST_PP_2_Z } },
	{ VAT_HAND_UNLOAD_TESTPP2,{ eAXIS_TEST_PP_2_X2,  eAXIS_TEST_PP_2_Y,  eAXIS_TEST_PP_2_Z2 } },
};

typedef struct _ST_VAT_PART_AXIS_INFO
{
	int axis_no_1; //Y축
	int axis_no_2; //Rotate

	_ST_VAT_PART_AXIS_INFO(int param1, int param2) :
		axis_no_1(param1), axis_no_2(param2)
	{}

	_ST_VAT_PART_AXIS_INFO() { clear(); };
	void clear() {
		axis_no_1 = -1; axis_no_2 = -1;
	};
}ST_VAT_PART_AXIS_INFO;

static const std::unordered_map<int, ST_VAT_PART_AXIS_INFO> cPartAxisParamMap = {
	{ VAT_PART_FEEDER, { eAXIS_TRAY_FEEDER_Y, -1} },
	{ VAT_PART_TABLE_1,{ eAXIS_LOAD_TABLE_1_Y, eAXIS_LOAD_TABLE_1_ROTATE } },
	{ VAT_PART_TABLE_2,{ eAXIS_LOAD_TABLE_2_Y, eAXIS_LOAD_TABLE_2_ROTATE } },
	{ VAT_PART_TABLE_3,{ eAXIS_LOAD_TABLE_3_Y, eAXIS_LOAD_TABLE_3_ROTATE } },
	{ VAT_PART_TABLE_4,{ eAXIS_LOAD_TABLE_4_Y, eAXIS_LOAD_TABLE_4_ROTATE } },
};



//Hand Pitch 정보 - GUI 및 Auto에서 Cmd Position 가져 올때 사용.

typedef struct _ST_VAT_HAND_PITCH_INFO
{
	int robot_id;

	int xp_pos_idx;
	int yp_pos_idx;

	int xp_axis_no;
	int yp_axis_no;

	_ST_VAT_HAND_PITCH_INFO(int param1, int param2, int param3, int param4, int param5) :
		robot_id(param1),
		xp_pos_idx(param2),
		yp_pos_idx(param3),
		xp_axis_no(param4),
		yp_axis_no(param5)
	{}


	_ST_VAT_HAND_PITCH_INFO() { clear(); };
	void clear() {
		robot_id = -1; xp_pos_idx = -1;  yp_pos_idx = -1; xp_axis_no = -1; yp_axis_no = -1;
	};
}ST_VAT_HAND_PITCH_INFO;


static const std::unordered_map<int, ST_VAT_HAND_PITCH_INFO> cHandPitchParamMap = {
	{ VAT_HAND_LOAD_TRAYPP,{ RID_TRAY_PP_(RIDX_TRAY_PP_1),  POS_IDX_TRAY_LOAD_PP_X_PITCH, POS_IDX_TRAY_LOAD_PP_Y_PITCH,  eAXIS_TRAY_PP_1_X_PITCH,  eAXIS_TRAY_PP_1_Y_PITCH } },
	{ VAT_HAND_UNLOAD_TRAYPP,{ RID_TRAY_PP_(RIDX_TRAY_PP_2), POS_IDX_TRAY_PP_X_PITCH,  POS_IDX_TRAY_PP_Y_PITCH, eAXIS_TRAY_PP_2_X_PITCH,  eAXIS_TRAY_PP_2_Y_PITCH } },
	{ VAT_HAND_LOAD_TESTPP1,{ RID_TEST_PP_(RIDX_TEST_PP_1), POS_IDX_TEST_PP_X1_PITCH,  POS_IDX_TEST_PP_Y1_PITCH, eAXIS_TEST_PP_1_X_PITCH,  eAXIS_TEST_PP_1_Y_PITCH } },
	{ VAT_HAND_UNLOAD_TESTPP1,{ RID_TEST_PP_(RIDX_TEST_PP_1), POS_IDX_TEST_PP_X2_PITCH, POS_IDX_TEST_PP_Y2_PITCH, eAXIS_TEST_PP_1_X2_PITCH, eAXIS_TEST_PP_1_Y2_PITCH } },
	{ VAT_HAND_LOAD_TESTPP2,{ RID_TEST_PP_(RIDX_TEST_PP_2), POS_IDX_TEST_PP_X1_PITCH,  POS_IDX_TEST_PP_Y1_PITCH, eAXIS_TEST_PP_2_X_PITCH,  eAXIS_TEST_PP_2_Y_PITCH } },
	{ VAT_HAND_UNLOAD_TESTPP2,{ RID_TEST_PP_(RIDX_TEST_PP_2), POS_IDX_TEST_PP_X2_PITCH, POS_IDX_TEST_PP_Y2_PITCH, eAXIS_TEST_PP_2_X2_PITCH, eAXIS_TEST_PP_2_Y2_PITCH } },
};

/////////////////////////////////////////////////////////////////////////

//<VISION Communication>
#define DEF_VAT_SIMUL_RETRY_CNT							(3)
#define DEF_VAT_VISION_RETRY_CNT_MAX					(10)
#define DEF_VAT_VISION_RETRY_CNT_FIND_Z					((g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) ? 20 :2)

//_SEND/RECEIVE DATA_ID
//[PC1]
//Request
#define VISION_REQ_DATAID_VAT_LD_UPPER						50004
#define VISION_REQ_DATAID_VAT_ULD_UPPER						50005
#define VISION_REQ_DATAID_VAT_TEST_1_LD_UPPER 				50006
#define VISION_REQ_DATAID_VAT_TEST_1_ULD_UPPER 				50007
#define VISION_REQ_DATAID_VAT_LD_LOWER 						50008
#define VISION_REQ_DATAID_VAT_ULD_LOWER 					50009
#define VISION_REQ_DATAID_VAT_TEST_1_LOWER 					50010

//Receive
#define VISION_RCV_DATAID_VAT_LD_UPPER						60004
#define VISION_RCV_DATAID_VAT_ULD_UPPER						60005
#define VISION_RCV_DATAID_VAT_TEST_1_LD_UPPER 				60006
#define VISION_RCV_DATAID_VAT_TEST_1_ULD_UPPER 				60007
#define VISION_RCV_DATAID_VAT_LD_LOWER 						60008
#define VISION_RCV_DATAID_VAT_ULD_LOWER 					60009
#define VISION_RCV_DATAID_VAT_TEST_1_LOWER 					60010


//[PC2]
//Request
#define VISION_REQ_DATAID_VAT_TEST_2_LD_UPPER				51004
#define VISION_REQ_DATAID_VAT_TEST_2_ULD_UPPER 				51005
#define VISION_REQ_DATAID_VAT_TEST_2_LOWER					51006

//Receive
#define VISION_RCV_DATAID_VAT_TEST_2_LD_UPPER				61004
#define VISION_RCV_DATAID_VAT_TEST_2_ULD_UPPER 				61005
#define VISION_RCV_DATAID_VAT_TEST_2_LOWER					61006




//__MEASURE STATUS [PROTOCOL]
#define VISION_STATUS_PLATE_JIG						(0)
#define VISION_STATUS_PICKER						(1)
#define VISION_STATUS_C_TRAY						(2)
#define VISION_STATUS_TABLE							(3)
#define VISION_STATUS_SOCKET						(4)
#define VISION_STATUS_AUTO_FOCUS					(10)
#define VISION_STATUS_LED_ON						(20)
#define VISION_STATUS_LED_OFF						(21)
#define VISION_STATUS_FIND_Z_MEASURE_PLATE_JIG		(30)
#define VISION_STATUS_FIND_Z_MEASURE_PICKER			(31)
#define VISION_STATUS_FIND_Z_MEASURE_C_TRAY			(32)
#define VISION_STATUS_FIND_Z_MEASURE_TABLE			(33)
#define VISION_STATUS_FIND_Z_MEASURE_SOCKET			(34)
#define VISION_STATUS_FIND_Z_RESULT_PLATE_JIG		(40)
#define VISION_STATUS_FIND_Z_RESULT_PICKER			(41)
#define VISION_STATUS_FIND_Z_RESULT_C_TRAY			(42)
#define VISION_STATUS_FIND_Z_RESULT_TABLE			(43)
#define VISION_STATUS_FIND_Z_RESULT_SOCKET			(44)


//__ERROR DISCRIPTION [PROTOCOL]
#define VISION_ERROR_INSPECTION_SEQ_FINISH_WAIT		(10)
#define VISION_ERROR_MEASURE_TYPE					(11)
#define VISION_ERROR_INSPECTION_SEQ_START			(12)
#define VISION_ERROR_CAMERA_RESET_SEQ_START			(13)
#define VISION_ERROR_CAMERA_RESER_RESULT_WAIT		(14)
#define VISION_ERROR_PRE_GRAB_SET					(15)
#define VISION_ERROR_GRAB_FAIL						(16)
#define VISION_ERROR_GRAB_WAIT						(17)
#define VISION_ERROR_INSPECTION						(100)
#define VISION_ERROR_INSPECTION_TIMEOUT				(101)
#define VISION_ERROR_RESULT_DATA_ERROR				(102)

enum _eVATCamIndex {
	VAT_CAM_LD_TRAY_PP_UPPER = 0,
	VAT_CAM_LD_TRAY_PP_LOWER,

	VAT_CAM_UD_TRAY_PP_UPPER,
	VAT_CAM_UD_TRAY_PP_LOWER,

	VAT_CAM_LD_TEST_PP_1_UPPER,
	VAT_CAM_UD_TEST_PP_1_UPPER,
	VAT_CAM_TEST_PP_1_LOWER,

	VAT_CAM_LD_TEST_PP_2_UPPER,
	VAT_CAM_UD_TEST_PP_2_UPPER,
	VAT_CAM_TEST_PP_2_LOWER,

	VAT_CAM_MAX,
};


enum eVatRcvStatus {
	VAT_RCV_STATUS_INIT = 98,
	VAT_RCV_STATUS_IDLE = 99,
	VAT_RCV_STATUS_DISCONNECT = 100,

	VAT_RCV_STATUS_SUCCESS = 0, //통신 프로토콜
	VAT_RCV_STATUS_ERROR,		//통신 프로토콜

};

typedef struct _ST_VAT_RCV_DATA {
	int nState;
	double dX;
	double dY;
	int nFocusedZIdx; //초점 맞는 높이 Idex값
	int nError;

	void clear() {
		nState = VAT_RCV_STATUS_INIT;
		dX = 0.0;
		dY = 0.0;
		nFocusedZIdx = 0;
		nError = 0;
	}

	_ST_VAT_RCV_DATA() {
		clear();
	}
}ST_VAT_RCV_DATA;


//Vision PC
enum _eVAT_VISION_PC 
{
	VAT_VISION_PC_1 = 0,
	VAT_VISION_PC_2,

	VAT_VISION_PC_MAX
};


// Cam  Type
// enum _eVAT_CAM
// {
// 	VAT_UPPER_CAM = 0,
// 	VAT_LOWER_CAM,
// 
// 	VAT_CAM_TYPE_MAX,
// };

enum _eVAT_VISION_OFFSET
{
	VAT_VISION_OFFSET_X = 0,
	VAT_VISION_OFFSET_Y,
	VAT_VISION_OFFSET_MAX,
};

//=================Measure Status 관리 구조체
//SingleTone에서 사용함.

// typedef struct _ST_VAT_MEASURE_STATUS
// {
//	int nCurPercent;
//	int nCamStatus;
//	int nVisionOffset[VAT_VISION_OFFSET_MAX];

//	void clear() {
//		nCurPercent = 0;
//		nCamStatus = VAT_RCV_STATUS_INIT;
//		memset(nVisionOffset, 0, sizeof(nVisionOffset));
//	}

//	_ST_VAT_MEASURE_STATUS() { clear();}
//}ST_VAT_MEASURE_STATUS;