// DlgVibrator.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgVibrator.h"
#include "afxdialogex.h"


// CDlgVibrator 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgVibrator, CDialogEx)

CDlgVibrator::CDlgVibrator(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIBRATOR, pParent)
{
	m_vCmdSite.clear();
	m_nExcuteCnt = 0;
	m_dDelayTime = 0.0;
	m_nStationNum = 0;
	m_nLoadingTableNum = 0;
	m_nCurCnt = 0;
	m_nStep = m_nOldStep = standby;  //m_nOldStep은 Log남기기 위해 선언.

	m_pThread = nullptr;
	m_pThread = AfxBeginThread(useThread, this);

	vFWDResoruceID.clear();
	vBWDResoruceID.clear();

	vFWDResoruceID.push_back(IDC_CTRL_ARV_TBL1_FWD);
	vFWDResoruceID.push_back(IDC_CTRL_ARV_TBL2_FWD);
	vFWDResoruceID.push_back(IDC_CTRL_ARV_TBL3_FWD);
	vFWDResoruceID.push_back(IDC_CTRL_ARV_TBL4_FWD);

	vBWDResoruceID.push_back(IDC_CTRL_ARV_TBL1_BWD);
	vBWDResoruceID.push_back(IDC_CTRL_ARV_TBL2_BWD);
	vBWDResoruceID.push_back(IDC_CTRL_ARV_TBL3_BWD);
	vBWDResoruceID.push_back(IDC_CTRL_ARV_TBL4_BWD);

}

CDlgVibrator::~CDlgVibrator()
{
	m_fontStation.DeleteObject();
	m_fontCnt.DeleteObject();
	m_fontDelay.DeleteObject();
	m_fontCaption.DeleteObject();

	int nCount = 0;
	if (m_pThread != NULL) 
	{
		while (::WaitForSingleObject(m_pThread->m_hThread, 1) == WAIT_TIMEOUT) 
		{
			nCount++;
			if (nCount > 500) 
			{
				::TerminateThread(m_pThread->m_hThread, 0);
				break;
			}
		}
	}
	m_pThread = NULL;
}

void CDlgVibrator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STATION_NUM, m_ComboStaionNum);
	DDX_Control(pDX, IDOK, m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);

	DDX_Control(pDX, IDC_EDIT_CNT, m_EditCnt);
	DDX_Control(pDX, IDC_EDIT_DELAY, m_EditDelayTime);

	DDX_Text(pDX, IDC_EDIT_CNT, m_nVibCnt);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_dVibDelay);

}


BEGIN_MESSAGE_MAP(CDlgVibrator, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgVibrator::OnBnClickedVibratorStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgVibrator::OnBnClickedVibratorStop)
	ON_BN_CLICKED(IDOK, &CDlgVibrator::OnBnClickedOk)
	ON_WM_TIMER()
	ON_EN_SETFOCUS(IDC_EDIT_CNT, &CDlgVibrator::OnEnSetfocusEditCnt)
	ON_EN_SETFOCUS(IDC_EDIT_DELAY, &CDlgVibrator::OnEnSetfocusEditDelay)
END_MESSAGE_MAP()

// CDlgVibrator 메시지 처리기입니다.

UINT CDlgVibrator::useThread(LPVOID lParam)
{
	CDlgVibrator* pSelf = (CDlgVibrator*)lParam;

	while (TRUE)
	{
		pSelf->VibrateActionSequence();
		Sleep(1);
	}
	return 0;
}

bool CDlgVibrator::GetBusy()
{
	return (m_nStep != standby);
}

void CDlgVibrator::next(VibratorStep nStep)
{
	m_nStep = nStep;
}

