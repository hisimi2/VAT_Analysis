#pragma once

#include <vector>
#include <afx.h>
#include <afxmt.h>
#include <initializer_list>
#include "../../Include/DEF_COMMON.h"
#include "../../Include/DEF_VAT_COMMON.h"
#include "VAT_FILE_SECTION.h"


#define VAT_DATA_PATH_Z_FOCUS					(0)
#define VAT_DATA_PATH_CAM_CENTER_CAL			(1)
#define VAT_DATA_PATH_PICKER_ALIGN				(2)
#define VAT_DATA_PATH_VISION_AUTO_TEACHING		(3)
#define VAT_DATA_PATH_Z_AUTO_TEACHING			(4)
#define VAT_DATA_PATH_PART_FEEDER				(5)
#define VAT_DATA_PATH_PART_TABLE1				(6)
#define VAT_DATA_PATH_PART_TABLE2				(7)
#define VAT_DATA_PATH_PART_TABLE3				(8)
#define VAT_DATA_PATH_PART_TABLE4				(9)
#define VAT_DATA_PATH_DESIGN_VALUE				(10) //수정 가능한 Design Value
#define VAT_DATA_PATH_OPTION					(11)		



enum _eVAT3P_EDGE
{
	VAT3P_EDGE_NONE = -1,

	VAT3P_EDGE_LEFT_TOP = 0,
	VAT3P_EDGE_RIGHT_TOP,
	VAT3P_EDGE_LEFT_BOTTOM,
	VAT3P_EDGE_RIGHT_BOTTOM, // Z축 4 Point Teaching
	VAT3P_EDGE_MAX
};


enum _eVAT_TEST_AREA
{
	VAT_TESTPP1 = 0,
	VAT_TESTPP2,
	VAT_TESTPP_MAX
};


enum _eVAT_Z_TEACHING_BASE_INFO
{
	VAT_START_POS = 0,
	VAT_PICK_POS,
//	VAT_CAM_SETTING_POS,

	VAT_MAX_Z_TEACHING_POS
};


// By jhRyu [180315] Vision Auto Teaching <Start>
typedef struct _St_VAT_Info
{
	_St_VAT_Info() {
		memset(this, 0x00, sizeof(_St_VAT_Info));
	}
} St_VAT_Info;

// By jhRyu [180823] 패키지별로 관리 되어야 하는 Vision Auto Teaching 구조체 추가.
typedef struct _St_VAT_PKG_Info
{
	_St_VAT_PKG_Info() {
		memset(this, 0, sizeof(_St_VAT_PKG_Info));
	}
} St_VAT_PKG_Info;


//================================================================================================================
//COMMENT : 해당 Mode Data 구조체
//================================================================================================================

typedef struct _St_VAT_ZFocusInfo
{
	double dPlateJIG_TargetPos[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ]; //[GUI Data] [측정 완료 Data Save]
	double dPlateJIG_VacuumPos[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ]; //[GUI Data] [측정 완료 Data Save]

	
	double dPlateJIG_LowerOffset[VAT_HAND_MAX][VAT_MAX_AXIS_XY]; //[측정 완료 Data Save]
	
	_St_VAT_ZFocusInfo() {
		memset(this, 0x00, sizeof(_St_VAT_ZFocusInfo));
	}
}St_VAT_ZFocusInfo;


typedef struct  _St_VAT_CamCenterCalInfo
{
	double dBotCameraPosition[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ];
	double dTopCamToPkrDistance[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ];

	_St_VAT_CamCenterCalInfo() {
		memset(this, 0x00, sizeof(_St_VAT_CamCenterCalInfo));
	}
}St_VAT_CamCenterCalInfo;


typedef struct  _St_VAT_PickerAlignInfo
{
// 	int nSelectedPicker[VAT_HAND_MAX][VAT_MAX_HAND_CNT_Y][VAT_MAX_HAND_CNT_X];
// 	
// 	//X, Y Data 저장.
// 	std::pair<double, double> pairPickerAlignWide[VAT_HAND_MAX][VAT_MAX_HAND_CNT_Y][VAT_MAX_HAND_CNT_X];
// 	std::pair<double, double> pairPickerAlignNarrow[VAT_HAND_MAX][VAT_MAX_HAND_CNT_Y][VAT_MAX_HAND_CNT_X];
	int nSelectedPicker[VAT_HAND_MAX][VAT_PICKER_MAX];

	//현재 얘만 사용 중.
	std::pair<double, double> pEachPickerOffset[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX][VAT_PICKER_MAX];
	std::pair<double, double> pEachPickerPos[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX][VAT_PICKER_MAX];

	std::pair<double, double> dPkrWideNarrowAvgOffset[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX];

	double dPickerZFocusPos[VAT_HAND_MAX][VAT_PICKER_MAX]; //(.ini 저장)

	std::vector<double> vMeasuredPickerPos[VAT_HAND_MAX]; //Picker Align 측정 시, 사용 되는 변수라서 .ini에 저장 할 필요 없음.

	_St_VAT_PickerAlignInfo() {
		memset(this, 0x00, sizeof(_St_VAT_PickerAlignInfo));
	}
}St_VAT_PickerAlignInfo;


