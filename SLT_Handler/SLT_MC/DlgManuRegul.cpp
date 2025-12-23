// DlgManuRegul.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuRegul.h"
#include "afxdialogex.h"
#include "PressUnitConvert.h"

// CDlgManuRegul 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuRegul, CDialogEx)

CDlgManuRegul::CDlgManuRegul(int nRegulatorIdx, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuRegul::IDD, pParent)
{
	m_nRegulatorIdx = nRegulatorIdx;
	m_dSetPressureValue = 0.0;
	m_nCurPressureUnit = 0;
}

CDlgManuRegul::~CDlgManuRegul()
{
	m_cBmpExit.DeleteObject();
}

void CDlgManuRegul::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_RVALUE, m_sliderPressure);
	DDX_Control(pDX, IDC_COMBO_PRESSURE_UNIT, m_cbPressureUnit);
	DDX_Control(pDX, IDC_EDIT_PRESS_MPA, m_editSetPressure);
	DDX_Control(pDX, IDC_COMBO_SEL_REG_NO, m_cbSelectRegulator);

	// exit
	DDX_Control(pDX, IDCANCEL, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgManuRegul, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDlgManuRegul::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SET_PRESS, &CDlgManuRegul::OnBnClickedButtonSetPress)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESSURE_UNIT, &CDlgManuRegul::OnCbnSelchangeComboPressureUnit)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RVALUE, &CDlgManuRegul::OnNMReleasedcaptureSliderRvalue)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_REG_NO, &CDlgManuRegul::OnCbnSelchangeComboSelRegNo)
	ON_EN_SETFOCUS(IDC_EDIT_PRESS_MPA, &CDlgManuRegul::OnEnSetfocusEditPressMpa)
	ON_BN_CLICKED(IDC_BUTTON_SET_FORCE, &CDlgManuRegul::OnBnClickedButtonSetForce)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_CVT_TO_GF, &CDlgManuRegul::OnBnClickedButtonUnitCvtToGf)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_CVT_TO_MPA, &CDlgManuRegul::OnBnClickedButtonUnitCvtToMpa)
	ON_BN_CLICKED(IDOK_RSAVE, &CDlgManuRegul::OnBnClickedRsave)
	ON_BN_CLICKED(IDC_BUTTON_REG_IDX_UP, &CDlgManuRegul::OnBnClickedButtonRegIdxUp)
	ON_BN_CLICKED(IDC_BUTTON_REG_IDX_DOWN, &CDlgManuRegul::OnBnClickedButtonRegIdxDown)
	ON_BN_CLICKED(IDC_BUTTON_SET_FORCE_ALL, &CDlgManuRegul::OnBnClickedButtonSetForceAll)
	ON_BN_CLICKED(IDOK_RSAVE_ALL, &CDlgManuRegul::OnBnClickedRsaveAll)
	ON_BN_CLICKED(IDC_BUTTON_SET_PRESS2, &CDlgManuRegul::OnBnClickedButtonSetPress2)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgManuRegul, CDialogEx)
	ON_EVENT(CDlgManuRegul, IDC_EDIT_SET_FORCE_PER_PIN, DISPID_CLICK, CDlgManuRegul::ClickEditSetForcePerPin, VTS_NONE)
	ON_EVENT(CDlgManuRegul, IDC_EDIT_SET_FORCE_TO_DEVICE, DISPID_CLICK, CDlgManuRegul::ClickEditSetForceToDevice, VTS_NONE)
	ON_EVENT(CDlgManuRegul, IDC_EDIT_CVT_REG_PRESSURE, DISPID_CLICK, CDlgManuRegul::ClickEditCvtRegPressure, VTS_NONE)
	ON_EVENT(CDlgManuRegul, IDC_EDIT_CVT_FORCE_PER_PIN, DISPID_CLICK, CDlgManuRegul::ClickEditCvtForcePerPin, VTS_NONE)
END_EVENTSINK_MAP()

// CDlgManuRegul 메시지 처리기입니다.

