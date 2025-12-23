// DlgContactAndSafety.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgContactAndSafety.h"
#include "afxdialogex.h"


// CDlgContactAndSafety 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgContactAndSafety, CDialog)

CDlgContactAndSafety::CDlgContactAndSafety(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONTACT_AND_SAFETY_MOVING, pParent)
{

}

CDlgContactAndSafety::~CDlgContactAndSafety()
{
}

void CDlgContactAndSafety::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_CONTACTOR_FORCE,m_SSPANELTitle);
	DDX_Control(pDX, IDC_LABEL_TIME_DISPLAY, m_SSPANELTime);
	
}


BEGIN_MESSAGE_MAP(CDlgContactAndSafety, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgContactAndSafety 메시지 처리기입니다.


BOOL CDlgContactAndSafety::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strSubTitle;
	strSubTitle = _T("Contact And Safety Moving...\n");
	m_SSPANELTitle.SetWindowText(strSubTitle);

	SetTimer(0, 500, NULL);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgContactAndSafety::OnDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.	
	CDialogEx::OnDestroy();

	KillTimer(0);
}


void CDlgContactAndSafety::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0)
	{
		KillTimer(0);

		CString strSubTitle,strTime;
		
		double dMaxTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_TESTPP_DELAY_TIME);
		double dCurTime = g_DMCont.m_dmEQP.GD(DDM0_TmContactCurTime);

		strSubTitle = _T("Contact And Safety Moving...\n");
		m_SSPANELTitle.SetWindowText(strSubTitle);
		
		strTime.Format("%.1f / %.1f sec", dCurTime/1000, dMaxTime);
		m_SSPANELTime.SetWindowText(strTime);

		SetTimer(0, 500, NULL);
	}
	CDialogEx::OnTimer(nIDEvent);
}
