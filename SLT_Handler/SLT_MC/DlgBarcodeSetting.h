#pragma once


// DlgBarcodeSetting 대화 상자입니다.

class DlgBarcodeSetting : public CDialogEx
{
	DECLARE_DYNAMIC(DlgBarcodeSetting)

public:
	DlgBarcodeSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgBarcodeSetting();

// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BARCODE_SETTING };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	enum _eTrigger_ID
	{
		_eTrigger_1 = 0,
		_eTrigger_2,
	};

	enum _eEncoder_ID
	{
		_eEncoder_1 = 1,
		_eEncoder_2,
	};

public:
	BOOL m_bManualCmdExcute;

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	enum _eTimer
	{
		_eTimer_Connect = 0,
		_eTimer_TriggerParam,
		_eTimer_BarcodeRetryCnt,
		_eTimer_CmdBusy,

		_eTimer_Max,
	};

	CShadeButtonST m_btnStop;
	CShadeButtonST m_btnSkip;
	afx_msg void OnBnClickedButtonManuTriggerCmdSkip();
	afx_msg void OnBnClickedButtonManuTriggerCmdStop();

	CShadeButtonST m_BtnCancel;



	CStaticTrans m_StaticComStatus;

	//Manual Button
	CButton m_btnLDTable1;
	CButton m_btnLDTable2;

	CBitmap m_cBmpLDTable1;
	CBitmap m_cBmpLDTable2;
	afx_msg void OnBnClickedBtnBarcodeSettingTbl2ManuMove();
	afx_msg void OnBnClickedBtnBarcodeSettingTbl3ManuMove();
	void OnBnClickedBtnAxisManuLoadTbl(int nTableIdx);

	//=====================================Trigger Param
	void InitTriggerParam();
	//Trigger Count
	CStaticTrans m_StaticTrigger1Cnt;
	CStaticTrans m_StaticTrigger2Cnt;

	afx_msg void OnBnClickedBtnTrigger1Clear();
	afx_msg void OnBnClickedBtnTrigger2Clear();

	//Trigger Position
	CStaticTrans m_StaticTrigger1Pos;
	CStaticTrans m_StaticTrigger2Pos;

	afx_msg void OnBnClickedBtnTriggerPos1Clear();
	afx_msg void OnBnClickedBtnTriggerPos2Clear();

	//Trigger Retry Count
	CStaticTrans m_StaticTrigger1RetryCnt;
	CStaticTrans m_StaticTrigger2RetryCnt;

	afx_msg void OnBnClickedBtnTrigger1RetryCntClear();
	afx_msg void OnBnClickedBtnTrigger2RetryCntClear();


	//======================================Trigger Board Setting Param
	void InitTriggerBoardSetParam();

	void DisplayTriggerBoardType();
	afx_msg void OnStnClickedTriggerBoardType();

	void DisplayTriggerCorrection();
	afx_msg void OnStnClickedTriggerCorrection();

	void DisplayTriggerBoardType2();
	afx_msg void OnStnClickedTriggerBoardType2();

	void DisplayTriggerCorrection2();
	afx_msg void OnStnClickedTriggerCorrection2();

	CStaticTrans m_StaticResolution;
	CStaticTrans m_StaticInterval;
	CStaticTrans m_StaticDirection;

	CStaticTrans m_StaticBoardType; //LD Table 1
	CStaticTrans m_StaticCorrection;//LD Table 1

	CStaticTrans m_StaticBoardType2;//LD Table 2
	CStaticTrans m_StaticCorrection2;//LD Table 2

	CStaticTrans m_StaticTbl1_StartPos;
	CStaticTrans m_StaticTbl1_EndPos;
	CStaticTrans m_StaticTbl2_StartPos;
	CStaticTrans m_StaticTbl2_EndPos;
	

	afx_msg void OnBnClickedButtonLed1On();
	afx_msg void OnBnClickedButtonLed1Off();
	afx_msg void OnBnClickedButtonLed2On();
	afx_msg void OnBnClickedButtonLed2Off();

	void BarcodeLED_OnOff(int nOnOff, int nLedIdx);

	CBitmap m_cBmpServoMotor;
	CButton m_btnServoMotor;
	afx_msg void OnBnClickedButtonTriggerSetServoMotor();


public:
	int m_nPort;


};
