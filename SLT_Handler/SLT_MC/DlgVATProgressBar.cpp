// DlgVATProgressBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgVATProgressBar.h"
#include "afxdialogex.h"


static int m_nPosicionControl[eVATPos_MaxPosition - 1] = {IDC_EDIT1,IDC_EDIT2,IDC_EDIT3,IDC_EDIT4,IDC_EDIT5,IDC_EDIT6,IDC_EDIT7,IDC_EDIT8,IDC_EDIT9,IDC_EDIT10
, IDC_EDIT11, IDC_EDIT12, IDC_EDIT13,IDC_EDIT14, IDC_EDIT15, IDC_EDIT16, IDC_EDIT17, IDC_EDIT18, IDC_EDIT19, IDC_EDIT20
, IDC_EDIT21, IDC_EDIT22, IDC_EDIT23, IDC_EDIT24, IDC_EDIT25, IDC_EDIT26, IDC_EDIT27, IDC_EDIT28, IDC_EDIT29, IDC_EDIT30
, IDC_EDIT31 ,IDC_EDIT32,IDC_EDIT33,IDC_EDIT34};
// CDlgVATProgressBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgVATProgressBar, CDialog)

CDlgVATProgressBar::CDlgVATProgressBar(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_VAT_PROGRESS_BAR, pParent)
{
	for (int i = 0; i < eVATPos_MaxPosition; i++)
		bIsRed[i] = FALSE;

}

CDlgVATProgressBar::~CDlgVATProgressBar()
{
}

void CDlgVATProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_UNIT_PICTURE, m_bitPic1);

	for (int i = 0; i < eVATPos_MaxPosition-1; i++)
		DDX_Control(pDX, m_nPosicionControl[i], m_editPosition[i]);
}


BEGIN_MESSAGE_MAP(CDlgVATProgressBar, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgVATProgressBar 메시지 처리기입니다.
BOOL CDlgVATProgressBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//InitReSize();
	InitPosition();
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.


}

void CDlgVATProgressBar::InitReSize()
{
	CRect r; this->GetWindowRect(&r);
	CRect rect;	m_bitPic1.GetWindowRect(&rect);

	int nDesktopWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nDesktopHeight = GetSystemMetrics(SM_CYFULLSCREEN);


	int nX = nDesktopWidth / 2 - r.Width() / 2;
	int nY = nDesktopHeight / 2 - r.Height() / 2;

	int nHeight = r.Height();
// 	if (m_nPosition <= 0)
// 	{
// 		nHeight = r.Height() - rect.Height() - 3;
// 		//this->SetWindowPos(&wndTopMost, 150, 250, r.Width(), rect.top -185, SWP_NOMOVE);
// 	}

	this->SetWindowPos(&wndTopMost, nX, nY,/*150, 250,*/ r.Width(), nHeight, 0);
}

void CDlgVATProgressBar::InitPosition()
{
// 	for (int i = 0; i < ePos_MaxPosition - 1; i++) {
// 			
// 			GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_HIDE);
// 	}
}

void CDlgVATProgressBar::StatusChange(_eVATProgressStatus __eVATProgressStatus)
{
	for (int i = 0; i < eVATPos_MaxPosition - 1; i++)
	{
		g_DMCont.m_dmHandlerOpt.SN(NDM11_VAT_POS_Stacker_1 + i, __eVATProgressStatus);
	}
}

void CDlgVATProgressBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{		
		KillTimer(1);
		for (int i = 0; i < eVATPos_MaxPosition - 1; i++) 
		{
			int nStatus = g_DMCont.m_dmHandlerOpt.GN(NDM11_VAT_POS_Stacker_1 + i);
			switch (nStatus)
			{
				case eVATProgressStatus_IDLE:
				{
					GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_HIDE);
					m_editPosition[i].ShowWindow(SW_HIDE);
				}break;
				case eVATProgressStatus_Reserved:
				{
					m_editPosition[i].SetBackColor(COLOR_YELLOW);
					GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_SHOW);
					m_editPosition[i].ShowWindow(SW_SHOW);
				}break;
				
				case eVATProgressStatus_Running:
				{
					bIsRed[i] = !bIsRed[i];
					m_editPosition[i].SetBackColor(COLOR_RED);
					GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_SHOW);
					m_editPosition[i].ShowWindow((bIsRed[i] == TRUE ? SW_SHOW : SW_HIDE));
				}break;
				case eVATProgressStatus_Complete:
				{
					m_editPosition[i].SetBackColor(COLOR_GREEN);
					GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_SHOW);
					m_editPosition[i].ShowWindow(SW_SHOW);
				}break;
				default:
					break;
			}	
		}
		SetTimer(1,1000,NULL);
	}
	CDialog::OnTimer(nIDEvent);
}



