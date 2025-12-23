// EngrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "EngrDlg.h"
#include "afxdialogex.h"


// CEngrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEngrDlg, CDialog)

CEngrDlg::CEngrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEngrDlg::IDD, pParent)
{
	m_nHddAlarmPer = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_ALARM);
	m_nHddInterlockPer = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_INTERLOCK);
	m_szPathFileName[MAX_PATH] = {0,};
}

CEngrDlg::~CEngrDlg()
{
}

void CEngrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ALIGNMENT, m_BtnAlign);
	DDX_Control(pDX, IDC_BUTTON_IO_MONITOR, m_BtnIOMonitor);
    DDX_Control(pDX, IDC_BUTTON_AIO_MONITOR, m_BtnAIOMonitor);
    DDX_Control(pDX, IDC_BUTTON_MANUAL, m_BtnManual);
	DDX_Control(pDX, IDC_BUTTON_MOT_MONITOR, m_BtnMotorMonitor);
	DDX_Control(pDX, IDC_BUTTON_EVENT_LOG, m_BtnEventLog);
	DDX_Control(pDX, IDC_BUTTON_GRR_SETTING, m_BtnGrrSetting);
	DDX_Control(pDX, IDC_BUTTON_GRR_RESULT, m_BtnGrrResult);
	DDX_Control(pDX, IDC_BUTTON_OFFSET, m_BtnOffset);
	DDX_Text(pDX, IDC_EDIT_HDD_ALARM, m_nHddAlarmPer);
	DDX_Text(pDX, IDC_EDIT_HDD_INTERLOCK, m_nHddInterlockPer);

	DDX_Control(pDX, IDC_EDIT_HDD_ALARM, m_CtrlHddAlarm);
	DDX_Control(pDX, IDC_EDIT_HDD_INTERLOCK, m_CtrlHddInterlock);
	DDX_Control(pDX, IDC_BUTTON_VISION_AUTO_TEACHING, m_BtnVisionAutoTeaching);
}


BEGIN_MESSAGE_MAP(CEngrDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALIGNMENT, &CEngrDlg::OnBnClickedButtonAlignment)
	ON_BN_CLICKED(IDC_BUTTON_IO_MONITOR, &CEngrDlg::OnBnClickedButtonIoMonitor)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_LOG, &CEngrDlg::OnBnClickedButtonEventLog)
    ON_BN_CLICKED(IDC_BUTTON_AIO_MONITOR, &CEngrDlg::OnBnClickedButtonAioMonitor)
    ON_BN_CLICKED(IDC_BUTTON_MANUAL, &CEngrDlg::OnBnClickedButtonManual)
    ON_BN_CLICKED(IDC_BUTTON_MOT_MONITOR, &CEngrDlg::OnBnClickedButtonMotMonitor)
	ON_BN_CLICKED(IDC_BUTTON_GRR_SETTING, &CEngrDlg::OnBnClickedButtonGrrSetting)
	ON_BN_CLICKED(IDC_BUTTON_GRR_RESULT, &CEngrDlg::OnBnClickedButtonGrrResult)
	ON_EN_SETFOCUS(IDC_EDIT_HDD_ALARM, &CEngrDlg::OnEnSetfocusEditHddAlarm)
	ON_EN_SETFOCUS(IDC_EDIT_HDD_INTERLOCK, &CEngrDlg::OnEnSetfocusEditHddInterlock)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OFFSET, &CEngrDlg::OnBnClickedButtonOffset)
	ON_BN_CLICKED(IDC_BUTTON_VISION_AUTO_TEACHING, &CEngrDlg::OnBnClickedButtonVisionAutoTeaching)
END_MESSAGE_MAP()


// CEngrDlg 메시지 처리기입니다.


BOOL CEngrDlg::IsEQPRunning()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB( BDM0_EQP_RUN_STATE );
	if( bLotState == TRUE ) {
		AfxMessageBox( _T( "Not available while running" ) );
		return TRUE;
	}
	return FALSE;
}

