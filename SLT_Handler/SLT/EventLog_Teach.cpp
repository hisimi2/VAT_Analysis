// EventLog_Teach.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "EventLog_Teach.h"
#include "afxdialogex.h"


// CEventLog_Teach 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventLog_Teach, CDialog)

CEventLog_Teach::CEventLog_Teach(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EVENT_TEACH_LOG, pParent)
{

}

CEventLog_Teach::~CEventLog_Teach()
{
}

void CEventLog_Teach::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEACH_LOG, m_listTeach);
}


BEGIN_MESSAGE_MAP(CEventLog_Teach, CDialog)
END_MESSAGE_MAP()


// CEventLog_Teach 메시지 처리기입니다.


BOOL CEventLog_Teach::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_listTeach.InsertColumn(0, "Robot", 0, 200);		// Robot Name
	m_listTeach.InsertColumn(1, "Position", 0, 200);		// Alarm Part
	m_listTeach.InsertColumn(2, "Axis", 0, 150);	// Alarm Message
	m_listTeach.InsertColumn(3, "Before", 0, 150);	// Occurred Time
	m_listTeach.InsertColumn(4, "After", 0, 150);	// Recovery selected
	m_listTeach.InsertColumn(5, "Occurred Time", 0, 250);	// Stop Time

	m_listTeach.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00004000);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEventLog_Teach::InitListGUI()
{
	m_listTeach.DeleteAllItems();

	MakeLog("List setting str");

	for (int i = 0; i < (int)CEventLogDlg::vDataTeachLog.size(); i++) {
		CString str[6];

		str[0].Format("%s", CEventLogDlg::vDataTeachLog[i].szRobot_Name);
		str[1].Format("%s", CEventLogDlg::vDataTeachLog[i].szPos_Name);
		str[2].Format("%s", CEventLogDlg::vDataTeachLog[i].szAxis_Name);
		str[3].Format("%f", CEventLogDlg::vDataTeachLog[i].dPosition_Before);
		str[4].Format("%f", CEventLogDlg::vDataTeachLog[i].dPosition_After);
		str[5] = CEventLogDlg::vDataTeachLog[i].strOccurred_Time;

		m_listTeach.AddItem(str[0]);

		for (int j = 1; j < 6; j++)
		{
			m_listTeach.SetItemText(i, j, str[j]);
		}
	}

	m_listTeach.Complete();


	MakeLog("List setting end");
	// 0 : ALID / 1 : PART / 2 : MESSAGE / 3: Occ Time / 4 : Recovery / 5 : Stop Time / 6 : User / 7 : Level
	//m_bSortAscending = TRUE;
}

void CEventLog_Teach::MakeLog(LPCTSTR fmt, ...)
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