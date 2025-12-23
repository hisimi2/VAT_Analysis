// VATMain_ZAutoFocus.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMain_ZAutoFocus.h"
#include "afxdialogex.h"
#include "VAT_STATIC_ID.h"


// CVATMain_ZAutoFocus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMain_ZAutoFocus, CDialogEx)

CVATMain_ZAutoFocus::CVATMain_ZAutoFocus(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_Z_AUTO_FOCUS, pParent)
{
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;	
}

CVATMain_ZAutoFocus::~CVATMain_ZAutoFocus()
{
}

void CVATMain_ZAutoFocus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_LOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_UNLOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_LOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_UNLOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_LOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_UNLOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_HAND_SELECT, m_ssPanel_HandSelect);
	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_MEASURE_STATUS, m_ssPanel_MeasureStatus);


	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_TARGET_POSITION, m_ssDataPanel_1_Title);
	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_TARGET_POS_DATA, m_ssDataPanel_1_Sub1);
	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_TARGET_POS_DATA_DIFF, m_ssDataPanel_1_Sub2);
	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_VACUUM_POSITION, m_ssDataPanel_2_Title);
	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_VACUUM_POS_DATA, m_ssDataPanel_2_Sub1);
	DDX_Control(pDX, IDC_SSPANEL_ZFOCUS_VACUUM_POS_DATA_DIFF, m_ssDataPanel_2_Sub2);

	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_TARGET_POSITION, m_ssPlateJigTargetPos);
	DDX_Control(pDX, IDC_SPREAD_ZFOCUS_VACUUM_POSITION, m_ssPlateJigVacPos);

	DDX_Control(pDX, IDC_BTN_ZFOCUS_ENCODER_READ_1, m_BtnOper1_EncoderRead);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_MOTOR_MOVE_1, m_BtnOper1_MotorMove);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_RELOAD_1, m_BtnOper1_Reload);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_DATA_SAVE_1, m_BtnOper1_Save);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_MAKE_BASE_POS_1, m_BtnOper1_MakeBasePos);

	DDX_Control(pDX, IDC_BTN_ZFOCUS_ENCODER_READ_2, m_BtnOper2_EncoderRead);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_MOTOR_MOVE_2, m_BtnOper2_MotorMove);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_RELOAD_2, m_BtnOper2_Reload);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_DATA_SAVE_2, m_BtnOper2_Save);
	DDX_Control(pDX, IDC_BTN_ZFOCUS_MAKE_BASE_POS_2, m_BtnOper2_MakeBasePos);

	//Measure Overview
	for (int i= 0; i < VAT_HAND_MAX; i++) {
		DDX_Control(pDX, m1_nHandPosition[i], m_editPosition[i]);
		DDX_Control(pDX, m1_nProgress[i], m_Progress[i]);

		DDX_Control(pDX, m1_nPercent[i], m_Percent[i]);
	}


	//DDX_Control(pDX, IDC_SLIDER_LOWER_LED_1, m_sliderLabel);

}

BEGIN_EVENTSINK_MAP(CVATMain_ZAutoFocus, CDialogEx)
	//ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_HAND, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadHand, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_TARGET_POSITION, 5, CVATMain_ZAutoFocus::ClickSpreadZfocusTargetPosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_VACUUM_POSITION, 5, CVATMain_ZAutoFocus::ClickSpreadZfocusVacuumPosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_TARGET_POSITION, 11, CVATMain_ZAutoFocus::DblClickSpreadZfocusTargetPosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_VACUUM_POSITION, 11, CVATMain_ZAutoFocus::DblClickSpreadZfocusVacuumPosition, VTS_I4 VTS_I4)

	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_LOAD_TRAYPP, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusLoadTraypp, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_UNLOAD_TRAYPP, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusUnloadTraypp, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_LOAD_TESTPP_1, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusLoadTestpp1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_UNLOAD_TESTPP_1, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusUnloadTestpp1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_LOAD_TESTPP_2, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusLoadTestpp2, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SPREAD_ZFOCUS_UNLOAD_TESTPP_2, 3, CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusUnloadTestpp2, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SSPANEL_ZFOCUS_TARGET_POS_DATA, DISPID_CLICK, CVATMain_ZAutoFocus::ClickSspanelZfocusTargetPosData, VTS_NONE)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SSPANEL_ZFOCUS_TARGET_POS_DATA_DIFF, DISPID_CLICK, CVATMain_ZAutoFocus::ClickSspanelZfocusTargetPosDataDiff, VTS_NONE)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SSPANEL_ZFOCUS_VACUUM_POS_DATA, DISPID_CLICK, CVATMain_ZAutoFocus::ClickSspanelZfocusVacuumPosData, VTS_NONE)
	ON_EVENT(CVATMain_ZAutoFocus, IDC_SSPANEL_ZFOCUS_VACUUM_POS_DATA_DIFF, DISPID_CLICK, CVATMain_ZAutoFocus::ClickSspanelZfocusVacuumPosDataDiff, VTS_NONE)
