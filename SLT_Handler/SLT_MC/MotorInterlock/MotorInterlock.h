#pragma  once

#include "DEF_MOTION.h"
#include "MotorInterlock_Stacker.h"
#include "MotorInterlock_PressUnit.h"
#include "MotorInterLock_LoadTbl.h"
#include "MotorInterlock_Transfer.h"
#include "MotorInterlock_TrayPP.h"
#include "MotorInterlock_TestPP.h"

//=============================================
// Move InterLock
//=============================================
int MotorMoveInterLock_PressUnit_Dn1_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn2_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn3_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn4_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn5_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn6_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn7_z       (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_PressUnit_Dn8_z       (double dTPos, double dSpd, int nAcc, int nDcc);

int MotorMoveInterLock_Test_Hand1_x1          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_x2			(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_y          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_z1          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_z2		(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1	(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2	(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1	(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2	(double dTPos, double dSpd, int nAcc, int nDcc);

int MotorMoveInterLock_Test_Hand2_x1          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_x2			(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_y          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_z1          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_z2		(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2(double dTPos, double dSpd, int nAcc, int nDcc);

int MotorMoveInterLock_Transfer_X            (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand1_x          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Feeder          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand1_z          (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand1_Pitch_x    (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand1_Pitch_y    (double dTPos, double dSpd, int nAcc, int nDcc);

int MotorMoveInterLock_CTray_Hand2_x		(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand2_y		(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand2_z		(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand2_Pitch_x (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_CTray_Hand2_Pitch_y (double dTPos, double dSpd, int nAcc, int nDcc);

int MotorMoveInterLock_LoadTbl1_y            (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_LoadTbl2_y            (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_LoadTbl3_y			 (double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_LoadTbl4_y			 (double dTPos, double dSpd, int nAcc, int nDcc);

int MotorMoveInterLock_LoadTbl1_Rotate(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_LoadTbl2_Rotate(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_LoadTbl3_Rotate(double dTPos, double dSpd, int nAcc, int nDcc);
int MotorMoveInterLock_LoadTbl4_Rotate(double dTPos, double dSpd, int nAcc, int nDcc);

//int MotorMoveInterLock_Tray_Feeder_End_Stopper(double dTPos, double dSpd, int nAcc, int nDcc);

//=============================================
// Origin InterLock
//=============================================
int MotorOrgInterLock_PressUnit_Dn1_z       ();
int MotorOrgInterLock_PressUnit_Dn2_z       ();
int MotorOrgInterLock_PressUnit_Dn3_z       ();
int MotorOrgInterLock_PressUnit_Dn4_z       ();
int MotorOrgInterLock_PressUnit_Dn5_z       ();
int MotorOrgInterLock_PressUnit_Dn6_z       ();
int MotorOrgInterLock_PressUnit_Dn7_z       ();
int MotorOrgInterLock_PressUnit_Dn8_z       ();

int MotorOrgInterLock_Test_Hand1_x1          ();
int MotorOrgInterLock_Test_Hand1_x2			();
int MotorOrgInterLock_Test_Hand1_y          ();
int MotorOrgInterLock_Test_Hand1_z1          ();
int MotorOrgInterLock_Test_Hand1_z2			();
int MotorOrgInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1	();
int MotorOrgInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2();
int MotorOrgInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1	();
int MotorOrgInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2	();

int MotorOrgInterLock_Test_Hand2_x1          ();
int MotorOrgInterLock_Test_Hand2_x2			();
int MotorOrgInterLock_Test_Hand2_y          ();
int MotorOrgInterLock_Test_Hand2_z1          ();
int MotorOrgInterLock_Test_Hand2_z2			();
int MotorOrgInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1();
int MotorOrgInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2();
int MotorOrgInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1();
int MotorOrgInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2();

int MotorOrgInterLock_Transfer_X            ();
int MotorOrgInterLock_CTray_Hand1_x();
int MotorOrgInterLock_CTray_Feeder();
int MotorOrgInterLock_CTray_Hand1_z();
int MotorOrgInterLock_CTray_Hand1_Pitch_x();
int MotorOrgInterLock_CTray_Hand1_Pitch_y();

int MotorOrgInterLock_CTray_Hand2_x();
int MotorOrgInterLock_CTray_Hand2_y();
int MotorOrgInterLock_CTray_Hand2_z();
int MotorOrgInterLock_CTray_Hand2_Pitch_x();
int MotorOrgInterLock_CTray_Hand2_Pitch_y();

int MotorOrgInterLock_LoadTbl1_y            ();
int MotorOrgInterLock_LoadTbl2_y            ();
int MotorOrgInterLock_LoadTbl3_y			();
int MotorOrgInterLock_LoadTbl4_y			();

int MotorOrgInterLock_LoadTbl1_Rotate();
int MotorOrgInterLock_LoadTbl2_Rotate();
int MotorOrgInterLock_LoadTbl3_Rotate();
int MotorOrgInterLock_LoadTbl4_Rotate();

//int MotorOrgInterLock_Tray_Feeder_End_Stopper();


//=============================================
// Jog InterLock
//=============================================
int MotorJogInterLock_PressUnit_Dn1_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn2_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn3_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn4_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn5_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn6_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn7_z( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_PressUnit_Dn8_z( double dSpd, int nAcc, int nDcc );



int MotorJogInterLock_Test_Hand1_x1( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_Test_Hand1_x2(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand1_y( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_Test_Hand1_z1( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_Test_Hand1_z2(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2(double dSpd, int nAcc, int nDcc);


int MotorJogInterLock_Test_Hand2_x1( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_Test_Hand2_x2(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand2_y( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_Test_Hand2_z1( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_Test_Hand2_z2(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2(double dSpd, int nAcc, int nDcc);

int MotorJogInterLock_Transfer_X( double dSpd, int nAcc, int nDcc );

int MotorJogInterLock_CTray_Hand1_x(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Feeder(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand1_z(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand1_Pitch_x(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand1_Pitch_y(double dSpd, int nAcc, int nDcc);

int MotorJogInterLock_CTray_Hand2_x(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand2_y(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand2_z(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand2_Pitch_x(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_CTray_Hand2_Pitch_y(double dSpd, int nAcc, int nDcc);

int MotorJogInterLock_LoadTbl1_y( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_LoadTbl2_y( double dSpd, int nAcc, int nDcc );
int MotorJogInterLock_LoadTbl3_y(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_LoadTbl4_y(double dSpd, int nAcc, int nDcc);

int MotorJogInterLock_LoadTbl1_Rotate(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_LoadTbl2_Rotate(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_LoadTbl3_Rotate(double dSpd, int nAcc, int nDcc);
int MotorJogInterLock_LoadTbl4_Rotate(double dSpd, int nAcc, int nDcc);

//int MotorJogInterLock_Tray_Feeder_End_Stopper(double dSpd, int nAcc, int nDcc);


typedef struct _ST_MOT_INTERLOCK_CALLBACK_MAP{
	int nLogicalIdx;
	FN_MOTOR_MOV_INTERLOCK_CALLBACK fnMovInterlockCallBack;
	FN_MOTOR_ORG_INTERLOCK_CALLBACK fnOrgInterlockCallBack;
	FN_MOTOR_JOG_INTERLOCK_CALLBACK fnJogInterlockCallBack;
}ST_MOT_INTERLOCK_CALLBACK_MAP;


extern ST_MOT_INTERLOCK_CALLBACK_MAP g_aMotorInterlockMap[];

void AfxGetMotorInterlockData(int nLogicalIdx, ST_MOT_INTERLOCK_CALLBACK_MAP& rData);