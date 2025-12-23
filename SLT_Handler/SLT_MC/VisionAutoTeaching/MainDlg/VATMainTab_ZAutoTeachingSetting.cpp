// VATMainTab_ZAutoTeachingSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMainTab_ZAutoTeachingSetting.h"
#include "afxdialogex.h"


// CVATMainTab_ZAutoTeachingSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMainTab_ZAutoTeachingSetting, CDialogEx)

CVATMainTab_ZAutoTeachingSetting::CVATMainTab_ZAutoTeachingSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_Z_AUTO_TEACHING_SETTING_DLG, pParent)
{
	for (int nIdx = 0; nIdx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; nIdx++) {
		m_dDesignValue[nIdx] = 0.0;
	}

	m_pChildDlgJog = NULL;

	m_bBlockEditFocus = FALSE;
}

CVATMainTab_ZAutoTeachingSetting::~CVATMainTab_ZAutoTeachingSetting()
{
	if (m_pChildDlgJog != NULL)
	{
		m_pChildDlgJog->DestroyWindow();
		delete m_pChildDlgJog;
		m_pChildDlgJog = NULL;
	}
}

void CVATMainTab_ZAutoTeachingSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_HAND_1, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_HAND_2, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_HAND_3, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_HAND_4, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_HAND_5, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_HAND_6, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	DDX_Control(pDX, IDC_SPREAD_Z_TEACHING_BASE_POSITION, m_ssZTeachBasePos);
	
	//Operate
	DDX_Control(pDX, IDC_BTN_Z_TEACHING_BASE_POS_ENCODER_READ, m_BtnZTeachBasePos_EncoderRead);
	DDX_Control(pDX, IDC_BTN_Z_TEACHING_BASE_POS_MOTOR_MOVE, m_BtnZTeachBasePos_MotorMove);
	DDX_Control(pDX, IDC_BTN_Z_TEACHING_BASE_POS_RELOAD, m_BtnZTeachBasePos_Reload);
	DDX_Control(pDX, IDC_BTN_Z_TEACHING_BASE_POS_DATA_SAVE, m_BtnZTeachBasePos_Save);

	DDX_Control(pDX, IDC_BTN_Z_TEACHING_MAKE_BASE_POS, m_BtnMakeBasePos);
	

	//=========================================================================================================Pick/Place Offset
// 	for (int nHand= VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
// 		DDX_Text(pDX, IDC_EDIT_Z_PICK_OFFSET1 + nHand, m_dZPickOffset[nHand]);
// 		DDX_Text(pDX, IDC_EDIT_Z_PLACE_OFFSET1 + nHand, m_dZPlaceOffset[nHand]);
// 	}

	DDX_Control(pDX, IDC_SPREAD_Z_TEACH_PICK_PLACE_OFFSET, m_ssZPickPlaceOffset);

	DDX_Control(pDX, IDC_BTN_Z_TEACH_PARAM_SET_SAVE_1, m_BtnParamSet_Save1);
	

	//=========================================================================================================Design
 	for (int nIdx = 0; nIdx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; nIdx++) {
 		DDX_Text(pDX, m4_nDesignValue[nIdx], m_dDesignValue[nIdx]);
 	}

	DDX_Control(pDX, IDC_BTN_Z_TEACH_PARAM_SET_SAVE_2, m_BtnParamSet_Save2);

}



BOOL CVATMainTab_ZAutoTeachingSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Data Load
	//InitDataLoad();

	//Z Base Position 
	DataCopyZBasePos_RealToGui(VAT_HAND_MAX);

	//Base Position Hand 선택 초기화.
	_Com_InitHandSelectSheet();

	//Spread Sheet Oper Btn 초기화.
	InitZTeachBasePosOperBtn();

	//Hand 선택 색상 초기 Setting(Tray Load Hand)
	_Com_ButtonClickedSpreadHand(1, 1, VAT_HAND_LOAD_TRAYPP, eVAT_SSTYPE_PART/*, m_editPosition*/);

	//Part Spread Sheet 초기 Setting(Tray Load Hand)
	_Com_InitPosSpreadSheet_PartType(m_ssZTeachBasePos, VAT_HAND_LOAD_TRAYPP);

	//Spread Sheet LastInfo Type & Hand 초기화.
	m_stZTeachBasePosLastInfo.nSSType = eVAT_SSTYPE_PART;
	m_stZTeachBasePosLastInfo.nHand = VAT_HAND_LOAD_TRAYPP;

	//Spread Sheet Panel 초기화.(고정 Panel)
	InitSSConstPanel();

	//초기 Data Display(VAT_HAND_LOAD_TRAYPP, eVAT_SSDISPLAYTYPE_DATA)
	DisplaySSData(m_stZTeachBasePosLastInfo.nHand);

	//Cell Block
	_Com_BlockCell(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo.nHand);

	//Control 활성화.
	EnableControls_SettingTab(TRUE);
	//=========================================================================================================Parameter Setting


	InitStaticValue(); //변수에 값 저장.
	InitParamSetBtn();

	//Design Value
	InitStaticCtrl_DesignValue();
	DisplayDesignValue();

	//Z Offset
	m_stZPickPlaceOffsetLastInfo.nSSType = eVAT_SSTYPE_NORMAL;
	m_stZPickPlaceOffsetLastInfo.nHand = VAT_HAND_LOAD_TRAYPP;
	InitZOffsetSpreadSheet(m_stZPickPlaceOffsetLastInfo.nHand);
	DisplayZOffsetData(m_stZPickPlaceOffsetLastInfo.nHand);
	

	//UpdateData(TRUE); //controll -> 변수

	//=========================================================================================================JOG
	InitMotorJog();

	//=========================================================================================================Timer
	//PageMoveInit_Tab에서 해줌.
	//SetTimer(_eZTeachSettingT_CheckExecuteStatus, 200, NULL);

	return TRUE;
}

