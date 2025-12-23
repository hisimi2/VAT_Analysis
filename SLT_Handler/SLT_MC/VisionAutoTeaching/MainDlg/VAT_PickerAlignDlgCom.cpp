#include "stdafx.h"
#include "VAT_PickerAlignDlgCom.h"


CVAT_PickerAlignDlgCom::CVAT_PickerAlignDlgCom()
{
	//GUI 상태 표기 변수
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_pOffset[nHand] = std::make_pair(0.0, 0.0);
// 		m_dOffsetX[nHand] = 0.0;
// 		m_dOffsetY[nHand] = 0.0;
	}


	memset(m_nGuiPickerStatus, VAT_STATUS_MEASURE_INIT, sizeof(m_nGuiPickerStatus));
}


CVAT_PickerAlignDlgCom::~CVAT_PickerAlignDlgCom()
{
}

void CVAT_PickerAlignDlgCom::_PkrAlignCom_InitSSPickerStatus()
{
	CSpreadSheet *ssPickerStatus[VAT_HAND_MAX] = { &m_ssPkr_LoadTrayPP, &m_ssPkr_UnloadTrayPP, &m_ssPkr_LoadTestPP1, &m_ssPkr_UnloadTestPP1, &m_ssPkr_LoadTestPP2, &m_ssPkr_UnloadTestPP2 };
	int nHandIdx = 0;

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		ssPickerStatus[nHand]->SetFontName(_T("Arial"));
		ssPickerStatus[nHand]->SetFontSize(11);
		ssPickerStatus[nHand]->SetTypeHAlign(2); // Center Align
		ssPickerStatus[nHand]->SetTypeVAlign(2);
		ssPickerStatus[nHand]->SetColHeadersShow(FALSE);
		ssPickerStatus[nHand]->SetRowHeadersShow(FALSE);
		ssPickerStatus[nHand]->SetCellType(7); // Button Type
		ssPickerStatus[nHand]->SetFontBold(TRUE);

		CString strPicker[VAT_MAX_HAND_CNT_Y][VAT_MAX_HAND_CNT_X] = {
			{ _T("1"), _T("2"), _T("3"), _T("4") },
			{ _T("5"), _T("6"), _T("7"), _T("8") }
		};

		int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
		int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

		for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++)
		{
			for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++)
			{
				int nCol = nGuiX + 1;
				int nRow = nGuiY + 1;

				ssPickerStatus[nHand]->SetCol(nCol);
				ssPickerStatus[nHand]->SetRow(nRow);


				ssPickerStatus[nHand]->SetTypeButtonColor(WWHITE);

				int nPkrX = nGuiX;
				int nPkrY = nGuiY;
				ConvPkrIdx_Gui_To_Real(nHand, nGuiX, nGuiY, nPkrX, nPkrY);
				ssPickerStatus[nHand]->SetTypeButtonText(strPicker[nPkrY][nPkrX]);
			}
		}

		//Col width 설정.
		for (int i = 1; i <= nMaxPkrX; i++) {
			ssPickerStatus[nHand]->SetColWidth(i, 4);
		}

		//Row Height 설정.
		for (int i = 1; i <= nMaxPkrY; i++) {
			ssPickerStatus[nHand]->SetRowHeight(i, 25);
		}

		ssPickerStatus[nHand]->SetAutoSize(TRUE);
	}
}


void CVAT_PickerAlignDlgCom::_PkrAlignCom_DrawBaseGrid(int nHand, CPaintDC &dc)
{
	CPen BLACKPEN(PS_SOLID, 2, BBLACK);
	CPen LIGHTGRAYPEN(PS_SOLID, 1, LLIGHTGRAY);

	CPen *pOldPen = dc.SelectObject(&BLACKPEN);

// 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
// 	{
		int nStartX = m_rect[nHand].left;
		int nStartY = m_rect[nHand].top;
		int nWidth = m_rect[nHand].Width();
		int nHeight = m_rect[nHand].Height();

		//큰 테두리 그림.
		dc.SelectObject(&BLACKPEN);
		dc.Rectangle(m_rect[nHand]);

		//연한선 그림.
		int nDivX = 4;
		int nDivY = 4;

		dc.SelectObject(&LIGHTGRAYPEN);
		for (int x = 1; x < nDivX; x++)
		{
			dc.MoveTo(nStartX + x*nWidth / nDivX, nStartY + 1);
			dc.LineTo(nStartX + x*nWidth / nDivX, nStartY + nHeight - 2);
		}

		for (int y = 1; y < nDivY; y++)
		{
			dc.MoveTo(nStartX + 1, nStartY + y*nHeight / nDivY);
			dc.LineTo(nStartX + nWidth - 2, nStartY + y*nHeight / nDivY);
		}

//	}

	dc.SelectObject(pOldPen);
}

