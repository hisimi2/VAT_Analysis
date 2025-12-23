// KeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NumberBox.h"

#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumberBoxDlg dialog

CNumberBoxDlg::CNumberBoxDlg(int maxchar, char* getnum, char* title, CWnd* pParent , char* cMax, char* cMin)
	: CDialog(CNumberBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumberBoxDlg)
	//}}AFX_DATA_INIT
	m_intMaxChar    = maxchar;
	m_strGetNum     = getnum;
	m_strCurrentNum = getnum;
	m_strWndText    = title;
	m_strMaxValue   = cMax;
	m_strMinValue   = cMin;
	m_bFirstTime    = TRUE;
	m_strSetData = _T("");
	m_dCurrent = 0.0;
	m_dpoint = 0.0;

	memset(m_cCurrent, 0x00, sizeof(m_cCurrent));
	m_nCyChar =0;
	m_nCxChar =0;
}

CNumberBoxDlg::~CNumberBoxDlg()
{

}

void CNumberBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_0      , m_btnN0   );
	DDX_Control(pDX, IDC_BTN_1      , m_btnN1   );
	DDX_Control(pDX, IDC_BTN_2      , m_btnN2   );
	DDX_Control(pDX, IDC_BTN_3      , m_btnN3   );
	DDX_Control(pDX, IDC_BTN_4      , m_btnN4   );
	DDX_Control(pDX, IDC_BTN_5      , m_btnN5   );
	DDX_Control(pDX, IDC_BTN_6      , m_btnN6   );
	DDX_Control(pDX, IDC_BTN_7      , m_btnN7   );
	DDX_Control(pDX, IDC_BTN_8      , m_btnN8   );
	DDX_Control(pDX, IDC_BTN_9      , m_btnN9   );
	DDX_Control(pDX, IDC_BTN_DOT    , m_btnDot  );
	DDX_Control(pDX, IDC_BTN_NEGA   , m_btnMinus);

	DDX_Control(pDX, IDC_BTN_BACK   , m_btnBkSp);
	DDX_Control(pDX, IDC_BTN_CLEAR  , m_btnClr);
	DDX_Control(pDX, IDC_BTN_CANCLE , m_btnCancel);
	DDX_Control(pDX, IDC_BTN_OK     , m_btnOk);
	DDX_Control(pDX, IDC_EDIT_CUR, m_EditCurData);
	DDX_Control(pDX, IDC_EDIT_MAX, m_EditMaxData);
	DDX_Control(pDX, IDC_EDIT_MIN, m_EditMinData);
	//DDX_Control(pDX, IDC_EDIT_SET, m_EditSetData);
	DDX_Text(pDX, IDC_EDIT_SET, m_strSetData);
}


