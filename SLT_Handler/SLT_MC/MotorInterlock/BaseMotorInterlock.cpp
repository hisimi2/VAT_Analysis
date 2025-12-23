#include "StdAfx.h"
#include "BaseMotorInterlock.h"


CBaseMotorInterlock::CBaseMotorInterlock(void)
{
}


CBaseMotorInterlock::~CBaseMotorInterlock(void)
{
}

int CBaseMotorInterlock::CommonInterlock()
{
	// Door open 상태에서 이동 명령 금지
	//int nCheckDoorLock = 0;

	int nErr = ERR_NO_ERROR;
	nErr = g_TaskSystemCtrl.ChkComInterlock();
	
	if( g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE) )
	{

	}
	return nErr;
}

int CBaseMotorInterlock::CommonMotorInterlock( OneAxis *pMotor, BOOL bOrgChk /*= TRUE*/)
{
	BOOL bAmpFault = pMotor->IsAmpFault();
	if (bAmpFault) {
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	BOOL bServoOn = pMotor->GetServoStat();
	if (bServoOn != TRUE) {
		return  ERR_MOTOR_CMN_SERVO_OFF;
	}

 	int nErrOrg = ERR_NO_ERROR;
 	BOOL bOrgCompleted = pMotor->IsOrgCompleted(&nErrOrg);
 	if (bOrgCompleted != TRUE && bOrgChk == TRUE) {
 		return nErrOrg;
 	}

	return ERR_NO_ERROR;
}