void CVAT_PickerAlignDlgCom::_PkrAlignCom_DrawPkrCenterHole(int nHand, CPaintDC &dc)
{
	CPen BLACKPEN(PS_SOLID, 2, BBLACK);
	CBrush BBLACKBRUSH(BBLACK);

	CPen *pOldPen = dc.SelectObject(&BLACKPEN);
	CBrush *pOldBrush = dc.SelectObject(&BBLACKBRUSH);

	dc.SelectObject(&BLACKPEN);
	dc.SelectObject(&BBLACKBRUSH);

// 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
// 	{
		int nCX = m_rect[nHand].CenterPoint().x;
		int nCY = m_rect[nHand].CenterPoint().y;

		CRect EllipseRect(nCX - 5, nCY - 5, nCX + 5, nCY + 5);

		dc.Ellipse(EllipseRect);
//	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

}


void CVAT_PickerAlignDlgCom::_PkrAlignCom_DrawHeadText(int nHand, CPaintDC &dc)
{
	CFont Font;
	CFont *pOldFont;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	lf.lfHeight = 16;
	lf.lfWidth = 4;
// 	if (nType == PICKER_ALIGN_TYPE_CAM_CENTER_CAL) {
// 		lf.lfHeight = 16;
// 		lf.lfWidth = 4;
// 	}
// 	else {
// 		lf.lfHeight = 16; //16
// 		lf.lfWidth = 4; //5
// 	}

	lf.lfWeight = FW_BOLD;
	lf.lfItalic = TRUE;

	Font.CreateFontIndirect(&lf);
	pOldFont = dc.SelectObject(&Font);

	CString strHeadText = _T("Real Time Offset");
	int nOldBkMode = dc.GetBkMode();
	dc.SetBkMode(TRANSPARENT);

	dc.DrawText(strHeadText, &m_rect[nHand], DT_TOP | DT_CENTER | DT_SINGLELINE);

// 	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
// 	{
//		dc.DrawText(strHeadText, &m_rect[nHand], DT_TOP | DT_CENTER | DT_SINGLELINE);
		//dc.TextOut(m_rect[nHand].left + 3, m_rect[nHand].top + 1, strHeadText);
//	}

	dc.SetBkMode(nOldBkMode);
	dc.SelectObject(pOldFont);
	Font.DeleteObject();
}

//================================================================================
//COMMENT : 1) PICKER_ALIGN_TAB_TYPE_MEASURE
//				-. Mode 측정 중, Picker 실시간 Offset Data 표기.
//			2) PICKER_ALIGN_TAB_TYPE_SETTING
//				-. Picker Setting중, 실시간 Offset Data 표기.
//			3) PICKER_ALIGN_TYPE_CAM_CENTER_CAL
//				-. Cam Center Calibration 측정 중, 실시간 Offset Data 표기.
//================================================================================
void CVAT_PickerAlignDlgCom::_PkrAlignCom_DrawOffsetData(int nHand, CPaintDC &dc)
{
	CFont Font;
	CFont *pOldFont;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	lf.lfHeight = 17; //17
	lf.lfWidth = 9;  //9
	lf.lfWeight = FW_EXTRABOLD;
	lf.lfItalic = TRUE;
	//lf.lfUnderline = TRUE;

	Font.CreateFontIndirect(&lf);
	pOldFont = dc.SelectObject(&Font);

	int nOldBkMode = dc.GetBkMode();
	dc.SetBkMode(TRANSPARENT);

	CString strHeadText = _T("");

	/*strHeadText.Format(_T("%+.3f, %+.3f"), dOffsetX, dOffsetY);*/

	//임시
// 	m_dOffsetX = 9999.0;
// 	m_dOffsetY = 9999.0;

	strHeadText.Format(_T("X : %+.3f"), double(m_pOffset[nHand].first / 1000.0));
	dc.TextOut(m_rectData[nHand][VAT_X].left, m_rectData[nHand][VAT_X].top, strHeadText);
	
	strHeadText.Format(_T("Y : %+.3f"), m_pOffset[nHand].second / 1000.0);
	dc.TextOut(m_rectData[nHand][VAT_Y].left, m_rectData[nHand][VAT_Y].top, strHeadText);


	dc.SetBkMode(nOldBkMode);
	dc.SelectObject(pOldFont);
	Font.DeleteObject();
}


