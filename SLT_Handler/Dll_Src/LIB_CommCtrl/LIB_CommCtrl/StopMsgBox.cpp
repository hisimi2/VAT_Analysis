// StopMsgBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StopMsgBox.h"
#include "afxdialogex.h"
#include <afxctl.h>

// CStopMsgBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStopMsgBox, CDialog)

CStopMsgBox::CStopMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CStopMsgBox::IDD, pParent)
{
	m_cx          = 110;
	m_cy          = 16;
	m_nFontHeight = 0;
	m_nFontWidth  = 0;
	m_nImagePos   = 0;

	strcpy(m_szErrorCode, _T("No Data"));
	strcpy(m_szSubCode, _T("No Data"));
	strcpy(m_szUnitName, _T("No Data"));
	m_nDesktopWidth = GetSystemMetrics( SM_CXFULLSCREEN ) ;		//by mjna [140930] Message Box Always Center
	m_nDesktopHeight = GetSystemMetrics( SM_CYFULLSCREEN ) ;	//by mjna [140930] Message Box Always Center

	memset(m_szSecondaryMsg, 0x00, sizeof(m_szSecondaryMsg));	// By jhjang [150507] Add OS Chinese Error Message

	m_nTimerPosChange = 0;
	m_nRes = 0;
	memset(m_strNo, 0x00, sizeof(m_strNo));

	memset(m_strYes, 0x00, sizeof(m_strYes));
	memset(m_strRetry, 0x00, sizeof(m_strRetry));
	memset(m_szTitle, 0x00, sizeof(m_szTitle));
	memset(m_szFirstMsg, 0x00, sizeof(m_szFirstMsg));

	m_icontype = 0;
	m_type = 0;
	m_error_type = 0;
	m_BitMap = 0;
	
}

CStopMsgBox::~CStopMsgBox()
{
	m_ErrorImage.DeleteObject();
}

void CStopMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECOVERY_HELP, m_bMyRecovery);
	DDX_Control(pDX, ID_MYYES, m_bMyYes);
	DDX_Control(pDX, ID_MYRETRY, m_bMyRetry);
	DDX_Control(pDX, ID_MYOK, m_bMyOk);
	DDX_Control(pDX, ID_MYNO, m_bMyNo);
	DDX_Control(pDX, ID_MYIGNORE, m_bMyIgnore);
	DDX_Control(pDX, ID_MYCANCEL, m_bMyCancel);
	DDX_Control(pDX, ID_MYABORT, m_bMyAbort);
	DDX_Control(pDX, IDC_MY_ICON, m_MyIcon);
	DDX_Control(pDX, IDC_MYMSGEDIT, m_MyMsgEdit);
	DDX_Control(pDX, IDC_IMAGE_POS, m_SetBitmap);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CStopMsgBox, CDialog)
	ON_BN_CLICKED(ID_MYABORT, OnMyabort)
	ON_BN_CLICKED(ID_MYCANCEL, OnMycancel)
	ON_BN_CLICKED(ID_MYIGNORE, OnMyignore)
	ON_BN_CLICKED(ID_MYOK, OnMyok)
	ON_BN_CLICKED(ID_MYOK2, OnMyok2)
	ON_BN_CLICKED(ID_MYRETRY, OnMyretry)
	ON_BN_CLICKED(ID_MYYES, OnMyyes)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_MYNO, OnMyno)
	ON_BN_CLICKED(ID_MYCANCEL3, OnMycancel3)
	ON_BN_CLICKED(ID_MYRETRY3, OnMyretry3)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RECOVERY_HELP, OnRecoveryHelp)
	//ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()					// By mjna [150203] Stop 또는 Run Message Box 뒤로 숨는 문제
END_MESSAGE_MAP()


// CStopMsgBox 메시지 처리기입니다.
void CStopMsgBox::OnMyok2() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDOK;
	DestroyWindow();
}

void CStopMsgBox::OnMyok() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDOK;
	DestroyWindow();
}

void CStopMsgBox::OnMycancel() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDCANCEL;
	DestroyWindow();
}

void CStopMsgBox::OnMyyes() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDYES;
	DestroyWindow();
}

