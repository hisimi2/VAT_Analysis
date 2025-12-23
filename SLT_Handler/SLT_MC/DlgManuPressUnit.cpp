// DlgManuContactor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuPressUnit.h"
#include "afxdialogex.h"

#include "../Include/OcxFiles/dsspanel.h"
#include "DlgVibrator.h"
// CDlgManuPressUnit 대화 상자입니다.

#define LED_OFF			_T("0")
#define LED_ON			_T("1")

#define SpreadSite_UP	(1)
#define SpreadSite_DOWN	(0)
#define SpreadColumnLength (32)

IMPLEMENT_DYNAMIC(CDlgManuPressUnit, CDialogEx)

CDlgManuPressUnit::CDlgManuPressUnit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuPressUnit::IDD, pParent)
{
	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;
	m_nCmdCycleCount = 0;
	m_dTimeCheck = 0.0;
	m_vCmdSite.clear();

	::ZeroMemory( m_nSenID_SafetyZ, sizeof( m_nSenID_SafetyZ ) );
	::ZeroMemory( m_nSenID_Up, sizeof( m_nSenID_Up ) );
	::ZeroMemory( m_nSenID_Down, sizeof( m_nSenID_Down ) );
	m_dCylActionTimeRecord = 0.0;
	m_nCmdManualCyl = 0;
	m_nCmdOperation = 0;
	m_nCmdParam_Time = 0;
	m_nCylinderNum = 0;

	memset(&m_dCylinderSpec, 0x00, sizeof(m_dCylinderSpec));

	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		m_dCylinderSpec[ePressUnit_Down_1 + i][ePressUp].dMax = 0.25;
		m_dCylinderSpec[ePressUnit_Down_1 + i][ePressUp].dMin = 0.20;

		m_dCylinderSpec[ePressUnit_Down_1 + i][ePressDown].dMax = 0.35;
		m_dCylinderSpec[ePressUnit_Down_1 + i][ePressDown].dMin = 0.30;
	}
		
}

CDlgManuPressUnit::~CDlgManuPressUnit()
{
	m_cBmpExit.DeleteObject();
}

void CDlgManuPressUnit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_CONTINUE_TIME, m_cbTimeSel);
	DDX_Control(pDX, IDC_COMBO_SITE_SEL, m_cbSiteSel);
	DDX_Control(pDX, IDC_BUTTON_ALL_UP, m_BtnUpAll);
	DDX_Control(pDX, IDC_BUTTON_ALL_DOWN, m_BtnAllDown);
	DDX_Control(pDX, IDC_BUTTON_CTYL_ACT_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BUTTON_EACH_UP, m_BtnUpEach);
	DDX_Control(pDX, IDC_BUTTON_EACH_DOWN, m_BtnDownEach);
	DDX_Control(pDX, IDC_SPREAD_TESTER_SELECT4, m_SpreadSiteDown);
	DDX_Control(pDX, IDC_BUTTON_VIBRATOR, m_BtnVibrator);
}


