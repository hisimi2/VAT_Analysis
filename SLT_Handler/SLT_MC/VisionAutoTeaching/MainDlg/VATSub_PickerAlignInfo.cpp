// VATSub_PickerAlignInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATSub_PickerAlignInfo.h"
#include "afxdialogex.h"


// CVATSub_PickerAlignInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATSub_PickerAlignInfo, CDialogEx)

CVATSub_PickerAlignInfo::CVATSub_PickerAlignInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_SUB_PICKER_ALIGN_INFO, pParent)
{
	m_nViewType = eDataViewType_Graphic;
	m_bAvgViewUse = FALSE;
}

CVATSub_PickerAlignInfo::~CVATSub_PickerAlignInfo()
{

}

void CVATSub_PickerAlignInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
}


BEGIN_MESSAGE_MAP(CVATSub_PickerAlignInfo, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CVATSub_PickerAlignInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CVATSub_PickerAlignInfo 메시지 처리기입니다.

BOOL CVATSub_PickerAlignInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Offset Rect Init 
	InitRect();

	//Hand Panel 초기화.
	InitHandPanel();

	//버튼 관련 초기화.
	InitSubDlgBtn();

	//SSCMD 변경
	ChangeSSCmd();

	return TRUE;
}


void CVATSub_PickerAlignInfo::OnPaint()
{
 	//Invalidate(TRUE); //BackGround Erase

	CPaintDC dc(this);

	if (m_nViewType == eDataViewType_Graphic)
		DisplayGraphicView(dc);
	else
		DisplayDataView(dc);
}

void CVATSub_PickerAlignInfo::InitHandPanel()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			CDSSPanel *pPanel = (CDSSPanel*)GetDlgItem(m3_nHandPanel_Sub[nHand][nInfoType]);
			pPanel->SetBackColor(LLIGHTCYAN);
		}
	}
}

void CVATSub_PickerAlignInfo::InitSubDlgBtn()
{
// 	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
// 	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
// 	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CVATSub_PickerAlignInfo::InitRect()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			CStatic *staticArea = (CStatic *)GetDlgItem(m3_nRectGraphicView_Sub[nHand][nInfoType]);
			staticArea->GetWindowRect(&m_rect[nHand][nInfoType]);
			ScreenToClient(m_rect[nHand][nInfoType]);
		}
	}
}

/*========================================================================*/
//COMMENT : Picker Align Data Graphic으로 Display
/*========================================================================*/
void CVATSub_PickerAlignInfo::DisplayGraphicView(CPaintDC &dc)
{
	//Base 격자 그리기.
	DrawBaseGrid(dc);

	//Picker Center Hole 그리기.
	DrawPickerCenterHole(dc);

	//Picker Real Hole 그리기.
	DrawPickerRealHole(dc);
}

/*========================================================================*/
//COMMENT : Picker Align Data Simple Data로 Display	
/*========================================================================*/
void CVATSub_PickerAlignInfo::DisplayDataView(CPaintDC &dc)
{
	//Base 격자 그리기.
	DrawSimpleBaseGrid(dc);

	//기준 Picker 그리기
	DrawSimpleStdPkrGrid(dc);


	//Offset Data 표기.
	DrawSimpleOffsetText(dc);
}

