#pragma once

#define SZ_DM_NAME_4		_T("DM_TEST_PP")

//============================================================BOOL <start>
#define NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL				(0x000)	// 
#define NDM04_TESTHAND2_ALREADY_PICK_TABLE_CALL				(0x001)	// 

#define NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL				(0x002)	// 
#define NDM04_TESTHAND2_ALREADY_PICK_UNLOAD_TABLE_CALL				(0x003)	// 

#define BDM4_TESTHAND1_ALL_SOCKET_OFF_PLACE_ART_BUFFER				(0x004)
#define BDM4_TESTHAND2_ALL_SOCKET_OFF_PLACE_ART_BUFFER				(0x005)
//============================================================BOOL <end>

//============================================================INTEGER <start>
#define NDM4_PICKER_COUNT_X            (0x000)
#define NDM4_PICKER_COUNT_Y            (0x001)
#define NDM4_HAND_PITCH_MIN_X          (0x002)
#define NDM4_HAND_PITCH_MIN_Y          (0x003)
#define NDM4_HAND_PITCH_MAX_X          (0x004)
#define NDM4_HAND_PITCH_MAX_Y          (0x005)

#define NDM04_RECOVERY_SEQ_ENABLE				(0x011)	// 돌던 중 멈췄는지에 따라 recovery loop 점검

#define NDM04_TESTHAND1_WORKING_IDX				(0x012)	// 마지막 index
#define NDM04_TESTHAND2_WORKING_IDX				(0x013)	// 마지막 index

#define NDM04_TESTHAND1_WORKING_BEFORE_IDX				(0x014)	// 마지막 index
#define NDM04_TESTHAND2_WORKING_BEFORE_IDX				(0x015)	// 마지막 index

#define NDM04_TESTHAND1_ALREADY_PICK_WORKING_IDX				(0x016)	// 마지막 index
#define NDM04_TESTHAND2_ALREADY_PICK_WORKING_IDX				(0x017)	// 마지막 index

#define NDM04_TESTHAND1_ALREADY_PLACE_WORKING_IDX				(0x018)
#define NDM04_TESTHAND2_ALREADY_PLACE_WORKING_IDX				(0x019)

#define NDM04_SITEGROUP1_RESERVED				(0x020)		// 0:Not Reserved, 1:Reserved
#define NDM04_SITEGROUP2_RESERVED				(0x021)
#define NDM04_SITEGROUP3_RESERVED				(0x022)
#define NDM04_SITEGROUP4_RESERVED				(0x023)
#define NDM04_SITEGROUP1_RESERVED_DEVICE_CNT	(0x040)		// 예약된 Device 개수
#define NDM04_SITEGROUP2_RESERVED_DEVICE_CNT	(0x041)
#define NDM04_SITEGROUP3_RESERVED_DEVICE_CNT	(0x042)
#define NDM04_SITEGROUP4_RESERVED_DEVICE_CNT	(0x043)
#define NDM04_SITEGROUP1_INSERTED_DEVICE_CNT	(0x060)		// 투입된 Device 개수
#define NDM04_SITEGROUP2_INSERTED_DEVICE_CNT	(0x061)
#define NDM04_SITEGROUP3_INSERTED_DEVICE_CNT	(0x062)
#define NDM04_SITEGROUP4_INSERTED_DEVICE_CNT	(0x063)

#define NDM4_TestPP1_Exch_Up_Station_Cnt			  (0x064)
#define NDM4_TestPP2_Exch_Up_Station_Cnt			  (0x065)	

#define NDM4_TestPP1_Exch_Dn_Station_Cnt			  (0x066)	
#define NDM4_TestPP2_Exch_Dn_Station_Cnt			  (0x067)

#define NDM4_TestPP1_Auto_Excuting_Step			  (0x068)	
#define NDM4_TestPP2_Auto_Excuting_Step			  (0x069)




