// SupervisorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SupervisorDlg.h"
#include "afxdialogex.h"


// CSupervisorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSupervisorDlg, CDialog)

CSupervisorDlg::CSupervisorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSupervisorDlg::IDD, pParent)
{

}

CSupervisorDlg::~CSupervisorDlg()
{
}

void CSupervisorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_REG_USER, m_BtnUserReg);
}


BEGIN_MESSAGE_MAP(CSupervisorDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_REG_USER, &CSupervisorDlg::OnBnClickedBtnRegUser)
END_MESSAGE_MAP()


// CSupervisorDlg 메시지 처리기입니다.


BOOL CSupervisorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSupervisorDlg::OnBnClickedBtnRegUser()
{
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[User Register Dialog Show]");

		g_LibCommCtrl.ShowRegisterUser();

		if (!(g_LibCommCtrl.m_strCreateID.IsEmpty()))
		{
			MakeLog("[ID = %s User Create ]", g_LibCommCtrl.m_strCreateID);
		}
	}
	
}


BOOL CSupervisorDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_ESCAPE ){
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CSupervisorDlg::OnBtnGUI()
{
	m_BtnUserReg.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnUserReg.SetIcon(IDI_BTN_REGISTER);
	m_BtnUserReg.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CSupervisorDlg::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}