void CVATSub_PickerAlignInfo::DrawBaseGrid(CPaintDC &dc)
{
	CPen BLACKPEN(PS_SOLID, 2, BBLACK);
	CPen LIGHTGRAYPEN(PS_SOLID, 1, LLIGHTGRAY);

	CPen BLACKPEN_DASH(PS_SOLID, 1, BBLACK);

	CPen *pOldPen = dc.SelectObject(&BLACKPEN);

	// 	CBrush MainPen( BBLACK);
	// 	CBrush LIGHTGRAYPEN(LLIGHTGRAY);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			int nStartX = m_rect[nHand][nInfoType].left;
			int nStartY = m_rect[nHand][nInfoType].top;
			int nWidth = m_rect[nHand][nInfoType].Width();
			int nHeight = m_rect[nHand][nInfoType].Height();

			//큰 테두리 그림.
			dc.SelectObject(&BLACKPEN);
			dc.Rectangle(m_rect[nHand][nInfoType]);
			
			//연한선 그림.
			dc.SelectObject(&LIGHTGRAYPEN);
			if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
			{
				//(X -> 8등분, Y -> 16등분)
				int nDivX = 16;
				int nDivY = 8;

				for (int x = 1; x < nDivX; x++)
				{
					dc.MoveTo(nStartX + x*nWidth / nDivX, nStartY + 2);
					dc.LineTo(nStartX + x*nWidth / nDivX, nStartY + nHeight - 2);
				}
				
				for (int y = 1; y < nDivY; y++)
				{
					dc.MoveTo(nStartX + 2, nStartY + y*nHeight / nDivY);
					dc.LineTo(nStartX + nWidth - 2, nStartY + y*nHeight / nDivY);
				}

				dc.SelectObject(&BLACKPEN_DASH);
				// 영역을 8등분 하기위한 선 그림. (세로줄)

				// 영역을 8등분 하기위한 선 그림. (가로줄)
				dc.MoveTo(nStartX, nStartY + nHeight / 2);
				dc.LineTo(nStartX + nWidth - 1, nStartY + nHeight / 2);

				// 영역을 8등분 하기위한 선 그림. (세로줄)
				for (int x = 1; x < 4; x++)
				{
					dc.MoveTo(nStartX + x*nWidth / 4, nStartY);
					dc.LineTo(nStartX + x*nWidth / 4, nStartY + nHeight - 1);
				}
			}
			else
			{
				//(X -> 16등분, Y -> 8등분)
				int nDivX = 8;
				int nDivY = 16;

				for (int x = 1; x < nDivX; x++)
				{
					dc.MoveTo(nStartX + x*nWidth / nDivX, nStartY + 2);
					dc.LineTo(nStartX + x*nWidth / nDivX, nStartY + nHeight - 2);
				}

				for (int y = 1; y < nDivY; y++)
				{
					dc.MoveTo(nStartX + 2, nStartY + y*nHeight / nDivY);
					dc.LineTo(nStartX + nWidth - 2, nStartY + y*nHeight / nDivY);
				}
				
				dc.SelectObject(&BLACKPEN_DASH);
				// 영역을 8등분 하기위한 선 그림. (세로줄)

				dc.MoveTo(nStartX + nWidth / 2, nStartY);
				dc.LineTo(nStartX + (nWidth / 2), nStartY + nHeight - 1);

				// 영역을 8등분 하기위한 선 그림. (세로줄)
				for (int y = 1; y < 4; y++)
				{
					dc.MoveTo(nStartX, nStartY + y*nHeight / 4);
					dc.LineTo(nStartX + nWidth - 1, nStartY + y*nHeight / 4);
				}
			}
		}
	}

	dc.SelectObject(pOldPen);
}

void CVATSub_PickerAlignInfo::DrawPickerCenterHole(CPaintDC &dc)
{
	CPen BLACKPEN(PS_SOLID, 2, BBLACK);
	CBrush BBLACKBRUSH(BBLACK);

	CPen *pOldPen = dc.SelectObject(&BLACKPEN);
	CBrush *pOldBrush = dc.SelectObject(&BBLACKBRUSH);

	dc.SelectObject(&BLACKPEN);
	dc.SelectObject(&BBLACKBRUSH);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
		int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			//Picker 8ea Center Hole 그림.
			for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++) {
				for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++) {
					CRect PickerRect = GetPickerRect(nHand, nInfoType, nGuiX, nGuiY);

					int nCX = PickerRect.CenterPoint().x;
					int nCY = PickerRect.CenterPoint().y;

					CRect EllipseRect(nCX - 5, nCY - 5, nCX + 5, nCY + 5);

					dc.Ellipse(EllipseRect);
				}
			}
		}
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

}


