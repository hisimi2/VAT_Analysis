// DlgManuZMotorTeaching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuZMotorTeaching.h"
#include "afxdialogex.h"


const int cDataStaticIdx[VAT_PICKER_MAX] = {
	IDC_PICKER_1_OFFSET, IDC_PICKER_2_OFFSET, IDC_PICKER_3_OFFSET, IDC_PICKER_4_OFFSET,
	IDC_PICKER_5_OFFSET, IDC_PICKER_6_OFFSET, IDC_PICKER_7_OFFSET, IDC_PICKER_8_OFFSET,
};

const int cVacStaticIdx[VAT_PICKER_MAX] = {
	IDC_FLATNESS_PKR_VAC_1, IDC_FLATNESS_PKR_VAC_2, IDC_FLATNESS_PKR_VAC_3, IDC_FLATNESS_PKR_VAC_4,
	IDC_FLATNESS_PKR_VAC_5, IDC_FLATNESS_PKR_VAC_6, IDC_FLATNESS_PKR_VAC_7, IDC_FLATNESS_PKR_VAC_8,
};

// CDlgManuZMotorTeaching 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuZMotorTeaching, CDialogEx)

CDlgManuZMotorTeaching::CDlgManuZMotorTeaching(CString* strZTeachPos, int nCurRobotId, int nLocIdx, int nPosIdx, int nAxisNo, int nPartNo, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MANU_Z_MOTOR_TEACHING, pParent), m_strZTeachPos(strZTeachPos), m_nCurRobotId(nCurRobotId), m_nLocIdx(nLocIdx), m_nPosIdx(nPosIdx), m_nAxisNo(nAxisNo), m_nPartNo(nPartNo)
{
	m_nBoardDir = -1;

	m_nHand = -1;

	memset(m_szPosSWLimit, 0x00, sizeof(m_szPosSWLimit));
	memset(m_szNegSWLimit, 0x00, sizeof(m_szNegSWLimit));

	//Data Init
	for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
		m_dCtrlPickerData[nPkr] = 0.0;
	}

	m_dCtrlZTeachData = atof(*m_strZTeachPos);
}


CDlgManuZMotorTeaching::~CDlgManuZMotorTeaching()
{
}

void CDlgManuZMotorTeaching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//Z Teach Data
	DDX_Text(pDX, IDC_Z_TEACH_POS, m_dCtrlZTeachData);
	DDX_Control(pDX, IDC_Z_TEACH_POS, m_EditZTeachData);

	//Flatness Offset
	for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
		//Controll Data 저장
		DDX_Text(pDX, cDataStaticIdx[nPkr], m_dCtrlPickerData[nPkr]);

		//Controll Design
		DDX_Control(pDX, cDataStaticIdx[nPkr], m_EditPickerData[nPkr]);
	}

	//Control Btn
	DDX_Control(pDX, IDC_MANU_Z_TEACH_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
}


BEGIN_MESSAGE_MAP(CDlgManuZMotorTeaching, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_PICKER_1_OFFSET, IDC_PICKER_8_OFFSET, &CDlgManuZMotorTeaching::OnEnSetFocusFlatnessOffset)
	ON_EN_SETFOCUS(IDC_Z_TEACH_POS, &CDlgManuZMotorTeaching::OnEnSetfocusZTeachPos)
	ON_BN_CLICKED(IDC_MANU_Z_TEACH_SAVE, &CDlgManuZMotorTeaching::OnBnClickedManuZTeachSave)
END_MESSAGE_MAP()


// CDlgManuZMotorTeaching 메시지 처리기입니다.