void CVAT_PickerAlignDlgCom::_PkrAlignCom_DrawRealHole(int nHand, CPaintDC &dc)
{
	CBrush OKBrush(GREEN); // 허용 범위에 들어올 경우
	CBrush NGBrush(RED);   // 허용 범위를 벗어난 경우

	CBrush *pOldBrush = dc.SelectObject(&OKBrush);

	double dOkayRangeX = VAT_PICKER_X_OFFSET_SPEC / 1000.0;
	double dOkayRangeY = VAT_PICKER_Y_OFFSET_SPEC / 1000.0;

	double dOffsetX = m_pOffset[nHand].first / 1000.0;
	double dOffsetY = m_pOffset[nHand].second / 1000.0;

	if (fabs(dOffsetX) > dOkayRangeX || fabs(dOffsetY) > dOkayRangeY)
		dc.SelectObject(&NGBrush);
	else
		dc.SelectObject(&OKBrush);

	//Offset 그리기.
 	int nPxOffsetX = (int)((dOffsetX /1000) * 50);//VAT_OFFSET_TO_PIXEL
 	int nPxOffsetY = (int)((dOffsetY /1000) * 50);

// 	int nPxOffsetX = (int)((m_dOffsetX / 1000) * 20 );//VAT_OFFSET_TO_PIXEL
// 	int nPxOffsetY = (int)((m_dOffsetY / 1000) * 20);

	int nCX = m_rect[nHand].CenterPoint().x + (int)nPxOffsetX;
	int nCY = m_rect[nHand].CenterPoint().y - (int)nPxOffsetY; // Y는 Handler 축과 UI 축 반대.(-)

	CRect EllipseRect(nCX - 6, nCY - 6, nCX + 6, nCY + 6);
	dc.Ellipse(EllipseRect);

	dc.SelectObject(pOldBrush);
}


//아래 두개 함수는 Z Auto Teaching, Vision Auto Teaching 공통 함수 일 경우 재작업 필요.

BOOL CVAT_PickerAlignDlgCom::IsGuiStandardPicker(int nHand, int nGuiX, int nGuiY)
{
	int nPkrX = nGuiX;
	int nPkrY = nGuiY;

	ConvPkrIdx_Gui_To_Real(nHand, nGuiX, nGuiY, nPkrX, nPkrY);

	if (nPkrX == g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].first &&
		nPkrY == g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[nHand].second) {
		return TRUE;
	}

	return FALSE;
}


/*========================================================================*/
//COMMENT : GUI X, Y 배열을 Hand Picker 배열로 변환
//			Tray PP는 동일, Test PP는 변경 필요		
/*========================================================================*/
void CVAT_PickerAlignDlgCom::ConvPkrIdx_Gui_To_Real(int nHand, int nGuiX, int nGuiY, int &nPkrX, int &nPkrY)
{
	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
		return;

	switch (nHand)
	{
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		if (nGuiX == 0)
		{
			nPkrX = nGuiY;
			nPkrY = 1;
		}
		else
		{
			nPkrX = nGuiY;
			nPkrY = 0;
		}
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		if (nGuiX == 0)
		{
			nPkrX = abs(nGuiY - 3);
			nPkrY = 0;
		}
		else
		{
			nPkrX = abs(nGuiY - 3);
			nPkrY = 1;
		}
	}break;
	default:
		break;
	}
}