END_EVENTSINK_MAP()


BEGIN_MESSAGE_MAP(CVATMain_ZAutoFocus, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_ENCODER_READ_1, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusEncoderRead1)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_MOTOR_MOVE_1, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMotorMove1)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_RELOAD_1, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusReload1)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_DATA_SAVE_1, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusDataSave1)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_ENCODER_READ_2, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusEncoderRead2)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_MOTOR_MOVE_2, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMotorMove2)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_RELOAD_2, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusReload2)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_DATA_SAVE_2, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusDataSave2)

	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_HAND_STATUS, &CVATMain_ZAutoFocus::OnReceiveUpdateHandStatus)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_PROGRESS, &CVATMain_ZAutoFocus::OnReceiveUpdateProgress)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_STATUS, &CVATMain_ZAutoFocus::OnReceiveUpdateVisionStatus)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_OFFSET, &CVATMain_ZAutoFocus::OnReceiveUpdateVisionOffset)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_RETRY_CNT, &CVATMain_ZAutoFocus::OnReceiveUpdateVisionMeasureCnt)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_SAVE_DATA, &CVATMain_ZAutoFocus::OnReceiveUpdateSavedData)

	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_UPPERCAM_OFFSET_X_DATA_1, IDC_LOERCAM_OFFSET_Y_DATA_4, &CVATMain_ZAutoFocus::OnEnSetFocusCursorBlock)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_MAKE_BASE_POS_1, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMakeBasePos1)
	ON_BN_CLICKED(IDC_BTN_ZFOCUS_MAKE_BASE_POS_2, &CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMakeBasePos2)
END_MESSAGE_MAP()


BOOL CVATMain_ZAutoFocus::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//Data Load
	m_stVATZFocusInfoGui = g_VATCommonData.m_stVATZFocusInfo;

	//Title SSPanel 초기화.
	CDSSPanel* panel[2] = { 
		&m_ssPanel_HandSelect, &m_ssPanel_MeasureStatus
	};
	for (auto p : panel) {
		_Com_InitSSTitlePanel(*p);
	}	

	//Data SSPanel 초기화
	//Z Auto Focus Spread Sheet SSPanel 초기화.
	_Com_InitSSDataPanel(m_ssDataPanel_1_Title, m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2);
	_Com_InitSSDataPanel(m_ssDataPanel_2_Title, m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2);

	//Arm 선택 Spread Sheet 초기화.
	_Com_InitHandSelectSheet();

	//Z Auto Focus Spread Sheet 초기화.
	InitZFocusBasePosSheet();

	//Z Auto Focus Spread Sheet Data Display
	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);
	DisplaySSData(_eZFocusSS_PlateJigVacPos,	m_nSS2_DisplayType);

	//Operate Button 초기화
	InitZFocusOperBtn();
	
	//Measure Overview 초기화.
	InitZFocusMeasureOverview();

	return TRUE;
}


//==================================================================
//COMMENT : Mode 변경 후,  Z Auto Focus Page 이동 시 초기화역할로 사용
//==================================================================
void CVATMain_ZAutoFocus::PageMoveInit_Main()
{
	//Timer Set
//  	for (int i = 0; i < _eZFocusT_Max; i++) {
//  		SetTimer(i, 200, NULL);
//  	}
	SetTimer(_eZFocusT_HandStatus, 1000, NULL);
	SetTimer(_eZFocusT_CheckExecuteStatus, 200, NULL);
	//Data Load
	m_stVATZFocusInfoGui = g_VATCommonData.m_stVATZFocusInfo;

	//Z Auto Focus Spread Sheet Data Display
	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);
	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS2_DisplayType);
}

