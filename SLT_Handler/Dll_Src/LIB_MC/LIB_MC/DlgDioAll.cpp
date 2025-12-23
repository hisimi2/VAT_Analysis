// DlgDioAll.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgDioAll.h"
#include "afxdialogex.h"
#include "DEF_LIB_GUI.h"
#include "MyList.h"
#include "DEF_DB_TBL.h"
#include "DIO_Cont_Imp.h"

// CDlgDioAll 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDioAll, CDialogEx)

CDlgDioAll::CDlgDioAll(int nDlgUsage, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDioAll::IDD, pParent)
{
	m_nDlgUsage       = nDlgUsage;
	m_nCur_PageNum_DO = 0;
	m_nCur_PageNum_DI = 0;
	int i=0;

	m_nDO_ButtonID[i] = IDC_BTN_OUT_0 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_0 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_1 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_1 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_2 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_2 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_3 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_3 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_4 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_4 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_5 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_5 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_6 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_6 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_7 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_7 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_8 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_8 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_9 ;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_9 ; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_10;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_10; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_11;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_11; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_12;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_12; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_13;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_13; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_14;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_14; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_15;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_15; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_16;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_16; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_17;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_17; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_18;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_18; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_19;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_19; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_20;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_20; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_21;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_21; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_22;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_22; i++;
	m_nDO_ButtonID[i] = IDC_BTN_OUT_23;  m_nDO_StaticID[i] = IDC_STATIC_DO_NAME_23; i++;

	i=0;
	m_nDI_ButtonID[i] = IDC_BTN_IN_0 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_0 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_1 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_1 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_2 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_2 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_3 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_3 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_4 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_4 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_5 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_5 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_6 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_6 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_7 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_7 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_8 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_8 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_9 ;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_9 ; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_10;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_10; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_11;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_11; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_12;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_12; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_13;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_13; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_14;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_14; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_15;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_15; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_16;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_16; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_17;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_17; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_18;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_18; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_19;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_19; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_20;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_20; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_21;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_21; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_22;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_22; i++;
	m_nDI_ButtonID[i] = IDC_BTN_IN_23;  m_nDI_StaticID[i] = IDC_STATIC_DI_NAME_23; i++;

}

CDlgDioAll::~CDlgDioAll()
{
}

void CDlgDioAll::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int i=0;
	for(i=0; i<MAX_DIO_ONE_PAGE; i++)
	{
		DDX_Control(pDX, m_nDO_ButtonID[i], m_btnDO[i]);
		DDX_Control(pDX, m_nDI_ButtonID[i], m_btnDI[i]);
		DDX_Control(pDX, m_nDO_StaticID[i], m_staticDO_Name[i]);
		DDX_Control(pDX, m_nDI_StaticID[i], m_staticDI_Name[i]);
	}
	DDX_Control(pDX, IDC_STATIC_DO_TITLE, m_staticDO_Title	);
	DDX_Control(pDX, IDC_STATIC_DI_TITLE, m_staticDI_Title	);
	DDX_Control(pDX, IDC_BTN_OUT_UP		, m_btnDO_UP		);
	DDX_Control(pDX, IDC_BTN_OUT_DOWN	, m_btnDO_DOWN		);
	DDX_Control(pDX, IDC_BTN_IN_UP		, m_btnDI_UP		);
	DDX_Control(pDX, IDC_BTN_IN_DOWN	, m_btnDI_DOWN		);
	DDX_Control(pDX, IDOK				, m_btnExit			);
}


BEGIN_MESSAGE_MAP(CDlgDioAll, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_OUT_UP, &CDlgDioAll::OnBnClickedBtnOutUp)
	ON_BN_CLICKED(IDC_BTN_OUT_DOWN, &CDlgDioAll::OnBnClickedBtnOutDown)
	ON_BN_CLICKED(IDC_BTN_IN_UP, &CDlgDioAll::OnBnClickedBtnInUp)
	ON_BN_CLICKED(IDC_BTN_IN_DOWN, &CDlgDioAll::OnBnClickedBtnInDown)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_OUT_0, IDC_BTN_OUT_23, OnBnClickedBtnOut)
	ON_BN_CLICKED(IDOK, &CDlgDioAll::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDioAll 메시지 처리기입니다.



BOOL CDlgDioAll::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitControl();
	Update_IO_Name(m_nCur_PageNum_DO, m_nCur_PageNum_DI);
	Update_IO_Status(m_nCur_PageNum_DO, m_nCur_PageNum_DI);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgDioAll::PreTranslateMessage(MSG* pMsg)
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

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgDioAll::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1);
}


