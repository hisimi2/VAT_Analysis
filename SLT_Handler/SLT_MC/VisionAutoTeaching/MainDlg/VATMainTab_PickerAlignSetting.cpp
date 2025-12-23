// VATMainTab_PickerAlignSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMainTab_PickerAlignSetting.h"
#include "afxdialogex.h"


// CVATMainTab_PickerAlignSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMainTab_PickerAlignSetting, CDialogEx)

CVATMainTab_PickerAlignSetting::CVATMainTab_PickerAlignSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_PICKER_ALIGN_SETTING_DLG, pParent)
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++){
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++){
			m_nSelectedPkr[nHand][nAxis] = 0;
		}

		m_bVisionLiveStatus[nHand] = eLive_Off;
	}
}

CVATMainTab_PickerAlignSetting::~CVATMainTab_PickerAlignSetting()
{
}

void CVATMainTab_PickerAlignSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICKER_ALIGN_SETTING_SPREAD_1, m_ssPkr_LoadTrayPP);
	DDX_Control(pDX, IDC_PICKER_ALIGN_SETTING_SPREAD_2, m_ssPkr_UnloadTrayPP);
	DDX_Control(pDX, IDC_PICKER_ALIGN_SETTING_SPREAD_3, m_ssPkr_LoadTestPP1);
	DDX_Control(pDX, IDC_PICKER_ALIGN_SETTING_SPREAD_4, m_ssPkr_UnloadTestPP1);
	DDX_Control(pDX, IDC_PICKER_ALIGN_SETTING_SPREAD_5, m_ssPkr_LoadTestPP2);
	DDX_Control(pDX, IDC_PICKER_ALIGN_SETTING_SPREAD_6, m_ssPkr_UnloadTestPP2);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		//Position Spread Sheet
		DDX_Control(pDX, m3_nEachPkr_TargetPos[nHand], m_ssEachPkrTargetPos[nHand]);
		DDX_Control(pDX, m3_nEachPkr_PrePos[nHand], m_ssEachPkrSettingPos[nHand]);

		//Operate Button
		DDX_Control(pDX, m3_nMoveTargetPos[nHand], m_BtnTargetMove[nHand]);
		DDX_Control(pDX, m3_nMovePrePos[nHand], m_BtnPreMove[nHand]);
		DDX_Control(pDX, m3_nEncoderRead[nHand], m_BtnRead[nHand]);
		DDX_Control(pDX, m3_nEncoderSave[nHand], m_BtnSave[nHand]);
		DDX_Control(pDX, m3_nVisionLive[nHand], m_BtnVisionLive[nHand]); //Vision Live Button
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
}


BEGIN_MESSAGE_MAP(CVATMainTab_PickerAlignSetting, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SELECTED_PKR_MOVE_1, IDC_BTN_SELECTED_PKR_MOVE_6, &CVATMainTab_PickerAlignSetting::OnBtnClickedTargetPosMove)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SELECTED_PKR_PRE_MOVE_1, IDC_BTN_SELECTED_PKR_PRE_MOVE_6, &CVATMainTab_PickerAlignSetting::OnBtnClickedPrePosMove)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SELECTED_PKR_READ_ENCODER_1, IDC_BTN_SELECTED_PKR_READ_ENCODER_6, &CVATMainTab_PickerAlignSetting::OnBtnClickedPrePosRead)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SELECTED_PKR_SAVE_ENCODER_1, IDC_BTN_SELECTED_PKR_SAVE_ENCODER_6, &CVATMainTab_PickerAlignSetting::OnBtnClickedPrePosSave)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_SELECTED_VISION_LIVE_1, IDC_BTN_SELECTED_VISION_LIVE_6, &CVATMainTab_PickerAlignSetting::OnBtnClickedVisionLive)

	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_OFFSET, &CVATMainTab_PickerAlignSetting::OnReceiveUpdateVisionOffset)
END_MESSAGE_MAP()




BOOL CVATMainTab_PickerAlignSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	///////////////////////////////////
	//순서 변경 하지 말것.

	//Picker Select Spread Sheet
	_PkrAlignCom_InitSSPickerStatus();

	//Offset Rect Graphic&Data View
	InitSettingTabRect();

	//Selected Picker 초기화.
	InitSelectedPicker();

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		//Spread Sheet 초기화
		InitPosSpreadSheet(m_ssEachPkrTargetPos[nHand]);
		InitPosSpreadSheet(m_ssEachPkrSettingPos[nHand]);

		//선택 Picker Color 변경.
		SelectedPickerColor(nHand);

		//Position Display
		DisplayTargetPos(nHand);
		DisplayPrePos(nHand);
	}

	//Type에 따라 Cell Block이 다름.
	for (int nType = 0; nType < eSSType_Max; nType++) {
		BlockSpreadSheet(nType);
	}

	//Operate Button 초기화.
	InitOperBtn();

	return TRUE;
}


void CVATMainTab_PickerAlignSetting::PageMoveInit_Tab()
{
	//Offset Grid&Data 다시 그리기.
	Invalidate(TRUE);

	//Timer
	SetTimer(_ePickerAlignT_CheckExecuteStatus, 200, NULL);

	SetTimer(_ePickerAlignT_VisionMeasureLive, 1500, NULL);
	//Data Display
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		DisplayTargetPos(nHand);
		DisplayPrePos(nHand);
	}
}