typedef struct _St_VAT_PickerAlignSubInfo
{
	//Picker Align Setting Tab Data
	double dPreSettingPos[VAT_HAND_MAX][VAT_MAX_AXIS_XY];

	_St_VAT_PickerAlignSubInfo() {
		memset(this, 0x00, sizeof(_St_VAT_PickerAlignSubInfo));
	}
}St_VAT_PickerAlignSubInfo;


//Z Auto Focus    - Plate Jig 상단 Vacuum Pos
//Z Auto Teaching - Part 상단 Vacuum Pos
typedef struct  _ST_PICKER_VACUUM_POS
{
	double dPickerVacPos_LD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_PICKER_MAX];
	double dPickerVacPos_UD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_PICKER_MAX];
	double dPickerVacPos_LD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
	double dPickerVacPos_UD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
	double dPickerVacPos_LD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
	double dPickerVacPos_UD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];

	void setPickerVacPos(int nHand, int nPartIdx, int nPickerIdx, double dPos) {
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:		dPickerVacPos_LD_TrayPp[nPartIdx][nPickerIdx] = dPos;	break;
		case VAT_HAND_UNLOAD_TRAYPP:	dPickerVacPos_UD_TrayPp[nPartIdx][nPickerIdx] = dPos;	break;
		case VAT_HAND_LOAD_TESTPP1:		dPickerVacPos_LD_TestPp1[nPartIdx][nPickerIdx] = dPos;	break;
		case VAT_HAND_UNLOAD_TESTPP1:	dPickerVacPos_UD_TestPp1[nPartIdx][nPickerIdx] = dPos;	break;
		case VAT_HAND_LOAD_TESTPP2:		dPickerVacPos_LD_TestPp2[nPartIdx][nPickerIdx] = dPos;	break;
		case VAT_HAND_UNLOAD_TESTPP2:	dPickerVacPos_UD_TestPp2[nPartIdx][nPickerIdx] = dPos;	break;
		default:
			break;
		}
	}

	double getPickerVacPos(int nHand, int nPartIdx, int nPickerIdx) {
		double dRetPos = 0.0;
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:		dRetPos = dPickerVacPos_LD_TrayPp[nPartIdx][nPickerIdx];	break;
		case VAT_HAND_UNLOAD_TRAYPP:	dRetPos = dPickerVacPos_UD_TrayPp[nPartIdx][nPickerIdx];	break;
		case VAT_HAND_LOAD_TESTPP1:		dRetPos = dPickerVacPos_LD_TestPp1[nPartIdx][nPickerIdx];	break;
		case VAT_HAND_UNLOAD_TESTPP1:	dRetPos = dPickerVacPos_UD_TestPp1[nPartIdx][nPickerIdx];	break;
		case VAT_HAND_LOAD_TESTPP2:		dRetPos = dPickerVacPos_LD_TestPp2[nPartIdx][nPickerIdx];	break;
		case VAT_HAND_UNLOAD_TESTPP2:	dRetPos = dPickerVacPos_UD_TestPp2[nPartIdx][nPickerIdx];	break;
		default:
			break;
		}

		return dRetPos;
	}

	_ST_PICKER_VACUUM_POS() {
		memset(this, 0x00, sizeof(_ST_PICKER_VACUUM_POS));
	}
}ST_PICKER_VACUUM_POS;