void CStopMsgBox::OnMyno() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDNO;
	DestroyWindow();
}

void CStopMsgBox::OnMyabort() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDABORT;
	DestroyWindow();
}

void CStopMsgBox::OnMyignore() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDIGNORE;
	DestroyWindow();
}

void CStopMsgBox::OnMyretry() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDRETRY;
	DestroyWindow();
}

void CStopMsgBox::OnMyretry3() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDRETRY;
	DestroyWindow();
}

void CStopMsgBox::OnMycancel3() 
{
	// TODO: Add your control notification handler code here
	m_nRes = IDCANCEL;
	DestroyWindow();
}

BOOL CStopMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nDesktopWidth = GetSystemMetrics( SM_CXFULLSCREEN ) ;
	m_nDesktopHeight = GetSystemMetrics( SM_CYFULLSCREEN ) ;

	SetWindowText(m_szTitle);

	// Ok/Cancle
	//	m_bMyOk.SetIcon(IDI_OK, 32, 32);
	m_bMyOk.SetImageOffset(3);
	m_bMyOk.SetTextOffset(3);
	m_bMyOk.SetTextSize(10, TRUE);

	//	m_bMyCancel.SetIcon(IDI_CANCLE, 32, 32);
	m_bMyCancel.SetImageOffset(3);
	m_bMyCancel.SetTextOffset(3);
	m_bMyCancel.SetTextSize(10, TRUE);

	m_bMyYes.SetWindowText(m_strYes);
	//	m_bMyYes.SetIcon(IDI_YES, 32, 32);
	m_bMyYes.SetImageOffset(3);
	m_bMyYes.SetTextOffset(3);
	m_bMyYes.SetTextSize(10, TRUE);

	m_bMyNo.SetWindowText(m_strNo);
	//	m_bMyNo.SetIcon(IDI_NO, 32, 32);
	m_bMyNo.SetImageOffset(3);
	m_bMyNo.SetTextOffset(3);
	m_bMyNo.SetTextSize(10, TRUE);

	//	m_bMyAbort.SetIcon(IDI_ABORT, 32, 32);
	m_bMyAbort.SetImageOffset(3);
	m_bMyAbort.SetTextOffset(3);
	m_bMyAbort.SetTextSize(10, TRUE);

	//	m_bMyRetry.SetIcon(IDI_RETRY, 32, 32);
	m_bMyRetry.SetImageOffset(3);
	m_bMyRetry.SetTextOffset(3);
	m_bMyRetry.SetTextSize(10, TRUE);

	//	m_bMyIgnore.SetIcon(IDI_IGNORE, 32, 32);
	m_bMyIgnore.SetImageOffset(3);
	m_bMyIgnore.SetTextOffset(3);
	m_bMyIgnore.SetTextSize(10, TRUE);
	
	//	m_bMyRecovery.SetIcon(IDI_RECOVERY, 32, 32);
	m_bMyRecovery.SetImageOffset(3);
	m_bMyRecovery.SetTextOffset(3);
	m_bMyRecovery.SetTextSize(10, TRUE);
	m_bMyRecovery.EnableWindow(FALSE); // Build2.1		// By ksj [141031] Help Button Hide

	// Show/Hide 설정
	m_bMyOk.ShowWindow(SW_HIDE);
	m_bMyCancel.ShowWindow(SW_HIDE);
	m_bMyYes.ShowWindow(SW_HIDE);
	m_bMyNo.ShowWindow(SW_HIDE);
	m_bMyAbort.ShowWindow(SW_HIDE);
	m_bMyRetry.ShowWindow(SW_HIDE);
	m_bMyIgnore.ShowWindow(SW_HIDE);

	switch(m_type)
	{
	case M_OK :
		m_bMyOk.ShowWindow(SW_SHOW);
		break;

	case M_OKCANCEL :
		m_bMyOk.ShowWindow(SW_SHOW);
		m_bMyCancel.ShowWindow(SW_SHOW);
		break;

	case M_YESNO :
		m_bMyYes.ShowWindow(SW_SHOW);
		m_bMyNo.ShowWindow(SW_SHOW);
		break;

	case M_ABORTRETRYIGNORE :
		m_bMyAbort.ShowWindow(SW_SHOW);
		m_bMyRetry.ShowWindow(SW_SHOW);
		m_bMyIgnore.ShowWindow(SW_SHOW);
		break;

	case M_RETRYCANCEL :
		m_bMyCancel.ShowWindow(SW_SHOW);
		m_bMyRetry.ShowWindow(SW_SHOW);
		break;

	default :
		m_bMyOk.ShowWindow(SW_SHOW);
		break;
	}

	switch(m_icontype)
	{
	case M_ICONINFORMATION :
		m_MyIcon.SetBitmap(IDB_INFOICON);
		break;

	case M_ICONQUESTION :
		m_MyIcon.SetBitmap(IDB_QUESTIONICON);
		break;

	case M_ICONSTOP :
		m_MyIcon.SetBitmap(IDB_STOPICON);
		break;

	case M_ICONERROR :
		m_MyIcon.SetBitmap(IDB_ERRORICON);
		break;

	default :
		m_MyIcon.SetBitmap(IDB_INFOICON);
		break;
	}

	// Message's Size.....Setting
	if(m_nFontHeight!=0 || m_nFontWidth!=0)
	{
		LOGFONT lf;
		m_MyMsgEdit.GetFont()->GetLogFont(&lf);
		lf.lfWidth = m_nFontWidth;
		lf.lfHeight = m_nFontHeight;
		m_Font.CreateFontIndirect(&lf);
		m_MyMsgEdit.SetFont(&m_Font);
	}

	CRect rc;	this->GetWindowRect(&rc);
	//CRect rect;	GetDlgItem(IDC_IMAGE_POS)->GetWindowRect(&rect);
	if( m_nImagePos == 0 )
	{
		int x = m_nDesktopWidth/2 - rc.Width()/2;
		int y = m_nDesktopHeight/2 - rc.Height()/2;
		SetWindowPos(&wndTopMost, x, y, rc.Width(), rc.Height()-450/*-rect.Height()-50*/, SWP_NOMOVE);
	}
	else
	{
		m_SetBitmap.SetBitmap(m_BitMap);
	}

	//20130828 Unicode->Muli String 으로 변경
	CString Ch_Msg = _T("");
	Ch_Msg = (LPCSTR)(LPSTR)m_szFirstMsg;
	Ch_Msg.Replace(_T("\\r\\n"), _T("\r\n"));
	GetDlgItem(IDC_MYMSGEDIT)->SetWindowText(Ch_Msg);
	GetDlgItem(IDC_MYMSGEDIT3)->SetWindowText(m_szSecondaryMsg);	// By jhjang [150507] Add OS Chinese Error Message
	GetDlgItem(IDC_TEXTBOX_ERRCODE)->SetWindowText(m_szErrorCode);
	GetDlgItem(IDC_TEXTBOX_UNITNAME)->SetWindowText(m_szUnitName);

	CString sSubCode = _T("");
	sSubCode.Format("%s", m_szSubCode);
	GetDlgItem(IDC_TEXTBOX_SUBCODE)->SetWindowText(sSubCode);

	//m_ErrorImage.LoadBitmap(IDB_OFFSET_TABLE);

	// 생성시 카운트 시작
	m_tCreateTime = CTime::GetTickCount();	// by ksi[131001]

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CStopMsgBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CStopMsgBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}


