// UserOffSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "UserOffSetTestPp_MP.h"
#include "afxdialogex.h"
#include "SLTView.h"
#include "MainFrm.h"
#include <algorithm>

// CUserOffSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserOffSetTestPp_MP, CDialog)

CUserOffSetTestPp_MP::CUserOffSetTestPp_MP(CWnd* pParent /*=NULL*/)
	: CDialog(CUserOffSetTestPp_MP::IDD, pParent)
{
	m_nRadioSel= 0;
	m_dX = 0;
	m_dY = 0;
	m_dZ_Pick = 0;
	m_dZ_Place = 0;
	m_nRadioSel_TestPp = 0;

	m_dRearSite_FrontPicker_X = 0.0;
	m_dRearSite_FrontPicker_Y = 0.0;
	m_dRearSite_FrontPicker_Z_Pick = 0.0;
	m_dRearSite_FrontPicker_Z_Place = 0.0;

	m_dFrontSite_FrontPicker_X = 0.0;
	m_dFrontSite_FrontPicker_Y = 0.0;
	m_dFrontSite_FrontPicker_Z_Pick = 0.0;
	m_dFrontSite_FrontPicker_Z_Place = 0.0;

	m_dLoadTable_2YRow_X = 0.0;
	m_dLoadTable_2YRow_Y = 0.0;
	m_dLoadTable_2YRow_Z_Pick = 0.0;
	m_dLoadTable_2YRow_Z_Place = 0.0;

	memset(m_szCokName, 0x00, sizeof(m_szCokName));

}

CUserOffSetTestPp_MP::~CUserOffSetTestPp_MP()
{
}

void CUserOffSetTestPp_MP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_BtnDefault);
	
	DDX_Control(pDX, IDC_EDIT_X, m_edit_X);
	DDX_Control(pDX, IDC_EDIT_Y, m_edit_Y);
	DDX_Control(pDX, IDC_EDIT_Z_PICK, m_edit_Z_Pick);
	DDX_Control(pDX, IDC_EDIT_Z_PLACE, m_edit_Z_Place);

	DDX_Radio(pDX, IDC_RD_SITE1, m_nRadioSel);
	DDX_Text(pDX, IDC_EDIT_X, m_dX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dY);
	DDX_Text(pDX, IDC_EDIT_Z_PICK, m_dZ_Pick);
	DDX_Text(pDX, IDC_EDIT_Z_PLACE, m_dZ_Place);

	DDX_Radio(pDX, IDC_RD_TEST_PP_1, m_nRadioSel_TestPp);

	DDX_Control(pDX, IDC_EDIT_REARSITE_FRONTPICKER_X, m_edit_RearSite_FrontPicker_X);
	DDX_Control(pDX, IDC_EDIT_REARSITE_FRONTPICKER_Y, m_edit_RearSite_FrontPicker_Y);
	DDX_Control(pDX, IDC_EDIT_REARSITE_FRONTPICKER_Z_PICK, m_edit_RearSite_FrontPicker_Z_Pick);
	DDX_Control(pDX, IDC_EDIT_REARSITE_FRONTPICKER_Z_PLACE, m_edit_RearSite_FrontPicker_Z_Place);

	DDX_Control(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_X, m_edit_FrontSite_FrontPicker_X);
	DDX_Control(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_Y, m_edit_FrontSite_FrontPicker_Y);
	DDX_Control(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PICK, m_edit_FrontSite_FrontPicker_Z_Pick);
	DDX_Control(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PLACE, m_edit_FrontSite_FrontPicker_Z_Place);

	DDX_Text(pDX, IDC_EDIT_REARSITE_FRONTPICKER_X, m_dRearSite_FrontPicker_X);
	DDX_Text(pDX, IDC_EDIT_REARSITE_FRONTPICKER_Y, m_dRearSite_FrontPicker_Y);
	DDX_Text(pDX, IDC_EDIT_REARSITE_FRONTPICKER_Z_PICK, m_dRearSite_FrontPicker_Z_Pick);
	DDX_Text(pDX, IDC_EDIT_REARSITE_FRONTPICKER_Z_PLACE, m_dRearSite_FrontPicker_Z_Place);

	DDX_Text(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_X, m_dFrontSite_FrontPicker_X);
	DDX_Text(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_Y, m_dFrontSite_FrontPicker_Y);
	DDX_Text(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PICK, m_dFrontSite_FrontPicker_Z_Pick);
	DDX_Text(pDX, IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PLACE, m_dFrontSite_FrontPicker_Z_Place);


	DDX_Control(pDX, IDC_EDIT_LOADTABLE_X, m_edit_LoadTable_2YRow_X);
	DDX_Control(pDX, IDC_EDIT_LOADTABLE_Y, m_edit_LoadTable_2YRow_Y);
	DDX_Control(pDX, IDC_EDIT_LOADTABLE_Z_PICK, m_edit_LoadTable_2YRow_Z_Pick);
	DDX_Control(pDX, IDC_EDIT_LOADTABLE_Z_PLACE, m_edit_LoadTable_2YRow_Z_Place);

	DDX_Text(pDX, IDC_EDIT_LOADTABLE_X, m_dLoadTable_2YRow_X);
	DDX_Text(pDX, IDC_EDIT_LOADTABLE_Y, m_dLoadTable_2YRow_Y);
	DDX_Text(pDX, IDC_EDIT_LOADTABLE_Z_PICK, m_dLoadTable_2YRow_Z_Pick);
	DDX_Text(pDX, IDC_EDIT_LOADTABLE_Z_PLACE, m_dLoadTable_2YRow_Z_Place);

	
}


