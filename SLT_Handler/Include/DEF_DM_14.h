#pragma once

#define SZ_DM_NAME_14		_T("DM_VAT")

//============================================================ BOOL
#define BDM14_VAT_UI_OPEN_HSITORY						(0x00)

#define BDM14_VAT_MD_USE								(0x01) //VAT MD 사용( VAT GUI 표기 )
#define BDM14_VAT_TEACHING_USE							(0x02) //VAT 측정 시, 사용(Motor 속도, Base Task Option 등)
#define BDM14_VAT_TEACHING_END							(0x03)





//============================================================ NUMERIC
#define NDM14_VAT_CURRENT_MODE									(0x00)

#define NDM14_VAT_TRANSFER_SOURCE_INDEX				(0x10)
#define NDM14_VAT_TRANSFER_DESTINATION_INDEX				(0x11)

//Simul Rcv Cnt - 해당 Cnt로 Simul시, Vision Retry 진행.
#define NDM14_VAT_SIMUL_CAM_1_RCV_CNT				(0x20)	  //VAT_CAM_LD_TRAY_PP_UPPER
#define NDM14_VAT_SIMUL_CAM_2_RCV_CNT				(0x21)	  //VAT_CAM_LD_TRAY_PP_LOWER
#define NDM14_VAT_SIMUL_CAM_3_RCV_CNT				(0x22)	  //VAT_CAM_UD_TRAY_PP_UPPER
#define NDM14_VAT_SIMUL_CAM_4_RCV_CNT				(0x23)	  //VAT_CAM_UD_TRAY_PP_LOWER
#define NDM14_VAT_SIMUL_CAM_5_RCV_CNT				(0x24)	  //VAT_CAM_LD_TEST_PP_1_UPPER
#define NDM14_VAT_SIMUL_CAM_6_RCV_CNT				(0x25)	  //VAT_CAM_UD_TEST_PP_1_UPPER
#define NDM14_VAT_SIMUL_CAM_7_RCV_CNT				(0x26)	  //VAT_CAM_TEST_PP_1_LOWER
#define NDM14_VAT_SIMUL_CAM_8_RCV_CNT				(0x27)	  //VAT_CAM_LD_TEST_PP_2_UPPER
#define NDM14_VAT_SIMUL_CAM_9_RCV_CNT				(0x28)	  //VAT_CAM_UD_TEST_PP_2_UPPER
#define NDM14_VAT_SIMUL_CAM_10_RCV_CNT				(0x29)	  //VAT_CAM_TEST_PP_2_LOWER


#define NMD14_VAT_PICKER_ALIGN_AUTO_FOCUS				(0x40)

#define NMD14_VAT_MEASURE_HAND_ONLY_1				(0x50)
#define NMD14_VAT_MEASURE_HAND_ONLY_2				(0x51)
#define NMD14_VAT_MEASURE_HAND_ONLY_3				(0x52)
#define NMD14_VAT_MEASURE_HAND_ONLY_4				(0x53) //spare(UD Hand)
#define NMD14_VAT_MEASURE_HAND_ONLY_5				(0x54) 
#define NMD14_VAT_MEASURE_HAND_ONLY_6				(0x55) //spare(UD Hand)

#define NMD14_VAT_MEASURE_UNIT_ONLY_1				(0x54)
#define NMD14_VAT_MEASURE_UNIT_ONLY_2				(0x55)
#define NMD14_VAT_MEASURE_UNIT_ONLY_3				(0x56)
#define NMD14_VAT_MEASURE_UNIT_ONLY_4				(0x57) //spare(UD Hand)
#define NMD14_VAT_MEASURE_UNIT_ONLY_5				(0x58)
#define NMD14_VAT_MEASURE_UNIT_ONLY_6				(0x59) //spare(UD Hand)

//Hand 및 Part 측정 상태.
//=================================[Z AUTO FOCUS]
//VAT_HAND_LOAD_TRAYPP		: 0x100
//VAT_HAND_UNLOAD_TRAYPP	: 0x101
//VAT_HAND_LOAD_TESTPP1		: 0x102
//VAT_HAND_UNLOAD_TESTPP1	: 0x103
//VAT_HAND_LOAD_TESTPP2		: 0x104
//VAT_HAND_UNLOAD_TESTPP2	: 0x105
#define NDM14_State_ZAutoFocus_Hand_1		(0x100)
#define NDM14_State_ZAutoFocus_Hand_6		(0x105)