//Z Auto Teaching - Picker Gap
 typedef struct _ST_PICKER_GAP
 {
 	//Picker간의 높이 Gap
 	double dPickerGap_LD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_PICKER_MAX];
 	double dPickerGap_UD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_PICKER_MAX];
 	double dPickerGap_LD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
 	double dPickerGap_UD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
 	double dPickerGap_LD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
 	double dPickerGap_UD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_PICKER_MAX];
 
 	void setPickerGap(int nHand, int nPartIdx, int nPickerIdx, double dGap) {
 		switch (nHand)
 		{
 		case VAT_HAND_LOAD_TRAYPP:		dPickerGap_LD_TrayPp[nPartIdx][nPickerIdx] = dGap;	break;
 		case VAT_HAND_UNLOAD_TRAYPP:	dPickerGap_UD_TrayPp[nPartIdx][nPickerIdx] = dGap;	break;
 		case VAT_HAND_LOAD_TESTPP1:		dPickerGap_LD_TestPp1[nPartIdx][nPickerIdx] = dGap;	break;
 		case VAT_HAND_UNLOAD_TESTPP1:	dPickerGap_UD_TestPp1[nPartIdx][nPickerIdx] = dGap;	break;
 		case VAT_HAND_LOAD_TESTPP2:		dPickerGap_LD_TestPp2[nPartIdx][nPickerIdx] = dGap;	break;
 		case VAT_HAND_UNLOAD_TESTPP2:	dPickerGap_UD_TestPp2[nPartIdx][nPickerIdx] = dGap;	break;
 		default:
 			break;
 		}
 	}
 
 	double getPickerGap(int nHand, int nPartIdx, int nPickerIdx) {
 		double dRetGap = 0.0;
 		switch (nHand)
 		{
 		case VAT_HAND_LOAD_TRAYPP:		dRetGap = dPickerGap_LD_TrayPp[nPartIdx][nPickerIdx];	break;
 		case VAT_HAND_UNLOAD_TRAYPP:	dRetGap = dPickerGap_UD_TrayPp[nPartIdx][nPickerIdx];	break;
 		case VAT_HAND_LOAD_TESTPP1:		dRetGap = dPickerGap_LD_TestPp1[nPartIdx][nPickerIdx];	break;
 		case VAT_HAND_UNLOAD_TESTPP1:	dRetGap = dPickerGap_UD_TestPp1[nPartIdx][nPickerIdx];	break;
 		case VAT_HAND_LOAD_TESTPP2:		dRetGap = dPickerGap_LD_TestPp2[nPartIdx][nPickerIdx];	break;
 		case VAT_HAND_UNLOAD_TESTPP2:	dRetGap = dPickerGap_UD_TestPp2[nPartIdx][nPickerIdx];	break;
 		default:
 			break;
 		}
 		return dRetGap;
 	}
 
 	_ST_PICKER_GAP() {
 		memset(this, 0x00, sizeof(_ST_PICKER_GAP));
 	}
 }ST_PICKER_GAP;


 typedef struct _St_VAT_ZTeachBasePos
 {
	 //Z Teach Base Position
	 //Hand당 part 관리.
	 double dZTeachBasePos_LD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	 double dZTeachBasePos_UD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	 double dZTeachBasePos_LD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	 double dZTeachBasePos_UD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	 double dZTeachBasePos_LD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	 double dZTeachBasePos_UD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];


	 void setPartBasePos(int nHand, int nPart, int nAxis, double dPos) {
		 switch (nHand)
		 {
		 case VAT_HAND_LOAD_TRAYPP:		dZTeachBasePos_LD_TrayPp[nPart][nAxis] = dPos;	break;
		 case VAT_HAND_UNLOAD_TRAYPP:	dZTeachBasePos_UD_TrayPp[nPart][nAxis] = dPos;	break;
		 case VAT_HAND_LOAD_TESTPP1:	dZTeachBasePos_LD_TestPp1[nPart][nAxis] = dPos;	break;
		 case VAT_HAND_UNLOAD_TESTPP1:	dZTeachBasePos_UD_TestPp1[nPart][nAxis] = dPos;	break;
		 case VAT_HAND_LOAD_TESTPP2:	dZTeachBasePos_LD_TestPp2[nPart][nAxis] = dPos;	break;
		 case VAT_HAND_UNLOAD_TESTPP2:	dZTeachBasePos_UD_TestPp2[nPart][nAxis] = dPos;	break;
		 default:
			 break;
		 }
	 }

	 double getPartBasePos(int nHand, int nPart, int nAxis) {
		 double dTargetPos = 0.0;

		 switch (nHand)
		 {
		 case VAT_HAND_LOAD_TRAYPP:		dTargetPos = dZTeachBasePos_LD_TrayPp[nPart][nAxis]; 	break;
		 case VAT_HAND_UNLOAD_TRAYPP:	dTargetPos = dZTeachBasePos_UD_TrayPp[nPart][nAxis]; 	break;
		 case VAT_HAND_LOAD_TESTPP1:	dTargetPos = dZTeachBasePos_LD_TestPp1[nPart][nAxis];	break;
		 case VAT_HAND_UNLOAD_TESTPP1:	dTargetPos = dZTeachBasePos_UD_TestPp1[nPart][nAxis];	break;
		 case VAT_HAND_LOAD_TESTPP2:	dTargetPos = dZTeachBasePos_LD_TestPp2[nPart][nAxis];	break;
		 case VAT_HAND_UNLOAD_TESTPP2:	dTargetPos = dZTeachBasePos_UD_TestPp2[nPart][nAxis];	break;
		 default:
			 break;
		 }

		 return dTargetPos;
	 }


	 _St_VAT_ZTeachBasePos() {
		 memset(this, 0x00, sizeof(_St_VAT_ZTeachBasePos));
	 }
 }St_VAT_ZTeachBasePos;


 typedef struct _St_Z_PICK_PLACE_OFFSET
 {
	 double dZTeachOffset_LD_TrayPp[VAT_Z_OFFSET_TYPE_MAX][VAT_LD_TRAY_PP_UNIT_MAX];
	 double dZTeachOffset_UD_TrayPp[VAT_Z_OFFSET_TYPE_MAX][VAT_UD_TRAY_PP_UNIT_MAX];
	 double dZTeachOffset_LD_TestPp1[VAT_Z_OFFSET_TYPE_MAX][VAT_LD_TEST_PP_UNIT_MAX];
	 double dZTeachOffset_UD_TestPp1[VAT_Z_OFFSET_TYPE_MAX][VAT_UD_TEST_PP_UNIT_MAX];
	 double dZTeachOffset_LD_TestPp2[VAT_Z_OFFSET_TYPE_MAX][VAT_LD_TEST_PP_UNIT_MAX];
	 double dZTeachOffset_UD_TestPp2[VAT_Z_OFFSET_TYPE_MAX][VAT_UD_TEST_PP_UNIT_MAX];

	 void setUnitZOffset(int nHand, int nType, int nUnit, double dPos) {
		 switch (nHand)
		 {
		 case VAT_HAND_LOAD_TRAYPP:		dZTeachOffset_LD_TrayPp[nType][nUnit] = dPos;	break;
		 case VAT_HAND_UNLOAD_TRAYPP:	dZTeachOffset_UD_TrayPp[nType][nUnit] = dPos;	break;
		 case VAT_HAND_LOAD_TESTPP1:	dZTeachOffset_LD_TestPp1[nType][nUnit] = dPos;	break;
		 case VAT_HAND_UNLOAD_TESTPP1:	dZTeachOffset_UD_TestPp1[nType][nUnit] = dPos;	break;
		 case VAT_HAND_LOAD_TESTPP2:	dZTeachOffset_LD_TestPp2[nType][nUnit] = dPos;	break;
		 case VAT_HAND_UNLOAD_TESTPP2:	dZTeachOffset_UD_TestPp2[nType][nUnit] = dPos;	break;
		 default:
			 break;
		 }
	 }

	 double getUnitZOffset(int nHand, int nType, int nUnit) {
		 double dOffset = 0.0;

		 switch (nHand)
		 {
		 case VAT_HAND_LOAD_TRAYPP:		dOffset = dZTeachOffset_LD_TrayPp[nType][nUnit]; 	break;
		 case VAT_HAND_UNLOAD_TRAYPP:	dOffset = dZTeachOffset_UD_TrayPp[nType][nUnit]; 	break;
		 case VAT_HAND_LOAD_TESTPP1:	dOffset = dZTeachOffset_LD_TestPp1[nType][nUnit];	break;
		 case VAT_HAND_UNLOAD_TESTPP1:	dOffset = dZTeachOffset_UD_TestPp1[nType][nUnit];	break;
		 case VAT_HAND_LOAD_TESTPP2:	dOffset = dZTeachOffset_LD_TestPp2[nType][nUnit];	break;
		 case VAT_HAND_UNLOAD_TESTPP2:	dOffset = dZTeachOffset_UD_TestPp2[nType][nUnit];	break;
		 default:
			 break;
		 }

		 return dOffset;
	 }

	 int getUnitType(int nHand, int nPart) {
		 int nType = -1;
		 switch (nHand)
		 {
			case VAT_HAND_LOAD_TRAYPP:
			{
				if (VAT_IDX_TRAY_PP_TBL_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_TBL_4)	nType = VAT_LD_TRAY_PP_UNIT_TABLE;
				else																	nType = VAT_LD_TRAY_PP_UNIT_FEEDER;
			}break;
			case VAT_HAND_UNLOAD_TRAYPP:
			{
				if (VAT_IDX_TRAY_PP_TBL_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_TBL_4)	nType = VAT_UD_TRAY_PP_UNIT_TABLE;
				else																	nType = VAT_UD_TRAY_PP_UNIT_C_TRAY;
			}break;
			case VAT_HAND_LOAD_TESTPP1:
			case VAT_HAND_LOAD_TESTPP2:
			{
				if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4)							nType = VAT_LD_TEST_PP_UNIT_STATION;
				else if (VAT_IDX_TEST_PP_TBL_1_LD <= nPart && nPart <= VAT_IDX_TEST_PP_TBL_4_UD)		nType = VAT_LD_TEST_PP_UNIT_TABLE;
				else																					nType = VAT_LD_TEST_PP_UNIT_CLEAN_TABLE;
			}break;
			case VAT_HAND_UNLOAD_TESTPP1:
			case VAT_HAND_UNLOAD_TESTPP2:
			{
				if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4)							nType = VAT_UD_TEST_PP_UNIT_STATION;
				else if (VAT_IDX_TEST_PP_TBL_1_LD <= nPart && nPart <= VAT_IDX_TEST_PP_TBL_4_UD)		nType = VAT_UD_TEST_PP_UNIT_TABLE;
				else																					nType = VAT_UD_TEST_PP_UNIT_MAX;
			}break;

			 default:
				break;
		 }

		 return nType;
	 }

	 _St_Z_PICK_PLACE_OFFSET() {
		 memset(this, 0x00, sizeof(_St_Z_PICK_PLACE_OFFSET));
	 }
 }St_Z_PICK_PLACE_OFFSET;

 //=============================================================================================Z AUTO TEACHING