/*========================================================================*/
//COMMENT : Hand Picker 배열을 GUI X, Y 배열로 변환
//			Tray PP는 동일, Test PP는 변경 필요		
/*========================================================================*/
void CVAT_PickerAlignDlgCom::ConvPkrIdx_Real_To_Gui(int nHand, int nPkrX, int nPkrY, int &nGuiX, int &nGuiY)
{
	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
		return;

	switch (nHand)
	{
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		if (nPkrY == 0)
		{
			nGuiY = nPkrX;
			nGuiX = 1;
		}
		else
		{
			nGuiY = nPkrX;
			nGuiX = 0;
		}
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		if (nPkrY == 0)
		{
			nGuiY = abs(nPkrX - 3);
			nGuiX = 0;
		}
		else
		{
			nGuiY = abs(nPkrX - 3);
			nGuiX = 1;
		}
	}break;
	default:
		break;
	}
}


void CVAT_PickerAlignDlgCom::_PkrAlignCom_tDisplayPickerStatus()
{
	//시퀀스 측정일 경우에만 상태 표기 실시간 진행.
	int nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();
	if (nCurLotState == eAutoLotState_InitNeed) {
		return;
	}

	CSpreadSheet *ssPickerStatus[VAT_HAND_MAX] = { &m_ssPkr_LoadTrayPP, &m_ssPkr_UnloadTrayPP, &m_ssPkr_LoadTestPP1, &m_ssPkr_UnloadTestPP1, &m_ssPkr_LoadTestPP2, &m_ssPkr_UnloadTestPP2 };

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{	
		int nPitchType = VAT_PITCH_TYPE_WIDE;
		BOOL bIsWidePitchMeasureDone = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_WIDE, VAT_STATUS_MEASURE_COMPLETE);
		if (bIsWidePitchMeasureDone) {
			nPitchType = VAT_PITCH_TYPE_NARROW;
		}
		for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++)
		{
			int nGuiPickerStatus = m_nGuiPickerStatus[nHand][nPitchType][nPickerIdx];

			if (!(VAT_STATUS_MEASURE_INIT <= nGuiPickerStatus && nGuiPickerStatus <= VAT_STATUS_DATA_SAVE_COMPLETE)) {
				return;
			}

			//Picker X,Y
			int nGuiX = VAT_PICKER_X(nPickerIdx);
			int nGuiY = VAT_PICKER_Y(nPickerIdx);

			//Tray PP는 실제 Pkr Index와 Gui Index가 동일.
			ConvPkrIdx_Real_To_Gui(nHand, VAT_PICKER_X(nPickerIdx), VAT_PICKER_Y(nPickerIdx), nGuiX, nGuiY);

			//Col, Row 변환
			int nCol = nGuiX + 1;
			int nRow = nGuiY + 1;

			ssPickerStatus[nHand]->SetCol(nCol);
			ssPickerStatus[nHand]->SetRow(nRow);

			ULONG ulPoscolor;
			switch (nGuiPickerStatus)
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

					//깜빡임으로 측정 진행 Picker 표기.
					if (m_bBlinkToggle[nHand]) {
						ulPoscolor = STATUS_BLINK_COLOR;
					}
					else {
						ulPoscolor = LLIGHTBLUE;
					}
				}break;
				case VAT_STATUS_MEASURE_COMPLETE:
				case VAT_STATUS_DATA_SAVE_COMPLETE:
				{
					ulPoscolor = LLIGHTYELLOW; //YELLOW_DUCK;
				}break;
				default:
					ulPoscolor = WWHITE;
					break;
			}

			if (ssPickerStatus[nHand]->GetTypeButtonColor() != ulPoscolor) {
				ssPickerStatus[nHand]->SetTypeButtonColor(ulPoscolor);
				ssPickerStatus[nHand]->Invalidate(FALSE); //배경만 repaint
			}
		}
	}
}