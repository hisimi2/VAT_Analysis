#include "stdafx.h"
#include "OEE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COEE::COEE()
{
	m_strOEEFilePath = _T("");

	m_strOEEData.strDate = _T("");
	m_strOEEData.nTheoreticaloutput = 0;
	m_strOEEData.nActualoutput = 0;
	m_strOEEData.dOEE = 0.0;
}


COEE::~COEE()
{
}

void COEE::InitData()
{
	m_strOEEFilePath.Format("D:\\Techwing\\OEE\\%s_OEE.csv", CheckCurDate(e_24Before_Year));
	if (GetFileAttributes("D:\\Techwing\\OEE") == -1) // OEE 파일이 없다면 OEE 파일 생성
	{
		CreateDirectory(_T("D:\\Techwing\\OEE"), NULL);
	}

	CalOEE();
}

//실시간 Data Update
void COEE::UpdateOEE()
{
	m_stDailyData.Clear();
	double dTime = 0.0;
	CString strMonth = _T("");
	int nTheoretical = g_DMCont.m_dmHandlerOpt.GN(NDM11_Theoretical_UPH);

	CString strCurDate = CheckCurDate(e_YearMonthDay);
	g_DB.SELECT_DAILY_DATA_TBL(strCurDate, strCurDate, &m_stDailyData);

	CTime cTmCurrent = CTime::GetCurrentTime();
	dTime = cTmCurrent.GetHour() + (cTmCurrent.GetMinute() / 60.0);
	strMonth += CheckCurDate(e_DateMonth);

	OEEData stCalOEE;

	stCalOEE.strDate = strMonth;                // 날짜
	stCalOEE.nTheoreticaloutput = nTheoretical; //이론수치 공유 메모리 저장 필요
	stCalOEE.nActualoutput = m_stDailyData.nPassCnt; // 실제 수치	
	if ((stCalOEE.nTheoreticaloutput * dTime) * 100 == 0)
	{
		stCalOEE.dOEE = 0;
	}
	else
	{
		stCalOEE.dOEE = (stCalOEE.nActualoutput / (stCalOEE.nTheoreticaloutput * dTime)) * 100; // OEE percentage	
	}

	double dOEE;
	if (Check24Hour())  //24시간 넘으면 GUI 초기화
		dOEE = 0.00;
	else
		dOEE = stCalOEE.dOEE;

	g_DMCont.m_dmHandlerOpt.SD(DDM11_OEE, dOEE);

}

//OEE.csv Write
void COEE::WriteData()
{
	CString strAddPer = _T("");
	FILE *fp;
	if (!fopen_s(&fp, m_strOEEFilePath, "w+") && vOEE.size() > 0)  //파일 open 성공시, 0 return
	{
		fprintf_s(fp, "Date, Theoretical Output, Actual Output, OEE\n");
		for (size_t i = 0; i < (int)vOEE.size(); i++)
		{
			fprintf_s(fp, "%s, %d, %d,", vOEE[i].strDate, vOEE[i].nTheoreticaloutput * 24, vOEE[i].nActualoutput);
			strAddPer.Format("%.2f", vOEE[i].dOEE);
			strAddPer += _T("%");
			fprintf_s(fp, "%s\n", strAddPer);
		}
		fclose(fp);
	}
}

CString COEE::CheckCurDate(int nMonthDate)
{
	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strCurTime = _T("");
	CString strConvert = _T("");

	switch (nMonthDate)
	{
	case e_Date:
		strCurTime.Format("%02d", cTmCurrent.GetDay());
		break;

	case e_Month:
		strCurTime.Format("%02d", cTmCurrent.GetMonth());
		break;

	case e_Year:
		strCurTime.Format("%02d", cTmCurrent.GetYear());
		break;

	case e_DateMonth:
		strConvert = MonthReplaceToEng(cTmCurrent.GetMonth());
		strCurTime.Format("%02d-%s", cTmCurrent.GetDay(), strConvert);
		break;

	case e_YearMonthDay:
		strCurTime.Format("%04d-%02d-%02d", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay());
		break;

	case e_24Before_Year:
		cTmCurrent -= 24;
		strCurTime.Format("%04d", cTmCurrent.GetYear());
		break;

	case e_Hour:
		strCurTime.Format("%02d", cTmCurrent.GetHour());
		break;

	case e_Minute:
		strCurTime.Format("%02d", cTmCurrent.GetMinute());
		break;

	default:
		break;
	}

	return strCurTime;

}

CString COEE::MonthReplaceToEng(int nMonth)
{
	CString strConvert = _T("");
	switch (nMonth)
	{
	case eJan:
		strConvert.Format("%s", "Jan.");
		break;

	case eFeb:
		strConvert.Format("%s", "Feb.");
		break;

	case eMar:
		strConvert.Format("%s", "Mar.");
		break;

	case eApr:
		strConvert.Format("%s", "Apr.");
		break;

	case eMay:
		strConvert.Format("%s", "May.");
		break;

	case eJun:
		strConvert.Format("%s", "Jun.");
		break;

	case eJul:
		strConvert.Format("%s", "Jul.");
		break;

	case eAug:
		strConvert.Format("%s", "Aug.");
		break;

	case eSep:
		strConvert.Format("%s", "Sep.");
		break;

	case eOct:
		strConvert.Format("%s", "Oct.");
		break;

	case eNov:
		strConvert.Format("%s", "Nov.");
		break;

	case eDec:
		strConvert.Format("%s", "Dec.");
		break;

	default:
		break;
	}

	return strConvert;
}


