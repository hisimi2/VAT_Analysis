#include "StdAfx.h"
#include "TimerTickCheck.h"

// dCheckTime : 점검해야 할 시간 [msec]
// start_mode
CTimerTickCheck::CTimerTickCheck(double dChecktime/*=0.0*/, BOOL start_mode/*=FALSE*/)
	:m_dCheckTime(dChecktime)
{
	m_bChecking = FALSE;
	if( start_mode )
	{
		StartTimer();
	}
	m_dEndTime = 0.0;
}

CTimerTickCheck::~CTimerTickCheck(void)
{
}

void CTimerTickCheck::ResetStartTime()
{
	m_dStartTime = (double)::GetTickCount64();
}

// msec_opt : 1=msec로 반환, 0=sec로 반환.
double CTimerTickCheck::CompareTime( BOOL msec_opt/*=true*/ )
{
	m_dEndTime = (double)::GetTickCount64();
	double dSpanTime =  m_dEndTime - m_dStartTime;
	if(dSpanTime<0) StartTimer();
	if( !msec_opt )
	{
		dSpanTime /= 1000.0;
	}
	return dSpanTime;
}

// 점검해야 할 시간. [sec]
double CTimerTickCheck::GetCheckTime()
{
	return m_dCheckTime;
}

// sec
void CTimerTickCheck::SetCheckTime( double dCheckTime )
{
	m_dCheckTime = dCheckTime;
}

// 측정 시간이 초과하였는지 확인 한다. 순환루틴에서 호출해준다.
BOOL CTimerTickCheck::CheckOverTime()
{
	BOOL bTimeOver = FALSE;
	m_dEndTime = (double)::GetTickCount64();
	double dSpanTime =  m_dEndTime - m_dStartTime;
	if( dSpanTime > m_dCheckTime*1000.0 ){
		bTimeOver = TRUE;
	}
	return bTimeOver;
}

// dTime : [sec]
BOOL CTimerTickCheck::MoreThan( double dTime )
{
	BOOL bRet = FALSE;
	if( CompareTime() > dTime ) bRet = TRUE;
	return bRet;
}

// dTime : [sec]
BOOL CTimerTickCheck::LessThan( double dTime )
{
	BOOL bRet = FALSE;
	if( CompareTime() < dTime ) bRet = TRUE;
	return bRet;
}

void CTimerTickCheck::StartTimer()
{
	m_dStartTime = (double)::GetTickCount64();
	m_bChecking = TRUE;
}

void CTimerTickCheck::SetTimerCheckEnd()
{
	m_bChecking = FALSE;
}

BOOL CTimerTickCheck::IsChecking()
{
	return m_bChecking;
}