HBRUSH CStopMsgBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

// 	if( m_icontype != M_ICONERROR ) 	return hbr;
// 
// 	HWND hwnd = pWnd->GetSafeHwnd();
// 	if(hwnd==GetDlgItem(IDC_MYMSGEDIT)->m_hWnd)
// 	{
// 		pDC->SetTextColor(RED);
// //		pDC->SetBkColor(WHITE);
// 		pDC->SetBkMode(TRANSPARENT);			// 글자를 쓸때 배경이 변경 안되게..
// //		return ::CreateSolidBrush(WHITE);	// Control의 바탕색
// 	}

	return hbr;
}

void CStopMsgBox::OnRecoveryHelp() // Build2.1
{
	
	SetRecoveryShowWindow();
}


BOOL CStopMsgBox::SetRecoveryShowWindow(int iShow) // Build2.1
{
	char m_html[100]="";
	

	if(m_szFirstMsg[0]=='/0')
		return 0;

	m_html[0] = m_szFirstMsg[0];
	m_html[1] = m_szFirstMsg[1];
	m_html[2] = m_szFirstMsg[2];
	m_html[3] = m_szFirstMsg[3];

//090525
	CString code1;
	code1 = *m_html;

//	HWND res;

	CString strFile;
	strFile.Format(_T("c:/TWHelp/TW Error List.chm::/kor/TWERROR%s.htm#%s"), code1, m_html);
	
	
// 	if(iShow == 1)
// 
// 		res=HtmlHelp(m_hWnd,(LPCSTR)strFile,HH_DISPLAY_TOPIC , 0);
// 
// 	else
// 		res=HtmlHelp(m_hWnd,(LPCSTR)strFile,HH_CLOSE_ALL , 0);

//	if(res==NULL) return 0;
	
	
	return 1;
}