void CVATMainTab_PickerAlignSetting::PageMoveDispose_Tab()
{
	for (int i = 0; i < _ePickerAlignT_Max; i++) {
		KillTimer(i);
	}
}


void CVATMainTab_PickerAlignSetting::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case _ePickerAlignT_CheckExecuteStatus:
		{
			//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
			if (g_bVATStatusExecute != m_bLastExecuteStatus)
			{
				EnableControls(!g_bVATStatusExecute);
				m_bLastExecuteStatus = g_bVATStatusExecute;
			}
		}break;
		case _ePickerAlignT_VisionMeasureLive:
		{
			if(g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				break;

			for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
			{
				BOOL bIsConnect = CheckVisionConnect(nHand);
				if(!bIsConnect)
					continue;

				if(m_bVisionLiveStatus[nHand] == eLive_Off)
					continue;

				//Cam ID && Data ID			
				int nCamNo = -1;
				int nDataId = -1;
				switch (nHand)
				{
				case VAT_HAND_LOAD_TRAYPP:		
				{
					nCamNo = VAT_CAM_LD_TRAY_PP_LOWER;
					nDataId = VISION_REQ_DATAID_VAT_LD_LOWER;
				}break;
				case VAT_HAND_UNLOAD_TRAYPP:	
				{
					nCamNo = VAT_CAM_UD_TRAY_PP_LOWER;
					nDataId = VISION_REQ_DATAID_VAT_ULD_LOWER;
				}break;
				case VAT_HAND_LOAD_TESTPP1:		
				case VAT_HAND_UNLOAD_TESTPP1:
				{
					nCamNo = VAT_CAM_TEST_PP_1_LOWER;
					nDataId = VISION_REQ_DATAID_VAT_TEST_1_LOWER; 
				}break;
				case VAT_HAND_LOAD_TESTPP2:		
				case VAT_HAND_UNLOAD_TESTPP2:
				{
					nCamNo = VAT_CAM_TEST_PP_2_LOWER;
					nDataId = VISION_REQ_DATAID_VAT_TEST_2_LOWER;
				}break;
				default:
					break;
				}

				//Vision Measure
				g_VatVisionMgr[nCamNo].SendMeasure(nDataId, VISION_STATUS_PICKER);
			}
		}break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CVATMainTab_PickerAlignSetting::OnDestroy()
{
	for (int i = 0; i < _ePickerAlignT_Max; i++) {
		KillTimer(i);
	}

	CDialogEx::OnDestroy();
}


void CVATMainTab_PickerAlignSetting::EnableControls(BOOL bStatus)
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		//Spread Sheet
		m_ssEachPkrSettingPos[nHand].EnableWindow(bStatus);

		//Operate Btn
		m_BtnTargetMove[nHand].EnableWindow(bStatus);
		m_BtnPreMove[nHand].EnableWindow(bStatus);
		m_BtnRead[nHand].EnableWindow(bStatus);
		m_BtnSave[nHand].EnableWindow(bStatus);

		m_BtnVisionLive[nHand].EnableWindow(bStatus);
	}
}


BOOL CVATMainTab_PickerAlignSetting::CheckVisionConnect(int nHand)
{
	ASSERT(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2);

	BOOL bRet = FALSE;

	if (VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP1) {
		bRet = g_pVisionTestHand_1->IsConnected();
	}
	else {
		bRet = g_pVisionTestHand_2->IsConnected();
	}

	//DisConnect시, 상태 Off로 변경.
	if (bRet != TRUE) {
		m_bVisionLiveStatus[nHand] = eLive_Off;
		DisplayVisionLiveStaus(nHand, m_bVisionLiveStatus[nHand]);
	}

	return bRet;
}


void CVATMainTab_PickerAlignSetting::OnPaint()
{
	CPaintDC dc(this);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		BOOL bIsReDraw = dc.RectVisible(&m_rect[nHand]) ||
			dc.RectVisible(&m_rectData[nHand][VAT_X]) ||
			dc.RectVisible(&m_rectData[nHand][VAT_Y]);

		if (bIsReDraw) { //필요한 rect만 다시 그리기
			_PkrAlignCom_DrawBaseGrid(nHand, dc);

			_PkrAlignCom_DrawPkrCenterHole(nHand, dc);

			_PkrAlignCom_DrawHeadText(nHand, dc);

			_PkrAlignCom_DrawOffsetData(nHand, dc);

			_PkrAlignCom_DrawRealHole(nHand, dc);
		}

	}

	//Invalidate(TRUE); //BackGround Erase

	//해당 Dlg ID로 Rect 초기화.
	// 	InitSettingTabRect();
	// 
	// 	CPaintDC dc(this);
	// 	_PkrAlignCom_DrawBaseGrid(0/*nHand*/, dc);
	// 
	// 	_PkrAlignCom_DrawPkrCenterHole(0/*nHand*/, dc);
	// 
	// 	_PkrAlignCom_DrawHeadText(0/*nHand*/, dc);
	// 
	// 	//임시
	// 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
	// 		_PkrAlignCom_DrawOffsetData(nHand, dc);
	// 	}
}


