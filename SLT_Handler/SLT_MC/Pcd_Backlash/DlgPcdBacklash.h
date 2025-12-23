#pragma once
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "afxwin.h"
#include "../Include/OcxFiles/spreadsheet.h"

// CDlgPcdBacklash 대화 상자입니다.

enum ePcdBacklashUnit
{
	ePcdBacklash_TrayLoadPP,
	ePcdBacklash_TrayUnloadPP,
	ePcdBacklash_TestPP1,
	ePcdBacklash_TestPP2,
	eMaxPcdBacklash_Unit,
};
enum ePcdBacklashAxis
{
	eAxisX,
	eAxisY,
	eAxisZ,
	eAxisX2,
	eAxisZ2,
};

class CDlgPcdBacklash : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPcdBacklash)

public:
	CDlgPcdBacklash(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPcdBacklash();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCDBACKLASH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg	void OnRadioButtonPcdAxisChanged(UINT ID);
	afx_msg	void OnRadioButtonBacklashAxisChanged(UINT ID);
	virtual BOOL OnInitDialog();

private:
	//feild
	std::vector<int> m_vRobotNo[eMaxPcdBacklash_Unit];
	UINT	m_nPcdAxis;
	UINT	m_nBacklashAxis;
	int		m_nRepeat;
	void HandSelectUIShowHide(BOOL bOnTimer=FALSE);
	//void UnitSelectUIShowHide();
	void SpreadSheetUIShowHide(BOOL bShow, BOOL bTrayLoadPP=FALSE);
	//method
	void InitControls();
	void DisplayCurrentMotorPos();
	void UpdateTeachingData();
	void UpdateOffsetData();
	void UpdateTestPpSafetyData();
	void OnBtnGui();
	void SaveScaleFactor(int nLogicalAxis, double dNewScaleFactor);
	void SetBacklashRepeat(BOOL &repeat, int &repeatCnt, int &tempRepeatCnt);
	void UpdateVisionConnectStatus();
	void ChangedBacklashPosition(int nAxis);
	void TrayPPBacklashPosition(int nAxis);
	void TestPPBacklashPosition(int nAxis);
	void ReloadMotionDB();
	void SaveBacklashPosition();
	void ClearSpreadBackColor(BOOL bOnTimer = FALSE);
	void ClearSpreadSelectBackColor(int nRow);
	void SetSpreadBackColor(int nRow, int nCol);
	void SetSpreaddisableBackColor();
	void SaveBacklashData();
	void SavePcdPositionX();
	void SavePcdPositionY();

	BOOL ChkPcdPosInterlock();
	BOOL ChkBacklashPosInterlock();
	BOOL ChkTestPPSafetyPosValue();
	
	CButton m_cbRepeat;
	CComboBox m_cbRobotName;
	CComboBox m_cbHandName;
	CComboBox m_cbBacklashSpd;
	CComboBox m_cbBacklashCnt;
	CComboBox m_cbBacklashDis;

	CComboBox m_cbPcdDis;


	//Accuracy
	CComboBox m_cbAccuracyCnt;
	CComboBox m_cbAccuracySpd;
	CComboBox m_cbAccuracyDis;
	void SaveAccuracyPosition();
	BOOL AccuracyPositionInterlock();
	int 	m_nAccuracyAxis;
	//

	//Repeatability
	CComboBox m_cbRepeatabilityCnt;
	CComboBox m_cbRepeatabilitySpd;
	void SaveRepeatabilityPosition();
	//

	CSpreadSheet m_SprdCurPos;
	CSpreadSheet m_SprdBacklashOffset;

	CShadeButtonST m_btnPcdReadEncX;
	CShadeButtonST m_btnPcdSavePosX;
	CShadeButtonST m_btnPcdReadEncY;
	CShadeButtonST m_btnPcdSavePosY;
	CShadeButtonST m_btnPcdStart;
	CShadeButtonST m_btnSaveSF;
	CShadeButtonST m_btnConnVision;
	CShadeButtonST m_btnConnVision2;
	CShadeButtonST m_btnStop;
	CShadeButtonST m_btnExit;
	CShadeButtonST m_btnBacklashReadEnc;
	CShadeButtonST m_btnBacklashSavePos;
	CShadeButtonST m_btnBacklashManualStart;
	CShadeButtonST m_btnBacklashAutoStart;
	CShadeButtonST m_btnBacklashTest;
	CShadeButtonST m_btnBacklashSave;
	CShadeButtonST m_btnTestppSafetySave;
	CShadeButtonST m_btnServo;
	CShadeButtonST m_btncancel;
	

	CButton m_btn_Stop;
	CButton m_btnSkip;

	CBitmap m_cBmpSkip;
	CBitmap m_cBmpStop;

	CEdit m_edit_vision_ip;
	CEdit m_edit_vision_port;
	CEdit m_edit_vision_ip2;
	CEdit m_edit_vision_port2;
	
	int m_nBacklashOffSet;

	BOOL m_bManualCmdExcute;

public:
	afx_msg void OnCbnSelchangeCbRobotChoose();
	afx_msg void OnBnClickedBtnManualStop();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnPcdXReadEncoder();
	afx_msg void OnBnClickedBtnPcdXSavePos();
	afx_msg void OnBnClickedBtnPcdYReadEncoder();
	afx_msg void OnBnClickedBtnPcdYSavePos();
	afx_msg void OnBnClickedBtnPcdStart();
	afx_msg void OnBnClickedBtnPcdSaveScaleFactor();
	afx_msg void OnBnClickedBtnSaveTargetPos();
	afx_msg void OnBnClickedBtnBacklashReadEncoder();
	afx_msg void OnBnClickedBtnTestPpSafetyZ();
	afx_msg void OnEnSetfocusEditPcdXaxisX();
	afx_msg void OnEnSetfocusEditPcdXaxisY();
	afx_msg void OnEnSetfocusEditPcdXaxisZ();
	afx_msg void OnEnSetfocusEditPcdYaxisX();
	afx_msg void OnEnSetfocusEditPcdYaxisY();
	afx_msg void OnEnSetfocusEditPcdYaxisZ();
	afx_msg void OnEnSetfocusEditSafetyUpper();
	afx_msg void OnEnSetfocusEditSafetyLower();
	afx_msg void OnEnSetfocusEditBacklashX();
	afx_msg void OnEnSetfocusEditBacklashY();
	afx_msg void OnEnSetfocusEditBacklashZ();
	
	afx_msg void OnBnClickedPcdbacklasRepeat();
	afx_msg void OnBnClickedBtnBacklashManualStart();
	afx_msg void OnBnClickedBtnBacklashAutoStart();
	
	afx_msg void OnEnSetfocusEditBacklashRepeat();
	afx_msg void OnBnClickedBtnBacklashAutoTest();
	afx_msg void OnBnClickedBtnBacklashSave();
	afx_msg void OnBnClickedBtnVisionConnect();
	
	afx_msg void OnBnClickedBtnPcdbacklashServo();

	void ReportBacklashResult(int nAxis, int nSpeed);
	void ReportBacklashDone();
	afx_msg void OnBnClickedBtnAccuracyReadEncoder();
	afx_msg void OnBnClickedBtnRepeatabilityReadEncoder();
	afx_msg void OnBnClickedBtnSaveRepeatabilityTargetPos();
	afx_msg void OnBnClickedBtnSaveAccuracyTargetPos();
	afx_msg void OnEnSetfocusEditRepeatabilityX();
	afx_msg void OnEnSetfocusEditRepeatabilityY();
	afx_msg void OnEnSetfocusEditRepeatabilityZ();
	afx_msg void OnEnSetfocusEditAccuracyX();
	afx_msg void OnEnSetfocusEditAccuracyY();
	afx_msg void OnEnSetfocusEditAccuracyZ();
	afx_msg void OnBnClickedBtnRepeatabilityStart();
	afx_msg void OnBnClickedBtnRepeatabilityAllStart();
	afx_msg void OnBnClickedBtnAccuracyStart();
	afx_msg void OnCbnSelchangeBacklashDistance();
	afx_msg void OnCbnSelchangeBacklashCnt();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCbHandChoose();
	afx_msg void OnBnClickedBtnVisionConnect2();
	afx_msg void OnBnClickedButtonManuProcCmdSkip();
	afx_msg void OnBnClickedButtonManuProcCmdStop();
	afx_msg void OnBnClickedCancel3();
};