BEGIN_MESSAGE_MAP(CUserOffSetTestPp_MP, CDialog)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_X, &CUserOffSetTestPp_MP::OnEnSetfocusEditX)
	ON_EN_SETFOCUS(IDC_EDIT_Y, &CUserOffSetTestPp_MP::OnEnSetfocusEditY)
	ON_EN_SETFOCUS(IDC_EDIT_Z_PICK, &CUserOffSetTestPp_MP::OnEnSetfocusEditZPick)
	ON_EN_SETFOCUS(IDC_EDIT_Z_PLACE, &CUserOffSetTestPp_MP::OnEnSetfocusEditZPlace)

	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUserOffSetTestPp_MP::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RD_SITE1, &CUserOffSetTestPp_MP::OnBnClickedRdSite1)
	ON_BN_CLICKED(IDC_RD_SITE2, &CUserOffSetTestPp_MP::OnBnClickedRdSite2)
	ON_BN_CLICKED(IDC_RD_SITE3, &CUserOffSetTestPp_MP::OnBnClickedRdSite3)
	ON_BN_CLICKED(IDC_RD_SITE4, &CUserOffSetTestPp_MP::OnBnClickedRdSite4)
	ON_BN_CLICKED(IDC_RD_SITE5, &CUserOffSetTestPp_MP::OnBnClickedRdSite5)
	ON_BN_CLICKED(IDC_RD_SITE6, &CUserOffSetTestPp_MP::OnBnClickedRdSite6)
	ON_BN_CLICKED(IDC_RD_SITE7, &CUserOffSetTestPp_MP::OnBnClickedRdSite7)
	ON_BN_CLICKED(IDC_RD_SITE8, &CUserOffSetTestPp_MP::OnBnClickedRdSite8)
	ON_BN_CLICKED(IDC_RD_SITE9, &CUserOffSetTestPp_MP::OnBnClickedRdSite9)
	ON_BN_CLICKED(IDC_RD_SITE10, &CUserOffSetTestPp_MP::OnBnClickedRdSite10)
	ON_BN_CLICKED(IDC_RD_SITE11, &CUserOffSetTestPp_MP::OnBnClickedRdSite11)
	ON_BN_CLICKED(IDC_RD_SITE12, &CUserOffSetTestPp_MP::OnBnClickedRdSite12)
	ON_BN_CLICKED(IDC_RD_SITE13, &CUserOffSetTestPp_MP::OnBnClickedRdSite13)
	ON_BN_CLICKED(IDC_RD_SITE14, &CUserOffSetTestPp_MP::OnBnClickedRdSite14)
	ON_BN_CLICKED(IDC_RD_SITE15, &CUserOffSetTestPp_MP::OnBnClickedRdSite15)
	ON_BN_CLICKED(IDC_RD_SITE16, &CUserOffSetTestPp_MP::OnBnClickedRdSite16)
	ON_EN_SETFOCUS(IDC_EDIT_REARSITE_FRONTPICKER_X, &CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerX)
	ON_EN_SETFOCUS(IDC_EDIT_REARSITE_FRONTPICKER_Y, &CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerY)
	ON_EN_SETFOCUS(IDC_EDIT_REARSITE_FRONTPICKER_Z_PICK, &CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerZPick)
	ON_EN_SETFOCUS(IDC_EDIT_REARSITE_FRONTPICKER_Z_PLACE, &CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerZPlace)
	ON_EN_SETFOCUS(IDC_EDIT_FRONTSITE_FRONTPICKER_X, &CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerX)
	ON_EN_SETFOCUS(IDC_EDIT_FRONTSITE_FRONTPICKER_Y, &CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerY)
	ON_EN_SETFOCUS(IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PICK, &CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerZPick)
	ON_EN_SETFOCUS(IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PLACE, &CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerZPlace)
	ON_EN_SETFOCUS(IDC_EDIT_LOADTABLE_X, &CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableX)
	ON_EN_SETFOCUS(IDC_EDIT_LOADTABLE_Y, &CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableY)
	ON_EN_SETFOCUS(IDC_EDIT_LOADTABLE_Z_PICK, &CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableZPick)
	ON_EN_SETFOCUS(IDC_EDIT_LOADTABLE_Z_PLACE, &CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableZPlace)
	ON_BN_CLICKED(IDC_RD_TEST_PP_1, &CUserOffSetTestPp_MP::OnBnClickedRdLdtbl1)
	ON_BN_CLICKED(IDC_RD_TEST_PP_2, &CUserOffSetTestPp_MP::OnBnClickedRdLdtbl2)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CUserOffSetTestPp_MP::OnBnClickedButtonDefault)