void CVATSub_PickerAlignInfo::DrawPickerRealHole(CPaintDC &dc)
{
	CBrush OKBrush(GREEN); // 허용 범위에 들어올 경우
	CBrush NGBrush(RED);   // 허용 범위를 벗어난 경우
	CBrush STDBrush(BLUE); // 기준 피커

	CBrush *pOldBrush = dc.SelectObject(&OKBrush);

	double dOkayRangeX = VAT_PICKER_X_OFFSET_SPEC / 1000.0;
	double dOkayRangeY = VAT_PICKER_Y_OFFSET_SPEC / 1000.0;

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
		int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			//Picker 8ea Real Hole 그림.
			for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++) {
				for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++) {
					//Selected Pkr 확인.
					if (/*g_VATCommonData.m_stVATPickerAlignInfoBuff.nSelectedPicker[nHand][nY][nX] == 1*/1)
					{
						double dOffsetX = 0.0, dOffsetY = 0.0;
						//Average View 만들기.
						GetPickerOffset(nHand, nInfoType, nGuiX, nGuiY, dOffsetX, dOffsetY);

						if (IsGuiStandardPicker(nHand, nGuiX, nGuiY) == TRUE)
						{
							dc.SelectObject(&STDBrush);
						}
						else
						{
							if (fabs(dOffsetX) > dOkayRangeX || fabs(dOffsetY) > dOkayRangeY)
								dc.SelectObject(&NGBrush);
							else
								dc.SelectObject(&OKBrush);
						}
					
						//Offset 그리기.
						CRect PickerRect = GetPickerRect(nHand, nInfoType, nGuiX, nGuiY);
						int nPxOffsetX = (int)(dOffsetX * VAT_OFFSET_TO_PIXEL);
						int nPxOffsetY = (int)(dOffsetY * VAT_OFFSET_TO_PIXEL);

						int nCX = PickerRect.CenterPoint().x + (int)nPxOffsetX;
						int nCY = PickerRect.CenterPoint().y - (int)nPxOffsetY; // Y는 Handler 축과 UI 축 반대.

						CRect EllipseRect(nCX - 6, nCY - 6, nCX + 6, nCY + 6);
						dc.Ellipse(EllipseRect);

						//Offset Text 표기.
						CPoint textPoint(PickerRect.CenterPoint().x - 33, PickerRect.CenterPoint().y + 19);
						DrawOffsetText(dc, textPoint, dOffsetX, dOffsetY);
					}
				}
			}
		}
	}

	dc.SelectObject(pOldBrush);

}

void CVATSub_PickerAlignInfo::DrawOffsetText(CPaintDC &dc, CPoint point, double dOffsetX, double dOffsetY)
{
	CFont Font;
	CFont *pOldFont;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	lf.lfHeight = 18;
	lf.lfWidth = 5;
	lf.lfWeight = FW_BOLD;

	Font.CreateFontIndirect(&lf);
	pOldFont = dc.SelectObject(&Font);

	CString strText(_T(""));
	strText.Format(_T("%+.3f, %+.3f"), dOffsetX, dOffsetY);

	int nOldBkMode = dc.GetBkMode();

	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(point.x, point.y, strText);

	dc.SetBkMode(nOldBkMode);

	dc.SelectObject(pOldFont);

	Font.DeleteObject();
}


