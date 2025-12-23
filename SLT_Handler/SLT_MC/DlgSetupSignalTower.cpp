// DlgSetupSignalTower.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgSetupSignalTower.h"
#include "afxdialogex.h"

// CDlgSetupSignalTower 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSetupSignalTower, CDialogEx)

CDlgSetupSignalTower::CDlgSetupSignalTower(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetupSignalTower::IDD, pParent)
{

}

CDlgSetupSignalTower::~CDlgSetupSignalTower()
{
	m_cBmpSave.DeleteObject();
	m_cBmpExit.DeleteObject();
}

void CDlgSetupSignalTower::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// save
	DDX_Control(pDX, IDC_BTN_TOWER_SAVE, m_btnSave);
	// exit
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	//Default
	DDX_Control(pDX, IDC_BTN_TOWER_DEFAULT, m_btnDefault);
}


BEGIN_MESSAGE_MAP(CDlgSetupSignalTower, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgSetupSignalTower::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_TOWER_SAVE, &CDlgSetupSignalTower::OnBnClickedBtnTowerSave)
	ON_BN_CLICKED(IDC_BTN_TOWER_DEFAULT, &CDlgSetupSignalTower::OnBnClickedBtnTowerDefault)
END_MESSAGE_MAP()


// CDlgSetupSignalTower 메시지 처리기입니다.

BOOL CDlgSetupSignalTower::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgSetupSignalTower::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);

	// save
	m_cBmpSave.LoadBitmap(IDB_TOWER_SAVE);
	m_btnSave.SetBitmap(m_cBmpSave);
	// exit
	m_cBmpExit.LoadBitmap(IDB_TOWER_EXIT);
	m_btnExit.SetBitmap(m_cBmpExit);

	m_btnDefault.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnDefault.SetAlign(CButtonST::ST_ALIGN_VERT);	

	GetTowerDataToSM();
	SetComboBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSetupSignalTower::GetTowerDataToSM()
{
	int nMainTitle[eSIGNAL_MAX] = {eSIGNAL_RUNNING, eSIGNAL_JAM_CALL, eSIGNAL_CYCLE_STOP, eSIGNAL_FINISH, eSIGNAL_RESET,
		                           eSIGNAL_PAUSE, eSIGNAL_REQUEST, eSIGNAL_TEMP_WAITING, eSIGNAL_TEST_RUNNING, eSIGNAL_START,
								   eSIGNAL_TEMP_STABILIZE};
	int nAddressSM[eSIGNAL_MAX] = {NDM0_Tower_Running_P, NDM0_Tower_JamCall_P, NDM0_Tower_Cycle_P, NDM0_Tower_Finish_P 
		                          ,NDM0_Tower_Reset_P, NDM0_Tower_Pause_P, NDM0_Tower_Request_P, NDM0_Tower_TempWait_P 
	                              ,NDM0_Tower_TestRun_P, NDM0_Tower_Start_P, NDM0_Tower_TempStabil_P};

	for(int i=0; i<eSIGNAL_MAX; i++)
	{
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_PRIORTY] = g_DMCont.m_dmEQP.GN(nAddressSM[i]);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_RED]	    = g_DMCont.m_dmEQP.GN(nAddressSM[i]+1);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_YELLOW]  = g_DMCont.m_dmEQP.GN(nAddressSM[i]+2);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_GREEN]	= g_DMCont.m_dmEQP.GN(nAddressSM[i]+3);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_SOUND]	= g_DMCont.m_dmEQP.GN(nAddressSM[i]+4);
	}
}

