
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SLT.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_ALARM_REPORT, OnReceivedAssignSkip)
//	ON_MESSAGE(WM_UPDATE_MAIN_DISPLAY,	OnUserFuncMsg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// LRESULT CMainFrame::OnUserFuncMsg(WPARAM wParam, LPARAM lParam)
// {
// 	CView* pView = (CView*)GetActiveView();
// 	pView->SendMessage(WM_UPDATE_MAIN_DISPLAY, UM_YIELD_DIALOG_CHANGE, NULL);
// 
// 	return 0L; 
// }
// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_nMode = 0; 
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString strAppName(AfxGetAppName());
	CWnd *pWnd = FindWindow(NULL, strAppName);
	if (pWnd != NULL)
	{
		AfxMessageBox("GUI Program is already execute!!!!\r\n");
		return -1;
	}
		

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

// 	if (!m_wndStatusBar.Create(this))
// 	{
// 		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
// 		return -1;      // 만들지 못했습니다.
// 	}

	// Movewindow and change style
	CRect m_cRectMaximize;
	GetWindowRect(&m_cRectMaximize);  
	MoveWindow(0, 0, m_cRectMaximize.Width(), m_cRectMaximize.Height(), TRUE);  
	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);  
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED);  

//	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	this->SetWindowText(_T("TW_SLT"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.cx = MAX_WIN_X_SIZE;    //가로 크기
	cs.cy = MAX_WIN_Y_SIZE;    //세로 크기

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~FWS_ADDTOTITLE; 

	if(cs.hMenu!=NULL){
		::DestroyMenu(cs.hMenu);      // delete menu if loaded
		cs.hMenu = NULL;              // no menu for this window
	}

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

LRESULT CMainFrame::OnReceivedAssignSkip(WPARAM wParam, LPARAM lParam)
{
	CDataAssignDlg* pAssignMsgBox = new CDataAssignDlg(TRUE);
	pAssignMsgBox->SetPosNum((int)wParam);
	pAssignMsgBox->SetSkiptoAssignFlag(TRUE);
	pAssignMsgBox->Create(IDD_DIALOG_DVC_CHANGE, this);
	pAssignMsgBox->ShowWindow(SW_SHOW);

	return 0L;
}