END_MESSAGE_MAP()


// CUserOffSet 메시지 처리기입니다.


BOOL CUserOffSetTestPp_MP::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBntGUI();
	OnEditBox();
	m_color.CreateSolidBrush(RGB(50,125,0));

	ZeroMemory(m_szCokName, sizeof(m_szCokName));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, m_szCokName, sizeof(m_szCokName));
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, 0, m_TeachTestPp);

	TestPpRadioButtonShow(eTestPP_1);
	OnUpdateEditOffSet(eATS_DN_STATION_L1);
	OnUpdateEditOffSet_LoadTable(eTestPP_1);

	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CUserOffSetTestPp_MP::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CUserOffSetTestPp_MP::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::DestroyWindow();
}

void CUserOffSetTestPp_MP::OnBntGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDefault.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnDefault.SetAlign(CButtonST::ST_ALIGN_VERT);
	
}

HBRUSH CUserOffSetTestPp_MP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE || pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE2 || pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE3){
		pDC->SetBkMode( TRANSPARENT  );
		return (HBRUSH) m_color;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

CString CUserOffSetTestPp_MP::getStrIndex(int nIndex)
{
	CString strIndex;

	switch (nIndex)
	{
	case eTestPpOffSet_X:		strIndex = _T("OffSet_X");			break;
	case eTestPpOffSet_Y :		strIndex = _T("OffSet_Y");			break;
	}
	return strIndex;
}


void CUserOffSetTestPp_MP::OnEditBox()
{
	m_edit_X.SetBackColor(COLOR_WWHITE);
	m_edit_X.SetTextSize(30);

	m_edit_Y.SetBackColor(COLOR_WWHITE);
	m_edit_Y.SetTextSize(30);

	m_edit_Z_Pick.SetBackColor(COLOR_WWHITE);
	m_edit_Z_Pick.SetTextSize(30);

	m_edit_Z_Place.SetBackColor(COLOR_WWHITE);
	m_edit_Z_Place.SetTextSize(30);

	m_edit_RearSite_FrontPicker_X.SetBackColor(COLOR_WWHITE);
	m_edit_RearSite_FrontPicker_X.SetTextSize(30);

	m_edit_RearSite_FrontPicker_Y.SetBackColor(COLOR_WWHITE);
	m_edit_RearSite_FrontPicker_Y.SetTextSize(30);

	m_edit_RearSite_FrontPicker_Z_Pick.SetBackColor(COLOR_WWHITE);
	m_edit_RearSite_FrontPicker_Z_Pick.SetTextSize(30);

	m_edit_RearSite_FrontPicker_Z_Place.SetBackColor(COLOR_WWHITE);
	m_edit_RearSite_FrontPicker_Z_Place.SetTextSize(30);

	m_edit_FrontSite_FrontPicker_X.SetBackColor(COLOR_WWHITE);
	m_edit_FrontSite_FrontPicker_X.SetTextSize(30);

	m_edit_FrontSite_FrontPicker_Y.SetBackColor(COLOR_WWHITE);
	m_edit_FrontSite_FrontPicker_Y.SetTextSize(30);

	m_edit_FrontSite_FrontPicker_Z_Pick.SetBackColor(COLOR_WWHITE);
	m_edit_FrontSite_FrontPicker_Z_Pick.SetTextSize(30);

	m_edit_FrontSite_FrontPicker_Z_Place.SetBackColor(COLOR_WWHITE);
	m_edit_FrontSite_FrontPicker_Z_Place.SetTextSize(30);

	m_edit_LoadTable_2YRow_X.SetBackColor(COLOR_WWHITE);
	m_edit_LoadTable_2YRow_X.SetTextSize(30);

	m_edit_LoadTable_2YRow_Y.SetBackColor(COLOR_WWHITE);
	m_edit_LoadTable_2YRow_Y.SetTextSize(30);

	m_edit_LoadTable_2YRow_Z_Pick.SetBackColor(COLOR_WWHITE);
	m_edit_LoadTable_2YRow_Z_Pick.SetTextSize(30);

	m_edit_LoadTable_2YRow_Z_Place.SetBackColor(COLOR_WWHITE);
	m_edit_LoadTable_2YRow_Z_Place.SetTextSize(30);
}



void CUserOffSetTestPp_MP::OnUpdateEditOffSet(int nIdx)
{
	m_dX = m_TeachTestPp.stTestSite[nIdx].nOffSet[eTestPpOffSet_X] / 1000.0;
	m_dY = m_TeachTestPp.stTestSite[nIdx].nOffSet[eTestPpOffSet_Y] / 1000.0;
	m_dZ_Pick = m_TeachTestPp.stTestSite[nIdx].nOffSet[eTestPpOffSet_Z_Pick] / 1000.0;
	m_dZ_Place = m_TeachTestPp.stTestSite[nIdx].nOffSet[eTestPpOffSet_Z_Place] / 1000.0;

	UpdateData(FALSE);
}

void CUserOffSetTestPp_MP::OnUpdateEditOffSet_LoadTable(int nIdx)
{
// 	m_dLoadTable_2YRow_X = m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_X] / 1000.0;
// 	m_dLoadTable_2YRow_Y = m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Y] / 1000.0;
// 	m_dLoadTable_2YRow_Z_Pick = m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick] / 1000.0;
// 	m_dLoadTable_2YRow_Z_Place = m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place] / 1000.0;
// 	
	UpdateData(FALSE);
}


void CUserOffSetTestPp_MP::IsSameOffSet(ST_USER_OFFSET_TESTPP_MP TeachTestPpBefore, int nSiteAreaIdx)
{
	CString strTestppName =_T("");
	CString strSiteNum =_T("");
	CString strDataName =_T("");

	switch(m_nRadioSel)
	{
		case ePressUnit_Down_1:	strSiteNum = _T("#1"); strTestppName = _T("Test Pp #1"); break;
		case ePressUnit_Down_2:	strSiteNum = _T("#2"); strTestppName = _T("Test Pp #1"); break;
		case ePressUnit_Down_3:	strSiteNum = _T("#3"); strTestppName = _T("Test Pp #1"); break;
		case ePressUnit_Down_4: strSiteNum = _T("#4"); strTestppName = _T("Test Pp #1"); break;
		case ePressUnit_Down_5:	strSiteNum = _T("#5"); strTestppName = _T("Test Pp #2"); break;
		case ePressUnit_Down_6:	strSiteNum = _T("#6"); strTestppName = _T("Test Pp #2"); break;
		case ePressUnit_Down_7:	strSiteNum = _T("#7"); strTestppName = _T("Test Pp #2"); break;
		case ePressUnit_Down_8: strSiteNum = _T("#8"); strTestppName = _T("Test Pp #2"); break;
// 		case ePressUnit_Up_1  :	strSiteNum = _T("#9"); strTestppName = _T("Test Pp #1"); break;
// 		case ePressUnit_Up_2  :	strSiteNum = _T("#10"); strTestppName = _T("Test Pp #1"); break;
// 		case ePressUnit_Up_3  :	strSiteNum = _T("#11"); strTestppName = _T("Test Pp #1"); break;
// 		case ePressUnit_Up_4  : strSiteNum = _T("#12"); strTestppName = _T("Test Pp #1"); break;
// 		case ePressUnit_Up_5  :	strSiteNum = _T("#13"); strTestppName = _T("Test Pp #2"); break;
// 		case ePressUnit_Up_6  :	strSiteNum = _T("#14"); strTestppName = _T("Test Pp #2"); break;
// 		case ePressUnit_Up_7  :	strSiteNum = _T("#15"); strTestppName = _T("Test Pp #2"); break;
// 		case ePressUnit_Up_8  : strSiteNum = _T("#16"); strTestppName = _T("Test Pp #2"); break;
	}
	
	if(m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_X] != TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_X])
	{
		MakeLog("[MENU = Offset, TRAY FILE NAME = %s, %s, Station NUMBER = %s, DATA NAME = X][BEFORE= %.2f, AFTER= %.2f]",m_szCokName, strTestppName, strSiteNum, (double)TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_X]/1000, (double)m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_X]/1000);
	}
	if(m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Y] != TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Y])
	{
		MakeLog("[MENU = Offset, TRAY FILE NAME = %s, %s, Station NUMBER = %s, DATA NAME = Y][BEFORE= %.2f, AFTER= %.2f]",m_szCokName,strTestppName, strSiteNum, (double)TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Y]/1000, (double)m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Y]/1000);
	}
	if (m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Pick] != TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Pick])
	{
		MakeLog("[MENU = Offset, TRAY FILE NAME = %s, %s, Station NUMBER = %s, DATA NAME = Z Pick][BEFORE= %.2f, AFTER= %.2f]", m_szCokName, strTestppName, strSiteNum, (double)TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Pick] / 1000, (double)m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Pick] / 1000);
	}
	if (m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Place] != TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Place])
	{
		MakeLog("[MENU = Offset, TRAY FILE NAME = %s, %s, Station NUMBER = %s, DATA NAME = Z Place][BEFORE= %.2f, AFTER= %.2f]", m_szCokName, strTestppName, strSiteNum, (double)TeachTestPpBefore.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Place] / 1000, (double)m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Place] / 1000);
 	}

}