void CVATMainTab_ZAutoTeachingSetting::OnTimer(UINT_PTR nIDEvent)
{

 	switch (nIDEvent)
 	{
 	case _eZTeachSettingT_CheckExecuteStatus:
 	{
 		KillTimer(nIDEvent);
 
		//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
		if (g_bVATStatusExecute != m_bLastExecuteStatus)
		{
			EnableControls_SettingTab(!g_bVATStatusExecute);
			m_bLastExecuteStatus = g_bVATStatusExecute;
		}

 
 		SetTimer(nIDEvent, 200, NULL);
 	}break;
 	default:
 		break;
 	}


	CDialogEx::OnTimer(nIDEvent);
}

void CVATMainTab_ZAutoTeachingSetting::OnDestroy()
{
	for (int i = 0; i < _eZTeachSettingT_Max; i++) {
		KillTimer(i);
	}

	CDialogEx::OnDestroy();
}


BEGIN_MESSAGE_MAP(CVATMainTab_ZAutoTeachingSetting, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_Z_TEACHING_BASE_POS_ENCODER_READ, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosEncoderRead)
	ON_BN_CLICKED(IDC_BTN_Z_TEACHING_BASE_POS_MOTOR_MOVE, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosMotorMove)
	ON_BN_CLICKED(IDC_BTN_Z_TEACHING_BASE_POS_RELOAD, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosReload)
	ON_BN_CLICKED(IDC_BTN_Z_TEACHING_BASE_POS_DATA_SAVE, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosDataSave)

	ON_BN_CLICKED(IDC_BTN_Z_TEACHING_MAKE_BASE_POS, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingMakeBasePos)
	//ON_EN_SETFOCUS(IDC_EDIT_Z_PICK_OFFSET, &CVATMainTab_ZAutoTeachingSetting::OnEnSetfocusEditZPickOffset)
	//ON_EN_SETFOCUS(IDC_EDIT_Z_PLACE_OFFSET, &CVATMainTab_ZAutoTeachingSetting::OnEnSetfocusEditZPlaceOffset)
	//ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_Z_PICK_OFFSET1, IDC_EDIT_Z_PICK_OFFSET6, &CVATMainTab_ZAutoTeachingSetting::OnEnSetfocusEditZPickOffset)
	//ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_Z_PLACE_OFFSET1, IDC_EDIT_Z_PLACE_OFFSET6, &CVATMainTab_ZAutoTeachingSetting::OnEnSetfocusEditZPlaceOffset)
	ON_BN_CLICKED(IDC_BTN_Z_TEACH_PARAM_SET_SAVE_1, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachParamSetSave1)

	//Design Value (EN_SETFOCUS)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_TBL_HEIGHT, IDC_EDIT_DEVICE_HEIGHT, &CVATMainTab_ZAutoTeachingSetting::OnEnSetFocusDesignValue)

	ON_BN_CLICKED(IDC_BTN_Z_TEACH_PARAM_SET_SAVE_2, &CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachParamSetSave2)
	

END_MESSAGE_MAP()


// CVATMainTab_ZAutoTeachingSetting 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CVATMainTab_ZAutoTeachingSetting, CDialogEx)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_HAND_1, 3, CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_HAND_2, 3, CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand2, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_HAND_3, 3, CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand3, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_HAND_4, 3, CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand4, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_HAND_5, 3, CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand5, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_HAND_6, 3, CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand6, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_BASE_POSITION, 5, CVATMainTab_ZAutoTeachingSetting::ClickSpreadZTeachingBasePosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACHING_BASE_POSITION, 11, CVATMainTab_ZAutoTeachingSetting::DblClickSpreadZTeachingBasePosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACH_PICK_PLACE_OFFSET, 5, CVATMainTab_ZAutoTeachingSetting::ClickSpreadZTeachPickPlaceOffset, VTS_I4 VTS_I4)
	ON_EVENT(CVATMainTab_ZAutoTeachingSetting, IDC_SPREAD_Z_TEACH_PICK_PLACE_OFFSET, 11, CVATMainTab_ZAutoTeachingSetting::DblClickSpreadZTeachPickPlaceOffset, VTS_I4 VTS_I4)
	
END_EVENTSINK_MAP()


void CVATMainTab_ZAutoTeachingSetting::EnableControls_SettingTab(BOOL bStatus)
{
	//Control Enable/Disable 처리 시, Focus가 활성화 되는 경우 발생.
	m_bBlockEditFocus = TRUE;

	//Design Value - eTABLE_HEIGHT ~ eDEVICE_HEIGHT
	for (int idx = VAT_TABLE_HEIGHT; idx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; idx++) {
		GetDlgItem(m4_nDesignValue[idx])->EnableWindow(bStatus);
	}

	//Button 
	GetDlgItem(IDC_BTN_Z_TEACHING_MAKE_BASE_POS)->EnableWindow(bStatus);
	GetDlgItem(IDC_BTN_Z_TEACHING_BASE_POS_ENCODER_READ)->EnableWindow(bStatus);
	GetDlgItem(IDC_BTN_Z_TEACHING_BASE_POS_MOTOR_MOVE)->EnableWindow(bStatus);
	GetDlgItem(IDC_BTN_Z_TEACHING_BASE_POS_RELOAD)->EnableWindow(bStatus);
	GetDlgItem(IDC_BTN_Z_TEACHING_BASE_POS_DATA_SAVE)->EnableWindow(bStatus);
	GetDlgItem(IDC_BTN_Z_TEACH_PARAM_SET_SAVE_1)->EnableWindow(bStatus);
	GetDlgItem(IDC_BTN_Z_TEACH_PARAM_SET_SAVE_2)->EnableWindow(bStatus);

	//Spread Sheet
	GetDlgItem(IDC_SPREAD_Z_TEACHING_BASE_POSITION)->EnableWindow(bStatus);
	GetDlgItem(IDC_SPREAD_Z_TEACH_PICK_PLACE_OFFSET)->EnableWindow(bStatus);

	//this->SetFocus();

	m_bBlockEditFocus = FALSE;
}