void CDlgVibrator::VibrateActionSequence()
{
	switch (m_nStep)
	{
	case standby: //대기(정지) Loop
	{
		break;
	}
	case start:
	{
		next(do_Vibrator_On);
		break;
	}
	case do_Vibrator_On:
	{
		if (m_nUnit == ePressVibratorUnit)
		{
			for (int i = 0; i < (int)m_vCmdSite.size(); i++)
			{
				g_TaskPressUnit[m_vCmdSite[i]].do_SideVibratorOnOff(DEF_ON);
			}
		}
		else if (m_nUnit == eLoadTableVibratorUnit)
		{
			for (int i = 0; i < (int)m_vCmdSite.size(); i++)
			{
				g_TaskLoadTbl[m_vCmdSite[i]].do_SideVibratorOnOff(DEF_ON);
			}
		}
		tmr.StartTmChk();
		next(do_Vibrator_On_Wait);
		break;
	}
	case do_Vibrator_On_Wait:
	{
		tmr.EndTmChk();
		if (tmr.GetTmDistanceMS() < m_dDelayTime*1000.0)
			break;

		next(do_Vibrator_Off);
		break;
	}
	case do_Vibrator_Off:
	{
		if (m_nUnit == ePressVibratorUnit)
		{
			for (int i = 0; i < (int)m_vCmdSite.size(); i++)
			{
				g_TaskPressUnit[m_vCmdSite[i]].do_SideVibratorOnOff(DEF_OFF);
			}
		}
		else if (m_nUnit == eLoadTableVibratorUnit)
		{
			for (int i = 0; i < (int)m_vCmdSite.size(); i++)
			{
				g_TaskLoadTbl[m_vCmdSite[i]].do_SideVibratorOnOff(DEF_OFF);
			}
		}

		tmr.StartTmChk();
		next(do_Vibrator_Off_Wait);
		break;
	}
	case do_Vibrator_Off_Wait:
	{
		tmr.EndTmChk();
		if (tmr.GetTmDistanceMS() < m_dDelayTime*1000.0)
			break;

		next(complete);
		break;
	}
	case complete:
	{
		if (++m_nCurCnt >= m_nExcuteCnt)
		{
			m_vCmdSite.clear();
			m_nCurCnt = 0;
			next(standby);
		}
		else
		{
			next(start);
		}
		break;
	}
	}

	
}

void CDlgVibrator::OnBnClickedVibratorStart()
{
	if (GetBusy())			//작업 수행 중일시 리턴.
		return ;

	setValueFromDlg();
	setStep(start); // Start(시작)
}

void CDlgVibrator::OnBnClickedVibratorStop()
{
	setStep(standby); // End(중단)
	clearValue();
}

void CDlgVibrator::setValueFromDlg() 
{
	CString strDelayTime = _T("");
	m_nExcuteCnt = GetDlgItemInt(IDC_EDIT_CNT);  //실행 횟수
	GetDlgItemText(IDC_EDIT_DELAY, strDelayTime);
	m_dDelayTime = atof(strDelayTime);					// Delay Time

	if (m_nUnit == ePressVibratorUnit)
	{
		m_nStationNum = m_ComboStaionNum.GetCurSel(); // Station 번호  Station 1번 이면 m_nStaionNum은 0번.	
		if (m_nStationNum == MaxStation)
		{
			for (int i = 0; i < MaxStation; i++)
			{
				m_vCmdSite.push_back(i);
			}
		}
		else
		{
			m_vCmdSite.push_back(m_nStationNum);
		}
	}
	else if(m_nUnit == eLoadTableVibratorUnit)
	{
		m_nLoadingTableNum = m_ComboStaionNum.GetCurSel();

		if (m_nLoadingTableNum == MaxLoadingTable)
		{
			for (int i = 0; i < MaxLoadingTable; i++)
			{
				m_vCmdSite.push_back(i);
			}
		}
		else
		{
			m_vCmdSite.push_back(m_nLoadingTableNum);
		}
	}


}

void CDlgVibrator::clearValue() 
{
	m_vCmdSite.clear();
	m_nCurCnt = 0;
}