BOOL CDlgManuRegul::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgManuRegul::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_REGULATOR);
	m_btnExit.SetBitmap(m_cBmpExit);

	m_dSetPressureValue = g_TaskRegulator[m_nRegulatorIdx].GetCommandPressure();
	InitControl();

	SetTimer(1, 500, NULL);

	MakeLog("E/P Regulator Dialog Show");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuRegul::InitControl()
{
	m_cbPressureUnit.ResetContent();
	m_cbPressureUnit.AddString("MPa");
	m_cbPressureUnit.AddString("KPa");
	m_cbPressureUnit.AddString("bar");
	m_cbPressureUnit.AddString("psi");
	m_cbPressureUnit.AddString("kgf/cm2");
	m_cbPressureUnit.AddString("torr");
	m_cbPressureUnit.SetCurSel(CPressUnitConvert::MPa);
	m_nCurPressureUnit = CPressUnitConvert::MPa;

	m_cbSelectRegulator.ResetContent();
	for(int i=0; i<eMaxPressUnitCount; i++){
		CString strRegul;
		strRegul.Format("Site#%02d [%s %02d]", i+1, (i<=ePressUnit_Down_8)?_T("Down"):_T("Up"), (i<=ePressUnit_Down_8)? (i+1) : (i%8 +1) );
		m_cbSelectRegulator.AddString(strRegul);
	}
	m_cbSelectRegulator.SetCurSel(m_nRegulatorIdx);

	// Set Pressure
	CPressUnitConvert cvt2(CPressUnitConvert::MPa, m_dSetPressureValue);
	double dDisplayPressure = cvt2.ConvertTo(m_nCurPressureUnit);
	CString strValue;
	strValue.Format("%f", dDisplayPressure);
	SetDlgItemText(IDC_EDIT_PRESS_MPA, strValue);

	int nAiIndex = g_TaskRegulator[m_nRegulatorIdx].m_vAI[CTaskRegulator::aiREGUL_IN];
	g_DB.SELECT_AIO_TBL(_tAIO_LS::eAI, nAiIndex, m_stAiInfo);
	int nPos = (int)(m_dSetPressureValue / (m_stAiInfo.dMaxPhyVal - m_stAiInfo.dMinPhyVal) * 100.0);
	m_sliderPressure.SetRange(0, 100);
	m_sliderPressure.SetPos(nPos);

	// contact force
	// pin count
	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
	strValue.Format("%d", nPinCount);
	SetDlgItemText(IDC_EDIT_SET_PIN_COUNT, strValue);
	
	// force per pin (gf)
	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	strValue.Format("%.3f", dForcePerPin);
	SetDlgItemText(IDC_EDIT_SET_FORCE_PER_PIN, strValue);

	// force to device (gf)
	double dForceToDevice = g_DMCont.m_dmHandlerOpt.GD(DDM11_ContactForce_gf);
	strValue.Format("%.3f", dForceToDevice/1000.0);
	SetDlgItemText(IDC_EDIT_SET_FORCE_TO_DEVICE, strValue);

	strValue.Format("0.000");
	SetDlgItemText(IDC_EDIT_CVT_REG_PRESSURE, strValue);
	strValue.Format("0.000");
	SetDlgItemText(IDC_EDIT_CVT_FORCE_PER_PIN, strValue);
	strValue.Format("0.000");
	SetDlgItemText(IDC_EDIT_CVT_FORCE_TO_DEVICE, strValue);
}


void CDlgManuRegul::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}


