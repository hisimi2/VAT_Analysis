// DlgAlarmZoom.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "DlgAlarmZoom.h"
#include "afxdialogex.h"


// CDlgAlarmZoom 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAlarmZoom, CDialog)

CDlgAlarmZoom::CDlgAlarmZoom(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ALARM_ZOOM, pParent)
{

}

CDlgAlarmZoom::~CDlgAlarmZoom()
{
}

void CDlgAlarmZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CDlgAlarmZoom, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAlarmZoom::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_USER_SEL_1, &CDlgAlarmZoom::OnBnClickedButtonUserSel1)
END_MESSAGE_MAP()


// CDlgAlarmZoom 메시지 처리기입니다.


void CDlgAlarmZoom::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}



BOOL CDlgAlarmZoom::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strAlid;
	strAlid.Format("%s", m_stDataListErrLog.szEQP_ALID);
	// Error Code
	SetDlgItemText(IDC_TEXTBOX_ERRCODE, strAlid);//m_tErrorMsg.szEQP_ALID);

	SetDlgItemText(IDC_TEXTBOX_SUBCODE, m_stDataListErrLog.szPart);

	SetDlgItemText(IDC_TEXTBOX_OCCURRED_TIME, m_stDataListErrLog.strOccurred_Time);


	SetDlgItemText(IDC_MYMSGEDIT_LANG_SETUP, m_stDataListErrLog.szMessage);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAlarmZoom::OnBnClickedButtonUserSel1()
{
	CDialog::OnOK();
}