void CVATSub_PickerAlignInfo::DrawSimpleBaseGrid(CPaintDC &dc)
{
	CPen BLACKPEN(PS_SOLID, 2, BBLACK);
	CPen LIGHTGRAYPEN(PS_SOLID, 1, LLIGHTGRAY);

	CPen BLACKPEN_DASH(PS_SOLID, 1, BBLACK);

	CPen *pOldPen = dc.SelectObject(&BLACKPEN);

	// 	CBrush MainPen( BBLACK);
	// 	CBrush LIGHTGRAYPEN(LLIGHTGRAY);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			int nStartX = m_rect[nHand][nInfoType].left;
			int nStartY = m_rect[nHand][nInfoType].top;
			int nWidth = m_rect[nHand][nInfoType].Width();
			int nHeight = m_rect[nHand][nInfoType].Height();

			//큰 테두리 그림.
			dc.SelectObject(&BLACKPEN);
			dc.Rectangle(m_rect[nHand][nInfoType]);

			if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
			{
				dc.SelectObject(&BLACKPEN_DASH);
				// 영역을 8등분 하기위한 선 그림. (세로줄)

				// 영역을 8등분 하기위한 선 그림. (가로줄)
				dc.MoveTo(nStartX, nStartY + nHeight / 2);
				dc.LineTo(nStartX + nWidth - 1, nStartY + nHeight / 2);

				// 영역을 8등분 하기위한 선 그림. (세로줄)
				for (int x = 1; x < 4; x++)
				{
					dc.MoveTo(nStartX + x*nWidth / 4, nStartY);
					dc.LineTo(nStartX + x*nWidth / 4, nStartY + nHeight - 1);
				}
			}
			else
			{
				dc.SelectObject(&BLACKPEN_DASH);
				// 영역을 8등분 하기위한 선 그림. (세로줄)

				dc.MoveTo(nStartX + nWidth / 2, nStartY);
				dc.LineTo(nStartX + (nWidth / 2), nStartY + nHeight - 1);

				// 영역을 8등분 하기위한 선 그림. (세로줄)
				for (int y = 1; y < 4; y++)
				{
					dc.MoveTo(nStartX, nStartY + y*nHeight / 4);
					dc.LineTo(nStartX + nWidth - 1, nStartY + y*nHeight / 4);
				}
			}
		}
	}

	dc.SelectObject(pOldPen);
}

void CVATSub_PickerAlignInfo::DrawSimpleStdPkrGrid(CPaintDC &dc)
{
	CPen STDPEN(PS_SOLID, 3, BBLACK);
	CBrush STDPKRBRUSH(ORANGE);

	CPen *pOldPen = dc.SelectObject(&STDPEN);
	CBrush *pOldBrush = dc.SelectObject(&STDPKRBRUSH);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
		int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			//Picker 8ea Real Hole 그림.
			for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++) {
				for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++) {
					if (IsGuiStandardPicker(nHand, nGuiX, nGuiY) == TRUE)
					{
						CRect PickerRect = GetPickerRect(nHand, nInfoType, nGuiX, nGuiY);

						dc.SelectObject(&STDPKRBRUSH);
						dc.Rectangle(PickerRect);

						dc.SelectObject(&STDPEN);
						dc.Rectangle(PickerRect);
					}
				}
			}
		}
	}
	
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);	
}


