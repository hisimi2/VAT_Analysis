/******************************************************************************
*
*	File Version: 3,0,1,0
*
*	Copyright(c) 2012 Alpha Motion Co,. Ltd. All Rights Reserved.
*
*	This file is strictly confidential and do not distribute it outside.
*
*	MODULE NAME :
*		pmiMApi.h
*
*	AUTHOR :
*		K.C. Lee
*
*	DESCRIPTION :
*		the header file for RC files of project.
*
*
* - Phone: +82-31-303-5050
* - Fax  : +82-31-303-5051
* - URL  : http://www.alphamotion.co.kr,
*
*
/****************************************************************************/

#ifndef NMI_MN_API_H
#define NMI_MN_API_H


//***********************************************************************************************
//				STRUCTURE for User												*
//***********************************************************************************************
// Informations of a motion device //
typedef struct
{
	//===========================================
	HANDLE	   hDevice;
	int        uiDeviceId;
	int        uiDeviceNo;
	int	       uiSlotNo;
	int	       uiFwVer;
	int	       uiHwVer;
	int	       uiApiVer;
	int        uiAxesNum;
	int        uiInitAxis;
	int        uiDiNumNo;
	int        uiDiInitCh;
	int        uiDoNumNo;
	int        uiDoInitCh;
	int        uiDeviceSpec;
	int		   uiIoNumSlave;	// 2015.10.07 추가
	int		   uiMotNumSlave;	// 2015.10.07 추가

} TMotInfoMap;

// Informations of motion devices defined by user 
typedef struct{
	//===========================================
	int       uiNumDevs;  // Number of user motion devices
	//===========================================
	int       uiNumAllAxis;
	int       uiNumAllDiCh;
	int       uiNumAllDoCh;
	
	TMotInfoMap taMapInfo[16];
	
}TMotDevInfoMap;


#ifdef __cplusplus
extern "C" {
#endif



#ifndef PMI_DLL_EXP
	#define PMI_DLL_EXP      _declspec(dllexport)
#endif

//====================== System Management Function ====================================================//
//bManual = TRUE, Con Number is set to dip switch(Default)
//npNumCons In a computer-set number of board
PMI_DLL_EXP int WINAPI nmiSysLoad					(/*[in]*/int bManual,/*[out]*/int *npNumCons );

PMI_DLL_EXP int WINAPI nmiSysUnload					(/*[none]*/ void);

// 통시 초기화
PMI_DLL_EXP int WINAPI nmiSysComm				(/*[in]*/ int nCon);
PMI_DLL_EXP int WINAPI nmiReset					(/*[in]*/ int nCon);
PMI_DLL_EXP int WINAPI nmiConInit				(/*[in]*/ int nCon);

// 사이클릭 통신
PMI_DLL_EXP int WINAPI nmiCyclicBegin			(/*[in]*/ int nCon);
PMI_DLL_EXP int WINAPI nmiCyclicEnd				(/*[in]*/ int nCon);

// 통신 속도
//0: 2.5Mbps, 1: 5Mbps, 2: 10Mbps, 3: 20Mbps
PMI_DLL_EXP int WINAPI nmiSetCommSpeed			(/*[in]*/ int nCon, /*[in]*/ int nCommSpeed);
PMI_DLL_EXP int WINAPI nmiGetCommSpeed			(/*[in]*/ int nCon, /*[out]*/ int *npCommSpeed);

//====================== Motion Parameter Management Function ====================================================//
//파라미터 파일 읽어오기(.PRM)
PMI_DLL_EXP int WINAPI nmiConParamLoad				(/*[none]*/ void);

//파라미터 파일 저장하기(.PRM)
PMI_DLL_EXP int WINAPI nmiConParamSave				(/*[none]*/ void);

//======================  Motion interface I/O Configure and Control Function ==============================================//
//nState=0, emSERVO_OFF.
//nState=1, emSERVO_ON.
PMI_DLL_EXP int WINAPI nmiAxSetServoOn				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nState );
PMI_DLL_EXP int WINAPI nmiAxGetServoOn				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npState );

//nState=0, reset.
//nState=1, set.
PMI_DLL_EXP int WINAPI nmiAxSetServoReset			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nState );
PMI_DLL_EXP int WINAPI nmiAxGetServoReset			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npState );

//inp_logic=0, active LOW.(Default)
//inp_logic=1, active HIGH.
PMI_DLL_EXP int WINAPI nmiAxSetServoInpLevel		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel);
PMI_DLL_EXP int WINAPI nmiAxGetServoInpLevel		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel);

//inp_enable=0, Disabled (Default)
//inp_enable=1, Enabled
PMI_DLL_EXP int WINAPI nmiAxSetServoInpEnable       (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nEnable);
PMI_DLL_EXP int WINAPI nmiAxGetServoInpEnable       (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npEnable);

PMI_DLL_EXP int WINAPI nmiAxGetServoInp		        (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nInp);

//alm_logic=0, active LOW. (Default)
//alm_logic=1, active HIGH.
PMI_DLL_EXP int WINAPI nmiAxSetServoAlarmLevel		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel);
PMI_DLL_EXP int WINAPI nmiAxGetServoAlarmLevel		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel);

//alm_mode=0, motor immediately stops(Default)
//alm_mode=1, motor decelerates then stops
PMI_DLL_EXP int WINAPI nmiAxSetServoAlarmAction     (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nAction);
PMI_DLL_EXP int WINAPI nmiAxGetServoAlarmAction     (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npAction);

//el_logic=0, active LOW.(Default)
//el_logic=1, active HIGH.
PMI_DLL_EXP int WINAPI nmiAxSetLimitLevel		    (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel);
PMI_DLL_EXP int WINAPI nmiAxGetLimitLevel		    (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel);

//el_act=0, motor immediately stops.(Default)
//el_act=1, motor decelerates then stops.
PMI_DLL_EXP int WINAPI nmiAxSetLimitAction          (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nAction);
PMI_DLL_EXP int WINAPI nmiAxGetLimitAction          (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npAction);

//org_logic=0, active LOW.(Default)
//org_logic=1, active HIGH.
PMI_DLL_EXP int WINAPI nmiAxSetOrgLevel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel);
PMI_DLL_EXP int WINAPI nmiAxGetOrgLevel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel);

//CPhase_logic=0, active LOW.(Default)
//CPhase_logic=1, active HIGH.
PMI_DLL_EXP int WINAPI nmiAxSetEzLevel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel);
PMI_DLL_EXP int WINAPI nmiAxGetEzLevel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel);

