// EventLog_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "EventLog_List.h"
#include "afxdialogex.h"
#include "DlgAlarmZoom.h"


// CEventLog_List 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventLog_List, CDialogEx)

CEventLog_List::CEventLog_List(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventLog_List::IDD, pParent)
{
	m_bSortAscending = false;
}

CEventLog_List::~CEventLog_List()
{
}

void CEventLog_List::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLOG);

}


BEGIN_MESSAGE_MAP(CEventLog_List, CDialogEx)
	//ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_LOG, &CEventLog_List::OnLvnColumnclickListLog)
	ON_MESSAGE(WM_USER + 100, &CEventLog_List::OnListDoubleClick)
END_MESSAGE_MAP()


BOOL CEventLog_List::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_listLOG.InsertColumn(0, "ALID", 0, 50);		// Alarm Code
	m_listLOG.InsertColumn(1, "PART", 0, 90);		// Alarm Part
	m_listLOG.InsertColumn(2, "MESSAGE", 0, 850);	// Alarm Message
	m_listLOG.InsertColumn(3, "Occ Time", 0, 200);	// Occurred Time
	m_listLOG.InsertColumn(4, "Recovery", 0, 85);	// Recovery selected
	m_listLOG.InsertColumn(5, "Stop Time", 0, 85);	// Stop Time
	m_listLOG.InsertColumn(6, "User", 0, 85);		// User
	m_listLOG.InsertColumn(7, "Level", 0, 60);		// Alarm Level
	
	m_listLOG.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00004000);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEventLog_List::InitListGUI()
{
	m_listLOG.DeleteAllItems();

	MakeLog("List setting str");


	for (int i = 0; i < (int)CEventLogDlg::vDataErrLog.size(); i++) {
		CString str[8];
	
		str[0].Format("%s", CEventLogDlg::vDataErrLog[i].szEQP_ALID);
		str[1].Format("%s", CEventLogDlg::vDataErrLog[i].szPart);
		str[2] = CEventLogDlg::vDataErrLog[i].szMessage;
		str[3] = CEventLogDlg::vDataErrLog[i].strOccurred_Time;
		str[4] = CEventLogDlg::vDataErrLog[i].szRecovery;
		str[5].Format("%d", CEventLogDlg::vDataErrLog[i].nStop_Time);
		str[6].Format("%s", CEventLogDlg::vDataErrLog[i].szUser_Info);
		str[7].Format("%d", CEventLogDlg::vDataErrLog[i].nError_Level);
		
		m_listLOG.AddItem(str[0]);
	
		for (int j = 1; j < 8; j++)
		{
			m_listLOG.SetItemText(i, j, str[j]);
		}
	}
	
	m_listLOG.Complete();


	MakeLog("List setting end");
	// 0 : ALID / 1 : PART / 2 : MESSAGE / 3: Occ Time / 4 : Recovery / 5 : Stop Time / 6 : User / 7 : Level
	m_bSortAscending = TRUE;
}


//int CALLBACK CompareFuncLog(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//{
//	CString* pStr1 = (CString*)lParam1;		// ItemData 문자열로 형 변환
//	CString* pStr2 = (CString*)lParam2;
//	BOOL bSortAscending = (BOOL)lParamSort;	// 정렬 방식
//
//	// strcmp() - 0  : str1, str2 동일 문자열
//	//            1  : str1이 str2보다 사전 순서 뒤에 있다.
//	//            -1 : str1이 str2보다 사전 순서 앞에 있다.
//	int nResult = strcmp(*pStr1, *pStr2);
//
//	if (bSortAscending == TRUE) // 오름차순 그냥 반환
//		return nResult;
//	else						// 내림차순 반대로 반환
//		return -nResult;
//	return 0;
//
//}

//void CEventLog_List::InitSorting(int nColumn)
//{
//	m_bSortAscending = !m_bSortAscending;	// 내림차순
//
//	int nItem = m_listLOG.GetItemCount();	// GetItemCount() - List의 개수 구해주는 함수
//	CString** arStr = new CString*[nItem];	// 문자열 저장 공간 배열 Item의 개수 만큼 만든다.
//	for (int i = 0; i < nItem; i++) {
//		// 문자열 저장 공간 배열에 각 Item의 내용을 넣어준다.
//		arStr[i] = new CString(m_listLOG.GetItemText(i, nColumn));		// GetItemText() - Item의 내용 얻어오는 함수 
//		m_listLOG.SetItemData(i, (LPARAM)arStr[i]);					// SetItemData() - ItemData에 Setting 해준다.
//	}
//	// 콜백함수의 주소, 정렬방식(오름차순/내림차순) 넘겨준다.
//	m_listLOG.SortItems(CompareFuncLog, (LPARAM)m_bSortAscending);
//
//	// 할당된 문자열 저장 공간 배열 삭제
//	for (int i = 0; i < nItem; i++) {
//		delete arStr[i];
//	}
//	delete[]arStr;
//}

//void CEventLog_List::OnLvnColumnclickListLog(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//
//	// 각 이벤트에 맞게 형 변환해서 사용해야 한다.
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//	int nColumn = pNMListView->iSubItem;		// Column - 선택된 Column 번호
//	m_bSortAscending = !m_bSortAscending;	// 오름차순, 내림차순 Switching
//
//	int nItem = m_listLOG.GetItemCount();	// GetItemCount() - List의 개수 구해주는 함수
//	CString** arStr = new CString*[nItem];	// 문자열 저장 공간 배열 Item의 개수 만큼 만든다.
//	for (int i = 0; i < nItem; i++) {
//		// 문자열 저장 공간 배열에 각 Item의 내용을 넣어준다.
//		arStr[i] = new CString(m_listLOG.GetItemText(i, nColumn));	// GetItemText() - Item의 내용 얻어오는 함수 
//		m_listLOG.SetItemData(i, (LPARAM)arStr[i]);					// SetItemData() - ItemData에 Setting 해준다.
//	}
//	// 콜백함수의 주소, 정렬방식(오름차순/내림차순) 넘겨준다.
//	m_listLOG.SortItems(CompareFuncLog, (LPARAM)m_bSortAscending);
//
//	// 할당된 문자열 저장 공간 배열 삭제
//	for (int i = 0; i < nItem; i++) {
//		delete arStr[i];
//	}
//	delete[]arStr;
//
//	*pResult = 0;
//}

LRESULT CEventLog_List::OnListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	int nItem = (int)(lParam >> 16);
	int nSubItem = (int)(lParam & 0xFFFF);

	if (nItem < 0 || nSubItem < 0)
		return 0;

	CDlgAlarmZoom dlg;
	strcpy_s(dlg.m_stDataListErrLog.szEQP_ALID, sizeof(dlg.m_stDataListErrLog.szEQP_ALID), m_listLOG.GetItemText(nItem, 0));
	strcpy_s(dlg.m_stDataListErrLog.szPart, sizeof(dlg.m_stDataListErrLog.szPart), m_listLOG.GetItemText(nItem, 1));
	strcpy_s(dlg.m_stDataListErrLog.szMessage, sizeof(dlg.m_stDataListErrLog.szMessage), m_listLOG.GetItemText(nItem, 2));
	dlg.m_stDataListErrLog.strOccurred_Time.Format("%s", m_listLOG.GetItemText(nItem, 3));
	dlg.DoModal();

	return 0;
}

void CEventLog_List::MakeLog(LPCTSTR fmt, ...)
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