void CDlgSetupSignalTower::OnDestroy()
{
	CDialogEx::OnDestroy();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgSetupSignalTower::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgSetupSignalTower::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgSetupSignalTower::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CDlgSetupSignalTower::SetComboBox()
{
	CComboBox*	pMdcCombo = NULL;

	int  nID_Red[eSIGNAL_MAX]    = {IDC_CMB_RED1, IDC_CMB_RED2, IDC_CMB_RED3, IDC_CMB_RED4, IDC_CMB_RED5, IDC_CMB_RED6
	                               ,IDC_CMB_RED7, IDC_CMB_RED8, IDC_CMB_RED9, IDC_CMB_RED10, IDC_CMB_RED11};
	int  nID_Yellow[eSIGNAL_MAX] = {IDC_CMB_YELLOW1, IDC_CMB_YELLOW2, IDC_CMB_YELLOW3, IDC_CMB_YELLOW4, IDC_CMB_YELLOW5, IDC_CMB_YELLOW6
	                               ,IDC_CMB_YELLOW7, IDC_CMB_YELLOW8, IDC_CMB_YELLOW9, IDC_CMB_YELLOW10,IDC_CMB_YELLOW11};
	int  nID_Green[eSIGNAL_MAX]  = {IDC_CMB_GREEN1, IDC_CMB_GREEN2, IDC_CMB_GREEN3, IDC_CMB_GREEN4, IDC_CMB_GREEN5, IDC_CMB_GREEN6
	                               ,IDC_CMB_GREEN7, IDC_CMB_GREEN8, IDC_CMB_GREEN9, IDC_CMB_GREEN10,IDC_CMB_GREEN11};
	int  nID_Buzzer[eSIGNAL_MAX] = {IDC_CMB_BUZZER1, IDC_CMB_BUZZER2, IDC_CMB_BUZZER3, IDC_CMB_BUZZER4, IDC_CMB_BUZZER5, IDC_CMB_BUZZER6
	                               ,IDC_CMB_BUZZER7, IDC_CMB_BUZZER8, IDC_CMB_BUZZER9, IDC_CMB_BUZZER10,IDC_CMB_BUZZER11};
	int  nID_Priority[eSIGNAL_MAX] = {IDC_COMBOBOX_PRIORITY1,IDC_COMBOBOX_PRIORITY2,IDC_COMBOBOX_PRIORITY3,IDC_COMBOBOX_PRIORITY4
		                             ,IDC_COMBOBOX_PRIORITY5,IDC_COMBOBOX_PRIORITY6,IDC_COMBOBOX_PRIORITY7,IDC_COMBOBOX_PRIORITY8
									 ,IDC_COMBOBOX_PRIORITY9,IDC_COMBOBOX_PRIORITY10,IDC_COMBOBOX_PRIORITY11};

	for(int i=0; i<eSIGNAL_MAX; i++){
		pMdcCombo = (CComboBox*)GetDlgItem(nID_Priority[i]);
		for(int nNum=0; nNum<eSIGNAL_MAX; nNum++){
			CString strNum = _T("");
			strNum.Format("%d", nNum+1);
			pMdcCombo->AddString(strNum);
		}
		pMdcCombo->SetCurSel(m_stTowerSetting.nTower[i][eSIGNAL_PRIORTY]);

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Red[i]);
		pMdcCombo->AddString("OFF");
		pMdcCombo->AddString("Light On");
		pMdcCombo->AddString("Blink");
		pMdcCombo->SetCurSel(m_stTowerSetting.nTower[i][eSIGNAL_RED]);

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Yellow[i]);
		pMdcCombo->AddString("OFF");
		pMdcCombo->AddString("Light On");
		pMdcCombo->AddString("Blink");
		pMdcCombo->SetCurSel(m_stTowerSetting.nTower[i][eSIGNAL_YELLOW]);

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Green[i]);
		pMdcCombo->AddString("OFF");
		pMdcCombo->AddString("Light On");
		pMdcCombo->AddString("Blink");
		pMdcCombo->SetCurSel(m_stTowerSetting.nTower[i][eSIGNAL_GREEN]);

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Buzzer[i]);
		pMdcCombo->AddString("OFF");
		pMdcCombo->AddString("Buzzer 1");
		pMdcCombo->AddString("Buzzer 2");
		pMdcCombo->AddString("Buzzer 3");
		pMdcCombo->AddString("Buzzer 4");
		pMdcCombo->SetCurSel(m_stTowerSetting.nTower[i][eSIGNAL_SOUND]);
	}

	pMdcCombo = NULL;
}