BEGIN_EVENTSINK_MAP(CVATMainTab_PickerAlignSetting, CDialogEx)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_1, 3, CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread1, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_2, 3, CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread2, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_3, 3, CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread3, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_4, 3, CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread4, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_5, 3, CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread5, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_6, 3, CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread6, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_SUB2_1, 11, CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread1, VTS_I4 VTS_I4)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_SUB2_2, 11, CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread2, VTS_I4 VTS_I4)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_SUB2_3, 11, CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread3, VTS_I4 VTS_I4)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_SUB2_4, 11, CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread4, VTS_I4 VTS_I4)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_SUB2_5, 11, CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread5, VTS_I4 VTS_I4)
ON_EVENT(CVATMainTab_PickerAlignSetting, IDC_PICKER_ALIGN_SETTING_SPREAD_SUB2_6, 11, CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread6, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

int CVATMainTab_PickerAlignSetting::GetHandType(int nHand)
{
	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
		return eHandType_TrayPp;
	else
		return eHandType_TestPp;
}

void CVATMainTab_PickerAlignSetting::InitPosSpreadSheet(CSpreadSheet &spread_sheet)
{
	//격자 표기.
	spread_sheet.SetBackColorStyle(1); //under grid

	spread_sheet.SetFontName(_T("Arial"));
	spread_sheet.SetFontBold(TRUE);
	//spread_sheet.SetFontSize(1); //Font size는 Sheet크기가 작아 Default값으로 설정 됨.
	spread_sheet.SetTypeHAlign(2); // Center
	spread_sheet.SetTypeVAlign(2); // Center
	spread_sheet.SetFontSize(10);
	spread_sheet.SetMaxCols(1);//Data 한개만 표기.
	spread_sheet.SetMaxRows(2); 



	//Col Title Setting 없음.


	const char* szDirection[VAT_MAX_AXIS_XY] = { "X", "Y" };
	//Row Title Setting
	for (int i = 0; i < spread_sheet.GetMaxRows(); i++)
	{
		spread_sheet.SetTextMatrix(i + 1, 0, szDirection[i]);
	}


	//Spread Sheet 가로 폭 조절.
	spread_sheet.SetColWidth(0, 3);
	spread_sheet.SetColWidth(1, 8);

	
	//Spread Sheet 세로 높이 조절.
 	for (int i = 1; i <= spread_sheet.GetMaxRows(); i++) {
 		spread_sheet.SetRowHeight(i, 16);
 	}


	//해당 Spread Sheet Cell위로 바로 Focus됨.
	spread_sheet.SetMoveActiveOnFocus(TRUE);	
}


void CVATMainTab_PickerAlignSetting::BlockSpreadSheet(int nType)
{
	if (nType == eSSType_EachPkrPos)
	{
		for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
		{
			//cell 색상 변경.
			m_ssEachPkrTargetPos[nHand].SetCol(1);
			for (int i = 1; i <= m_ssEachPkrTargetPos[nHand].GetMaxRows(); i++)
			{
				m_ssEachPkrTargetPos[nHand].SetRow(i);
				m_ssEachPkrTargetPos[nHand].SetBackColor(LLIGHTBLUE);
				//spread_sheet.SetBlockMode(TRUE);
			}
		}
	}
	else //nType == eSSType_PkrSettingPos
	{
		//Load Tray PP는 Y축이 없어 해당 Cell Color 및 Text Block 처리.
		m_ssEachPkrSettingPos[VAT_HAND_LOAD_TRAYPP].SetCol(1);
		m_ssEachPkrSettingPos[VAT_HAND_LOAD_TRAYPP].SetRow(2);
		m_ssEachPkrSettingPos[VAT_HAND_LOAD_TRAYPP].SetBackColor(LLIGHTGRAY);

		m_ssEachPkrSettingPos[VAT_HAND_LOAD_TRAYPP].SetTextMatrix(2, 1, "-");
	}
}


/*===============================================================*/
//COMMENT : Pre Position Spread Sheet만 DblClick가능함.
/*============================================================*/
void CVATMainTab_PickerAlignSetting::DblClickSheetTextChange(int nHand, int nCol, int nRow)
{
	if (nCol <= 0 || nRow <= 0)
		return;

	//Load Tray PP는 Y축 Pre Position이 없음.
	if (nHand == VAT_HAND_LOAD_TRAYPP && (nRow - 1) == VAT_Y)
		return;

	//Positive, Negative Limit 표기.
	char strPosSWLimit[128];
	char strNegSWLimit[128];

	int nPhysicalNo = ConvertSSCellToMotorAxisNo(nHand, nRow);

	double dPosSwLimit = g_Motor[nPhysicalNo]->GetSwLimitPosi() / 1000.0;
	double dNegSwLimit = g_Motor[nPhysicalNo]->GetSwLimitNega() / 1000.0;

	sprintf(strPosSWLimit, "%.3f", dPosSwLimit);
	sprintf(strNegSWLimit, "%.3f", dNegSwLimit);

	char szInputValue[VAT_STRING_SIZE] = { 0, };
	char szCurrentValue[VAT_STRING_SIZE] = { 0, };
	char szHeadText[VAT_STRING_SIZE] = { 0, };

	CString strCurrentValue = m_ssEachPkrSettingPos[nHand].GetTextMatrix(nRow, nCol);
	sprintf_s(szCurrentValue, sizeof(szCurrentValue), "%s", strCurrentValue.GetString());

	CString strHead = (nRow - 1 == VAT_X) ? _T("X Pre Position") : _T("Y Pre Position");
	sprintf_s(szHeadText, sizeof(szHeadText), "%s", strHead.GetString());

	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 15, szCurrentValue, szHeadText, strPosSWLimit, strNegSWLimit);
	if (nRet != TRUE)
		return;

	m_ssEachPkrSettingPos[nHand].SetTextMatrix(nRow, nCol, szInputValue);
}