//================================================================
//COMMENT : Mode 변경 후, 다른 Page 이동 시, 소멸자 역할로 사용
//================================================================
void CVATMain_ZAutoFocus::PageMoveDispose()
{
	//Kill Timer Set
 	for (int i = 0; i < _eZFocusT_Max; i++) {
 		KillTimer(i);
 	}	
}

//추가해야 할 거.
//_eZTeachMeasureT_CheckExecuteStatus

void CVATMain_ZAutoFocus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nIDEvent)
	{	
		case _eZFocusT_HandStatus:
		{
			//KillTimer(nIDEvent);

			_Com_tDisplayHandStatus(m_editPosition);
		
			//SetTimer(_eZFocusT_HandStatus, 1000, NULL);
		}break;
		case _eZFocusT_CheckExecuteStatus:
		{
			//KillTimer(nIDEvent);

			//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
			if (g_bVATStatusExecute != m_bLastExecuteStatus)
			{
				EnableControls(!g_bVATStatusExecute);
				m_bLastExecuteStatus = g_bVATStatusExecute;
			}

			//SetTimer(_eZFocusT_CheckExecuteStatus, 200, NULL);
		}break;
		default:
			break;
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CVATMain_ZAutoFocus::OnDestroy()
{
	for (int i = 0; i < _eZFocusT_Max; i++) {
		KillTimer(i);
	}

	CDialogEx::OnDestroy();
}


void CVATMain_ZAutoFocus::InitZFocusBasePosSheet()
{
	//Plate Jig Target Pos
	_Com_InitPosSpreadSheet_HandType(m_ssPlateJigTargetPos);

	//Plate Jig Vacuum Pos
	_Com_InitPosSpreadSheet_HandType(m_ssPlateJigVacPos);

	//Last Sheet Info 설정.
	m_stTargetPosSheetLastInfo.nSSType = eVAT_SSTYPE_HAND;
	m_stPlateJigVacPosSheetLastInfo.nSSType = eVAT_SSTYPE_HAND;
}


void CVATMain_ZAutoFocus::InitZFocusOperBtn()
{
	CShadeButtonST *btnOper1[eSSOper_MAX] = { &m_BtnOper1_EncoderRead,  &m_BtnOper1_MotorMove, &m_BtnOper1_Reload, &m_BtnOper1_Save, &m_BtnOper1_MakeBasePos };
	CShadeButtonST *btnOper2[eSSOper_MAX] = { &m_BtnOper2_EncoderRead,  &m_BtnOper2_MotorMove, &m_BtnOper2_Reload, &m_BtnOper2_Save, &m_BtnOper2_MakeBasePos };
	for (int nType = 0; nType < eSSOper_MAX; nType++) {
		//Plate Jig Target Pos 
		_Com_InitOperBtn(*btnOper1[nType], nType);

		//Plate Jig Vacuum Pos
		_Com_InitOperBtn(*btnOper2[nType], nType);
	}
}

void CVATMain_ZAutoFocus::InitZFocusMeasureOverview()
{
	//Progress Bar
	_Com_InitProgressBar();

	//Vision Offset
	CFont StaticFont;
	StaticFont.CreatePointFont(120, "Fixedsys");

	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
		for (int nOffsetIdx = 0; nOffsetIdx < VAT_VISION_OFFSET_MAX; nOffsetIdx++) {
			CEdit *editOffset = (CEdit *)GetDlgItem(m1_nCamOffset[nCam][nOffsetIdx]);
			editOffset->SetWindowText(_T("0.000"));

			GetDlgItem(m1_nCamOffset[nCam][nOffsetIdx])->SetFont(&StaticFont);
		}
	}

	//Vision Status && Vision Retry Cnt
	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
		int nFrame = 1; //VAT_RCV_STATUS_INIT 
		CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m1_nCamStatus[nCam]);
		chkbt->SetStatus(nFrame);
		chkbt->SetForeColor(BBLACK);
		chkbt->SetWindowText(_T("0"));
	}

	
}


// void CVATMain_ZAutoFocus::InitZFocusEditControl()
// {
// 	CFont StaticFont;
// 	StaticFont.CreatePointFont(120, "Fixedsys");
// 
// 	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
// 		for (int nOffsetIdx = 0; nOffsetIdx < VAT_VISION_OFFSET_MAX; nOffsetIdx++) {
// 			CEdit *editOffset = (CEdit *)GetDlgItem(m1_nCamOffset[nCam][nOffsetIdx]);
// 			editOffset->SetWindowText(_T("0.000"));
// 
// // 			editOffset->ModifyStyle(WS_TABSTOP, 0);
// // 			editOffset->SetReadOnly(TRUE);
// 
// 			GetDlgItem(m1_nCamOffset[nCam][nOffsetIdx])->SetFont(&StaticFont);
// 		}
// 	}
// }