void CDlgManuRegul::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuRegul::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			UpdateRegulatorValue();
			SetTimer(1, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuRegul::UpdateRegulatorValue()
{
	// Current Pressure
	double dCurPressure = g_TaskRegulator[m_nRegulatorIdx].GetCurrentPressure();
	CPressUnitConvert cvt(CPressUnitConvert::MPa, dCurPressure);
	double dDisplayPressure = cvt.ConvertTo(m_nCurPressureUnit);
	CString strValue;
	strValue.Format("%.3f", dDisplayPressure);
	SetDlgItemText(IDC_EDIT_MON_REG_PRESSURE, strValue);

	// Set Pressure
	CPressUnitConvert cvt2(CPressUnitConvert::MPa, m_dSetPressureValue);
	dDisplayPressure = cvt2.ConvertTo(m_nCurPressureUnit);
	strValue.Format("%f", dDisplayPressure);
	SetDlgItemText(IDC_EDIT_PRESS_MPA, strValue);

	// force
	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
	strValue.Format("%.3f", g_TaskRegulator[m_nRegulatorIdx].GetCurrentForcePerPin());
	SetDlgItemText(IDC_EDIT_MON_FORCE_PER_PIN, strValue);

	strValue.Format("%.3f", (g_TaskRegulator[m_nRegulatorIdx].GetCurrentForcePerPin() *(double)nPinCount) / 1000.0 );
	SetDlgItemText(IDC_EDIT_MON_FORCE_TO_DEVICE, strValue);
}

void CDlgManuRegul::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CDlgManuRegul::OnBnClickedButtonSetPress()
{
	// TODO SLT : show confirm message box
	g_TaskRegulator[m_nRegulatorIdx].SetCylinderRadious(eContactCylinderSize_32mm);
	int nErr = g_TaskRegulator[/*m_nCurPressureUnit*/m_nRegulatorIdx].do_SetRegulator(m_dSetPressureValue);
	if( nErr != ERR_NO_ERROR ){
		// TODO SLT : error message box
	}
	char szValue[32] = {0,};

	GetDlgItemText(IDC_EDIT_PRESS_MPA, szValue,sizeof(szValue));
	MakeLog("Site #%d Value: %s Set Press",m_nRegulatorIdx+1,szValue);

}


void CDlgManuRegul::OnCbnSelchangeComboPressureUnit()
{
	if( m_cbPressureUnit.GetCurSel() < 0 ){
		return;
	}
	m_nCurPressureUnit = m_cbPressureUnit.GetCurSel();
}


void CDlgManuRegul::OnNMReleasedcaptureSliderRvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	double dPressureSetRange = (double)m_sliderPressure.GetPos();
	m_dSetPressureValue =  m_stAiInfo.dMaxPhyVal * dPressureSetRange / 100.0;
	*pResult = 0;
}



void CDlgManuRegul::OnCbnSelchangeComboSelRegNo()
{
	if( m_cbSelectRegulator.GetCurSel() < 0 ){
		return;
	}
	m_nRegulatorIdx = m_cbSelectRegulator.GetCurSel();
	m_dSetPressureValue = g_TaskRegulator[m_nRegulatorIdx].GetCommandPressure();

	InitControl();
}


void CDlgManuRegul::OnEnSetfocusEditPressMpa()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgManuRegul::OnBnClickedButtonSetForce()
{
	double dGramPerPin = 0.0;
	CString rValue;
	GetDlgItemText(IDC_EDIT_SET_FORCE_PER_PIN, rValue);
	dGramPerPin = atof(rValue);
	int nErr = g_TaskRegulator[m_nRegulatorIdx].do_SetForcePerPin(dGramPerPin);
	if( nErr != ERR_NO_ERROR )
	{
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

/*
	// Set Pressure
	m_dSetPressureValue = g_TaskRegulator[m_nRegulatorIdx].GetCommandPressure();
	CPressUnitConvert cvt2(CPressUnitConvert::MPa, m_dSetPressureValue);
	double dDisplayPressure = cvt2.ConvertTo(m_nCurPressureUnit);
	CString strValue;
	strValue.Format("%f", dDisplayPressure);
	SetDlgItemText(IDC_EDIT_PRESS_MPA, strValue);

	int nAiIndex = g_TaskRegulator[m_nRegulatorIdx].m_vAI[CTaskRegulator::aiREGUL_IN];
	g_DB.SELECT_AIO_TBL(_tAIO_LS::eAI, nAiIndex, m_stAiInfo);
	int nPos = (int)(m_dSetPressureValue / (m_stAiInfo.dMaxPhyVal - m_stAiInfo.dMinPhyVal) * 100.0);
	m_sliderPressure.SetRange(0, 100);
	m_sliderPressure.SetPos(nPos);
*/
}


void CDlgManuRegul::ClickEditSetForcePerPin()
{
	char szValue_New[128]={0,};
	char szValue_Old[128]={0,};
	char szTitle[128]={0,};
	char szMin[128]={0,};
	char szMax[128]={0,};
	CString strValue_Old;

	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);

	GetDlgItemText(IDC_EDIT_SET_FORCE_PER_PIN, strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", strValue_Old.GetString() );
	sprintf_s(szMin, sizeof(szMin), "%.3f", 0.0);
	sprintf_s(szMax, sizeof(szMax), "%.3f", 999.9);//500.0/*N*/ / (double)nPinCount); 500N -> 500Kg/m2 -> 0.05gf/cm2

	int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, szTitle, szMax, szMin);
	if( nRet == RETURN_OK ){
		SetDlgItemText(IDC_EDIT_SET_FORCE_PER_PIN, szValue_New);
		double dNewForce = atof(szValue_New) * (double)nPinCount / 1000.0;
		sprintf_s(szValue_New, sizeof(szValue_New), "%.3f", dNewForce);
		SetDlgItemText(IDC_EDIT_SET_FORCE_TO_DEVICE, szValue_New);
	}
}