//soft limit of positive direction -134,217,728 <= P <= 134,217,727
//soft limit of negative direction -134,217,728 <= N <= 134,217,727
PMI_DLL_EXP int WINAPI nmiAxSetSoftLimitPos			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dLimitP,/*[in]*/  double  dLimitN );
PMI_DLL_EXP int WINAPI nmiAxGetSoftLimitPos			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpLimitP,/*[out]*/ double *dpLimitN );

//Action: The reacting method of soft limit
//Action =0, INT
//Action =1, Immediately stop
//Action =2, slow down then stop
//Action =3, Change Velocity
PMI_DLL_EXP int WINAPI nmiAxSetSoftLimitAction		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nAction );
PMI_DLL_EXP int WINAPI nmiAxGetSoftLimitAction		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npAction );

//0 softlimit disale (default)
//1 softlimit enable
PMI_DLL_EXP int WINAPI nmiAxSetSoftLimitEnable		(/*[in]*/ int nCon,/*[in]*/ int nAxis, /*[in]*/int nEnable );
PMI_DLL_EXP int WINAPI nmiAxGetSoftLimitEnable		(/*[in]*/ int nCon,/*[in]*/ int nAxis, /*[out]*/int *npEnable );

//limit pos ( 0 <= ResetPos <= 134217727)
PMI_DLL_EXP int WINAPI nmiAxSetRCountResetPos		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dPos );
PMI_DLL_EXP int WINAPI nmiAxGetRCountResetPos		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpPos );

//0 ring counter(Position RollOver) disale (default)
//1 ring counter(Position RollOver) enable
PMI_DLL_EXP int WINAPI nmiAxSetRCountEnable			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nEnable );
PMI_DLL_EXP int WINAPI nmiAxGetRCountEnable			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/  int *npEnable );

//erc_logic=0, active LOW.(Default)
//erc_logic=1, active HIGH
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetServoCrcLevel		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel );
PMI_DLL_EXP int WINAPI nmiAxGetServoCrcLevel		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel );

//crc_on_time=0 12us
//crc_on_time=1 102us
//crc_on_time=2 409us
//crc_on_time=3 1.6ms
//crc_on_time=4 13ms
//crc_on_time=5 52ms
//crc_on_time=6 104ms (Default)
//crc_on_time=7 erc level out
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetServoCrcTime			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nOnTime );
PMI_DLL_EXP int WINAPI nmiAxGetServoCrcTime			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npOnTime );

//Automatically outputs an CRC signal when the axis is stopped immediately by a +EL,-EL, ALM, or #CEMG input signal. However, the CRC signal is not output when a
//deceleration stop occurs on the axis. 
//Even if the EL signal is specified for a normal stop, by setting MOD = "010X000" (feed to the EL position) in the RMD register, 
//the CRC signal is output if an immediate stop occurs
//crc_enable=0, manual error counter clear output 
//crc_enable=1, automatic error counter clear output (Default)
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetServoCrcEnable       (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nEnable);
PMI_DLL_EXP int WINAPI nmiAxGetServoCrcEnable       (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npEnable);

//crc signal active command 
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetServoCrcOn	        (/*[in]*/ int nCon,/*[in]*/ int nAxis);

//crc signal deactive command 
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetServoCrcReset        (/*[in]*/ int nCon,/*[in]*/ int nAxis);

//Reset COUNTER when the CLR input turns OFF->ON
//Action = 0, Falling edge clear
//Action = 1, Rising edge clear
//Action = 2, Low Level clear
//Action = 3, Hi Level clear
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetCountAction			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nAction );
PMI_DLL_EXP int WINAPI nmiAxGetCountAction			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npAction );

//Reset COUNTER when the CLR input turns OFF->ON
//enable=0, Disabled (Default)
//enable=1, Enabled
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetCountReset			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nEnable );
PMI_DLL_EXP int WINAPI nmiAxGetCountReset			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npEnable );


//sd_logic=0, active LOW.(Default)
//sd_logic=1, active HIGH.
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetSdLevel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLevel );
PMI_DLL_EXP int WINAPI nmiAxGetSdLevel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLevel );

//sd_mode=0, slow down only(Default)
//sd_mode=1, slow down then stop
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetSdAction				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nStop );
PMI_DLL_EXP int WINAPI nmiAxGetSdAction				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npStop );

// SDLT Specify the latch function of the SD input. (0: OFF. 1: ON.)
//sd_latch=0, do not latch.(Default)
//sd_latch=1, latch.
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetSdLatch				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nLatch );
PMI_DLL_EXP int WINAPI nmiAxGetSdLatch				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npLatch );
//Decelerates (deceleration stop) by turning ON the input
////Connected to DI[0]
//enable=0, Disabled (Default)
//enable=1, Enabled
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetSdEnable				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nEnable );
PMI_DLL_EXP int WINAPI nmiAxGetSdEnable				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npEnable );

//target=0, PA/PB.
//target=1, EA/EB/EZ.
//enable=0.
//enable=1.
PMI_DLL_EXP int WINAPI nmiAxSetFilterABEnable		 (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nTarget,/*[in]*/  int  nEnable );
PMI_DLL_EXP int WINAPI nmiAxGetFilterABEnable		 (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nTarget,/*[out]*/ int *npEnable );

//Command pulse signal output mode
//0 OUT/DIR OUT Falling edge, DIR+ is high level
//1 OUT/DIR OUT Rising edge,  DIR+ is high level
//2 OUT/DIR OUT Falling edge, DIR+ is low level
//3 OUT/DIR OUT Rising edge,  DIR+ is low level
//4 CW/CCW Falling edge(Default)
//5 CW/CCW Rising edge
//6 CW/CCW Falling edge Inverse
//7 CW/CCW Rising  edge Inverse
//8 Two Phase Mode Pulse Phase lead than Dir
//9 Two Phase Mode Dir   Phase lead than Pulse
PMI_DLL_EXP int WINAPI nmiAxSetPulseType			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nType );
PMI_DLL_EXP int WINAPI nmiAxGetPulseType			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npType );

//Encoder pulse input signal mode
//0x00 1X A/B
//0x01 2X A/B
//0x02 4X A/B(Default)
//0x03 CW/CCW
PMI_DLL_EXP int WINAPI nmiAxSetEncType				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nType );
PMI_DLL_EXP int WINAPI nmiAxGetEncType				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npType );

//Feedback counter UP / DOWN direction is opposite
//0 normal counting(Default)
//1 reverse counting
PMI_DLL_EXP int WINAPI nmiAxSetEncDir				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nDir  );
PMI_DLL_EXP int WINAPI nmiAxGetEncDir				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npDir  );