BEGIN_MESSAGE_MAP(CNumberBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CNumberBoxDlg)
	ON_WM_PAINT()	
	ON_BN_CLICKED(IDC_BTN_7, &CNumberBoxDlg::OnBnClickedBtn7)
	ON_BN_CLICKED(IDC_BTN_8, &CNumberBoxDlg::OnBnClickedBtn8)
	ON_BN_CLICKED(IDC_BTN_0, &CNumberBoxDlg::OnBnClickedBtn0)
	ON_BN_CLICKED(IDC_BTN_1, &CNumberBoxDlg::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_2, &CNumberBoxDlg::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN_3, &CNumberBoxDlg::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_BTN_4, &CNumberBoxDlg::OnBnClickedBtn4)
	ON_BN_CLICKED(IDC_BTN_5, &CNumberBoxDlg::OnBnClickedBtn5)
	ON_BN_CLICKED(IDC_BTN_6, &CNumberBoxDlg::OnBnClickedBtn6)
	ON_BN_CLICKED(IDC_BTN_9, &CNumberBoxDlg::OnBnClickedBtn9)
	ON_BN_CLICKED(IDC_BTN_NEGA, &CNumberBoxDlg::OnBnClickedBtnNega)
	ON_BN_CLICKED(IDC_BTN_DOT, &CNumberBoxDlg::OnBnClickedBtnDot)
	ON_BN_CLICKED(IDC_BTN_NE01, &CNumberBoxDlg::OnBnClickedBtnNe01)
	ON_BN_CLICKED(IDC_BTN_PO01, &CNumberBoxDlg::OnBnClickedBtnPo01)
	ON_BN_CLICKED(IDC_BTN_NE05, &CNumberBoxDlg::OnBnClickedBtnNe05)
	ON_BN_CLICKED(IDC_BTN_PO05, &CNumberBoxDlg::OnBnClickedBtnPo05)
	ON_BN_CLICKED(IDC_BTN_NE1, &CNumberBoxDlg::OnBnClickedBtnNe1)
	ON_BN_CLICKED(IDC_BTN_PO1, &CNumberBoxDlg::OnBnClickedBtnPo1)
	ON_BN_CLICKED(IDC_BTN_NE10, &CNumberBoxDlg::OnBnClickedBtnNe10)
	ON_BN_CLICKED(IDC_BTN_PO10, &CNumberBoxDlg::OnBnClickedBtnPo10)
	ON_BN_CLICKED(IDC_BTN_BACK, &CNumberBoxDlg::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CNumberBoxDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_CANCLE, &CNumberBoxDlg::OnBnClickedBtnCancle)
	ON_BN_CLICKED(IDC_BTN_OK, &CNumberBoxDlg::OnBnClickedBtnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////InitStaticDis//////////////////////////////////////////////////////////////////////
// CNumberBoxDlg message handlers

// void CNumberBoxDlg::InitStaticDispWnd()
// {
//     TEXTMETRIC tm;
//     CClientDC dc(this);
//     dc.GetTextMetrics(&tm);
//     m_nCxChar = tm.tmAveCharWidth;
//     m_nCyChar = tm.tmHeight - tm.tmDescent;
// }

BOOL CNumberBoxDlg::VerifyMaxChar()
{
	if( m_strGetNum.GetLength() >= m_intMaxChar ) return FALSE;
	else return TRUE;
}

void CNumberBoxDlg::UpdateDisplay()
{
	//GetDlgItem(IDC_EDIT_SET)->SetWindowText(m_strGetNum);
//	SetDlgItemText(IDC_EDIT_SET, m_strGetNum);
	GetDlgItem(IDC_EDIT_SET)->SetWindowText(m_strGetNum);	
	//GetDlgItem(IDC_EDIT_CUR)->SetWindowText(m_strGetNum);
}

BOOL CNumberBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	OnGuiBtnSet();

	GetDlgItem(IDC_EDIT_CUR)->SetWindowText(m_strCurrentNum);
	GetDlgItem(IDC_EDIT_MAX)->SetWindowText(m_strMaxValue);
	GetDlgItem(IDC_EDIT_MIN)->SetWindowText(m_strMinValue);

	GetDlgItem(IDC_EDIT_SET)->SetWindowText(m_strGetNum);

	SetWindowText(m_strWndText);

	if(m_intMaxChar <= 0) m_intMaxChar = 1;

	GetDlgItem(IDC_BTN_CANCLE)->SetFocus();

	CFont EditFont;
	EditFont.CreatePointFont( 200, TEXT( "MS Sans Serif" ) );
	GetDlgItem( IDC_EDIT_SET )->SetFont( &EditFont, TRUE );
	EditFont.Detach();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CNumberBoxDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	UpdateDisplay();
	SetWindowText(m_strWndText);
	// Do not call CDialog::OnPaint() for painting messages
}


void CNumberBoxDlg::SetWindowTitle(LPCTSTR title)
{
	m_strWndText = title;
}


void CNumberBoxDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CNumberBoxDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

/*================================================================================
  DESCRIPT : 현재 값이 저장되어 있는 멤버 변수를 double 형태로 변환하여 Point만큼 가감
  KEY WORD : // 2014.06.23 JH string -> double 변환하여 현재 값에 point 만큼 가감
  RETURN   : None
  ARGUMENT : point : 가감 수치(0~9), sign : 가감 식별
  CONTENT  : sign 변수의 상태에 따라 Point 만큼 가감
=================================================================================*/
void CNumberBoxDlg::GetStoD(double point, BOOL sign)
{
	m_dCurrent = atof(m_strGetNum);
	if (sign == TRUE)
	{
		m_dCurrent = m_dCurrent + point;
	}
	else
	{
		m_dCurrent = m_dCurrent - point;
	}

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << m_dCurrent;
	m_strGetNum = ss.str().c_str();
}

void CNumberBoxDlg::OnBnClickedBtn0()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();

	m_strGetNum += "0";

	if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtn1()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "1";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtn2()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "2";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtn3()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "3";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtn4()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "4";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtn5()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "5";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtn6()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "6";
	UpdateDisplay();
}

void CNumberBoxDlg::OnBnClickedBtn7()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "7";
	UpdateDisplay();
}

void CNumberBoxDlg::OnBnClickedBtn8()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "8";
	UpdateDisplay();
}