BEGIN_MESSAGE_MAP(CDlgManuPressUnit, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgManuPressUnit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS1, &CDlgManuPressUnit::OnBnClickedButtonBlowPress1)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS2, &CDlgManuPressUnit::OnBnClickedButtonBlowPress2)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS3, &CDlgManuPressUnit::OnBnClickedButtonBlowPress3)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS4, &CDlgManuPressUnit::OnBnClickedButtonBlowPress4)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS5, &CDlgManuPressUnit::OnBnClickedButtonBlowPress5)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS6, &CDlgManuPressUnit::OnBnClickedButtonBlowPress6)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS7, &CDlgManuPressUnit::OnBnClickedButtonBlowPress7)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS8, &CDlgManuPressUnit::OnBnClickedButtonBlowPress8)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS9, &CDlgManuPressUnit::OnBnClickedButtonBlowPress9)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS10, &CDlgManuPressUnit::OnBnClickedButtonBlowPress10)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS11, &CDlgManuPressUnit::OnBnClickedButtonBlowPress11)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS12, &CDlgManuPressUnit::OnBnClickedButtonBlowPress12)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS13, &CDlgManuPressUnit::OnBnClickedButtonBlowPress13)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS14, &CDlgManuPressUnit::OnBnClickedButtonBlowPress14)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS15, &CDlgManuPressUnit::OnBnClickedButtonBlowPress15)
	ON_BN_CLICKED(IDC_BUTTON_BLOW_PRESS16, &CDlgManuPressUnit::OnBnClickedButtonBlowPress16)
	ON_BN_CLICKED(IDC_BUTTON_EACH_UP, &CDlgManuPressUnit::OnBnClickedButtonEachUp)
	ON_BN_CLICKED(IDC_BUTTON_EACH_DOWN, &CDlgManuPressUnit::OnBnClickedButtonEachDown)
	ON_BN_CLICKED(IDC_BUTTON_ALL_UP, &CDlgManuPressUnit::OnBnClickedButtonAllUp)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DOWN, &CDlgManuPressUnit::OnBnClickedButtonAllDown)
	ON_BN_CLICKED(IDC_BUTTON_CTYL_ACT_STOP, &CDlgManuPressUnit::OnBnClickedButtonCtylActStop)
	ON_BN_CLICKED(IDC_BUTTON_VIBRATOR, &CDlgManuPressUnit::OnBnClickedButtonVibrator)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgManuPressUnit 메시지 처리기입니다.

BOOL CDlgManuPressUnit::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_SYSKEYDOWN) && (pMsg->wParam==VK_F4) )
	{
		return TRUE;
	}
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgManuPressUnit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_CYLINDER_PRESS);
	m_btnExit.SetBitmap(m_cBmpExit);

	m_nSenID_SafetyZ[0]  = IDC_DN_SAFE_SEN1;
	m_nSenID_SafetyZ[1]  = IDC_DN_SAFE_SEN2;
	m_nSenID_SafetyZ[2]  = IDC_DN_SAFE_SEN3;
	m_nSenID_SafetyZ[3]  = IDC_DN_SAFE_SEN4;
	m_nSenID_SafetyZ[4]  = IDC_DN_SAFE_SEN5;
	m_nSenID_SafetyZ[5]  = IDC_DN_SAFE_SEN6;
	m_nSenID_SafetyZ[6]  = IDC_DN_SAFE_SEN7;
	m_nSenID_SafetyZ[7]  = IDC_DN_SAFE_SEN8;


	