//0 Command pulse(Default) 
//1 External Feedback     
PMI_DLL_EXP int WINAPI nmiAxSetEncCount				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nCount );
PMI_DLL_EXP int WINAPI nmiAxGetEncCount				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npCount );


//Motion setting the Velocity limit
//0 < dVel <= 6553500 ) [pps]
PMI_DLL_EXP int WINAPI nmiAxSetMaxVel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dVel );
PMI_DLL_EXP int WINAPI nmiAxGetMaxVel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/  double *dpVel );

//Set the jOG start velocity
//0 <= dVel <= 6553500 ) [pps]
PMI_DLL_EXP int WINAPI nmiAxSetInitJogVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dVel );  
PMI_DLL_EXP int WINAPI nmiAxGetInitJogVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/  double *dpVel );

//Set the POS start velocity
//0 <= dVel <= 6553500 ) [pps]
PMI_DLL_EXP int WINAPI nmiAxSetInitVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dVel );
PMI_DLL_EXP int WINAPI nmiAxGetInitVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/  double *dpVel );

//Jog Motion transfer Velocity setting standards
//nType = 0 Constant
//nType = 1 T-Curve
//nType = 2 S-Curve
//0 <= dVel <= 6553500 [pps]
//0 <= Tacc < 60000  [ms]
PMI_DLL_EXP int WINAPI nmiAxSetJogVelProf			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nProfileType, /*[in]*/  double  dVel,/*[in]*/  double  dTacc );
PMI_DLL_EXP int WINAPI nmiAxGetJogVelProf			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nProfileType,/*[out]*/ double *dpVel,/*[out]*/ double *dpTacc );

//Motion transfer Velocity setting standards
//nType = 0 Constant
//nType = 1 T-Curve
//nType = 2 S-Curve
//0 <= dVel <= 6553500 [pps]
//0 <= Tacc < 60000  [ms]
//0 <= Tdec < 60000  [ms]
PMI_DLL_EXP int WINAPI nmiAxSetVelProf				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nProfileType, /*[in]*/  double  dVel,/*[in]*/  double  dTacc,/*[in]*/  double  dTdec );
PMI_DLL_EXP int WINAPI nmiAxGetVelProf				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nProfileType,/*[out]*/ double *dpVel,/*[out]*/ double *dpTacc,/*[out]*/ double *dpTdec );

//Set the deceleration start point detection method
//nType = 0 AutoDetect
//nType = 1 ManulDetect
PMI_DLL_EXP int WINAPI nmiAxSetDecelType			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/   int  nType );
PMI_DLL_EXP int WINAPI nmiAxGetDecelType			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/  int *npType );

//The ramping down point is the position where deceleration starts. 
//The data is stored as pulse count, and it cause the axis start to decelerate when remaining pulse count reach the data.
//-8388608 <= dPul <= 8388607
PMI_DLL_EXP int WINAPI nmiAxSetRemainPul			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/   double  dPulse );
PMI_DLL_EXP int WINAPI nmiAxGetRemainPul			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/  double *dpPulse );

//+el.-el.sd.org. alm.inp
//+el.-el.sd.org. alm.inp
//TMC-BAxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxSetFilterEnable			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nEnable);
PMI_DLL_EXP int WINAPI nmiAxGetFilterEnable			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npEnable );

//====================== HOME-RETURN FUNCTIONS ================================================//

PMI_DLL_EXP int WINAPI nmiAxHomeSetResetPos			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nResetPos );
PMI_DLL_EXP int WINAPI nmiAxHomeGetResetPos			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npResetPos );

//Automatically outputs an CRC signal when the axis is stopped immediately by a ORG input signal. However, the CRC signal is not output when a
//deceleration stop occurs on the axis. 
//Even if the EL signal is specified for a normal stop, by setting MOD = "010X000" (feed to the EL position) in the RMD register, 
//the CRC signal is output if an immediate stop occurs
//crc_enable=0
//crc_enable=1
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiAxHomeSetCrcEnable(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nEnable );
PMI_DLL_EXP int WINAPI nmiAxHomeGetCrcEnable(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int* npEnable );

//Home search mode setting one axis
//TYPE=0 ORG ON -> Slow down -> Stop 
//TYPE=1 ORG ON -> Stop -> Go back(Rev Spd) -> ORG OFF -> Go forward(Rev Spd) -> ORG ON -> Stop(Default)
//TYPE=2 ORG ON -> Slow down(Low Spd) -> Stop on EZ signal
//TYPE=3 ORG ON -> EZ signal -> Slow down -> Stop
//TYPE=4 ORG ON -> Stop -> Go back(Rev Spd) -> ORG OFF -> Stop on EZ signal
//TYPE=5 ORG ON -> Stop -> Go back(High Spd) -> ORG OFF -> EZ signal -> Slow down -> Stop
//TYPE=6 EL ON -> Stop -> Go back(Rev Spd) -> EL OFF -> Stop
//TYPE=7 EL ON -> Stop -> Go back(Rev Spd) -> EL OFF -> Stop on EZ signal
//TYPE=8 EL ON -> Stop -> Go back(High Spd) -> EL OFF -> Stop on EZ signal
//TYPE=9 ORG ON -> Slow down -> Stop -> Go back -> Stop at beginning edge of ORG
//TYPE=10 ORG ON -> EZ signal -> Slow down -> Stop -> Go back -> Stop at beginning edge of EZ;
//TYPE=11 ORG ON -> Slow down -> Stop -> Go back (High Spd) -> ORG OFF -> EZ signal -> Slow down -> Stop -> Go forward(High Spd) -> Stop at beginning edge of EZ
//TYPE=12 EL ON -> Stop -> Go back (High Spd) -> EL OFF -> EZ signal -> Slow down -> Stop -> Go forward(High Spd) -> Stop at beginning edge of EZ
//TYPE=13 EZ signal -> Slow down -> Stop
PMI_DLL_EXP int WINAPI nmiAxHomeSetType				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nType );
PMI_DLL_EXP int WINAPI nmiAxHomeGetType				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npType );

//Home search direction of one axis
//home_dir = 0, CW direction (Default)
//home_dir = 1, CCW direction
PMI_DLL_EXP int WINAPI nmiAxHomeSetDir				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nDir  );
PMI_DLL_EXP int WINAPI nmiAxHomeGetDir				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npDir  );

//The origin of the origin of the behavior to escape from the distance sensor
//pps ( -134217728 ~ 134217727 )
PMI_DLL_EXP int WINAPI nmiAxHomeSetEscapePul		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dEscape );
PMI_DLL_EXP int WINAPI nmiAxHomeGetEscapePul		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpEscape );

//EZ Count
//nEzCount(1 ~ 16)
PMI_DLL_EXP int WINAPI nmiAxHomeSetEzCount			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nEzCnt );
PMI_DLL_EXP int WINAPI nmiAxHomeGetEzCount			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npEzCnt );

//Driving home from the relative position of one axis to complete the search
//pps ( -134217728 ~ 134217727 )
PMI_DLL_EXP int WINAPI nmiAxHomeSetShiftDist		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dShift );
PMI_DLL_EXP int WINAPI nmiAxHomeGetShiftDist		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpShift );

//pps ( 0 ~ 6553500 )
PMI_DLL_EXP int WINAPI nmiAxHomeSetInitVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ double  dVel );
PMI_DLL_EXP int WINAPI nmiAxHomeGetInitVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpVel );


//Home search drive Vel setting one axis
//nType=0 Constant
//nType=1 T-Curve (Default)
//nType=2 S-Curve
//dVel    pps ( 0 ~ 6553500 )
//dRevVel pps ( 0 ~ 6553500 )
//0 <= Tacc < 60000  [ms]
PMI_DLL_EXP int WINAPI nmiAxHomeSetVelProf		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nProfileType,/*[in]*/ double  dVel, /*[in]*/  double dRevVel,  /*[in]*/  double  dTacc);
PMI_DLL_EXP int WINAPI nmiAxHomeGetVelProf		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nProfileType,/*[out]*/ double *dpVel, /*[out]*/ double *dpRevVel, /*[out]*/ double *dpTacc);

PMI_DLL_EXP int WINAPI nmiAxHomeSetVelProfEx		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nProfileType,/*[in]*/ double  dVel, /*[in]*/  double dRevVel,  /*[in]*/  double  dTacc, /*[in]*/  double  dTdec);
PMI_DLL_EXP int WINAPI nmiAxHomeGetVelProfEx		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nProfileType,/*[out]*/ double *dpVel, /*[out]*/ double *dpRevVel, /*[out]*/ double *dpTacc, /*[out]*/ double *dpTdec);


//One axis driving home search
PMI_DLL_EXP int WINAPI nmiAxHomeMove				(/*[in]*/ int nCon,/*[in]*/ int nAxis );

//Multi axis driving home search
PMI_DLL_EXP int WINAPI nmiMxHomeMove				(/*[in]*/ int nCon,/*[in]*/ int nNAxis,/*[in]*/ int naAxis[] );

//Home search check one axis driving
//0 stop
//1 Home search driving
PMI_DLL_EXP int WINAPI nmiAxHomeCheckDone			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nDone );

//Home search has been completed successfully one axis
//0 Home search incomplete one axis
//1 Home search completed one axis
PMI_DLL_EXP int WINAPI nmiAxHomeSetStatus			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nStatus );
PMI_DLL_EXP int WINAPI nmiAxHomeGetStatus			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *nStatus );

//======================  Velocity mode And Single Axis Position Motion Configure  ==============================================//

//Driving one axis continuous operation
//#define PMI_DIR_P                      0  //Positive Direction
//#define PMI_DIR_N                      1  //Negative Direction
PMI_DLL_EXP int WINAPI nmiAxJogMove					(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nDir );

//One axis transported to a specified location
//nAbsMode = 0 Driving the relative coordinates
//nAbsMode = 1 Driving the absolute coordinate
//-134217728 <= pos <= 134217727
PMI_DLL_EXP int WINAPI nmiAxPosMove				    (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nAbsMode,/*[in]*/ double dPos );

//nDone = 0 stop
//nDone = 1 driving
PMI_DLL_EXP int WINAPI nmiAxCheckDone				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npDone );

//One axis deceleration stop
PMI_DLL_EXP int WINAPI nmiAxStop					(/*[in]*/ int nCon,/*[in]*/ int nAxis );

//One axis emergency stop
PMI_DLL_EXP int WINAPI nmiAxEStop					(/*[in]*/ int nCon,/*[in]*/ int nAxis );

//======================  Velocity mode And Multi Axis Point to Point Motion Configure  ==============================================//
//Driving multi axis continuous operation
//#define PMI_DIR_P                      0  //Positive Direction
//#define PMI_DIR_N                      1  //Negative Direction
PMI_DLL_EXP int WINAPI nmiMxJogMove					(/*[in]*/ int nCon,/*[in]*/ int nNAxis,/*[in]*/ int naAxis[],/*[in]*/ int naDir[]);

//Multi axis transported to a specified location
//0 Driving the relative coordinates
//1 Driving the absolute coordinate
//-134217728 <= pos <= 134217727
PMI_DLL_EXP int WINAPI nmiMxPosMove					(/*[in]*/ int nCon,/*[in]*/ int nNAxis,/*[in]*/ int nAbsMode,/*[in]*/ int naAxis[],/*[in]*/ double faDist[]);

//motion check multi axis driving
//0 stop
//1 driving
PMI_DLL_EXP int WINAPI nmiMxCheckDone				(/*[in]*/ int nCon,/*[in]*/ int nNAxis,/*[in]*/ int naAxis[],/*[out]*/ int *npDone);

//Multi axis deceleration stop
PMI_DLL_EXP int WINAPI nmiMxStop					(/*[in]*/ int nCon,/*[in]*/ int nNAxis,/*[in]*/ int naAxis[]);

//Multi axis emergency stop
PMI_DLL_EXP int WINAPI nmiMxEStop					(/*[in]*/ int nCon,/*[in]*/ int nNAxis,/*[in]*/ int naAxis[]);

//====================== Motion I/O Monitoring FUNCTIONS =========================================//
//BIT0 EMG pin input
//BIT1 ALM Alarm Signal
//BIT2 +EL Positive Limit Switch
//BIT3 -EL Negative Limit Switch
//BIT4 ORG Origin Switch
//BIT5 DIR DIR output( 0 : +방향 , 1 : -방향)
//BIT6 Home Success
//BIT7 PCS PCS signal input
//BIT8 CRC CRC pin output
//BIT9 EZ Index signal
//BIT10 CLR 입력 신호 상태
//BIT11 Latch Latch signal input
//BIT12 SD Slow Down signal input
//BIT13 INP In-Position signal input
//BIT14 SVON Servo-ON output status
//BIT15 ServoAlarmReset output status
//BIT16 STA signal input
//BIT17 STP signal input
//BIT18 SYNC Pos Error signal input
//BIT19 GANT Pos Erorr signal input
//BIT20 DRV	 구동중
//BIT21 CMP  CMP 동작중
//BIT22 SYNC  SYNC 동작중
//BIT23 GANT  GANT 동작중

