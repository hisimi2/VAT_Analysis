#include "stdafx.h"
#include "VATMain_Common.h"
#include "VAT_STATIC_ID.h"

CVATMain_Common::CVATMain_Common()
{
	m_bLastExecuteStatus = FALSE;

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		//Hand Index
		m_vAllHand.push_back(nHand);

		//Hand Select
		m_bSelectedHand[nHand] = FALSE;
				
		//GUI 상태 표기 변수
		m_nGuiHandStatus[nHand] = VAT_STATUS_MEASURE_INIT;
		m_bBlinkToggle[nHand] = FALSE;
	}
}


CVATMain_Common::~CVATMain_Common()
{
}

void CVATMain_Common::_Com_MakeLog_GUI(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}

	TWLOG.MakeLog(static_cast<int>(eLogID_VAT_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}

void CVATMain_Common::_Com_InitTabControl(CTabCtrl &tab, int nTabType /*= eVatTabType_CellChange*/)
{
	tab.DeleteAllItems();

	if (nTabType == eVatTabType_CellChange)
	{
// 		TCITEM tcItem;
// 		tcItem.mask = TCIF_TEXT;
// 		tcItem.pszText = _T("Measure Position");
// 		tcItem.dwState = 1;
// 		tab.InsertItem(0, &tcItem);
// 
// 		tcItem.mask = TCIF_TEXT;
// 		tcItem.pszText = _T("Position Gap");
// 		tab.InsertItem(1, &tcItem);

		tab.InsertItem(eSSOper_ENCODER_READ, "Encoder Read");
		tab.InsertItem(eSSOper_MOTOR_MOVING, "Motor Move");
		tab.InsertItem(eSSOper_DATA_RELOAD, "Reload");
		tab.InsertItem(eSSOper_DATA_SAVE, "Save");
	}
	else //nTabType == eVatTabType_CellNotChange
	{
		tab.InsertItem(eSSOper_ENCODER_READ, "Reload");
	}

	tab.DeselectAll(FALSE); //FALSE -> 범위 모든 tab
	
	//tab.SetCurSel(0);
}


/*===============================================================*/
//COMMENT : Base Position Spread Sheet 초기화 - HandType
//          한번 초기화 후, Sheet Row/Col Head 변경 안함.
//RETURN : 
/*============================================================*/
void CVATMain_Common::_Com_InitPosSpreadSheet_HandType(CSpreadSheet &spread_sheet)
{
	spread_sheet.SetFontName(_T("Arial"));
	spread_sheet.SetFontBold(TRUE);
	spread_sheet.SetFontSize(10);
	spread_sheet.SetTypeHAlign(2); // Center
	spread_sheet.SetTypeVAlign(2); // Center

	spread_sheet.SetMaxCols(VAT_MAX_AXIS_XYZ);
	spread_sheet.SetMaxRows(VAT_HAND_MAX);
	spread_sheet.SetTextMatrix(0, 0, _T(""));

	//Col Title Setting(Axis)
	const char* szDirection[VAT_MAX_AXIS_XYZ] = { "X(mm)", "Y(mm)", "Z(mm)" };
	for (int i = 0; i < spread_sheet.GetMaxCols(); i++)
	{
		spread_sheet.SetTextMatrix(0, i + 1, szDirection[i]);
	}

	//Row Title Setting(Hand Name)
	for (int i = 0; i < spread_sheet.GetMaxRows(); i++)
	{
		spread_sheet.SetTextMatrix(i + 1, 0, _Com_convert_hand_idx_to_name(i));		
	}

	//Spread Sheet 가로 폭 조절.
	spread_sheet.SetColWidth(0, 15);
	for (int i = 1; i <= spread_sheet.GetMaxCols(); i++) {
		spread_sheet.SetColWidth(i, 10);
	}

	//Spread Sheet 세로 높이 조절.
	for (int i = 0; i <= spread_sheet.GetMaxRows(); i++) {
		spread_sheet.SetRowHeight(i, 20);
	}

	//해당 Spread Sheet Cell위로 바로 Focus됨.
	spread_sheet.SetMoveActiveOnFocus(TRUE);
}

/*===============================================================*/
//COMMENT : Base Position Spread Sheet 초기화 - PartType
//          한번 초기화 후, Hand선택에 따라 Sheet Row/Col Head 변경 됨.
//RETURN : 
/*============================================================*/
void CVATMain_Common::_Com_InitPosSpreadSheet_PartType(CSpreadSheet &spread_sheet, int nHand)
{
	//해당 Spread Sheet Cell위로 바로 Focus됨.
	spread_sheet.SetMoveActiveOnFocus(TRUE);

	spread_sheet.SetFontName(_T("Arial"));
	spread_sheet.SetFontBold(TRUE);
	spread_sheet.SetFontSize(10);
	spread_sheet.SetTypeHAlign(2); // Center
	spread_sheet.SetTypeVAlign(2); // Center

	int nMaxRow = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
	
// 	//Hide 되었던, Row Show
// 	for (int nPartIdx = 0; nPartIdx < nMaxRow; nPartIdx++) {
// 		spread_sheet.SetRowHidden(FALSE);
// 	}

	spread_sheet.SetMaxRows(nMaxRow);
	spread_sheet.SetMaxCols(VAT_MAX_AXIS_XYZ);
	
	spread_sheet.SetTextMatrix(0, 0, _T(""));

	//Col Title Setting(Axis)
	const char* szDirection[VAT_MAX_AXIS_XYZ] = { "X(mm)", "Y(mm)", "Z(mm)" };
	for (int i = 0; i < spread_sheet.GetMaxCols(); i++)
	{
		spread_sheet.SetTextMatrix(0, i + 1, szDirection[i]);
	}

	//Row Title Setting(Hand Name)
	spread_sheet.SetScrollBarShowMax(TRUE);
	for (int i = 0; i < spread_sheet.GetMaxRows(); i++)
	{
		spread_sheet.SetTextMatrix(i + 1, 0, _Com_convert_part_idx_to_name(nHand, i));
	}

	//Spread Sheet 가로 폭 조절.
	spread_sheet.SetColWidth(0, 15);
	for (int i = 1; i <= spread_sheet.GetMaxCols(); i++) {
		spread_sheet.SetColWidth(i, 10);
	}

	//Spread Sheet 세로 높이 조절.
	for (int i = 0; i <= spread_sheet.GetMaxRows(); i++) {
		spread_sheet.SetRowHeight(i, 20);
	}

 	//불필요 Part Hide
 	for (int nPartIdx = 0; nPartIdx < spread_sheet.GetMaxRows(); nPartIdx++) {
 		spread_sheet.SetRow(nPartIdx + 1);
 		if (_Com_IsNecessaryPart(nHand, nPartIdx) != TRUE) {
 			spread_sheet.SetRowHidden(TRUE);
 		}
  		else {
  			//Hidden Row Show
  			spread_sheet.SetRowHidden(FALSE); // 이거 초기에 한번 넣어줘야겠다.
  		}
  	}

	//Spread Sheet 변경 때 마다 Top Row를 첫번째 열로 고정.
	//spread sheet 좌,우 움직이지 않도록 마지막 Col 고정.
	spread_sheet.SetTopRow(0);	
}

/*================================================================================
AUTHOR   : 
NAME     :
PARAMS   :
RETURN   :
COMMENTS : 해당 spread_sheet의 선택 된 Cell Color 변경 및 LastSheet의 col, row 저장
==================================================================================*/
void CVATMain_Common::_Com_UpdateSheetCellColor(CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo, int nCol, int nRow)
{
	BOOL bIsSSTypeBasePos = (stLastSheetInfo.nSSType != eVAT_SSTYPE_NORMAL) ? TRUE : FALSE;

	//[이전 cell 원복]
	//Spread Sheet 선택 하지 않고 다른 Hand 변경 할 경우 nCol,nRow -1
	//이전 Spread Sheet 선택 하지 않았으면 White로 변경 해줄 필요 없음.
	if (stLastSheetInfo.nCol > 0 && stLastSheetInfo.nRow > 0) 
	{
		//배경색
		spread_sheet.SetCol(stLastSheetInfo.nCol);
		spread_sheet.SetRow(stLastSheetInfo.nRow);
		spread_sheet.SetBackColor(WWHITE);

// 		//글자색	
// 		if (bIsSSTypeBasePos) {
// 			double dPos = atof(spread_sheet.GetTextMatrix(stLastSheetInfo.nRow, stLastSheetInfo.nCol)) * 1000.0;
// 			BOOL bIsLimitPos = _Com_BasePositionLimitCheck(stLastSheetInfo.nHand, stLastSheetInfo.nCol - 1, dPos);
// 			OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED :NORMAL_COLOR_BLACK;
// 			spread_sheet.SetForeColor(color);
// 		}
	}

	//[선택 cell Update]
	//Data가 없는 Spread Sheet 영역 click했을 경우 색상 변경 필요 없음.
	if (nCol > 0 && nRow > 0)
	{
		//배경색
		spread_sheet.SetCol(nCol);
		spread_sheet.SetRow(nRow);

		//Limit값 글자 색상 빨간색  상시 표기를 위해 ORANGE로 변경함. [5/28/2025 dohyeong.kim]
		spread_sheet.SetBackColor(ORANGE); //RED 

// 		//글자색	
// 		if (bIsSSTypeBasePos && spread_sheet.GetForeColor() == LIMIT_COLOR_RED) {
// 			spread_sheet.SetForeColor(NORMAL_COLOR_BLACK);
// 		}

	}



}

void CVATMain_Common::_Com_UpdateLastSheetData(_stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo, int nCol, int nRow)
{
	if (nCol <= 0 || nRow <= 0)
		return;

	//마지막 Col, Row 저장.
	stLastSheetInfo.nCol = nCol;
	stLastSheetInfo.nRow = nRow;

	//Spread Sheet Hand Type은 Row가 Hand Index				-> Hand Index 상시 가변.
	//Spread Sheet Part Type은 Sheet전체가 Hand에 대한 Part  -> Sheet당 Hand 고정.
// 	if (stLastSheetInfo.nSSType == eVAT_SSTYPE_HAND)
// 	{
// 		stLastSheetInfo.nHand = nRow - 1;
// 	}
}

//=========================================================================================
// COMMENT : Spread Sheet Hand/Part Type에 따라 Hand Index 저장 시점이 달라 해당 함수 분기
//			1) Hand Type : Spread Sheet Row가 Hand Index			-> Hand Index 상시 가변.
//			2) Part Type : Spread Sheet 전체가 Hand에 대한 Part  -> Sheet당 Hand 고정.
//=========================================================================================
void CVATMain_Common::_Com_UpdateLastHandData(_stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo, int nHand)
{
	if (VAT_HAND_LOAD_TRAYPP > nHand || nHand > VAT_HAND_UNLOAD_TESTPP2)
		return;

	stLastSheetInfo.nHand = nHand;
}

//=========================================================================================
// COMMENT : Spread Sheet Hand/Part Type만 사용 가능.
//			 Normal Type 사용 X.
//=========================================================================================
void CVATMain_Common::_Com_DblClickSheetTextChange(CSpreadSheet &spread_sheet, int nHand, CWnd* cWnd, int nCol, int nRow)
{
	if (nCol == 0 || nRow == 0)
		return;

	//Positive, Negative Limit 표기.
	char strPosSWLimit[128];
	char strNegSWLimit[128];

	int nPhysicalNo = _Com_ConvertAxisToMotorPhysicalNo(nHand, nCol - 1); //nHand, nAxis

	double dPosSwLimit = g_Motor[nPhysicalNo]->GetSwLimitPosi() / 1000.0;
	double dNegSwLimit = g_Motor[nPhysicalNo]->GetSwLimitNega() / 1000.0;

	sprintf(strPosSWLimit, "%.3f", dPosSwLimit);
	sprintf(strNegSWLimit, "%.3f", dNegSwLimit);

	char szInputValue[VAT_STRING_SIZE] = { 0, };
	char szCurrentValue[VAT_STRING_SIZE] = { 0, };

	CString strCurrentValue = spread_sheet.GetTextMatrix(nRow, nCol);
	sprintf_s(szCurrentValue, sizeof(szCurrentValue), "%s", strCurrentValue.GetString());

	int nRet = g_LibCommCtrl.GetNumberBox(cWnd, szInputValue, 15, szCurrentValue, "Target Pos", strPosSWLimit, strNegSWLimit);
	if (nRet != TRUE)
		return;

	spread_sheet.SetTextMatrix(nRow, nCol, szInputValue);

	//Limit Check 완료 된 상태.
	spread_sheet.SetForeColor(NORMAL_COLOR_BLACK);
}

//=========================================================================================
// COMMENT : Cell Block "NONE"처리.
//			 Data Display 이후, 사용.
//=========================================================================================
void CVATMain_Common::_Com_BlockCell(CSpreadSheet &spread_sheet, int nHand)
{
	//Tray Load PP, Y축 Block 진행.
	int nCol = 2;
	spread_sheet.SetCol(nCol);
	for(int nRow  = 1; nRow <= spread_sheet.GetMaxRows(); nRow++)
	{
		spread_sheet.SetRow(nRow);
		   			
		if (nHand == VAT_HAND_LOAD_TRAYPP)
		{
// 			if (nRow == VAT_IDX_TRAY_PP_FEEDER + 1) { //Feeder
// 				continue;
// 			}

			spread_sheet.SetBackColor(DDARKGRAY);
			spread_sheet.SetForeColor(WWHITE);
			spread_sheet.SetTextMatrix(nRow, nCol, "NONE");
		}
 		else
 		{
 			//다른 Hand Spread Sheet 원복.
 			spread_sheet.SetBackColor(WWHITE);
			
			//글자 색상은 Display Data에서 Limit값 여부에 따라 이미 표기 진행.
 		}
	}
}


BOOL CVATMain_Common::_Com_CheckBlockCell(int nHand, int nCol)
{
	//Tray Load PP, Y축 Blocking
	if (nHand == VAT_HAND_LOAD_TRAYPP && nCol == 2)
		return TRUE;

	return FALSE;
}


BOOL CVATMain_Common::_Com_CheckBlockCell(int nHand, int nCol, int nRow)
{
	//Tray Load Pp, Feeder 제외 Y축 Blocking
	if (nHand == VAT_HAND_LOAD_TRAYPP && nCol == 2 && nRow == 3)
		return TRUE;

	return FALSE;
}

/*===============================================================*/
//COMMENT : Spread Sheet의 Cell 위치를 모터 Physical No으로 변환.
//          nCol은 Axis No으로 사용.
//RETURN : Motor Physical No.
/*============================================================*/
int CVATMain_Common::_Com_ConvertAxisToMotorPhysicalNo(int nHand, int nAxis)
{
	int nPhysicalNo = -1;
	
	//Hand XYZ 정보 가져옴.
	ST_VAT_HAND_XYZ_INFO HandXYZInfo;

	auto it = cHandXYZParamMap.find(nHand);
	if (it != cHandXYZParamMap.end()) {
		HandXYZInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	if (nAxis == VAT_X)			nPhysicalNo = HandXYZInfo.x_axis_no;
	else if (nAxis == VAT_Y)	nPhysicalNo = HandXYZInfo.y_axis_no;
	else if (nAxis == VAT_Z)	nPhysicalNo = HandXYZInfo.z_axis_no;

	return nPhysicalNo;


// 	switch (nHand)
// 	{
// 	case VAT_HAND_LOAD_TRAYPP:
// 	{
// 		if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TRAY_PP_1_X;
// 		else if (nAxis == VAT_Y)		nPhysicalNo = eAXIS_TRAY_FEEDER_Y;
// 		else 							nPhysicalNo = eAXIS_TRAY_PP_1_Z;
// 	}break;
// 	case VAT_HAND_UNLOAD_TRAYPP:
// 	{
// 		if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TRAY_PP_2_X;
// 		else if (nAxis == VAT_Y)		nPhysicalNo = eAXIS_TRAY_PP_2_Y;
// 		else							nPhysicalNo = eAXIS_TRAY_PP_2_Z;
// 	}break;
// 	case VAT_HAND_LOAD_TESTPP1:
// 	{
// 		if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_1_X;
// 		else if (nAxis == VAT_Y)		nPhysicalNo = eAXIS_TEST_PP_1_Y;
// 		else							nPhysicalNo = eAXIS_TEST_PP_1_Z;
// 	}break;
// 	case VAT_HAND_UNLOAD_TESTPP1:
// 	{
// 		if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_1_X2;
// 		else if (nAxis == VAT_Y)		nPhysicalNo = eAXIS_TEST_PP_1_Y;
// 		else							nPhysicalNo = eAXIS_TEST_PP_1_Z2;
// 	}break;
// 	case VAT_HAND_LOAD_TESTPP2:
// 	{
// 		if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_2_X;
// 		else if (nAxis == VAT_Y)		nPhysicalNo = eAXIS_TEST_PP_2_Y;
// 		else							nPhysicalNo = eAXIS_TEST_PP_2_Z;
// 	}break;
// 	case VAT_HAND_UNLOAD_TESTPP2:
// 	{
// 		if (nAxis == VAT_X)				nPhysicalNo = eAXIS_TEST_PP_2_X2;
// 		else if (nAxis == VAT_Y)		nPhysicalNo = eAXIS_TEST_PP_2_Y;
// 		else							nPhysicalNo = eAXIS_TEST_PP_2_Z2;
// 	}break;
// 	default:
// 	{
// 	}break;
// 	}

//	return nPhysicalNo;
}

/*=====================================================================================*/
//Oper

int CVATMain_Common::_Com_Oper_EncoderRead(double dPos[], _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, int nSSDisplayType)
{
	//[Interlock]
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return ERR_VAT_ERROR;

	//"Do you want to read encoder?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_ENCODER_READ, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return ERR_VAT_ERROR;

	//[Interlock]
	bChkInterlock = _Com_SSInterlock_EncoderRead(stLastSheetInfo, nSSDisplayType);
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return ERR_VAT_ERROR;

	//int nHandIdx = stLastSheetInfo.nRow - 1;
	int nHandIdx = stLastSheetInfo.nHand;

	//Encoder Read(VAT_X, VAT_Y, VAT_Z)
	for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
	{
		int nPhysicalNo = _Com_ConvertAxisToMotorPhysicalNo(nHandIdx, nAxis);
		if (nAxis == VAT_Z/*eAXIS_TRAY_PP_1_Z <= nPhysicalNo && nPhysicalNo <= eAXIS_TEST_PP_2_Z2*/)
		{
			//Read한 Encoder는 Cam 초점 맞는 위치이므로, Z축은 기준 Picker Encoder값을 가져옴.
			int nBBxIdx = 0;
			switch (nPhysicalNo)
			{
 			case eAXIS_TRAY_PP_1_Z:		nBBxIdx = eBMAxis_LOAD_TRAY_Picker07;		break;
 			case eAXIS_TEST_PP_1_Z:		nBBxIdx = eBMAxis_LOAD_TESTPP_1_Picker03;	break;
 			case eAXIS_TEST_PP_1_Z2:	nBBxIdx = eBMAxis_UNLOAD_TESTPP_1_Picker02;	break;
 			case eAXIS_TRAY_PP_2_Z:		nBBxIdx = eBMAxis_LOAD_TESTPP_2_Picker02;		break;
 			case eAXIS_TEST_PP_2_Z:		nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker03;	break;
 			case eAXIS_TEST_PP_2_Z2:	nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker08;	break;
 			default:
 				break;
			}

			g_Motor[nPhysicalNo]->GetCurrentPos(dPos[VAT_Z], _eBbxmnpAxis(nBBxIdx));
		}
		else // X, Y
		{
			(nAxis == VAT_X) ? g_Motor[nPhysicalNo]->GetCurrentPos(dPos[VAT_X]) : g_Motor[nPhysicalNo]->GetCurrentPos(dPos[VAT_Y]);
		}
		//참고.
		//	dCurPos -= g_Motor[nAxisNo]->GetLastBacklashOffset();

	}	

	return ERR_VAT_NO_ERROR;
}


//===========================================================================
//COMMENT : 1ea Part에 대해 X,Y,Z축 Position 생성.
//===========================================================================
void CVATMain_Common::_Com_Oper_TeachingTableRead(double dPos[], int nHand, int nPart)
{
	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			int nLocStageConv = g_TaskTrayLoadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_1, nPart);
			dPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), nLocStageConv, POS_IDX_TRAY_LOAD_PP_X);
			dPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Z_SAFETY);
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			int nLocStageConv = g_TaskTrayUnloadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_2, nPart);
			dPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_PP_X);
			dPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_PP_Y);
			dPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_PP_Z_SAFETY);
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP1:
		case VAT_HAND_LOAD_TESTPP2:
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			int nTestPpIdx = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_UNLOAD_TESTPP1) ? RIDX_TEST_PP_1 : RIDX_TEST_PP_2;
			int nHandType = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_TESTPP_LOAD : VAT_TESTPP_UNLOAD;

			dPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(nTestPpIdx), nPart, (nHandType == VAT_TESTPP_LOAD) ? POS_IDX_TEST_PP_X1 : POS_IDX_TEST_PP_X2);
			dPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(nTestPpIdx), nPart, (nHandType == VAT_TESTPP_LOAD) ? POS_IDX_TEST_PP_Y1 : POS_IDX_TEST_PP_Y2);
			dPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(nTestPpIdx), nPart, (nHandType == VAT_TESTPP_LOAD) ? POS_IDX_TEST_PP_Z1_SAFETY : POS_IDX_TEST_PP_Z2_SAFETY);
		}break;
		default:
			break;
	}	
}