BOOL CEngrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();

	ZeroMemory(m_szPathFileName, sizeof(m_szPathFileName));
	char szPathTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
	sprintf_s(m_szPathFileName, sizeof(m_szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);

    CRect rect;
    GetClientRect(&rect);
    SetWindowPos(&wndNoTopMost, 0,0, rect.Width(), rect.Height(), SWP_SHOWWINDOW );

	SetTimer( 0, 1000, NULL );

	CompanySetting();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEngrDlg::OnBtnGUI()
{
	m_BtnAlign.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnAlign.SetIcon(IDI_BTN_ALIGNMENT);
	m_BtnAlign.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnIOMonitor.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnIOMonitor.SetIcon(IDI_ICON_DIO);
	m_BtnIOMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);	

    m_BtnManual.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnManual.SetIcon(IDI_BTN_POSITION);
    m_BtnManual.SetAlign(CButtonST::ST_ALIGN_VERT);	
    
    m_BtnAIOMonitor.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnAIOMonitor.SetIcon(IDI_ICON_AIO);
    m_BtnAIOMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnMotorMonitor.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnMotorMonitor.SetIcon(IDI_BTN_MOTOR_MONITOR);
	m_BtnMotorMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnEventLog.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnEventLog.SetIcon(IDI_MAIN_EVENTLOG);
	m_BtnEventLog.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnGrrSetting.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnGrrSetting.SetIcon(IDI_BTN_GRR_SETTING);
	m_BtnGrrSetting.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnGrrResult.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnGrrResult.SetIcon(IDI_BTN_GRR);
	m_BtnGrrResult.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnOffset.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnOffset.SetIcon(IDI_BTN_OFFSET);
	m_BtnOffset.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE) == FALSE) {
		GetDlgItem(IDC_BUTTON_VISION_AUTO_TEACHING)->ShowWindow(SW_HIDE);
	}
	else {
		m_BtnVisionAutoTeaching.SetWindowText(_T("Z Auto Teaching"));
		m_BtnVisionAutoTeaching.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
		m_BtnVisionAutoTeaching.SetIcon(IDI_BTN_VAT);
		m_BtnVisionAutoTeaching.SetAlign(CButtonST::ST_ALIGN_VERT);
	}

	m_CtrlHddAlarm.SetBackColor(COLOR_WWHITE);
	m_CtrlHddAlarm.SetTextSize(30);

	m_CtrlHddInterlock.SetBackColor(COLOR_WWHITE);
	m_CtrlHddInterlock.SetTextSize(30);
}

void CEngrDlg::OnTimer( UINT_PTR nIDEvent )
{
	UpdateData();
	KillTimer( nIDEvent );
	//if( g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue ) == eStartMD_AutoGRR || g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue ) == eStartMD_AutoGRR_Continue ) {
	//	GetDlgItem( IDC_BUTTON_GRR_SETTING )->ShowWindow( SW_SHOW );
	//	//GetDlgItem( IDC_BUTTON_GRR_RESULT )->ShowWindow( SW_SHOW );
	//}else {
	//	GetDlgItem( IDC_BUTTON_GRR_SETTING )->ShowWindow( SW_HIDE );
	//	//GetDlgItem( IDC_BUTTON_GRR_RESULT )->ShowWindow( SW_HIDE );
	//}
	SetTimer( 0, 1000, NULL );

	CDialog::OnTimer( nIDEvent );
}

void CEngrDlg::OnBnClickedButtonAlignment()
{
	if( IsEQPRunning() == TRUE )
		return;

	MakeLog("[Alignment Dialog Show]");

	g_McComm.OnSendDlgShow(eDlgProcCmd_Alignment);
}


void CEngrDlg::OnBnClickedButtonIoMonitor()
{
	if( IsEQPRunning() == TRUE )
		return;

	MakeLog("[Dio Monitor Dialog Show]");

	g_McComm.OnSendDlgShow(eDlgProcCmd_IoMonitorDIO);
}