PMI_DLL_EXP int WINAPI nmiAxGetMechanical			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ unsigned int *npMechanical );

//BIT0 DRV_STOP:   1;	모션 구동 중
//BIT1 WAIT_DR:    1;	DR 입력 신호 기다림
//BIT2 WAIT_STA:   1;	STA 입력 신호 기다림
//BIT3 WAIT_INSYNC:1;	내부 동기 신호 기다림
//BIT4 WAIT_OTHER: 1;	타축 정지 신호 기다림
//BIT5 WAIT_:   1;	ERC 출력 완료 기다림
//BIT6 WAIT_DIR:   1;	방향 변화를 기다림
//BIT7 BACKLASH:   1;	BACKLASH 상태
//BIT8 WAIT_PE:    1;	PE 입력 신호 기다림
//BIT9 IN_FA:      1;	FA 정속 동작 중 ( 홈 스페셜 속도 )
//BIT10 IN_FL:     1;	FL 정속 동작 중
//BIT11 IN_FUR:    1;	가속 중
//BIT12 IN_FH:     1;	FH 정속 동작 중 
//BIT13 IN_FDR:    1;	감속 중 
//BIT14 WAIT_INP:  1;	INP 입력 신호 기다림
//BIT15 IN_CMP:    1;	CMP 동작중
//BIT16 WAIT_SYNC:  1;	SYNC 동작중
//BIT17 WAIT_GANT: 1;	GANT 동작중
PMI_DLL_EXP int WINAPI nmiAxGetMotion				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ unsigned int *npMotion );

//BIT0 EMG Error
//BIT1 ALM Alarm Signal Error
//BIT2 +EL Positive Limit Switch Error
//BIT3 -EL Negative Limit Switch Error
PMI_DLL_EXP int WINAPI nmiAxGetErrStatus			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ unsigned int *npErrStatus );

//current Vel in pps
PMI_DLL_EXP int WINAPI nmiAxGetCmdVel				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpVel );

//Command position counter value
//-134,217,728 <= <= 143,217,727
PMI_DLL_EXP int WINAPI nmiAxSetCmdPos			    (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dPos );
PMI_DLL_EXP int WINAPI nmiAxGetCmdPos			    (/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpPos );

//Feedback position counter value
//-134,217,728 <= <= 143,217,727
PMI_DLL_EXP int WINAPI nmiAxSetActPos				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dPos );
PMI_DLL_EXP int WINAPI nmiAxGetActPos				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpPos );

//Position error counter value
PMI_DLL_EXP int WINAPI nmiAxSetPosError				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  double  dErrPos );
PMI_DLL_EXP int WINAPI nmiAxGetPosError				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpErrPos );

//=========================================================================================================================


//0x0 Command pulse(Default) 
//0x1 External Feedback  
//0x2 PA/PB 
PMI_DLL_EXP int WINAPI nmiAxSetGenCount				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  int  nCount );
PMI_DLL_EXP int WINAPI nmiAxGetGenCount				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npCount );

//This function is used to enable the step losing checking facility. 
//By giving an tolerance value
PMI_DLL_EXP int WINAPI nmiAxSetGenSource		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nSource);
PMI_DLL_EXP int WINAPI nmiAxGetGenSource		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npSource);

//Method =0, 조건 성립 하지 않음
//Method =1, Counter 방향과 무관
//Method =2, Counter Up 중
//Method =3, Counter Down 중

//Action =0, 사용하지 않음
//Action =1, Immediately stop
//Action =2, slow down then stop
//Action =3, 속도 변경
// -134217728 < dPos < 134217727 [pps]
PMI_DLL_EXP int WINAPI nmiAxSetGenAction		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nMethod,/*[in]*/ int nAction, /*[in]*/ double dPos );
PMI_DLL_EXP int WINAPI nmiAxGetGenAction		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npMethod,/*[out]*/ int *npAction, /*[out]*/ double *dpPos );

PMI_DLL_EXP int WINAPI nmiAxSetGenPos			(/*[in]*/ int nCon,/*[in]*/ int nAxisNo,/*[in]*/ double dPos );
PMI_DLL_EXP int WINAPI nmiAxGetGenPos			(/*[in]*/ int nCon,/*[in]*/ int nAxisNo,/*[out]*/ double* dPos );


//=========================================================================================================================


//sync_mode=1, 내부 동기 시작 신호에의해 시작 
//sync_mode=2, 지정축의 정지에 의해 시작
PMI_DLL_EXP int WINAPI nmiAxSetSyncType				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int  nType );
PMI_DLL_EXP int WINAPI nmiAxGetSyncType				(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npType );

//다른 축 동기 구동 동기 신호
//sync_condition=0 내부 동기 신호 출력 OFF
//sync_condition=1 가속 개시 때 이송 
//sync_condition=2 가속 종료 때 이송
//sync_condition=3 감속 개시 때 이송
//sync_condition=4 감속 종료 때 이송
//sync_condition=5 -SL 신호가 검출 되었을 때 이송 시작
//sync_condition=6 +SL 신호가 검출 되었을 때 이송 시작
//sync_condition=7 범용 비교기기에 설정된 조건이 만족하면 이송 
//sync_condition=8 TRG-CMP 조건이 만족되었을 때 이송을 시작합니다. 
PMI_DLL_EXP int WINAPI nmiAxSetSyncAction			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/int nMaskAxes,/*[in]*/ int  nContion );
PMI_DLL_EXP int WINAPI nmiAxGetSyncAction			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/int nMaskAxes,/*[out]*/ int *npContion );

//This function is select a auto speed change method to use when the comparator conditions are satisfied. 
//By giving an tolerance value
//Counter = 0 , Command counter
//Counter = 1 , Feedback counter
//Counter = 2 , Error Counter 
//Counter = 3 , General Counter
PMI_DLL_EXP int WINAPI nmiAxSetCmpModifySource		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nCounter);
PMI_DLL_EXP int WINAPI nmiAxGetCmpModifySource		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npCounter);

//Method =0, 조건 성립 하지 않음
//Method =1, Counter 방향과 무관
//Method =2, Counter Up 중
//Method =3, Counter Down 중

//Action =0, 사용하지 않음
//Action =1, Immediately stop
//Action =2, slow down then stop
//Action =3, 속도 변경
// -134217728 < dPos < 134217727 [pps]
PMI_DLL_EXP int WINAPI nmiAxSetCmpModifyAction	(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ int nMethod,/*[in]*/ int nAction, /*[in]*/ double dPos );
PMI_DLL_EXP int WINAPI nmiAxGetCmpModifyAction	(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ int *npMethod,/*[out]*/ int *npAction, /*[out]*/ double *dpPos );