//===========================================================================
//COMMENT : 1) Hand Type - Reload시 사용.
//				- Click된 Cell 색상 변경(O)
//				- nRow, nCol 초기화(O)	
//				- nHand 초기화(O)
//				- nSSType 초기화 안함.(X)
//
//			2) Part Type - Reload 또는 Hand Select 변경 시 사용.
//				- Click된 Cell 색상 변경(O)
//				- nRow, nCol 초기화(O)	
//				- nHand 초기화 안함.(X)
//				- nSSType 초기화 안함.(X)
//			3) Normal Type - Hand Select 변경 시 사용.
//				- Click된 Cell 색상 변경(O)				
//				- nRow, nCol 초기화(O)	
//				- nHand 초기화 안함.(Normal Type에서는 Hand 사용 안함.)
//===========================================================================
void CVATMain_Common::_Com_Clear_SpreadSheet(CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo)
{
	//1. 이전 cell 원복 및 초기화.
	if (stLastSheetInfo.nCol > 0 && stLastSheetInfo.nRow > 0) //-1 방지 -> 이전에 아무것도 안눌렀을 경우
	{
		spread_sheet.SetCol(stLastSheetInfo.nCol);
		spread_sheet.SetRow(stLastSheetInfo.nRow);
		spread_sheet.SetBackColor(WWHITE);
	}

	stLastSheetInfo.nCol = -1;
	stLastSheetInfo.nRow = -1;

	if (stLastSheetInfo.nSSType == eVAT_SSTYPE_HAND)
	{
		stLastSheetInfo.nHand = -1;
	}
	else if (stLastSheetInfo.nSSType == eVAT_SSTYPE_PART)
	{
		//stLastSheetInfo.nHand = -1;
	}
	else if (stLastSheetInfo.nSSType == eVAT_SSTYPE_PART_ONLY)
	{

	}
	else if (stLastSheetInfo.nSSType == eVAT_SSTYPE_NORMAL)
	{

	}
	else
	{
		AfxMessageBox("Spread Sheet Type ERROR");
	}
}


