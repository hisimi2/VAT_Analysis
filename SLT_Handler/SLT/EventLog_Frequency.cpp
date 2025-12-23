// EventLog_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "EventLog_Frequency.h"
#include "afxdialogex.h"

// CEventLog_Frequency 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventLog_Frequency, CDialogEx)

CEventLog_Frequency::CEventLog_Frequency(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventLog_Frequency::IDD, pParent)
{

}

CEventLog_Frequency::~CEventLog_Frequency()
{
}

void CEventLog_Frequency::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG_FREQUENCY, m_listLOG);
}


BEGIN_MESSAGE_MAP(CEventLog_Frequency, CDialogEx)
END_MESSAGE_MAP()


BOOL CEventLog_Frequency::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listLOG.InsertColumn(0, "ALID", 0, 50);		// Alarm Code
	m_listLOG.InsertColumn(1, "PART", 0, 90);		// Alarm Part
	m_listLOG.InsertColumn(2, "MESSAGE", 0, 850);	// Alarm Message
	m_listLOG.InsertColumn(3, "Count", 0, 60);	// Occurred Time
	m_listLOG.InsertColumn(4, "Rate", 0, 60);	// Recovery selected
	m_listLOG.InsertColumn(5, "Stop Time", 0, 85);	// Stop Time

	m_listLOG.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00004000);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


bool veactorcompare(ST_ERROR_MESSAGE a, ST_ERROR_MESSAGE b) {
	return atoi(a.szEQP_ALID) < atoi(b.szEQP_ALID);

}
void CEventLog_Frequency::InitListGUI()
{
	m_listLOG.DeleteAllItems();

	MakeLog("Frequency setting str");
	
	sort(CEventLogDlg::vDataErrLog.begin(), CEventLogDlg::vDataErrLog.end(), veactorcompare);

	CString strID = _T("");
	int nIdx = 0;
	int nCnt = 1;
	int nStopTime = 0;
	int nTotalCnt = (int)CEventLogDlg::vDataErrLog.size();

	for (int i = 0; i < nTotalCnt; i++) {
		if (i == 0) {
			strID.Format("%s", CEventLogDlg::vDataErrLog[i].szEQP_ALID);
		}
		else {
			if (strID.Compare(CEventLogDlg::vDataErrLog[i].szEQP_ALID) != 0) {
				CString str[6];

				str[0].Format("%s", CEventLogDlg::vDataErrLog[i-1].szEQP_ALID);
				str[1].Format("%s", CEventLogDlg::vDataErrLog[i-1].szPart);
				str[2] = CEventLogDlg::vDataErrLog[i-1].szMessage;
				str[3].Format("%d", nCnt);
				str[4].Format("%.0f%%", ((double)nCnt / (double)nTotalCnt * 100.0) );
				str[5].Format("%d", nStopTime);

				m_listLOG.AddItem(str[0]);

				for (int j = 1; j < 6; j++)
				{
					m_listLOG.SetItemText(nIdx, j, str[j]);
				}

				nIdx++;
				strID.Format("%s", CEventLogDlg::vDataErrLog[i].szEQP_ALID);
				nCnt = 1;
				nStopTime = CEventLogDlg::vDataErrLog[i].nStop_Time;
			}
			else {
				nCnt++;
				nStopTime =+ CEventLogDlg::vDataErrLog[i].nStop_Time;
			}

			if (i == nTotalCnt - 1) {
				CString str[6];

				str[0].Format("%s", CEventLogDlg::vDataErrLog[i].szEQP_ALID);
				str[1].Format("%s", CEventLogDlg::vDataErrLog[i].szPart);
				str[2] = CEventLogDlg::vDataErrLog[i].szMessage;
				str[3].Format("%d", nCnt);
				str[4].Format("%.0f%%", ((double)nCnt / (double)nTotalCnt * 100.0));
				str[5].Format("%d", nStopTime);

				m_listLOG.AddItem(str[0]);

				for (int j = 1; j < 6; j++)
				{
					m_listLOG.SetItemText(nIdx, j, str[j]);
				}
			}

		}
	}
	m_listLOG.Complete();

	MakeLog("Frequency setting end");
	// 초기정렬 내림차순으로 정리
	// 0 : ALID / 1 : PART / 2 : MESSAGE / 3: count / 4 : Rate / 5 : Stop Time
	MakeLog("Frequency sorting str");
	m_listLOG.SortByCol_Int(3,TRUE);

	MakeLog("Frequency sorting end");
}