void CVATSub_PickerAlignInfo::DrawSimpleOffsetText(CPaintDC &dc)
{
	//Offset Font
	CFont Font;
	CFont *pOldFont;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	lf.lfHeight = 25;
	lf.lfWidth = 9;
	lf.lfWeight = FW_BOLD;

	Font.CreateFontIndirect(&lf);
	pOldFont = dc.SelectObject(&Font);
	//////////////////////////////////////////////////////


	//Standard Picker Font
	CFont StdFont;
	CFont *pOldStdFont;

	LOGFONT lfStd;
	memset(&lfStd, 0, sizeof(lfStd));

	wsprintf(lfStd.lfFaceName, TEXT("%s"), TEXT("Arial"));
	lfStd.lfHeight = 20;
	lfStd.lfWidth = 6;
	lfStd.lfWeight = FW_BOLD;

	StdFont.CreateFontIndirect(&lfStd);
	pOldStdFont = dc.SelectObject(&StdFont);
	/////////////////////////////////////////////////////


	int nOldBkMode = dc.GetBkMode();
	dc.SetBkMode(TRANSPARENT);
	
	//Read Offset 
	double dOffsetX = 0.0;
	double dOffsetY = 0.0;

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
		int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

		for (int nInfoType = 0; nInfoType < VAT_DATA_INFO_TYPE_MAX; nInfoType++)
		{
			//Picker 8ea Offset Text 표기.
			for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++)
			{
				for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++)
				{
					GetPickerOffset(nHand, nInfoType, nGuiX, nGuiY, dOffsetX, dOffsetY);

					CRect PickerRect = GetPickerRect(nHand, nInfoType, nGuiX, nGuiY);

					//CString strText(_T(""));
					CString strOffsetX(_T(""));
					CString strOffsetY(_T(""));

					if (IsGuiStandardPicker(nHand, nGuiX, nGuiY) == TRUE)
					{
						dc.SelectObject(&StdFont);

						//Offset X Text Point
						strOffsetX.Format(_T("X Offset"));
						CPoint textPointX(PickerRect.CenterPoint().x - 22, PickerRect.CenterPoint().y - 20);
						dc.TextOut(textPointX.x, textPointX.y, strOffsetX);

						//Offset Y Text Point
						strOffsetY.Format(_T("Y Offset"));
						CPoint textPointY(PickerRect.CenterPoint().x - 22, PickerRect.CenterPoint().y + 8);
						dc.TextOut(textPointY.x, textPointY.y, strOffsetY);
					}
					else
					{
						dc.SelectObject(&Font);

						//Offset X Text Point
						strOffsetX.Format(_T("%+.3f"), dOffsetX);
						CPoint textPointX(PickerRect.CenterPoint().x - 30, PickerRect.CenterPoint().y - 20);
						dc.TextOut(textPointX.x, textPointX.y, strOffsetX);
					

						//Offset Y Text Point
						strOffsetY.Format(_T("%+.3f"), dOffsetY);
						CPoint textPointY(PickerRect.CenterPoint().x - 30, PickerRect.CenterPoint().y + 8);
						dc.TextOut(textPointY.x, textPointY.y, strOffsetY);				
					}
				}
			}
		}
	}


	dc.SetBkMode(nOldBkMode);

	dc.SelectObject(pOldFont);
	Font.DeleteObject();

	dc.SelectObject(pOldStdFont);
	StdFont.DeleteObject();
}


CRect CVATSub_PickerAlignInfo::GetPickerRect(int nHand, int nInfoType, int nHandX, int nHandY)
{
	int nHandType = GetHandType(nHand);

	//사각형 탐색
	int nStartX = 0;
	int nStartY = 0;
	int nEndX = 0;
	int nEndY = 0;

	if (nHandType == ePkrAlignHandType_TrayPP)
	{
		nStartX = m_rect[nHand][nInfoType].left + (nHandX * m_rect[nHand][nInfoType].Width() / 4);
		nStartY = m_rect[nHand][nInfoType].top + (nHandY * m_rect[nHand][nInfoType].Height() / 2);

		nEndX = nStartX + (m_rect[nHand][nInfoType].Width() / 4);
		nEndY = nStartY + (m_rect[nHand][nInfoType].Height() / 2);
	}
	else //nHandType == ePkrAlignHandType_TestPP
	{
		nStartX = m_rect[nHand][nInfoType].left + (nHandX * m_rect[nHand][nInfoType].Width() / 2);
		nStartY = m_rect[nHand][nInfoType].top + (nHandY * m_rect[nHand][nInfoType].Height() / 4);

		nEndX = nStartX + (m_rect[nHand][nInfoType].Width() / 2);
		nEndY = nStartY + (m_rect[nHand][nInfoType].Height() / 4) + 1;
	}

	return CRect(nStartX, nStartY, nEndX, nEndY);
}