/*===============================================================*/
//COMMENT : Spread Sheet의 Cell 위치를 모터 Physical No으로 변환.
//          nRow은 Axis No으로 사용.
//RETURN : Motor Physical No.
/*============================================================*/
int CVATMainTab_PickerAlignSetting::ConvertSSCellToMotorAxisNo(int nHand, int nRow)
{
	int nPhysicalNo = -1;
	int nAxis = nRow - 1;

	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TRAY_PP_1_X;
			//else							nPhysicalNo = eAXIS_TRAY_FEEDER_Y;
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TRAY_PP_2_X;
			else							nPhysicalNo = eAXIS_TRAY_PP_2_Y;
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		{
			if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_1_X;
			else							nPhysicalNo = eAXIS_TEST_PP_1_Y;
		}break;
		case VAT_HAND_UNLOAD_TESTPP1:
		{
			if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_1_X2;
			else							nPhysicalNo = eAXIS_TEST_PP_1_Y;
		}break;
		case VAT_HAND_LOAD_TESTPP2:
		{
			if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_2_X;
			else							nPhysicalNo = eAXIS_TEST_PP_2_Y;
		}break;
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_2_X2;
			else							nPhysicalNo = eAXIS_TEST_PP_2_Y;
		}break;
	}


	return nPhysicalNo;
}


void CVATMainTab_PickerAlignSetting::InitSelectedPicker()
{
	//기준 Picker로 m_SelectedPkr 초기화
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_nSelectedPkr[nHand][VAT_X] = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].first;
		m_nSelectedPkr[nHand][VAT_Y] = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].second;
	}
}

//=======================================================================
//COMMENT : 기준 Picker 색상 변경.
//          선택 Picker 색상 변경.
//=======================================================================
void CVATMainTab_PickerAlignSetting::SelectedPickerColor(int nHand)
{
	if (nHand < VAT_HAND_LOAD_TRAYPP && nHand > VAT_HAND_UNLOAD_TESTPP2)
		return;

	CSpreadSheet *ssPickerStatus[VAT_HAND_MAX] = { &m_ssPkr_LoadTrayPP, &m_ssPkr_UnloadTrayPP, &m_ssPkr_LoadTestPP1, &m_ssPkr_UnloadTestPP1, &m_ssPkr_LoadTestPP2, &m_ssPkr_UnloadTestPP2 };

	int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
	int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

	//m_nSelectedPkr -> 실제 Picker Index로 저장
	//gui Idx로 변환 필요.
	int nGuiSelectedPkrX = m_nSelectedPkr[nHand][VAT_X];
	int nGuiSelectedPkrY = m_nSelectedPkr[nHand][VAT_Y];

	//Tray PP는 실제 Pkr Index와 Gui Index가 동일.
	ConvPkrIdx_Real_To_Gui(nHand, m_nSelectedPkr[nHand][VAT_X], m_nSelectedPkr[nHand][VAT_Y], nGuiSelectedPkrX, nGuiSelectedPkrY);

	//Selected Picker Index -> GUI Col&Row
	int nSelectedPkrCol = nGuiSelectedPkrX + 1;
	int nSelectedPkrRow = nGuiSelectedPkrY + 1;

	for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++)
	{
		for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++)
		{
			int nCol = nGuiX + 1;
			int nRow = nGuiY + 1;

			ssPickerStatus[nHand]->SetCol(nCol);
			ssPickerStatus[nHand]->SetRow(nRow);

			//SelectedPkr일 경우, 노란색으로 표기.
			if (nSelectedPkrCol == nCol && nSelectedPkrRow == nRow)
			{
				ssPickerStatus[nHand]->SetTypeButtonColor(LLIGHTBLUE);
			}
			else 
			{
				//기준 Picker 색상 표기.
				if (IsGuiStandardPicker(nHand, nGuiX, nGuiY) == TRUE) {
					ssPickerStatus[nHand]->SetTypeButtonColor(LLIGHTYELLOW/*LLIGHTCYAN*/);
				}
				else {
					//Unselect Picker 흰색 표기.
					ssPickerStatus[nHand]->SetTypeButtonColor(WWHITE);
				}
			}
		}
	}
}

void CVATMainTab_PickerAlignSetting::SetSelectedPicker(int nHand, int nCol, int nRow)
{
	if (nCol < 0 || nRow < 0)
		return;

	//GUI Idx -> Real Idx로 변환.
	int nPkrX = nCol - 1;
	int nPkrY = nRow - 1;

	ConvPkrIdx_Gui_To_Real(nHand, nCol - 1, nRow - 1, nPkrX, nPkrY);

	m_nSelectedPkr[nHand][VAT_X] = nPkrX;/*nCol;*/
	m_nSelectedPkr[nHand][VAT_Y] = nPkrY;/*nRow;*/
}


void CVATMainTab_PickerAlignSetting::DisplaySelectedPicker(int nHand)
{
	//Selected Picker Color 변경.
	SelectedPickerColor(nHand);

	//Selected Picker Target/Pre Position 표기.
	DisplayTargetPos(nHand);
}