//===========================================================================
//COMMENT: Resource ID를 Hand Idx로 변환 함수.
//			ON_CONTROL_RANGE를 사용하기 위함.
//===========================================================================
int CVATMain_Common::_Com_ConvertResourceIdToHandIdx(UINT nID, int nStartId, int nEndId)
{
	ASSERT(nStartId <= nID && nID <= nEndId);

	int nRetHandIdx = nID - nStartId;

	if (nRetHandIdx < VAT_HAND_LOAD_TRAYPP || nRetHandIdx > VAT_HAND_UNLOAD_TESTPP2) {
		return -1;
	}

	return nRetHandIdx;
}

/*===============================================================*/
//COMMENT : Spread Sheet 동작 Alarm 비활성화 Check.
/*============================================================*/
_eVAT_INTERLOCK CVATMain_Common::_Com_SSInterlock_AlarmDeActivate()
{
	BOOL bAlarmActivate = g_Error.IsVATAlarmActivate();
	if (bAlarmActivate == TRUE) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Please Alarm Clear First!!");
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_ALARM_DEACTIVATE, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}

	return eVAT_INTERLOCK_PASS;
}


/*===============================================================*/
//COMMENT : Hand에 대한 원점 확인.(X, Y, Z)
/*============================================================*/
_eVAT_INTERLOCK CVATMain_Common::_Com_SSInterlock_Origin(int nHand)
{
	for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
	{
		int nPhysicalNo = _Com_ConvertAxisToMotorPhysicalNo(nHand, nAxis);
		BOOL bIsOrgComplete = g_Motor[nPhysicalNo]->IsOrgCompleted(nullptr);
		if (bIsOrgComplete != TRUE)
		{
			CString strHand = _Com_convert_hand_idx_to_name(nHand);
			CString strAxis = _Com_convert_hand_axis_to_name(nAxis);

			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("[%s][%s] Motor Not Origin Check.", strHand, strAxis);
			g_Error.AlarmReport(ERR_VAT_NOT_ORIGIN_CHECK, AlarmData, NULL);
			return eVAT_INTERLOCK_FAIL;
		}
	}

	return eVAT_INTERLOCK_PASS;
}


_eVAT_INTERLOCK CVATMain_Common::_Com_SSInterlock_DblClick(int nCol, int nRow, int nSSDisplayType)
{
	//Alarm 발생 안함.
	if (nCol == 0 || nRow == 0)
		return eVAT_INTERLOCK_FAIL;

	if (nSSDisplayType != eVAT_SSDISPLAYTYPE_DATA)
		return eVAT_INTERLOCK_FAIL;

	return eVAT_INTERLOCK_PASS;
}


_eVAT_INTERLOCK CVATMain_Common::_Com_SSInterlock_EncoderRead(_stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, int nSSDisplayType)
{
	ASSERT(eVAT_SSTYPE_HAND <= stLastSheetInfo.nSSType && stLastSheetInfo.nSSType <= eVAT_SSTYPE_PART);

	//1. Data Show Type 확인.
	if (nSSDisplayType != eVAT_SSDISPLAYTYPE_DATA)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_SELECT_RIGHT_SPREAD_SHEET_TYPE, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}

	//[Interlock] Spread Sheet Unselect 확인.
	if (stLastSheetInfo.nHand == -1 || stLastSheetInfo.nCol == -1 || stLastSheetInfo.nRow == -1) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_UNSELECT_SPREAD_SHEET, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}


// 	//2. nCol 확인(Axis)
// 	int nAxis = stLastSheetInfo.nCol - 1;
// 	if (nAxis != VAT_X && nAxis != VAT_Y && nAxis != VAT_Z)
// 	{
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(ERR_VAT_INTERLOCK_UNSELECT_SPREAD_SHEET, AlarmData, NULL);
// 		return eVAT_INTERLOCK_FAIL;
// 	}
// 
// 	//3. nRow 확인(Hand or Part)	
// 	int nUnit = stLastSheetInfo.nRow - 1;
// 	if (stLastSheetInfo.nSSType == eVAT_SSTYPE_HAND)
// 	{		
// 		//nUnit == Hand
// 		if (nUnit < VAT_HAND_LOAD_TRAYPP || nUnit > VAT_HAND_UNLOAD_TESTPP2) {
// 			CParamList AlarmData;
// 			g_Error.AlarmReport(ERR_VAT_INTERLOCK_UNSELECT_SPREAD_SHEET, AlarmData, NULL);
// 			return eVAT_INTERLOCK_FAIL;
// 		}
// 	}
// 	else
// 	{
// 		//nUnit == PART
// 		BOOL bIsSelectPart = FALSE;
// 
// 		//가독성을 위해 If ~ else로 분기.
// 		if ((stLastSheetInfo.nHand == VAT_HAND_LOAD_TRAYPP || stLastSheetInfo.nHand == VAT_HAND_UNLOAD_TRAYPP))
// 		{
// 			if (VAT_IDX_TRAY_PP_TBL_1 <= nUnit && nUnit <= VAT_IDX_TRAY_PP_FEEDER) {
// 				bIsSelectPart = TRUE;
// 			}
// 		}
// 		else
// 		{
// 			if (VAT_IDX_STATION_1 <= nUnit && nUnit <= VAT_IDX_CLEAN_TBL) {
// 				bIsSelectPart = TRUE;
// 			}
// 		}
// 
// 		if (bIsSelectPart != TRUE) {
// 			CParamList AlarmData;
// 			g_Error.AlarmReport(ERR_VAT_INTERLOCK_UNSELECT_SPREAD_SHEET, AlarmData, NULL);
// 			return eVAT_INTERLOCK_FAIL;
// 		}
// 	}

	//4. 원점 Check 확인.
	if (_Com_SSInterlock_Origin(stLastSheetInfo.nHand) != eVAT_INTERLOCK_PASS)
	{
		return eVAT_INTERLOCK_FAIL;
	}



	return eVAT_INTERLOCK_PASS;
}