// TEST HAND #1
#define NDM4_SPD_TestPP1_X1			   (0x090)	// NDM11_SPD_TestPP1_X1
#define NDM4_ACC_TestPP1_X1			   (0x091)	// NDM11_ACC_TestPP1_X1
#define NDM4_DCC_TestPP1_X1			   (0x092)	// NDM11_DCC_TestPP1_X1
#define NDM4_SPD_TestPP1_X2			   (0x093)	// NDM11_SPD_TestPP1_X2
#define NDM4_ACC_TestPP1_X2			   (0x094)	// NDM11_ACC_TestPP1_X2
#define NDM4_DCC_TestPP1_X2			   (0x095)	// NDM11_DCC_TestPP1_X2
#define NDM4_SPD_TestPP1_Y			   (0x096)	// NDM11_SPD_TestPP1_Y
#define NDM4_ACC_TestPP1_Y			   (0x097)	// NDM11_ACC_TestPP1_Y
#define NDM4_DCC_TestPP1_Y			   (0x098)	// NDM11_DCC_TestPP1_Y
#define NDM4_SPD_TestPP1_Z1			   (0x099)	// NDM11_SPD_TestPP1_Z
#define NDM4_ACC_TestPP1_Z1			   (0x09A)	// NDM11_ACC_TestPP1_Z
#define NDM4_DCC_TestPP1_Z1			   (0x09B)	// NDM11_DCC_TestPP1_Z
#define NDM4_SPD_TestPP1_Z2			   (0x09C)
#define NDM4_ACC_TestPP1_Z2			   (0x09D)
#define NDM4_DCC_TestPP1_Z2			   (0x09E)
#define NDM5_SPD_TestPP_1_Pitch_X1	   (0x09F)
#define NDM5_ACC_TestPP_1_Pitch_X1	   (0x0A0)
#define NDM5_DCC_TestPP_1_Pitch_X1	   (0x0A1)
#define NDM5_SPD_TestPP_1_Pitch_X2	   (0x0A2)
#define NDM5_ACC_TestPP_1_Pitch_X2	   (0x0A3)
#define NDM5_DCC_TestPP_1_Pitch_X2	   (0x0A4)
#define NDM5_SPD_TestPP_1_Pitch_Y1	   (0x0A5)
#define NDM5_ACC_TestPP_1_Pitch_Y1	   (0x0A6)
#define NDM5_DCC_TestPP_1_Pitch_Y1	   (0x0A7)
#define NDM5_SPD_TestPP_1_Pitch_Y2	   (0x0A8)
#define NDM5_ACC_TestPP_1_Pitch_Y2	   (0x0A9)
#define NDM5_DCC_TestPP_1_Pitch_Y2	   (0x0AA)

// TEST HAND #2
#define NDM4_SPD_TestPP2_X1			   (0x120)	// NDM11_SPD_TestPP2_X1
#define NDM4_ACC_TestPP2_X1			   (0x121)	// NDM11_ACC_TestPP2_X1
#define NDM4_DCC_TestPP2_X1			   (0x122)	// NDM11_DCC_TestPP2_X1
#define NDM4_SPD_TestPP2_X2			   (0x123)	// NDM11_SPD_TestPP2_X2
#define NDM4_ACC_TestPP2_X2			   (0x124)	// NDM11_ACC_TestPP2_X2
#define NDM4_DCC_TestPP2_X2			   (0x125)	// NDM11_DCC_TestPP2_X2
#define NDM4_SPD_TestPP2_Y			   (0x126)	// NDM11_SPD_TestPP2_Y
#define NDM4_ACC_TestPP2_Y			   (0x127)	// NDM11_ACC_TestPP2_Y
#define NDM4_DCC_TestPP2_Y			   (0x128)	// NDM11_DCC_TestPP2_Y
#define NDM4_SPD_TestPP2_Z1			   (0x129)	// NDM11_SPD_TestPP1_Z
#define NDM4_ACC_TestPP2_Z1			   (0x12A)	// NDM11_ACC_TestPP1_Z
#define NDM4_DCC_TestPP2_Z1			   (0x12B)	// NDM11_DCC_TestPP1_Z
#define NDM4_SPD_TestPP2_Z2			   (0x12C)
#define NDM4_ACC_TestPP2_Z2			   (0x12D)
#define NDM4_DCC_TestPP2_Z2			   (0x12E)
#define NDM5_SPD_TestPP_2_Pitch_X1	   (0x12F)
#define NDM5_ACC_TestPP_2_Pitch_X1	   (0x130)
#define NDM5_DCC_TestPP_2_Pitch_X1	   (0x131)
#define NDM5_SPD_TestPP_2_Pitch_X2	   (0x132)
#define NDM5_ACC_TestPP_2_Pitch_X2	   (0x133)
#define NDM5_DCC_TestPP_2_Pitch_X2	   (0x134)
#define NDM5_SPD_TestPP_2_Pitch_Y1	   (0x135)
#define NDM5_ACC_TestPP_2_Pitch_Y1	   (0x136)
#define NDM5_DCC_TestPP_2_Pitch_Y1	   (0x137)
#define NDM5_SPD_TestPP_2_Pitch_Y2	   (0x138)
#define NDM5_ACC_TestPP_2_Pitch_Y2	   (0x139)
#define NDM5_DCC_TestPP_2_Pitch_Y2	   (0x13A)




