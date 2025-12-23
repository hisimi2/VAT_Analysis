/********************************************************************
	created:	2015/07/24
	created:	24:7:2015   15:25
	filename: 	SLT_Handler\Include\DEF_MOTION.h
	file path:	SLT_Handler\Include
	file base:	DEF_MOTION
	file ext:	h
	author:		Na Myoung-Jin
	
	purpose:	LIB_MC Dll의 Motion관련 매크로 모음
*********************************************************************/
#pragma  once

#include "MyList.h"

#define MOTOR_INPOSITION				(200.0) // 마이크로 미터
#define MOTOR_MOVE_TIMEOUT				(30)	 // Sec
#define MOTOR_INPOSITION_PULSE_COUNT	(40)
#define PREMOVE_POSITION_GAB		(5000.0)	// 3mm

#ifndef NEXT
	#define NEXT						Sleep(1)
#endif
#define MMC_BOARD_MAX_CNT			(8)
#define MAX_AXIS_COUNT_ONE_PC		(64)


//==============================================================================
// Motion Board Type Define
//==============================================================================
#define DEF_MOTION_TYPE_SIM				 (0)
#define DEF_MOTION_TYPE_MMC				 (1)
#define DEF_MOTION_TYPE_ECAT_SOFT_SERVO  (2)


//==============================================================================
// 외부에서 인터락 조건을 구현 후 전달 하기 위해 사용 한다.
// 인터락 OK 이면 0을 반환하고 인터락 코드를 반환 시킨다.
//==============================================================================
#ifndef __FN_MOTOR_INTERLOCK_CALLBACK__
#define __FN_MOTOR_INTERLOCK_CALLBACK__
	typedef INT (*FN_MOTOR_MOV_INTERLOCK_CALLBACK)(double dTPos, double dSpd, int nAcc, int nDcc); // Pos는 절대위치이다.
	typedef INT( *FN_MOTOR_JOG_INTERLOCK_CALLBACK )( double dSpd, int nAcc, int nDcc );
	typedef INT (*FN_MOTOR_ORG_INTERLOCK_CALLBACK)();
#endif // __FN_MOTOR_INTERLOCK_CALLBACK__



//==============================================================================
//
//==============================================================================
typedef struct  _ST_MOTORSTATUS	{
	int			iAxisNum;			// 0~15
	double		dTarget;			// 움직이려는 위치	[um]
	double		dEncoder;			// 현재 위치			[um]
	double		dCommand;			// 지령				[um]
	double		dMotorSpeed;        // 움직이는 속도		[um/sec]
	int			dAcc;				// 가속				[msec]
	int			dDcc;				// 감속				[msec]
	BOOL        bServoStatus;       // 0 비정상 1 정상        
	int         iInsequence;        // Insequence TRUE/FALSE
	int			iInmotion;			// Insequence TRUE/FALSE
	int         iInposition;		// Insequence TRUE/FALSE
	int			iFramesleft;		// Insequence TRUE/FALSE
	int         iMotiondone;		// Insequence TRUE/FALSE
	int			iAxisdone;			// Insequence TRUE/FALSE
	short		sEncoderRPM;		// Motor Amp Fault

	_ST_MOTORSTATUS()
	{	
		memset(this, 0x00, sizeof(_ST_MOTORSTATUS));
	};
}ST_MOTORSTATUS, *LPST_MOTORSTATUS;


//==============================================================================
//
//==============================================================================
typedef struct _ST_ROBOT_DB_TBL {
	int    nAxisNo_Logical;			// 프로그램에서 사용하는 논리적인 축번호
	char   szRobotDef[128];			// 축 그룹
	int    nAxisNo_Physical;		// Board상의 axis 번호
	char   szDisplayNam[128];		// Display Name
	double dScaleFactor;			// um/pulse
	int    nDirZR;					// -1:역 , 1:정
	int    nDirPulse;				// -1:역 , 1:정
	int    nDirMove;				// -1:역 , 1:정
	int	   nEncoderType;
	BOOL   bDisableAxis;			// 해당 축의 사용 유/무

	double dLimit_Posi;
	double dLimit_Nega;
	int    nSpd_Normal;				// pps
	int    nAcc_Normal;				// msec/4
	int    nDec_Normal;				// msec/4
	int    nSpd_ZR;					// pps
	int    nAcc_ZR;					// msec/4
	int    nDec_ZR;					// msec/4

	_ST_ROBOT_DB_TBL()
	{
		memset(this, 0x00, sizeof(_ST_ROBOT_DB_TBL));
	};
}ST_ROBOT_DB_TBL;


//==============================================================================
//
//==============================================================================
class CAxisInfoItem : public CObject
{
public:
	CAxisInfoItem(){};
	~CAxisInfoItem(){};

	ST_ROBOT_DB_TBL m_stAxisInfo;
};

/*
CMyList AxisList;
CAxisInfoItem* pItem = AxisList.GetData(0);
pItem->m_stAxisInfo.dLimit_Nega;
*/
