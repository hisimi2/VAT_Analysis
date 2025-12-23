#pragma once

#define PICKER_ALIGN_TYPE_MEASURE			(0)
#define PICKER_ALIGN_TYPE_SETTING			(1)
#define PICKER_ALIGN_TYPE_CAM_CENTER_CAL	(2)

#define GUI_MAX_TRAY_PP_PICKER_CNT_X  (4)
#define GUI_MAX_TRAY_PP_PICKER_CNT_Y  (2)

#define GUI_MAX_TEST_PP_PICKER_CNT_X  (2)
#define GUI_MAX_TEST_PP_PICKER_CNT_Y  (4)

//Offset
#define VAT_OFFSET_TO_PIXEL			(30)
// #define PICKER_ALIGN_OK_RANGE		(250) //um
// #define PICKER_ALIGN_X_OK_RANGE		(250) //um
// #define PICKER_ALIGN_Y_OK_RANGE		(250) //um


//Main/Sub Picker Align Dlg에서 공통으로 사용.

class CVAT_PickerAlignDlgCom
{
public:
	CVAT_PickerAlignDlgCom();
	~CVAT_PickerAlignDlgCom();

//=========================================================================================================
//COMMENT : 측정 진행 간, GUI 실시간 상태 표기 관련 변수 및 함수.
// GUI 상태 변수 초기화는 선언 후, 바로 초기화 진행
//=========================================================================================================
public:
	//Main Tab에서 사용.(Measure Tab/Setting Tab)
	CSpreadSheet m_ssPkr_LoadTrayPP;
	CSpreadSheet m_ssPkr_UnloadTrayPP;
	CSpreadSheet m_ssPkr_LoadTestPP1;
	CSpreadSheet m_ssPkr_UnloadTestPP1;
	CSpreadSheet m_ssPkr_LoadTestPP2;
	CSpreadSheet m_ssPkr_UnloadTestPP2;

	void _PkrAlignCom_InitSSPickerStatus();


	CRect m_rect[VAT_HAND_MAX];
	void _PkrAlignCom_DrawBaseGrid(int nHand, CPaintDC &dc);
	void _PkrAlignCom_DrawPkrCenterHole(int nHand, CPaintDC &dc);
	void _PkrAlignCom_DrawHeadText(int nHand, CPaintDC &dc);

	//Offset 표기
	CRect m_rectData[VAT_HAND_MAX][VAT_MAX_AXIS_XY];
	std::pair<double, double> m_pOffset[VAT_HAND_MAX];
// 	double m_dOffsetX;
// 	double m_dOffsetY;
	void _PkrAlignCom_DrawOffsetData(int nHand, CPaintDC &dc);
	void _PkrAlignCom_DrawRealHole(int nHand, CPaintDC &dc);

	//Picker Status
	int m_nGuiPickerStatus[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX][VAT_PICKER_MAX];

	//Blink Toggle
	BOOL m_bBlinkToggle[VAT_HAND_MAX];

	//[Timer]
	void _PkrAlignCom_tDisplayPickerStatus();
public:
	//Main/Sub에서 사용.

	BOOL IsGuiStandardPicker(int nHand, int nGuiX, int nGuiY);
	//void ConvHandPkrGuiIdxToRealIdx(int nHand, int nGuiX, int nGuiY, int &nPkrX, int &nPkrY);
	void ConvPkrIdx_Gui_To_Real(int nHand, int nGuiX, int nGuiY, int &nPkrX, int &nPkrY);
	void ConvPkrIdx_Real_To_Gui(int nHand, int nPkrX, int nPkrY, int &nGuiX, int &nGuiY);


//=========================================================================================================
//COMMENT : 측정 진행 간, GUI 실시간 상태 표기 관련 변수 및 함수.
// GUI 상태 변수 초기화는 선언 후, 바로 초기화 진행
//=========================================================================================================
public:

};