#define NDM4_TestPP1_RetryCnt		   (0x160)	// NDM11_TestPP1_RetryCnt
#define NDM4_TestPP2_RetryCnt		   (0x161)	// NDM11_TestPP2_RetryCnt

#define NDM4_TM_TestPP1_Vacuum		   (0x166)	// NDM11_TM_TestPP1_Vacuum	
#define NDM4_TM_TestPP1_Blow		   (0x167)	// NDM11_TM_TestPP1_Blow	
#define NDM4_TM_TestPP2_Vacuum		   (0x168)	// NDM11_TM_TestPP2_Vacuum	
#define NDM4_TM_TestPP2_Blow		   (0x169)	// NDM11_TM_TestPP2_Blow	

// Test site
#define NDM4_TestSite_Pitch_X		   (0x180)	// NDM11_TestSite_Pitch_X		
#define NDM4_TestSite_Pitch_Y		   (0x181)	// NDM11_TestSite_Pitch_Y		
#define NDM4_TestSite_Div_X			   (0x182)	// NDM11_TestSite_Div_X		
#define NDM4_TestSite_Div_Y			   (0x183)	// NDM11_TestSite_Div_Y		
// ART buffer table
#define NDM4_TestBuffTbl_Div_X         (0x184)	// NDM11_TestBuffTbl_Div_X     
#define NDM4_TestBuffTbl_Div_Y         (0x185)	// NDM11_TestBuffTbl_Div_Y     
#define NDM4_TestBuffTbl_Pitch_X	   (0x186)	// NDM11_TestBuffTbl_Pitch_X	
#define NDM4_TestBuffTbl_Pitch_Y	   (0x187)	// NDM11_TestBuffTbl_Pitch_Y	
// Cleaning device table
#define NDM4_TestCleanTbl_Div_X        (0x188)
#define NDM4_TestCleanTbl_Div_Y        (0x189)
#define NDM4_TestCleanTbl_Pitch_X	   (0x18A)
#define NDM4_TestCleanTbl_Pitch_Y	   (0x18B)

#define NDM4_TestPP_Pitch_X			   (0x18C)	
#define NDM4_TestPP_Pitch_Y			   (0x18D)

#define NDM4_TestPP_ST_Pitch_X		   (0x18E)
#define NDM4_TestPP_ST_Pitch_Y			(0x18F)




//============================================================INTEGER <end>

//============================================================  DOUBLE <start>

#define DDM4_TestPP1_Exch_Dn_Station_Time			  (0x000)	
#define DDM4_TestPP2_Exch_Dn_Station_Time			  (0x001)

#define DDM4_TestPP1_Exch_Up_Station_Time			  (0x002)	
#define DDM4_TestPP2_Exch_Up_Station_Time			  (0x003)

#define DDM4_TestPP1_Pick_Retry_Offset				  (0x004)	
#define DDM4_TestPP2_Pick_Retry_Offset			      (0x005)


#define DDM4_TestPP1_Picker_Wait_Time				 (0x006)	
#define DDM4_TestPP2_Picker_Wait_Time			      (0x007)

//============================================================  DOUBLE <end>

//============================================================STRING <start>
//============================================================STRING <end>