void CUserOffSetTestPp_MP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_Teaching_Offset), Debug, "", __LINE__, NULL, tmpMsg);
}

void CUserOffSetTestPp_MP::InitControl()
{
	
}

void CUserOffSetTestPp_MP::OnEnSetfocusEditX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dX);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "X", "3.0", "-3.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_X)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dY);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Y", "3.0", "-3.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_Y)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditZPick()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dZ_Pick);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z Pick", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_Z_PICK)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditZPlace()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dZ_Place);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z Place", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_Z_PLACE)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnBnClickedButtonSave()
{
	UpdateData(TRUE);
	ST_USER_OFFSET_TESTPP_MP TeachTestPpBefore;

	int nRadio = 0;
	int nPpIdx = 0, nSiteAreaIdx = 0, nLoadTable = 0;

	// Get Test Pp Idx
	//switch (m_nRadioSel)
	//{
	//case ePressUnit_Down_1:	case ePressUnit_Down_2:	case ePressUnit_Down_3:	case ePressUnit_Down_4:
	//case ePressUnit_Up_1:	case ePressUnit_Up_2: case ePressUnit_Up_3:	case ePressUnit_Up_4:
	//{
	//	nPpIdx = eTestPP_1;
	//}break;
	//case ePressUnit_Down_5:	case ePressUnit_Down_6:	case ePressUnit_Down_7:	case ePressUnit_Down_8:
	//case ePressUnit_Up_5:	case ePressUnit_Up_6: case ePressUnit_Up_7:	case ePressUnit_Up_8:
	//{
	//	nPpIdx = eTestPP_2;
	//}break;
	//}
	switch (m_nRadioSel_TestPp)
	{
		case eTestPP_1:
		{
			nPpIdx = eTestPP_1;
		}break;

		case eTestPP_2:
		{
			nPpIdx = eTestPP_2;
		}break;
	}


	// Get Area Idx
	switch (m_nRadioSel)
	{
	case ePressUnit_Down_1:	case ePressUnit_Down_5:
	{
		nSiteAreaIdx = eATS_DN_STATION_L1;
	}break;
	case ePressUnit_Down_2:	case ePressUnit_Down_6:
	{
		nSiteAreaIdx = eATS_DN_STATION_L2;
	}break;
	case ePressUnit_Down_3:	case ePressUnit_Down_7:
	{
		nSiteAreaIdx = eATS_DN_STATION_L3;
	}break;
	case ePressUnit_Down_4:	case ePressUnit_Down_8:
	{
		nSiteAreaIdx = eATS_DN_STATION_L4;
	}break;

// 	case ePressUnit_Up_1:	case ePressUnit_Up_5:
// 	{
// 		nSiteAreaIdx = eATS_UP_STATION_L1;
// 	}break;
// 	case ePressUnit_Up_2:	case ePressUnit_Up_6:
// 	{
// 		nSiteAreaIdx = eATS_UP_STATION_L2;
// 	}break;
// 	case ePressUnit_Up_3:	case ePressUnit_Up_7:
// 	{
// 		nSiteAreaIdx = eATS_UP_STATION_L3;
// 	}break;
// 	case ePressUnit_Up_4:	case ePressUnit_Up_8:
// 	{
// 		nSiteAreaIdx = eATS_UP_STATION_L4;
// 	}break;
	}

	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, nPpIdx, m_TeachTestPp);

	TeachTestPpBefore = m_TeachTestPp;

	m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_X] = (int)(m_dX * 1000);
	m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Y] = (int)(m_dY * 1000);
 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Pick] = (int)(m_dZ_Pick * 1000);
 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Place] = (int)(m_dZ_Place * 1000);