BOOL CDlgManuZMotorTeaching::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT(eAXIS_TRAY_PP_1_Z <= m_nAxisNo && m_nAxisNo <= eAXIS_TEST_PP_2_Z2);

	//Hand 저장.
	InitHandIdx(m_nAxisNo);

	//[Pos/Neg Limit값 저장]
	sprintf(m_szPosSWLimit, "%.3f", g_Motor[m_nAxisNo]->GetSwLimitPosi() / 1000.0);
	sprintf(m_szNegSWLimit, "%.3f", g_Motor[m_nAxisNo]->GetSwLimitNega() / 1000.0);

	//[Edit 초기화]	
	for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
 		m_EditPickerData[nPkr].SetBackColor(COLOR_WWHITE);
 		m_EditPickerData[nPkr].SetTextSize(19);
	} 

 	m_EditZTeachData.SetBackColor(COLOR_WWHITE);
 	m_EditZTeachData.SetTextSize(20);
	

	
	//[Data Display]
	DisplayData();


	//[Control Btn]
	//SAVE
	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	//CANCEL
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_nBoardDir = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	SetTimer(0, 100, NULL);

	return TRUE;
}


void CDlgManuZMotorTeaching::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
		case 0:
		{
			KillTimer(nIDEvent);

			CDSSPanel* chkbt = NULL;

			for (int y = 0; y < MAX_PICKER_Y_CNT; y++)
			{
				for (int x = 0; x < MAX_PICKER_X_CNT; x++)
				{
					int vacuum_sen = GetPickerVacuum(m_nHand, CPoint(x, y));

					int nGuiIdx = x + (y * 4);
					if (VAT_HAND_LOAD_TESTPP1 < m_nHand && m_nHand < VAT_HAND_UNLOAD_TESTPP2) {
						if (m_nBoardDir == eBoardDir_Forward) {
							nGuiIdx = abs((x + 4 * y) - 7);
						}
						else { //nBoardType == eBoardDir_Backward			
							nGuiIdx = x + 4 * y;
						}
					}

					chkbt = (CDSSPanel*)GetDlgItem(cVacStaticIdx[nGuiIdx]);
					if (vacuum_sen)		chkbt->On();
					else				chkbt->Off();
				}
			}

			SetTimer(nIDEvent, 100, NULL);
		}break;
		default:
			break;
	}

}


void CDlgManuZMotorTeaching::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(0);	
}