void CDlgManuRegul::ClickEditSetForceToDevice()
{
}


void CDlgManuRegul::OnBnClickedButtonUnitCvtToGf()
{
	// Set Pressure
	CString strValue;
	GetDlgItemText(IDC_EDIT_CVT_REG_PRESSURE, strValue);
	double dPressureMPa = atof(strValue);

	// force
	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
	double dForcePerPin_gf = g_TaskRegulator[m_nRegulatorIdx].ConvertMPa_To_ForcePerPin(dPressureMPa, nPinCount);
	double dForcePerPin_Trim_gf = g_TaskRegulator[m_nRegulatorIdx].GetTrimForceValue_gf(dForcePerPin_gf);
	dForcePerPin_gf -= dForcePerPin_Trim_gf;
	strValue.Format("%.3f", dForcePerPin_gf);
	SetDlgItemText(IDC_EDIT_CVT_FORCE_PER_PIN, strValue);

	strValue.Format("%.3f", dForcePerPin_gf / 1000.0 * (double)nPinCount);
	SetDlgItemText(IDC_EDIT_CVT_FORCE_TO_DEVICE, strValue);
}


void CDlgManuRegul::OnBnClickedButtonUnitCvtToMpa()
{
	// force
	CString strValue;
	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);

	GetDlgItemText(IDC_EDIT_CVT_FORCE_PER_PIN, strValue);
	double dForcePerPin_gf = atof(strValue);
	//double dForcePerPin_Trime_gf = g_TaskRegulator[m_nRegulatorIdx].GetTrimForceValue_gf(dForcePerPin_gf);
	//dForcePerPin_gf += dForcePerPin_Trime_gf;
	double dPressureMPa = g_TaskRegulator[m_nRegulatorIdx].ConvertForcePerPin_To_MPa(dForcePerPin_gf, nPinCount);

	// Set Pressure
	strValue.Format("%.6f", dPressureMPa);
	SetDlgItemText(IDC_EDIT_CVT_REG_PRESSURE, strValue);
}


void CDlgManuRegul::ClickEditCvtRegPressure()
{
	char szValue_New[128]={0,};
	char szValue_Old[128]={0,};
	char szTitle[128]={0,};
	char szMin[128]={0,};
	char szMax[128]={0,};
	CString strValue_Old;

	GetDlgItemText(IDC_EDIT_CVT_REG_PRESSURE, strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", strValue_Old.GetString() );
	sprintf_s(szMin, sizeof(szMin), "%.3f", 0.0);
	sprintf_s(szMax, sizeof(szMax), "%.3f", 0.5);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, szTitle, szMax, szMin);
	if( nRet == RETURN_OK ){
		SetDlgItemText(IDC_EDIT_CVT_REG_PRESSURE, szValue_New);
	}
}


void CDlgManuRegul::ClickEditCvtForcePerPin()
{
	char szValue_New[128]={0,};
	char szValue_Old[128]={0,};
	char szTitle[128]={0,};
	char szMin[128]={0,};
	char szMax[128]={0,};
	CString strValue_Old;

	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);

	GetDlgItemText(IDC_EDIT_CVT_FORCE_PER_PIN, strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", strValue_Old.GetString() );
	sprintf_s(szMin, sizeof(szMin), "%.3f", 0.0);
	sprintf_s(szMax, sizeof(szMax), "%.3f", 100.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, szTitle, szMax, szMin);
	if( nRet == RETURN_OK ){
		SetDlgItemText(IDC_EDIT_CVT_FORCE_PER_PIN, szValue_New);
		double dNewForce = atof(szValue_New) * (double)nPinCount / 1000.0;
		sprintf_s(szValue_New, sizeof(szValue_New), "%.3f", dNewForce);
		SetDlgItemText(IDC_EDIT_CVT_FORCE_TO_DEVICE, szValue_New);
	}
}


