// DlgSetupCOKInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgSetupCOKInfo.h"
#include "afxdialogex.h"


// CDlgSetupCOKInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSetupCOKInfo, CDialogEx)

CDlgSetupCOKInfo::CDlgSetupCOKInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetupCOKInfo::IDD, pParent)
{
	m_nPlateDiv_X = 0;
	m_nPlateDiv_Y = 0;
	m_dPlate_XPitch = 0.0;
	m_dPlate_XStart = 0.0;
	m_dPlate_YPitch = 0.0;
	m_dPlate_YStart = 0.0;

	m_nTrayDiv_Y = 0;
	m_nTrayDiv_X = 0;
	m_dTray_YPitch = 0.0;
	m_dTray_YStart = 0.0;
	m_dTray_XPitch = 0.0;
	m_dTray_XStart = 0.0;
	m_dTray_Thickness = 0.0;

	m_dLdTbl_XPitch = 0.0;
	m_dLdTbl_YPitch = 0.0;

	m_nTempMode = eTEMP_AMBIENT;
}

CDlgSetupCOKInfo::~CDlgSetupCOKInfo()
{
	m_cBmpBaseSetting.DeleteObject();
	m_cBmpLoad.DeleteObject();
	m_cBmpSave.DeleteObject();
	m_cBmpExit.DeleteObject();
}

void CDlgSetupCOKInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_DIV_PLATE, m_nPlateDiv_X);
	DDX_Text(pDX, IDC_EDIT_X_DIV_TRAY, m_nTrayDiv_X);
	DDX_Text(pDX, IDC_EDIT_XPITCH_PLATE, m_dPlate_XPitch);
	DDX_Text(pDX, IDC_EDIT_XSTART_PLATE, m_dPlate_XStart);
	DDX_Text(pDX, IDC_EDIT_YPITCH_PLATE, m_dPlate_YPitch);
	DDX_Text(pDX, IDC_EDIT_YSTART_PLATE, m_dPlate_YStart);
	DDX_Text(pDX, IDC_EDIT_XPITCH_TRAY, m_dTray_XPitch);
	DDX_Text(pDX, IDC_EDIT_XSTART_TRAY, m_dTray_XStart);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_TRAY, m_nTrayDiv_Y);
	DDX_Text(pDX, IDC_EDIT_YPITCH_TRAY, m_dTray_YPitch);
	DDX_Text(pDX, IDC_EDIT_YSTART_TRAY, m_dTray_YStart);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_PLATE, m_nPlateDiv_Y);
	DDX_Radio(pDX, IDC_RADIO_TMP1, m_nTempMode);
	DDX_Text(pDX, IDC_EDIT_CTRAY_THICKNESS, m_dTray_Thickness);
	DDX_Text(pDX, IDC_EDIT_XPITCH_LD_TBL, m_dLdTbl_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_LD_TBL, m_dLdTbl_YPitch);		

	// base setting
	DDX_Control(pDX, IDC_BUTTON_BASE_SET, m_btnBaseSetting);
	// load
	DDX_Control(pDX, IDC_BUTTON_KIT_CHANGE, m_btnLoad);
	// save
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	// exit
	DDX_Control(pDX, IDCANCEL, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgSetupCOKInfo, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgSetupCOKInfo::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_XSTART_TRAY, &CDlgSetupCOKInfo::OnEnSetfocusEditXstartTray)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_TRAY, &CDlgSetupCOKInfo::OnEnSetfocusEditXpitchTray)
	ON_EN_SETFOCUS(IDC_EDIT_YSTART_TRAY, &CDlgSetupCOKInfo::OnEnSetfocusEditYstartTray)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_TRAY, &CDlgSetupCOKInfo::OnEnSetfocusEditYpitchTray)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_TRAY, &CDlgSetupCOKInfo::OnEnSetfocusEditXDivTray)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_TRAY, &CDlgSetupCOKInfo::OnEnSetfocusEditYDivTray)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_LD_TBL, &CDlgSetupCOKInfo::OnEnSetfocusEditXpitchLdTbl)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_LD_TBL, &CDlgSetupCOKInfo::OnEnSetfocusEditYpitchLdTbl)
	ON_EN_SETFOCUS(IDC_EDIT_CTRAY_THICKNESS, &CDlgSetupCOKInfo::OnEnSetfocusEditCtrayThickness)