void CDlgSetupSignalTower::OnBnClickedBtnTowerSave()
{
	CComboBox*	pMdcCombo = NULL;

	int  nID_Red[eSIGNAL_MAX]    = {IDC_CMB_RED1, IDC_CMB_RED2, IDC_CMB_RED3, IDC_CMB_RED4, IDC_CMB_RED5, IDC_CMB_RED6
		,IDC_CMB_RED7, IDC_CMB_RED8, IDC_CMB_RED9, IDC_CMB_RED10, IDC_CMB_RED11};
	int  nID_Yellow[eSIGNAL_MAX] = {IDC_CMB_YELLOW1, IDC_CMB_YELLOW2, IDC_CMB_YELLOW3, IDC_CMB_YELLOW4, IDC_CMB_YELLOW5, IDC_CMB_YELLOW6
		,IDC_CMB_YELLOW7, IDC_CMB_YELLOW8, IDC_CMB_YELLOW9, IDC_CMB_YELLOW10,IDC_CMB_YELLOW11};
	int  nID_Green[eSIGNAL_MAX]  = {IDC_CMB_GREEN1, IDC_CMB_GREEN2, IDC_CMB_GREEN3, IDC_CMB_GREEN4, IDC_CMB_GREEN5, IDC_CMB_GREEN6
		,IDC_CMB_GREEN7, IDC_CMB_GREEN8, IDC_CMB_GREEN9, IDC_CMB_GREEN10,IDC_CMB_GREEN11};
	int  nID_Buzzer[eSIGNAL_MAX] = {IDC_CMB_BUZZER1, IDC_CMB_BUZZER2, IDC_CMB_BUZZER3, IDC_CMB_BUZZER4, IDC_CMB_BUZZER5, IDC_CMB_BUZZER6
		,IDC_CMB_BUZZER7, IDC_CMB_BUZZER8, IDC_CMB_BUZZER9, IDC_CMB_BUZZER10,IDC_CMB_BUZZER11};
	int  nID_Priority[eSIGNAL_MAX] = {IDC_COMBOBOX_PRIORITY1,IDC_COMBOBOX_PRIORITY2,IDC_COMBOBOX_PRIORITY3,IDC_COMBOBOX_PRIORITY4
		,IDC_COMBOBOX_PRIORITY5,IDC_COMBOBOX_PRIORITY6,IDC_COMBOBOX_PRIORITY7,IDC_COMBOBOX_PRIORITY8
		,IDC_COMBOBOX_PRIORITY9,IDC_COMBOBOX_PRIORITY10,IDC_COMBOBOX_PRIORITY11};
	int nAddressSM[eSIGNAL_MAX] = {NDM0_Tower_Running_P, NDM0_Tower_JamCall_P, NDM0_Tower_Cycle_P, NDM0_Tower_Finish_P 
		,NDM0_Tower_Reset_P, NDM0_Tower_Pause_P, NDM0_Tower_Request_P, NDM0_Tower_TempWait_P 
		,NDM0_Tower_TestRun_P, NDM0_Tower_Start_P, NDM0_Tower_TempStabil_P};

	for(int i=0; i<eSIGNAL_MAX; i++){
		pMdcCombo = (CComboBox*)GetDlgItem(nID_Priority[i]);
		m_stTowerSetting.nTower[i][eSIGNAL_PRIORTY]  = pMdcCombo->GetCurSel();

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Red[i]);
		m_stTowerSetting.nTower[i][eSIGNAL_RED] = pMdcCombo->GetCurSel();

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Yellow[i]);
		m_stTowerSetting.nTower[i][eSIGNAL_YELLOW] = pMdcCombo->GetCurSel();

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Green[i]);
		m_stTowerSetting.nTower[i][eSIGNAL_GREEN] = pMdcCombo->GetCurSel();

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Buzzer[i]);
		m_stTowerSetting.nTower[i][eSIGNAL_SOUND] = pMdcCombo->GetCurSel();

		g_DMCont.m_dmEQP.SN(nAddressSM[i]               , m_stTowerSetting.nTower[i][eSIGNAL_PRIORTY]);
		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_RED   , m_stTowerSetting.nTower[i][eSIGNAL_RED]);
		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_YELLOW, m_stTowerSetting.nTower[i][eSIGNAL_YELLOW]);
		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_GREEN , m_stTowerSetting.nTower[i][eSIGNAL_GREEN]);
		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_SOUND , m_stTowerSetting.nTower[i][eSIGNAL_SOUND]);
	}

	pMdcCombo = NULL;

	g_CokMgr.SaveSignalTower(&m_stTowerSetting);
	g_TaskSystemCtrl.SetSignalTowerDataFromSM();
}