//====================================================================================
//COMMENT : Mode에 따른 Cmd Picker 생성.
//====================================================================================
void CVATMain_Common::_Com_ManualMove(int nMode, CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, int nSSDisplayType /*=eVAT_SSDISPLAYTYPE_DATA*/)
{
	//[Interlock]
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Encoder Move?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_MOTOR_MOVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//1. Data Show Type 확인.
	if (nSSDisplayType != eVAT_SSDISPLAYTYPE_DATA)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_SELECT_RIGHT_SPREAD_SHEET_TYPE, AlarmData, NULL);
		return ;
	}

	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);

	//[Interlock] Spread Sheet Unselect 확인.
	if (stLastSheetInfo.nHand == -1 || stLastSheetInfo.nCol == -1 || stLastSheetInfo.nRow == -1) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_UNSELECT_SPREAD_SHEET, AlarmData, NULL);
		return;
	}

	//[Interlock] 실행 중, Manual Cmd 확인.
 	if (g_bVATStatusExecute) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("manual command is Excuting!! please wait working finish!!!!");
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_MANUAL_CMD_IS_WORKING, AlarmData, NULL);		
 		return;
 	}

	//[Interlock] Cover 확인.
	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}
	
	//[HAND]
	int nHand = stLastSheetInfo.nHand;

	//[Interlock] MultiOrg 확인.(X, Y, Z)
	_eVAT_INTERLOCK eAxisOrgCheck = _Com_SSInterlock_Origin(nHand);
	if (eAxisOrgCheck != eVAT_INTERLOCK_PASS){
		return; //함수 내부 Alarm
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
				return;
			}

			//Safety Pos
			if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press%d is not Safety Position.", nPressIdx);
				g_Error.AlarmReport(ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx, AlarmData, NULL);
				return;
			}
		}
	}

	//[Picker]
	std::vector<CPoint> vPicker;
	vPicker.clear();
	_Com_MakeCmdPicker(nMode, nHand, vPicker);

	//[Position]
	double dCmdPos[VAT_MAX_AXIS] = { 0, };
	_Com_MakeCmdPosition(spread_sheet, stLastSheetInfo, dCmdPos);

	//[Eng Option Move] - SW 검증용 Operation Button
	_Com_MakeCmdPositionForEngOpt(nMode, stLastSheetInfo, dCmdPos);

	//[Interlock] Position Pos/Neg Limit Check
	std::pair<int, int> pairResult = _Com_CmdPositionLimitCheck(nHand, dCmdPos);
	if (pairResult.first != ERR_VAT_NO_ERROR) {
		int nAxis = pairResult.second;
		CString strAxis = _T("");

		if (nAxis == VAT_X)			strAxis = _T("X");
		else if (nAxis == VAT_Y)	strAxis = _T("Y");
		else						strAxis = _T("Z");

		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("[Axis %s] Position is Over Range.", strAxis);
		g_Error.AlarmReport(pairResult.first, AlarmData, NULL);
		return;
	}

	//[Position for LOG] 
	CString strCmdStage = _T("");
	_Com_MakeCmdPosition_Name(stLastSheetInfo, strCmdStage);


	int nErr = -1;
	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(nMode);
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_vCmdPicker = vPicker;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

// 			CVatTrayLoadPPCmd_MovePos* pSeqCmd = new CVatTrayLoadPPCmd_MovePos;
// 			pSeqCmd->m_nMode = nMode;
// 			pSeqCmd->m_vCmdPicker = vPicker;
// 			pSeqCmd->m_strCmdStage = strCmdStage;
// 			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

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

void CVATMain_Common::_Com_MakeCmdPicker(int nMode, int nHand, std::vector<CPoint> &vPicker)
{
	switch (nMode)
	{
		//기준 Picker
		case VAT_SEQ_MODE_Z_FOCUS:
		case VAT_SEQ_MODE_CAM_CAL:
		{
			CPoint cStdPicker;
			cStdPicker.x = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].first;
			cStdPicker.y = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].second;

			vPicker.push_back(cStdPicker);
		}break;
		case VAT_SEQ_MODE_PICKER_ALIGN:
		case VAT_SEQ_MODE_VAT:
		{

		}break;
		// All Picker
		case VAT_SEQ_MODE_Z_TEACHING:
		{
			int nMAX_PICKER_X = MAX_PICKER_X_CNT;
			int nMAX_PICKER_Y = MAX_PICKER_Y_CNT;
			if (!(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)) {
				nMAX_PICKER_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			}

			for (int nPkrY = 0; nPkrY < nMAX_PICKER_Y; nPkrY++) {
				for (int nPkrX = 0; nPkrX < nMAX_PICKER_X; nPkrX++) {
					vPicker.push_back(CPoint(nPkrX, nPkrY));
				}
			}

		}break;
		default:
			break;
	}
}


//====================================================================================
//COMMENT : Cmd Position 생성.
//			Spread Sheet 참조하여, XYZ data값 불러옴.
//			Teaching Table 및 Limit 값에서 X Pitch, Y Pitch 불러옴.
//====================================================================================
void CVATMain_Common::_Com_MakeCmdPosition( CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, double dCmdPos[])
{
	//double dCmdPos[VAT_MAX_AXIS] = { 0, };
	int nRow = stLastSheetInfo.nRow;
	int nCol = stLastSheetInfo.nCol;
	int nHand = stLastSheetInfo.nHand;

	if (nRow == -1 || nCol == -1)
		return;

	//[X, Y , Z]
	for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) { //X, Y, Z => spread sheet에서 가져옴.
		dCmdPos[i] = atof(spread_sheet.GetTextMatrix(nRow, i + 1) ) * 1000.0;
	}
	

	//Hand Pitch 정보 가져옴.
	ST_VAT_HAND_PITCH_INFO HandPitchInfo;

	auto it = cHandPitchParamMap.find(nHand);
	if (it != cHandPitchParamMap.end()) {
		HandPitchInfo = it->second;
	}
	else {
		ASSERT(0);
	}


	int nLocStage = -1;
	if (stLastSheetInfo.nSSType == eVAT_SSTYPE_HAND){	
		dCmdPos[VAT_PITCH_X] = g_Motor[HandPitchInfo.xp_axis_no]->GetSwLimitPosi() - 1000.0;
		dCmdPos[VAT_PITCH_Y] = g_Motor[HandPitchInfo.yp_axis_no]->GetSwLimitPosi() - 1000.0;
	}
	else{ 
		nLocStage = stLastSheetInfo.nRow - 1;

		// LD/UD Tray PP만 m_nCurLoc Convert 필요함.
		CTaskTrayBasePP* pTaskTray = nullptr;
		if (nHand == VAT_HAND_LOAD_TRAYPP) {
			pTaskTray = &g_TaskTrayLoadPP;
		}
		else if (nHand == VAT_HAND_UNLOAD_TRAYPP) {
			pTaskTray = &g_TaskTrayUnloadPP;
		}

		if (pTaskTray != nullptr) {
			int nLocStageConv = pTaskTray->TrayHandLocIdxConvertor((nHand == VAT_HAND_LOAD_TRAYPP) ? RIDX_TRAY_PP_1 : RIDX_TRAY_PP_2, nLocStage);
			nLocStage = nLocStageConv;
		}

		//X-Pitch, Y-Pitch
		dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, nLocStage, HandPitchInfo.xp_pos_idx);
		dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, nLocStage, HandPitchInfo.yp_pos_idx);
	}	
}

//===================================================================================
//COMMENT : Cmd Target Stage LOG 기록을 위함.
//			Hand/Part 측정 Mode에 따라 Target Stage가 다름.
//===================================================================================
void CVATMain_Common::_Com_MakeCmdPosition_Name(_stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, CString &strCmdStage)
{
	//Spread Sheet Hand Type
	if (stLastSheetInfo.nSSType == eVAT_SSTYPE_HAND)
	{
		strCmdStage = _T("Target");
	}
	else //Spread Sheet Part Type
	{
		int nHand = stLastSheetInfo.nHand;
		int nPart = stLastSheetInfo.nRow - 1;
		
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:
			strCmdStage = g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)nPart);
			break;
		case VAT_HAND_UNLOAD_TRAYPP:
			strCmdStage = g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)nPart);
			break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP1:
			strCmdStage = g_TaskTestPP[eTestPP_1].GetLocName(nPart);
			break;
		case VAT_HAND_LOAD_TESTPP2:
		case VAT_HAND_UNLOAD_TESTPP2:
			strCmdStage = g_TaskTestPP[eTestPP_2].GetLocName(nPart);
			break;
		default:
			break;
		}
	}
}


//1. 이거 작업 필요 함.
//2. Step이동 필요.
void CVATMain_Common::_Com_MakeCmdPositionForEngOpt(int nMode, _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, double dCmdPos[])
{
	if (!g_DMCont.m_dmEQP.GN(NDM0_TEST_SW_ENGINEER_OPTION)) {
		return;
	}

	switch (nMode)
	{
		case VAT_SEQ_MODE_CAM_CAL:
		{
			//Engineer Type이 아닐 경우, Operate 동작 정상
			if (stLastSheetInfo.nSSType != eVAT_SSTYPE_ENGINEER) {
				return;
			}

			int nHand = stLastSheetInfo.nHand;

			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
				double dZCamFocusPos = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis];
			
				if (nAxis == VAT_Z) {
					dCmdPos[nAxis] = dZCamFocusPos;
				}
				else {
					//[상단 Cam Plate Jig Center Pos] + [기준 Picker ~ 상단 Cam]
					dCmdPos[nAxis] += dZCamFocusPos;
				}
			}
		}break;
	default:
		break;
	}
}