//===========================================================
//COMMENT : 해당 Mode로 변경 시, 초기화 필요 사항.
//===========================================================
void CVATMainTab_ZAutoTeachingSetting::PageMoveInit_Tab()
{
	if (m_pChildDlgJog != NULL) {
		m_pChildDlgJog->InitHandChange(m_stZTeachBasePosLastInfo.nHand);
	}

	for (int i = 0; i < _eZTeachSettingT_Max; i++) {
		SetTimer(i, 200, NULL);
	}

	//Data Load
	m_stVATZTeachBasePosGui = g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos;

	//초기 Data Display(VAT_HAND_LOAD_TRAYPP, eVAT_SSDISPLAYTYPE_DATA)
	DisplaySSData(m_stZTeachBasePosLastInfo.nHand);

	//Cell Block
	_Com_BlockCell(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo.nHand);
}


//===========================================================
//COMMENT : 다른 Mode 변경 시, 불필요 Check 사항 제거.
//===========================================================
void CVATMainTab_ZAutoTeachingSetting::PageMoveDispose_Tab()
{
	if (m_pChildDlgJog != NULL) {
		m_pChildDlgJog->Kill_Timer();
	}
}


void CVATMainTab_ZAutoTeachingSetting::InitDataLoad()
{
	//Z Base Position 
	DataCopyZBasePos_RealToGui(VAT_HAND_MAX);
}


void CVATMainTab_ZAutoTeachingSetting::InitZTeachBasePosOperBtn()
{
	//Z Teach Base Position Oper Btn
	CShadeButtonST *btnOper[eSSOper_MAX] = { &m_BtnZTeachBasePos_EncoderRead, &m_BtnZTeachBasePos_MotorMove, &m_BtnZTeachBasePos_Reload, &m_BtnZTeachBasePos_Save, &m_BtnMakeBasePos };
	for (int nType = 0; nType < eSSOper_MAX; nType++) {
		_Com_InitOperBtn(*btnOper[nType], nType);
	}

	//Make Base Position Btn
// 	CFont BtnFont;
// 	BtnFont.CreatePointFont(140, _T("Arial"));
// 
// 	m_BtnMakeBasePos.SetFont(&BtnFont);
// 	m_BtnMakeBasePos.SetWindowText(_T("MAKE BASE POS"));
// 	m_BtnMakeBasePos.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
// 	m_BtnMakeBasePos.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_BtnMakeBasePos.SetIcon(IDI_BTN_CLEAR);

}


void CVATMainTab_ZAutoTeachingSetting::InitSSConstPanel()
{

}






void CVATMainTab_ZAutoTeachingSetting::DisplaySSData(int nHand, BOOL bMakeBasePos/* = FALSE*/)
{
	int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
	for (int nPart = 0; nPart < nMaxPart; nPart++) {
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			double dPos = 0.0;

			if (bMakeBasePos == FALSE)
			{
				dPos = g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.getPartBasePos(nHand, nPart, nAxis);
			}
			else
			{
				dPos = m_stVATZTeachBasePosGui.getPartBasePos(nHand, nPart, nAxis);
			}

			int nCol = nAxis + 1;
			int nRow = nPart + 1;

			CString strPos = _T("");
			strPos.Format("%.3f", dPos / 1000.0);
			m_ssZTeachBasePos.SetTextMatrix(nRow, nCol, strPos);

			//Pos/Neg Limit에 따른 글자 색상 변경.
			BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
			//OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
			OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
			m_ssZTeachBasePos.SetForeColor(color);
		}
	}
}

