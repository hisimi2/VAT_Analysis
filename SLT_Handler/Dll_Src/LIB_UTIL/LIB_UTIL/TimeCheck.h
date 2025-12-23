#ifndef	_CTIMERCHECK
#define	_CTIMERCHECK


//==============================================================================
//
//==============================================================================
class  AFX_EXT_CLASS CTimerCheck
{
public:
	CTimerCheck(double checktime=0.0, BOOL start_mode=FALSE);	//단위는 1.0 초...
	~CTimerCheck();

public:
	void ResetStartTime();
	double	CompareTime(bool msec=true);//현재 까지의 시간을 리턴....
	double GetCheckTime();
	void SetCheckTime(double ctime);
	BOOL CheckOverTime();
	BOOL MoreThan(double ctime);	//단위는 1.0 초...
	BOOL LessThan(double ctime);	//단위는 1.0 초...
	void StartTimer();

	void SetTimerCheckEnd(){m_bchecking=FALSE;}
	bool IsChecking(){return m_bchecking;}

private:
	double m_CheckTime;
	ULONG StartTime;
	ULONG EndTime;
	bool m_bchecking;
};


//==============================================================================
//
//==============================================================================
class  AFX_EXT_CLASS CMyTime
{
public:
	int GetHour();
	int GetMonth();
	int GetYear();
	int GetDay();
	int GetMinute();
	int GetSecond();

	CTime mTime;
	CMyTime(){ 
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		min = 0;
		sec = 0;
	};
	~CMyTime(){ };

	void GetCurTime();

	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};
#endif