// 	m_nSenID_Slide[0]  = IDC_SLIDE_DN1;
// 	m_nSenID_Slide[1]  = IDC_SLIDE_DN2;
// 	m_nSenID_Slide[2]  = IDC_SLIDE_DN3;
// 	m_nSenID_Slide[3]  = IDC_SLIDE_DN4;
// 	m_nSenID_Slide[4]  = IDC_SLIDE_DN5;
// 	m_nSenID_Slide[5]  = IDC_SLIDE_DN6;
// 	m_nSenID_Slide[6]  = IDC_SLIDE_DN7;
// 	m_nSenID_Slide[7]  = IDC_SLIDE_DN8;
// 	m_nSenID_Slide[8]  = IDC_SLIDE_DN9;
// 	m_nSenID_Slide[9]  = IDC_SLIDE_DN10;
// 	m_nSenID_Slide[10] = IDC_SLIDE_DN11;
// 	m_nSenID_Slide[11] = IDC_SLIDE_DN12;
// 	m_nSenID_Slide[12] = IDC_SLIDE_DN13;
// 	m_nSenID_Slide[13] = IDC_SLIDE_DN14;
// 	m_nSenID_Slide[14] = IDC_SLIDE_DN15;
// 	m_nSenID_Slide[15] = IDC_SLIDE_DN16;

	//if(g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_ON){
		m_nSenID_Up[0] = IDC_COLORCHECKBTCTRL_UP_SITE1;
		m_nSenID_Up[1] = IDC_COLORCHECKBTCTRL_UP_SITE2;
		m_nSenID_Up[2] = IDC_COLORCHECKBTCTRL_UP_SITE3;
		m_nSenID_Up[3] = IDC_COLORCHECKBTCTRL_UP_SITE4;
		m_nSenID_Up[4] = IDC_COLORCHECKBTCTRL_UP_SITE5;
		m_nSenID_Up[5] = IDC_COLORCHECKBTCTRL_UP_SITE6;
		m_nSenID_Up[6] = IDC_COLORCHECKBTCTRL_UP_SITE7;
		m_nSenID_Up[7] = IDC_COLORCHECKBTCTRL_UP_SITE8;

		m_nSenID_Down[0] = IDC_COLORCHECKBTCTRL_DOWN_SITE1;
		m_nSenID_Down[1] = IDC_COLORCHECKBTCTRL_DOWN_SITE2;
		m_nSenID_Down[2] = IDC_COLORCHECKBTCTRL_DOWN_SITE3;
		m_nSenID_Down[3] = IDC_COLORCHECKBTCTRL_DOWN_SITE4;
		m_nSenID_Down[4] = IDC_COLORCHECKBTCTRL_DOWN_SITE5;
		m_nSenID_Down[5] = IDC_COLORCHECKBTCTRL_DOWN_SITE6;
		m_nSenID_Down[6] = IDC_COLORCHECKBTCTRL_DOWN_SITE7;
		m_nSenID_Down[7] = IDC_COLORCHECKBTCTRL_DOWN_SITE8;


		m_cbOperationSel.Clear();
		m_cbOperationSel.AddString("1: Only One Action & No Check Sensor");
		m_cbOperationSel.AddString("2: Only One Action & Check Sensor");
		m_cbOperationSel.AddString("3: Continue Action & No Check Sensor");
		m_cbOperationSel.AddString("4: Continue Action & Check Sensor");
		m_cbOperationSel.AddString("5: Time Check Action (Once & Sensor Check)");
		m_cbOperationSel.SetCurSel(0);
		m_cbTimeSel.SetCurSel(0);
		
		m_cbSiteSel.Clear();
		m_cbSiteSel.AddString("1: Site#1");
		m_cbSiteSel.AddString("2: Site#2");
		m_cbSiteSel.AddString("3: Site#3");
		m_cbSiteSel.AddString("4: Site#4");
		m_cbSiteSel.AddString("5: Site#5");
		m_cbSiteSel.AddString("6: Site#6");
		m_cbSiteSel.AddString("7: Site#7");
		m_cbSiteSel.AddString("8: Site#8");

		m_cbSiteSel.SetCurSel(0);
		
		CreateThread(0);
		SetBtnDisplay();
	/*}else{
		ChangeDialog();
	}*/
	InitSpreadSheet();

	char szCompany[16] = { 0, };
	CString strCustomer = _T("");
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	char szPathFileName[MAX_PATH] = { 0, };
	char szPathTemp[512] = { 0, };
	char szFilePath[MAX_PATH] = { 0, };

	::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_FILE_PATH);
	CConfigData ConfigPathData(szPathFileName);

	// Vibrator Option
	char szCompanyVibratorMode[64] = { 0, };
	CString strTemp = _T("");
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigVibMode(szPathFileName);
	ConfigVibMode.GetString("Mode", "Vibrator Mode", DEF_NOT_USE, szCompanyVibratorMode);
	//
// 	if (strcmp(szCompany, DEF_COMPANY_USI) == 0 && strcmp(szCompanyVibratorMode, DEF_USE) == 0) //USI 이고 Vibrator Mode 사용 하면 버튼 활성화
// 	{
		GetDlgItem(IDC_BUTTON_VIBRATOR)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_VIBRATOR)->ShowWindow(TRUE);
		