int CVATMainTab_ZAutoTeachingSetting::CheckPosLimit(int nHand)
{
	int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
	for (int nPart = 0; nPart < nMaxPart; nPart++) {
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			double dPos = 0.0;

			dPos = m_stVATZTeachBasePosGui.getPartBasePos(nHand, nPart, nAxis);

			//Pos/Neg Limit에 따른 글자 색상 변경.
			BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
			if (bIsLimitPos == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


void CVATMainTab_ZAutoTeachingSetting::ButtonClickHandChange(long Col, long Row, int nHand)
{
	//=========================================================================================Base Position(spread sheet)
	//Hand Color & Select/Unselect 변경.
	_Com_ButtonClickedSpreadHand(Col, Row, nHand, eVAT_SSTYPE_PART/*, m_editPosition*/);

	//Base Pos Spread Sheet Display
	_Com_InitPosSpreadSheet_PartType(m_ssZTeachBasePos, nHand);

	//Hand 변경 시, Base Pos Last Info Hand 변경.
	_Com_UpdateLastHandData(m_stZTeachBasePosLastInfo, nHand);

	//Hand 변경 시, Spread Sheet 색상 초기화 및 Last Info Data 초기화(Last Info Hand 제외).
	_Com_Clear_SpreadSheet(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo);

	//Data Real -> Gui로 저장.
	DataCopyZBasePos_RealToGui(nHand);

	//Display Data
	DisplaySSData(nHand);

	//Cell Block
	_Com_BlockCell(m_ssZTeachBasePos, nHand);

	//=========================================================================================Z Offset(spread sheet)
	//Hand 변경 시, Z Offset Last Info Hand 변경.
	_Com_UpdateLastHandData(m_stZPickPlaceOffsetLastInfo, nHand);

	//Hand 변경 시, Spread Sheet 색상 초기화 및 Last Info Data 초기화(Last Info Hand 제외).
	_Com_Clear_SpreadSheet(m_ssZPickPlaceOffset, m_stZPickPlaceOffsetLastInfo);

	InitZOffsetSpreadSheet(nHand);

	DisplayZOffsetData(nHand);

	//========================================================================================Jog Control Hand 변경.
	if (m_pChildDlgJog != NULL) {
		m_pChildDlgJog->InitHandChange(nHand);
	}
}



void CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand1(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange(Col, Row, VAT_HAND_LOAD_TRAYPP);
}


void CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand2(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange(Col, Row, VAT_HAND_UNLOAD_TRAYPP);
}


void CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand3(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange(Col, Row, VAT_HAND_LOAD_TESTPP1);
}


void CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand4(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange(Col, Row, VAT_HAND_UNLOAD_TESTPP1);
}


void CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand5(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange(Col, Row, VAT_HAND_LOAD_TESTPP2);
}


void CVATMainTab_ZAutoTeachingSetting::ButtonClickedSpreadZAutoTeachingHand6(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange(Col, Row, VAT_HAND_UNLOAD_TESTPP2);
}





void CVATMainTab_ZAutoTeachingSetting::ClickSpreadZTeachingBasePosition(long Col, long Row)
{
	BOOL bIsBlockCell = _Com_CheckBlockCell(m_stZTeachBasePosLastInfo.nHand, Col);
	if (bIsBlockCell == TRUE)
		return;

	_Com_UpdateSheetCellColor(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo, Col, Row);
	_Com_UpdateLastSheetData(m_stZTeachBasePosLastInfo, Col, Row);
}


void CVATMainTab_ZAutoTeachingSetting::DblClickSpreadZTeachingBasePosition(long Col, long Row)
{
	if (_Com_SSInterlock_DblClick(Col, Row, eVAT_SSDISPLAYTYPE_DATA) != eVAT_INTERLOCK_PASS) //Z Base Pos ssType은 Data밖에 없음.
		return;

	BOOL bIsBlockCell = _Com_CheckBlockCell(m_stZTeachBasePosLastInfo.nHand, Col);
	if (bIsBlockCell == TRUE)
		return;

	_Com_DblClickSheetTextChange(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo.nHand, this, Col, Row);

	CString strCurValue = m_ssZTeachBasePos.GetTextMatrix(Row, Col);

	int nPart = Row - 1;
	int nAxis = Col - 1;
	double dPos = atof(strCurValue) * 1000.0;

	m_stVATZTeachBasePosGui.setPartBasePos(m_stZTeachBasePosLastInfo.nHand, nPart, nAxis, dPos); //hand, part, axis, pos
}


void CVATMainTab_ZAutoTeachingSetting::ZTeachingMakeBasePos(double dPos[], int nHand, int nPart)
{
	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			//Hand Y축이 없어 Table/Feeder가 이동함.
			if (nPart == VAT_IDX_TRAY_PP_FEEDER) {
				dPos[VAT_X] += 3000.0;
			}

			dPos[VAT_Z] += 10000.0;
			return;
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			if (VAT_IDX_TRAY_PP_TBL_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_TBL_4) {
				double tbl_y_half_pitch = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 2.0;
				dPos[VAT_Y] -= tbl_y_half_pitch;
				dPos[VAT_Z] += 5000.0;
			}
			else { //Stacker
				dPos[VAT_X] += 3000.0;
				dPos[VAT_Y] += 0.0;
				dPos[VAT_Z] += 10000.0;
			}
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP1:
		{
			if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4) {
				if (nHand == VAT_HAND_LOAD_TESTPP1 && nPart == VAT_IDX_STATION_4) {
					dPos[VAT_X] -= 20000.0;
					dPos[VAT_Y] -= 17500.0;
				}
				else {
					dPos[VAT_X] += 20000.0;
					dPos[VAT_Y] += 17500.0;
				}
			}
			else if (VAT_IDX_TEST_PP_TBL_1_LD <= nPart && nPart <= VAT_IDX_TEST_PP_TBL_4_UD) {
				double tbl_y_half_pitch = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 2.0;
				dPos[VAT_X] -= tbl_y_half_pitch;
			}
			else { //clean table
				double tbl_y_half_pitch = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_Y) / 2.0;
				dPos[VAT_X] -= tbl_y_half_pitch;
				dPos[VAT_Y] += 0.0;
			}

			dPos[VAT_Z] += 5000.0;
		}break;
		case VAT_HAND_LOAD_TESTPP2:
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4) {
				if (nHand == VAT_HAND_UNLOAD_TESTPP2 && nPart == VAT_IDX_STATION_1) {
					dPos[VAT_X] += 20000.0;
					dPos[VAT_Y] += 17500.0;
				}
				else {
					dPos[VAT_X] -= 20000.0;
					dPos[VAT_Y] -= 17500.0;
				}
			}
			else if (VAT_IDX_TEST_PP_TBL_1_LD <= nPart && nPart <= VAT_IDX_TEST_PP_TBL_4_UD) {
				double tbl_y_half_pitch = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 2.0;
				dPos[VAT_X] += tbl_y_half_pitch;
				
			}
			else { //clean table
				double tbl_y_half_pitch = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_Y) / 2.0;
				dPos[VAT_X] += tbl_y_half_pitch;
				dPos[VAT_Y] += 0.0;
			}

			dPos[VAT_Z] += 5000.0;
		}break;
		default:
			break;
	}
}


