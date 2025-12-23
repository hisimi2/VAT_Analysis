#pragma once


// CDlgManuZMotorTeaching 대화 상자입니다.

class CDlgManuZMotorTeaching : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuZMotorTeaching)

public:
	CDlgManuZMotorTeaching(CString* strZTeachPos, int nCurRobotId, int nLocIdx, int nPosIdx, int nAxisNo, int nPartNo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuZMotorTeaching();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_Z_MOTOR_TEACHING };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	int m_nBoardDir;
	int GetPickerVacuum(int nHand, CPoint picker);
public:
	//외부에서 Param 입력.
	int m_nCurRobotId;
	int m_nAxisNo;
	int m_nPartNo;
	CString* m_strZTeachPos;
	int m_nLocIdx;
	int m_nPosIdx;

	int m_nHand;
	char m_szPosSWLimit[16];
	char m_szNegSWLimit[16];

	//CFont StaticFont;
public:
	void InitHandIdx(int nAxisNo);
	void DisplayData();

public:
	//Z Teach Data
	double m_dCtrlZTeachData;
	CEditTrans m_EditZTeachData;
	afx_msg void OnEnSetfocusZTeachPos();

	//Flatness Offset
	double m_dCtrlPickerData[VAT_PICKER_MAX]; //변수 저장.
	CEditTrans m_EditPickerData[VAT_PICKER_MAX]; //Contorl Design
	afx_msg void OnEnSetFocusFlatnessOffset(UINT nID);
	
	//Control Btn
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;
	afx_msg void OnBnClickedManuZTeachSave();
};