//===================================================================================
//COMMENT : Cmd Target Position Pos/Neg Limit Check
//			파라미터 dPos 변경 불가.
//			X,Y,Z,XP,YP 모든 축에 대한 Limit Check
//RETURN  : frist = ERROR Message
//			secont = 해당 Axis가 Positive Limit 또는 Negative Limit이면 해당 축 return
//===================================================================================
std::pair<int, int> CVATMain_Common::_Com_CmdPositionLimitCheck(int nHand, const double dPos[])
{
	std::pair<int, int> p;
	p.first = ERR_VAT_NO_ERROR;
	p.second = -1;

	for (int axis = 0; axis < VAT_MAX_AXIS_XYZ; axis++)
	{
		int nPhysicalNo = _Com_ConvertAxisToMotorPhysicalNo(nHand, axis); //nHand, nAxis
	
		if (nPhysicalNo != -1)
		{
			//Positive Limit Check
			double dPosSwLimit = g_Motor[nPhysicalNo]->GetSwLimitPosi();
			if (dPos[axis] > dPosSwLimit) {
				p.first = ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
				p.second = axis;
				return p;
			}

			//Negative Limit Check
			double dNegSwLimit = g_Motor[nPhysicalNo]->GetSwLimitNega();
			if (dPos[axis] < dNegSwLimit) {
				p.first = ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;
				p.second = axis;
				return p;
			}
		}

	}
	
	return p;

}


CString CVATMain_Common::_Com_convert_hand_axis_to_name(int nAxis)
{
	CString strRet = _T("");

	switch (nAxis)
	{
	case VAT_X:			strRet = _T("X Axis"); break;
	case VAT_Y:			strRet = _T("Y Axis"); break;
	case VAT_Z:			strRet = _T("Z Axis"); break;
	case VAT_PITCH_X:	strRet = _T("X Pitch Axis"); break;
	case VAT_PITCH_Y:	strRet = _T("Y Pitch Axis"); break;
	default:
		break;
	}

	return strRet;
}


BOOL CVATMain_Common::_Com_BasePositionLimitCheck(int nHand, int nAxis, double dPos)
{
	//Pos/Neg Limit 확인 및 색상 변경.
	int nPhysicalNo = _Com_ConvertAxisToMotorPhysicalNo(nHand, nAxis); //nHand, nCol

	if (nPhysicalNo != -1)
	{
		double dPosSwLimit = g_Motor[nPhysicalNo]->GetSwLimitPosi();
		double dNegSwLimit = g_Motor[nPhysicalNo]->GetSwLimitNega();

		if (dPos > dPosSwLimit || dPos < dNegSwLimit) {
			return TRUE;
		}
	}

	return FALSE;
}


void CVATMain_Common::_Com_InitProgressBar()
{
	//Progress
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_Progress[nHand].SetRange(0, 100);
		m_Progress[nHand].SetBkColor(PROGRESS_BACK_COLOR);
		m_Progress[nHand].SetBarColor(PROGRESS_BAR_COLOR);

		m_Percent[nHand].SetWindowText(_T("0%"));
	}
}

//Cam Status -> Bmp Frame Index 변환
int CVATMain_Common::_Com_ConvertCamStatusToFrameIdx(int nStatus)
{
	int nFrame = 0;
	switch (nStatus)
	{
	case VAT_RCV_STATUS_INIT:				nFrame = 1; break;
	case VAT_RCV_STATUS_IDLE:				nFrame = 2; break;
	case VAT_RCV_STATUS_DISCONNECT:			nFrame = 3; break;
	case VAT_RCV_STATUS_SUCCESS:			nFrame = 4; break;
	case VAT_RCV_STATUS_ERROR:				nFrame = 3; break;
	default:
		break;
	}

	return nFrame;
}

//==============================================================================
// COMMENT : Lower Cam Idx -> Hand Idx
//           ※특이사항
//				-. Test PP 하단 Cam은 1개이지만 GUI는 LD/UD 나눠서 표기함.
//==============================================================================
int CVATMain_Common::_Com_ConvertLowerCamToHandIdx(int nCam)
{
	int nHand = -1;

	switch (nCam)
	{
	case VAT_CAM_LD_TRAY_PP_LOWER:
	{
		nHand = VAT_HAND_LOAD_TRAYPP;
	}break;
	case VAT_CAM_UD_TRAY_PP_LOWER:
	{

		nHand = VAT_HAND_UNLOAD_TRAYPP;
	}break;
	case VAT_CAM_TEST_PP_1_LOWER:
	{
		int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(VAT_TESTPP_LOAD);
		if (nHandOwner == -1) 
			break;

		nHand = (nHandOwner == VAT_TESTPP_LOAD) ? VAT_HAND_LOAD_TESTPP1 : VAT_HAND_UNLOAD_TESTPP1;


// 		if (m_Progress[VAT_HAND_LOAD_TESTPP1].GetPos() >= 100) {
// 			nHand = VAT_HAND_UNLOAD_TESTPP1;
// 		}
// 		else {
// 			nHand = VAT_HAND_LOAD_TESTPP1;
// 		}
	}break;
	case VAT_CAM_TEST_PP_2_LOWER:
	{
		int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(VAT_TESTPP_UNLOAD);
		if (nHandOwner == -1)
			break;

		nHand = (nHandOwner == VAT_TESTPP_LOAD) ? VAT_HAND_LOAD_TESTPP2 : VAT_HAND_UNLOAD_TESTPP2;

// 		if (m_Progress[VAT_HAND_LOAD_TESTPP2].GetPos() >= 100) {
// 			nHand = VAT_HAND_UNLOAD_TESTPP2;
// 		}
// 		else {
// 			nHand = VAT_HAND_LOAD_TESTPP2;
// 		}
	}break;
	default:
		break;
	}

	return nHand;
}


/*===============================================================*/
//COMMENT : Auto Sequence에서 Hand 상태 변화 시, Hand Status 표기
//			OnTimer에서 사용
//RETURN : 
/*============================================================*/
void CVATMain_Common::_Com_tDisplayHandStatus(CEditTrans editPos[])
{
	//시퀀스 측정일 경우에만 상태 표기 실시간 진행.
	int nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();
	if (nCurLotState == eAutoLotState_InitNeed) {
		return;
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		if (!(VAT_STATUS_MEASURE_INIT <= m_nGuiHandStatus[nHand] && m_nGuiHandStatus[nHand] <= VAT_STATUS_DATA_SAVE_COMPLETE)) {
			return;
		}

		ULONG ulPoscolor;
		switch (m_nGuiHandStatus[nHand])
		{
		case VAT_STATUS_MEASURE_INIT:
		{
			ulPoscolor = WWHITE;
		}break;
		case VAT_STATUS_MEASURE_READY:
		case VAT_STATUS_MEASURE_WAIT:
		{
			ulPoscolor = LLIGHTBLUE;
		}break;
		case VAT_STATUS_MEASURE_ONGOING:
		{
			m_bBlinkToggle[nHand] = !m_bBlinkToggle[nHand];

			//깜빡임으로 측정 진행 Hand 표기.
			//COLORREF color = editPos[nHand].GetBackColor();
			if (m_bBlinkToggle[nHand]/*color == LLIGHTBLUE*/) {
				ulPoscolor = STATUS_BLINK_COLOR;
			}
			else {
				ulPoscolor = LLIGHTBLUE;
			}
		}break;
		case VAT_STATUS_MEASURE_COMPLETE:
		case VAT_STATUS_DATA_SAVE_COMPLETE:
		{
			ulPoscolor = YELLOW;
		}break;
		default:
			ulPoscolor = WWHITE;		
			break;
		}

		if (editPos[nHand].GetBackColor() != ulPoscolor) {
			editPos[nHand].SetBackColor(ulPoscolor);
			editPos[nHand].Invalidate(FALSE); //배경만 repaint
		}
	}
}

//Title Panel 1개 색상 처리.
void CVATMain_Common::_Com_InitSSTitlePanel(CDSSPanel &panel)
{
	panel.SetForeColor(BBLACK);
	panel.SetBackColor(LLIGHTCYAN);
}

//Data관련 Panel 전체 색상 처리.
void CVATMain_Common::_Com_InitSSDataPanel(CDSSPanel &panelTitle, CDSSPanel &panelSub1, CDSSPanel &panelSub2, _eVAT_SPREAD_SHEET_ACCESS_TYPE eAccessType/* = eVAT_SSACCESSTYPE_RW*/)
{
	panelTitle.SetForeColor(BBLACK);
	panelSub1.SetForeColor(BBLACK);
	panelSub2.SetForeColor(BBLACK);

	OLE_COLOR color = (eAccessType == eVAT_SSACCESSTYPE_RO) ? SHEET_READ_ONLY_COLOR : LLIGHTCYAN;

	//RGB(110, 180, 180)
	//RGB(90, 150, 150) -> 조금더 어두움

	panelTitle.SetBackColor(color);
	panelSub1.SetBackColor(color);
	panelSub2.SetBackColor(TAB_BACKGROUND_COLOR);
}

void CVATMain_Common::_Com_ChangeSSPanelColor(CDSSPanel &panelSub1, CDSSPanel &panelSub2, int nSSDisplayType, _eVAT_SPREAD_SHEET_ACCESS_TYPE eAccessType/* = eVAT_SSACCESSTYPE_RW*/)
{
	OLE_COLOR color = (eAccessType == eVAT_SSACCESSTYPE_RO) ? SHEET_READ_ONLY_COLOR : LLIGHTCYAN;

	if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA)
	{
		panelSub1.SetBackColor(color);
		panelSub2.SetBackColor(TAB_BACKGROUND_COLOR);
	}
	else // nDataType == eVAT_SSDISPLAYTYPE_DIFFERENCE
	{
		panelSub1.SetBackColor(TAB_BACKGROUND_COLOR);
		panelSub2.SetBackColor(color);
	}
}