int CVATSub_PickerAlignInfo::GetHandType(int nHand)
{
	return (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? ePkrAlignHandType_TrayPP : ePkrAlignHandType_TestPP;
}


void CVATSub_PickerAlignInfo::GetPickerOffset(int nHand, int nInfoType, int nGuiX, int nGuiY, double &dOffsetX, double &dOffsetY)
{
	int nPkrX = nGuiX;
	int nPkrY = nGuiY;

	ConvPkrIdx_Gui_To_Real(nHand, nGuiX, nGuiY, nPkrX, nPkrY);
	int nPickerIdx = nPkrX + (nPkrY * 4);

	switch (nInfoType)
	{
	case VAT_DATA_INFO_TYPE_WIDE:
	case VAT_DATA_INFO_TYPE_NARROW:
	{
		dOffsetX = g_VATCommonData.m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nInfoType][nPickerIdx].first / 1000.0;
		dOffsetY = g_VATCommonData.m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nInfoType][nPickerIdx].second / 1000.0;

		//Average View일 경우, 평균값 적용.
		if (m_bAvgViewUse)
		{
			double dAverageOffsetX = g_VATCommonData.m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nInfoType].first / 1000.0;
			double dAverageOffsetY = g_VATCommonData.m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nInfoType].second / 1000.0;

			dOffsetX -= dAverageOffsetX;
			dOffsetY -= dAverageOffsetY;
		}
	}break;
	case VAT_DATA_INFO_TYPE_WIDE_TO_NARROW:
	{
		const std::pair<double, double> &pWideOffset = g_VATCommonData.m_stVATPickerAlignInfo.pEachPickerOffset[nHand][VAT_DATA_INFO_TYPE_WIDE][nPickerIdx];
		const std::pair<double, double> &pNarrowOffset = g_VATCommonData.m_stVATPickerAlignInfo.pEachPickerOffset[nHand][VAT_DATA_INFO_TYPE_NARROW][nPickerIdx];
		dOffsetX = (pWideOffset.first - pNarrowOffset.first) / 1000.0;
		dOffsetY = (pWideOffset.second - pNarrowOffset.second) / 1000.0;

		if (m_bAvgViewUse)
		{
			//X 평균
			double dAvgWideX = g_VATCommonData.m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][VAT_DATA_INFO_TYPE_WIDE].first;
			double dAvgNarrowX = g_VATCommonData.m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][VAT_DATA_INFO_TYPE_NARROW].first;

			//y 평균
			double dAvgWideY = g_VATCommonData.m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][VAT_DATA_INFO_TYPE_WIDE].second;
			double dAvgNarrowY = g_VATCommonData.m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][VAT_DATA_INFO_TYPE_NARROW].second;

			// wide-narrow 평균 차이
			double dAvgDiffX = (dAvgWideX - dAvgNarrowX) / 1000.0;
			double dAvgDiffY = (dAvgWideY - dAvgNarrowY) / 1000.0;

			dOffsetX -= dAvgDiffX;
			dOffsetY -= dAvgDiffY;
		}
	}break;
	default:
		break;
	}
 }


void CVATSub_PickerAlignInfo::GetAvgPickerOffset(int nHand, int nInfoType, double &dOffsetX, double &dOffsetY)
{
	double dTempX = 0.0;
	double dTempY = 0.0;

	std::vector<double> vAllPkrOffsetX; vAllPkrOffsetX.clear();
	std::vector<double> vAllPkrOffsetY; vAllPkrOffsetY.clear();

	for (int nGuiY = 0; nGuiY < VAT_MAX_HAND_CNT_Y; nGuiY++) {
		for (int nGuiX = 0; nGuiX < VAT_MAX_HAND_CNT_X; nGuiX++) {
			//GetPickerOffset(nHand, nInfoType, nGuiX, nGuiY, dTempX, dTempY);

			vAllPkrOffsetX.push_back(dTempX);
			vAllPkrOffsetY.push_back(dTempY);
		}
	}

	//평균값 확인.
	double dMaxOffsetX = *max_element(vAllPkrOffsetX.begin(), vAllPkrOffsetX.end());
	double dMinOffsetX = *min_element(vAllPkrOffsetX.begin(), vAllPkrOffsetX.end());
	double dMaxOffsetY = *max_element(vAllPkrOffsetY.begin(), vAllPkrOffsetY.end());
	double dMinOffsetY = *min_element(vAllPkrOffsetY.begin(), vAllPkrOffsetY.end());

	dOffsetX = (dMaxOffsetX + dMinOffsetX) / 2.0;
	dOffsetY = (dMaxOffsetY + dMinOffsetY) / 2.0;
}