typedef struct _St_VAT_ZAutoTeachInfo
{
	//Pick&Place Offset
//	double dZPickOffset[VAT_HAND_MAX];
//	double dZPlaceOffset[VAT_HAND_MAX];
	St_Z_PICK_PLACE_OFFSET stUnitZOffset;

	//Picker간의 높이 Gap
	ST_PICKER_GAP stPickerGap;

	//Picker 실제 Vacuum Position
	ST_PICKER_VACUUM_POS stPickerVacPos;

	//Z Teach Base Position
	St_VAT_ZTeachBasePos stZTeachBasePos;


	_St_VAT_ZAutoTeachInfo() {
		memset(this, 0x00, sizeof(_St_VAT_ZAutoTeachInfo));
	}

} St_VAT_ZAutoTeachInfo;


 typedef struct _St_VAT_HandBasePos
 {
	//Part Base Position
	//Hand당 part 관리.
	double dHandBasePos_LD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	double dHandBasePos_UD_TrayPp[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	double dHandBasePos_LD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	double dHandBasePos_UD_TestPp1[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	double dHandBasePos_LD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];
	double dHandBasePos_UD_TestPp2[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ];


	void setBasePos(int nHand, int nPart, int nAxis, double dPos) {
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:		dHandBasePos_LD_TrayPp[nPart][nAxis] = dPos;	break;
		case VAT_HAND_UNLOAD_TRAYPP:	dHandBasePos_UD_TrayPp[nPart][nAxis] = dPos;	break;
		case VAT_HAND_LOAD_TESTPP1:		dHandBasePos_LD_TestPp1[nPart][nAxis] = dPos;	break;
		case VAT_HAND_UNLOAD_TESTPP1:	dHandBasePos_UD_TestPp1[nPart][nAxis] = dPos;	break;
		case VAT_HAND_LOAD_TESTPP2:		dHandBasePos_LD_TestPp2[nPart][nAxis] = dPos;	break;
		case VAT_HAND_UNLOAD_TESTPP2:	dHandBasePos_UD_TestPp2[nPart][nAxis] = dPos;	break;
		default:
			break;
		}
	}
	
	double getBasePos(int nHand, int nPart, int nAxis) {
		double dTargetPos = 0.0;	

		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:		dTargetPos = dHandBasePos_LD_TrayPp[nPart][nAxis]; 	break;
		case VAT_HAND_UNLOAD_TRAYPP:	dTargetPos = dHandBasePos_UD_TrayPp[nPart][nAxis]; 	break;
		case VAT_HAND_LOAD_TESTPP1:		dTargetPos = dHandBasePos_LD_TestPp1[nPart][nAxis];	break;
		case VAT_HAND_UNLOAD_TESTPP1:	dTargetPos = dHandBasePos_UD_TestPp1[nPart][nAxis];	break;
		case VAT_HAND_LOAD_TESTPP2:		dTargetPos = dHandBasePos_LD_TestPp2[nPart][nAxis];	break;
		case VAT_HAND_UNLOAD_TESTPP2:	dTargetPos = dHandBasePos_UD_TestPp2[nPart][nAxis];	break;
		default:
			break;
		}

		return dTargetPos;
	}

	_St_VAT_HandBasePos() {
 		memset(this, 0x00, sizeof(_St_VAT_HandBasePos));
 	}

 }St_VAT_HandBasePos;


 typedef struct _St_VAT_PartBasePos
 {
	double dPartBasePos_Feeder[VAT_MAX_FEEDER_LOC_COUNT][VAT_MAX_AXIS_Y];			//Y축
	double dPartBasePos_Table1[VAT_MAX_TBL_LOC_COUNT][VAT_MAX_AXIS_YROTATE];		//Y축 & Rotate축
	double dPartBasePos_Table2[VAT_MAX_TBL_LOC_COUNT][VAT_MAX_AXIS_YROTATE];
	double dPartBasePos_Table3[VAT_MAX_TBL_LOC_COUNT][VAT_MAX_AXIS_YROTATE];
	double dPartBasePos_Table4[VAT_MAX_TBL_LOC_COUNT][VAT_MAX_AXIS_YROTATE];

	void setBasePos(int nVATPart, int nPartPos, int nAxis, double dPos) {
	 switch (nVATPart)
	 {
	 case VAT_PART_FEEDER:		dPartBasePos_Feeder[nPartPos][nAxis] = dPos;	break;
	 case VAT_PART_TABLE_1:		dPartBasePos_Table1[nPartPos][nAxis] = dPos;	break;
	 case VAT_PART_TABLE_2:		dPartBasePos_Table2[nPartPos][nAxis] = dPos;	break;
	 case VAT_PART_TABLE_3:		dPartBasePos_Table3[nPartPos][nAxis] = dPos;	break;
	 case VAT_PART_TABLE_4:		dPartBasePos_Table4[nPartPos][nAxis] = dPos;	break;
	 default:
		 break;
	 }
	}

	double getBasePos(int nVATPart, int nPartPos, int nAxis) {
	 double dTargetPos = 0.0;
	 switch (nVATPart)
	 {
	 case VAT_PART_FEEDER:		dTargetPos = dPartBasePos_Feeder[nPartPos][nAxis]; 	break;
	 case VAT_PART_TABLE_1:		dTargetPos = dPartBasePos_Table1[nPartPos][nAxis]; 	break;
	 case VAT_PART_TABLE_2:		dTargetPos = dPartBasePos_Table2[nPartPos][nAxis];	break;
	 case VAT_PART_TABLE_3:		dTargetPos = dPartBasePos_Table3[nPartPos][nAxis];	break;
	 case VAT_PART_TABLE_4:		dTargetPos = dPartBasePos_Table4[nPartPos][nAxis];	break;
	 default:
		 break;
	 }

	 return dTargetPos;
	}
	_St_VAT_PartBasePos() {
	 memset(this, 0x00, sizeof(_St_VAT_PartBasePos));
	}
 }St_VAT_PartBasePos;



 //==============================================================================================================VISION AUTO TEACHING
 typedef struct  _St_VAT_VisionAutoTeachingInfo
 {
	 St_VAT_HandBasePos stHandBasePos;
	 St_VAT_PartBasePos stPartBasePos;

	 _St_VAT_VisionAutoTeachingInfo() {
		 memset(this, 0x00, sizeof(_St_VAT_VisionAutoTeachingInfo));
	 }
 }St_VAT_VisionAutoTeachingInfo;


 //======================================================================Design Value.

 typedef struct _St_VAT_DesignValueConstInfo
 {
	 //1. Z Auto Focus
	 //2. Cam Center Calibration
	 //3. Picker Align


	 //4. Vision Auto Teaching
	 //5. Z Auto Teaching

	 double dZAutoTeaching[VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING];
	





	 _St_VAT_DesignValueConstInfo() {
		 memset(this, 0x00, sizeof(_St_VAT_DesignValueConstInfo));
	 }

 }St_VAT_DesignValueConstInfo;

 //======================================================================VAT Option.
 
 typedef struct _St_VAT_OPTION
 {
	 //Picker Align
	 int nPickerAutoFocus;

	 _St_VAT_OPTION() {
		 nPickerAutoFocus = FALSE;
		 //memset(this, 0x00, sizeof(_St_VAT_OPTION));
	 }
 }St_VAT_OPTION;