/*========================================================================*/
//COMMENT : Spread Sheet에 Data표기 함수.
//			Data는 Buff Data로만 표기 한다.			
//Param	  :	Param1  : Z Auto Focus에 존재하는 Spread Sheet 종류.
//			Param2  : Spread Sheet의 Data 표기 Type(Data, Data Difference)
/*========================================================================*/
void CVATMain_ZAutoFocus::DisplaySSData(int nZFocusSSType, int nSSDisplayType,BOOL bMakeBasePos/* = FALSE*/)
{
	switch (nZFocusSSType)
	{
		case _eZFocusSS_PlateJigTargetPos:
		{
			for (auto nHand : m_vAllHand)
			{
				for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
				{
					double dPos = 0.0;
					if (bMakeBasePos == FALSE)
					{
						if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) {
							dPos = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis];
						}
						else { //nSSDisplayType == eVAT_SSDISPLAYTYPE_DIFFERENCE
							   //변경된 Data - 이전 Data
							dPos = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis] - g_VATCommonData.m_stVATZFocusInfoBuff.dPlateJIG_TargetPos[nHand][nAxis];
						}
					} 
					else
					{
						dPos = m_stVATZFocusInfoGui.dPlateJIG_TargetPos[nHand][nAxis];
					}
					

					CString strPos = _T("");
					strPos.Format("%.3f", dPos / 1000.0);
					int nCol = nAxis + 1;
					int nRow = nHand + 1;

					m_ssPlateJigTargetPos.SetTextMatrix(nRow, nCol, strPos);

					//Diff에서 Limit 상관 없음 
					if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) 
					{
						//Pos/Neg Limit에 따른 글자 색상 변경.
						BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
						OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
						m_ssPlateJigTargetPos.SetForeColor(color);
					}
					else
					{
						//Pos/Neg Limit에 따른 글자 색상 변경.
						m_ssPlateJigTargetPos.SetForeColor(NORMAL_COLOR_BLACK);
					}
				}
			}
		}break;
		case _eZFocusSS_PlateJigVacPos:
		{
			for (auto nHand : m_vAllHand)
			{
				for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
				{
					double dPos = 0.0;
					if (bMakeBasePos == FALSE)
					{
						if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) {
							dPos = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis];
						}
						else { //nSSDisplayType == eVAT_SSDISPLAYTYPE_DIFFERENCE
							   //변경된 Data - 이전 Data
							dPos = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis] - g_VATCommonData.m_stVATZFocusInfoBuff.dPlateJIG_VacuumPos[nHand][nAxis];
						}
					}
					else
					{
						dPos = m_stVATZFocusInfoGui.dPlateJIG_VacuumPos[nHand][nAxis];
					}

					CString strPos = _T("");
					strPos.Format("%.3f", dPos / 1000.0);
					int nCol = nAxis + 1;
					int nRow = nHand + 1;

					m_ssPlateJigVacPos.SetTextMatrix(nRow, nCol, strPos);

					//Diff에서 Limit 상관 없음 
					if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA)
					{
						//Pos/Neg Limit에 따른 글자 색상 변경.
						BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
						OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
						m_ssPlateJigVacPos.SetForeColor(color);
					}
					else
					{
						//Pos/Neg Limit에 따른 글자 색상 변경.
						m_ssPlateJigVacPos.SetForeColor(NORMAL_COLOR_BLACK);
					}
				}
			}
		}break;
		default:
			break;
	}
}

int CVATMain_ZAutoFocus::CheckPosLimit(int nZFocusSSType)
{
	switch (nZFocusSSType)
	{
	case _eZFocusSS_PlateJigTargetPos:
	{
		for (auto nHand : m_vAllHand)
		{
			for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
			{
				double dPos = 0.0;
				dPos = m_stVATZFocusInfoGui.dPlateJIG_TargetPos[nHand][nAxis];
				
				//Pos/Neg Limit Over 시 TRUE 반환
				BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
				if (bIsLimitPos == TRUE)
				{
					return TRUE;
				}
			}
		}
	}break;
	case _eZFocusSS_PlateJigVacPos:
	{
		for (auto nHand : m_vAllHand)
		{
			for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
			{
				double dPos = 0.0;
				dPos = m_stVATZFocusInfoGui.dPlateJIG_VacuumPos[nHand][nAxis];

				//Pos/Neg Limit Over 시 TRUE 반환
				BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
				OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
				if (bIsLimitPos == TRUE)
				{
					return TRUE;
				}
			}
		}
	}break;
	default:
		break;
	}
	return FALSE;
}