BOOL CDlgVibrator::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ComboStaionNum.Clear();

	m_CmbFont.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_ComboStaionNum.SetFont(&m_CmbFont);

	if (m_nUnit == ePressVibratorUnit)
	{
		m_ComboStaionNum.AddString("#1");
		m_ComboStaionNum.AddString("#2");
		m_ComboStaionNum.AddString("#3");
		m_ComboStaionNum.AddString("#4");
		m_ComboStaionNum.AddString("#5");
		m_ComboStaionNum.AddString("#6");
		m_ComboStaionNum.AddString("#7");
		m_ComboStaionNum.AddString("#8");
		m_ComboStaionNum.AddString("#All Station");
		m_ComboStaionNum.SetCurSel(0);
		GetDlgItem(IDC_STATIC_STATION_NUM)->SetWindowText("- Station");

		GetDlgItem(IDC_STATIC_TEXT68)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT124)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT125)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT126)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT127)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BWD)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_CTRL_ARV_TBL1_FWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CTRL_ARV_TBL2_FWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CTRL_ARV_TBL3_FWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CTRL_ARV_TBL4_FWD)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CTRL_ARV_TBL1_BWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CTRL_ARV_TBL2_BWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CTRL_ARV_TBL3_BWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CTRL_ARV_TBL4_BWD)->ShowWindow(SW_HIDE);		
	}
	else
	{
		m_ComboStaionNum.AddString("#1");
		m_ComboStaionNum.AddString("#2");
		m_ComboStaionNum.AddString("#3");
		m_ComboStaionNum.AddString("#4");
		m_ComboStaionNum.AddString("#All LoadingTable");
		m_ComboStaionNum.SetCurSel(0);
		GetDlgItem(IDC_STATIC_STATION_NUM)->SetWindowText("- LoadTbl");
	}
	

	CEdit *pEditCnt = (CEdit*)GetDlgItem(IDC_EDIT_CNT);
	pEditCnt->SetWindowText(_T("5"));
	m_nVibCnt = 5;
	CEdit *pEditDelay = (CEdit*)GetDlgItem(IDC_EDIT_DELAY);
	pEditDelay->SetWindowText(_T("0.1"));
	m_dVibDelay = 0.1;

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT);
	m_btnExit.SetBitmap(m_cBmpExit);

	m_cBmpStart.LoadBitmap(IDB_START);
	m_btnStart.SetBitmap(m_cBmpStart);

	m_cBmpStop.LoadBitmap(IDB_STOP);
	m_btnStop.SetBitmap(m_cBmpStop);

	setFont();

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgVibrator::setFont() 
{

	m_EditCnt.SetBackColor(COLOR_WWHITE);
	m_EditDelayTime.SetBackColor(COLOR_WWHITE);

	m_EditCnt.SetTextSize(30);
	m_EditDelayTime.SetTextSize(30);

	LOGFONT        LogFontStation;
	LOGFONT        LogFontCnt;
	LOGFONT        LogFontDelay;
	LOGFONT        LogFontCaption;
	// Edit 컨트롤의 Font 정보를 LogFont에 가져옵니다.
	GetDlgItem(IDC_STATIC_STATION_NUM)->GetFont()->GetLogFont(&LogFontStation);
	GetDlgItem(IDC_STATIC_CNT)->GetFont()->GetLogFont(&LogFontCnt);
	GetDlgItem(IDC_STATIC_DELAY)->GetFont()->GetLogFont(&LogFontDelay);
	GetDlgItem(IDC_STATIC_SETTING)->GetFont()->GetLogFont(&LogFontCaption);
	

	// Font 글자 설정
	LogFontCaption.lfWeight = LogFontStation.lfWeight = LogFontCnt.lfWeight = LogFontDelay.lfWeight = 1000;      // 굵기설정( MAX : 1000 )
	LogFontCaption.lfHeight = LogFontStation.lfHeight = LogFontCnt.lfHeight = LogFontDelay.lfHeight = 20;      // Font Size 설정

								// LogFont의정보로설정
	m_fontStation.CreateFontIndirect(&LogFontStation);
	m_fontCnt.CreateFontIndirect(&LogFontCnt);
	m_fontDelay.CreateFontIndirect(&LogFontDelay);
	m_fontCaption.CreateFontIndirect(&LogFontCaption);
	// EDit 박스의 폰트를 설정합니다. 
	GetDlgItem(IDC_STATIC_STATION_NUM)->SetFont(&m_fontStation);
	GetDlgItem(IDC_STATIC_CNT)->SetFont(&m_fontCnt);
	GetDlgItem(IDC_STATIC_DELAY)->SetFont(&m_fontDelay);
	GetDlgItem(IDC_STATIC_SETTING)->SetFont(&m_fontCaption);
}


void CDlgVibrator::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlgVibrator::OnTimer(UINT_PTR nIDEvent)
{
	CDSSPanel*   chkbt = NULL;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0)
	{
		KillTimer(0);

		//Load Table Vibrator
		for (int nTblIdx = 0; nTblIdx < eMaxTblCount; nTblIdx++)
		{
			//FWD
			chkbt = (CDSSPanel*)GetDlgItem(vFWDResoruceID[nTblIdx]);
			if (g_TaskLoadTbl[nTblIdx].GetDI(CTaskLoadTable::xLOAD_TABLE_VIB_FOR_SEN, DEF_ON) == ERR_NO_ERROR)
			 {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}

			//BWD
			chkbt = (CDSSPanel*)GetDlgItem(vBWDResoruceID[nTblIdx]);
			if (g_TaskLoadTbl[nTblIdx].GetDI(CTaskLoadTable::xLOAD_TABLE_VIB_BACK_SEN, DEF_ON) == ERR_NO_ERROR)
			{
				chkbt->On();
			}
			else {
				chkbt->Off();
			}
		}

		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgVibrator::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KillTimer(0);

	return CDialogEx::DestroyWindow();
}


void CDlgVibrator::OnEnSetfocusEditCnt()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	_itoa_s(m_nVibCnt, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Count", "50", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_CNT)->SetWindowText(szRetVal);
		m_nVibCnt = atoi(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CDlgVibrator::OnEnSetfocusEditDelay()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dVibDelay);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Delay Time", "1", "0.05");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DELAY)->SetWindowText(szRetVal);
		m_dVibDelay = atof(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}