//=================================[CAM CENTER CALIBRATION]
//VAT_HAND_LOAD_TRAYPP		: 0x110
//VAT_HAND_UNLOAD_TRAYPP	: 0x111
//VAT_HAND_LOAD_TESTPP1		: 0x112
//VAT_HAND_UNLOAD_TESTPP1	: 0x113
//VAT_HAND_LOAD_TESTPP2		: 0x114
//VAT_HAND_UNLOAD_TESTPP2	: 0x115
#define NDM14_State_CamCenterCal_Hand_1		(0x110)
#define NDM14_State_CamCenterCal_Hand_6		(0x115)

//=================================[PICKER HEIGHT CAL]
//VAT_HAND_LOAD_TRAYPP		: 0x120
//VAT_HAND_UNLOAD_TRAYPP	: 0x121
//VAT_HAND_LOAD_TESTPP1		: 0x122
//VAT_HAND_UNLOAD_TESTPP1	: 0x123
//VAT_HAND_LOAD_TESTPP2		: 0x124
//VAT_HAND_UNLOAD_TESTPP2	: 0x125
#define NDM14_State_PickerAlign_Hand_1		(0x120)
#define NDM14_State_PickerAlign_Hand_6		(0x125)

//=========================================[VISION AUTO TEACHING]
//VAT_HAND_LOAD_TRAYPP		: 0x130 ~ 0X13A
#define NDM14_State_Vat_LD_Tray_Pp_Part1	(0x130)
#define NDM14_State_Vat_LD_Tray_Pp_Part11	(0X13A)

//VAT_HAND_UNLOAD_TRAYPP	: 0x140 ~ 0x14A
#define NDM14_State_Vat_UD_Tray_Pp_Part1	(0x140)
#define NDM14_State_Vat_UD_Tray_Pp_Part11	(0x14A)

//VAT_HAND_LOAD_TESTPP1		: 0x150 ~ 0x15B
#define NDM14_State_Vat_LD_Test_Pp_1_Part1	(0x150)
#define NDM14_State_Vat_LD_Test_Pp_1_Part12	(0x15B)

//VAT_HAND_UNLOAD_TESTPP1	: 0x160 ~ 0x16B
#define NDM14_State_Vat_UD_Test_Pp_1_Part1	(0x160)
#define NDM14_State_Vat_UD_Test_Pp_1_Part12	(0x16B)

//VAT_HAND_LOAD_TESTPP2		: 0x170 ~ 0x17B
#define NDM14_State_Vat_LD_Test_Pp_2_Part1	(0x170)
#define NDM14_State_Vat_LD_Test_Pp_2_Part12	(0x17B)

//VAT_HAND_UNLOAD_TESTPP2	: 0x180 ~ 0x18B
#define NDM14_State_Vat_UD_Test_Pp_2_Part1	(0x180)
#define NDM14_State_Vat_UD_Test_Pp_2_Part12	(0x18B)


//===============================================[Z AUTO TEACHING]
//VAT_HAND_LOAD_TRAYPP		: 0x190 ~ 0X19A
#define NDM14_State_ZTeach_LD_Tray_Pp_Part1	(0x190)
#define NDM14_State_ZTeach_LD_Tray_Pp_Part11 (0X19A)

//VAT_HAND_UNLOAD_TRAYPP	: 0x1A0 ~ 0x1AA
#define NDM14_State_ZTeach_UD_Tray_Pp_Part1	(0x1A0)
#define NDM14_State_ZTeach_UD_Tray_Pp_Part11	(0x1AA)

//VAT_HAND_LOAD_TESTPP1		: 0x1B0 ~ 0x1BB
#define NDM14_State_ZTeach_LD_Test_Pp_1_Part1	(0x1B0)
#define NDM14_State_ZTeach_LD_Test_Pp_1_Part12	(0x1BB)

//VAT_HAND_UNLOAD_TESTPP1	: 0x1C0 ~ 0x1CB
#define NDM14_State_ZTeach_UD_Test_Pp_1_Part1	(0x1C0)
#define NDM14_State_ZTeach_UD_Test_Pp_1_Part12	(0x1CB)

//VAT_HAND_LOAD_TESTPP2		: 0x1D0 ~ 0x1DB
#define NDM14_State_ZTeach_LD_Test_Pp_2_Part1	(0x1D0)
#define NDM14_State_ZTeach_LD_Test_Pp_2_Part12	(0x1DB)

//VAT_HAND_UNLOAD_TESTPP2	: 0x1E0 ~ 0x1EB
#define NDM14_State_ZTeach_UD_Test_Pp_2_Part1	(0x1E0)
#define NDM14_State_ZTeach_UD_Test_Pp_2_Part12	(0x1EB)


//============================================================ DOUBLE
#define DDM14_000									(0x00)

//============================================================ STRING
#define SDM14_000								(0x00)