void CDlgManuRegul::OnBnClickedRsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//int nRet = AfxMessageBox("please confirm!!!", MB_OKCANCEL);
	//if( nRet == IDOK )
	//{
		g_TaskRegulator[m_nRegulatorIdx].Read_Cylinder_Config();
	//}
}


void CDlgManuRegul::OnBnClickedButtonRegIdxUp()
{
	if( m_cbSelectRegulator.GetCurSel() < 0 ){
		return;
	}
	int nIdx = m_cbSelectRegulator.GetCurSel()+1;
	if( nIdx >= eMaxPressUnitCount ) nIdx = eMaxPressUnitCount-1;
	m_nRegulatorIdx = nIdx;
	m_cbSelectRegulator.SetCurSel(m_nRegulatorIdx);
	m_dSetPressureValue = g_TaskRegulator[m_nRegulatorIdx].GetCommandPressure();
	InitControl();
}


void CDlgManuRegul::OnBnClickedButtonRegIdxDown()
{
	if( m_cbSelectRegulator.GetCurSel() < 0 ){
		return;
	}
	int nIdx = m_cbSelectRegulator.GetCurSel()-1;
	if( nIdx < 0 ) nIdx = 0;
	m_nRegulatorIdx = nIdx;
	m_cbSelectRegulator.SetCurSel(m_nRegulatorIdx);
	m_dSetPressureValue = g_TaskRegulator[m_nRegulatorIdx].GetCommandPressure();
	InitControl();
}


void CDlgManuRegul::OnBnClickedButtonSetForceAll()
{
	double dGramPerPin = 0.0;
	CString rValue;
	GetDlgItemText(IDC_EDIT_SET_FORCE_PER_PIN, rValue);
	dGramPerPin = atof(rValue);

 	g_LibCommCtrl.ProgressBarCreate();
 	g_LibCommCtrl.ProgressBarSetStatus("Set Regulator!!!!");

	for(int i=0; i < eMaxPressUnitCount; i++){
		int nErr = g_TaskRegulator[i].do_SetForcePerPin(dGramPerPin);
		double dPos = ((i+1)/(double)eMaxPressUnitCount)*100.0;
		g_LibCommCtrl.ProgressBarSetPos((int)dPos);

		if( nErr != ERR_NO_ERROR )
		{
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, NULL);
			g_LibCommCtrl.ProgressBarDestroy();
			return;
		}
	}	
	g_LibCommCtrl.ProgressBarDestroy();
}


void CDlgManuRegul::OnBnClickedRsaveAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//int nRet = AfxMessageBox("please confirm!!!", MB_OKCANCEL);
	for(int i=0; i < eMaxPressUnitCount; i++){
	//	if( nRet == IDOK )
	//	{
			g_TaskRegulator[i].Read_Cylinder_Config();
		//}
	}
}


void CDlgManuRegul::OnBnClickedButtonSetPress2()
{
	g_LibCommCtrl.ProgressBarCreate();
	g_LibCommCtrl.ProgressBarSetStatus("Set Regulator!!!!");

	for(int i=0; i < eMaxPressUnitCount; i++){
		g_TaskRegulator[i].SetCylinderRadious(eContactCylinderSize_32mm);
		int nErr = g_TaskRegulator[i].do_SetRegulator(m_dSetPressureValue);
		double dPos = ((i+1)/(double)eMaxPressUnitCount)*100.0;
		g_LibCommCtrl.ProgressBarSetPos((int)dPos);
		if( nErr != ERR_NO_ERROR ){
			// TODO SLT : error message box
		}
	}
	g_LibCommCtrl.ProgressBarDestroy();
	
	char szValue[32] = {0,};
	GetDlgItemText(IDC_EDIT_PRESS_MPA, szValue,sizeof(szValue));
	MakeLog("All Site Set Press Value: %s",szValue);
}

void CDlgManuRegul::MakeLog(LPCTSTR fmt, ...)
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