void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosEncoderRead()
{
	double dCurPos[VAT_MAX_AXIS_XYZ] = { 0.0, };

	int nRet = _Com_Oper_EncoderRead(dCurPos, m_stZTeachBasePosLastInfo, eVAT_SSDISPLAYTYPE_DATA);
	if (nRet == ERR_VAT_ERROR)
		return;

	for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XY; nAxis++)
	{
		//Spread Sheet 표기.
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", dCurPos[nAxis] / 1000.0);

		int nCol = nAxis + 1;
		m_ssZTeachBasePos.SetTextMatrix(m_stZTeachBasePosLastInfo.nRow, nCol, strCurPos);

		//Data Update
		int nHand = m_stZTeachBasePosLastInfo.nHand;
		int nPart = m_stZTeachBasePosLastInfo.nRow - 1;
		m_stVATZTeachBasePosGui.setPartBasePos(nHand, nPart, nAxis, dCurPos[nAxis]);
	}
}


void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosMotorMove()
{	

	//Manual Cmd Move 모듈 함수 사용.
	_Com_ManualMove(VAT_SEQ_MODE_Z_TEACHING, m_ssZTeachBasePos, m_stZTeachBasePosLastInfo);
}


void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosReload()
{
	//"Do you want to Base Position Reload?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_BASE_POSITION_RELOAD, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//Reload 2번누르면 none 없어짐.
	int nHand = m_stZTeachBasePosLastInfo.nHand;

	DataCopyZBasePos_RealToGui(nHand);

	DisplaySSData(nHand);

	_Com_BlockCell(m_ssZTeachBasePos, nHand);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo);
}


void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingBasePosDataSave()
{
	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	int nCurHand = m_stZTeachBasePosLastInfo.nHand;

	int nLimitCheck = CheckPosLimit(nCurHand);
	if (nLimitCheck == TRUE)
	{
		int nLimitCheckRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_LIMIT_OVER_CHECK, ERR_VAT_TYPE_YESNO);
		if (nLimitCheckRet == IDNO)
		{
			return;
		}
	}

	//현재 Hand Page의 Part 저장.
	//Gui to real
	//int nCurHand = m_stZTeachBasePosLastInfo.nHand;

	DataCopyZBasePos_GuiToReal(nCurHand);

	g_VATCommonData.SaveEachZAutoTeachingInformation(eZAutoTeaching_ZTeachBasePos, nCurHand);

	DisplaySSData(nCurHand);

	_Com_BlockCell(m_ssZTeachBasePos, nCurHand);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssZTeachBasePos, m_stZTeachBasePosLastInfo);
}


void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachingMakeBasePos()
{
	//////////////////////////////////////////////////////////////////////////// VAT_MAKEBASEPOS_CODE_START
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	int nRet = _Com_Oper_MakeBasePos(eVAT_SSDISPLAYTYPE_DATA/*Interlock Check용*/);
// 	if (nRet == ERR_VAT_ERROR)
// 		return;
// 
// 	int nHand = m_stZTeachBasePosLastInfo.nHand;
// 	
// 	double dTrayPPCurPos[VAT_MAX_TRAY_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ] = { 0 };
// 	double dTestPPCurPos[VAT_MAX_TEST_PP_LOC_COUNT][VAT_MAX_AXIS_XYZ] = { 0 };
// 	
// 	switch (nHand)
// 	{
// 	case VAT_HAND_LOAD_TRAYPP:  
// 	case VAT_HAND_UNLOAD_TRAYPP: 
// 	{ 
// 		g_DBVAT.SELECT_VAT_VISION_AUTO_TEACHING(dTrayPPCurPos, nHand);
// 	}break;
// 	case VAT_HAND_LOAD_TESTPP1:
// 	case VAT_HAND_UNLOAD_TESTPP1:
// 	case VAT_HAND_LOAD_TESTPP2:
// 	case VAT_HAND_UNLOAD_TESTPP2:
// 	{ 
// 		g_DBVAT.SELECT_VAT_VISION_AUTO_TEACHING(dTestPPCurPos, nHand);
// 	}break;
// 	}
// 	
// 	int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
// 
// 	for (int nPart = 0; nPart < nMaxPart; nPart++)
// 	{
// 		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
// 		{
// 			if (nHand == VAT_HAND_LOAD_TRAYPP && nAxis == VAT_Y) {
// 				continue;
// 			}
// 
// 			switch (nHand)
// 			{
// 			case VAT_HAND_LOAD_TRAYPP:
// 			case VAT_HAND_UNLOAD_TRAYPP:
// 			{
// 				//Data Update
// 				m_stVATZTeachBasePosGui.setPartBasePos(nHand, nPart, nAxis, dTrayPPCurPos[nPart][nAxis]);
// 			}break;
// 			case VAT_HAND_LOAD_TESTPP1:
// 			case VAT_HAND_UNLOAD_TESTPP1:
// 			case VAT_HAND_LOAD_TESTPP2:
// 			case VAT_HAND_UNLOAD_TESTPP2:
// 			{
// 				//Data Update
// 				m_stVATZTeachBasePosGui.setPartBasePos(nHand, nPart, nAxis, dTestPPCurPos[nPart][nAxis]);
// 			}break;
// 			}
// 		}		
// 	}
// 	
// 	//Display Data
// 	DisplaySSData(nHand, TRUE);
// 
// 	//Cell Block
// 	_Com_BlockCell(m_ssZTeachBasePos, nHand);
	//////////////////////////////////////////////////////////////////////////// VAT_MAKEBASEPOS_CODE_END




	//////////////////////////////////////////////////////////////////////////// ZAT_MAKEBASEPOS_CODE_START
	// int a = 0;
 	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_MAKE_BASE_POS, ERR_VAT_TYPE_YESNO);
 	if (nRet == IDNO)
 		return;
 
 	int nHand = m_stZTeachBasePosLastInfo.nHand;
 
 	int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
 	for (int nPart = 0; nPart < nMaxPart; nPart++)
 	{
 		if (_Com_IsNecessaryPart(nHand, nPart) != TRUE)
 			continue;
 
 		//Teaching Table Read
 		double dCurPos[VAT_MAX_AXIS_XYZ] = { 0.0, };
 		_Com_Oper_TeachingTableRead(dCurPos, nHand, nPart);
 
 		//Z Teaching Base Position에 맞게 Data 가공.
 		ZTeachingMakeBasePos(dCurPos, nHand, nPart);
 
 		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
 		{
			if (nHand == VAT_HAND_LOAD_TRAYPP && nAxis == VAT_Y) {
				continue;
			}

 			//Spread Sheet 표기.
 			CString strCurPos = _T("");
 			strCurPos.Format("%.3f", dCurPos[nAxis] / 1000.0);
 
 			int nCol = nAxis + 1;
 			int nRow = nPart + 1;
 			m_ssZTeachBasePos.SetTextMatrix(nRow, nCol, strCurPos);
 
 			//Data Update
 			m_stVATZTeachBasePosGui.setPartBasePos(nHand, nPart, nAxis, dCurPos[nAxis]);
 		}
 	}
