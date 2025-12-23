#pragma once
#include "afxwin.h"


// CSPD_TM_TEST_PP 대화 상자입니다.

class CSPD_TM_TEST_PP : public CDialog
{
	DECLARE_DYNAMIC(CSPD_TM_TEST_PP)

public:
	CSPD_TM_TEST_PP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_TM_TEST_PP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPD_TM_TESTPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void MakeLog(LPCTSTR fmt, ...);
	// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	void TestPPSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData);

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	void OnBtnGUI();
	void OnEditBoxGUI();
	void ChkLotState();

	int m_nSPD_TestPP1_X1;
	int m_nACC_TestPP1_X1;
	int m_nDCC_TestPP1_X1;
	int m_nSPD_TestPP1_X2;
	int m_nACC_TestPP1_X2;
	int m_nDCC_TestPP1_X2;
	int m_nSPD_TestPP1_Y;
	int m_nACC_TestPP1_Y;
	int m_nDCC_TestPP1_Y;
	int m_nSPD_TestPP1_Z1;
	int m_nACC_TestPP1_Z1;
	int m_nDCC_TestPP1_Z1;
	int m_nSPD_TestPP1_Z2;
	int m_nACC_TestPP1_Z2;
	int m_nDCC_TestPP1_Z2;
	int m_nSPD_TestPP1_X1_Pitch;
	int m_nACC_TestPP1_X1_Pitch;
	int m_nDCC_TestPP1_X1_Pitch;
	int m_nSPD_TestPP1_X2_Pitch;
	int m_nACC_TestPP1_X2_Pitch;
	int m_nDCC_TestPP1_X2_Pitch;
	int m_nSPD_TestPP1_Y1_Pitch;
	int m_nACC_TestPP1_Y1_Pitch;
	int m_nDCC_TestPP1_Y1_Pitch;
	int m_nSPD_TestPP1_Y2_Pitch;
	int m_nACC_TestPP1_Y2_Pitch;
	int m_nDCC_TestPP1_Y2_Pitch;

	int m_nSPD_TestPP2_X1;
	int m_nACC_TestPP2_X1;
	int m_nDCC_TestPP2_X1;
	int m_nSPD_TestPP2_X2;
	int m_nACC_TestPP2_X2;
	int m_nDCC_TestPP2_X2;
	int m_nSPD_TestPP2_Y;
	int m_nACC_TestPP2_Y;
	int m_nDCC_TestPP2_Y;
	int m_nSPD_TestPP2_Z1;
	int m_nACC_TestPP2_Z1;
	int m_nDCC_TestPP2_Z1;
	int m_nSPD_TestPP2_Z2;
	int m_nACC_TestPP2_Z2;
	int m_nDCC_TestPP2_Z2;
	int m_nSPD_TestPP2_X1_Pitch;
	int m_nACC_TestPP2_X1_Pitch;
	int m_nDCC_TestPP2_X1_Pitch;
	int m_nSPD_TestPP2_X2_Pitch;
	int m_nACC_TestPP2_X2_Pitch;
	int m_nDCC_TestPP2_X2_Pitch;
	int m_nSPD_TestPP2_Y1_Pitch;
	int m_nACC_TestPP2_Y1_Pitch;
	int m_nDCC_TestPP2_Y1_Pitch;
	int m_nSPD_TestPP2_Y2_Pitch;
	int m_nACC_TestPP2_Y2_Pitch;
	int m_nDCC_TestPP2_Y2_Pitch;
	

	double m_dTestPP1_Vacuum;
	double m_dTestPP2_Vacuum;

	double m_dTestPP1_Blow;
	double m_dTestPP2_Blow;
	

