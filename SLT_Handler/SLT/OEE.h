#pragma once

 typedef struct OEEData
 {
 	CString strDate;
 	int nTheoreticaloutput;
 	int nActualoutput;
 	double dOEE;
	//CString strOEE;
 }_stOEEData;

class COEE
{
public:
	COEE();
	~COEE();

	ST_DAILY_REPORT m_stDailyData;

	enum { e_Date, e_Month, e_Year, e_DateMonth, e_YearMonthDay, e_24Before_Year, e_Hour, e_Minute };
	enum { eJan = 1, eFeb, eMar, eApr, eMay, eJun, eJul, eAug, eSep, eOct, eNov, eDec };
	enum { eInit, e1Hour, e1Day };

public:
	void InitData();
	CString CheckCurDate(int nMonthDate);
	CString MonthReplaceToEng(int nMonth);
	BOOL Check24Hour();


	void UpdateOEE();
	void WriteData();
	void CalOEE();
public:
	CString m_strOEEFilePath;
	OEEData m_strOEEData;

private:
	
	std::vector<_stOEEData> vOEE;

	enum eOEEIdx
	{
		estrDate = 0,
		eTheoreticaloutput,
		eActualoutput,
		eOEE,
	};

};

