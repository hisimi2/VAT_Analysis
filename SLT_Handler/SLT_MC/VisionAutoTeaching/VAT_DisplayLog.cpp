// VAT_DisplayLog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VAT_DisplayLog.h"
#include "afxdialogex.h"


// CVAT_DisplayLog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVAT_DisplayLog, CDialogEx)

CVAT_DisplayLog::CVAT_DisplayLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_DISPLAY_LOG, pParent)
{

}

CVAT_DisplayLog::~CVAT_DisplayLog()
{
}

void CVAT_DisplayLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnExit);
	DDX_Control(pDX, IDC_BTN_LOG_FILE, m_BtnLog);

	DDX_Control(pDX, IDC_LIST_LOAD_TRAY_PP, m_LoadTrayPPList);
	DDX_Control(pDX, IDC_LIST_UNLOAD_TRAY_PP, m_UnloadTrayPPList);
	DDX_Control(pDX, IDC_LIST_TEST_PP_1, m_TestPP1List);
	DDX_Control(pDX, IDC_LIST_TEST_PP_2, m_TestPP2List);
}


BEGIN_MESSAGE_MAP(CVAT_DisplayLog, CDialogEx)
	ON_MESSAGE(UM_VAT_SEQ_MSG_PRINT_LOG, &CVAT_DisplayLog::OnReceiveLogDataFromSequence)
	ON_BN_CLICKED(IDC_BTN_LOG_FILE, &CVAT_DisplayLog::OnBnClickedBtnLogFile)
END_MESSAGE_MAP()

BOOL CVAT_DisplayLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Reset List Box.
	ClearListBox();

	//Button 초기화
	InitBtnControl();

	// LOG Title Bar
	CDSSPanel *pLoadTrayPpPanel = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_LOAD_TRAY_PP);
	CDSSPanel *pUnloadTrayPpPanel = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_UNLOAD_TRAY_PP);
	CDSSPanel *pTestPp1Panel = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_TEST_PP_1);
	CDSSPanel *pTestPp2Panel = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_TEST_PP_2);


	pLoadTrayPpPanel->SetBackColor(LLIGHTCYAN);
	pUnloadTrayPpPanel->SetBackColor(LLIGHTCYAN);
	pTestPp1Panel->SetBackColor(LLIGHTCYAN);
	pTestPp2Panel->SetBackColor(LLIGHTCYAN);

// 	pTrayPPLog->SetWindowText(_T("Tray PP Area"));
// 	pTestPP1Log->SetWindowText(_T("Test PP #1 Area"));
// 	pTestPP2Log->SetWindowText(_T("Test PP #2 Area"));

	HWND hOperDlg = GetSafeHwnd();
	g_TaskTrayLoadPP.SetVatLogHandle(hOperDlg);
	g_TaskTrayUnloadPP.SetVatLogHandle(hOperDlg);
	g_TaskTestPP[eTestPP_1].SetVatLogHandle(hOperDlg);
	g_TaskTestPP[eTestPP_2].SetVatLogHandle(hOperDlg);


	return TRUE;
}

afx_msg LRESULT CVAT_DisplayLog::OnReceiveLogDataFromSequence(WPARAM wParam, LPARAM lParam)
{
	int nHand = (int)wParam;
	char *strLog = (char *)lParam;

	CListBox *LogList = NULL;
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:		LogList = &m_LoadTrayPPList; break;
	case VAT_HAND_UNLOAD_TRAYPP:	LogList = &m_UnloadTrayPPList; break;
	case VAT_HAND_TESTPP1:			LogList = &m_TestPP1List; break;
	case VAT_HAND_TESTPP2:			LogList = &m_TestPP2List; break;
	default:
		break;
	}

	if (LogList != NULL)
	{
		LogList->AddString((CString)strLog);
		LogList->SetTopIndex(LogList->GetCount() - 1);
		LogList->SetCurSel(LogList->GetCount() - 1);
		SetHorizontalScroll(*LogList);
	}

	return 0L;
}

void CVAT_DisplayLog::SetHorizontalScroll(CListBox &LogList)
{
	CString str;
	CSize sz;
	int dx = 0;
	CDC* pDC = LogList.GetDC();
	for (int i = 0; i < LogList.GetCount(); i++)
	{
		LogList.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	LogList.ReleaseDC(pDC);
	LogList.SetHorizontalExtent(dx);
}

void CVAT_DisplayLog::ClearListBox()
{
	m_LoadTrayPPList.ResetContent();
	m_UnloadTrayPPList.ResetContent();
	m_TestPP1List.ResetContent();
	m_TestPP2List.ResetContent();
}

void CVAT_DisplayLog::InitBtnControl()
{
	//Button 초기화.
	CFont BtnFont;
	BtnFont.CreatePointFont(120, "Fixedsys");

	m_BtnExit.SetFont(&BtnFont);
	m_BtnExit.SetWindowText(_T("EXIT"));
	m_BtnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnExit.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLog.SetFont(&BtnFont);
	m_BtnLog.SetWindowText(_T("LOG FILE"));
	m_BtnLog.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLog.SetAlign(CButtonST::ST_ALIGN_VERT);

	BtnFont.Detach();
}

void CVAT_DisplayLog::OnBnClickedBtnLogFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strPath;
	strPath.Format("D:\\Techwing\\VAT_LOG\\%4d%02d%02d", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay());
	CFileFind finder;

	BOOL bWorking = finder.FindFile(strPath + "*.*");

	CString fileName;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsArchived())
		{
			CString _filename = finder.GetFileName();

			if (_filename == "." || _filename == ".." || _filename == "Thumbs.db")
				continue;

			fileName = finder.GetFileName();
		}
	}
	strPath += fileName;
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOW);

}
