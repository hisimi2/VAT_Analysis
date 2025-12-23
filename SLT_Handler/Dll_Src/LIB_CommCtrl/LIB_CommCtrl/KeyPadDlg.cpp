// GetVal.cpp : implementation file
//

#include "stdafx.h"
#include "KeyPadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyPadDlg dialog
CKeyPadDlg::CKeyPadDlg(int maxchar, char* getnum, char* title,BOOL password, CWnd* pParent)
	: CDialog(CKeyPadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyPadDlg)
	//}}AFX_DATA_INIT
	m_nMaxChar   = maxchar;
	m_strGetVal  = getnum;
	m_strWndText = title;
	m_bPassword  = password;
	m_nCxChar = 0;
	m_nCyChar = 0;
}


void CKeyPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyPadDlg)
	DDX_Control(pDX, IDC_DISP_VAL, m_sDispValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyPadDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyPadDlg)
	ON_BN_CLICKED(IDC_BTN_BAR, OnBtnBar)
	ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON_A, OnButtonA)
	ON_BN_CLICKED(IDC_BUTTON_B, OnButtonB)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_C, OnButtonC)
	ON_BN_CLICKED(COMMCTRLDLL_IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_COMMA, OnButtonComma)
	ON_BN_CLICKED(IDC_BUTTON_D, OnButtonD)
	ON_BN_CLICKED(IDC_BUTTON_E, OnButtonE)
	ON_BN_CLICKED(IDC_BUTTON_F, OnButtonF)
	ON_BN_CLICKED(IDC_BUTTON_G, OnButtonG)
	ON_BN_CLICKED(IDC_BUTTON_H, OnButtonH)
	ON_BN_CLICKED(IDC_BUTTON_I, OnButtonI)
	ON_BN_CLICKED(IDC_BUTTON_J, OnButtonJ)
	ON_BN_CLICKED(IDC_BUTTON_K, OnButtonK)
	ON_BN_CLICKED(IDC_BUTTON_L, OnButtonL)
	ON_BN_CLICKED(IDC_BUTTON_M, OnButtonM)
	ON_BN_CLICKED(IDC_BUTTON_N, OnButtonN)
	ON_BN_CLICKED(IDC_BUTTON_O, OnButtonO)
	ON_BN_CLICKED(IDC_BUTTON_P, OnButtonP)
	ON_BN_CLICKED(IDC_BUTTON_Q, OnButtonQ)
	ON_BN_CLICKED(IDC_BUTTON_R, OnButtonR)
	ON_BN_CLICKED(IDC_BUTTON_S, OnButtonS)
	ON_BN_CLICKED(IDC_BUTTON_T, OnButtonT)
	ON_BN_CLICKED(IDC_BUTTON_U, OnButtonU)
	ON_BN_CLICKED(IDC_BUTTON_V, OnButtonV)
	ON_BN_CLICKED(IDC_BUTTON_W, OnButtonW)
	ON_BN_CLICKED(IDC_BUTTON_X, OnButtonX)
	ON_BN_CLICKED(IDC_BUTTON_Y, OnButtonY)
	ON_BN_CLICKED(IDC_BUTTON_Z, OnButtonZ)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SLASH, OnButtonSlash)
	ON_BN_CLICKED(IDC_BUTTON_UNDERLINE, OnButtonUnderline)
	ON_BN_CLICKED(IDC_BUTTON_COLON, OnButtonColon)
	ON_BN_CLICKED(IDC_BUTTON_EQUAL, OnButtonEqual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyPadDlg message handlers

void CKeyPadDlg::OnBtnBar() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "-";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "1";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "0";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "2";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "3";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "4";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "5";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "6";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "7";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "8";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "9";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonA() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "A";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonB() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "B";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonBack() 
{
	// TODO: Add your control notification handler code here
	int len = m_strGetVal.GetLength();
	if( len )
	{
		m_strGetVal.SetAt(len-1,' ');
		m_strGetVal.TrimRight();
		UpdateDisplay();
	}
}

void CKeyPadDlg::OnButtonC() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "C";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_strGetVal = "";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonComma() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += ".";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonD() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "D";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonE() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "E";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonF() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "F";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonG() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "G";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonH() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "H";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonI() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "I";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonJ() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "J";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonK() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "K";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonL() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "L";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonM() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "M";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonN() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "N";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonO() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "O";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonP() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "P";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonQ() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "Q";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonR() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "R";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonS() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "S";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonT() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "T";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonU() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "U";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonV() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "V";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonW() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "W";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonX() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "X";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonY() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "Y";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonZ() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "Z";	
	UpdateDisplay();
}

void CKeyPadDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CKeyPadDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CKeyPadDlg::UpdateDisplay(COLORREF bkcolor)
{
	if(m_bPassword)
	{
		CString tmp = _T("");
		tmp.Empty();
		if(!m_strGetVal.IsEmpty())
		{
			for(int i=0;i<m_strGetVal.GetLength();i++)
				tmp += "*";
		}
		m_sDispValue.SetText(tmp);
	}
	else
		m_sDispValue.SetText(m_strGetVal);
}

BOOL CKeyPadDlg::VerifyMaxChar()
{
	if( m_strGetVal.GetLength() >= m_nMaxChar ) return FALSE;
	else return TRUE;
}

void CKeyPadDlg::InitStaticDispWnd()
{
    CStatic* pRect = (CStatic*)GetDlgItem(IDC_DISP_VAL);
    pRect->GetWindowRect(&m_rect);
    pRect->DestroyWindow();
    ScreenToClient(&m_rect);

    TEXTMETRIC tm;
    CClientDC dc(this);
    dc.GetTextMetrics(&tm);
    m_nCxChar = tm.tmAveCharWidth;
    m_nCyChar = tm.tmHeight - tm.tmDescent;

	ReleaseDC(&dc); // TEST
}

BOOL CKeyPadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDOK)->SetFocus();

	// TODO: Add extra initialization here
	SetWindowText(m_strWndText);	
	m_sDispValue
		.SetFontSize(13)
		.SetFontBold(TRUE)
		.SetColor(RGB(0,0,0), RGB(255,255,255));

	if(m_nMaxChar <= 0) m_nMaxChar = 1;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyPadDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateDisplay();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CKeyPadDlg::OnButtonSlash() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "\\";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonUnderline() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "_";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonColon() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += ":";	
	UpdateDisplay();
}

void CKeyPadDlg::OnButtonEqual() 
{
	// TODO: Add your control notification handler code here
	if( !VerifyMaxChar() ) return;

	m_strGetVal += "=";	
	UpdateDisplay();
}