// 	}
// 	else 
// 	{
// 		GetDlgItem(IDC_BUTTON_VIBRATOR)->ShowWindow(FALSE);
// 		GetDlgItem(IDC_STATIC_VIBRATOR)->ShowWindow(FALSE);
// 		
// 	}

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuPressUnit::OnDestroy()
{
	KillTimer(0);
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuPressUnit::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuPressUnit::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		CDSSPanel*   pSensor = NULL;

		for(int i=0; i<eMaxPressUnitCount; i++) {
			pSensor = (CDSSPanel*)GetDlgItem(m_nSenID_SafetyZ[i]);
			if(g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_SAFETY_SEN) == DEF_ON)
			{
				pSensor->On();
			}else{
				pSensor->Off();
			}

			int nPressArrive1 = 0;
			int nPressArrive2 = 0;
			int nPressArrive3 = 0;
			int nPressArrive4 = 0;
			int nPressArrive5 = 0;
			int nPressArrive6 = 0;
			int nPressArrive7 = 0;
			int nPressArrive8 = 0;

			int nSpreadIndex = i < 8 ? SpreadSite_DOWN : SpreadSite_UP; //0부터 Index가 시작함.

		
			int nPressSensorType = g_DMCont.m_dmEQP.GN(NDM0_PRESS_ARRIVED_SENSOR_TYPE);
			if (nPressSensorType == CTaskPressUnit::eArriveSensorType::eEach) {
				nPressArrive1 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_1);
				nPressArrive2 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_2);
				nPressArrive3 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_3);
				nPressArrive4 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_4);

				nPressArrive5 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_5);
				nPressArrive6 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_6);
				nPressArrive7 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_7);
				nPressArrive8 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_8);
			}
			else { // FiberSensor
				nPressArrive1 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1);
				nPressArrive2 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1);
				nPressArrive3 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1);
				nPressArrive4 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1);

				nPressArrive5 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_F1);
				nPressArrive6 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_F1);
				nPressArrive7 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_F1);
				nPressArrive8 = g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_F1);
			}




			if (nSpreadIndex == SpreadSite_DOWN)
			{
				m_SpreadSiteDown.SetTextMatrix(1, (i * 4) + 1, nPressArrive1 == DEF_ON ? LED_ON : LED_OFF);
				m_SpreadSiteDown.SetTextMatrix(1, (i * 4) + 2, nPressArrive2 == DEF_ON ? LED_ON : LED_OFF);
				m_SpreadSiteDown.SetTextMatrix(1, (i * 4) + 3, nPressArrive3 == DEF_ON ? LED_ON : LED_OFF);
				m_SpreadSiteDown.SetTextMatrix(1, (i * 4) + 4, nPressArrive4 == DEF_ON ? LED_ON : LED_OFF);				
				m_SpreadSiteDown.SetTextMatrix( 2, ( i * 4 ) + 1, nPressArrive5 == DEF_ON ? LED_ON : LED_OFF );
				m_SpreadSiteDown.SetTextMatrix( 2, ( i * 4 ) + 2, nPressArrive6 == DEF_ON ? LED_ON : LED_OFF );
				m_SpreadSiteDown.SetTextMatrix( 2, ( i * 4 ) + 3, nPressArrive7 == DEF_ON ? LED_ON : LED_OFF );
				m_SpreadSiteDown.SetTextMatrix( 2, ( i * 4 ) + 4, nPressArrive8 == DEF_ON ? LED_ON : LED_OFF );
				
			}
			

			//pSensor = (CDSSPanel*)GetDlgItem(m_nSenID_Slide[i]);
			//if(g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN) == DEF_ON){
			//	pSensor->On();
			//}else{
			//	pSensor->Off();
			//}

			if(g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_ON){
				pSensor = (CDSSPanel*)GetDlgItem(m_nSenID_Down[i]);
// 				if(g_TaskPressUnit[i].GetDI(CTaskPressUnit::xZAXIS_VIBRATION_CYL_DOWN_SEN) == DEF_ON){
// 					pSensor->On();
// 				}else{
// 					pSensor->Off();
// 				}
			}
		}	

		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuPressUnit::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress1()
{
	MakeLog("Press Unit #1 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_1].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_1].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress2()
{
	MakeLog("Press Unit #2 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_2].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_2].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress3()
{
	MakeLog("Press Unit #3 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_3].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_3].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress4()
{
	MakeLog("Press Unit #4 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_4].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_4].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress5()
{
	MakeLog("Press Unit #5 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_5].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_5].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress6()
{
	MakeLog("Press Unit #6 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_6].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_6].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress7()
{
	MakeLog("Press Unit #7 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_7].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_7].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress8()
{
	MakeLog("Press Unit #8 Blow Button Click");
	g_TaskPressUnit[ePressUnit_Down_8].do_VacBlow(eVacBlwSt_BlwOn);
	Sleep(1000);
	g_TaskPressUnit[ePressUnit_Down_8].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress9()
{
// 	MakeLog("Press Unit #9 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_1].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_1].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress10()
{
// 	MakeLog("Press Unit #10 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_2].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_2].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress11()
{
// 	MakeLog("Press Unit #11 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_3].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_3].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress12()
{
// 	MakeLog("Press Unit #12 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_4].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_4].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress13()
{
// 	MakeLog("Press Unit #13 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_5].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_5].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress14()
{
// 	MakeLog("Press Unit #14 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_6].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_6].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress15()
{
// 	MakeLog("Press Unit #15 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_7].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_7].do_VacBlow(eVacBlwSt_Off);
}


void CDlgManuPressUnit::OnBnClickedButtonBlowPress16()
{
// 	MakeLog("Press Unit #16 Blow Button Click");
// 	g_TaskPressUnit[ePressUnit_Up_8].do_VacBlow(eVacBlwSt_BlwOn);
// 	Sleep(1000);
// 	g_TaskPressUnit[ePressUnit_Up_8].do_VacBlow(eVacBlwSt_Off);
}

void CDlgManuPressUnit::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}


int CDlgManuPressUnit::do_CylCommand(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	return nErrCode;
}

int CDlgManuPressUnit::Check_CylSensor(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachUp:
	case eCylManualCmd_AllUp:
		{	
// 			int nCmd = (bUndo==FALSE)?CTaskPressUnit::xZAXIS_VIBRATION_CYL_UP_SEN:CTaskPressUnit::xZAXIS_VIBRATION_CYL_DOWN_SEN;	
// 			for(int i =0; i< (int)m_vCmdSite.size(); i++){
// 				nErrCode += g_TaskPressUnit[m_vCmdSite[i]].ChkDI(nCmd);
// 			}
		}break;
	case eCylManualCmd_EachDown:
	case eCylManualCmd_AllDown:
		{
// 			int nCmd = (bUndo==FALSE)?CTaskPressUnit::xZAXIS_VIBRATION_CYL_DOWN_SEN:CTaskPressUnit::xZAXIS_VIBRATION_CYL_UP_SEN;	
// 			for(int i =0; i< (int)m_vCmdSite.size(); i++){
// 				nErrCode += g_TaskPressUnit[m_vCmdSite[i]].ChkDI(nCmd);
// 			}
		}break;
	}
	return nErrCode;
}

void CDlgManuPressUnit::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) ){
		if( GetPauseFlag(0) ) {
			NEXT;
			continue;
		}
		CylinderManualAction();
		NEXT;
	}
}


void CDlgManuPressUnit::CylinderManualAction()
{
	switch( m_nCylManualOpActState )
	{
	case eCylManualOper_None:
		{
			CylManualOper_None();
		}break;
	case eCylManualOper_Only_One_Action_and_No_Check_Sensor :
		{
			CylManualOper_Only_One_Action_and_No_Check_Sensor();
		}break;
	case eCylManualOper_Only_One_Action_and_Check_Sensor:
		{
			CylManualOper_Only_One_Action_and_Check_Sensor();
		}break;
	case eCylManualOper_Continue_Action_and_No_Check_Sensor:
		{
			CylManualOper_Continue_Action_and_No_Check_Sensor();
		}break;
	case eCylManualOper_Continue_Action_and_Check_Sensor:
		{
			CylManualOper_Continue_Action_and_Check_Sensor();
		}break;
	case eCylManualOper_Time_Check_Action_Once_and_Sensor_Check:
		{
			CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
		}break;
	}
}

void CDlgManuPressUnit::CylManualOper_None()
{
	switch( m_nCylManualActionStep )
	{
	case 0: // 아무것도 하지 않고 대기 한다.
		{
			ClearCommand();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
		}break;
	}
}

//==============================================================================
// cylinder 한번만 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuPressUnit::CylManualOper_Only_One_Action_and_No_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			do_CylCommand();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

//==============================================================================
// cylinder 한번만 동작하고 센서체크 함.
//==============================================================================
void CDlgManuPressUnit::CylManualOper_Only_One_Action_and_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 2:
		{
			int nErr = Check_CylSensor();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep++;
				break;
			}
			m_nCylManualActionStep++;
		}break;
	case 3:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// 작업자가  stop하기 전까지 cylinder 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuPressUnit::CylManualOper_Continue_Action_and_No_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCmdCycleCount=0;
			m_nCylManualActionStep = 1000;
		}break;
	case 1000:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCmdCycleCount++;
				m_nCylManualActionStep = 2000;
			}
		}break;
	case 2000:
		{
			int nErr = do_CylCommand(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 2001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep = 1000;
			}
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuPressUnit::CylManualOper_Continue_Action_and_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCmdCycleCount = 0;
			m_nCylManualActionStep = 1000;
		}break;
	case 1000:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 1002:
		{
			int nErr = Check_CylSensor();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep=9000;
				break;
			}
			m_nCmdCycleCount++;
			m_nCylManualActionStep = 2000;
		}break;

	case 2000:
		{
			int nErr = do_CylCommand(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 2001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 2002:
		{
			int nErr = Check_CylSensor(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_nCylManualActionStep = 1000;
		}break;

	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuPressUnit::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			//m_nCylActionTimeRetry = 0;
			DisplayCylinderActionTime(TRUE);
			m_nCylManualActionStep = 1000;
			m_dCylActionTimeRecord = 0.0;
		}break;
		//실린더 동작
	case 1000: 
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
		// 센서 체크
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > 10000.0 )
			{
				// 				if( m_nCylActionTimeRetry < 1)
				// 				{
				// 					m_nCylManualActionStep = 1000;
				// 					m_nCylActionTimeRetry++;
				// 					break;
				// 				}
				// 				else
				// 				{
				int nErr = Check_CylSensor();
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
				//				}
			}
			int nChk = Check_CylSensor();
			if( nChk == ERR_NO_ERROR )
			{
				m_dCylActionTimeRecord = dSpanTime / 1000.0;
				m_nCylManualActionStep++;
				break;
			}
		}break;
	case 1002:
		{
			DisplayCylinderActionTime();
			g_DB.UPDATE_CYLINDER_TIME_TBL(m_nCylinderNum, m_dCylActionTimeRecord);
			m_nCylinderNum = 0;
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

void CDlgManuPressUnit::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
	CString strTime;
	strTime.Format("%.3f", m_dCylActionTimeRecord);
	if( bClear ) strTime = "";
	SetDlgItemText(UNITSETDLL_IDC_STATIC_TIME, strTime);
}

void CDlgManuPressUnit::ClearCommand()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdOperation = eCylManualOper_None;
	m_nCmdParam_Time = -1;
}

void CDlgManuPressUnit::SetCommand(int nCmd)
{
	if( m_nCylManualOpActState != eCylManualCmd_None ){
		return;
	}
	if( m_cbOperationSel.GetCurSel() < 0 ){
		return;
	}
	if( m_cbTimeSel.GetCurSel() < 0 ){
		return;
	}
	if( m_cbSiteSel.GetCurSel() < 0){
		return;
	}
	m_vCmdSite.clear();
	if(nCmd == eCylManualCmd_AllUp || nCmd == eCylManualCmd_AllDown){
		for(int i=0; i< eMaxPressUnitCount; i++){
			m_vCmdSite.push_back(i);
		}
	}else{
		GetSiteSel(m_vCmdSite);
	}

	m_nCmdManualCyl = nCmd;
	m_nCmdOperation = m_cbOperationSel.GetCurSel();
	m_nCmdParam_Time = m_cbTimeSel.GetCurSel()+1;

	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = m_nCmdOperation;

	CylinderSpecUpdate();
}

void CDlgManuPressUnit::OnBnClickedButtonEachUp()
{
	SetCommand(eCylManualCmd_EachUp);
	CylinderNum(DEF_UP);
}


void CDlgManuPressUnit::OnBnClickedButtonEachDown()
{
	SetCommand(eCylManualCmd_EachDown);
	CylinderNum(DEF_DOWN);
}


void CDlgManuPressUnit::GetSiteSel(std::vector<int>& nSiteSel)
{
	int nCurSel = m_cbSiteSel.GetCurSel();
	if( nCurSel < 0  || nCurSel >= 16){
		return;
	}

	nSiteSel.push_back(nCurSel);
}



void CDlgManuPressUnit::OnBnClickedButtonAllUp()
{
	SetCommand(eCylManualCmd_AllUp);
}


void CDlgManuPressUnit::OnBnClickedButtonAllDown()
{
	SetCommand(eCylManualCmd_AllDown);
}



void CDlgManuPressUnit::OnBnClickedButtonCtylActStop()
{
	m_nCylManualOpActState = eCylManualCmd_None;
}

void CDlgManuPressUnit::InitSpreadSheet()
{
	CString msg = "";
	CString msg1 = "";
	long SPREADHEADER = -1000;

	m_SpreadSiteDown.SetRow(SPREADHEADER);

	int n1STATION_IN_MAX_SITE_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int n1STATION_IN_MAX_SITE_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nCol = 0; nCol < eMaxPressUnitCount; nCol++)
	{
		m_SpreadSiteDown.AddCellSpan(nCol*n1STATION_IN_MAX_SITE_X + 1, 0, n1STATION_IN_MAX_SITE_X, 1);
		m_SpreadSiteDown.SetCol(nCol*n1STATION_IN_MAX_SITE_X + 1);
		msg1.Format("%s%d", _T("Station#"), nCol + 1);
		m_SpreadSiteDown.SetText(msg1);
	}

	m_SpreadSiteDown.SetRow(n1STATION_IN_MAX_SITE_Y);
	m_SpreadSiteDown.SetCol((eMaxPressUnitCount * n1STATION_IN_MAX_SITE_X) / 2);

	m_SpreadSiteDown.SetAutoSize(TRUE);
}

void CDlgManuPressUnit::SetBtnDisplay()
{
	m_BtnUpAll.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnUpAll.SetIcon(IDI_BTN_UP);
	m_BtnUpAll.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAllDown.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnAllDown.SetIcon(IDI_BTN_DOWN);
	m_BtnAllDown.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnUpEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnUpEach.SetIcon(IDI_BTN_UP);
	m_BtnUpEach.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDownEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDownEach.SetIcon(IDI_BTN_DOWN);
	m_BtnDownEach.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnVibrator.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	//m_BtnUpEach.SetIcon(IDI_BTN_UP);
	m_BtnVibrator.SetAlign(CButtonST::ST_ALIGN_VERT);
	
}

void CDlgManuPressUnit::ChangeDialog()
{
	GetDlgItem(IDC_STATIC_TEXT_ANTI_VIB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT67)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_OPERATION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_CONTINUE_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SELECT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_SITE_SEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT65)->ShowWindow(SW_HIDE);
	GetDlgItem(UNITSETDLL_IDC_STATIC_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SEC)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_ALL_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_ALL_DOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CTYL_ACT_STOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EACH_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EACH_DOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT71)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_UP_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_DOWN_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_UP_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_DOWN_2)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TEXT_SITE_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_12)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_13)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_14)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_15)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_SITE_16)->ShowWindow(SW_HIDE);
	

	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_DOWN_SITE8)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COLORCHECKBTCTRL_UP_SITE8)->ShowWindow(SW_HIDE);

	std::vector<int> vControl;
	vControl.push_back(IDOK);

	for(int i=0; i<(int)vControl.size(); i++){
		HWND hwndBox = ::GetDlgItem(this->m_hWnd, vControl[i]);
		RECT rc1;
		::GetWindowRect(hwndBox, &rc1);

		GetDlgItem(vControl[i])->SetWindowPos(NULL,rc1.left,(rc1.top-380),(rc1.right - rc1.left),(rc1.bottom - rc1.top),0);
	}
	HWND hwndBox = ::GetDlgItem(this->m_hWnd, IDD_MANU_PRESSUNIT);
	RECT rc1;
	::GetWindowRect(this->m_hWnd, &rc1);

	MoveWindow(250,250,rc1.right,(rc1.bottom -350));
}