//int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//{
//	CString* pStr1 = (CString*)lParam1;		// ItemData 문자열로 형 변환
//	CString* pStr2 = (CString*)lParam2;
//	BOOL bSortAscending = (BOOL)lParamSort;	// 정렬 방식
//
//											// strcmp() - 0  : str1, str2 동일 문자열
//											//            1  : str1이 str2보다 사전 순서 뒤에 있다.
//											//            -1 : str1이 str2보다 사전 순서 앞에 있다.
//	int nResult = atof(*pStr1) > atof(*pStr2) ? 1 : -1; //strcmp(*pStr1, *pStr2);
//
//
//	if (bSortAscending == TRUE) // 오름차순 그냥 반환
//		return nResult;
//	else						// 내림차순 반대로 반환
//		return -nResult;
//	return 0;
//
//}

//void CEventLog_Frequency::InitSorting(int nColumn)
//{
//	int nItem = m_listLOG.GetItemCount();	// GetItemCount() - List의 개수 구해주는 함수
//	CString** arStr = new CString*[nItem];	// 문자열 저장 공간 배열 Item의 개수 만큼 만든다.
//	for (int i = 0; i < nItem; i++) {
//		// 문자열 저장 공간 배열에 각 Item의 내용을 넣어준다.
//		arStr[i] = new CString(m_listLOG.GetItemText(i, nColumn));		// GetItemText() - Item의 내용 얻어오는 함수 
//		m_listLOG.SetItemData(i, (LPARAM)arStr[i]);					// SetItemData() - ItemData에 Setting 해준다.
//	}
//	// 콜백함수의 주소, 정렬방식(오름차순/내림차순) 넘겨준다.
//	m_listLOG.SortItems(CompareFunc, (LPARAM)FALSE);
//
//	// 할당된 문자열 저장 공간 배열 삭제
//	for (int i = 0; i < nItem; i++) {
//		delete arStr[i];
//	}
//	delete[]arStr;
//}

bool compare(std::pair<int, CString> a, std::pair<int, CString> b) {
	return a.first > b.first;
}

CString CEventLog_Frequency::MakeExpertData(std::vector<ST_ERROR_MESSAGE> vData)
{
	sort(vData.begin(), vData.end(), veactorcompare);

	CString strTmp = _T(""), strMsg = _T("");
	CString strID= _T("");
	int nCnt = 1;
	int nStopTime = 0;
	int nTotalCnt = (int)vData.size();
	std::vector<std::pair<int, CString>> vSortData;

	for (int i = 0; i < nTotalCnt; i++) {
		if (i == 0) {
			strID.Format("%s", vData[i].szEQP_ALID);
		}
		else {
			if (strID.Compare(vData[i].szEQP_ALID) != 0) {
				
				strMsg.Format("%s", vData[i-1].szMessage);
				strMsg.Replace("\r\n", "");
				strMsg.Replace(",", "");
				strMsg.Replace("\n", "");

				strTmp.Format("%s,%s,%s,%d,%.0f%%,%d", vData[i - 1].szEQP_ALID, vData[i - 1].szPart, strMsg, nCnt, ((double)nCnt / (double)nTotalCnt * 100.0), nStopTime);

				std::pair<int, CString> p;
				p.first = nCnt;
				p.second = strTmp;
				vSortData.push_back(p);

				strID.Format("%s", vData[i].szEQP_ALID);
				nCnt = 1;
				nStopTime = vData[i].nStop_Time;
			}
			else {
				nCnt++;
				nStopTime =+ vData[i].nStop_Time;
			}
			if (i == nTotalCnt - 1) {
				strMsg.Format("%s", vData[i].szMessage);
				strMsg.Replace("\r\n", "");
				strMsg.Replace(",", "");
				strMsg.Replace("\n", "");

				strTmp.Format("%s,%s,%s,%d,%.0f%%,%d", vData[i].szEQP_ALID, vData[i].szPart, strMsg, nCnt, ((double)nCnt / (double)nTotalCnt * 100.0), nStopTime);

				std::pair<int, CString> p;
				p.first = nCnt;
				p.second = strTmp;
				vSortData.push_back(p);
			}

		}
	}

	sort(vSortData.begin(), vSortData.end(), compare);

	CString strData = _T(""); 
	for (int i = 0; i < (int)vSortData.size(); i++)
	{
		CString strDataTmp = vSortData[i].second;
		strData += strDataTmp;
		strData += "\r\n";
	}

	return strData;
}

void CEventLog_Frequency::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}