PMI_DLL_EXP int WINAPI nmiAxSetCmpModifyVel	(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ double dVel );
PMI_DLL_EXP int WINAPI nmiAxGetCmpModifyVel	(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ double *dpVel );


//====================== Overriding FUNCTIONS =================================================//
//new target position -134,217,728 <= new pos <= 134,217,727
PMI_DLL_EXP int WINAPI nmiAxModifyPos			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ double dPos ); 
PMI_DLL_EXP int WINAPI nmiMxModifyPos           (/*[in]*/ int nCon,/*[in]*/int nNAxis,/*[in]*/int naAxes[], /*[in]*/double daPos[] ); 

//new Vel in pps 1 ~ 6553500
PMI_DLL_EXP int WINAPI nmiAxModifyVel			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ double dOvr );
PMI_DLL_EXP int WINAPI nmiMxModifyVel           (/*[in]*/ int nCon,/*[in]*/int nNAxis,/*[in]*/int naAxes[],/*[in]*/double daOvr[] );

//new Vel in pps 1 ~ 6553500
//0 <= Tacc < 60000  [ms]
//0 <= Tdec < 60000  [ms]
PMI_DLL_EXP int WINAPI nmiAxModifyVelProf			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/ double dVel,/*[in]*/ double dTacc,/*[in]*/ double dTdec);
//====================== Coordinat Motion Control =========================================//
//nCS = 0,1,2,3
PMI_DLL_EXP int WINAPI nmiCsSetAxis					(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/ int nNAxis,/*[in]*/ int naAxis[] );

PMI_DLL_EXP int WINAPI nmiCsSetInitVel				(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/ double dVel );
PMI_DLL_EXP int WINAPI nmiCsGetInitVel				(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[out]*/ double *dpVel );

//Op_type = 0 vector vel, 
//Op_type = 1 long axis vel, 
//nType=0 Constant
//nType=1 T-Curve (Default)
//nType=2 S-Curve
PMI_DLL_EXP int WINAPI nmiCsSetVelProf			(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/  int  nOpType, /*[in]*/ int nProfileType,/*[in]*/  double  dVel,/*[in]*/  double  dTacc,/*[in]*/  double  dTdec );
PMI_DLL_EXP int WINAPI nmiCsGetVelProf			(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/  int  nOpType, /*[out]*/ int *nProfileType,/*[out]*/ double *dpVel,/*[out]*/ double *dTacc,/*[out]*/ double *dTdec );

//Line Coordinate Motion 
PMI_DLL_EXP int WINAPI nmiCsLinMove				(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/ int nAbsMode,/*[in]*/ double daPos[] );

//End Point Circular Coordinate Motion 
//0 : CW
//1 : CCW 
PMI_DLL_EXP int WINAPI nmiCsArcPMove			(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/ int nAbsMode,/*[in]*/ double daCen[],/*[in]*/ double daPos[],/*[in]*/ int nDir );

//Angle Circular Coordinate Motion 
PMI_DLL_EXP int WINAPI nmiCsArcAMove			(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[in]*/ int nAbsMode,/*[in]*/ double daCen[],/*[in]*/ double dAngle );

//-- Helical Motion -------------------
PMI_DLL_EXP int WINAPI nmiCsHelMove				(/*[in]*/ int nCon,/*[in]*/ int nHelId, /*[in]*/ double dCenX,/*[in]*/ double dCenY,/*[in]*/ double dPosX,/*[in]*/ double dPosY,/*[in]*/ double dPosZ,/*[in]*/ int nDir );

//motion check Coordinate axis driving
//0 stop
//1 driving
PMI_DLL_EXP int WINAPI nmiCsCheckDone				(/*[in]*/ int nCon,/*[in]*/ int nCs,/*[out]*/ int *npDone);

PMI_DLL_EXP int WINAPI nmiCsStop					(/*[in]*/ int nCon,/*[in]*/ int nCs);

PMI_DLL_EXP int WINAPI nmiCsEStop				    (/*[in]*/ int nCon,/*[in]*/ int nCs);



//====================== interrupt FUNCTIONS ==================================================//
PMI_DLL_EXP int WINAPI nmiIntSetHandler			    (/*[in]*/ int nCon,/*[in]*/ int nType,/*[in]*/ HANDLE hWnd,/*[in]*/ void* hHandler,/*[in]*/ unsigned int nMsg);

PMI_DLL_EXP int WINAPI nmiIntSetHandlerEnable		(/*[in]*/ int nCon,/*[in]*/ int nEnable);

//-------- Axis--------------------------------------------------------------------------------------
//BIT0	; 자동 정지때
//BIT1	; 다음 동작 계속 START 때
//BIT2	; 동작용 2nd pre register 기입 가능 때
//BIT3	; Comparator 5용 2nd pre register 기입 가능 때
//BIT4	; 가속 개시 때
//BIT5	; 가속 종료 때
//BIT6	; 감속 개시 때
//BIT7	; 감속 종료 때
//BIT8	; Compatator1 조건 성립 때
//BIT9	; Compatator2 조건 성립 때
//BIT10	; Compatator3 조건 성립 때
//BIT11	; Compatator4 조건 성립 때
//BIT12	; Compatator6 조건 성립 때
//BIT13	; CLR 신호 입력에 의해 COUNTER 값 RESET 때
//BIT14	; LTC 신호 입력에 의해 COUNTER 값 LATCH 때
//BIT15	; ORG 신호 입력에 의해 COUNTER 값 LATCH 때
//BIT16	; SD 입력 ON 때
//BIT17	; /STA 입력 ON 때

PMI_DLL_EXP int WINAPI nmiIntSetAxisEnable			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[in]*/  unsigned int  nMask );

PMI_DLL_EXP int WINAPI nmiIntGetAxisEnable			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ unsigned int *nMask );

//BIT0	; 자동 정지때
//BIT1	; 다음 동작 계속 START 때
//BIT2	; 동작용 2nd pre register 기입 가능 때
//BIT3	; Comparator 5용 2nd pre register 기입 가능 때
//BIT4	; 가속 개시 때
//BIT5	; 가속 종료 때
//BIT6	; 감속 개시 때
//BIT7	; 감속 종료 때
//BIT8	; Compatator1 조건 성립 때
//BIT9	; Compatator2 조건 성립 때
//BIT10	; Compatator3 조건 성립 때
//BIT11	; Compatator4 조건 성립 때
//BIT12	; Compatator6 조건 성립 때
//BIT13	; CLR 신호 입력에 의해 COUNTER 값 RESET 때
//BIT14	; LTC 신호 입력에 의해 COUNTER 값 LATCH 때
//BIT15	; ORG 신호 입력에 의해 COUNTER 값 LATCH 때(사용안함)
//BIT16	; SD 입력 ON 때
//BIT19	; /STA 입력 ON 때