void CVATMain_ZAutoFocus::EnableControls(BOOL bStatus)
{
	//Select Hand
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_ssSelectSheet[nHand].EnableWindow(bStatus);
	}

	//Spread Sheet
	m_ssPlateJigTargetPos.EnableWindow(bStatus);
	m_ssPlateJigVacPos.EnableWindow(bStatus);

	//Oper Button
	m_BtnOper1_EncoderRead.EnableWindow(bStatus);		m_BtnOper2_EncoderRead.EnableWindow(bStatus);
	m_BtnOper1_MotorMove.EnableWindow(bStatus);			m_BtnOper2_MotorMove.EnableWindow(bStatus);
	m_BtnOper1_Reload.EnableWindow(bStatus);			m_BtnOper2_Reload.EnableWindow(bStatus);
	m_BtnOper1_Save.EnableWindow(bStatus);				m_BtnOper2_Save.EnableWindow(bStatus);
	m_BtnOper1_MakeBasePos.EnableWindow(bStatus);		m_BtnOper2_MakeBasePos.EnableWindow(bStatus);
}



void CVATMain_ZAutoFocus::ClickSspanelZfocusTargetPosData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	//Set Display Type
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);
}


void CVATMain_ZAutoFocus::ClickSspanelZfocusTargetPosDataDiff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Set Display Type
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DIFFERENCE;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);
}


void CVATMain_ZAutoFocus::ClickSspanelZfocusVacuumPosData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Set Display Type
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS1_DisplayType);
}


void CVATMain_ZAutoFocus::ClickSspanelZfocusVacuumPosDataDiff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DIFFERENCE;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, eVAT_SSDISPLAYTYPE_DIFFERENCE);

	//Display Data
	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS2_DisplayType);
}


void CVATMain_ZAutoFocus::ClickSpreadZfocusTargetPosition(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_UpdateSheetCellColor(m_ssPlateJigTargetPos, m_stTargetPosSheetLastInfo, Col, Row);
	_Com_UpdateLastSheetData(m_stTargetPosSheetLastInfo, Col, Row);
	_Com_UpdateLastHandData(m_stTargetPosSheetLastInfo, Row - 1);
}

void CVATMain_ZAutoFocus::DblClickSpreadZfocusTargetPosition(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (_Com_SSInterlock_DblClick(Col, Row, m_nSS1_DisplayType) != eVAT_INTERLOCK_PASS)
		return;


	_Com_DblClickSheetTextChange(m_ssPlateJigTargetPos, m_stTargetPosSheetLastInfo.nHand, this, Col, Row);
	
	int nHand = Row - 1;
	int nAxis = Col - 1;

	CString strCurValue = m_ssPlateJigTargetPos.GetTextMatrix(Row, Col);
	m_stVATZFocusInfoGui.dPlateJIG_TargetPos[nHand][nAxis] = atof(strCurValue) * 1000.0;
}


void CVATMain_ZAutoFocus::ClickSpreadZfocusVacuumPosition(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_UpdateSheetCellColor(m_ssPlateJigVacPos, m_stPlateJigVacPosSheetLastInfo, Col, Row);
	_Com_UpdateLastSheetData(m_stPlateJigVacPosSheetLastInfo, Col, Row);
	_Com_UpdateLastHandData(m_stPlateJigVacPosSheetLastInfo, Row - 1);
}


void CVATMain_ZAutoFocus::DblClickSpreadZfocusVacuumPosition(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (_Com_SSInterlock_DblClick(Col, Row, m_nSS2_DisplayType) != eVAT_INTERLOCK_PASS)
		return;

	_Com_DblClickSheetTextChange(m_ssPlateJigVacPos, m_stPlateJigVacPosSheetLastInfo.nHand, this, Col, Row);

	//Data Buff에 Update.
	int nHand = Row - 1;
	int nAxis = Col - 1;

	CString strCurValue = m_ssPlateJigVacPos.GetTextMatrix(Row, Col);
	m_stVATZFocusInfoGui.dPlateJIG_VacuumPos[nHand][nAxis] = atof(strCurValue) * 1000.0;
}


void CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusLoadTraypp(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TRAYPP, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusUnloadTraypp(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TRAYPP, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusLoadTestpp1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TESTPP1, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusUnloadTestpp1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TESTPP1, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusLoadTestpp2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TESTPP2, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_ZAutoFocus::ButtonClickedSpreadZfocusUnloadTestpp2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TESTPP2, eVAT_SSTYPE_HAND, m_editPosition);
}

void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusEncoderRead1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dCurPos[VAT_MAX_AXIS_XYZ] = { 0.0, };

	int nRet = _Com_Oper_EncoderRead(dCurPos, m_stTargetPosSheetLastInfo, m_nSS1_DisplayType/*Interlock Check용*/);
	if (nRet == ERR_VAT_ERROR)
		return;

	for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
	{		
		//Spread Sheet 표기.
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", dCurPos[nAxis] / 1000.0);

		int nCol = nAxis + 1;
		m_ssPlateJigTargetPos.SetTextMatrix(m_stTargetPosSheetLastInfo.nRow, nCol, strCurPos); // 

		//Data Update
		m_stVATZFocusInfoGui.dPlateJIG_TargetPos[m_stTargetPosSheetLastInfo.nHand][nAxis] = dCurPos[nAxis];
	}
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMotorMove1()
{
	//Manual Cmd Move 모듈 함수 사용.
	_Com_ManualMove(VAT_SEQ_MODE_Z_FOCUS, m_ssPlateJigTargetPos, m_stTargetPosSheetLastInfo, m_nSS1_DisplayType/*Interlock Check용*/);
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusReload1()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Reload?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_BASE_POSITION_RELOAD, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//REAL -> GUI 
	COPY_VAT_ARRAY(m_stVATZFocusInfoGui.dPlateJIG_TargetPos, g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos);

	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssPlateJigTargetPos, m_stTargetPosSheetLastInfo);
}

/*========================================================================*/
//COMMENT :	GUI Data를 Buffer Data에 Update.
//          ※GUI Data Update 시점.
//             -. Read Encoder 에서 GUI Data Update 진행.
//             -. DblClick시 GUI Data Update 진행.
/*========================================================================*/
void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusDataSave1()
{	
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	int nLimitCheck = CheckPosLimit(eZFocus_PlateJigTargetPos);
	if (nLimitCheck == TRUE)
	{
		int nLimitCheckRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_LIMIT_OVER_CHECK, ERR_VAT_TYPE_YESNO);
		if (nLimitCheckRet == IDNO)
		{
			return;
		}
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {	
			//GUI <-> Real 다른 값만 Buffer에 저장.(작업자 수정 되었다는 의미)
			//※저장 시, 마다 Buffer에 저장할 경우 이전과 차이 Data가 0으로 변경 되는 문제.
			double dRealData = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis];
			double dGuiData = m_stVATZFocusInfoGui.dPlateJIG_TargetPos[nHand][nAxis];
			if (dRealData != dGuiData) {
				// Copy 순서 변경하지 말것. [9/15/2025 dohyeong.kim]
				//1. [REAL -> BUFFER]
				g_VATCommonData.m_stVATZFocusInfoBuff.dPlateJIG_TargetPos[nHand][nAxis] = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis];

				//2. [GUI -> REAL]
				g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis] = m_stVATZFocusInfoGui.dPlateJIG_TargetPos[nHand][nAxis];

				//3. [ini File 저장]
				g_VATCommonData.SaveEachZAutoFocusInformation(eZFocus_PlateJigTargetPos, nHand, nAxis);
			}
		}
	}

	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssPlateJigTargetPos, m_stTargetPosSheetLastInfo);
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusEncoderRead2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dCurPos[VAT_MAX_AXIS_XYZ] = { 0.0, };

	int nRet = _Com_Oper_EncoderRead(dCurPos, m_stPlateJigVacPosSheetLastInfo, m_nSS2_DisplayType/*Interlock Check용*/);
	if (nRet == ERR_VAT_ERROR)
		return;

	for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
	{
		//Spread Sheet 표기.
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", dCurPos[nAxis] / 1000.0);

		int nCol = nAxis + 1;
		m_ssPlateJigVacPos.SetTextMatrix(m_stPlateJigVacPosSheetLastInfo.nRow, nCol, strCurPos);

		//Data Update
		m_stVATZFocusInfoGui.dPlateJIG_VacuumPos[m_stPlateJigVacPosSheetLastInfo.nHand][nAxis] = dCurPos[nAxis];
	}
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMotorMove2()
{
	//Manual Cmd Move 모듈 함수 사용.
	_Com_ManualMove(VAT_SEQ_MODE_Z_FOCUS, m_ssPlateJigVacPos, m_stPlateJigVacPosSheetLastInfo, m_nSS2_DisplayType/*Interlock Check용*/);
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusReload2()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Reload?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_BASE_POSITION_RELOAD, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//REAL -> GUI 
	COPY_VAT_ARRAY(m_stVATZFocusInfoGui.dPlateJIG_VacuumPos, g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos);

	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS2_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssPlateJigVacPos, m_stPlateJigVacPosSheetLastInfo);
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusDataSave2()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	int nLimitCheck = CheckPosLimit(eZFocus_PlateJigVacPos);
	if (nLimitCheck == TRUE)
	{
		int nLimitCheckRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_LIMIT_OVER_CHECK, ERR_VAT_TYPE_YESNO);
		if (nLimitCheckRet == IDNO)
		{
			return;
		}
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			//GUI <-> Real 다른 값만 Buffer에 저장.(작업자 수정 되었다는 의미)
			double dRealData = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis];
			double dGuiData = m_stVATZFocusInfoGui.dPlateJIG_VacuumPos[nHand][nAxis];
			if (dRealData != dGuiData) {
				// Copy 순서 변경하지 말것. [9/15/2025 dohyeong.kim]
				//1. [REAL -> BUFFER]
				g_VATCommonData.m_stVATZFocusInfoBuff.dPlateJIG_VacuumPos[nHand][nAxis] = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis];

				//2. [GUI -> REAL]
				g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis] = m_stVATZFocusInfoGui.dPlateJIG_VacuumPos[nHand][nAxis];

				//3. [ini File 저장]
				g_VATCommonData.SaveEachZAutoFocusInformation(eZFocus_PlateJigVacPos, nHand, nAxis);
			}
		}
	}

	//ini File 저장.
