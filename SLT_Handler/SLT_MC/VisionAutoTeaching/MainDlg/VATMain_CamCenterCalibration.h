#pragma once
#include "VAT_PickerAlignDlgCom.h"

// CVATMain_CamCenterCalibration 대화 상자입니다.

class CVATMain_CamCenterCalibration : public CDialogEx, public CVATMain_Common, public CVAT_PickerAlignDlgCom
{
	DECLARE_DYNAMIC(CVATMain_CamCenterCalibration)

public:
	CVATMain_CamCenterCalibration(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMain_CamCenterCalibration();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_CAM_CENTER_CALIBRATION };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Main();
	void PageMoveDispose();

public: //OnTimer
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	enum _eCamCenterCalTimer
	{
		_eCamCenterCalT_HandStatus = 0,
		_eCamCenterCalT_CheckExecuteStatus,

		_eCamCenterCalT_Max,
	};

	enum _eCamCenterCalSSType
	{
		_eCamCenCalSS_BotCamCenterPos = 0,
		_eCamCenCalSS_TopCamToPkrDis,
	};

public: //OnPaint
	afx_msg void OnPaint();

private:
	//Gui에서 변경 된 Data 관리.
	St_VAT_CamCenterCalInfo m_stVATCamCenCalInfoGui;

public:
	CDSSPanel m_ssPanel_HandSelect;
	CDSSPanel m_ssPanel_MeasureStatus;

	//Can Center Pos
	CDSSPanel m_ssDataPanel_1_Title;
	CDSSPanel m_ssDataPanel_1_Sub1;
	CDSSPanel m_ssDataPanel_1_Sub2;
	int m_nSS1_DisplayType;
	CSpreadSheet m_ssBotCamCenterPos;
	_stVAT_BASE_POS_SHEET_INFO m_stBotCamCenterPosLastInfo;

	//Standard Picker to Cam Pos;
	CDSSPanel m_ssDataPanel_2_Title;
	CDSSPanel m_ssDataPanel_2_Sub1;
	CDSSPanel m_ssDataPanel_2_Sub2;
	int m_nSS2_DisplayType;
	CSpreadSheet m_ssTopCamToPkrDis;
	_stVAT_BASE_POS_SHEET_INFO m_stTopCamToPkrDisLastInfo;

	//Operate Button
	CShadeButtonST m_BtnOper1_EncoderRead;
	CShadeButtonST m_BtnOper1_MotorMove;
	CShadeButtonST m_BtnOper1_Reload;
	CShadeButtonST m_BtnOper1_Save;
	CShadeButtonST m_BtnOper1_MakeBasePos;

	CShadeButtonST m_BtnOper2_MotorMove;

	void InitCamCenterCalBasePosSheet();
	void InitCamCenterOperBtn();
	void InitCamCenterMeasureOverView();
	void InitOverViewRect();


	void DisplaySSData(int nCamCenCalSSType, int nSSDisplayType, BOOL bMakeBasePos = FALSE);
	int CheckPosLimit();

	void EnableControls(BOOL bStatus);
public:

	DECLARE_EVENTSINK_MAP()

	void ButtonClickedSpreadCamCenterCalLoadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadCamCenterCalUnloadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadCamCenterCalLoadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadCamCenterCalUnloadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadCamCenterCalLoadTestpp2(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadCamCenterCalUnloadTestpp2(long Col, long Row, short ButtonDown);

	void ClickSspanelBotCamPosData();
	void ClickSspanelBotCamPosDataDiff();
	void ClickSspanelCamToPkrDisData();
	void ClickSspanelCamToPkrDisDataDiff();

	void ClickSpreadCamCenterCalTargetPosition(long Col, long Row);
	void DblClickSpreadCamCenterCalTargetPosition(long Col, long Row);
	void ClickSpreadTopCamToPickerDistance(long Col, long Row);
	void DblClickSpreadTopCamToPickerDistance(long Col, long Row);

	afx_msg void OnBnClickedBtnCamCenterEncoderRead1();
	afx_msg void OnBnClickedBtnCamCenterMotorMove1();
	afx_msg void OnBnClickedBtnCamCenterReload1();
	afx_msg void OnBnClickedBtnCamCenterDataSave1();

	//Measure Overview
	afx_msg LRESULT OnReceiveUpdateHandStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam);
 	afx_msg LRESULT OnReceiveUpdateVisionMeasureCnt(WPARAM wParam, LPARAM lParam);
 	afx_msg LRESULT OnReceiveUpdateSavedData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnCamCenterMakeBasePos1();
	afx_msg void OnBnClickedBtnCamCenterMotorMove2();
};