PMI_DLL_EXP int WINAPI nmiIntGetAxisStatus			(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ unsigned int *npStatus );

//BIT0 STOP_BY_SLP:   1;	양의 소프트 리미트에 의해 정지
//BIT1 STOP_BY_SLN:   1;	음의 소프트 리미트에 의해 정지
//BIT2 STOP_BY_CMP3:  1;	비교기3에 의해 정지
//BIT3 STOP_BY_CMP4:  1;	비교기4에 의해 정지
//BIT4 STOP_BY_CMP5:  1;	비교기5에 의해 정지
//BIT5 STOP_BY_ELP:   1;	+EL 에 의해 정지
//BIT6 STOP_BY_ELN:   1;	-EL 에 의해 정지
//BIT7 STOP_BY_ALM:   1;	알람에 의해 정지
//BIT8 STOP_BY_STP:   1;	CSTP에 의해 정지
//BIT9 STOP_BY_EMG:   1;	EMG에 의해 정지
//BIT10 STOP_BY_SD:   1;	SD 입력에 의해 정지
//BIT11 STOP_BY_DT:   1;	보간 동작 DATA 이상에 의해 정지
//BIT12 STOP_BY_IP:   1;	보간 동작 중에 타 축의 이상 정지에 의해 동시 정지
//BIT13 STOP_BY_PO:   1;	PA/PB 입력용 buffer counter dml overflow 에 의해 정지
//BIT14 STOP_BY_AO:   1;	보간 동작 때의 위치 범위를 벗어나서 정지
//BIT15	STOP_BY_EE:   1;	EA/EB 입력 에러 발생 (정지 하지 않음)
//BIT16	STOP_BY_PE:   1;	PA/PB 입력 에러 발생 (정지 하지 않음)

PMI_DLL_EXP int WINAPI nmiIntGetAxisErrStatus  		(/*[in]*/ int nCon,/*[in]*/ int nAxis,/*[out]*/ unsigned int *npStatus );

//-------- DI----------------------------------------------------------------------------------------
//0 nId => 0  ~ 31
//1 nId => 32 ~ 63
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiIntSetDiEnable			(/*[in]*/ int nCon,/*[in]*/ int nSlaveAddress, /*[in]*/ int nId,/*[in]*/  unsigned int  nMask );
//0 nId => 0  ~ 31
//1 nId => 32 ~ 63
//TMC-BBxxP is not Supported a Function.
PMI_DLL_EXP int WINAPI nmiIntGetDiStatus			(/*[in]*/ int nCon,/*[in]*/ int nSlaveAddress, /*[in]*/ int nId,/*[out]*/ unsigned int *npStatus );

//====================== DIGITAL I/O FUNCTIONS ==============================================//
// 통신 Digital In/Out FUNCTIONS
PMI_DLL_EXP int WINAPI nmiDiGetData				(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[out]*/ unsigned int *npData);
PMI_DLL_EXP int WINAPI nmiDiGetBit				(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[in]*/ int nBit, /*[out]*/ unsigned int *npData);
PMI_DLL_EXP int WINAPI nmiDoSetData				(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[in]*/ unsigned int npData);
PMI_DLL_EXP int WINAPI nmiDoGetData				(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[out]*/ unsigned int *npData);
PMI_DLL_EXP int WINAPI nmiDoSetBit				(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[in]*/ int nBit, /*[in]*/ unsigned int nData);
PMI_DLL_EXP int WINAPI nmiDoGetBit				(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[in]*/ int nBit, /*[out]*/ unsigned int *npData);

//====================== DEBUG-LOGGING FUNCTIONS ==============================================//
PMI_DLL_EXP int WINAPI nmiDLogSetFile				(/*[in]*/ const char *szFilename );
PMI_DLL_EXP int WINAPI nmiDLogGetFile				(/*[out]*/ char *szFilename );

PMI_DLL_EXP int WINAPI nmiDLogSetLevel				(/*[in]*/ int nLevel);
PMI_DLL_EXP int WINAPI nmiDLogGetLevel				(/*[out]*/ int *nLevel);

PMI_DLL_EXP int WINAPI nmiDLogSetEnable				(/*[in]*/ int nEnable);
PMI_DLL_EXP int WINAPI nmiDLogGetEnable  			(/*[out]*/ int *nEnable);

//====================== GENERAL FUNCTIONS ==============================================//
//긴급 정지 신호 레벨 
PMI_DLL_EXP int WINAPI nmiGnSetEmgLevel				(/*[in]*/ int nCon,/*[in]*/  int  nLevel );
PMI_DLL_EXP int WINAPI nmiGnGetEmgLevel				(/*[in]*/ int nCon,/*[out]*/ int *npLevel );
 //모션 보드에 설치된 전체 축 개수 
PMI_DLL_EXP int WINAPI nmiGnGetAxesNumAll			(/*[in]*/ int nCon,/*[out]*/ int* nNAxesNum);
//스테이션 별 축 개수 
PMI_DLL_EXP int WINAPI nmiGnGetAxesNumSlave		(/*[in]*/ int nCon,/*[in]*/ int nSlaveAddress,/*[out]*/ int* nNAxesNum);
//스테이션 별 맵핑 축 범위 확인
PMI_DLL_EXP int WINAPI nmiGnGetSlaveAxisRange	(/*[in]*/int nCon,/*[in]*/int nSlaveAddress,/*[out]*/ int* pnInitAxis, /*[out]*/ int* pnEndAxis);

//모델명 읽어오기
PMI_DLL_EXP int WINAPI nmiConGetModel				(/*[in]*/ int nCon,/*[out]*/ int *npModel );

//F/W 버전 읽어오기
PMI_DLL_EXP int WINAPI nmiConGetFwVersion( int nCon, int *nVer );

//H/W 버전 읽어오기
PMI_DLL_EXP int WINAPI nmiConGetHwVersion( int nCon, int *nVer );

//dll 버전 읽어오기
PMI_DLL_EXP int WINAPI nmiConGetMApiVersion( int nCon, int *nVer );

//보드 LED ON/OFF
PMI_DLL_EXP int WINAPI nmiConSetCheckOn	(/*[in]*/ int nCon,/*[in]*/ int nOn );
PMI_DLL_EXP int WINAPI nmiConGetCheckOn	(/*[in]*/ int nCon,/*[out]*/ int *nOn );