void CDlgManuPressUnit::OnBnClickedButtonVibrator()
{
	//Vibrator 다이얼로그 
	CDlgVibrator DlgVibrator;
	DlgVibrator.m_nUnit  = ePressVibratorUnit;
	DlgVibrator.DoModal();
}


void CDlgManuPressUnit::CylinderSpecIndex(int& nCmdManualCyl,int& nCmdSite)
{
	switch (m_nCmdManualCyl)
	{
	case eCylManualCmd_EachUp:
	{
		nCmdManualCyl = ePressUp;
		nCmdSite = m_vCmdSite[0];
	}
	break;
	case eCylManualCmd_EachDown:
	{
		nCmdManualCyl = ePressDown;
		nCmdSite = m_vCmdSite[0];
	}
	break;


	default:
		break;
	}
}

void CDlgManuPressUnit::CylinderSpecInOut(CDC* pDC)
{
	if (m_vCmdSite.size() == 0)
		return;

	int nCmdManualCyl = 0,nCmdSite = 0;

	CylinderSpecIndex(nCmdManualCyl, nCmdSite);

	if (m_dCylinderSpec[nCmdSite][nCmdManualCyl].dMin <= m_dCylActionTimeRecord &&
		m_dCylinderSpec[nCmdSite][nCmdManualCyl].dMax >= m_dCylActionTimeRecord)
		pDC->SetTextColor(RGB(0, 0, 255));
	else
		pDC->SetTextColor(RGB(255, 0, 0));

}