void CVATMain_Common::_Com_InitOperBtn(CShadeButtonST &button, int nType)
{
 	CFont BtnFont;
 
	BtnFont.CreatePointFont(100, _T("Arial"));
 	button.SetFont(&BtnFont);

	switch (nType)
	{
		case eSSOper_ENCODER_READ:
		{		
			button.SetWindowText(_T("READ"));
			button.SetIcon(IDI_BTN_NEW);
		}break;
		case eSSOper_MOTOR_MOVING:
		{
			button.SetWindowText(_T("MOVE"));
			button.SetIcon(IDI_BTN_BASE);
		}break;
		case eSSOper_DATA_RELOAD:
		{
			button.SetWindowText(_T("RELOAD"));
			button.SetIcon(IDI_BTN_CLEAR);
		}break;
		case eSSOper_DATA_SAVE:
		{
			button.SetWindowText(_T("SAVE"));
			button.SetIcon(IDI_BTN_SAVE);
		}break;
		case eSSOper_MAKE_BASE_POS:
		{
			button.SetWindowText(_T("MAKE BASE POS"));
			button.SetIcon(IDI_BTN_IMPORT);
		}break;
	default:
		break;
	}

	//button.SizeToContent();
	//button.font
	button.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 10, 0, RGB(55, 55, 255));
	button.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CVATMain_Common::_Com_InitHandSelectSheet()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		m_ssSelectSheet[nHand].SetFontName(_T("Arial"));
		m_ssSelectSheet[nHand].SetFontSize(10);
		m_ssSelectSheet[nHand].SetTypeHAlign(2); // Center Align
		m_ssSelectSheet[nHand].SetTypeVAlign(0);
		m_ssSelectSheet[nHand].SetCellType(7);
		m_ssSelectSheet[nHand].SetTypeButtonType(0);
		m_ssSelectSheet[nHand].SetMaxCols(1);
		m_ssSelectSheet[nHand].SetMaxRows(1);
		m_ssSelectSheet[nHand].SetColHeadersShow(FALSE);
		m_ssSelectSheet[nHand].SetRowHeadersShow(FALSE);

		m_ssSelectSheet[nHand].SetColWidth(1, 18);
		m_ssSelectSheet[nHand].SetRowHeight(1, 35);
		m_ssSelectSheet[nHand].SetAutoSize(TRUE);

		/*Hand Name Setting*/
		m_ssSelectSheet[nHand].SetCol(1);
		m_ssSelectSheet[nHand].SetRow(1);

		m_ssSelectSheet[nHand].SetTypeButtonText(_Com_convert_hand_idx_to_name(nHand));
		m_ssSelectSheet[nHand].SetTypeButtonColor(WWHITE);
	}
}

// void CVATMain_Common::_Com_InitHandSelected()
// {
// 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
// 	{
// 		_Com_SetSelectedHandStatus(nHand, VAT_STATUS_MEASURE_READY);
// 	}
// }

//HAND IDX -> NAME
CString CVATMain_Common::_Com_convert_hand_idx_to_name(int nHand)
{
	CString strRet = _T("");

	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:			strRet = _T("TRAY PP_LD"); break;
	case VAT_HAND_UNLOAD_TRAYPP:		strRet = _T("TRAY PP_UD"); break;
	case VAT_HAND_LOAD_TESTPP1:			strRet = _T("TEST PP #1_LD"); break;
	case VAT_HAND_UNLOAD_TESTPP1:		strRet = _T("TEST PP #1_UD"); break;
	case VAT_HAND_LOAD_TESTPP2:			strRet = _T("TEST PP #2_LD");break;
	case VAT_HAND_UNLOAD_TESTPP2:		strRet = _T("TEST PP #2_UD");break;
	default:
		break;
	}

	return strRet;
}

/*===============================================================*/
//COMMENT : Hand별 불필요한 Part 확인.
/*============================================================*/
BOOL CVATMain_Common::_Com_IsNecessaryPart(int nHand, int nPartIdx)
{
	BOOL bRet = TRUE;

	//불필요한 Part 표기
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		if (nPartIdx == VAT_IDX_TRAY_PP_TBL_1 || nPartIdx == VAT_IDX_TRAY_PP_TBL_4 || (VAT_IDX_TRAY_PP_CONV_1 <= nPartIdx && nPartIdx <= VAT_IDX_TRAY_PP_CONV_7)) {
			bRet = FALSE;
		}
	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		if (nPartIdx == VAT_IDX_TRAY_PP_TBL_2 || nPartIdx == VAT_IDX_TRAY_PP_TBL_3 || (VAT_IDX_TRAY_PP_CONV_1 <= nPartIdx && nPartIdx <= VAT_IDX_TRAY_PP_CONV_3) || nPartIdx == VAT_IDX_TRAY_PP_FEEDER) {
			bRet = FALSE;
		}
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_LOAD_TESTPP2:
	{
		//모든 Part 필요함.
	}break;
	case VAT_HAND_UNLOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		if (nPartIdx == VAT_IDX_TEST_PP_TBL_1_LD || nPartIdx == VAT_IDX_TEST_PP_TBL_2_LD || nPartIdx == VAT_IDX_TEST_PP_TBL_4_LD || nPartIdx == VAT_IDX_CLEAN_TBL) {
			bRet = FALSE;
		}
	}break;
	default:
		break;
	}

	return bRet;
}


//PART IDX -> NAME
CString CVATMain_Common::_Com_convert_part_idx_to_name(int nHand, int nPartIdx)
{
	CString strRet = _T("");

	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
	{
		CString strTrayPP[VAT_MAX_TRAY_PP_LOC_COUNT] = {

			"UD Table 1",
			"LD Table 1",
			"LD Table 2",
			"UD Table 2",

			"Empty C-Tray 1",
			"Loading C-Tray 2",
			"Empty C-Tray 3",
			"C-Tray 4",
			"C-Tray 5",
			"C-Tray 6",
			"C-Tray 7",

			"Tray Feeder"
		};

		strRet = strTrayPP[nPartIdx];
	}
	else if (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_UNLOAD_TESTPP1)
	{
		CString strTestPP[VAT_MAX_TEST_PP_LOC_COUNT] = {
			"Station 1",
			"Station 2",
			"Station 3",
			"Station 4",

			"LD Table Pos 1",
			"LD Table Pos 2",
			"LD Table Pos 3",
			"LD Table Pos 4",

			"UD Table Pos 1",
			"UD Table Pos 2",
			"UD Table Pos 3",
			"UD Table Pos 4",

			"Clean Table",
		};

		strRet = strTestPP[nPartIdx];
	}
	else
	{
		CString strTestPP[VAT_MAX_TEST_PP_LOC_COUNT] = {
			"Station 5",
			"Station 6",
			"Station 7",
			"Station 8",

			"LD Table Pos 1",
			"LD Table Pos 2",
			"LD Table Pos 3",
			"LD Table Pos 4",

			"UD Table Pos 1",
			"UD Table Pos 2",
			"UD Table Pos 3",
			"UD Table Pos 4",

			"Clean Table",
		};

		strRet = strTestPP[nPartIdx];
	}


	return strRet;
}

/*===============================================================*/
//COMMENT : Hand Select/Unselect 처리
//			Hand Spread Sheet Color 변경
//				1) Hand Type : 선택된 Hand Color 변경.
//				2) Picker Type : 선택된 Hand Color 변경.
//				3) Part Type : 선택된 Hand만 Color 변경.
//			Hand Position Color 변경
/*============================================================*/
void CVATMain_Common::_Com_ButtonClickedSpreadHand(long Col, long Row, int nHand, int nSSType, CEditTrans editPos[])
{
	if (g_VatTaskAutoSystem.GetCurrentLotState() != eAutoLotState_InitNeed)
		return;

	ASSERT(eVAT_SSTYPE_HAND <= nSSType && nSSType <= eVAT_SSTYPE_PART);

	if (Col <= 0 || Row <= 0)
		return;

	/* 순서 변경 X */
	//선택 된 Hand Color 변경.
	ULONG ulSSBtnColor = 0;
	if (nSSType == eVAT_SSTYPE_HAND || nSSType == eVAT_SSTYPE_PICKER) {
		//Select 처리.
		(m_bSelectedHand[nHand] == TRUE) ? m_bSelectedHand[nHand] = FALSE : m_bSelectedHand[nHand] = TRUE;

		ulSSBtnColor = (m_bSelectedHand[nHand] == TRUE) ? (LLIGHTBLUE) : (WWHITE);

		m_ssSelectSheet[nHand].SetCol(Col);
		m_ssSelectSheet[nHand].SetRow(Row);
		m_ssSelectSheet[nHand].SetTypeButtonColor(ulSSBtnColor);
	}
	else { //nSSType == eVAT_SSTYPE_PART
		//Select Hand 초기화
		for (auto hand_idx : m_vAllHand) {
			m_bSelectedHand[hand_idx] = FALSE;
		}

		//Select 처리.
		m_bSelectedHand[nHand] = TRUE;

		for (auto hand_idx : m_vAllHand) {
			ulSSBtnColor = (m_bSelectedHand[hand_idx] == TRUE) ? (LLIGHTBLUE) : (WWHITE);

			m_ssSelectSheet[hand_idx].SetCol(Col);
			m_ssSelectSheet[hand_idx].SetRow(Row);
			m_ssSelectSheet[hand_idx].SetTypeButtonColor(ulSSBtnColor);
		}
	}


	//선택 된 Position Color 변경.
	if (editPos != NULL)
	{
		if (editPos[nHand].m_hWnd != NULL) {
			ULONG ulPoscolor = (m_bSelectedHand[nHand] == TRUE) ? (LLIGHTBLUE) : (WWHITE);
			editPos[nHand].SetBackColor(ulPoscolor);
		}
	}


	//Hand Status 변경
	//측정 시작 후, 상태 표기로 사용함.
// 	int nStatus = (m_bSelectedHand[nHand] == TRUE) ? (VAT_STATUS_MEASURE_WAIT) : (VAT_STATUS_INIT);
// 	_Com_SetSelectedHandStatus(nHand, nStatus);
}