void CVATMainTab_PickerAlignSetting::InitSettingTabRect()
{
	//Offset Graphic View
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		CStatic *staticArea = (CStatic *)GetDlgItem(m3_nRectGraphicView_SettingTab[nHand]);
		staticArea->GetWindowRect(&m_rect[nHand]);
		ScreenToClient(m_rect[nHand]);
	}

 	//Offset Data View
 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
 	{
 		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
 		{
 			CStatic *staticArea = (CStatic *)GetDlgItem(m3_nRectDataView_SettingTab[nHand][nAxis]);
 			staticArea->GetWindowRect(&m_rectData[nHand][nAxis]);
 			ScreenToClient(m_rectData[nHand][nAxis]);
 		}
 	}
}


void CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSelectedPicker(VAT_HAND_LOAD_TRAYPP, Col, Row);
	DisplaySelectedPicker(VAT_HAND_LOAD_TRAYPP);
}


void CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSelectedPicker(VAT_HAND_UNLOAD_TRAYPP, Col, Row);
	DisplaySelectedPicker(VAT_HAND_UNLOAD_TRAYPP);
}


void CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread3(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSelectedPicker(VAT_HAND_LOAD_TESTPP1, Col, Row);
	DisplaySelectedPicker(VAT_HAND_LOAD_TESTPP1);
}


void CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread4(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSelectedPicker(VAT_HAND_UNLOAD_TESTPP1, Col, Row);
	DisplaySelectedPicker(VAT_HAND_UNLOAD_TESTPP1);
}


void CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread5(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSelectedPicker(VAT_HAND_LOAD_TESTPP2, Col, Row);
	DisplaySelectedPicker(VAT_HAND_LOAD_TESTPP2);
}


void CVATMainTab_PickerAlignSetting::ButtonClickedPkrSelectSpread6(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SetSelectedPicker(VAT_HAND_UNLOAD_TESTPP2, Col, Row);
	DisplaySelectedPicker(VAT_HAND_UNLOAD_TESTPP2);
}


void CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread1(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DblClickSheetTextChange(VAT_HAND_LOAD_TRAYPP, Col, Row);
}


void CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread2(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DblClickSheetTextChange(VAT_HAND_UNLOAD_TRAYPP, Col, Row);
}


void CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread3(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DblClickSheetTextChange(VAT_HAND_LOAD_TESTPP1, Col, Row);
}


void CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread4(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DblClickSheetTextChange(VAT_HAND_UNLOAD_TESTPP1, Col, Row);
}


void CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread5(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DblClickSheetTextChange(VAT_HAND_LOAD_TESTPP2, Col, Row);

}


void CVATMainTab_PickerAlignSetting::DblClickedPkrPrePositionSpread6(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DblClickSheetTextChange(VAT_HAND_UNLOAD_TESTPP2, Col, Row);
}


void CVATMainTab_PickerAlignSetting::InitOperBtn()
{
	CFont OperBtnFont;
	OperBtnFont.CreatePointFont(90, "Arial");

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{	
		m_BtnTargetMove[nHand].SetFont(&OperBtnFont);
		m_BtnTargetMove[nHand].SetWindowText(_T("MOVE"));
		m_BtnTargetMove[nHand].SetIcon(IDI_BTN_BASE);
		m_BtnTargetMove[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
		m_BtnTargetMove[nHand].SetAlign(CButtonST::ST_ALIGN_VERT);
		
		m_BtnPreMove[nHand].SetFont(&OperBtnFont);
		m_BtnPreMove[nHand].SetWindowText(_T("MOVE"));
		m_BtnPreMove[nHand].SetIcon(IDI_BTN_BASE);
		m_BtnPreMove[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
		m_BtnPreMove[nHand].SetAlign(CButtonST::ST_ALIGN_VERT);

		m_BtnRead[nHand].SetFont(&OperBtnFont);
		m_BtnRead[nHand].SetWindowText(_T("READ"));
		m_BtnRead[nHand].SetIcon(IDI_BTN_NEW);
		m_BtnRead[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
		m_BtnRead[nHand].SetAlign(CButtonST::ST_ALIGN_VERT);

		m_BtnSave[nHand].SetFont(&OperBtnFont);
		m_BtnSave[nHand].SetWindowText(_T("SAVE"));
		m_BtnSave[nHand].SetIcon(IDI_BTN_SAVE);
		m_BtnSave[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
		m_BtnSave[nHand].SetAlign(CButtonST::ST_ALIGN_VERT);

		m_BtnVisionLive[nHand].SetFont(&OperBtnFont);
		m_BtnVisionLive[nHand].SetWindowText(_T("LIVE"));
		m_BtnVisionLive[nHand].SetIcon(IDI_BTN_VISION_LIVE_OFF, 20, 20);
		m_BtnVisionLive[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
		m_BtnVisionLive[nHand].SetAlign(CButtonST::ST_ALIGN_VERT);
		m_BtnVisionLive[nHand].SetImageOrg(0, 70);
	}
}


//얘는 vat_taskautotrayloadpp -> 여기서 가져오는걸로
//객체 생성해서 가져오는걸로 하면 좋겠다.
//_Base에서 정의해둘 거임
void CVATMainTab_PickerAlignSetting::DisplayTargetPos(int nHand)
{
	//Wide Data로 표기.

	//기준 Picker Index
	int std_x_idx = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].first;
	int std_y_idx = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].second;
	
	//Select Picker Index
	int target_x_idx = m_nSelectedPkr[nHand][VAT_X];
	int target_y_idx = m_nSelectedPkr[nHand][VAT_Y];

	//Move Step
// 	int move_x_step = std_x_idx - target_x_idx;
// 	int move_y_step = target_y_idx - std_y_idx;

	int move_x_step = 0;
	int move_y_step = 0;
	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			move_x_step = std_x_idx - target_x_idx;
			move_y_step = std_y_idx - target_y_idx;
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			move_x_step = std_x_idx - target_x_idx;
			move_y_step = target_y_idx - std_y_idx;
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP1:
		{
			move_x_step = target_x_idx - std_x_idx;
			move_y_step = target_y_idx - std_y_idx;
		}break;
		case VAT_HAND_LOAD_TESTPP2:
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			move_x_step = std_x_idx - target_x_idx;
			move_y_step = std_y_idx - target_y_idx;
		}break;
		default:
			break;
	}


	double dHandPitchX = 0.0;
	double dHandPitchY = 0.0;
	if (GetHandType(nHand) == eHandType_TrayPp) {
		dHandPitchX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_X);
		dHandPitchY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);
	}
	else { //eHandType_TestPp
		dHandPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X); //46mm[Station]
		dHandPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y); //60mm[Station]
	}
		
	//Display Target Position
	double dStdPkrPosX = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][VAT_X];
	double dStdPkrPosY = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][VAT_Y];



	double dTargetPosX = 0.0;
	double dTargetPosY = 0.0;


	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) {
		dTargetPosX = dStdPkrPosX + (move_x_step*dHandPitchX);
		dTargetPosY = dStdPkrPosY + (move_y_step*dHandPitchY);
	}
	else {
		//※※※Test PP의 경우 계산되는 Pitch와 실제 장비 정면에서 Pitch가 반대.※※※
		dTargetPosX = dStdPkrPosX + (move_y_step*dHandPitchY);
		dTargetPosY = dStdPkrPosY + (move_x_step*dHandPitchX);
	}

	//Test PP의 경우 계산되는 Pitch와 실제 장비 정면에서 Pitch가 반대.
