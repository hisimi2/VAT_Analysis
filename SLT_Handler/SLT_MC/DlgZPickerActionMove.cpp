// DlgZPickerActionMove.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgZPickerActionMove.h"
#include "afxdialogex.h"


// CDlgZPickerActionMove 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgZPickerActionMove, CDialog)

CDlgZPickerActionMove::CDlgZPickerActionMove(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MANU_Z_PICKER_ACTION, pParent)
{
	m_nAxisZNo = eAXIS_TRAY_PP_1_Z;
}

CDlgZPickerActionMove::~CDlgZPickerActionMove()
{
}

void CDlgZPickerActionMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_HAND_POS1, m_ChkHand1_1);
	DDX_Control(pDX, IDC_CHECK_HAND_POS2, m_ChkHand1_2);
	DDX_Control(pDX, IDC_CHECK_HAND_POS3, m_ChkHand1_3);
	DDX_Control(pDX, IDC_CHECK_HAND_POS4, m_ChkHand1_4);
	DDX_Control(pDX, IDC_CHECK_HAND_POS9, m_ChkHand2_1);
	DDX_Control(pDX, IDC_CHECK_HAND_POS10, m_ChkHand2_2);
	DDX_Control(pDX, IDC_CHECK_HAND_POS11, m_ChkHand2_3);
	DDX_Control(pDX, IDC_CHECK_HAND_POS12, m_ChkHand2_4);
}


BEGIN_MESSAGE_MAP(CDlgZPickerActionMove, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgZPickerActionMove::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_MANU_MOVE, &CDlgZPickerActionMove::OnBnClickedButtonManuMove)
	ON_BN_CLICKED(IDC_BUTTON_SET_Y1, &CDlgZPickerActionMove::OnBnClickedButtonSetY1)
	ON_BN_CLICKED(IDC_BUTTON_SET_Y2, &CDlgZPickerActionMove::OnBnClickedButtonSetY2)
	ON_BN_CLICKED(IDC_BUTTON_SET_ALL, &CDlgZPickerActionMove::OnBnClickedButtonSetAll)
	ON_BN_CLICKED(IDC_BUTTON_SET_CLEAR, &CDlgZPickerActionMove::OnBnClickedButtonSetClear)
END_MESSAGE_MAP()


// CDlgZPickerActionMove 메시지 처리기입니다.


void CDlgZPickerActionMove::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}


void CDlgZPickerActionMove::OnBnClickedButtonManuMove()
{
	int nUseHand[2][4] = { { 0 }, };

	nUseHand[0][0] = m_ChkHand1_1.GetCheck();	nUseHand[0][1] = m_ChkHand1_2.GetCheck();
	nUseHand[0][2] = m_ChkHand1_3.GetCheck();	nUseHand[0][3] = m_ChkHand1_4.GetCheck();
	nUseHand[1][0] = m_ChkHand2_1.GetCheck();	nUseHand[1][1] = m_ChkHand2_2.GetCheck();
	nUseHand[1][2] = m_ChkHand2_3.GetCheck();	nUseHand[1][3] = m_ChkHand2_4.GetCheck();

	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++) {
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++) {
			if (nUseHand[nY][nX] == TRUE) { m_vPicker.push_back(CPoint(nX, nY)); }
		}
	}

	CDialog::OnOK();
}


void CDlgZPickerActionMove::OnBnClickedButtonSetY1()
{
	m_ChkHand1_1.SetCheck(TRUE);
	m_ChkHand1_2.SetCheck(TRUE);
	m_ChkHand1_3.SetCheck(TRUE);
	m_ChkHand1_4.SetCheck(TRUE);
}


void CDlgZPickerActionMove::OnBnClickedButtonSetY2()
{
	m_ChkHand2_1.SetCheck(TRUE);
	m_ChkHand2_2.SetCheck(TRUE);
	m_ChkHand2_3.SetCheck(TRUE);
	m_ChkHand2_4.SetCheck(TRUE);
}


void CDlgZPickerActionMove::OnBnClickedButtonSetAll()
{
	m_ChkHand1_1.SetCheck(TRUE);
	m_ChkHand1_2.SetCheck(TRUE);
	m_ChkHand1_3.SetCheck(TRUE);
	m_ChkHand1_4.SetCheck(TRUE);
	m_ChkHand2_1.SetCheck(TRUE);
	m_ChkHand2_2.SetCheck(TRUE);
	m_ChkHand2_3.SetCheck(TRUE);
	m_ChkHand2_4.SetCheck(TRUE);

	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	if (nSiteDivY != 2 && IsAxisZTestPp() == TRUE) {
		m_ChkHand2_1.SetCheck(FALSE);
		m_ChkHand2_2.SetCheck(FALSE);
		m_ChkHand2_3.SetCheck(FALSE);
		m_ChkHand2_4.SetCheck(FALSE);
	}
}


void CDlgZPickerActionMove::OnBnClickedButtonSetClear()
{
	m_ChkHand1_1.SetCheck(FALSE);
	m_ChkHand1_2.SetCheck(FALSE);
	m_ChkHand1_3.SetCheck(FALSE);
	m_ChkHand1_4.SetCheck(FALSE);
	m_ChkHand2_1.SetCheck(FALSE);
	m_ChkHand2_2.SetCheck(FALSE);
	m_ChkHand2_3.SetCheck(FALSE);
	m_ChkHand2_4.SetCheck(FALSE);
}


BOOL CDlgZPickerActionMove::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	if (nSiteDivY != 2 && IsAxisZTestPp() == TRUE)
	{
		GetDlgItem(IDC_CHECK_HAND_POS1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS4)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_CHECK_HAND_POS9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HAND_POS10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HAND_POS11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HAND_POS12)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BUTTON_SET_Y1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET_Y2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_HAND_POS1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS4)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_CHECK_HAND_POS9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_HAND_POS12)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_BUTTON_SET_Y1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET_Y2)->EnableWindow(TRUE);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgZPickerActionMove::IsAxisZTestPp()
{
	if (m_nAxisZNo == eAXIS_TEST_PP_1_Z || m_nAxisZNo == eAXIS_TEST_PP_1_Z2 || m_nAxisZNo == eAXIS_TEST_PP_2_Z || m_nAxisZNo == eAXIS_TEST_PP_2_Z2) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}