//Hand 선택 확인.
BOOL CVATMain_Common::_Com_IsHandSelected(int nHand)
{
	return m_bSelectedHand[nHand];
}


BOOL CVATMain_Common::_Com_IsAnyHandSelected()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		if (m_bSelectedHand[nHand] == TRUE) {
			return TRUE;
		}
	}

	return FALSE;
}

//Picker Common
//=======================================================================================================================================
// BOOL CVATMain_Common::_Com_IsPickerSelected(int nHand)
// {
// 
// }


//Part Common
//=======================================================================================================================================
void CVATMain_Common::_Com_InitSSHandPanel_PartType()
{
	m_ssPanel_LD_TrayPP.SetWindowText(_T("LD TRAY PP"));
	m_ssPanel_UD_TrayPP.SetWindowText(_T("UD TRAY PP"));
	m_ssPanel_LD_TestPP1.SetWindowText(_T("LD TEST PP#1"));
	m_ssPanel_UD_TestPP1.SetWindowText(_T("UD TEST PP#1"));
	m_ssPanel_LD_TestPP2.SetWindowText(_T("LD TEST PP#2"));
	m_ssPanel_UD_TestPP2.SetWindowText(_T("UD TEST PP#2"));

	m_ssPanel_LD_TrayPP.SetBackColor(LLIGHTCYAN);
	m_ssPanel_UD_TrayPP.SetBackColor(LLIGHTCYAN);
	m_ssPanel_LD_TestPP1.SetBackColor(LLIGHTCYAN);
	m_ssPanel_UD_TestPP1.SetBackColor(LLIGHTCYAN);
	m_ssPanel_LD_TestPP2.SetBackColor(LLIGHTCYAN);
	m_ssPanel_UD_TestPP2.SetBackColor(LLIGHTCYAN);
}


void CVATMain_Common::_Com_InitPartSelectSheet()
{
	//CSpreadSheet *ssHand[VAT_HAND_MAX] = { &m_ssLoadTrayPP, &m_ssUnloadTrayPP, &m_ssLoadTestPP1, &m_ssUnloadTestPP1, &m_ssLoadTestPP2, &m_ssUnloadTestPP2 };
	int nHandPartMax[VAT_HAND_MAX] = { VAT_MAX_TRAY_PP_LOC_COUNT , VAT_MAX_TRAY_PP_LOC_COUNT };

	//int nHand = 0;
	//for (auto spread_sheet : ssHand)
	for(int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		//Spread Sheet 공통사항 Init
		m_ssSelectSheet[nHand].SetFontName(_T("Times New Roman"));
		m_ssSelectSheet[nHand].SetFontSize(10);
		m_ssSelectSheet[nHand].SetTypeHAlign(2); // Center Align
		m_ssSelectSheet[nHand].SetTypeVAlign(2);
		m_ssSelectSheet[nHand].SetCellType(7);
		m_ssSelectSheet[nHand].SetTypeButtonType(0);
		m_ssSelectSheet[nHand].SetColHeadersShow(FALSE);
		m_ssSelectSheet[nHand].SetRowHeadersShow(FALSE);
		m_ssSelectSheet[nHand].SetAutoSize(TRUE);

		//Spread Sheet 폭 조절.
		m_ssSelectSheet[nHand].SetColWidth(1, 12.8);

		ULONG ulBtnColor = 0;
		int nHAND_PART_MAX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

		//Max Col&Row 설정.
		m_ssSelectSheet[nHand].SetMaxCols(1);
		m_ssSelectSheet[nHand].SetMaxRows(nHAND_PART_MAX);

		for (int nPartIdx = 0; nPartIdx < nHAND_PART_MAX; nPartIdx++)
		{
			ulBtnColor = (m_stPartSelected.getPartSelected(nHand, nPartIdx) == TRUE) ? (LLIGHTGREEN) : (WWHITE);
			m_ssSelectSheet[nHand].SetCol(1);
			m_ssSelectSheet[nHand].SetRow(nPartIdx + 1);

			//Spread Sheet 높이 조절.
			m_ssSelectSheet[nHand].SetRowHeight(nPartIdx + 1, 17.6);

			//Button Color
			m_ssSelectSheet[nHand].SetTypeButtonColor(ulBtnColor);

			//Button String
			CString strPartName = _Com_convert_part_idx_to_name(nHand, nPartIdx);
			m_ssSelectSheet[nHand].SetTypeButtonText(strPartName);

			//불필요 Part Hide
			if (_Com_IsNecessaryPart(nHand, nPartIdx) != TRUE) {
				m_ssSelectSheet[nHand].SetRowHidden(TRUE);
			}
		}
	}
}


void CVATMain_Common::_Com_InitComboPos()
{
	//CComboBox *ComboPos[VAT_HAND_MAX] = { &m_CmbPos_LD_TrayPP, &m_CmbPos_UD_TrayPP, &m_CmbPos_LD_TestPP1, &m_CmbPos_UD_TestPP1, &m_CmbPos_LD_TestPP2, &m_CmbPos_UD_TestPP2 };

	//int nHand = 0;
	//for (auto combo_box : ComboPos)
	for(int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		m_ComboPos[nHand].ResetContent();
		int nHAND_PART_MAX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPartIdx = 0; nPartIdx < nHAND_PART_MAX; nPartIdx++)
		{
			if(_Com_IsNecessaryPart(nHand, nPartIdx) == TRUE)
				m_ComboPos[nHand].AddString(_Com_convert_part_idx_to_name(nHand, nPartIdx));
		}

		m_ComboPos[nHand].SetCurSel(0);
		//nHand++;
	}
}


/*===============================================================*/
//COMMENT : Combo Box 현재 Sel에서 Part Index 추출.
/*============================================================*/
int CVATMain_Common::_Com_GetPartIdxfromComboBox(int nHand)
{
	int nRetPartIdx = -1;

	int nCurComboSel = m_ComboPos[nHand].GetCurSel();
	CString strCurComboPart = _T("");
	m_ComboPos[nHand].GetLBText(nCurComboSel, strCurComboPart);

	int nHAND_PART_MAX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
	for (int nPartIdx = 0; nPartIdx < nHAND_PART_MAX; nPartIdx++)
	{
		CString strPart = _Com_convert_part_idx_to_name(nHand, nPartIdx);
		if (strCurComboPart == strPart) {
			nRetPartIdx = nPartIdx;
			break;
		}
	}

	return nRetPartIdx;
}


BOOL CVATMain_Common::_Com_IsPartHidden(int nHand, int nPart)
{
	ASSERT(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2);
	ASSERT(m_ssSelectSheet[nHand].m_hWnd != NULL);

	int nRow = nPart + 1;
	m_ssSelectSheet[nHand].SetRow(nRow);

	return m_ssSelectSheet[nHand].GetRowHidden();
}


/*=============================================================================*/
//COMMENT : Part Status와 상관 없이 Select/UnSelect로만 Spread Sheet에 Display
//			Part Status는 Select 상태 일 경우에만 측정 상태에 따라 Color 변경.
//RETURN : 
/*=============================================================================*/
void CVATMain_Common::_Com_SSDisplaySelectedPart(int nHand, int nCol, int nRow)
{
	int nPartIdx = DEF_PART_INDEX(nRow);
	BOOL bIsSelected = m_stPartSelected.getPartSelected(nHand, nPartIdx);


	if (m_ssSelectSheet[nHand].m_hWnd == NULL || nPartIdx < 0)
		return;

	ULONG ulPoscolor = (bIsSelected) ? (LLIGHTBLUE) : (WWHITE);

	//[Part Select Spread Sheet]
	m_ssSelectSheet[nHand].SetCol(nCol);
	m_ssSelectSheet[nHand].SetRow(nRow);

	m_ssSelectSheet[nHand].SetTypeButtonColor(ulPoscolor);
// 	if (bIsSelected == TRUE) {
// 		
// 	}
// 	else {
// 		m_ssSelectSheet[nHand].SetTypeButtonColor(WWHITE);
// 	}

 	//[Edit Position]
 	ulPoscolor = (bIsSelected) ? (LLIGHTBLUE) : (WWHITE);
 	if(m_editPositionEachHand[nHand].editPosition[nPartIdx].m_hWnd != NULL)
 		m_editPositionEachHand[nHand].editPosition[nPartIdx].SetBackColor(ulPoscolor);
}