// 
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_X] = (int)(m_dRearSite_FrontPicker_X * 1000);
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_Y] = (int)(m_dRearSite_FrontPicker_Y * 1000);
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = (int)(m_dRearSite_FrontPicker_Z_Pick * 1000);
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = (int)(m_dRearSite_FrontPicker_Z_Place * 1000);
// 
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_X] = (int)(m_dFrontSite_FrontPicker_X * 1000);
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y] = (int)(m_dFrontSite_FrontPicker_Y * 1000);
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = (int)(m_dFrontSite_FrontPicker_Z_Pick * 1000);
// 	m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = (int)(m_dFrontSite_FrontPicker_Z_Place * 1000);
// 
// 	m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_X] = (int)(m_dLoadTable_2YRow_X * 1000);
// 	m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Y] = (int)(m_dLoadTable_2YRow_Y * 1000);
// 	m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick] = (int)(m_dLoadTable_2YRow_Z_Pick * 1000);
// 	m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place] = (int)(m_dLoadTable_2YRow_Z_Place * 1000);

	g_COK_FileMgr.SaveUserOffSetDataTestPp_MP(m_szCokName, nPpIdx, &m_TeachTestPp);

	g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_USER_OFFSET_REFESH);

	IsSameOffSet(TeachTestPpBefore, nSiteAreaIdx);

}