	CEditTrans m_editSPD_TestPP1_X1;
	CEditTrans m_editACC_TestPP1_X1;
	CEditTrans m_editDCC_TestPP1_X1;
	CEditTrans m_editSPD_TestPP1_X2;
	CEditTrans m_editACC_TestPP1_X2;
	CEditTrans m_editDCC_TestPP1_X2;
	CEditTrans m_editSPD_TestPP1_Y;
	CEditTrans m_editACC_TestPP1_Y;
	CEditTrans m_editDCC_TestPP1_Y;
	CEditTrans m_editSPD_TestPP1_Z1;
	CEditTrans m_editACC_TestPP1_Z1;
	CEditTrans m_editDCC_TestPP1_Z1;
	CEditTrans m_editSPD_TestPP1_Z2;
	CEditTrans m_editACC_TestPP1_Z2;
	CEditTrans m_editDCC_TestPP1_Z2;
	CEditTrans m_editSPD_TestPP1_X1_Pitch;
	CEditTrans m_editACC_TestPP1_X1_Pitch;
	CEditTrans m_editDCC_TestPP1_X1_Pitch;
	CEditTrans m_editSPD_TestPP1_X2_Pitch;
	CEditTrans m_editACC_TestPP1_X2_Pitch;
	CEditTrans m_editDCC_TestPP1_X2_Pitch;
	CEditTrans m_editSPD_TestPP1_Y1_Pitch;
	CEditTrans m_editACC_TestPP1_Y1_Pitch;
	CEditTrans m_editDCC_TestPP1_Y1_Pitch;
	CEditTrans m_editSPD_TestPP1_Y2_Pitch;
	CEditTrans m_editACC_TestPP1_Y2_Pitch;
	CEditTrans m_editDCC_TestPP1_Y2_Pitch;
	CEditTrans m_editSPD_TestPP2_X1;
	CEditTrans m_editACC_TestPP2_X1;
	CEditTrans m_editDCC_TestPP2_X1;
	CEditTrans m_editSPD_TestPP2_X2;
	CEditTrans m_editACC_TestPP2_X2;
	CEditTrans m_editDCC_TestPP2_X2;
	CEditTrans m_editSPD_TestPP2_Y;
	CEditTrans m_editACC_TestPP2_Y;
	CEditTrans m_editDCC_TestPP2_Y;
	CEditTrans m_editSPD_TestPP2_Z1;
	CEditTrans m_editACC_TestPP2_Z1;
	CEditTrans m_editDCC_TestPP2_Z1;
	CEditTrans m_editSPD_TestPP2_Z2;
	CEditTrans m_editACC_TestPP2_Z2;
	CEditTrans m_editDCC_TestPP2_Z2;
	CEditTrans m_editSPD_TestPP2_X1_Pitch;
	CEditTrans m_editACC_TestPP2_X1_Pitch;
	CEditTrans m_editDCC_TestPP2_X1_Pitch;
	CEditTrans m_editSPD_TestPP2_Y1_Pitch;
	CEditTrans m_editACC_TestPP2_Y1_Pitch;
	CEditTrans m_editDCC_TestPP2_Y1_Pitch;
	CEditTrans m_editSPD_TestPP2_X2_Pitch;
	CEditTrans m_editACC_TestPP2_X2_Pitch;
	CEditTrans m_editDCC_TestPP2_X2_Pitch;
	CEditTrans m_editSPD_TestPP2_Y2_Pitch;
	CEditTrans m_editACC_TestPP2_Y2_Pitch;
	CEditTrans m_editDCC_TestPP2_Y2_Pitch;
	

	CEditTrans m_editTestPP1_Vacuum;
	CEditTrans m_editTestPP2_Vacuum;
	CEditTrans m_editTestPP1_Blow;
	CEditTrans m_editTestPP2_Blow;
	CEditTrans m_editTestPP1_Retry;
	CEditTrans m_editTestPP2_Retry;

	CEditTrans m_editTestPP1_Pick_Retry_Offset;
	CEditTrans m_editTestPP2_Pick_Retry_Offset;


	CEditTrans m_editTestPP1_Picker_Wait_Time;
	CEditTrans m_editTestPP2_Picker_Wait_Time;

	CEditTrans m_editTestSiteRetouchDis;

	CEditTrans m_Edit_Pick_1Stage_Offset;
	CEditTrans m_Edit_Pick_1Stage_Speed;

	int m_nRetryCountPP1;
	int m_nRetryCountPP2;

	double m_dTestPP_Pick_1Stage_Offset;
	double m_dTestPP_Pick_1Stage_Speed;

	double m_dTestPP1_Pick_Retry_Offset;
	double m_dTestPP2_Pick_Retry_Offset;

	double m_dTestPP1_Picker_Wait_Time;
	double m_dTestPP2_Picker_Wait_Time;

	BOOL   m_bTestSiteVisionOn;
	BOOL   m_bBackVisionOnOff;
	
	BOOL   m_bReTouch;
	double m_dReTouch_Dis;

public:
	afx_msg void OnBnClickedButtonSave();
	
	afx_msg void OnEnSetfocusEditSpdXTest1();
	afx_msg void OnEnSetfocusEditAccXTest1();
	afx_msg void OnEnSetfocusEditDccXTest1();
	afx_msg void OnEnSetfocusEditSpdYTest1();
	afx_msg void OnEnSetfocusEditAccYTest1();
	afx_msg void OnEnSetfocusEditDccYTest1();

	afx_msg void OnEnSetfocusEditSpdXTest2();
	afx_msg void OnEnSetfocusEditAccXTest2();
	afx_msg void OnEnSetfocusEditDccXTest2();
	afx_msg void OnEnSetfocusEditSpdYTest2();
	afx_msg void OnEnSetfocusEditAccYTest2();
	afx_msg void OnEnSetfocusEditDccYTest2();