// 	if (GetHandType(nHand) == eHandType_TestPp)
// 	{
// 		double dTempX = dTargetPosX; 
// 		double dTempY = dTargetPosY;
// 
// 		dTargetPosX = dTempY;
// 		dTargetPosY = dTempX;
// 	}

	CString strPos = _T("");
	strPos.Format("%.3f", dTargetPosX/1000.0);
	m_ssEachPkrTargetPos[nHand].SetTextMatrix(VAT_X + 1, 1, strPos);

	strPos.Format("%.3f", dTargetPosY/1000.0);
	m_ssEachPkrTargetPos[nHand].SetTextMatrix(VAT_Y + 1, 1, strPos);

	m_ssEachPkrTargetPos[nHand].SetFontBold(TRUE);
}

void CVATMainTab_PickerAlignSetting::DisplayPrePos(int nHand)
{
	for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
	{
		double dPos = g_VATCommonData.m_stVAT_PickerAlignSubInfo.dPreSettingPos[nHand][nAxis];

		CString strPos = _T("");
		strPos.Format("%.3f", dPos/1000.0);

		int nRow = nAxis + 1;
		m_ssEachPkrSettingPos[nHand].SetTextMatrix(nRow, 1, strPos);
	}
}


void CVATMainTab_PickerAlignSetting::DisplayVisionLiveStaus(int nHand, int nStatus)
{
	if (nStatus == eLive_On) {
		m_BtnVisionLive[nHand].SetIcon(IDI_BTN_VISION_LIVE_ON, 20, 20);
	}
	else {
		m_BtnVisionLive[nHand].SetIcon(IDI_BTN_VISION_LIVE_OFF, 20, 20);
	}
}