BOOL COEE::Check24Hour()
{
	BOOL b24Hour;
	CTime cTmCurrent = CTime::GetCurrentTime();
	int nCurHour = cTmCurrent.GetHour();
	int nCurMin = cTmCurrent.GetMinute();

	if (nCurHour == 0 && nCurMin == 0)
		b24Hour = TRUE;
	else
		b24Hour = FALSE;

	return b24Hour;


}

//OEE 계산
void COEE::CalOEE()
{
	CTime cTmCurrent = CTime::GetCurrentTime();
	int nMonth = cTmCurrent.GetMonth();
	int nDay = cTmCurrent.GetDay();
	int nCalMonth = 1;
	int nCalDay = 1;
	CString strCurYear = CheckCurDate(e_Year);
	CString strCurDate = CheckCurDate(e_YearMonthDay);
	CString strFirstDate = _T("");
	strFirstDate.Format("%04d-%02d-%02d", cTmCurrent.GetYear(), nCalMonth, nCalDay);

	int ndays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if ((cTmCurrent.GetYear() % 4 == 0) && ((cTmCurrent.GetYear() % 100 != 0) || (cTmCurrent.GetYear() % 400 == 0))) //윤년 계산
	{
		ndays[1] = 29;
	}

	for (int i = 0; i < nMonth; i++) //오늘 날짜 까지 일수 계산
	{
		if( nMonth - 1 != i )
			nDay += ndays[i];
	}

	int nCntMonth = 0;
	for (int i = 0; i < nDay; i++)
	{
		m_stDailyData.Clear();
		g_DB.SELECT_DAILY_DATA_TBL(strFirstDate, strFirstDate, &m_stDailyData);
		double dTime = cTmCurrent.GetHour() + (cTmCurrent.GetMinute() / 60.0);
		int nTheoretical = g_DMCont.m_dmHandlerOpt.GN(NDM11_Theoretical_UPH);

		CString strConvert = MonthReplaceToEng(nCalMonth);

		m_strOEEData.strDate.Format("%02d-%s", nCalDay, strConvert); //날짜
		m_strOEEData.nTheoreticaloutput = nTheoretical; //이론수치 공유 메모리 저장 필요
		m_strOEEData.nActualoutput = m_stDailyData.nPassCnt; // 실제 수치	
		if (strFirstDate != strCurDate)
		{
			m_strOEEData.dOEE = (m_strOEEData.nActualoutput / (m_strOEEData.nTheoreticaloutput * 24.0)) * 100;
		}
		else 
		{
			if (dTime == 0)
				m_strOEEData.dOEE = 0;
			else
				m_strOEEData.dOEE = (m_strOEEData.nActualoutput / (m_strOEEData.nTheoreticaloutput * dTime)) * 100; // OEE percentage	
		}

		if((m_strOEEData.nTheoreticaloutput * dTime) * 100 == 0)
		{
			m_strOEEData.dOEE = 0;			
		}

		vOEE.push_back(m_strOEEData);

		nCalDay++;
		if (nCalDay == ndays[nCntMonth]+1)
		{
			nCntMonth++;
			nCalMonth++;
			nCalDay = 1;
		}

		strFirstDate.Format("%04d-%02d-%02d", cTmCurrent.GetYear(), nCalMonth, nCalDay);
	}

	WriteData();
}


















// 	m_stDailyData.Clear();
// 	double dTime = 0.0;
// 	CString strMonth = _T("");
// 	int nTheoretical = g_DMCont.m_dmHandlerOpt.GN(NDM11_Theoretical_UPH);
// 
// 	if (Check24Hour()) //24시간 지나면 하루 전 날 Data 저장
// 	{
// 		CString strLastDate = CheckCurDate(e_1DayBefore_YearMonthDay);
// 		g_DB.SELECT_DAILY_DATA_TBL(strLastDate, strLastDate, &m_stDailyData);
// 
// 		dTime = 24.0;
// 		strMonth += CheckCurDate(e_1DayBefore_DateMonth);
// 	}
// 	else
// 	{
// 		CString strCurDate = CheckCurDate(e_YearMonthDay);
// 		g_DB.SELECT_DAILY_DATA_TBL(strCurDate, strCurDate, &m_stDailyData);
// 
// 		CTime cTmCurrent = CTime::GetCurrentTime();
// 		dTime = cTmCurrent.GetHour() + (cTmCurrent.GetMinute() / 60.0);
// 		strMonth += CheckCurDate(e_DateMonth);
// 	}
// 
// 	//m_strOEEFilePath.Format("D:\\Techwing\\OEE\\%s_OEE.csv", CheckCurDate(e_24Before_Year));
// 
// 	
// 
// 
// 
// 
// 
// 	stCalOEE.strDate = strMonth;                // 날짜
// 	stCalOEE.nTheoreticaloutput = nTheoretical; //이론수치 공유 메모리 저장 필요
// 	stCalOEE.nActualoutput = m_stDailyData.nPassCnt; // 실제 수치	
// 	if ((stCalOEE.nTheoreticaloutput * dTime) * 100 == 0)
// 	{
// 		stCalOEE.dOEE = 0;
// 	}
// 	else
// 	{
// 		stCalOEE.dOEE = (stCalOEE.nActualoutput / (stCalOEE.nTheoreticaloutput * dTime)) * 100; // OEE percentage	
// 	}
// 
// 
// 	double dOEE;
// 	if (Check24Hour())  //24시간 넘으면 GUI 초기화
// 		dOEE = 0.00;
// 	else
// 		dOEE = stCalOEE.dOEE;
// 
// 	g_DMCont.m_dmHandlerOpt.SD(DDM11_OEE, dOEE);
