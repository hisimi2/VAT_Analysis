// UserConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UserConfigDlg.h"
#include "afxdialogex.h"
#include "../Include/OcxFiles/SpreadSheet/SSOCX.H"
// CUserConfigDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserConfigDlg, CDialog)

CUserConfigDlg::CUserConfigDlg(ST_USER_CONFIG_ARRAY stUserConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CUserConfigDlg::IDD, pParent)
{
	m_nSpreadHeader = -1000;

	m_StUserConfigSet = stUserConfig;
}

CUserConfigDlg::~CUserConfigDlg()
{
}

void CUserConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_WAIT, m_FlexGrid);
}


BEGIN_MESSAGE_MAP(CUserConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUserConfigDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM, &CUserConfigDlg::OnBnClickedButtonSystem)
END_MESSAGE_MAP()


// CUserConfigDlg 메시지 처리기입니다.


BOOL CUserConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitSpread();
	SetItemList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUserConfigDlg::InitSpread()
{
	m_FlexGrid.SetRowHeaderCols(2);
	m_FlexGrid.SetColHeaderRows(1);
	m_FlexGrid.SetMaxCols(1);
	m_FlexGrid.SetMaxRows(MAX_SPREAD_USER_CONFIG);
	m_FlexGrid.SetTypeHAlign(SS_CELL_H_ALIGN_LEFT);

	m_FlexGrid.SetColWidth(m_nSpreadHeader,10);
	m_FlexGrid.SetColWidth(m_nSpreadHeader+1,77);

	m_FlexGrid.SetTextMatrix(0,m_nSpreadHeader, _T("Axis"));
	m_FlexGrid.SetTextMatrix(0,m_nSpreadHeader+1, _T("Item"));
	m_FlexGrid.SetTextMatrix(0, 1, "Select Mode");

	for(int i=1; i<MAX_SPREAD_USER_CONFIG; i++)
	{
		m_FlexGrid.SetTextMatrix(i,m_nSpreadHeader, _T(" "));
		m_FlexGrid.SetTextMatrix(i,m_nSpreadHeader+1, _T(" "));
	}
}

void CUserConfigDlg::SetItemList()
{
	CString strData = _T("");
	for(int i=1; i<MAX_SPREAD_USER_CONFIG+1;i++)
	{
		if(m_StUserConfigSet.StContent[i-1].szAxis != NULL)
		{
			m_FlexGrid.SetTextMatrix(i,m_nSpreadHeader, m_StUserConfigSet.StContent[i-1].szAxis);
			m_FlexGrid.SetTextMatrix(i,m_nSpreadHeader+1, m_StUserConfigSet.StContent[i-1].szItem);
			strData.Format("%d",m_StUserConfigSet.StContent[i-1].nUserConfigSet);
			m_FlexGrid.SetTextMatrix(i,1,strData);
		}
	}
}

void CUserConfigDlg::GetItemList()
{
	double dSet = 0.0;
	for(int i=1; i<MAX_SPREAD_USER_CONFIG+1;i++)
	{
		if(m_StUserConfigSet.StContent[i-1].szAxis != NULL)
		{
			strcpy_s(m_StUserConfigSet.StContent[i-1].szAxis, sizeof(m_StUserConfigSet.StContent[i-1].szAxis), m_FlexGrid.GetTextMatrix(i,m_nSpreadHeader));
			strcpy_s(m_StUserConfigSet.StContent[i-1].szItem, sizeof(m_StUserConfigSet.StContent[i-1].szItem),  m_FlexGrid.GetTextMatrix(i,m_nSpreadHeader+1));
			dSet = atof( m_FlexGrid.GetTextMatrix(i,1));
			m_StUserConfigSet.StContent[i-1].nUserConfigSet = (int)dSet;
		}
	}
}

void CUserConfigDlg::OnBnClickedButtonSave()
{
	GetItemList();
}


void CUserConfigDlg::OnBnClickedButtonSystem()
{
	for(int i=1; i<MAX_SPREAD_USER_CONFIG+1;i++)
	{
		m_StUserConfigSet.StContent[i-1].nUserConfigSet = 0;
	}

	SetItemList();
}
