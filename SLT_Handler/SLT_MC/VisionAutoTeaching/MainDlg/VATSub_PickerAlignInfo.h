#pragma once

#include "VAT_PickerAlignDlgCom.h"

// CVATSub_PickerAlignInfo 대화 상자입니다.

class CVATSub_PickerAlignInfo : public CDialogEx, public CVAT_PickerAlignDlgCom
{
	DECLARE_DYNAMIC(CVATSub_PickerAlignInfo)

public:
	CVATSub_PickerAlignInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATSub_PickerAlignInfo();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VAT_SUB_PICKER_ALIGN_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();


public:
	afx_msg void OnPaint();


public:
// 	enum _eVAT_PICKER_ALIGN_INFO_TYPE
// 	{
// 		ePkrAlignInfoType_Wide = 0,
// 		ePkrAlignInfoType_Narrow,
// 		ePkrAlignInfoType_Wide_To_Narrow,
// 		ePkrAlignInfoType_Max,
// 	};

	enum _eVAT_PICKER_ALIGN_HAND_TYPE
	{
		ePkrAlignHandType_TrayPP =0,
		ePkrAlignHandType_TestPP,
	};

	enum _eVAT_DATA_VIEW_TYPE
	{
		eDataViewType_Graphic = 0,
		eDataViewType_Simple,
 	};

public:
//	CShadeButtonST m_BtnCancel;

	CRect m_rect[VAT_HAND_MAX][VAT_DATA_INFO_TYPE_MAX];
	int m_nViewType;
	BOOL m_bAvgViewUse;
public:
	void InitHandPanel();

	void InitSubDlgBtn();
	
	void InitRect();

	void DisplayGraphicView(CPaintDC &dc);
	void DisplayDataView(CPaintDC &dc);

	//Graphic View
	void DrawBaseGrid(CPaintDC &dc);
	void DrawPickerCenterHole(CPaintDC &dc);
	void DrawPickerRealHole(CPaintDC &dc);
	void DrawOffsetText(CPaintDC &dc, CPoint point, double dOffsetX, double dOffsetY);


	//Simple View
	void DrawSimpleBaseGrid(CPaintDC &dc);
	void DrawSimpleStdPkrGrid(CPaintDC &dc);
	void DrawSimpleOffsetText(CPaintDC &dc);


// 	BOOL IsGuiStandardPicker(int nHand, int nGuiX, int nGuiY);
// 	void ConvTestPpPkrGuiIdxToRealIdx(int nHand, int nGuiX, int nGuiY, int &nPkrX, int &nPkrY);

	CRect GetPickerRect(int nHand, int nInfoType, int nHandX, int nHandY);
	
	int GetHandType(int nHand);

	void GetPickerOffset(int nHand, int nInfoType, int nGuiX, int nGuiY, double &dOffsetX, double &dOffsetY);
	void GetAvgPickerOffset(int nHand, int nInfoType, double &dOffsetX, double &dOffsetY);

	void ChangeSSCmd();

	void RefreshDisplay();

	DECLARE_EVENTSINK_MAP()
	void ClickSscmdGraphicView();
	void ClickSscmdSimpleView();
	void ClickSscmdAverage();
	afx_msg void OnBnClickedCancel();

};