void CEngrDlg::OnBnClickedButtonMotorMonitor()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);
	if(bLotState==TRUE){
		return;
	}

	g_McComm.OnSendDlgShow(eDlgProcCmd_MotorMonitor);
}


void CEngrDlg::OnBnClickedButtonEventLog()
{
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Event Log Dialog Show]");
		CEventLogDlg DO;
		DO.DoModal();
	}
}


BOOL CEngrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_ESCAPE ){
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CEngrDlg::OnBnClickedButtonAioMonitor()
{
	if( IsEQPRunning() == TRUE )
		return;

	MakeLog("[Aio Monitor Dialog Show]");

    g_McComm.OnSendDlgShow(eDlgProcCmd_IoMonitorAIO);
}


void CEngrDlg::OnBnClickedButtonManual()
{
	if( IsEQPRunning() == TRUE )
		return;

	MakeLog("[Manual All Dialog Show]");

    g_McComm.OnSendDlgShow(eDlgProcCmd_Position);
}


void CEngrDlg::OnBnClickedButtonMotMonitor()
{
	if( IsEQPRunning() == TRUE )
		return;

	MakeLog("[Motor Monitor Dialog Show]");

    g_McComm.OnSendDlgShow(eDlgProcCmd_MotorMonitor);
}

void CEngrDlg::MakeLog(LPCTSTR fmt, ...)
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

void CEngrDlg::OnBnClickedButtonGrrSetting()
{
	if( IsEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[GrrSetting Dialog Show]");
		CAutoGrrSetDlg dlg;
		dlg.DoModal();
	}
}


void CEngrDlg::OnBnClickedButtonGrrResult()
{
	//if( IsEQPRunning() == TRUE )
	//	return;
	MakeLog( "[GrrResult Dialog Show]" );
	//CAutoGrrResultDlg dlg;
	//dlg.DoModal();

	CAutoGrrResultDlg *dlg = new CAutoGrrResultDlg;
	dlg->Create(IDD_DIALOG_AUTOGRR_RESULT, this);
	dlg->ShowWindow(SW_SHOW);
}

void CEngrDlg::OnEnSetfocusEditHddAlarm()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nHddAlarmPer, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "HDD Alarm", "80", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_HDD_ALARM)->SetWindowText(szRetVal);
		g_DMCont.m_dmEQP.SN(NDM0_HDD_SIZE_ALARM, atoi(szRetVal));

		CConfigData ConfigPathData(m_szPathFileName);
		ConfigPathData.Set("HDD","ALARM", atoi(szRetVal));
	}

	UpdateData(TRUE);
}


void CEngrDlg::OnEnSetfocusEditHddInterlock()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nHddInterlockPer, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "HDD Interlock", "80", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_HDD_INTERLOCK)->SetWindowText(szRetVal);
		g_DMCont.m_dmEQP.SN(NDM0_HDD_SIZE_INTERLOCK, atoi(szRetVal));

		CConfigData ConfigPathData(m_szPathFileName);
		ConfigPathData.Set("HDD","INTERLOCK", atoi(szRetVal));
	}

	UpdateData(TRUE);
}

void CEngrDlg::CompanySetting()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	//if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		GetDlgItem(IDC_BUTTON_GRR_SETTING)->ShowWindow(SW_SHOW);
	//}
	//else if (strcmp(szCompany, DEF_COMPANY_USI) == 0) {
	//	GetDlgItem(IDC_BUTTON_GRR_SETTING)->ShowWindow(SW_HIDE);
	//}
}

void CEngrDlg::OnBnClickedButtonOffset()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);
	if (bLotState == TRUE) {
		return;
	}

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Offset Test Pp Dialog Show]");

		CUserOffSetTestPp_MP DO;
		DO.DoModal();
	}
}


void CEngrDlg::OnBnClickedButtonVisionAutoTeaching()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IsEQPRunning() == TRUE)
		return;

	MakeLog("[Vision Auto Teaching Dialog Show]");

	g_McComm.OnSendDlgShow(eDlgProcCmd_VAT);
}