//////////////////////////////////////////////////////////////////////////// ZAT_MAKEBASEPOS_CODE_END
}

void CVATMainTab_ZAutoTeachingSetting::InitStaticValue()
{
	//Z Pick/Place Offset
// 	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
// 		m_dZPickOffset[nHand] = g_VATCommonData.m_stVATZTeachConstInfo.dZPickOffset[nHand] / 1000.0;
// 		m_dZPlaceOffset[nHand] = g_VATCommonData.m_stVATZTeachConstInfo.dZPickOffset[nHand] / 1000.0;
// 	}

	//Design Value
	for (int idx = VAT_TABLE_HEIGHT; idx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; idx++) {
		m_dDesignValue[idx] = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[idx] / 1000.0;
	}
}


void CVATMainTab_ZAutoTeachingSetting::InitParamSetBtn()
{
	_Com_InitOperBtn(m_BtnParamSet_Save1, eSSOper_DATA_SAVE);
	_Com_InitOperBtn(m_BtnParamSet_Save2, eSSOper_DATA_SAVE);
}


//=====================================================================================================
// COMMENT : 
//		1) Set Max Row/Col
//			<nMaxRow> - Hand가 보유한 Unit Type	
//				LD Tray PP : Table, Feeder,(2ea)
//				UD Tray PP : Table, C-Tray,(2ea)
//				Test PP : Table, Clean Table, Station(3ea)
//			<nMaxCol> - Z Pick Offset, Z Place Offset(2ea)
//
//
//=====================================================================================================
void CVATMainTab_ZAutoTeachingSetting::InitZOffsetSpreadSheet(int nHand)
{
	//해당 Spread Sheet Cell위로 바로 Focus됨.
	m_ssZPickPlaceOffset.SetMoveActiveOnFocus(TRUE);

	m_ssZPickPlaceOffset.SetFontName(_T("Arial"));
	m_ssZPickPlaceOffset.SetFontBold(TRUE);
	m_ssZPickPlaceOffset.SetFontSize(10);
	m_ssZPickPlaceOffset.SetTypeHAlign(2); // Center
	m_ssZPickPlaceOffset.SetTypeVAlign(2); // Center


	int nMaxCol = 2;
	int nMaxRow = 2; 
	if (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) {
		nMaxRow = 3;
	}

	m_ssZPickPlaceOffset.SetMaxCols(nMaxCol);
	m_ssZPickPlaceOffset.SetMaxRows(nMaxRow);
	

	//Col Title Setting
	m_ssZPickPlaceOffset.SetTextMatrix(0, 0, _T(""));
	m_ssZPickPlaceOffset.SetTextMatrix(0, 1, _T("PICK"));
	m_ssZPickPlaceOffset.SetTextMatrix(0, 2, _T("PLACE"));

	//Row Title Setting
	if (nHand == VAT_HAND_LOAD_TRAYPP) {
		m_ssZPickPlaceOffset.SetTextMatrix(1, 0, "TABLE");
		m_ssZPickPlaceOffset.SetTextMatrix(2, 0, "FEEDER");
	}
	else if (nHand == VAT_HAND_UNLOAD_TRAYPP) {
		m_ssZPickPlaceOffset.SetTextMatrix(1, 0, "TABLE");
		m_ssZPickPlaceOffset.SetTextMatrix(2, 0, "C-TRAY");
	}
	else if(nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2){ //Test PP
		m_ssZPickPlaceOffset.SetTextMatrix(1, 0, "TABLE");
		m_ssZPickPlaceOffset.SetTextMatrix(2, 0, "STATION");
		m_ssZPickPlaceOffset.SetTextMatrix(3, 0, "CLEAN TABLE");
	}
	else {
		m_ssZPickPlaceOffset.SetTextMatrix(1, 0, "TABLE");
		m_ssZPickPlaceOffset.SetTextMatrix(2, 0, "STATION");
	}

	//Spread Sheet 가로 폭 조절.
	m_ssZPickPlaceOffset.SetColWidth(0, 15);
	for (int i = 1; i <= m_ssZPickPlaceOffset.GetMaxCols(); i++) {
		m_ssZPickPlaceOffset.SetColWidth(i, 10);
	}
	//Spread Sheet 세로 높이 조절.
	for (int i = 0; i <= m_ssZPickPlaceOffset.GetMaxRows(); i++) {
		m_ssZPickPlaceOffset.SetRowHeight(i, 20);
	}

	//Spread Sheet 변경 때 마다 Top Row를 첫번째 열로 고정.
	//spread sheet 좌,우 움직이지 않도록 마지막 Col 고정.
	m_ssZPickPlaceOffset.SetTopRow(0);
}