END_MESSAGE_MAP()


// CDlgSetupCOKInfo 메시지 처리기입니다.


BOOL CDlgSetupCOKInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgSetupCOKInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// base setting
	m_cBmpBaseSetting.LoadBitmap(IDB_COK_BASE_SETTING);
	m_btnBaseSetting.SetBitmap(m_cBmpBaseSetting);
	// load
	m_cBmpLoad.LoadBitmap(IDB_COK_LOAD);
	m_btnLoad.SetBitmap(m_cBmpLoad);
	// save
	m_cBmpSave.LoadBitmap(IDB_COK_SAVE);
	m_btnSave.SetBitmap(m_cBmpSave);
	// exit
	m_cBmpExit.LoadBitmap(IDB_COK_EXIT);
	m_btnExit.SetBitmap(m_cBmpExit);

	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));
	ST_COK_INFO   stCokData;
	ST_BASIC_INFO stBasicData;

	g_CokMgr.LoadCokInfo(szData, stCokData);
	g_CokMgr.LoadBasicInfo(szData, stBasicData);
	
	CString strTmpData = _T("");
	strTmpData.Format("%.3f", stCokData.aTrayForm.nStart_PitchX/1000.0);
	SetDlgItemText(IDC_EDIT_XSTART_TRAY, strTmpData);
	strTmpData.Format("%.3f", stCokData.aTrayForm.nPitchX/1000.0);
	SetDlgItemText(IDC_EDIT_XPITCH_TRAY, strTmpData);

	strTmpData.Format("%.3f", stCokData.aTrayForm.nStart_PitchY/1000.0);
	SetDlgItemText(IDC_EDIT_YSTART_TRAY, strTmpData);
	strTmpData.Format("%.3f", stCokData.aTrayForm.nPitchY/1000.0);
	SetDlgItemText(IDC_EDIT_YPITCH_TRAY, strTmpData);
	
	strTmpData.Format("%d", stCokData.aTrayForm.nCnt_X);
	SetDlgItemText(IDC_EDIT_X_DIV_TRAY, strTmpData);
	strTmpData.Format("%d", stCokData.aTrayForm.nCnt_Y);
	SetDlgItemText(IDC_EDIT_Y_DIV_TRAY, strTmpData);
	
	strTmpData.Format("%.3f", stCokData.aTrayForm.nThickness/1000.0);
	SetDlgItemText(IDC_EDIT_CTRAY_THICKNESS, strTmpData);

	strTmpData.Format("%.3f", stCokData.aPlateForm.nStart_PitchX/1000.0);
	SetDlgItemText(IDC_EDIT_XSTART_PLATE, strTmpData);
	strTmpData.Format("%.3f", stCokData.aPlateForm.nPitchX/1000.0);
	SetDlgItemText(IDC_EDIT_XPITCH_PLATE, strTmpData);

	strTmpData.Format("%.3f", stCokData.aPlateForm.nStart_PitchY/1000.0);
	SetDlgItemText(IDC_EDIT_YSTART_PLATE, strTmpData);
	strTmpData.Format("%.3f", stCokData.aPlateForm.nPitchY/1000.0);
	SetDlgItemText(IDC_EDIT_YPITCH_PLATE, strTmpData);

	strTmpData.Format("%d", stCokData.aPlateForm.nCnt_X);
	SetDlgItemText(IDC_EDIT_X_DIV_PLATE, strTmpData);
	strTmpData.Format("%d", stCokData.aPlateForm.nCnt_Y);
	SetDlgItemText(IDC_EDIT_Y_DIV_PLATE, strTmpData);

	strTmpData.Format("%.3f", stCokData.nLoadTbl_Xpitch/1000.0);
	SetDlgItemText(IDC_EDIT_XPITCH_LD_TBL, strTmpData);
	strTmpData.Format("%.3f", stCokData.nLoadTbl_Ypitch/1000.0);
	SetDlgItemText(IDC_EDIT_YPITCH_LD_TBL, strTmpData);

	stBasicData.bTempMode = (BOOL)m_nTempMode;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSetupCOKInfo::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgSetupCOKInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgSetupCOKInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgSetupCOKInfo::OnEnSetfocusEditXstartTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_XStart);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray X Start", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XSTART_TRAY)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditXpitchTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_TRAY)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditYstartTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_YStart);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Y Start", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YSTART_TRAY)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditYpitchTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_TRAY)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditXDivTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTrayDiv_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray X Division", "100", "2");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_X_DIV_TRAY)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditYDivTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTrayDiv_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Y Division", "100", "2");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_Y_DIV_TRAY)->SetWindowText(szRetVal);
	}
}