	afx_msg void OnEnSetfocusEditVacuumDelayPp1();
	afx_msg void OnEnSetfocusEditBlowDelayPp1();
	afx_msg void OnEnSetfocusEditVacuumDelayPp2();
	afx_msg void OnEnSetfocusEditBlowDelayPp2();
	afx_msg void OnEnSetfocusEditTestPp1Retry();
	afx_msg void OnEnSetfocusEditTestPp2Retry();

	CButton m_ChkExistDevice;
	CButton m_ChkAfterExistDevice;
	CButton m_ChkExistDevice_Sensor;
	CButton m_ChkArtPickPlace;
	CButton m_ChkPreVacuum;
	CButton m_ChkSiteArrivedSen;
	CButton m_ChkSiteMoveArrivedSen;
	CButton m_ChkTestppStationFR_PNP;
	CButton m_ChkLoadHandAlreadyPick;

	afx_msg void OnClickedTestSiteVision(UINT nID);
	afx_msg void OnEnSetfocusEditTestPpRetouch();


	afx_msg void OnEnSetfocusEditSpdX2Test1();
	afx_msg void OnEnSetfocusEditAccX2Test1();
	afx_msg void OnEnSetfocusEditDccX2Test1();
	afx_msg void OnEnSetfocusEditSpdX2Test2();
	afx_msg void OnEnSetfocusEditAccX2Test2();
	afx_msg void OnEnSetfocusEditDccX2Test2();
	afx_msg void OnEnSetfocusEditSpdZ1Test1();
	afx_msg void OnEnSetfocusEditAccZ1Test1();
	afx_msg void OnEnSetfocusEditDccZ1Test1();
	afx_msg void OnEnSetfocusEditSpdZ2Test1();
	afx_msg void OnEnSetfocusEditAccZ2Test1();
	afx_msg void OnEnSetfocusEditDccZ2Test1();
	afx_msg void OnEnSetfocusEditSpdX1PitchTest1();
	afx_msg void OnEnSetfocusEditAccX1PitchTest1();
	afx_msg void OnEnSetfocusEditDccX1PitchTest1();
	afx_msg void OnEnSetfocusEditSpdY1PitchTest1();
	afx_msg void OnEnSetfocusEditAccY1PitchTest1();
	afx_msg void OnEnSetfocusEditDccY1PitchTest1();
	afx_msg void OnEnSetfocusEditSpdX2PitchTest1();
	afx_msg void OnEnSetfocusEditAccX2PitchTest1();
	afx_msg void OnEnSetfocusEditDccX2PitchTest1();
	afx_msg void OnEnSetfocusEditSpdY2PitchTest1();
	afx_msg void OnEnSetfocusEditAccY2PitchTest1();
	afx_msg void OnEnSetfocusEditDccY2PitchTest1();
	afx_msg void OnEnSetfocusEditSpdZ1Test2();
	afx_msg void OnEnSetfocusEditAccZ1Test2();
	afx_msg void OnEnSetfocusEditDccZ1Test2();
	afx_msg void OnEnSetfocusEditSpdZ2Test2();
	afx_msg void OnEnSetfocusEditAccZ2Test2();
	afx_msg void OnEnSetfocusEditDccZ2Test2();
	afx_msg void OnEnSetfocusEditSpdX1PitchTest2();
	afx_msg void OnEnSetfocusEditAccX1PitchTest2();
	afx_msg void OnEnSetfocusEditDccX1PitchTest2();
	afx_msg void OnEnSetfocusEditSpdY1PitchTest2();
	afx_msg void OnEnSetfocusEditAccY1PitchTest2();
	afx_msg void OnEnSetfocusEditDccY1PitchTest2();
	afx_msg void OnEnSetfocusEditSpdX2PitchTest2();
	afx_msg void OnEnSetfocusEditAccX2PitchTest2();
	afx_msg void OnEnSetfocusEditDccX2PitchTest2();
	afx_msg void OnEnSetfocusEditSpdY2PitchTest2();
	afx_msg void OnEnSetfocusEditAccY2PitchTest2();
	afx_msg void OnEnSetfocusEditDccY2PitchTest2();
	afx_msg void OnEnSetfocusEditTestPpPick1stageOffset();
	afx_msg void OnEnSetfocusEditTestPpPick1stageSpeed();
	afx_msg void OnEnSetfocusEditTestPp1PickRetryOffset();
	afx_msg void OnEnSetfocusEditTestPp2PickRetryOffset();
	afx_msg void OnEnSetfocusEditTestPp1PickerWaitTime();
	afx_msg void OnEnSetfocusEditTestPp2PickerWaitTime();
};