void CVATMainTab_ZAutoTeachingSetting::DisplayZOffsetData(int nHand)
{
	for (int nType = VAT_Z_PICK_OFFSET; nType < VAT_Z_OFFSET_TYPE_MAX; nType++) {
		int nMAX_UNIT = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_Z_OFFSET_UNIT_MAX : VAT_Z_OFFSET_UNIT_MIN;
		for (int nUnit = 0; nUnit < nMAX_UNIT; nUnit++)
		{
			double dOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(nHand, nType, nUnit);

			CString strOffset = _T("");
			strOffset.Format("%.3f", dOffset / 1000.0);

			int nCol = nType + 1;
			int nRow = nUnit + 1;

			m_ssZPickPlaceOffset.SetTextMatrix(nRow, nCol, strOffset);
		}
	}
}


// void CVATMainTab_ZAutoTeachingSetting::OnEnSetfocusEditZPickOffset(UINT nID)
// {
// 	char szInputValue[10] = { 0, };
// 	char szCurrentValue[10] = { 0, };
// 
// 	int nHand = nID - IDC_EDIT_Z_PICK_OFFSET1;
// 	sprintf(szCurrentValue, "%.2f", m_dZPickOffset[nHand]);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 10, szCurrentValue, "Z Pick Offset", "10.0", "-10.0");
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(nID)->SetWindowText(szInputValue);
// 
// 		//Data Save에서 진행.
// 		//UpdateData(TRUE); //Control -> 변수
// 	}
// }


// void CVATMainTab_ZAutoTeachingSetting::OnEnSetfocusEditZPlaceOffset(UINT nID)
// {
// 	char szInputValue[10] = { 0, };
// 	char szCurrentValue[10] = { 0, };
// 
// 	int nHand = nID - IDC_EDIT_Z_PLACE_OFFSET1;
// 	sprintf(szCurrentValue, "%.2f", m_dZPlaceOffset[nHand]);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 10, szCurrentValue, "Z Place Offset", "10.0", "-10.0");
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(nID)->SetWindowText(szInputValue);
// 
// 		//Data Save에서 진행.
// 		//UpdateData(TRUE); //Control -> 변수
// 	}
// }

void CVATMainTab_ZAutoTeachingSetting::ClickSpreadZTeachPickPlaceOffset(long Col, long Row)
{
	_Com_UpdateSheetCellColor(m_ssZPickPlaceOffset, m_stZPickPlaceOffsetLastInfo, Col, Row);
	_Com_UpdateLastSheetData(m_stZPickPlaceOffsetLastInfo, Col, Row);
}



void CVATMainTab_ZAutoTeachingSetting::DblClickSpreadZTeachPickPlaceOffset(long Col, long Row)
{
	if (Col <= 0 || Row <= 0)
		return;

	int nCurHand = m_stZPickPlaceOffsetLastInfo.nHand;

	char szInputValue[VAT_STRING_SIZE] = { 0, };
	char szCurrentValue[VAT_STRING_SIZE] = { 0, };

	CString strCurrentValue = m_ssZPickPlaceOffset.GetTextMatrix(Row, Col);
	sprintf_s(szCurrentValue, sizeof(szCurrentValue), "%s", strCurrentValue.GetString());

	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 15, szCurrentValue, "Target Pos", "5.0", "-5.0");
	if (nRet != TRUE)
		return;

	m_ssZPickPlaceOffset.SetTextMatrix(Row, Col, szInputValue);
}

void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachParamSetSave1()
{
	int nCurHand = m_stZPickPlaceOffsetLastInfo.nHand;

	for (int nType = VAT_Z_PICK_OFFSET; nType < VAT_Z_OFFSET_TYPE_MAX; nType++) {
		int nMAX_UNIT = (nCurHand == VAT_HAND_LOAD_TESTPP1 || nCurHand == VAT_HAND_LOAD_TESTPP2) ? VAT_Z_OFFSET_UNIT_MAX : VAT_Z_OFFSET_UNIT_MIN;
		for (int nUnit = 0; nUnit < nMAX_UNIT; nUnit++) {
			int nCol = nType + 1;
			int nRow = nUnit + 1;
			CString strOffset = m_ssZPickPlaceOffset.GetTextMatrix(nRow, nCol);

			double dOffset = atof(strOffset) * 1000.0;

			g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.setUnitZOffset(nCurHand, nType, nUnit, dOffset);
		}
	}
	//ini File 저장.
	g_VATCommonData.SaveEachZAutoTeachingInformation(eZAutoTeaching_PickPlaceOffset, nCurHand);

	DisplayZOffsetData(nCurHand);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssZPickPlaceOffset, m_stZPickPlaceOffsetLastInfo);
}