void CVATMainTab_PickerAlignSetting::MotorMove(int nHand, _eMotorMoveType eMoveType)
{
	//"Do you want to Encoder Move?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_MOTOR_MOVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);

	//[Move Interlock 확인]
	_eVAT_INTERLOCK eInterlock = ChkMotorMoveInterlock(nHand);
	if (eInterlock == eVAT_INTERLOCK_FAIL)
		return;


	//[Make Cmd Position]
	//1. Picker
	std::vector<CPoint> vPicker;
	vPicker.clear();
	vPicker.push_back(CPoint(m_nSelectedPkr[nHand][VAT_X], m_nSelectedPkr[nHand][VAT_Y]));
	
	//2. Position
	CSpreadSheet* spread_sheet[eMoveTypeMax] = { m_ssEachPkrTargetPos, m_ssEachPkrSettingPos };

	double dCmdPos[VAT_MAX_AXIS] = { 0, };
	dCmdPos[VAT_X] = atof(spread_sheet[eMoveType][nHand].GetTextMatrix(VAT_X + 1, 1)) * 1000.0;
	dCmdPos[VAT_Y] = atof(spread_sheet[eMoveType][nHand].GetTextMatrix(VAT_Y + 1, 1)) * 1000.0;
	int nPickerIdx = m_nSelectedPkr[nHand][VAT_X] + (m_nSelectedPkr[nHand][VAT_Y] * 4);
	dCmdPos[VAT_Z] = g_VATCommonData.m_stVATPickerAlignInfo.dPickerZFocusPos[nHand][nPickerIdx]; //각각의 Picker 초점 맞는 Z축 위치.

																								 //Hand Pitch 정보 가져옴.
	ST_VAT_HAND_PITCH_INFO HandPitchInfo;

	auto it = cHandPitchParamMap.find(nHand);
	if (it != cHandPitchParamMap.end()) {
		HandPitchInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	//[XP, YP] - Teaching Table에서 가져옴.
	int nLoc = -1;
	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) {
		nLoc = (nHand == VAT_HAND_LOAD_TRAYPP) ? LOC_IDX_TRAY_LOAD_PP_TBL_2 : LOC_IDX_TRAY_UNLOAD_PP_TBL_1;
	}
	else { //Test PP
		nLoc = LOC_IDX_STATION_1;
	}

	dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, nLoc, HandPitchInfo.xp_pos_idx);
	dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, nLoc, HandPitchInfo.yp_pos_idx);



	//3. Mode
	int nMode = VAT_SEQ_MODE_PICKER_ALIGN;
																								 //[Position for LOG] 
	CString strCmdStage = _T("");
	strCmdStage.Format("Target [Selected Pkr:%d]", nPickerIdx );

	int nErr = -1;
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(nMode);
		pSeqCmd->m_strCmdStage = strCmdStage;
		pSeqCmd->m_vCmdPicker = vPicker;
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		nErr = g_VatTaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);

	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(nMode);
		pSeqCmd->m_vCmdPicker = vPicker;
		pSeqCmd->m_strCmdStage = strCmdStage;
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		nErr = g_VatTaskSystemCtrl.SendCommand(g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual);

	}break;
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(nMode);
		pSeqCmd->m_vCmdPicker = vPicker;
		pSeqCmd->m_nTestPPType = (nHand == VAT_HAND_LOAD_TESTPP1) ? VAT_TESTPP_LOAD : VAT_TESTPP_UNLOAD;
		pSeqCmd->m_strCmdStage = strCmdStage;
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		nErr = g_VatTaskSystemCtrl.SendCommand(g_TaskTestPP[eTestPP_1], pSeqCmd, eEqpCtrlMode_Manual);
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(nMode);
		pSeqCmd->m_vCmdPicker = vPicker;
		pSeqCmd->m_nTestPPType = (nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_TESTPP_LOAD : VAT_TESTPP_UNLOAD;
		pSeqCmd->m_strCmdStage = strCmdStage;
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		nErr = g_VatTaskSystemCtrl.SendCommand(g_TaskTestPP[eTestPP_2], pSeqCmd, eEqpCtrlMode_Manual);
	}break;
	default:
		break;
	}

	if (nErr == ERR_NO_ERROR) {
		_Com_MakeLog_GUI("[Manual Move] [VAT Mode : %d] [Target : %s]", nMode, strCmdStage);
	}
	else {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Failed to Manual Move");
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}
}

_eVAT_INTERLOCK CVATMainTab_PickerAlignSetting::ChkMotorMoveInterlock(int nHand)
{
	//[Interlock] 실행 중, Manual Cmd 확인.
	if (g_bVATStatusExecute) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("manual command is Excuting!! please wait working finish!!!!");
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_MANUAL_CMD_IS_WORKING, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}

	//[Interlock] Cover 확인.
	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}

	//[Interlock] 원점 Check 확인.(X, Y, Z)
	_eVAT_INTERLOCK eAxisOrgCheck = _Com_SSInterlock_Origin(nHand);
	if (eAxisOrgCheck != eVAT_INTERLOCK_PASS) {
		return eVAT_INTERLOCK_FAIL; //함수 내부 Alarm
	}

	//[Interlock] Test Pp일 경우, Press Safety 확인.
	if (VAT_HAND_LOAD_TESTPP1 <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2)
	{
		int nPressStartIdx = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_UNLOAD_TESTPP1) ? ePressUnit_Down_1 : ePressUnit_Down_5;
		for (int i = 0; i < eMaxPressUnitCount / 2; i++)
		{
			int nPressIdx = nPressStartIdx + i;

			//Safety Sensor
			if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press%d is not detected Safety Sensor.", nPressIdx);
				g_Error.AlarmReport(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx, AlarmData, NULL);
				return eVAT_INTERLOCK_FAIL;
			}

			//Safety Pos
			if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press%d is not Safety Position.", nPressIdx);
				g_Error.AlarmReport(ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx, AlarmData, NULL);
				return eVAT_INTERLOCK_FAIL;
			}
		}
	}

	return eVAT_INTERLOCK_PASS;
}


void CVATMainTab_PickerAlignSetting::OnBtnClickedTargetPosMove(UINT nID)
{
	int nHand = ERR_VAT_ERROR; //-1
	for (int i = 0; i < VAT_HAND_MAX; i++) {
		if (m3_nMoveTargetPos[i] == nID) {
			nHand = i;
			break;
		}
	}

	if (nHand == ERR_VAT_ERROR)
		return;

	MotorMove(nHand, eMoveType_SelectedPkrPos);
}


void CVATMainTab_PickerAlignSetting::OnBtnClickedPrePosMove(UINT nID)
{
	int nHand = ERR_VAT_ERROR; //-1
	for (int i = 0; i < VAT_HAND_MAX; i++) {
		if (m3_nMovePrePos[i] == nID) {
			nHand = i;
			break;
		}
	}

	if (nHand == ERR_VAT_ERROR)
		return;

	MotorMove(nHand, eMoveType_PrePosition);
}


