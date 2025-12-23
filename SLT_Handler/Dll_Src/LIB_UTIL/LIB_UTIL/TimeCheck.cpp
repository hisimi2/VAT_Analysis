#include "stdafx.h"
#include "TimeCheck.h"
#include <time.h>

///////////////////////////////////////////////////////////////////////////////
///
///							::CTimerCheck::
///
///////////////////////////////////////////////////////////////////////////////

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTimerCheck::CTimerCheck(double checktime, BOOL start_mode)
{
	m_CheckTime = checktime;
	m_bchecking = false;
	if(start_mode)
	{
		//바로 시작한다.
		StartTimer();
		m_bchecking = true;
	}
	EndTime = 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTimerCheck::~CTimerCheck()
{
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTimerCheck::StartTimer()
{
	StartTime = clock();
	m_bchecking=TRUE;
}




/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : //Auto Speed 1.04__________________________________
             //시간 계수의 시전을 현재 기준으로 Startime을 리셋한다.
==============================================================================*/
void CTimerCheck::ResetStartTime()
{
	ULONG curTime = clock();
	StartTime = (curTime - StartTime) + StartTime;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CTimerCheck::LessThan(double ctime)
{
	EndTime = clock();

	if( CompareTime() < ctime ) return TRUE;
	else return FALSE;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CTimerCheck::MoreThan(double ctime)
{
	EndTime = clock();

	if( CompareTime() > ctime ) return TRUE;
	else return FALSE;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : msec : true = sec  반환,  false = msec 반환
  RETURN   : 경과된 시간을 반환한다.
  COMMENTS : 
==============================================================================*/
double CTimerCheck::CompareTime(bool msec)
{
	EndTime = clock();

	double tbuf = (double)((EndTime - StartTime) / CLK_TCK);
	if(tbuf<0) StartTimer();

	if( !msec )	
	{
		long lbuf = EndTime - StartTime;
		return lbuf;
	}

	return tbuf;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 시간이 경과된경우 'TRUE' 를 리턴 한다.
==============================================================================*/
BOOL CTimerCheck::CheckOverTime()
{
	EndTime = clock();

	if( CompareTime() < m_CheckTime ) return FALSE;
	else return TRUE;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : ctime : 점검할 시간
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTimerCheck::SetCheckTime(double ctime)
{
	m_CheckTime = ctime;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double CTimerCheck::GetCheckTime()
{
	return m_CheckTime;
}

///////////////////////////////////////////////////////////////////////////////
///
///								::CMyTime::
///
///////////////////////////////////////////////////////////////////////////////


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CMyTime::GetCurTime()
{
	CTime time = CTime::GetCurrentTime();
	CString fname;

	year = time.GetYear();
	month = time.GetMonth();
	day = time.GetDay();
	hour = time.GetHour();
	min = time.GetMinute();
	sec = time.GetSecond();
	
	if(0)
	{
		if( hour < int(0))
		{
			day = day - 1;
			if(day==0)
			{
				month = month - 1;
				if(month==0)
				{
					month = 12;
					year = year -1;
				}

				switch(month)
				{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					day = 31;
					break;
				case 2:
					day = 28;
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					day = 30;
					break;
				}
			}
		}
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMyTime::GetDay()
{
	return day;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMyTime::GetYear()
{
	return year;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMyTime::GetMonth()
{
	return month;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMyTime::GetHour()
{
	return hour;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMyTime::GetMinute()
{
	return min;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMyTime::GetSecond()
{
	return sec;
}