void CUserOffSetTestPp_MP::OnBnClickedRdSite1()
{
	m_nRadioSel = ePressUnit_Down_1;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L1);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite2()
{
	m_nRadioSel = ePressUnit_Down_2;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L2);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite3()
{
	m_nRadioSel = ePressUnit_Down_3;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L3);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite4()
{
	m_nRadioSel = ePressUnit_Down_4;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L4);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite5()
{
	m_nRadioSel = ePressUnit_Down_5;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L1);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite6()
{
	m_nRadioSel = ePressUnit_Down_6;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L2);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite7()
{
	m_nRadioSel = ePressUnit_Down_7;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L3);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite8()
{
	m_nRadioSel = ePressUnit_Down_8;
	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
	OnUpdateEditOffSet(eATS_DN_STATION_L4);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite9()
{
// 	m_nRadioSel = ePressUnit_Up_1;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L1);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite10()
{
// 	m_nRadioSel = ePressUnit_Up_2;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L2);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite11()
{
// 	m_nRadioSel = ePressUnit_Up_3;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L3);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite12()
{
// 	m_nRadioSel = ePressUnit_Up_4;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L4);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite13()
{
// 	m_nRadioSel = ePressUnit_Up_5;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L1);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite14()
{
// 	m_nRadioSel = ePressUnit_Up_6;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L2);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite15()
{
// 	m_nRadioSel = ePressUnit_Up_7;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L3);
}


void CUserOffSetTestPp_MP::OnBnClickedRdSite16()
{
// 	m_nRadioSel = ePressUnit_Up_8;
// 	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
// 	OnUpdateEditOffSet(eATS_UP_STATION_L4);
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dRearSite_FrontPicker_X);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "X", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_REARSITE_FRONTPICKER_X)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dRearSite_FrontPicker_Y);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Y", "3.0", "-3.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_REARSITE_FRONTPICKER_Y)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerZPick()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dRearSite_FrontPicker_Z_Pick);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_REARSITE_FRONTPICKER_Z_PICK)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditRearsiteFrontpickerZPlace()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dRearSite_FrontPicker_Z_Place);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_REARSITE_FRONTPICKER_Z_PLACE)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFrontSite_FrontPicker_X);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "X", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FRONTSITE_FRONTPICKER_X)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFrontSite_FrontPicker_Y);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Y", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FRONTSITE_FRONTPICKER_Y)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerZPick()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFrontSite_FrontPicker_Z_Pick);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PICK)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditFrontsiteFrontpickerZPlace()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFrontSite_FrontPicker_Z_Place);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FRONTSITE_FRONTPICKER_Z_PLACE)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLoadTable_2YRow_X);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "X", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOADTABLE_X)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLoadTable_2YRow_Y);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Y", "3.0", "-3.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOADTABLE_Y)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableZPick()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLoadTable_2YRow_Z_Pick);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOADTABLE_Z_PICK)->SetWindowText(szRetVal);
	}
}