// typedef struct _St_VAT_PickerInfo
// {
// 	std::pair<double, double> pairPickerAlignWide[VAT_AREA_MAX][MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X]; // Wide Pitch 상태에서 Picker 틀어짐.
// 	std::pair<double, double> pairPickerAlignNarrow[VAT_AREA_MAX][MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X]; // Narrow Pitch 상태에서 picker 틀어짐.
// 
// 	double dMeasureScaleFactor[VAT_MAX_AXIS_XY]; // X,Y Pitch 모터에 대한 Scale Factor 측정치.
// 	double dAvgPickerData[eMaxTestPPCount][VAT_MAX_AXIS_XY];
// 	
// 	_St_VAT_PickerInfo() {
// 		memset(this, 0x00, sizeof(_St_VAT_PickerInfo));
// 	}
// } St_VAT_PickerInfo;

// typedef struct _St_VAT_Option
// {
// 	// Teaching Option.
// 	int nTeachingMode; // Init or PM.
// 	int nTeachingPoint; // 2 Point or 4 Point.											//TWSL301N 현재 사용 안함.
// 	int nTeachingHandSpeed; // 50% or Handler Setting Speed.							//TWSL301N 현재 사용 안함.
// 	int nVisionOKRangeType; // Vision Offset 허용 범위. Scale Factor or User Value.
// 	double dVisionUserOKRange;
// 	int nAutoTrayLoad; // Loader, Auto Tray 자동적재.									//TWSL301N 현재 사용 안함.
// 	int nUsePickerDown; // Picker Down 사용 유무.										//TWSL301N 현재 사용 안함.
// 	int nAlwaysPickerDown; // Picker 상시 Down.											//TWSL301N 현재 사용 안함.
// 	int nRepeatMode; // VAT Repeat Mode
// 	int nVisionSettingMode; // VAT Vision Setting Mode									//TWSL301N 현재 사용 안함.
// 	int nStdPicker[VAT_HAND_MAX][VAT_MAX_AXIS_XY]; // 기준피커 정보 ( [0] : X, [1] : Y )
// 	double dZDownOffset; // Vacuum 감지된 높이로부터 몇 mm 더 내려서 Pick 할 것인지.		
// 	double dZPlaceOffset;
// 	double dVacuumWaitTime; // Vacuum 동작 시간.											//TWSL301N 현재 사용 안함.
// 	double dVisionWaitTime; // Vision Measure 전 Wait Time.
// 
// 	int nPitchScaleFactorCalOption;														//TWSL301N 현재 사용 안함.
// 	int nSelectedPicker[VAT_HAND_MAX][MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];
// 
// 	// Running Option.
// 	int nApplyGradientOffset; // 3 Point Offset (기울기) 적용 여부.						//TWSL301N 현재 사용 안함.
// 	int nApplyScaleFactorOffset;														//TWSL301N 현재 사용 안함.
// 	int nApplyPickerAlignOffset; // Picker Align 측정 결과 보정 여부.						//TWSL301N 현재 사용 안함.				
// 	int nApplyZ3DOffset; // Z축 Pick&Place 입체 보정 적용 여부.							//TWSL301N 현재 사용 안함.
// 	int nPlaceMode;																		//TWSL301N 현재 사용 안함.
// 
// 	_St_VAT_Option() {
// 		memset(this, 0x00, sizeof(_St_VAT_Option));
// 	}
// } St_VAT_Option;

