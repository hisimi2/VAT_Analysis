#pragma once

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
typedef struct _ST_VAT_FILE_SECTION
{
	/////////////////////////////////////////////////////////////////////////
	//section
	/////////////////////////////////////////////////////////////////////////
	// [1] Z Auto Focus
	char *pZFocusSec_PlateJigTargetPos = { "PLATE JIG TARGET POS" };
	char *pZFocusSec_PlateJigVacPos = { "PLATE JIG VACUUM POS" };
	char *pZFocusSec_PlateJigLowerOffset = { "PLATE JIG LOWER TARGET OFFSET" };

	char *pZFocusSecBuff_PlateJigTargetPos = { "PLATE JIG TARGET POS<BUFF>" };
	char *pZFocusSecBuff_PlateJigVacPos = { "PLATE JIG VACUUM POS<BUFF>" };
	char *pZFocusSecBuff_PlateJigLowerOffset = { "PLATE JIG LOWER TARGET OFFSET<BUFF>" };

	// [2] Cam Center Calibration
	char *pCamCenCalSec_BotCamCenterPos = { "BOTTOM CAM CENTER POS" };
	char *pCamCenCalSec_TopCamToPkrDis = { "TOP CAM TO PICKER DISTANCE" };

	char *pCamCenCalSecBuff_BotCamCenterPos = { "BOTTOM CAM CENTER POS<BUFF>" };
	char *pCamCenCalSecBuff_TopCamToPkrDis = { "TOP CAM TO PICKER DISTANCE<BUFF>" };

	// [3] Picker Align
		//Buffer 저장할 필요 없음.
		//Base Data가 없으며, 작업자 임의로 Data변경 불가.
		// -> Sequence 작업시, Buffer 사용 유무 check.
	char *pPickerAlignSec_SelectedPkr = { "SELECTED PICKER" };

	char *pPickerAlignSec_PickerOffset[VAT_PITCH_TYPE_MAX] = {"WIDE OFFSET", "NARROW OFFSET"};
	char *pPickerAlignSec_PickerPos[VAT_PITCH_TYPE_MAX] = {"WIDE PICK POSITION", "NARROW PICK POSITION"};
	char *pPickerAlignSec_WideNarrowAvgOffset[VAT_PITCH_TYPE_MAX] = {"WIDE AVG OFFSET", "NARROW AVG OFFSET"};
	char *pPickerAlignSec_ZFocusPos = { "Picker Z Focus Position" };




	char *pPickerAlignSecSub_PrePos = { "PICKER SETTING PRE POSITION", };
	
	
	
	// [4] Vision Auto Teaching
	char *pVATSec_TrayPpBasePos[VAT_MAX_TRAY_PP_LOC_COUNT] = {
		"UD Table 1<BASE POS>",
		"LD Table 1<BASE POS>",
		"LD Table 2<BASE POS>",
		"UD Table 2<BASE POS>",

		"C-Tray 1<BASE POS>",
		"C-Tray 2<BASE POS>",
		"C-Tray 3<BASE POS>",
		"C-Tray 4<BASE POS>",
		"C-Tray 5<BASE POS>",
		"C-Tray 6<BASE POS>",
		"C-Tray 7<BASE POS>",

		"Tray Feeder<BASE POS>"
	};

	char *pVATSec_TestPpBasePos[VAT_MAX_TEST_PP_LOC_COUNT] = {
		"Station 1<BASE POS>",
		"Station 2<BASE POS>",
		"Station 3<BASE POS>",
		"Station 4<BASE POS>",

		"LD Table Pos 1<BASE POS>",
		"LD Table Pos 2<BASE POS>",
		"LD Table Pos 3<BASE POS>",
		"LD Table Pos 4<BASE POS>",

		"UD Table Pos 1<BASE POS>",
		"UD Table Pos 2<BASE POS>",
		"UD Table Pos 3<BASE POS>",
		"UD Table Pos 4<BASE POS>",

		"Clean Table<BASE POS>",
	};

	char *pVATSecBuff_TrayPpBasePos[VAT_MAX_TRAY_PP_LOC_COUNT] = {
		"UD Table 1 <BASE POS><BUFF>",
		"LD Table 1 <BASE POS><BUFF>",
		"LD Table 2 <BASE POS><BUFF>",
		"UD Table 2 <BASE POS><BUFF>",

		"C-Tray 1 <BASE POS><BUFF>",
		"C-Tray 2 <BASE POS><BUFF>",
		"C-Tray 3 <BASE POS><BUFF>",
		"C-Tray 4 <BASE POS><BUFF>",
		"C-Tray 5 <BASE POS><BUFF>",
		"C-Tray 6 <BASE POS><BUFF>",
		"C-Tray 7 <BASE POS><BUFF>",

		"Tray Feeder <BASE POS><BUFF>"
	};

	char *pVATSecBuff_TestPpBasePos[VAT_MAX_TEST_PP_LOC_COUNT] = {
		"Station 1 <BASE POS><BUFF>",
		"Station 2 <BASE POS><BUFF>",
		"Station 3 <BASE POS><BUFF>",
		"Station 4 <BASE POS><BUFF>",

		"LD Table Pos 1 <BASE POS><BUFF>",
		"LD Table Pos 2 <BASE POS><BUFF>",
		"LD Table Pos 3 <BASE POS><BUFF>",
		"LD Table Pos 4 <BASE POS><BUFF>",

		"UD Table Pos 1 <BASE POS><BUFF>",
		"UD Table Pos 2 <BASE POS><BUFF>",
		"UD Table Pos 3 <BASE POS><BUFF>",
		"UD Table Pos 4 <BASE POS><BUFF>",

		"Clean Table <BASE POS><BUFF>",
	};

	///////////////////////////////////////////////////////////////////////
	char *pVATSec_PartFeederBasePos[VAT_MAX_FEEDER_LOC_COUNT] = {
		"C Tray Jig <BASE POS>"
// 		"Load Pick Start <BASE POS>",
// 		"Pick <BASE POS>",
// 		"Place <BASE POS>",
	};

	char *pVATSec_PartTableBasePos[VAT_MAX_TBL_LOC_COUNT] = {
		"Load Side <BASE POS>",
		"Test Side 1 <BASE POS>",
		"Test Side 2 <BASE POS>",
		"Test Side 3 <BASE POS>",
		"Test Side 4 <BASE POS>",
	};

	char *pVATSecBuff_PartFeederBasePos[VAT_MAX_FEEDER_LOC_COUNT] = {
		"C Tray Jig <BASE POS><BUFF>"
// 		"Load Pick Start <BASE POS><BUFF>",
// 		"Pick <BASE POS><BUFF>",
// 		"Place <BASE POS><BUFF>",
	};

	char *pVATSecBuff_PartTableBasePos[VAT_MAX_TBL_LOC_COUNT] = {
		"Load Side <BASE POS><BUFF>",
		"Test Side 1 <BASE POS><BUFF>",
		"Test Side 2 <BASE POS><BUFF>",
		"Test Side 3 <BASE POS><BUFF>",
		"Test Side 4 <BASE POS><BUFF>",
	};




	// [5] Z Auto Teaching - (NON-Compare Type)
	char *pZAutoTeachingSec_SettingParam = { "Setting Param" };
	char *pZAutoTeachingSec_ZPickPlaceOffset[VAT_Z_OFFSET_TYPE_MAX] = { "Z PICK OFFSET", "Z PLACE OFFSET" };

	char *pZAutoTeachingSec_PickerGap_TrayPP[VAT_MAX_TRAY_PP_LOC_COUNT] = {
		"UD Table 1<PICKER GAP>",
		"LD Table 1<PICKER GAP>",
		"LD Table 2<PICKER GAP>",
		"UD Table 2<PICKER GAP>",

		"C-Tray 1<PICKER GAP>",
		"C-Tray 2<PICKER GAP>",
		"C-Tray 3<PICKER GAP>",
		"C-Tray 4<PICKER GAP>",
		"C-Tray 5<PICKER GAP>",
		"C-Tray 6<PICKER GAP>",
		"C-Tray 7<PICKER GAP>",

		"Tray Feeder<PICKER GAP>"
	};

	char *pZAutoTeachingSec_PickerGap_TesstPP[VAT_MAX_TEST_PP_LOC_COUNT] = {
		"Station 1<PICKER GAP>",
		"Station 2<PICKER GAP>",
		"Station 3<PICKER GAP>",
		"Station 4<PICKER GAP>",

		"LD Table Pos 1<PICKER GAP>",
		"LD Table Pos 2<PICKER GAP>",
		"LD Table Pos 3<PICKER GAP>",
		"LD Table Pos 4<PICKER GAP>",

		"UD Table Pos 1<PICKER GAP>",
		"UD Table Pos 2<PICKER GAP>",
		"UD Table Pos 3<PICKER GAP>",
		"UD Table Pos 4<PICKER GAP>",

		"Clean Table<PICKER GAP>",
	};


	char *pZAutoTeachingSec_PickerVacPos_TrayPP[VAT_MAX_TRAY_PP_LOC_COUNT] = { 
		"UD Table 1<PICKER VACUUM POS>",
		"LD Table 1<PICKER VACUUM POS>",
		"LD Table 2<PICKER VACUUM POS>",
		"UD Table 2<PICKER VACUUM POS>",

		"C-Tray 1<PICKER VACUUM POS>",
		"C-Tray 2<PICKER VACUUM POS>",
		"C-Tray 3<PICKER VACUUM POS>",
		"C-Tray 4<PICKER VACUUM POS>",
		"C-Tray 5<PICKER VACUUM POS>",
		"C-Tray 6<PICKER VACUUM POS>",
		"C-Tray 7<PICKER VACUUM POS>",

		"Tray Feeder<PICKER VACUUM POS>"
	};

	char *pZAutoTeachingSec_PickerVacPos_TestPP[VAT_MAX_TEST_PP_LOC_COUNT] = {
		"Station 1<PICKER VACUUM POS>",
		"Station 2<PICKER VACUUM POS>",
		"Station 3<PICKER VACUUM POS>",
		"Station 4<PICKER VACUUM POS>",

		"LD Table Pos 1<PICKER VACUUM POS>",
		"LD Table Pos 2<PICKER VACUUM POS>",
		"LD Table Pos 3<PICKER VACUUM POS>",
		"LD Table Pos 4<PICKER VACUUM POS>",

		"UD Table Pos 1<PICKER VACUUM POS>",
		"UD Table Pos 2<PICKER VACUUM POS>",
		"UD Table Pos 3<PICKER VACUUM POS>",
		"UD Table Pos 4<PICKER VACUUM POS>",

		"Clean Table<PICKER VACUUM POS>",
	};

	// [6] Common Static Data
	char* pComStaticSec_StdPicker[VAT_HAND_MAX] = {
		"Standard Picker <LOAD TRAY PP>",
		"Standard Picker <UNLOAD TRAY PP>",
		"Standard Picker <LOAD TEST PP#1>",
		"Standard Picker <UNLOAD TEST PP#1>",
		"Standard Picker <LOAD TEST PP#2>",
		"Standard Picker <UNLOAD TEST PP#2>",
	};

	char* pComStaticSec_Hand[VAT_HAND_MAX] = {
		"LOAD TRAY PP",
		"UNLOAD TRAY PP",
		"LOAD TEST PP#1",
		"UNLOAD TEST PP#1",
		"LOAD TEST PP#2",
		"UNLOAD TEST PP#2",
	};

	
	// [7] Design Value - PKG따라 수정 가능
	char* pDesignValue_ZAutoTeaching[VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING] = {
		"TABLE_HEIGHT",
		"TABLE_POCKET_Z",
		"CLEAN_TABLE_HEIGHT",
		"CLEAN_TABLE_POCKET_Z",
		"C_TRAY_HEIGHT",
		"C_TRAY_POCKET_Z",
		"STATION_HEIGHT",
		"STATION_SOCKET_Z",
		"C_TRAY_JIG_HEIGHT",
		"DEVICE_HEIGHT",
	};



	char* pOption_PkrAutoFocus = { "Picker Auto Focus"};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//key
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Common
	char *pKeyAxisXYZ[VAT_MAX_AXIS_XYZ] = {
		"AXIS_X(um)", "AXIS_Y(um)", "AXIS_Z(um)"
	};

	char *pKeyAxisXY[VAT_MAX_AXIS_XY] = {
		"AXIS_X(um)", "AXIS_Y(um)"
	};

	char *pKeyX = { "X(um)" };
	char *pKeyY = { "Y(um)" };
	char *pKeyZ = { "Z(um)" };

	//1. Z Auto Focus

	//2. Cam Center Calibration

	//3. Picker Align
	char *pKeyPicker[VAT_PICKER_MAX] = {
		"PICKER1", "PICKER2", "PICKER3", "PICKER4",
		"PICKER5", "PICKER6", "PICKER7", "PICKER8",
	};

	char *pKeyPickerX[VAT_PICKER_MAX] = {
		"PICKER1_X(um)", "PICKER2_X(um)", "PICKER3_X(um)", "PICKER4_X(um)",
		"PICKER5_X(um)", "PICKER6_X(um)", "PICKER7_X(um)", "PICKER8_X(um)"
	};

	char *pKeyPickerY[VAT_PICKER_MAX] = {
		"PICKER1_Y(um)", "PICKER2_Y(um)", "PICKER3_Y(um)", "PICKER4_Y(um)",
		"PICKER5_Y(um)", "PICKER6_Y(um)", "PICKER7_Y(um)", "PICKER8_Y(um)"
	};


	//4. Z Auto Teaching
 	char *pKeyHandPickerNum[VAT_PICKER_MAX] = {
 		"PICKER_1","PICKER_2","PICKER_3","PICKER_4",
 		"PICKER_5","PICKER_6","PICKER_7","PICKER_8"
 	};

	char *pKeyZOffset[VAT_HAND_MAX][VAT_Z_OFFSET_UNIT_MAX] = {
		{ "TABLE(um)", "FEEDER(um)", "no_use" },
		{ "TABLE(um)", "C-TRAY(um)", "no_use" },
		{ "TABLE(um)", "STATION(um)", "CLEAN TABLE(um)" },
		{ "TABLE(um)", "STATION(um)", "no_use"},
		{ "TABLE(um)", "STATION(um)", "CLEAN TABLE(um)" },
		{ "TABLE(um)", "STATION(um)", "no_use"},
	};


	//5. Vision Auto Teaching
	char *pKeyAxisYRotate[VAT_MAX_AXIS_XY] = {
		"AXIS_Y(um)", "AXIS_ROTATE(deg)"
	};





	//6. Common Static Data
	char* pKeyStdPicker[VAT_MAX_AXIS_XY] = {"X", "Y"};
	
	char* pKeyPlateJigHeight = "Plate_Jig_Height[um]";
	char* pKeyTopPlateThickness = "Top_Plate_Thickness[um]";
	char* pKeyBaseToPlateJigHeight = "Base_TO_PLATE_JIG_HEIGHT[um]";
	char* pKeyBaseToUnit[VAT_HAND_MAX][VAT_MAX_BASE_TO_UNIT] = {
		{ "BASE_TO_TABLE(um)", "BASE_TO_FEEDER(um)", "NONE"},								//LD Tray Pp
		{ "BASE_TO_TABLE(um)", "BASE_TO_STACKER(um)", "NONE"},								//UD Tray Pp
		{ "BASE_TO_TABLE(um)", "BASE_TO_STATION(um)", "BASE_TO_CLEAN_TABLE(um)" },		//LD Test PP#1
		{ "BASE_TO_TABLE(um)", "BASE_TO_STATION(um)", "NONE" },								//UD Test PP#1
		{ "BASE_TO_TABLE(um)", "BASE_TO_STATION(um)", "BASE_TO_CLEAN_TABLE(um)" },		//LD Test PP#2
		{ "BASE_TO_TABLE(um)", "BASE_TO_STATION(um)", "NONE" },								//UD Test PP#2
	};

	//7. Design Value
	char* pKeyValue = { "VALUE[um]" };

	//8. VAT Option
	char* pkeyOption = { "USE" };

	////////////////////////////////////////////////////////////////////////////////////////////////////


	BOOL _IsNecessaryPart(int nHand, int nPart)
	{
		BOOL bRet = TRUE;

		//불필요한 Part 표기
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:
		{
			if (nPart == VAT_IDX_TRAY_PP_TBL_1 || nPart == VAT_IDX_TRAY_PP_TBL_4 || (VAT_IDX_TRAY_PP_CONV_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_CONV_7)) {
				bRet = FALSE;
			}
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			if (nPart == VAT_IDX_TRAY_PP_TBL_2 || nPart == VAT_IDX_TRAY_PP_TBL_3 || (VAT_IDX_TRAY_PP_CONV_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_CONV_3) || nPart == VAT_IDX_TRAY_PP_FEEDER) {
				bRet = FALSE;
			}
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_LOAD_TESTPP2:
		{
			//모든 Part 필요함.
		}break;
		case VAT_HAND_UNLOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			if (nPart == VAT_IDX_TEST_PP_TBL_1_LD || nPart == VAT_IDX_TEST_PP_TBL_2_LD || nPart == VAT_IDX_TEST_PP_TBL_4_LD || nPart == VAT_IDX_CLEAN_TBL) {
				bRet = FALSE;
			}
		}break;
		default:
			break;
		}

		return bRet;
	}


}ST_VAT_FILE_SECTION;