void CNumberBoxDlg::OnBnClickedBtn9()
{
	if( !VerifyMaxChar() ) return;

	if(TRUE == m_bFirstTime)
	{
		m_strGetNum = "0";
		m_bFirstTime = FALSE;
	}

	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";

	m_strGetNum += "9";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnBack()
{
	int len = m_strGetNum.GetLength();
	if(len)
	{
		m_strGetNum.SetAt(len-1,' ');
		m_strGetNum.TrimRight();
		UpdateDisplay();
	}
}


void CNumberBoxDlg::OnBnClickedBtnClear()
{
	m_strGetNum = "0";	
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnCancle()
{
	CDialog::OnCancel();
}


void CNumberBoxDlg::OnBnClickedBtnOk()
{
	CDialog::OnOK();
}

void CNumberBoxDlg::OnBnClickedBtnNega()
{
	if( !m_strGetNum.GetLength() ) return;

	if(m_strGetNum.GetLength()==1 && m_strGetNum.Mid(0,1).Compare("0")==0)
	{
		m_strGetNum = "-";
	}
	else if(m_strGetNum.GetAt(0) == '-')
	{
		m_strGetNum.SetAt(0, ' ');
		m_strGetNum.TrimLeft();		
	}
	else
	{
		CString m_buf;
		m_buf="-";
		m_buf += m_strGetNum;
		m_strGetNum = m_buf;	
	}

	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnDot()
{
	if( !VerifyMaxChar() ) return;

	// 첫입력이면 "0." 형태로 추가
	if(!m_strGetNum.GetLength())
		m_strGetNum += "0.";
	else
		m_strGetNum += ".";
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnNe01()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 0.05;
	GetStoD(m_dpoint, FALSE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnPo01()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 0.05;
	GetStoD(m_dpoint, TRUE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnNe05()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 0.1;
	GetStoD(m_dpoint, FALSE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnPo05()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 0.1;
	GetStoD(m_dpoint, TRUE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnNe1()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 1;
	GetStoD(m_dpoint, FALSE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnPo1()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 1;
	GetStoD(m_dpoint, TRUE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnNe10()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 10;
	GetStoD(m_dpoint, FALSE);
	UpdateDisplay();
}


void CNumberBoxDlg::OnBnClickedBtnPo10()
{
	if(m_strGetNum.Compare("0")==0)
		m_strGetNum.Empty();
	else if(m_strGetNum.Compare("-0")==0)
		m_strGetNum = "-";
	m_dpoint = 10;
	GetStoD(m_dpoint, TRUE);
	UpdateDisplay();
}

void CNumberBoxDlg::OnGuiBtnSet()
{		  
	m_btnDot.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnDot.SetIcon(IDI_ICON_DOT);
	m_btnDot.SetAlign(CButtonST::ST_ALIGN_VERT);
		
	m_btnMinus.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnMinus.SetIcon(IDI_ICON_MINUS);
	m_btnMinus.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN0.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN0.SetIcon(IDI_ICON_N0);
	m_btnN0.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN1.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN1.SetIcon(IDI_ICON_N1);
	m_btnN1.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN2.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN2.SetIcon(IDI_ICON_N2);
	m_btnN2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN3.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN3.SetIcon(IDI_ICON_N3);
	m_btnN3.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN4.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN4.SetIcon(IDI_ICON_N4);
	m_btnN4.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN5.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN5.SetIcon(IDI_ICON_N5);
	m_btnN5.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN6.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN6.SetIcon(IDI_ICON_N6);
	m_btnN6.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN7.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN7.SetIcon(IDI_ICON_N7);
	m_btnN7.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN8.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN8.SetIcon(IDI_ICON_N8);
	m_btnN8.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnN9.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnN9.SetIcon(IDI_ICON_N9);
	m_btnN9.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBkSp.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnBkSp.SetIcon(IDI_ICON_BKSP);
	m_btnBkSp.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnClr.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnClr.SetIcon(IDI_ICON_CLR);
	m_btnClr.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnCancel.SetIcon(IDI_ICON_CANCEL);
	m_btnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnOk.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnOk.SetIcon(IDI_ICON_OK);
	m_btnOk.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_EditCurData.SetBackColor(COLOR_GREEN);
	m_EditCurData.SetTextSize(30);

	m_EditMaxData.SetBackColor(COLOR_DDARKYELLOW);
	m_EditMaxData.SetTextSize(30);

	m_EditMinData.SetBackColor(COLOR_DDARKYELLOW);
	m_EditMinData.SetTextSize(30);
}


