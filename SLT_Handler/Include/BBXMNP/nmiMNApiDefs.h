/******************************************************************************
*
*	File Version: 1,0,0,0
*
*	Copyright(c) 2012 Alpha Motion Co,. Ltd. All Rights Reserved.
*
*	This file is strictly confidential and do not distribute it outside.
*
*	MODULE NAME :
*		nmiMApiDefs.h
*
*	AUTHOR :
*		K.C. Lee
*
*	DESCRIPTION :
*		the header file for RC files of pfroject.
*
*
* - Phone: +82-31-303-5050
* - Fax  : +82-31-303-5051
* - URL  : http://www.alphamotion.co.kr,
*
*
/****************************************************************************/
#ifndef TMC_MAPI_DEFS_H
#define TMC_MAPI_DEFS_H
	

#ifdef __cplusplus
extern "C" {
#endif



// VALUE가 MIN과 MAX 사이의 값인가 ?
#ifndef InBound
#define InBound(MIN, MAX, VALUE)		((MIN) <= (VALUE) && (VALUE) <= (MAX) ? TRUE : FALSE)
#endif	// InBound

// VALUE가 MIN보다 작은 값이면 MIN, MAX보다 큰 값이면 MAX, MIN과 MAX사이의 값이면 VALUE를 리턴
#ifndef Bound
#define Bound(MIN, MAX, VALUE)			((MIN) > (VALUE) ? (MIN) : ((MAX) < (VALUE) ? (MAX) : (VALUE)))
#endif

// 반올림
#ifndef round
#define round(x)						(x >= 0)?(INT32)(x + 0.5):(INT32)(x -0.5)
#endif	// round


// Device ID definition //
#define   TMC_BA800P					0xB8     
#define   TMC_BA600P					0xB6
#define   TMC_BA400P					0xB4
#define   TMC_BA200P					0xB2

#define   TMC_BB160P					0xCF
#define   TMC_BB120P					0xCC


#define   TMC_BB8MNP					0xD8
#define   TMC_BB4MNP					0xD4
#define   TMC_BB0MNP					0xD0
#define   TMC_BB800P					0xC8
#define   TMC_BB400P					0xC4


// Definition of maximum number of things //

//Device Max
#define  TMC_MAX_MOT_DEV                 16  // Maximum number of Motion Devices in one PC
#define  TMC_MAX_MOT_AXES                16  // Maximum number of motion axes

#define  TMC_MAX_DI						 48 // Maximum number of D/I channels
#define  TMC_MAX_DO						 48 // Maximum number of D/O channels


//String
#define TMC_MAX_STR  	                  256 //Maximum String Length

// Slave Max
#define  TMC_MAX_SLAVE_DEC				 64 // Maximum number of Slave Device in one Motion-Net											



//사용자 이벤트 메시지
#define TMC_WM_INTERRUPT	 (WM_USER + 2000)




// Motion Chip Registers //
enum {	
	  MSTS  =  0, RMV,   PRMV,  RFL,   PRFL,  RFH,   PRFH,  RUR,   PRUR,  RDR,   PRDR,  RMG,   PRMG,  RDP,   PRDP, 
	  RMD   = 15, PRMD,  RIP,   PRIP,  RUS,   PRUS,  RDS,   PRDS,  RFA,   RENV1, RENV2, RENV3, RENV4, RENV5, RENV6, RENV7, 
      RCUN1 = 31, RCUN2, RCUN3, RCUN4, RCMP1, RCMP2, RCMP3, RCMP4, RCMP5, PRCP5, RIRQ,  RLTC1, RLTC2, RLTC3, RLTC4, 
	  RSTS  = 46, REST,  RIST,  RPLS,  RSPD,  RPSDC, RCI,  RIPS,  RCIC,
	  RFCUN = 55, RFSER, RFSMR, RFGER0, RFGMR0, RFGER1, RFGMR1, RFGER2, RFGMR2, RFGER3, RFGMR3
};


//***********************************************************************************************
//				CONSTATNS for User												*
//***********************************************************************************************

// Axis index definition //
#define emAX0						0
#define emAX1						1
#define emAX2						2
#define emAX3						3
#define emAX4						4
#define emAX5						5
#define emAX6						6
#define emAX7						7
#define emAX8						8
#define emAX9						9
#define emAXA						10
#define emAXB						11
#define emAXC						12
#define emAXD						13
#define emAXE						14
#define emAXF						15


// Definition for axes mask  //
#define emAX0_MASK 					0x00000001
#define emAX1_MASK 					0x00000002
#define emAX2_MASK 					0x00000004
#define emAX3_MASK 					0x00000008
#define emAX4_MASK 					0x00000010
#define emAX5_MASK 					0x00000020
#define emAX6_MASK 					0x00000040
#define emAX7_MASK 					0x00000080
#define emAX8_MASK 					0x00000100
#define emAX9_MASK 					0x00000200
#define emAXA_MASK 					0x00000400
#define emAXB_MASK 					0x00000800
#define emAXC_MASK 					0x00001000
#define emAXD_MASK 					0x00002000
#define emAXE_MASK 					0x00004000
#define emAXF_MASK 					0x00008000


// Definition for axes mask  //
#define emGAX0_BIT 					0x00000000
#define emGAX1_BIT 					0x00000001
#define emGAX2_BIT 					0x00000002
#define emGAX3_BIT 					0x00000003
#define emGAX4_BIT 					0x00000004
#define emGAX5_BIT 					0x00000005
#define emGAX6_BIT 					0x00000006
#define emGAX7_BIT 					0x00000007
#define emGMPG_BIT 					0x00000008
#define emGPAB_BIT 					0x00000009


/////////////////////////////////////////////////////////////////
#define emBIT_OFF					0
#define emBIT_ON					1

#define	 emOFF			 			0
#define  emON			    		1

// Boolean type definition //
#define emFALSE 					0
#define emTRUE 						1

// Used type definition //
#define emNUSED 					0
#define emUSED		 				1

#define emDISABLED 					0
#define emENABLED	 				1


// Used type definition //
#define emLOW						0 
#define emHIGH						1

// Used type definition //
#define emLOGIC_A					0 
#define emLOGIC_B					1


// motion type definition 	
#define	 emSTAND                0 /*steady*/
#define  emRUNNING              1 /*running*/

// stop type definition //
#define emESTOP 						0
#define emSSTOP 						1 
#define emSTOP  						1 

//Operation direction 
#define  emDIR_P                0 /*(+)Dir*/
#define  emDIR_N                1/*(-)Dir*/

// speed profile type definition //
#define emPROF_C				 0   //Constant Speed Profile
#define emPROF_T                 1  //Trapezoidal Speed Profile
#define emPROF_S                 2  //S-Curve Speed Profile
#define emPROF_N                 3  //Curve Speed Profile


// speed profile type definition 
#define	emCONST                 0
#define emTCURVE                1 
#define emSCURVE                2
#define emNCURVE                3


#define emINC                   0	/*Relative*/ 
#define emABS                   1	/*Absolute*/

// Arc operation direction //
#define emCW                    0  //Clockwise Direction         ( Angle Decreases  < 0)
#define emCCW                   1  //Counter Clockwise Direction ( Angle Increases  > 0)

#define emOPEN_LOOP					0   //OPEN CLOSE LOOP
#define emSEMI_LOOP					1   //SEMI CLOSE LOOP
#define emFULL_LOOP					2   //FULL CLOSE LOOP(추후 예약)

// Bit order of pmiAxGetMechanical() return value  //
#define emMEC_S_EMG 					0x000000001 // EMG 입력 신호 상태
#define emMEC_S_ALM 					0x000000002 // 알람 입력 신호 상태
#define emMEC_S_ELP  					0x000000004 // POSITIVE 리미트 스위치 입력 신호 상태
#define emMEC_S_ELN  					0x000000008 // NEGATIVE 리미트 스위치 입력 신호 상태
#define emMEC_S_ORG  					0x000000010 // ORIGIN 스위치 입력 신호 상태
#define emMEC_S_DIR 					0x000000020 // 동작 방향 상태 ( 0 : +방향 , 1 : -방향) 
#define emMEC_S_OK	 					0x000000040 // HOME_OK 입력 신호 상태
#define emMEC_S_PCS 					0x000000080 // PCS 입력 신호 상태
#define emMEC_S_CRC  					0x000000100 // CRC 출력 신호 상태
#define emMEC_S_EZ 						0x000000200 // EZ 입력 신호 상태
#define emMEC_S_CLR  					0x000000400 // CLR 입력 신호 상태
#define emMEC_S_LTC  					0x000000800 // LATCH 입력 신호 상태
#define emMEC_S_SD   					0x000001000 // SLOW DOWN 입력 신호 상태
#define emMEC_S_INP 					0x000002000 // IN-POSITION 입력 신호 상태
#define emMEC_S_SVON				    0x000004000 //SVON 신호 상태
#define emMEC_S_ARST				    0x000008000 //SERVO ALARM RESET 신호 상태
#define emMEC_S_STA  					0x000010000 // STA 입력 신호 상태
#define emMEC_S_STP  					0x000020000 // STP 입력 신호 상태
#define emMEC_S_ESYNC  					0x000040000 // SYNC 펀차 입력 신호 상태
#define emMEC_S_EGANT  					0x000080000 // GANTRY 편차 에러 입력 신호 상태
#define emMEC_S_DRV  					0x000100000 // 구동중
#define emMEC_S_CMP  					0x000200000 // 위치비교 사용중
#define emMEC_S_SYNC  					0x000400000 // SYNC 사용중
#define emMEC_S_GANT  					0x000800000 // GANTRY 사용중


#define  emDRV_STOP 					0x00000001 // 모션 정지 중
#define  emWAIT_DR 						0x00000002 // DR 입력 신호 기다림
#define  emWAIT_STA 					0x00000004 // STA 입력 신호 기다림
#define  emWAIT_INSYNC 					0x00000008 // 내부 동기 신호 기다림
#define  emWAIT_OTHER 					0x00000010 // 타축 정지 신호 기다림
#define  emWAIT_CRC 					0x00000020 // CRC 출력 완료 기다림
#define  emWAIT_DIR 					0x00000040 // 방향 변화를 기다림
#define  emBACKLASH 					0x00000080 // BACKLASH 상태
#define  emWAIT_MPG 					0x00000100 // PA/PB 입력 신호 기다림
#define  emIN_FA 						0x00000200 // FA 정속 동작 중 ( 홈 스페셜 속도 )
#define  emIN_FL 						0x00000400 // FL 정속 동작 중
#define  emIN_FUR 						0x00000800 // 가속 중
#define  emIN_FH 						0x00001000 // FH 정속 동작 중 
#define  emIN_FDR 						0x00002000 // 감속 중 
#define  emWAIT_INP 					0x00004000 // INP 입력 신호 기다림

// LEVEL type definition //
#define emLEVEL_LOW 					0 
#define emLEVEL_HIGH 					1 

// stp stop type definition //
#define emSTP_NSTOP 					0 
#define emSTP_ESTOP 					1 
#define emSTP_SSTOP 					2 

//soft limit action
#define emSOFT_NONE 					0 
#define emSOFT_ESTOP 					1 
#define emSOFT_SSTOP 					2 
#define emSOFT_CHVEL 					3 

// remain pulse stop type definition //
#define emAUTO 				     	0
#define emMANUAL 					1

#define emSD_INIT 					0 
#define emSD_STOP 					1
#define emSD_LATCH 					0 	
// 
#define emERR_NONE 		   			    0 /*사용하지 않음*/ 
#define emERR_ESTOP 					1 /*급   정지*/
#define emERR_STOP 					2 /*감속 정지*/
#define emERR_CHGVEL					3 /*속도 변경*/

// Encoder input mode definition //
#define emEAB1X 					0x00
#define emEAB2X 					0x01 
#define emEAB4X 					0x02 
#define emCWCCW 					0x03 

//  PA/PB input mode definition //
#define emPAB1X 					0x00
#define emPAB2X 					0x01
#define emPAB4X 					0x02
#define emPCWCCW 					0x03

// Command output mode definition //
#define emONE_LOW_HIGH_LOW 			0x00
#define emONE_HIGH_HIGH_LOW 		0x01
#define emONE_LOW_LOW_HIGH 			0x02
#define emONE_HIGH_LOW_HIGH 		0x03
#define emTWO_CW_CCW_LOW 			0x04
#define emTWO_CW_CCW_HIGH 			0x05
#define emTWO_CCW_CW_LOW 			0x06
#define emTWO_CCW_CW_HIGH 			0x07
#define emTWO_PHASE 				0x08
#define emTWO_PHASE_RESERVE         0x09 

#define emCOMM                  0/*Command*/
#define emFEED                  1/*Feedback*/
#define emPAPB					2/*PA/B*/
#define emDEV                   2/*Deviation*/ 
#define emGEN                   3/*General*/
#define emCURVEL                4/*현재속도*/

#define emEQ_PDIR               0   //CmpData = CmpSrc_Value ( while counting up )
#define emEQ_NDIR               1  // CmpData = CmpSrc_Value ( while counting down )


// sta method definition //	
#define emSTA_SOFTWARE			  0 /*Software */
#define emSTA_HARDWARE			  1 /* Hardware*/

// sta action definition //	
#define emACT_LEVEL					0  /* Level Trigger */
#define emACT_FALL					1  /* Edge Trigger*/

// Compare Type //
#define emCMP_IDX0					0  /*compare[0]*/
#define emCMP_IDX1					1  /*compare[1]*/
#define emCMP_IDX2					2  /*compare[2]*/
#define emCMP_IDX3					3  /*compare[3]*/

// Gantry Type //
#define emGANT_IDX0					0  /*Gantry[0]*/
#define emGANT_IDX1					1  /*Gantry[1]*/
#define emGANT_IDX2					2  /*Gantry[2]*/
#define emGANT_IDX3					3  /*Gantry[3]*/

// motion type definition //	
#define emFORWARD					0/*forward*/
#define emRESERVE 					1 /*reserved*/

// Action when general comparator met the condition //
#define emEVT_ONLY					0  // 인터럽트만 발생
#define emEVT_ESTOP					1  // 인터럽트 및 급 정지
#define emEVT_STOP					2  // 인터럽트 및 감속후 정지
#define emEVT_SPDCHG				        3  // 인터럽트 및 속도변경

// Backlash/Slip correction mode //
#define  emCORR_DIS					0 // Disable correction 
#define  emCORR_BACK				1 // Backlash correction mode 
#define  emCORR_SLIP				2 // Slip correction mode

//position trigger output level
#define  emRISING					    0
#define  emFALLING						1

// Interrupt Handler Type //
#define  emIHT_MESSAGE					0
#define  emIHT_CALLBACK					1
#define  emIHT_EVENT 		        	2

// SetFilterAB의 대상 //
#define  emPULSAR_AB			        0
#define  emENC_AB				      	1

#define  emHOME_POS_RST0		        0 	// ORG(/EL/EZ) 신호가 발생할 때 COMMAND & FEEDBACK 위치를 0으로 클리어한다.
#define  emHOME_POS_RST1				1	// 원점복귀를 모두 완료하고 나서 COMMAND & FEEDBACK 위치를 모두 0으로 클리어한다.
#define  emHOME_POS_RST2				2	// 원점복귀를 모두 완료하고 나서 FEEDBACK 위치는 그대로 두고 COMMAND 위치를 FEEDBACK 위치에 일치시킨다.

#define emHOME_NG                       0  	// 원점복귀를 정상적으로 완료하지 않음
#define emHOME_OK                       1	// 원점복귀를 정상적으로 완료함.

// Sync mode //
#define  emSYNC_DISABLE					0
#define  emSYNC_INT_SYNC				1
#define  emSYNC_OTHER_STOP				2

// Internal sync. conditions //
#define  emISYNC_ACC_STA				1	// 1: at start of acceleration
#define  emISYNC_ACC_END				2	// 2: at end of acceleration
#define  emISYNC_DEC_STA				3	// 3: at start of deceleration
#define  emISYNC_DEC_END				4	// 4: at end of deceleration
#define  emISYNC_SLN					5	// 5: when (-)software limit met
#define  emISYNC_SLP					6	// 6: when (+)software limit met
#define  emISYNC_GCMP					7	// 7: when General Comparator condition is satisfied
#define  emISYNC_TCMP					8	// 8: when Trigger Comparator condition is satisfied


//Coordinate Systems
#define emMAX_CS                     4 //Maximum Number of Coordinate Systems

//Coordinate System No.
#define emCS0                        0
#define emCS1                        1
#define emCS2                        2
#define emCS3                        3

//Coordinate System Mask
#define emCS0_MASK                   0x00000001
#define emCS1_MASK                   0x00000002
#define emCS2_MASK                   0x00000004
#define emCS3_MASK                   0x00000008
#define emCS4_MASK                   0x00000010
#define emCS5_MASK                   0x00000020
#define emCS6_MASK                   0x00000040
#define emCS7_MASK                   0x00000080
#define emCS8_MASK                   0x00000100
#define emCS9_MASK                   0x00000200
#define emCSA_MASK                   0x00000400
#define emCSB_MASK                   0x00000800
#define emCSC_MASK                   0x00001000
#define emCSD_MASK                   0x00002000
#define emCSE_MASK                   0x00004000
#define emCSF_MASK                   0x00008000


// Internal sync. conditions //
#define  emCNS_VECTOR                0   //0:Vector Vel
#define  emCNS_MASTER	             1   //1:Long Axis Vel

// Function Log Level //
#define  emDLOG_NONE			 0   //LOG 출력 하지 않음
#define  emDLOG_ERROR_ONLY	     1   //에러만 발생한 경우 
#define  emDLOG_MOT_SET		     2   //모션 관련 함수 + 설정 함수(SET)  
#define  emDLOG_DO_SET		     3   //LEVEL 2 + 반환 함수(SET) + 디지털 출력 함수(DO)
#define  emDLOG_DI_SET		     4   //LEVEL 3 + 디지털 입력 함수(DI) 
#define  emDLOG_ALL	    	     5   //전부

// Motion net
#define  emCOMM_SPEED2_5	     0  // 2.5Mbps 통신 속도  
#define  emCOMM_SPEED5		     1  // 5Mbps   통신 속도   
#define  emCOMM_SPEED10		     2  // 10Mbps  통신 속도  
#define  emCOMM_SPEED20		     3  // 25Mbps  통신 속도

//슬레이브 중에서 SB-MN16GPIO-M 제품 
#define  emNONE_DIO               0   //  사용 안함
#define  emONLY_DI                1   //  입력 전용 
#define  emONLY_DO                2   //  출력 전용

// =======================================
// == IO Bit
// =======================================
#define TMC_BIT_OFF					0
#define TMC_BIT_ON					1

// Boolean type definition //
#define TMC_FALSE 					0
#define TMC_TRUE 					1

// Used type definition //
#define TMC_NOTUSED 				0
#define TMC_USED		 			1

// Used type definition //
#define TMC_LOW                     0 
#define TMC_HIGH                    1

// Axis index definition //
#define TMC_AX0 					0
#define TMC_AX1 					1
#define TMC_AX2 					2
#define TMC_AX3 					3
#define TMC_AX4 					4
#define TMC_AX5 					5
#define TMC_AX6 					6
#define TMC_AX7 					7
#define TMC_AX8 					8
#define TMC_AX9 					9
#define TMC_AXA 					10
#define TMC_AXB 					11
#define TMC_AXC 					12
#define TMC_AXD 					13
#define TMC_AXE 					14
#define TMC_AXF 					15


// Definition for axes mask  //
#define TMC_AX0_MASK 					0x00000001
#define TMC_AX1_MASK 					0x00000002
#define TMC_AX2_MASK 					0x00000004
#define TMC_AX3_MASK 					0x00000008
#define TMC_AX4_MASK 					0x00000010
#define TMC_AX5_MASK 					0x00000020
#define TMC_AX6_MASK 					0x00000040
#define TMC_AX7_MASK 					0x00000080
#define TMC_AX8_MASK 					0x00000100
#define TMC_AX9_MASK 					0x00000200
#define TMC_AXA_MASK 					0x00000400
#define TMC_AXB_MASK 					0x00000800
#define TMC_AXC_MASK 					0x00001000
#define TMC_AXD_MASK 					0x00002000
#define TMC_AXE_MASK 					0x00004000
#define TMC_AXF_MASK 					0x00008000



#define TMC_OPEN_LOOP					0   //OPEN CLOSE LOOP
#define TMC_SEMI_LOOP					1   //SEMI CLOSE LOOP
#define TMC_FULL_LOOP					2   //FULL CLOSE LOOP(추후 예약)

// Bit order of pmiAxGetMechanical() return value  //
#define TMC_MEC_S_EMG 					0x000000001 // EMG 입력 신호 상태
#define TMC_MEC_S_ALM 					0x000000002 // 알람 입력 신호 상태
#define TMC_MEC_S_ELP  					0x000000004 // POSITIVE 리미트 스위치 입력 신호 상태
#define TMC_MEC_S_ELN  					0x000000008 // NEGATIVE 리미트 스위치 입력 신호 상태
#define TMC_MEC_S_ORG  					0x000000010 // ORIGIN 스위치 입력 신호 상태
#define TMC_MEC_S_DIR 					0x000000020 // 동작 방향 상태 ( 0 : +방향 , 1 : -방향) 
#define TMC_MEC_S_OK	 				0x000000040 // HOME_OK 입력 신호 상태
#define TMC_MEC_S_PCS 					0x000000080 // PCS 입력 신호 상태
#define TMC_MEC_S_CRC  					0x000000100 // CRC 출력 신호 상태
#define TMC_MEC_S_EZ 					0x000000200 // EZ 입력 신호 상태
#define TMC_MEC_S_CLR  					0x000000400 // CLR 입력 신호 상태
#define TMC_MEC_S_LTC  					0x000000800 // LATCH 입력 신호 상태
#define TMC_MEC_S_SD   					0x000001000 // SLOW DOWN 입력 신호 상태
#define TMC_MEC_S_INP 					0x000002000 // IN-POSITION 입력 신호 상태
#define TMC_MEC_S_SVON                  0x000004000 //SVON 신호 상태
#define TMC_MEC_S_ARST                  0x000008000 //SERVO ALARM RESET 신호 상태
#define TMC_MEC_S_STA  					0x000010000 // STA 입력 신호 상태
#define TMC_MEC_S_STP  					0x000020000 // STP 입력 신호 상태
#define TMC_MEC_S_SYNC  				0x000040000 // SYNC 펀차 입력 신호 상태
#define TMC_MEC_S_GANT  				0x000080000 // GANTRY 편차 에러 입력 신호 상태

#define  TMC_DRV_STOP 					0x00000001 // 모션 정지 중
#define  TMC_WAIT_DR 					0x00000002 // DR 입력 신호 기다림
#define  TMC_WAIT_STA 					0x00000004 // STA 입력 신호 기다림
#define  TMC_WAIT_INSYNC 				0x00000008 // 내부 동기 신호 기다림
#define  TMC_WAIT_OTHER 				0x00000010 // 타축 정지 신호 기다림
#define  TMC_WAIT_CRC 					0x00000020 // CRC 출력 완료 기다림
#define  TMC_WAIT_DIR 					0x00000040 // 방향 변화를 기다림
#define  TMC_BACKLASH 					0x00000080 // BACKLASH 상태
#define  TMC_WAIT_PE 					0x00000100 // PA/PB 입력 신호 기다림
#define  TMC_IN_FA 						0x00000200 // FA 정속 동작 중 ( 홈 스페셜 속도 )
#define  TMC_IN_FL 						0x00000400 // FL 정속 동작 중
#define  TMC_IN_FUR 					0x00000800 // 가속 중
#define  TMC_IN_FH 						0x00001000 // FH 정속 동작 중 
#define  TMC_IN_FDR 					0x00002000 // 감속 중 
#define  TMC_WAIT_INP 					0x00004000 // INP 입력 신호 기다림
#define  TMC_IN_CMP 					0x00008000 // CMP 동작중
#define  TMC_WAIT_SYNC 					0x00010000 // SYNC 입력 신호 기다림
#define  TMC_WAIT_GANT 					0x00020000 // GANT 입력 신호 기다림



// LEVEL type definition //
#define TMC_LOGIC_A 					0 
#define TMC_LOGIC_B 					1 

#define TMC_LEVEL_LOW 					0 
#define TMC_LEVEL_HIGH 					1 

// stp stop type definition //
#define TMC_STP_NSTOP 					0 
#define TMC_STP_ESTOP 					1 
#define TMC_STP_STOP 					2 

// stop type definition //
#define TMC_ESTOP 						0
#define TMC_STOP 						1 

//soft limit action
#define TMC_SOFT_NONE 					0 
#define TMC_SOFT_ESTOP 					1 
#define TMC_SOFT_STOP 					2 
#define TMC_SOFT_CHVEL 					3 

// remain pulse stop type definition //
#define TMC_AUTO 					0
#define TMC_MANUAL 					1

#define TMC_SD_SLOW 					0 
#define TMC_SD_STOP 					1	


#define TMC_ERR_NONE 					0 /*사용하지 않음*/ 
#define TMC_ERR_ESTOP 					1 /*급   정지*/
#define TMC_ERR_STOP 					2 /*감속 정지*/
#define TMC_ERR_CHGVEL					3 /*속도 변경*/

// Encoder input mode definition //
#define TMC_EAB1X 					0x00
#define TMC_EAB2X 					0x01 
#define TMC_EAB4X 					0x02 
#define TMC_CWCCW 					0x03 

//  PA/PB input mode definition //
#define TMC_PAB1X 					0x00
#define TMC_PAB2X 					0x01
#define TMC_PAB4X 					0x02
#define TMC_PCWCCW 					0x03

// Command output mode definition //
#define TMC_ONE_LOW_HIGH_LOW 			0x00
#define TMC_ONE_HIGH_HIGH_LOW 			0x01
#define TMC_ONE_LOW_LOW_HIGH 			0x02
#define TMC_ONE_HIGH_LOW_HIGH 			0x03
#define TMC_TWO_CW_CCW_LOW 				0x04
#define TMC_TWO_CW_CCW_HIGH 			0x05
#define TMC_TWO_CCW_CW_LOW 				0x06
#define TMC_TWO_CCW_CW_HIGH 			0x07
#define TMC_TWO_PHASE 					0x08
#define TMC_TWO_PHASE_RESERVE           0x09 

// motion type definition //	
#define TMC_STAND 						0x00 /*steady*/
#define TMC_RUNNING 					0x01 /*running*/

// motion type definition //	
#define TMC_FORWARD						0/*forward*/
#define TMC_RESERVE 					1 /*reserved*/

#define TMC_DIR_P                     	0  //Positive Direction
#define TMC_DIR_N                       1  //Negative Direction

// Position Move type definition //
#define TMC_INC                        0  //Incremental Position 
#define TMC_ABS                        1  //Absolute Position

// Arc operation direction //
#define TMC_CW						   0  //Clockwise Direction         ( Angle Decreases  < 0)
#define TMC_CCW						   1  //Counter Clockwise Direction ( Angle Increases  > 0)

// Counter name //
#define TMC_COMM					0 /*Command*/
#define TMC_ACT						1 /*Feedback*/
#define TMC_DEV						2 /*Deviation*/ 
#define TMC_GEN						3 /*General*/
#define TMC_CURVEL					4 /*현재속도*/

// speed profile type definition //
#define TMC_PROF_C					   0   //Constant Speed Profile
#define TMC_PROF_T                    1  //Trapezoidal Speed Profile
#define TMC_PROF_S                    2  //S-Curve Speed Profile

// sta method definition //	
#define TMC_STA_SOFTWARE					    0 /*Software */
#define TMC_STA_HARDWARE					1 /* Hardware*/

// sta action definition //	
#define TMC_ACT_LEVEL					0  /* Level Trigger */
#define TMC_ACT_FALL					1  /* Edge Trigger*/

// Compare Method //
#define TMC_EQ_PDIR						0  // CmpData = CmpSrc_Value ( while counting up )
#define TMC_EQ_NDIR						1  // CmpData = CmpSrc_Value ( while counting down )

// Compare Type //
#define TMC_CMP_IDX0					0  /*compare[0]*/
#define TMC_CMP_IDX1					1  /*compare[1]*/
#define TMC_CMP_IDX2					2  /*compare[2]*/
#define TMC_CMP_IDX3					3  /*compare[3]*/

// Gantry Type //
#define TMC_GANT_IDX0					0  /*Gantry[0]*/
#define TMC_GANT_IDX1					1  /*Gantry[1]*/
#define TMC_GANT_IDX2					2  /*Gantry[2]*/
#define TMC_GANT_IDX3					3  /*Gantry[3]*/

// Method when general comparator met the condition //
#define TMC_MTH_NONE					0  // 조건 성립 하지 않음
#define TMC_MTH_EQ_DIR				    1  // Counter 방향과 무관
#define TMC_MTH_EQ_PDIR				    2  // Counter Up 중
#define TMC_MTH_EQ_NDIR				    3  // Counter Down 중

// Action when general comparator met the condition //
#define TMC_EVT_ONLY					0  // 처리하지 않음 (인터럽트만 발생)
#define TMC_EVT_ESTOP					1  // 정지
#define TMC_EVT_STOP					2  // 감속후 정지
#define TMC_EVT_SPDCHG				    3  // 속도변경


// Backlash/Slip correction mode //
#define  TMC_CORR_DIS					0 // Disable correction 
#define  TMC_CORR_BACK				    1 // Backlash correction mode 
#define  TMC_CORR_SLIP				    2 // Slip correction mode

//position trigger output level
#define  TMC_RISING					    0
#define  TMC_FALLING					1


// Interrupt Handler Type //
#define  TMC_IHT_MESSAGE				0
#define  TMC_IHT_CALLBACK				1
#define  TMC_IHT_EVENT 			        2

// SetFilterAB의 대상 //
#define  TMC_ENC_AB				      	0
#define  TMC_PULSAR_AB			        1

#define  TMC_HOME_POS_RST0		        0 	// ORG(/EL/EZ) 신호가 발생할 때 COMMAND & FEEDBACK 위치를 0으로 클리어한다.
#define  TMC_HOME_POS_RST1			1	// 원점복귀를 모두 완료하고 나서 COMMAND & FEEDBACK 위치를 모두 0으로 클리어한다.
#define  TMC_HOME_POS_RST2			2	// 원점복귀를 모두 완료하고 나서 FEEDBACK 위치는 그대로 두고 COMMAND 위치를 FEEDBACK 위치에 일치시킨다.

// Sync mode //
#define  TMC_SYNC_DISABLE             0
#define  TMC_SYNC_INT_SYNC            1
#define  TMC_SYNC_OTHER_STOP          2

// Internal sync. conditions //
#define  TMC_ISYNC_ACC_STA			0	// 0: at start of acceleration
#define  TMC_ISYNC_ACC_END			1	// 1: at end of acceleration
#define  TMC_ISYNC_DEC_STA			2	// 2: at start of deceleration
#define  TMC_ISYNC_DEC_END			3	// 3: at end of deceleration
#define  TMC_ISYNC_SLN				4	// 4: when (-)software limit met
#define  TMC_ISYNC_SLP				5	// 5: when (+)software limit met
#define  TMC_ISYNC_GCMP				6	// 6: when General Comparator condition is satisfied
#define  TMC_ISYNC_TCMP				7	// 7: when Trigger Comparator condition is satisfied


//Coordinate Systems
#define TMC_MAX_CS                     4 //Maximum Number of Coordinate Systems

//Coordinate System No.
#define TMC_CS0                        0
#define TMC_CS1                        1
#define TMC_CS2                        2
#define TMC_CS3                        3

//Coordinate System Mask
#define TMC_CS0_MASK                   0x00000001
#define TMC_CS1_MASK                   0x00000002
#define TMC_CS2_MASK                   0x00000004
#define TMC_CS3_MASK                   0x00000008
#define TMC_CS4_MASK                   0x00000010
#define TMC_CS5_MASK                   0x00000020
#define TMC_CS6_MASK                   0x00000040
#define TMC_CS7_MASK                   0x00000080
#define TMC_CS8_MASK                   0x00000100
#define TMC_CS9_MASK                   0x00000200
#define TMC_CSA_MASK                   0x00000400
#define TMC_CSB_MASK                   0x00000800
#define TMC_CSC_MASK                   0x00001000
#define TMC_CSD_MASK                   0x00002000
#define TMC_CSE_MASK                   0x00004000
#define TMC_CSF_MASK                   0x00008000


// Internal sync. conditions //
#define  TMC_CNS_VECTOR              0   //0:Vector Vel
#define  TMC_CNS_MASTER	             1   //1:Long Axis Vel

// Function Log Level //
#define  TMC_DLOG_LEVEL0			 0   //LOG 출력 하지 않음
#define  TMC_DLOG_LEVEL1		     1   //에러만 발생한 경우 
#define  TMC_DLOG_LEVEL2		     2   //모션 관련 함수 + 설정 함수(SET)  
#define  TMC_DLOG_LEVEL3		     3   //LEVEL 2 + 반환 함수(SET) + 디지털 출력 함수(DO)
#define  TMC_DLOG_LEVEL4		     4   //LEVEL 3 + 디지털 입력 함수(DI) 
#define  TMC_DLOG_LEVEL5		     5   //전부

#define  TMC_COMM_SPEED2_5		     0  // 2.5Mbps 통신 속도  
#define  TMC_COMM_SPEED5		     1  // 5Mbps   통신 속도   
#define  TMC_COMM_SPEED10		     2  // 10Mbps  통신 속도  
#define  TMC_COMM_SPEED20		     3  // 25Mbps  통신 속도

//슬레이브 중에서 SB-MN16GPIO-M 제품 
#define  TMC_NONE_DIO               0   //  사용 안함
#define  TMC_ONLY_DI                1   //  입력 전용 
#define  TMC_ONLY_DO                2   //  출력 전용


/* Return code () */ 
/////////////////////////////////////////////////////////////////

#define TMC_RV_OK		      1   //성공


/////////////////////////////////////////////////////////////////
#define TMC_RV_MOT_INIT_ERR			  -100   //라이브러리 초기화 실패
#define TMC_RV_MOT_FILE_SAVE_ERR      -101   //모션 설정값을 저장하는 파일 저장에 실패함
#define TMC_RV_MOT_FILE_LOAD_ERR      -102   //모션 설정값이 저장된 파일이 로드가 안됨

/////////////////////////////////////////////////////////////////
#define TMC_RV_STOP_P_S_END_ERR        -200  //(+) 방향 소프트웨어 리미트에 의한 정지
#define TMC_RV_STOP_N_S_END_ERR        -201  //(-) 방향 소프트웨어 리미트에 의한 정지
#define TMC_RV_STOP_CMP3_ERR	       -202  //CMP3 에 의한 정지
#define TMC_RV_STOP_CMP4_ERR	       -203  //CMP4 에 의한 정지
#define TMC_RV_STOP_CMP5_ERR	       -204  //CMP5 에 의한 정지
#define TMC_RV_STOP_P_H_END_ERR        -205  //(+) 방향 하드웨어 리미트에 의한 정지
#define TMC_RV_STOP_N_H_END_ERR        -206  //(-) 방향 하드웨어 리미트에 의한 정지 
#define TMC_RV_STOP_ALM_ERR            -207  //서보 알람에 의한 정지
#define TMC_RV_STOP_CSTP_ERR           -208  //CSTP 신호에 의한 정지
#define TMC_RV_STOP_EMG_ERR            -209  //긴급 정지 신호에 의한 정지
#define TMC_RV_STOP_ESSD_ERR	       -210  //SD 입력 ON 에 의한 정지
#define TMC_RV_STOP_ESDT_ERR           -211  //보간 동작 DATA 이상에 의해 정지
#define TMC_RV_STOP_ESIP_ERR           -212  //보간 동작 중에 타축의 이상에 의해 정지
#define TMC_RV_STOP_ESPO_ERR           -213  //PA/PB 입력용 버퍼 의 오버플로워 발생에 의한 정지
#define TMC_RV_STOP_ESAO_ERR           -214  //보간 동작 때 위치결정용 카운터의 카운터 범위 OVER 발생에 의한 정지
#define TMC_RV_STOP_ESEE_ERR           -215  //EA/EB 입력 ERROR 발생 때(정지 하지 않음)
#define TMC_RV_STOP_ESPE_ERR           -216  //PA/PB 입력 ERROR 발생 때(정지 하지 않음)
#define TMC_RV_STOP_SYNC_ERR           -217  //SYNC 동작 중에 마스터 축과 편차 에 의해 정지
#define TMC_RV_STOP_GANT_ERR           -218  //GANT 동작 중에 마스터 축과 편차 에 의해 정지

/////////////////////////////////////////////////////////////////
#define TMC_RV_DRV_VER_ERR             -1000
#define TMC_RV_LOC_MEM_ERR             -1001    //메모리 생성 되지 실패
#define TMC_RV_GLB_MEM_ERR             -1002    //공유 메모리 생성 되지 실패
#define TMC_RV_HANDLE_ERR              -1003    //드바이스 핸들값이 에러
#define TMC_RV_CREATE_KERNEL_ERR       -1004    // 커널 드라이브 생성 에러
#define TMC_RV_CREATE_THREAD_ERR       -1005    //스레드 생성 에러
#define TMC_RV_CREATE_EVENT_ERR        -1006    //이벤트 생성 에러
#define TMC_RV_CREATE_FILE_ERR	       -1007    //파일 생성 에러

/////////////////////////////////////////////////////////////////
#define TMC_RV_CON_NOT_FOUND_ERR	 -1030  //CONTROLLER NOT FOUND 에러
#define TMC_RV_CON_NOT_LOAD_ERR		 -1031  //CONTROLLER LOAD 에러
#define TMC_RV_CON_DIP_SW_ERR		 -1032  //보드 ID 세팅 에러
#define TMC_RV_CON_MAX_ERR		     -1033  // CONTROLLER 최대 갯수 에러
#define TMC_RV_PCI_BUS_LINE_ERR		 -1034   //PCI 버스 데이타가 이상합니다.
#define TMC_RV_MOD_POS_ERR		     -1035   //모듈 순서가 잘못되었습니다.
#define TMC_RV_SUPPORT_PROCESS_ERR   -1036   // 지원하지 않은 프로세스
#define TMC_RV_SUPPORT_FUCTION_ERR   -1037   // 지원하지 않은 함수
#define TMC_RV_CON_OPEN_MODE_ERR     -1038   // 수동/자동 모드가 틀림
#define TMC_RV_CON_MODEL_TYPE_ERR    -1039   // 제어기 모델명이 틀림
#define TMC_RV_SUPPORT_HARDWARE_ERR  -1040   // 지원하지 않은 하드웨어
/////////////////////////////////////////////////////////////////
#define TMC_RV_PRM_LOAD_ERR             -1050  //파라미터 로드 에러
#define TMC_RV_PRM_VAL_ERR              -1051  //파라미터 값 에러
#define TMC_RV_PRM_FILENAME_ERR         -1052  //파라미터 파일이 존재하지 않음
#define TMC_RV_PRM_FILENAME_DIFF_ERR    -1053  //이미 오픈된 파라미터 파일과 동일하지 않음 
#define TMC_RV_PRM_OPEN_FILENAME_ERR    -1054  //이미 오픈된 파라미터 파일이 존재
/////////////////////////////////////////////////////////////////
#define TMC_RV_NOT_SPT_ERR            -1100 // 모델에서 지원하지 않는 기능
#define TMC_RV_DIV_BY_ZERO_ERR        -1101 // DIVIDE BY ZERO 에러
#define TMC_RV_TIME_OUT_ERR           -1102 // TIME OUT 에러
#define TMC_RV_WM_QUIT_ERR            -1103 // WM_QUIT 발생 에러
#define TMC_RV_CON_NO_ERR             -1120 // 해당 카드번호가 존재하지 않음
#define TMC_RV_AXIS_NO_ERR            -1121 // 축 번호 에러
#define TMC_RV_MASTER_AXIS_NO_ERR     -1122 // 마스터축 번호 에러 
#define TMC_RV_SLAVE_AXIS_NO_ERR      -1123 // 슬레이브축 번호 에러
#define TMC_RV_COORD_NO_ERR           -1124 // COORDINATE 번호 에러
#define TMC_RV_ARG_RNG_ERR            -1125 // 함수 인자 범위 에러
#define TMC_RV_CS_AXIS_ERR            -1126 // COORDINATE AXIS 에러
#define TMC_RV_INT_CFG_ERR            -1127 // 인터럽트 설정 에러
#define TMC_RV_COORD_AXES_ERR         -1128 // COORDINATE AXIS 갯수 에러
#define TMC_RV_CONT_AXES_ERR          -1129 // 연속 모션  AXIS 갯수 에러

/////////////////////////////////////////////////////////////////
#define TMC_RV_GROUP_RNG_ERR          -1130 // Group 범위 에러
#define TMC_RV_AXES_MIN_ERR           -1131 // AXIS 최소 갯수 에러
#define TMC_RV_AXES_MAX_ERR           -1132 // AXIS 최대 갯수  에러

/////////////////////////////////////////////////////////////////

#define TMC_RV_MTN_IN_STOP_ERR        -1140  // 모션 구동중이어야 되는데 모션 구동중이 아닐 때 에러
#define TMC_RV_MTN_DRV_ERR            -1141  // 모션 정지중이어야 되는데 모션 구동중 일 때 에러
#define TMC_RV_HOME_BUSY_ERR          -1142  // ORG SEARCH 중인 상태
#define TMC_RV_DRV_STEADY_ERR         -1143  // 정지 상태
#define TMC_RV_DRV_PAB_ERR			  -1144  // 수동펄스 시작중일때 모션 명령 실행 때 에러
#define TMC_RV_DRV_MODIFY_POS_ERR	  -1145  // 모션 에러가 발생 또는 이미 이송이 완료되어 위치 오버라이드가 적용되지 않음
#define TMC_RV_PRE_POS_ERR       	  -1146  // 프리버퍼 데이타 에러
#define TMC_RV_PRE_FULL_ERR       	  -1147  // 프리버퍼 데이타 풀 상태 에러
/////////////////////////////////////////////////////////////////
#define TMC_RV_MIN_VEL_ERR		      -1150   //  MIN VEL VALUE IS UNDER VALID 
#define TMC_RV_MAX_VEL_ERR			  -1151   //  MAX VEL VALUE IS OVER VALID 
#define TMC_RV_MIN_STARTV_ERR         -1152   //  START VEL VALUE IS UNDER VALID SV
#define TMC_RV_MAX_STARTV_ERR         -1153   //  START VEL VALUE IS OVER VALID SV
#define TMC_RV_MIN_WORKV_ERR          -1154   //  WORK  VEL VALUE IS UNDER VALID WV
#define TMC_RV_MAX_WORKV_ERR          -1155   //  WORK  VEL IS OVER VALID WV
#define TMC_RV_MIN_ACC_ERR            -1156   //  ACC TIME IS UNDER VALID AC
#define TMC_RV_MAX_ACC_ERR            -1157   //  ACC TIME IS OVER VALID AC
#define TMC_RV_MIN_DEC_ERR            -1158   //  DEC TIME IS UNDER VALID AC
#define TMC_RV_MAX_DEC_ERR            -1159   //  DEC TIME IS OVER VALID AC

/////////////////////////////////////////////////////////////////
#define TMC_RV_MIN_DISTANCE_ERR        -1160   //  이동 거리가 최소값 이상
#define TMC_RV_MAX_DISTANCE_ERR        -1161   //  이동 거리가 최대값 이상

/////////////////////////////////////////////////////////////////
#define TMC_RV_DLOG_ERR               -1180   // FUNCTION 로그 에러
#define TMC_RV_DLOG_LEVEL_ERR         -1181   // FUNCTION 로그 레벨 에러
#define TMC_RV_DLOG_FILENAME_ERR      -1182   // FUNCTION 로그 파일 에러
/////////////////////////////////////////////////////////////////
#define TMC_RV_CMP_CONNECT_ERR        -1200   //CMP 연결 동작 에러

/////////////////////////////////////////////////////////////////
#define TMC_RV_STATION_COMMUNICATION_ERR  -2000   //슬레이브 연결 에러
#define TMC_RV_STATION_INFO_ERR			  -2001   //슬레이브 정보 획득 에러
#define TMC_RV_CONNECT_CYCLIC_ERR		  -2002   //사이클릭 통신 시작 에러 또는 Off 상태
#define TMC_RV_STATION_NO_ERR			  -2003   //슬레이브 번호 에러
#define TMC_RV_CONNECT_SYS_ERR			  -2004   //시스템 통신 시작 에러	// Gee 2014.08.19
#define TMC_RV_COMM_RESET_ERR			  -2005   //모든 통신 리셋 에러		// Gee 2014.08.19
#define TMC_RV_SLAVE_ZERO_ERR			  -2006   //사용하는 슬레이브가 0으로 사이클릭 통신 사작	// Gee 2014.08.19
#define TMC_RV_GPIO_ERR					  -2007   //GPIO 설정으로 지원하지 않는 함수 에러
#define TMC_RV_DATA_TX_ERR				  -2008   //데이터 통신 실패 에러
#define TMC_RV_TX_CLR_ERR				  -2009   //수신처리 완료 인터럽트 클리어 에러
#define TMC_RV_STATION_TYPE_ERR			  -2010   //슬레이브 모듈의 종류가 함수와 맞지 않음
#define TMC_RV_MULTI_AXIS_ERR			  -2011   //멀티무브나 보간 이송시 같은 슬레이브 내의 축이 아님
#define TMC_RV_CYCLIC_COMM_ERR			  -2012   //사이클릭 통신 중 통신 에러 발생
#define TMC_RV_CONFIG_MISMATCHING_ERR	  -2013   //현재 슬레이브 모듈 구성이 저장된 정보와 일치하지 않음
#define TMC_RV_SYSCOMM_BEGIN_ERR		  -2014   //사이클릭 통신 중인데 슬레이브 모듈 정보 갱신 실행 에러
#define TMC_RV_DATA_RX_ERR				  -2015   //데이터 통신 실패 에러
#define TMC_RV_TX_CYCLIC_ERR			  -2016   //Cyclic 통신 에러로 데이터 전송 실패
#define TMC_RV_TX_CPU_ACCESS_ERR		  -2017   //슬레이브 CPU 에러로 데이터 전송 실패
#define TMC_RV_TX_STATION_RECEIVE_ERR	  -2018   //슬레이브 수신 에러로 데이터 전송 실패
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#define TMC_RV_UNKNOWN_ERR            -9999  // 알수 없는 에러




#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of TMC_MAPI_DEFS_H definition

//===========================================================================
// End of file.
//===========================================================================