// 	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
// 		g_VATCommonData.SaveEachZAutoFocusInformation(eZFocus_PlateJigVacPos, nHand);
// 	}

	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS2_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssPlateJigVacPos, m_stPlateJigVacPosSheetLastInfo);
}


//===============Measure Overview==============

//============================================================================================================
//COMMENT : Hand Status Update 함수. 
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Hand Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//			 ※ 멤버 변수에만 저장 후, 진행 상태 Timer에서 표기.
//              -. 진행 상태 OnGoing의 경우, 깜빡임 표기 하기 위함.
//============================================================================================================
LRESULT CVATMain_ZAutoFocus::OnReceiveUpdateHandStatus(WPARAM wParam, LPARAM lParam)
{
	int nHand = (int)wParam;

	if (!(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2))
		return 0L;

	m_nGuiHandStatus[nHand] = GuiNotifier->GetHandStatus(nHand);

	return 0L;
}

//============================================================================================================
//COMMENT : 모든 Hand 측정에 대한 진행률 상태 Update 함수.(Progress) 
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Hand Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//============================================================================================================
LRESULT CVATMain_ZAutoFocus::OnReceiveUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	int nHand = (int)wParam;

	if (!(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2))
		return 0L;

	double dProgressPercent = GuiNotifier->GetProgressPercent(nHand);
	
	m_Progress[nHand].SetPos(static_cast<int>(dProgressPercent));

	CString strPercent;
	strPercent.Format(_T("%.0f%c"), dProgressPercent, _T('%'));
	m_Percent[nHand].SetWindowText(strPercent);

	return 0L;
}

//============================================================================================================
//COMMENT : Vision Cam Status Update 함수.
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//============================================================================================================
LRESULT CVATMain_ZAutoFocus::OnReceiveUpdateVisionStatus(WPARAM wParam, LPARAM lParam)
{
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;

	int nStatus = GuiNotifier->GetVisionStatus(nCam);

	int nFrame = _Com_ConvertCamStatusToFrameIdx(nStatus);
	CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m1_nCamStatus[nCam]);
	chkbt->SetStatus(nFrame);

	return 0L;
}