typedef struct _stVAT_Hand_Base_Sheet_Info
{
	_stVAT_Hand_Base_Sheet_Info() { clear(); };
	void clear() { ZeroMemory(this, sizeof(_stVAT_Hand_Base_Sheet_Info)); };

	int nSheetType;
	int nRow;
	int nCol;

} stVAT_Hand_Base_Sheet_Info;



class CVAT_CommonData
{
public:
	CVAT_CommonData();
	~CVAT_CommonData();

//측정 후, 저장이 필요한 변수
	CString m_strFilePath;

// 	St_VAT_Info						m_stVATInfo;
// 	St_VAT_Info						m_stVATInfoBuff;
	St_VAT_PKG_Info					m_stVATPKGInfo;
	St_VAT_PKG_Info					m_stVATPKGInfoBuff;

// 	St_VAT_PickerInfo				m_stVATPickerInfo;
// 	St_VAT_PickerInfo				m_stVATPickerInfoBuff;
//	St_VAT_Option					m_stVATOption;

	
	///////////////신규
	//================================================================================
	
	/*<VAT DATA TYPE 설명(2가지 Type)>
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	1) 측정 완료 후 이전 Data와의 차이를 확인 할 수 있는 Type(Compare Type)
	-. 해당 Data는 Real/Buffer 2개의 구조체로 운영 된다.
	-. 주로 vision 검사로 Offset 보정이 이루어 지는 Data들로 구성 됨.(ex. X Pos, Y Pos)

	-. 사용자 값 변경에 따른 이전 Data 비교가 필요 할때.
	-. Mode 측정 후, 이전 측정 값과 비교가 필요 할때,

	2) 이전 Data와의 차이 비교가 필요 없는 Type(Non-Compare Type)
	-. 해당 Data는 Real/Buffer가 아닌 Const 1개의 구조체로 운영 된다.
	-. 한번 설정 이후, 변경 되지 않는 Data.
	-. Mode 측정 중, Vision Offset 보정이 이루어 지지 않으며, 마지막 Data Save만 이루어 질때.
	-. 작업자가 GUI에서 변경 할 경우, VAT 측정에서 고정 값으로 바로 적용. 또는 Auto Running때 해당 Data가 바로 적용 됨.

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/
	ST_VAT_FILE_SECTION				m_stVATFileSection;
	

	St_VAT_ZFocusInfo				m_stVATZFocusInfo;
	St_VAT_ZFocusInfo				m_stVATZFocusInfoBuff;
	St_VAT_ZFocusInfo				m_stVATZFocusInfoSeq;

	St_VAT_CamCenterCalInfo			m_stVATCamCenterCalInfo;
	St_VAT_CamCenterCalInfo			m_stVATCamCenterCalInfoBuff;
	St_VAT_CamCenterCalInfo			m_stVATCamCenterCalInfoSeq;

	St_VAT_PickerAlignInfo			m_stVATPickerAlignInfo;
	St_VAT_PickerAlignInfo			m_stVATPickerAlignInfoSeq;

	St_VAT_PickerAlignSubInfo		m_stVAT_PickerAlignSubInfo;

	St_VAT_ZAutoTeachInfo			m_stVATZTeachInfo;
	St_VAT_ZAutoTeachInfo			m_stVATZTeachInfoSeq;

	St_VAT_VisionAutoTeachingInfo   m_stVATInfo;
	St_VAT_VisionAutoTeachingInfo   m_stVATInfoBuff;
	St_VAT_VisionAutoTeachingInfo	m_stVATInfoSeq;

// 	St_VAT_PartBasePos				m_stVATPartBasePos;
// 	St_VAT_PartBasePos				m_stVATPartBasePosBuff;

	St_VAT_DesignValueConstInfo		m_stVATDesignValueConstInfo;

	St_VAT_OPTION					m_stVATOption;
public:
	//=======================================================================================Auto & Cmd 동시 사용 변수 및 함수 관리
	//->측정 후, 저장이 필요 없는 변수

	//[Z Auto Focus]

	//[Cam Center Calibration]

	//[Picker Height Cal]

	//[Vision Auto Teaching]

	//[Z Auto Teaching]
	double first_move_offset_um[VAT_HAND_MAX];

	void SetFirstPickOffset(int nHand, double dPos);
	double GetFirstPickOffset(int nHand);
	void ClearFirstPickOffset(int nHand);
	//===============================================================================================================

public:
	void SetDataSaveComplete(BOOL bChk ) { data_save_complete = bChk; }
	BOOL GetDataSaveComplete() { return data_save_complete; }
	

private:
	CRITICAL_SECTION  m_cs;

	BOOL data_save_complete;

//측정 Data Save부
public:
	BOOL IsSelectUnitInState(int nHand, _eVAT_MeasureStatus nMeasureStatus);

	void MeasureDataSave();
	void MeasureDataSaveToFile(int nMode);

	void DataSaveZAutoFocus();
	void DataSaveCamCenterCalibration();
	void DataSavePickerAlign();	
	void DataSaveVisionAutoTeaching();
	void DataSaveZAutoTeaching();


	// Z Auto Focus
	double VAT_GetBaseToUnitHeight(int nHand, int nPart);
	double VAT_CalZFocusHeight(int nHand, int nPart, double dCamBasePos);

	// Picker Align
	//Wide/Narrow/Wide-Narrow Picker별 Offset 계산
	void VAT_CalWideNarrowOffset(int nHand);
	void VAT_CalXYPitchTeachingPos(int nHand);
	void VAT_CalWideNarrowAvgOffset(int nHand);

	BOOL VAT_IsSpecOutOffsetExist(int nHand, int nPitchType, int nPitchAxis);
	std::pair<int, int> VAT_GetPitchOffsetDir(int nHand, int nPitchType);
	int VAT_GetPartPitchType(int nHand, int nPart);



	//Z Auto Teaching
	//void Make_AutoTeaching_ZPos(int nHand);
	void VAT_Load_TraySiteCal_ZPos();
	void VAT_Unload_TraySiteCal_ZPos();
	void VAT_TestSite_Cal_ZPos(int nHand);

public:
	//최신/////////////////////////////////////////////
	
	//Data INI 파일 Load 및 Save
	void LoadVATData();
	void SaveVATData();


	//Data Load & Save
	CString GetFilePathCom(int nMode); //Data 공통 File에 저장.

	CString GetFilePathHand(int nMode, int nHand); //Data Hand별로 File 저장.
	CString GetHandString(int nHand);

	CString GetFilePathPart(int nVATPart);
	CString GetPartString(int nVATPart);

	void IsExistVATDirectory();


	void LoadZAutoFocusInformation();
	void LoadCamCenterCalInformation();
	void LoadPickerAlignInformation();
	void LoadZAutoTeachingInformation();
	void LoadVisionAutoTeachingInformation();
	//void LoadPartBasePosition(); //Part Base Position 따로 관리.
	void LoadDesignValue();
	void LoadVATOption();

	////////////////////////////////////////////////
	//모든 정보저장. (사용 하지 않을 경우 제거 필요)
	void SaveZAutoFocusInformation();
	void SaveCamCenterCalInformation();
	void SavePickerAlignInformation();
	void SaveZAutoTeachingInformation();
	void SaveVisionAutoTeachingInformation();
	
	void SaveVATOption(int nOptionType);
	//void SavePartBasePosition(); //Part Base Position 따로 관리.
	

	//개별 Target 정보만 저장.
	//작업자가 GUI Data Save시, 필요 정보만 저장 하기 위함. (bIsBuffSave == FALSE)
	//Auto Seq 측정 완료 후, 필요 정보만 저장 하기 위함.	 (bIsBuffSave == TRUE)
	void SaveEachZAutoFocusInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand, int nTargetAxis = -1);
	void SaveEachCamCenterCalInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand, int nTargetAxis = -1);
	void SaveEachPickerAlignInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand);
	void SaveEachZAutoTeachingInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand);
	void SaveEachVisionAutoTeachingInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nUnit, int nSection, int nTargetAxis = -1);
	//void SaveEachHandPartBasePosition(int nHand);
	void SaveEachDesignValue(_eVAT_MODE_EACH_DATA_TYPE _eType);

	////////////////////////////////////////////////////////////////////////




//Data 관리.
public:
	void DataCopyBufferToSource();
	void DataCopySourceToBuffer();

	void DataCopyRealToSequence(int nMode); //[Sequence시작 시] Real -> Sequence
	void DataCopyRealToBuffer(int nMode);	 //[측정 Data Save 전] Real -> Buffer 
	void DataCopySequenceToReal(int nMode);   //[측정 Data Save 시] Sequence -> Real


	//afx_msg LRESULT OnReceiveSeqFinished(WPARAM wParam, LPARAM lParam);
// 	char m_szVisionPC_IpAddress[126];
// 
// 	void LoadVATData();
// 	void LoadVATInformation();
// 	void LoadVATZTeachingInformation();
// 	void LoadVATPickerInformation();
// 	void LoadVATOption();
// 	void LoadVATPackageInfo();
// 
// 	void SaveVATData();
// 	void SaveVATInformation();
// 	void SaveVATZTeachingInformation();
// 	void SaveVATPickerInformation();
// 	void SaveVATOption();
// 	void SaveVATPackageInfo();
// 	void SaveVATTestPPOffset();


//	void Make_AutoTeaching(BOOL TR_FL, int nPart = 0); //TR_FL : TRUE->전체, FALSE -> 부분, nPart은 Axis별
//	void VAT_AutoTeachingCal(BOOL TR_FL = TEACHING_ALL_PART, int nPart = 0); // TR_FL : TRUE -> 전체 파트, FALSE -> 개별 파트
//	void VAT_TraySiteCal(int nTrayPart);
//	void VAT_TestSiteCal(int nTestSitePart);
//	void VAT_ShuttleCal_TrayPP(int nTablePart);
// 	void VAT_ShuttleCal_TestPP(int nTablePart);
// 	void VAT_CleanTableCal(int nCleanTablePart);
// 	void VAT_BufferTableCal(int nBufferTablePart);
	
//	void Make_AutoTeaching_ZPos(BOOL TR_FL, int Rocation = 0); //TR_FL : TRUE->전체, FALSE -> 부분, rocation은 Axis별
//	void VAT_AutoTeachingCal_ZPos(BOOL TR_FL = TEACHING_ALL_PART, int nPart = 0); // TR_FL : TRUE -> 전체 파트, FALSE -> 개별 파트
//	void VAT_TraySiteCal_ZPos(int nTrayPart);
//	void VAT_TestSiteCal_ZPos(int nTestSitePart);
//	void VAT_ShuttleCal_TrayPP_ZPos(int nTablePart);

//	void VAT_ShuttleCal_TestPP_ZPos(int nTablePart);
//	void VAT_CleanTableCal_ZPos(int nCleanTablePart);
//	void VAT_BufferTableCal_ZPos(int nBufferTablePart);

//	void VAT_Table_TestPP_ZPos(int nTalbePart);
//	void VAT_TraySiteCal_XYPitch();
//	void VAT_TestSiteCal_XYPitch(BOOL bTestPPSelectPart[]);

//	int VAT_CompareDir(std::vector<std::pair<double, double>>vPickerData, int nXYDir);
//	void VAT_CalAvgOffset(std::vector<std::pair<double, double>>vPickerWideData, std::vector<std::pair<double, double>>vPickerNarrowData, int nDirResult, int nXYDir);

//	std::pair<double, double> GetVATPickerAlignOffset(_eVAT_AREA eArea, int(*pUsePickerArray)[MAX_TRAY_PP_CNT_X], double dPitchX, double dPitchY);
//	std::pair<double, double> GetVATPickerAlginOffset_Each(_eVAT_AREA eArea, int nHandX, int nHandY, double dPitchX, double dPitchY);

// 	void SetVATPackageInfo();
// 
// 	int SetVATPitchScaleFactor(int nPitchAxis);
// 	void ReloadMotionDB();
};

