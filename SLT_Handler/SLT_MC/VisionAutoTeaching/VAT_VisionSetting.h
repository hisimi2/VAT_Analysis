#pragma once

// CVAT_VisionSetting 대화 상자입니다.

class CVAT_VisionSetting : public CDialogEx, public CVAT_VisionComm
{
	DECLARE_DYNAMIC(CVAT_VisionSetting)

public:
	CVAT_VisionSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVAT_VisionSetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VAT_VISION_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	void InitVisionIPAndPort();
	void InitVisionLED();
	void InitComboBox();
public:
	CFont m_PageFont;
	CShadeButtonST m_BtnCancel;

	//CVAT_MeasureStatus& m_GuiNotifier = CVAT_MeasureStatus::GetInstance();
	VATSimul m_simulCtrl;

	CSliderCtrl m_sliderCamLED[VAT_CAM_MAX];

	CComboBox m_ComboBox[VAT_CAM_MAX];
	
	CShadeButtonST m_BtnMeasure[VAT_CAM_MAX];
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	enum _eVisionSetTimer
	{
		//_eVisionSetT_LEDStatus = 0,

		_eVisionSetT_Max,
	};

public:
	afx_msg void OnNMCustomDrawSliderLed(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLed(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	
	//int ConvertSliderIdToVisionDataId(UINT nID);
	afx_msg void OnBnClickedBtnVatVisionManualConnect1();
	afx_msg void OnBnClickedBtnVatVisionManualDisconnect1();
	afx_msg void OnBnClickedBtnVatVisionManualConnect2();
	afx_msg void OnBnClickedBtnVatVisionManualDisconnect2();

	afx_msg void OnBtnClickedSendMeasure(UINT nID);


	int FindCamNo(const int staticId[],  int nID);

};