void CDlgManuPressUnit::CylinderSpecUpdate()
{
	if (m_vCmdSite.size() == 0)
		return;

	int nCmdManualCyl = 0, nCmdSite = 0;

	CylinderSpecIndex(nCmdManualCyl, nCmdSite);

	CString str;
	str.Format("%.2f ~ %.2f", m_dCylinderSpec[nCmdSite][nCmdManualCyl].dMin, m_dCylinderSpec[nCmdSite][nCmdManualCyl].dMax);
	GetDlgItem(UNITSETDLL_IDC_STATIC_SPEC)->SetWindowText(str);
}

HBRUSH CDlgManuPressUnit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if (pWnd->GetDlgCtrlID() == UNITSETDLL_IDC_STATIC_TIME)
			{
				CylinderSpecInOut(pDC);
				//pDC->SetBkColor(RGB(0, 255, 0));
			}
		}
	}
	return hbr;
}

void CDlgManuPressUnit::CylinderNum(BOOL bUpDown)
{
	int nStationNum = m_cbSiteSel.GetCurSel() +1;

	if (bUpDown == DEF_UP)
		m_nCylinderNum = eAnti_Vibration_1_Up + (nStationNum * 2 - 1) - 1;
	else
		m_nCylinderNum = eAnti_Vibration_1_Up + (nStationNum * 2) - 1;
}