void CUserOffSetTestPp_MP::OnEnSetfocusEditLoadtableZPlace()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLoadTable_2YRow_Z_Place);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Z", "1.0", "-1.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOADTABLE_Z_PLACE)->SetWindowText(szRetVal);
	}
}

void CUserOffSetTestPp_MP::OnBnClickedRdLdtbl1()
{
	m_nRadioSel_TestPp = eTestPP_1;
	m_nRadioSel = ePressUnit_Down_1;
	TestPpRadioButtonShow(eTestPP_1);

	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_1, m_TeachTestPp);
	OnUpdateEditOffSet_LoadTable(eTestPP_1);
	OnUpdateEditOffSet(eATS_DN_STATION_L1);	
}


void CUserOffSetTestPp_MP::OnBnClickedRdLdtbl2()
{
	m_nRadioSel_TestPp = eTestPP_2;
	m_nRadioSel = ePressUnit_Down_5;
	TestPpRadioButtonShow(eTestPP_2);

	g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, eTestPP_2, m_TeachTestPp);
	OnUpdateEditOffSet_LoadTable(eTestPP_2);
	OnUpdateEditOffSet(eATS_DN_STATION_L1);
}

void CUserOffSetTestPp_MP::TestPpRadioButtonShow(int nIdx) 
{
	switch (nIdx)
	{
		case eTestPP_1:
		{
			GetDlgItem(IDC_RD_SITE1)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE3)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE4)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE9)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE10)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE11)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE12)->EnableWindow(TRUE);

			GetDlgItem(IDC_RD_SITE5)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE6)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE7)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE8)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE13)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE14)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE15)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE16)->EnableWindow(FALSE);

		}break;
		case eTestPP_2:
		{
			GetDlgItem(IDC_RD_SITE1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE3)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE4)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE9)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE10)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE11)->EnableWindow(FALSE);
			GetDlgItem(IDC_RD_SITE12)->EnableWindow(FALSE);

			GetDlgItem(IDC_RD_SITE5)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE6)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE7)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE8)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE13)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE14)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE15)->EnableWindow(TRUE);
			GetDlgItem(IDC_RD_SITE16)->EnableWindow(TRUE);
		}break;
	}
}