void CDlgSetupCOKInfo::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));

	ST_COK_INFO   stCokData;
	ST_BASIC_INFO stBasicData;
	g_CokMgr.LoadCokInfo(szData, stCokData);
	g_CokMgr.LoadBasicInfo(szData, stBasicData);

	stCokData.aTrayForm.nCnt_X         = m_nTrayDiv_X;
	stCokData.aTrayForm.nCnt_Y         = m_nTrayDiv_Y;
	stCokData.aTrayForm.nPitchX        = (int)(m_dTray_XPitch*1000);
	stCokData.aTrayForm.nPitchY        = (int)(m_dTray_YPitch*1000);
	stCokData.aTrayForm.nStart_PitchX  = (int)(m_dTray_XStart*1000);
	stCokData.aTrayForm.nStart_PitchY  = (int)(m_dTray_YStart*1000);
	stCokData.aTrayForm.nThickness     = (int)(m_dTray_Thickness*1000);

	stCokData.aPlateForm.nCnt_X        = m_nPlateDiv_X;
	stCokData.aPlateForm.nCnt_Y        = m_nPlateDiv_Y;
	stCokData.aPlateForm.nPitchX       = (int)(m_dPlate_XPitch*1000);
	stCokData.aPlateForm.nPitchY       = (int)(m_dPlate_YPitch*1000);
	stCokData.aPlateForm.nStart_PitchX = (int)(m_dPlate_XStart*1000);
	stCokData.aPlateForm.nStart_PitchY = (int)(m_dPlate_YStart*1000);

	stCokData.nLoadTbl_Xpitch		   = (int)(m_dLdTbl_XPitch*1000);
	stCokData.nLoadTbl_Ypitch		   = (int)(m_dLdTbl_YPitch*1000);

	stBasicData.bTempMode			   = (BOOL)m_nTempMode;

	g_CokMgr.SaveCokInfo(szData  , &stCokData);
	g_CokMgr.SaveBasicInfo(szData, &stBasicData);

	g_DMCont.m_dmTrayPP.SN(NDM5_TrayDiv_X   ,   stCokData.aTrayForm.nCnt_X);
	g_DMCont.m_dmTrayPP.SN(NDM5_TrayDiv_Y   ,   stCokData.aTrayForm.nCnt_Y);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Start_X,   stCokData.aTrayForm.nStart_PitchX);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Start_Y,   stCokData.aTrayForm.nStart_PitchY);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Pitch_X,   stCokData.aTrayForm.nPitchX);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Pitch_Y,   stCokData.aTrayForm.nPitchY);
	g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Thickness, stCokData.aTrayForm.nThickness);	

	g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Pitch_X,  stCokData.nLoadTbl_Xpitch);
	g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Pitch_Y,  stCokData.nLoadTbl_Ypitch);
}


void CDlgSetupCOKInfo::OnEnSetfocusEditXpitchLdTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLdTbl_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_LD_TBL)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditYpitchLdTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLdTbl_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_LD_TBL)->SetWindowText(szRetVal);
	}
}


void CDlgSetupCOKInfo::OnEnSetfocusEditCtrayThickness()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_Thickness);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Thickness", "10", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CTRAY_THICKNESS)->SetWindowText(szRetVal);
	}
}