void CDlgSetupSignalTower::OnBnClickedBtnTowerDefault()
{
	CComboBox*	pMdcCombo = NULL;

	int  nID_Red[eSIGNAL_MAX]    = {IDC_CMB_RED1, IDC_CMB_RED2, IDC_CMB_RED3, IDC_CMB_RED4, IDC_CMB_RED5, IDC_CMB_RED6
		,IDC_CMB_RED7, IDC_CMB_RED8, IDC_CMB_RED9, IDC_CMB_RED10, IDC_CMB_RED11};
	int  nID_Yellow[eSIGNAL_MAX] = {IDC_CMB_YELLOW1, IDC_CMB_YELLOW2, IDC_CMB_YELLOW3, IDC_CMB_YELLOW4, IDC_CMB_YELLOW5, IDC_CMB_YELLOW6
		,IDC_CMB_YELLOW7, IDC_CMB_YELLOW8, IDC_CMB_YELLOW9, IDC_CMB_YELLOW10,IDC_CMB_YELLOW11};
	int  nID_Green[eSIGNAL_MAX]  = {IDC_CMB_GREEN1, IDC_CMB_GREEN2, IDC_CMB_GREEN3, IDC_CMB_GREEN4, IDC_CMB_GREEN5, IDC_CMB_GREEN6
		,IDC_CMB_GREEN7, IDC_CMB_GREEN8, IDC_CMB_GREEN9, IDC_CMB_GREEN10,IDC_CMB_GREEN11};
	int  nID_Buzzer[eSIGNAL_MAX] = {IDC_CMB_BUZZER1, IDC_CMB_BUZZER2, IDC_CMB_BUZZER3, IDC_CMB_BUZZER4, IDC_CMB_BUZZER5, IDC_CMB_BUZZER6
		,IDC_CMB_BUZZER7, IDC_CMB_BUZZER8, IDC_CMB_BUZZER9, IDC_CMB_BUZZER10,IDC_CMB_BUZZER11};
	int  nID_Priority[eSIGNAL_MAX] = {IDC_COMBOBOX_PRIORITY1,IDC_COMBOBOX_PRIORITY2,IDC_COMBOBOX_PRIORITY3,IDC_COMBOBOX_PRIORITY4
		,IDC_COMBOBOX_PRIORITY5,IDC_COMBOBOX_PRIORITY6,IDC_COMBOBOX_PRIORITY7,IDC_COMBOBOX_PRIORITY8
		,IDC_COMBOBOX_PRIORITY9,IDC_COMBOBOX_PRIORITY10,IDC_COMBOBOX_PRIORITY11};
	int nAddressSM[eSIGNAL_MAX] = {NDM0_Tower_Running_P, NDM0_Tower_JamCall_P, NDM0_Tower_Cycle_P, NDM0_Tower_Finish_P 
		,NDM0_Tower_Reset_P, NDM0_Tower_Pause_P, NDM0_Tower_Request_P, NDM0_Tower_TempWait_P 
		,NDM0_Tower_TestRun_P, NDM0_Tower_Start_P, NDM0_Tower_TempStabil_P};

	for(int i=0; i<eSIGNAL_MAX; i++){
		pMdcCombo = (CComboBox*)GetDlgItem(nID_Priority[i]);
		pMdcCombo->SetCurSel(0);

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Red[i]);
		if( i == 1){ pMdcCombo->SetCurSel(1); } // Jam/ Call Light on
		else if(i == 3){ pMdcCombo->SetCurSel(2); }// Finish Blink
		else{ pMdcCombo->SetCurSel(0);	}

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Yellow[i]);
		if(i == 3) { pMdcCombo->SetCurSel(2); } // Finish Blink
		else if(i == 5){ pMdcCombo->SetCurSel(1); } // Pause Light On
		else{ pMdcCombo->SetCurSel(0); }

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Green[i]);
		if(i == 0){pMdcCombo->SetCurSel(1);} // Running Light On
		else if(i == 3){pMdcCombo->SetCurSel(2);} // Finish Blink
		else{ pMdcCombo->SetCurSel(0);}

		pMdcCombo = (CComboBox*)GetDlgItem(nID_Buzzer[i]);
		if(i ==1){pMdcCombo->SetCurSel(3);} // Jam/Call Buzzer 3
		else if(i == 3){pMdcCombo->SetCurSel(4);} // Finish Buzzer 4
		else{pMdcCombo->SetCurSel(0);}
	}
}