void CUserOffSetTestPp_MP::OnBnClickedButtonDefault()
{
	UpdateData(TRUE);
	ST_USER_OFFSET_TESTPP_MP TeachTestPpBefore;

	int nRet = AfxMessageBox("Do you wan's Default?", MB_TOPMOST | MB_YESNO);
	if (nRet != IDYES) {
		return;
	}

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();

	int nUserLevel = pSltView->m_nLogInLv;
	int nRetLevel = 0;

	// 이전 선택이 Use였고, 현재 변경 된 Status가 Not Use 일 경우만 판단 & 사용자 Level이 Engineer 이하 등급일 경우[현재 로그인 상태 포함]

	BOOL bRet = g_LibCommCtrl.ShowLogInBox(nUserLevel, &nRetLevel);

	if (RETURN_OK == bRet) {
	}
	else
	{
		return;
	}


	int nRadio = 0;
	int nPpIdx = 0, nSiteAreaIdx = 0, nLoadTable = 0;

	for (int nPpIdx = 0; nPpIdx < eMaxTestPPCount; nPpIdx++)
	{
		g_COK_FileMgr.LoadUserOffSetDataTestPp_MP(m_szCokName, nPpIdx, m_TeachTestPp);

		TeachTestPpBefore = m_TeachTestPp;
		for (nSiteAreaIdx = 0; nSiteAreaIdx < eMaxATS_Area; nSiteAreaIdx++)
		{
			m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_X] = 0.0;
			m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Y] = 0.0;
			m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Pick] = 0.0;
 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nOffSet[eTestPpOffSet_Z_Place] = 0.0;
// 
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_X] = 0.0;
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_Y] = 0.0;
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = 0.0;
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = 0.0;
// 
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_X] = 0.0;
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y] = 0.0;
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = 0.0;
// 			m_TeachTestPp.stTestSite[nSiteAreaIdx].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = 0.0;
// 
// 			m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_X] = 0.0;
// 			m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Y] = 0.0;
// 			m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick] = 0.0;
// 			m_TeachTestPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place] = 0.0;
		}

		g_COK_FileMgr.SaveUserOffSetDataTestPp_MP(m_szCokName, nPpIdx, &m_TeachTestPp);

		IsSameOffSet(TeachTestPpBefore, nSiteAreaIdx);
	}

	g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_USER_OFFSET_REFESH);

	TestPpRadioButtonShow(eTestPP_1);
	OnUpdateEditOffSet(eATS_DN_STATION_L1);
	OnUpdateEditOffSet_LoadTable(eTestPP_1);

	this->SendMessage(WM_COMMAND, IDC_RD_SITE1, 0);
}