//모션 보드에 설치된 전체 디지털 범용 I/O 개수
PMI_DLL_EXP int WINAPI nmiGnGetDioNum				(/*[in]*/ int nCon, /*[out]*/ int* nNDiChNum, /*[out]*/ int* nNDoChNum);
//스테이션 별 디지털 범용 I/O 개수
PMI_DLL_EXP int WINAPI nmiGetSlaveDioNum			(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[out]*/ int *npDiNum, /*[out]*/ int *npDoNum);

// 전체 통신 에러 발생 횟수
PMI_DLL_EXP int WINAPI nmiGetCommErrNum			(/*[in]*/ int nCon, /*[out]*/ int *npErrNum);
// 전체 통신 에러 발생 횟수 0으로 클리어
PMI_DLL_EXP int WINAPI nmiCommErrNumClear		(/*[in]*/ int nCon);
// 각 스테이션 별 에러 발생 플래그
PMI_DLL_EXP int WINAPI nmiGetCyclicErrFlag		(/*[in]*/ int nCon, /*[in]*/ int nStNo, /*[out]*/ int *npFlag );
// 각 스테이션 별 에러 발생 플래그 해제
PMI_DLL_EXP int WINAPI nmiCyclicErrFlagClear		(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress);
// 전체 통신 에러 발생 플래그
PMI_DLL_EXP int WINAPI nmiGetCyclicErrFlagAll		(/*[in]*/ int nCon, /*[out]*/ unsigned int *nHighFlag, /*[out]*/ unsigned int *nLowFlag );
// 전체 통신 에러 발생 플래그 해제
PMI_DLL_EXP int WINAPI nmiCyclicErrFlagClearAll		(/*[in]*/ int nCon );


// 사이클릭 통신 에러 상태
// 1 || 0  비트
// 0    0  통신 종료
// 0    1  사이클릭 통신 중
// 1    0  사이클릭 통신 종료. 에러 해제 안한 상태
// 1    1  사이클릭 통신 중 에러 발생
PMI_DLL_EXP int WINAPI nmiGetCyclicStatus		(/*[in]*/ int nCon, /*[out]*/ unsigned int *unpStatus);

// 사이클릭 통신 속도 확인. [us] 단위.
PMI_DLL_EXP int WINAPI nmiGetCyclicSpeed			(/*[in]*/ int nCon, /*[out]*/ int *npTime);

PMI_DLL_EXP int WINAPI nmiGetSlaveTotal			(/*[in]*/ int nCon, /*[out]*/ int *nSlvNum);

// 매핑된 축의 Station 번호와, Station에서 축 번호를 가져온다. 
// 축 번호는 Station 번호 순으로 매핑된다.
PMI_DLL_EXP int WINAPI nmiGnCheckAxesMap	(/*[in]*/ int nCon,/*[in]*/ int nAxis, /*[out]*/ int* nActStNo, /*[out]*/ int *nActAxis);

//디바이스 정보
// 4 || 3 || 2 || 1 || 0  비트
//           0    0    0  : 32점 출력 전용
//           0    1    0  : 16점 출력 16점 입력 
//           1    1    1  : 32점 입력 전용
//      0            : I/O 전용
//      1            : Motion 전용
// 0            : 사용 안함
// 1            : 사용
PMI_DLL_EXP int WINAPI nmiGetSlaveInfo			(/*[in]*/ int nCon, /*[in]*/ int nSlaveAddress, /*[out]*/ int *npSlvInfo);

// 1: 사이클릭 통신 에러 발생
PMI_DLL_EXP int WINAPI nmiGetEIOE				(/*[in]*/ int nCon, /*[out]*/ unsigned int *unpStatus );
// 1: 사이클릭 통신 중
PMI_DLL_EXP int WINAPI nmiGetSBSY				(/*[in]*/ int nCon, /*[out]*/ unsigned int *unpStatus );



//====================== ERROR HANDLING FUNCTIONS =============================================//
PMI_DLL_EXP int WINAPI nmiErrGetCode	            (/*[in]*/ int nCon, /*[out]*/ int *nCode );

PMI_DLL_EXP int WINAPI nmiErrGetString		        (/*[in]*/ int nCon, /*[in]*/ int nCode,/*[out]*/ char* szStr );

PMI_DLL_EXP int WINAPI nmiErrAxGetCode    		    (/*[in]*/ int nCon, /*[in]*/ int nAxis,/*[out]*/ int *pCode );

PMI_DLL_EXP int WINAPI nmiGnGetMotDevMap			(/*[out]*/TMotDevInfoMap *TMotDevBuf);

//====================== Etc. FUNCTIONS =======================================================//
PMI_DLL_EXP void WINAPI nmiConSetPrmPath			(/*[in]*/ const char* szFilename);
PMI_DLL_EXP void WINAPI nmiConGetPrmPath			(/*[in]*/ char* szFilename);

PMI_DLL_EXP int WINAPI nmiConSetSysCommFlag			(/*[in]*/ int nFlag);
PMI_DLL_EXP int WINAPI nmiConGetSysCommFlag			(/*[out]*/ int *nFlag);



PMI_DLL_EXP int WINAPI nmiAxSetCmpTrgPos			(/*[in]*/ int nCon, /*[in]*/ int nAxis, double dSPos, double dPos, double dEPos);
//PMI_DLL_EXP int WINAPI nmiAxCmpTrgBegin(/*[in]*/ int nCon, /*[in]*/ int nAxis);
//PMI_DLL_EXP int WINAPI nmiAxCmpTrgEnd(/*[in]*/ int nCon, /*[in]*/ int nAxis);

// nId = 0 Or 1
// 0 = > Each Axis
// 1 = > All Axis
// nCur => 0  0 [A]
// nCur => 1  0.2 [A]
// nCur => 2  0.4 [A]
// nCur => 3  0.5 [A]
// nCur => 4  0.6 [A] => Default
// nCur => 5  0.7 [A] 
// nCur => 6  0.8 [A]
// nCur => 7  1.0 [A]
PMI_DLL_EXP int WINAPI nmiGnSetCurrent	(/*[in]*/ int nCon, /*[in]*/ int nAxis, /*[in]*/ int nId, /*[in]*/ int nCur);
PMI_DLL_EXP int WINAPI nmiGnGetCurrent	(/*[in]*/ int nCon, /*[in]*/ int nAxis, /*[in]*/ int nId, /*[out]*/ int *nCur);

PMI_DLL_EXP int WINAPI nmiGnGetOrgStatus(/*[in]*/ int nCon, /*[in]*/ int nStaNo, /*[out]*/ int nOrg[]);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of PMI_MAPI_H definition

//===========================================================================
// End of file.
//===========================================================================