void CVATMainTab_ZAutoTeachingSetting::InitStaticCtrl_DesignValue()
{
	CFont StaticFont;
	StaticFont.CreatePointFont(120, "Fixedsys");

 	//eTABLE_HEIGHT ~ eDEVICE_HEIGHT
 	for (int idx = VAT_TABLE_HEIGHT; idx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; idx++) {		
 		GetDlgItem(m4_nDesignValue[idx])->SetFont(&StaticFont);
 	}
}


void CVATMainTab_ZAutoTeachingSetting::DisplayDesignValue()
{
	//eTABLE_HEIGHT ~ eDEVICE_HEIGHT
	for (int idx = VAT_TABLE_HEIGHT; idx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; idx++) {
		CString strValue = _T("");
		strValue.Format("%7.3f", m_dDesignValue[idx] /*/ 1000.0*/);
		GetDlgItem(m4_nDesignValue[idx])->SetWindowText(strValue);
	}
}


void CVATMainTab_ZAutoTeachingSetting::OnEnSetFocusDesignValue(UINT nID)
{
	//focus가 design일 경우 에만 진행.
	if (m_bBlockEditFocus)
		return;


	char szInputValue[10] = { 0, };
	char szCurrentValue[10] = { 0, };

	int nIdx = nID - IDC_EDIT_TBL_HEIGHT;
	sprintf(szCurrentValue, "%.2f", m_dDesignValue[nIdx]);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 10, szCurrentValue, "Design Value", "20.0", "-20.0");
	if (nRet == RETURN_OK) {
		GetDlgItem(nID)->SetWindowText(szInputValue);

		//Data Save에서 진행.
		//UpdateData(TRUE); //Control - > 변수
	}
}


//Design Value값 저장.
void CVATMainTab_ZAutoTeachingSetting::OnBnClickedBtnZTeachParamSetSave2()
{
	UpdateData(TRUE); //Control -> 변수

	//eTABLE_HEIGHT ~ eDEVICE_HEIGHT
	for (int idx = VAT_TABLE_HEIGHT; idx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; idx++) {
		g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[idx] = m_dDesignValue[idx] * 1000.0;
	}

	DisplayDesignValue();

	//data file 저장.
	g_VATCommonData.SaveEachDesignValue(eDesignValue_ZAutoTeaching);
}



//=====================================================================================================
// COMMENT : Data Copy, GUI ==> REAL 
//=====================================================================================================
void CVATMainTab_ZAutoTeachingSetting::DataCopyZBasePos_GuiToReal(int nHand)
{
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
		memcpy_s(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TrayPp, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TrayPp),
			m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TrayPp, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TrayPp));
		break;
	case VAT_HAND_UNLOAD_TRAYPP:
		memcpy_s(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TrayPp, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TrayPp),
			m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TrayPp, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TrayPp));
		break;
	case VAT_HAND_LOAD_TESTPP1:
		memcpy_s(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp1, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp1),
			m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp1, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp1));
		break;
	case VAT_HAND_UNLOAD_TESTPP1:
		memcpy_s(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp1, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp1),
			m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp1, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp1));
		break;
	case VAT_HAND_LOAD_TESTPP2:
		memcpy_s(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp2, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp2),
			m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp2, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp2));
		break;
	case VAT_HAND_UNLOAD_TESTPP2:
		memcpy_s(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp2, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp2),
			m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp2, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp2));
		break;
	case VAT_HAND_MAX:
		memcpy_s(&g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos), &m_stVATZTeachBasePosGui, sizeof(m_stVATZTeachBasePosGui));
		break;
	default:
		break;
	}
}


void CVATMainTab_ZAutoTeachingSetting::DataCopyZBasePos_RealToGui(int nHand)
{
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
		memcpy_s(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TrayPp, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TrayPp),
			g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TrayPp, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TrayPp));
		break;
	case VAT_HAND_UNLOAD_TRAYPP:
		memcpy_s(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TrayPp, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TrayPp),
			g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TrayPp, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TrayPp));
		break;
	case VAT_HAND_LOAD_TESTPP1:
		memcpy_s(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp1, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp1),
			g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp1, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp1));
		break;
	case VAT_HAND_UNLOAD_TESTPP1:
		memcpy_s(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp1, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp1),
			g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp1, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp1));
		break;
	case VAT_HAND_LOAD_TESTPP2:
		memcpy_s(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp2, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_LD_TestPp2),
			g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp2, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_LD_TestPp2));
		break;
	case VAT_HAND_UNLOAD_TESTPP2:
		memcpy_s(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp2, sizeof(m_stVATZTeachBasePosGui.dZTeachBasePos_UD_TestPp2),
			g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp2, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos.dZTeachBasePos_UD_TestPp2));
		break;
	case VAT_HAND_MAX:
		memcpy_s(&m_stVATZTeachBasePosGui, sizeof(m_stVATZTeachBasePosGui), &g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos, sizeof(g_VATCommonData.m_stVATZTeachInfo.stZTeachBasePos));
		break;
	default:
		break;
	}
}



void CVATMainTab_ZAutoTeachingSetting::InitMotorJog()
{
	m_pChildDlgJog = new CVAT_ManuMotorJog;
	m_pChildDlgJog->Create(IDD_VAT_MANU_MOTOR_JOG, this);

	//Dlg 위치 조정.
	CRect rectChild;
	m_pChildDlgJog->GetWindowRect(rectChild);
	
	m_pChildDlgJog->MoveWindow(544, 40, rectChild.Width(), rectChild.Height());

	m_pChildDlgJog->ShowWindow(SW_SHOW);
}