/*=============================================================================*/
//COMMENT : Part Status에 따라 Color변경.
//			DM 저장 된 기준으로 Status 확인.
//RETURN : 
/*=============================================================================*/
void CVATMain_Common::_Com_SSDisplayPartStatusByDM(int nMode, int nHand, int nPart)
{
	int nCol = 1;
	int nRow = nPart + 1;

	m_ssSelectSheet[nHand].SetCol(nCol);
	m_ssSelectSheet[nHand].SetRow(nRow);

	int nDMStartIdx = _Com_GetPartStatusFirstIdx_DM(nMode, nHand);

// 	//Status 색상 Test 용.
// 	if (nHand == VAT_HAND_LOAD_TESTPP1) {
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_STATION_1, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_STATION_2, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_STATION_3, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_STATION_4, VAT_STATUS_MEASURE_UNSELECTED);
// 
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_1_LD, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_2_LD, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_3_LD, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_4_LD, VAT_STATUS_MEASURE_UNSELECTED);
// 
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_1_UD, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_2_UD, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_3_UD, VAT_STATUS_MEASURE_UNSELECTED);
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_TEST_PP_TBL_4_UD, VAT_STATUS_MEASURE_UNSELECTED);
// 
// 		g_DMCont.m_dmVAT.SN(nDMStartIdx + VAT_IDX_CLEAN_TBL, VAT_STATUS_MEASURE_UNSELECTED);
// 	}



	int nStatus = g_DMCont.m_dmVAT.GN(nDMStartIdx + nPart);

	//측정 시, 2가지 색상으로만 표기 => 상세히 나누면 사용자 혼돈 발생.
	switch (nStatus)
	{
	case VAT_STATUS_MEASURE_INIT: 
	{
		//DM Status Base는 상시 0.
		//작업자 Part Selected Click할 경우, 해당 상태로 표기.
 		m_ssSelectSheet[nHand].SetTypeButtonColor(WWHITE);
	}break;
	case VAT_STATUS_MEASURE_READY:		
	case VAT_STATUS_MEASURE_WAIT:
	{
		m_ssSelectSheet[nHand].SetTypeButtonColor(LLIGHTBLUE); 
	}break;
		
	case VAT_STATUS_MEASURE_ONGOING:	
	{
		//깜빡임으로 측정 진행 part 표기.
		unsigned long color = m_ssSelectSheet[nHand].GetTypeButtonColor();
		if (color == LLIGHTBLUE) {
			m_ssSelectSheet[nHand].SetTypeButtonColor(RGB(90, 90, 180));
		}
		else {
			m_ssSelectSheet[nHand].SetTypeButtonColor(LLIGHTBLUE);
		}

		
	}break;
	case VAT_STATUS_MEASURE_COMPLETE:	
	case VAT_STATUS_DATA_SAVE_COMPLETE:
	{
		m_ssSelectSheet[nHand].SetTypeButtonColor(YELLOW);
	}break;
	default:
		m_ssSelectSheet[nHand].SetTypeButtonColor(WWHITE);
		break;
	}

	//코랄 핑크
	//RGB(255, 140, 140)
}


int CVATMain_Common::_Com_GetPartStatusFirstIdx_DM(int nMode, int nHand)
{
	int nDMStartIdx = -1;

	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:		nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_LD_Tray_Pp_Part1   : NDM14_State_ZTeach_LD_Tray_Pp_Part1;		break;
	case VAT_HAND_UNLOAD_TRAYPP:	nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_UD_Tray_Pp_Part1   : NDM14_State_ZTeach_UD_Tray_Pp_Part1;		break;
	case VAT_HAND_LOAD_TESTPP1:		nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_LD_Test_Pp_1_Part1 : NDM14_State_ZTeach_LD_Test_Pp_1_Part1;	break;
	case VAT_HAND_UNLOAD_TESTPP1:	nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_UD_Test_Pp_1_Part1 : NDM14_State_ZTeach_UD_Test_Pp_1_Part1;	break;
	case VAT_HAND_LOAD_TESTPP2:		nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_LD_Test_Pp_2_Part1 : NDM14_State_ZTeach_LD_Test_Pp_2_Part1;	break;
	case VAT_HAND_UNLOAD_TESTPP2:	nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_UD_Test_Pp_2_Part1 : NDM14_State_ZTeach_UD_Test_Pp_2_Part1;	break;
	default:
		break;
	}

	return nDMStartIdx;
}


// int CVATMain_Common::GetSelectedPartStatus(int nHandIdx, int nPart)
// {
// 	int nStatus = ERR_VAT_ERROR;
// 
// 	switch (nHandIdx)
// 	{
// 	case VAT_HAND_LOAD_TRAYPP:			nStatus = g_VatTaskAutoTrayLoadPP.GetPartStatus(nPart);				break;
// 	case VAT_HAND_UNLOAD_TRAYPP:		nStatus = g_VatTaskAutoTrayUnloadPP.GetPartStatus(nPart);			break;
// 	case VAT_HAND_LOAD_TESTPP1:			nStatus = g_VatTaskAutoTestPP[eTestPP_1].GetPartStatus(nPart);		break;
// 	case VAT_HAND_UNLOAD_TESTPP1:		nStatus = g_VatTaskAutoTestPP[eTestPP_2].GetPartStatus(nPart);		break;
// 	case VAT_HAND_LOAD_TESTPP2:			nStatus = g_VatTaskAutoTestPP[eTestPP_1].GetPartStatus(nPart);		break;
// 	case VAT_HAND_UNLOAD_TESTPP2:		nStatus = g_VatTaskAutoTestPP[eTestPP_2].GetPartStatus(nPart);		break;
// 	default:
// 		break;
// 	}
// 
// 	return nStatus;
// }




/*==========================================================================================================*/
//COMMENT : 구조체 내 Hand에 맞는 part SELECT/UNSELECT
//		     1Part에 대해서 Select/Unselect	
//paint     : (bIsPainting == TRUE)(작업자 호출) 작업자 Part Select할 경우, painting진행.
//            (bIsPainting == FALSE)(측정 완료 후, 호출) 결과에 따라 Part 자동 Select/Unselect 진행 될 경우
/*========================================================================================================*/
void CVATMain_Common::_Com_SetSelectedPart(int nHand, int nPart, BOOL bIsPainting/* = TRUE*/)
{
	//Disable Cell Check.
	if (_Com_IsPartDisable(nHand, nPart))
		return;

	//Part 선택
	BOOL TR_FL = (m_stPartSelected.getPartSelected(nHand, nPart) == TRUE) ? FALSE : TRUE;
	m_stPartSelected.setPartSelected(nHand, nPart, TR_FL);

	if (bIsPainting) {
		//색상 변경.
		int nCol = 1;
		int nRow = nPart + 1;
		_Com_SSDisplaySelectedPart(nHand, nCol, nRow);
	}
}


/*===============================================================*/
//COMMENT : 구조체 내 Hand에 맞는 part SELECT/UNSELECT
//		     1Hand에 대한 모든 Part Select/Unselect	
//조건		: Select : 한 Part라도 Unselect이면, Select로 바꿈.
//			  Unselect : 전체 Part Select 상태 이면, Unselect.
/*============================================================*/
void CVATMain_Common::_Com_SetSelectedAllPart(int nHand)
{
	int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

	//Part Select/Unselect 확인.
	BOOL bAllSelect = FALSE;
	for (int nPart = 0; nPart < nMaxPart; nPart++)
	{
		BOOL bIsHiddenRow = _Com_IsPartHidden(nHand, nPart);

		//Hidden Row가 아님, Part선택 되어 있지 않음, PartDisable이 아님 --> All Select로 변경.
		if (bIsHiddenRow != TRUE && m_stPartSelected.getPartSelected(nHand, nPart) != TRUE && _Com_IsPartDisable(nHand, nPart) != TRUE)
		{
			bAllSelect = TRUE;
			break;
		}
	}

	//Set All Select/Unselect && 색상 변경.
	for (int nPart = 0; nPart < nMaxPart; nPart++)
	{
		if (_Com_IsPartHidden(nHand, nPart) != TRUE && _Com_IsPartDisable(nHand, nPart) != TRUE) {
			m_stPartSelected.setPartSelected(nHand, nPart, bAllSelect);

			int nCol = 1;
			int nRow = nPart + 1;
			_Com_SSDisplaySelectedPart(nHand, nCol, nRow);
		}
	}

}


/*===============================================================*/
//COMMENT : 구조체 내 Hand에 맞는 part SELECT/UNSELECT
//		     모든 Hand에 대한 모든 Part Select/Unselect
// 
/*============================================================*/
void CVATMain_Common::_Com_SetSelectedAll()
{

}


/*===============================================================*/
//COMMENT : 구조체 내 Hand에 맞는 part가 선택 되어 있는지 확인.
//			
//RETURN : TRUE or FALSE
/*============================================================*/
BOOL CVATMain_Common::_Com_IsPartSelected(int nHand, int nPart)
{
	return m_stPartSelected.getPartSelected(nHand, nPart);
}


/*=============================================================================*/
//COMMENT : 전체 Hand중, 1개의 Part라도 선택 되어 있는지 확인.
//RETURN : TRUE -> selected / FALSE -> unselected.
/*=============================================================================*/
BOOL CVATMain_Common::_Com_IsAnyPartSelected()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

		for (int nPart = 0; nPart < nMaxPart; nPart++)
		{
			if (m_stPartSelected.getPartSelected(nHand, nPart)) {
				return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL CVATMain_Common::_Com_IsPartDisable(int nHand, int nPart)
{
	int nCol = 1;
	int nRow = nPart + 1;

	m_ssSelectSheet[nHand].SetCol(nCol);
	m_ssSelectSheet[nHand].SetRow(nRow);

	unsigned long color = m_ssSelectSheet[nHand].GetTypeButtonColor();
	if (color == DISABLE_CELL_COLOR) {
		return TRUE;
	}

	return FALSE;
}



int CVATMain_Common::_Com_Oper_MakeBasePos(int nSSDisplayType)
{
	//[Interlock]
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return ERR_VAT_ERROR;

	//"Do you want to create the base position?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_MAKE_BASE_POS, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return ERR_VAT_ERROR;

	//[Interlock]
	bChkInterlock = _Com_SSInterlock_MakeBasePos(nSSDisplayType);
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return ERR_VAT_ERROR;

	return ERR_VAT_NO_ERROR;
}

_eVAT_INTERLOCK CVATMain_Common::_Com_SSInterlock_MakeBasePos(int nSSDisplayType)
{

	//1. TW_LH_VAT Update 확인.
	int nCheckUpdateSql = g_DBVAT.CHECK_VAT_TBL();

	if (nCheckUpdateSql == TRUE)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Please Off Handler OS and Update VAT DB.");
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_MAKE_BASE_POS_NEED_UPDATE_DB, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}

	//2. Data Show Type 확인.
	if (nSSDisplayType != eVAT_SSDISPLAYTYPE_DATA)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_SELECT_RIGHT_SPREAD_SHEET_TYPE, AlarmData, NULL);
		return eVAT_INTERLOCK_FAIL;
	}

	return eVAT_INTERLOCK_PASS;
}