BEGIN_EVENTSINK_MAP(CVATSub_PickerAlignInfo, CDialogEx)
ON_EVENT(CVATSub_PickerAlignInfo, IDC_SSCMD_GRAPHIC_VIEW, 22, CVATSub_PickerAlignInfo::ClickSscmdGraphicView, VTS_NONE)
ON_EVENT(CVATSub_PickerAlignInfo, IDC_SSCMD_SIMPLE_VIEW, 22, CVATSub_PickerAlignInfo::ClickSscmdSimpleView, VTS_NONE)
ON_EVENT(CVATSub_PickerAlignInfo, IDC_SSCMD_AVERAGE, 22, CVATSub_PickerAlignInfo::ClickSscmdAverage, VTS_NONE)
END_EVENTSINK_MAP()


void CVATSub_PickerAlignInfo::ChangeSSCmd()
{
	//Graphic & Simple View
	((CSSCommand *)GetDlgItem(IDC_SSCMD_GRAPHIC_VIEW))->SetBackColor(WWHITE);
	((CSSCommand *)GetDlgItem(IDC_SSCMD_SIMPLE_VIEW))->SetBackColor(WWHITE);

	if (m_nViewType == eDataViewType_Graphic)
	{
		((CSSCommand *)GetDlgItem(IDC_SSCMD_GRAPHIC_VIEW))->SetBackColor(LLIGHTGREEN);
	}
	else //m_nViewType == eDataViewType_Simple
	{
		((CSSCommand *)GetDlgItem(IDC_SSCMD_SIMPLE_VIEW))->SetBackColor(LLIGHTGREEN);
	}

	//Option 
	if (m_nViewType == eDataViewType_Graphic)
	{
		((CSSCommand *)GetDlgItem(IDC_SSCMD_AVERAGE))->ShowWindow(SW_SHOW);

		if (m_bAvgViewUse == TRUE) {
			((CSSCommand *)GetDlgItem(IDC_SSCMD_AVERAGE))->SetBackColor(ORANGE);
		}
		else {
			((CSSCommand *)GetDlgItem(IDC_SSCMD_AVERAGE))->SetBackColor(WWHITE);
		}	
	}
	else //m_nViewType == eDataViewType_Simple
	{
		((CSSCommand *)GetDlgItem(IDC_SSCMD_AVERAGE))->ShowWindow(SW_HIDE);
	}

}


void CVATSub_PickerAlignInfo::RefreshDisplay()
{
	//SSCMD 변경
	ChangeSSCmd();

	Invalidate();
}


void CVATSub_PickerAlignInfo::ClickSscmdGraphicView()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nViewType = eDataViewType_Graphic;

	RefreshDisplay();
}


void CVATSub_PickerAlignInfo::ClickSscmdSimpleView()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nViewType = eDataViewType_Simple;

	RefreshDisplay();
}

void CVATSub_PickerAlignInfo::ClickSscmdAverage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	(m_bAvgViewUse == TRUE) ? m_bAvgViewUse = FALSE : m_bAvgViewUse = TRUE;

	RefreshDisplay();
}

void CVATSub_PickerAlignInfo::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nViewType = eDataViewType_Graphic;
	m_bAvgViewUse = FALSE;
	CDialogEx::OnCancel();
}