void CDlgDioAll::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(nIDEvent);
			Update_IO_Status(m_nCur_PageNum_DO, m_nCur_PageNum_DI);
			SetTimer(nIDEvent, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgDioAll::InitControl()
{
	// Title
	m_staticDO_Title.m_fnSetFont(_T("Fixedsys"));
	m_staticDO_Title.SetTextSize(21);
	m_staticDO_Title.SetWeight(80);
	m_staticDO_Title.SetBackColor(COLOR_BBLACK);
	m_staticDO_Title.SetTextColor(COLOR_LLIGHTGREEN);	
	if( m_nDlgUsage == eDlgUsageDIO ){
		m_staticDO_Title.SetWindowText(_T("DIGITAL OUT"));
	}else{
		m_staticDO_Title.SetWindowText(_T("ANALOG OUT"));
	}

	m_staticDI_Title.m_fnSetFont(_T("Fixedsys"));
	m_staticDI_Title.SetTextSize(21);
	m_staticDI_Title.SetWeight(80);
	m_staticDI_Title.SetBackColor(COLOR_BBLACK);
	m_staticDI_Title.SetTextColor(COLOR_LLIGHTGREEN);
	if( m_nDlgUsage == eDlgUsageDIO ){
		m_staticDI_Title.SetWindowText(_T("DIGITAL IN"));
	}else{
		m_staticDI_Title.SetWindowText(_T("ANALOG IN"));
	}

	// Button IO Status
	int i=0;
	for(i=0; i<MAX_DIO_ONE_PAGE; i++)
	{
		m_btnDO[i].SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
		m_btnDO[i].SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
		m_btnDO[i].SetIcon(IDI_DO_OFF);
		m_btnDO[i].SetAlign(CButtonST::ST_ALIGN_HORIZ);

		m_btnDI[i].SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
		m_btnDI[i].SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
		m_btnDI[i].SetIcon(IDI_DI_OFF);
		m_btnDI[i].SetAlign(CButtonST::ST_ALIGN_HORIZ);
	}

	// Static IO Name 
	for(i=0; i<MAX_DIO_ONE_PAGE; i++)
	{
		m_staticDO_Name[i].m_fnSetFont(_T("Tahoma"));
		m_staticDO_Name[i].SetTextSize(15);
		m_staticDO_Name[i].SetWeight(80);
		m_staticDO_Name[i].SetBackColor(COLOR_WWHITE);
		m_staticDO_Name[i].SetTextColor(COLOR_BBLACK);

		m_staticDI_Name[i].m_fnSetFont(_T("Tahoma"));
		m_staticDI_Name[i].SetTextSize(15);
		m_staticDI_Name[i].SetWeight(80);
		m_staticDI_Name[i].SetBackColor(COLOR_WWHITE);
		m_staticDI_Name[i].SetTextColor(COLOR_BBLACK);
	}

	// Operation Button
	m_btnDO_UP.SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
	m_btnDO_UP.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
	//m_btnDO_UP.SetIcon(IDI_DO_OFF); // TODO : Icon 삽입
	m_btnDO_UP.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnDO_DOWN.SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
	m_btnDO_DOWN.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
	//m_btnDO_DOWN.SetIcon(IDI_DO_OFF); // TODO : Icon 삽입
	m_btnDO_DOWN.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnDI_UP.SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
	m_btnDI_UP.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
	//m_btnDO_UP.SetIcon(IDI_DO_OFF);// TODO : Icon 삽입
	m_btnDI_UP.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnDI_DOWN.SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
	m_btnDI_DOWN.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
	//m_btnDI_DOWN.SetIcon(IDI_DO_OFF);// TODO : Icon 삽입
	m_btnDI_DOWN.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnExit.SetShade(CShadeButtonST::SHS_METAL,8,28,5,COLOR_DDARKGRAY);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLUE);
	//m_btnExit.SetIcon(IDI_DO_OFF);// TODO : Icon 삽입
	m_btnExit.SetAlign(CButtonST::ST_ALIGN_HORIZ);
}