BOOL CStopMsgBox::OnEraseBkgnd(CDC* pDC) 
{
	CDialog::OnEraseBkgnd(pDC);

	return FALSE;	// 20131221 hyb Warning 때문에 추가 함
}

void CStopMsgBox::OnDestroy()	// by ksi[130927]
{
	CDialog::OnDestroy();
	
	// 다이얼로그 종료시 카운트끝
	m_tCloseTime = CTime::GetTickCount();

	CTime t = CTime::GetCurrentTime();

	CString LogMessage;
	switch(m_type)
	{
	case M_OK:
		LogMessage.Format("%s", m_strYes);
		break;

	case M_OKCANCEL:
		if(IDOK == m_nRes)
			LogMessage.Format("%s", m_strYes);
		else
			LogMessage.Format("%s", m_strNo);
		break;

	case M_YESNO:
		if(m_nRes == IDYES)
			LogMessage.Format("%s", m_strYes);
		else
			LogMessage.Format("%s", m_strNo);
		break;

	case M_RETRYCANCEL:
		if(m_nRes == IDRETRY)
			LogMessage.Format("%s", m_strYes);
		else
			LogMessage.Format("%s", m_strNo);
		break;
	default:
		LogMessage.Format("%s", _T("OK"));
		break;
	}

	CTime tCur = m_tCloseTime.GetTime()-m_tCreateTime.GetTime();
}
/*=============================================================================
  DESCRIPT : Message Box 항상 화면 중앙으로 위치되도록 적용
  KEY WORD : //by mjna [140930] Message Box Always Center
  ARGUMENT : bShow, nStatus
  RETURN   : -
==============================================================================*/
void CStopMsgBox::OnShowWindow(BOOL bShow, UINT nStatus)	// by ksi[130927]
{
	CDialog::OnShowWindow(bShow, nStatus);

	// 다이얼로그를 화면 중앙으로 위치
	if(bShow == TRUE)
	{
		CRect rc, rc2;
// 		this->GetWindowRect(&rc);
// 		CTW_MSApp*	pApp = (CTW_MSApp *)AfxGetApp();
// 		CMainFrame*	pFrame = (CMainFrame *)pApp->GetMainWnd();
// 		pFrame->GetWindowRect(&rc2);
		this->GetWindowRect(&rc);
		int x,y,w,h;
		x = m_nDesktopWidth/2 - rc.Width()/2;		//by mjna [140930] Message Box Always Center
		y = m_nDesktopHeight/2  - rc.Height()/2;	//by mjna [140930] Message Box Always Center
		w = rc.Width();
		h = rc.Height();
		//MoveWindow(x, y, w, h);
		SetWindowPos(&wndTopMost, x, y, w, h, SWP_SHOWWINDOW );
	}
}

BOOL CStopMsgBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if((pMsg->message==WM_SYSKEYDOWN) && (pMsg->wParam==VK_F4) )
	{
		return TRUE;
	}
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CStopMsgBox::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	if( (m_nTimerPosChange == -1) && lpwndpos->hwndInsertAfter == NULL )
	{
		m_nTimerPosChange = SetTimer(2, 10*1000, NULL); // 10초 이후에 다시 위로 올린다.
		//TRACE("======>> CStopMsgBox Position Change Timer Started.============\n");
	}
}