int CDlgManuZMotorTeaching::GetPickerVacuum(int nHand, CPoint picker)
{
	int nChkVacuum = 0;

	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		nChkVacuum = g_TaskTrayLoadPP.GetVacuum(CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		nChkVacuum = g_TaskTrayUnloadPP.GetVacuum(CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	{
		nChkVacuum = g_TaskTestPP[eTestPP_1].GetVacuum(eTestPPLoadType, CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		nChkVacuum = g_TaskTestPP[eTestPP_1].GetVacuum(eTestPPUnLoadType, CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	{
		nChkVacuum = g_TaskTestPP[eTestPP_2].GetVacuum(eTestPPLoadType, CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_UNLOAD_TESTPP2: {
		nChkVacuum = g_TaskTestPP[eTestPP_2].GetVacuum(eTestPPUnLoadType, CPoint(picker.x, picker.y));
	} break;
	default:
		break;
	}

	return nChkVacuum;

}


void CDlgManuZMotorTeaching::InitHandIdx(int nAxisNo)
{
	if (eAXIS_TRAY_PP_1_Z == nAxisNo)		m_nHand = VAT_HAND_LOAD_TRAYPP;
	else if (eAXIS_TRAY_PP_2_Z == nAxisNo)	m_nHand = VAT_HAND_UNLOAD_TRAYPP;
	else if (eAXIS_TEST_PP_1_Z == nAxisNo)	m_nHand = VAT_HAND_LOAD_TESTPP1;
	else if (eAXIS_TEST_PP_1_Z2 == nAxisNo)	m_nHand = VAT_HAND_UNLOAD_TESTPP1;
	else if (eAXIS_TEST_PP_2_Z == nAxisNo)	m_nHand = VAT_HAND_LOAD_TESTPP2;
	else if (eAXIS_TEST_PP_2_Z2 == nAxisNo)	m_nHand = VAT_HAND_UNLOAD_TESTPP2;
}

void CDlgManuZMotorTeaching::DisplayData()
{
	ASSERT(VAT_HAND_LOAD_TRAYPP <= m_nHand && m_nHand <= VAT_HAND_UNLOAD_TESTPP2);

	//Z Teach Pos
	CString strZTeachData = _T("");
	strZTeachData.Format("%7.3f", m_dCtrlZTeachData);
	GetDlgItem(IDC_Z_TEACH_POS)->SetWindowText(strZTeachData);
	
	//Flatness Offset
	CString strDisplayEnc = _T("");
	for (int nPkrIdx = 0; nPkrIdx < VAT_PICKER_MAX; nPkrIdx++)
	{
		double dDisplayEnc = g_VATCommonData.m_stVATZTeachInfo.stPickerGap.getPickerGap(m_nHand, m_nPartNo, nPkrIdx);

		strDisplayEnc.Format("%7.3f", dDisplayEnc / 1000.0);
		GetDlgItem(cDataStaticIdx[nPkrIdx])->SetWindowText(strDisplayEnc);
	}

}


void CDlgManuZMotorTeaching::OnEnSetFocusFlatnessOffset(UINT nID)
{
	UpdateData(TRUE);

	char szInputValue[10] = { 0, };
	char szCurrentValue[10] = { 0, };

	int nPkrIdx = nID - IDC_PICKER_1_OFFSET;

	sprintf(szCurrentValue, "%.2f", m_dCtrlPickerData[nPkrIdx]);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 10, szCurrentValue, "Flateness Offset", "5.0", "-5.0");
	if (nRet == RETURN_OK) {
		GetDlgItem(nID)->SetWindowText(szInputValue);
	}
}


void CDlgManuZMotorTeaching::OnEnSetfocusZTeachPos()
{
	char szInputValue[10] = { 0, };
	char szCurrentValue[10] = { 0, };

	sprintf(szCurrentValue, "%.2f", m_dCtrlZTeachData);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 10, szCurrentValue, "Z Teaching Position", m_szPosSWLimit, m_szNegSWLimit);
	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_Z_TEACH_POS)->SetWindowText(szInputValue);		
	}
}


void CDlgManuZMotorTeaching::OnBnClickedManuZTeachSave()
{
	int nRet = AfxMessageBox("do you want to save?", MB_YESNO);
	if (nRet != IDYES) {
		return;
	}

	UpdateData(TRUE); //Control -> 변수

	//1. Teaching Position 저장.
	//변경된 값 참조변수 확인을 위함.
	GetDlgItem(IDC_Z_TEACH_POS)->GetWindowText(*m_strZTeachPos); 

	//Teaching Table Save
	g_DMCont.m_Teach.SetTeachPos(m_nCurRobotId, m_nLocIdx, m_nPosIdx, m_dCtrlZTeachData * 1000.0);
	g_DMCont.m_Teach.Save();




	// 2. Flatness Offset 저장.
 	CString strFilePath = g_VATCommonData.GetFilePathHand(VAT_DATA_PATH_Z_AUTO_TEACHING, m_nHand);
 	CConfigData cfData((LPSTR)(LPCTSTR)strFilePath);
 	char *pSection = (m_nHand == VAT_HAND_LOAD_TRAYPP || m_nHand == VAT_HAND_UNLOAD_TRAYPP) ? g_VATCommonData.m_stVATFileSection.pZAutoTeachingSec_PickerGap_TrayPP[m_nPartNo] : g_VATCommonData.m_stVATFileSection.pZAutoTeachingSec_PickerGap_TesstPP[m_nPartNo];
 
 	for (int nPkrIdx = 0; nPkrIdx < VAT_PICKER_MAX; nPkrIdx++) {
 		double dFlatnessOffset = m_dCtrlPickerData[nPkrIdx] * 1000.0;
 
 		//구조체 Data 저장.
 		g_VATCommonData.m_stVATZTeachInfo.stPickerGap.setPickerGap(m_nHand, m_nPartNo, nPkrIdx, dFlatnessOffset);
 
 		//File 저장.
 		cfData.Set(pSection, g_VATCommonData.m_stVATFileSection.pKeyHandPickerNum[nPkrIdx], dFlatnessOffset);
 	}


	DisplayData();
}