void CDlgDioAll::Update_IO_Name(int nDO_PageNum, int nDI_PageNum)
{
	if( nDO_PageNum >= 0 )
	{
		CMyList OutList;
		int nFrmIdx = 0;
		int nCount  = 0;
		nFrmIdx = nDO_PageNum*MAX_DIO_ONE_PAGE;
		nCount  = MAX_DIO_ONE_PAGE;
		if( m_nDlgUsage == eDlgUsageDIO ){
			g_pDbIf->SELECT_DIO_TBL(_tDIO_LS::eDO, nFrmIdx, nCount, OutList);
		}else{
			g_pDbIf->SELECT_AIO_TBL(_tAIO_LS::eAO, nFrmIdx, nCount, OutList);
		}

		for(int i=0; i<MAX_DIO_ONE_PAGE; i++){
			CString strName;
			if( i<OutList.m_fnGetCount() ){
				if( m_nDlgUsage == eDlgUsageDIO ){
					_tDIO_LS* pItem = (_tDIO_LS*)OutList.m_fnGetData(i);
					strName.Format(_T("%03d : %s - %s"), pItem->index, pItem->label, pItem->name);
					strName.TrimLeft();
					strName.TrimRight();
					if( pItem->contact == _tDIO_LS::eContact_B )
					{
						strName += _T(" (B TYPE)");
					}
				}else{
					_tAIO_LS* pItem = (_tAIO_LS*)OutList.m_fnGetData(i);
					strName.Format(_T("%03d : %s - %s"), pItem->nIdx_Logical, pItem->strLabel, pItem->strName);
				}
			}else{
				strName.Format("%03d : %s - %s", nFrmIdx+i, _T("-"), _T("-"));
			}
			SetDlgItemText(m_nDO_StaticID[i], strName);
		}
		m_nCur_PageNum_DO = nDO_PageNum;
	}

	if( nDI_PageNum >= 0 )
	{
		CMyList InList;
		int nFrmIdx = 0;
		int nCount  = 0;
		nFrmIdx = nDI_PageNum*MAX_DIO_ONE_PAGE;
		nCount  = MAX_DIO_ONE_PAGE;
		if( m_nDlgUsage == eDlgUsageDIO ){
			g_pDbIf->SELECT_DIO_TBL(_tDIO_LS::eDI, nFrmIdx, nCount, InList);
		}else{
			g_pDbIf->SELECT_AIO_TBL(_tAIO_LS::eAI, nFrmIdx, nCount, InList);
		}

		for(int i=0; i<MAX_DIO_ONE_PAGE; i++){
			CString strName;
			if( i<InList.m_fnGetCount() ){
				if( m_nDlgUsage == eDlgUsageDIO ){
					_tDIO_LS* pItem = (_tDIO_LS*)InList.m_fnGetData(i);
					strName.Format(_T("%03d : %s - %s"), pItem->index, pItem->label, pItem->name);
					strName.TrimLeft();
					strName.TrimRight();
					if( pItem->contact == _tDIO_LS::eContact_B ){
						strName += _T(" (B TYPE)");
					}
				}else{
					_tAIO_LS* pItem = (_tAIO_LS*)InList.m_fnGetData(i);
					strName.Format(_T("%03d : %s - %s"), pItem->nIdx_Logical, pItem->strLabel, pItem->strName);
				}
			}else{
				strName.Format(_T("%03d : %s - %s"), nFrmIdx+i, _T("-"), _T("-"));
			}
			SetDlgItemText(m_nDI_StaticID[i], strName);
		}
		m_nCur_PageNum_DI = nDI_PageNum;
	}
}