void CVATMainTab_PickerAlignSetting::ReadEncoder(int nHand)
{
	//"Do you want to read encoder?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_ENCODER_READ, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//[Interlock] 원점 Check 확인.(X, Y, Z)
	_eVAT_INTERLOCK eAxisOrgCheck = _Com_SSInterlock_Origin(nHand);
	if (eAxisOrgCheck != eVAT_INTERLOCK_PASS) {
		return; //함수 내부 Alarm
	}

	//Encoder Read(X, Y)
	for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
	{
		double dCurPos = 0.0;
		int nRow = nAxis + 1;

		int nPhysicalNo = ConvertSSCellToMotorAxisNo(nHand, nRow);
		g_Motor[nPhysicalNo]->GetCurrentPos(dCurPos);

		//Spread Sheet 표기
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", dCurPos / 1000.0);
	
		m_ssEachPkrSettingPos[nHand].SetTextMatrix(nRow, 1, strCurPos);
	}
}


void CVATMainTab_PickerAlignSetting::OnBtnClickedPrePosRead(UINT nID)
{
	int nHand = ERR_VAT_ERROR; //-1
	for (int i = 0; i < VAT_HAND_MAX; i++) {
		if (m3_nEncoderRead[i] == nID) {
			nHand = i;
			break;
		}
	}

	if (nHand == ERR_VAT_ERROR)
		return;

	ReadEncoder(nHand);
}


void CVATMainTab_PickerAlignSetting::SaveEncoder(int nHand)
{
	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//Spread Sheet상 Data Get하여 변수 저장 하는 방식.
	for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
	{
		CString strPos = _T("");
		int nRow = nAxis + 1;
		strPos = m_ssEachPkrSettingPos[nHand].GetTextMatrix(nRow, 1);

		double dPos = atof(strPos)*1000.0;
		g_VATCommonData.m_stVAT_PickerAlignSubInfo.dPreSettingPos[nHand][nAxis] = dPos;
	}

	g_VATCommonData.SaveEachPickerAlignInformation(ePickerAlign_PreSettingPos, nHand);
}

void CVATMainTab_PickerAlignSetting::OnBtnClickedPrePosSave(UINT nID)
{
	int nHand = ERR_VAT_ERROR; //-1
	for (int i = 0; i < VAT_HAND_MAX; i++) {
		if (m3_nEncoderSave[i] == nID) {
			nHand = i;
			break;
		}
	}

	if (nHand == ERR_VAT_ERROR)
		return;

	SaveEncoder(nHand);

	DisplayPrePos(nHand);
}


void CVATMainTab_PickerAlignSetting::OnBtnClickedVisionLive(UINT nID)
{
	int nHand = ERR_VAT_ERROR; //-1
	for (int i = 0; i < VAT_HAND_MAX; i++) {
		if (m3_nVisionLive[i] == nID) {
			nHand = i;
			break;
		}
	}

	if (nHand == ERR_VAT_ERROR)
		return;

	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			m_bVisionLiveStatus[nHand] = !m_bVisionLiveStatus[nHand];
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP1:
		case VAT_HAND_LOAD_TESTPP2:
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			int nOtherHand = -1;
		
			if (nHand == VAT_HAND_LOAD_TESTPP1)			nOtherHand = VAT_HAND_UNLOAD_TESTPP1;
			else if (nHand == VAT_HAND_UNLOAD_TESTPP1)	nOtherHand = VAT_HAND_LOAD_TESTPP1;
			else if (nHand == VAT_HAND_LOAD_TESTPP2)	nOtherHand = VAT_HAND_UNLOAD_TESTPP2;
			else if (nHand == VAT_HAND_UNLOAD_TESTPP2)	nOtherHand = VAT_HAND_LOAD_TESTPP2;

			//현재 꺼져 있을때(On 시도)
			if (m_bVisionLiveStatus[nHand] == eLive_Off) { 
				if (m_bVisionLiveStatus[nOtherHand] == eLive_Off) {
					m_bVisionLiveStatus[nHand] = eLive_On;
				}
			}
			else {
				m_bVisionLiveStatus[nHand] = eLive_Off;
			}
		}break;
		default:
			break;
	}

	DisplayVisionLiveStaus(nHand, m_bVisionLiveStatus[nHand]);
}



//============================================================================================================
//COMMENT : Vision Cam Offset X/Y Update 함수.
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMainTab_PickerAlignSetting::OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam)
{
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;

	std::pair<double, double> pOffset = GuiNotifier->GetVisionOffset(nCam);

// 	m_dOffsetX = pOffset.first;
// 	m_dOffsetY = pOffset.second;

	int nHand = _Com_ConvertLowerCamToHandIdx(nCam);

	//Upper Cam 상태 초기화 시, 상단 Cam 예외처리.
	if (nHand == -1) {
		return 0L;
	}

	//CVAT_PickerAlignDlgCom 변수저장.
	m_pOffset[nHand] = pOffset;

	InvalidateRect(&m_rect[nHand]);				//Offset Graphic View 새로 고침.
	InvalidateRect(&m_rectData[nHand][VAT_X]);	//Offset Data X 새로 고침.
	InvalidateRect(&m_rectData[nHand][VAT_Y]);	//Offset Data Y 새로 고침.

	return 0L;
}