//============================================================================================================
//COMMENT : Vision Cam Offset X/Y Update 함수.
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMain_ZAutoFocus::OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam)
{
	//CVAT_MeasureStatus::GetInstance().UpdateVisionStatus(nCam, GetReceiveState()); //Idle
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;

	std::pair<double, double> pOffset = GuiNotifier->GetVisionOffset(nCam);

	//Display Offset
	for (int nOffsetIdx = 0; nOffsetIdx < VAT_VISION_OFFSET_MAX; nOffsetIdx++) {
		CString strOffset = _T("");
		strOffset.Format("%.3f", (nOffsetIdx == VAT_VISION_OFFSET_X) ? pOffset.first / 1000.0 : pOffset.second / 1000.0 );

		CEdit *editOffset = (CEdit *)GetDlgItem(m1_nCamOffset[nCam][nOffsetIdx]);
		editOffset->SetWindowText(strOffset);
	}

	return 0L;
}


//============================================================================================================
//COMMENT : Vision Cam 측정 Count 표기
//           1. 실제 Vision Measure Cnt
//           2. Cam Auto Focus의 경우, -1로 Data를 받으며 'F'로 표기 됨.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMain_ZAutoFocus::OnReceiveUpdateVisionMeasureCnt(WPARAM wParam, LPARAM lParam)
{
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;

	int nVisionMeasureCnt = GuiNotifier->GetVisionMeasureCnt(nCam);

	CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m1_nCamStatus[nCam]);
	CString strVisionRetryCnt = _T("");
	if(nVisionMeasureCnt == VAT_VISION_MEASURE_AUTO_FOCUS){
		strVisionRetryCnt = _T("F"); //특수 case
	}
	else {
		strVisionRetryCnt.Format("%d", nVisionMeasureCnt); //일반 표기
	}

	chkbt->SetWindowText(strVisionRetryCnt);

	return 0L;
}

//============================================================================================================
//COMMENT : 측정 완료 된, Data Update 진행.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMain_ZAutoFocus::OnReceiveUpdateSavedData(WPARAM wParam, LPARAM lParam)
{
	//[Real -> GUI Update]
	//1. Plate Jig Target Position
	COPY_VAT_ARRAY(m_stVATZFocusInfoGui.dPlateJIG_TargetPos, g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos);
	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS1_DisplayType);

	//2. Plate Jig Vacuum Position
	COPY_VAT_ARRAY(m_stVATZFocusInfoGui.dPlateJIG_VacuumPos, g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_VacuumPos);
	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS2_DisplayType);

// 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
// 		m_nHandStatus[nHand] = VAT_STATUS_MEASURE_INIT;
// 	}
	
	return 0L;
}

void CVATMain_ZAutoFocus::OnEnSetFocusCursorBlock(UINT nID)
{
	this->SetFocus();
// 	if (GetParent()) 
// 		GetParent()->SetFocus();
}

void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMakeBasePos1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRet = _Com_Oper_MakeBasePos(m_nSS1_DisplayType/*Interlock Check용*/);
	if (nRet == ERR_VAT_ERROR)
		return;

	double dPlateJIG_TargetPos[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ] = { 0 };
	g_DBVAT.SELECT_VAT_Z_AUTO_FOCUS(dPlateJIG_TargetPos, _eZFocusSS_PlateJigTargetPos);

	COPY_VAT_ARRAY(m_stVATZFocusInfoGui.dPlateJIG_TargetPos, dPlateJIG_TargetPos);

	DisplaySSData(_eZFocusSS_PlateJigTargetPos, m_nSS2_DisplayType, TRUE);
}


void CVATMain_ZAutoFocus::OnBnClickedBtnZfocusMakeBasePos2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nRet = _Com_Oper_MakeBasePos(m_nSS2_DisplayType/*Interlock Check용*/);
	if (nRet == ERR_VAT_ERROR)
		return;

	double dPlateJIG_TargetPos[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ] = { 0 };
	g_DBVAT.SELECT_VAT_Z_AUTO_FOCUS(dPlateJIG_TargetPos, _eZFocusSS_PlateJigVacPos);

	COPY_VAT_ARRAY(m_stVATZFocusInfoGui.dPlateJIG_VacuumPos, dPlateJIG_TargetPos);

	DisplaySSData(_eZFocusSS_PlateJigVacPos, m_nSS2_DisplayType, TRUE);
}