// nDO_PageNum : 0 base, nDI_PageNum : 0 base 
void CDlgDioAll::Update_IO_Status(int nDO_PageNum, int nDI_PageNum)
{
	// Output Status
	if( nDO_PageNum >= 0 )
	{
		int nFrmIdx = nDO_PageNum*MAX_DIO_ONE_PAGE;
		int test = (BOOL)g_pDIOContImp->_out[3]; 
		for(int i=0; i<MAX_DIO_ONE_PAGE; i++)
		{
			int test = (BOOL)g_pDIOContImp->_out[i];
			int nIdx = nFrmIdx+i;
			if( m_nDlgUsage == eDlgUsageDIO ){
				BOOL bStatus = FALSE;
				if( nIdx < g_pDIOContImp->GetDOPointCount() ){
					bStatus = (BOOL)g_pDIOContImp->GET_DO(nIdx);
				}
				m_btnDO[i].SetIcon(bStatus?IDI_DO_ON:IDI_DO_OFF);
			}else{
				double dStatus = 0.0;
				if( nIdx < g_pAioContImp->GetAoCount() ){
					dStatus = (double)g_pAioContImp->out(nIdx);
				}
				m_btnDO[i].SetIcon(IDI_DO_ON);
				CString strValue = _T("");
				strValue.Format("%.3f", dStatus);
				m_btnDO[i].SetWindowText(strValue);
			}
		}
	}

	// Input Status
	if( nDI_PageNum >= 0 )
	{
		int nFrmIdx = nDI_PageNum*MAX_DIO_ONE_PAGE;
		for(int i=0; i<MAX_DIO_ONE_PAGE; i++)
		{
			int nIdx = nFrmIdx+i;
			if( m_nDlgUsage == eDlgUsageDIO ){
				BOOL bStatus = FALSE;
				if( nIdx < g_pDIOContImp->GetDIPointCount() ){
					bStatus = (BOOL)g_pDIOContImp->GET_DI(nIdx);
				}
				m_btnDI[i].SetIcon(bStatus?IDI_DI_ON:IDI_DI_OFF);
			}else{
				double dStatus = 0.0;
				if( nIdx < g_pAioContImp->GetAiCount() ){
					dStatus = (double)g_pAioContImp->in(nIdx);
				}
				m_btnDI[i].SetIcon(IDI_DI_ON);
				CString strValue = _T("");
				strValue.Format("%.3f", dStatus);
				m_btnDI[i].SetWindowText(strValue);
			}
		}
	}
}


void CDlgDioAll::OnBnClickedBtnOutUp()
{
	int nDo_PageNum = m_nCur_PageNum_DO + 1;
	int nMaxPageCount = 0;
	if( m_nDlgUsage == eDlgUsageDIO ){
		nMaxPageCount = ((g_pDIOContImp->GetDOPointCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}else{
		nMaxPageCount = ((g_pAioContImp->GetAoCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}
	if( nDo_PageNum >= nMaxPageCount ) nDo_PageNum = 0;
	Update_IO_Name(nDo_PageNum);	
}


void CDlgDioAll::OnBnClickedBtnOutDown()
{
	int nDo_PageNum = m_nCur_PageNum_DO - 1;
	int nMaxPageCount = 0;
	if( m_nDlgUsage == eDlgUsageDIO ){
		nMaxPageCount = ((g_pDIOContImp->GetDOPointCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}else{
		nMaxPageCount = ((g_pAioContImp->GetAoCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}
	if( nDo_PageNum < 0 ) nDo_PageNum = nMaxPageCount-1;
	Update_IO_Name(nDo_PageNum);	
}


void CDlgDioAll::OnBnClickedBtnInUp()
{
	int nDi_PageNum = m_nCur_PageNum_DI + 1;
	int nMaxPageCount = 0;
	if( m_nDlgUsage == eDlgUsageDIO ){
		nMaxPageCount = ((g_pDIOContImp->GetDIPointCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}else{
		nMaxPageCount = ((g_pAioContImp->GetAiCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}
	if( nDi_PageNum >= nMaxPageCount ) nDi_PageNum = 0;
	Update_IO_Name(-1, nDi_PageNum);	
}


void CDlgDioAll::OnBnClickedBtnInDown()
{
	int nDi_PageNum = m_nCur_PageNum_DI - 1;
	int nMaxPageCount = 0;
	if( m_nDlgUsage == eDlgUsageDIO ){
		nMaxPageCount = ((g_pDIOContImp->GetDIPointCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}else{
		nMaxPageCount = ((g_pAioContImp->GetAiCount()-1) / MAX_DIO_ONE_PAGE)+1;
	}
	if( nDi_PageNum < 0 ) nDi_PageNum = nMaxPageCount-1;
	Update_IO_Name(-1, nDi_PageNum);	
}


void CDlgDioAll::OnBnClickedBtnOut(UINT nCtrlID)
{
	ASSERT( nCtrlID >= IDC_BTN_OUT_0 && nCtrlID <= IDC_BTN_OUT_23 );
	int nOffset = nCtrlID - IDC_BTN_OUT_0;

	if( m_nDlgUsage == eDlgUsageDIO )
	{
		int nFrmIdx = m_nCur_PageNum_DO*MAX_DIO_ONE_PAGE;
		int nIdx = nFrmIdx + nOffset;
		char cValue = (g_pDIOContImp->GET_DO(nIdx) != 0 ) ? 0 : 1;
		g_pDIOContImp->SET_DO(nIdx, cValue);
	}

	// Analog 일 때는 출력 강제로 내보내지 않음.
}


void CDlgDioAll::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
