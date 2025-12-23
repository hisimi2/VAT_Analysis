#pragma once

#include "../Include/OcxFiles/sscommand.h"
#include "ShadeButtonST.h"
// CDlgManuMotorJog 대화 상자입니다.

#define MAX_ROW (2)
#define MAX_COL (8)

class CDlgManuMotorJog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuMotorJog)

public:
	CDlgManuMotorJog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuMotorJog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_MOTOR_JOG_TEACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnClose();

	OneAxis* m_pMotorX;
	OneAxis* m_pMotorY;
	OneAxis* m_pMotorZ;
	OneAxis* m_pMotorR;

public:
	int     m_nSelRobot;
	CString m_strRobotName;
	CString m_strTitleName;
	std::vector<int> m_vAxisNo;
	int m_nTestPPHandType;

	CButton m_btnCylinder;
	CBitmap m_cBmpPress;
	CBitmap m_cBmpLoadTestPP1;
	CBitmap m_cBmpLoadTestPP2;
	CBitmap m_cBmpTrayPP;
	CBitmap m_cBmpTrayFeeder;
	CBitmap m_cBmpTransfer;

	CShadeButtonST m_btnSTOperation;
	CButton m_btnOperation;

	CBitmap m_cBmpTestPP1Axis;
	CBitmap m_cBmpTestPP2Axis;
	CBitmap m_cBmpTrayPPAxis;
	CBitmap m_cBmpLoadTable1Axis;
	CBitmap m_cBmpLoadTable2Axis;
	CBitmap m_cBmpLoadTable3Axis;
	CBitmap m_cBmpLoadTable4Axis;
	CBitmap m_cBmpTrayFeederAxis;
	CBitmap m_cBmpTransferAxis;

	CShadeButtonST m_btnStop;
	CShadeButtonST m_btnSkip;
	CButton m_chkVisionLED;
	CString m_sTestHandName;
	//CButton m_btnStop;
	//CBitmap m_cBmpStop;
	CDSSPanel m_SSPANELTitle;

private:
	void InitUI();
	std::vector<int> m_vHandGui;
	std::vector<int> m_vSpeed;
	std::vector<int> m_vRotateSpeed;
	std::vector<int> m_vZSpeed;
	std::vector<int> m_vTransfer;
	BOOL m_bServoStat;
	CSSCommand	m_btnServoFree;


	BOOL m_bManualCmdExcute;

	int m_nHandSenID[ MAX_ROW ][ MAX_COL ];
	int m_nHandVacBlwSen[ MAX_ROW ][ MAX_COL ];

	int m_nJogSpeed;
	int m_nContactForceAxisNo;

	void UpdateSensorSts(int nCtrlId, int nDiId);
	void InitMotorAxis();
	void UpdateSenValue(int nCtrlId, int nVal);
public:
	void ClickCbuttonAllVacuum();
	void ClickCbuttonAllBlow();
	void ClickBtnServoFree();
	void MouseDownColorbtctrlZdown(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlZdown(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlZup(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlZup(short Button, short Shift, long X, long Y);
	void MouseDownIdcColorbtctrlUp(short Button, short Shift, long X, long Y);
	void MouseUpIdcColorbtctrlUp(short Button, short Shift, long X, long Y);
	void MouseDownIdcColorbtctrlDown(short Button, short Shift, long X, long Y);
	void MouseUpIdcColorbtctrlDown(short Button, short Shift, long X, long Y);
	void MouseDownIdcColorbtctrlLeft(short Button, short Shift, long X, long Y);
	void MouseUpIdcColorbtctrlLeft(short Button, short Shift, long X, long Y);
	void MouseDownIdcColorbtctrlRight(short Button, short Shift, long X, long Y);
	void MouseUpIdcColorbtctrlRight(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlRPlus(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlRPlus(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlRMinus(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlRMinus(short Button, short Shift, long X, long Y);

	void OnBnClickedBtnAixsManuPress(int nPressIdx);
	void OnBnClickedBtnAixsManuLoadTbl(int nTableIdx);
	void OnBnClickedBtnAixsManuTestPp(int nTestPPIdx);
	void InitCylinderUI();
	void InitUnitOperationUI();

	void OnBnClickedBtnCylinderManual();
	void OnBnClickedBtnUnitOperation();
	

	void MakeLog(LPCTSTR fmt, ...);
		
	int FindAxisNum(int nSelRobot);
	enum eJogMotorAxis{
		eJogMotor_X = 0,
		eJogMotor_Y,
		eJogMotor_Z,
		eJogMotor_R,
	};
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	int ComInterlock(int nAxis);
public:
	afx_msg void OnBnClickedButtonCylinder();
	afx_msg void OnBnClickedButtonUnitOperation();
	afx_msg void OnBnClickedButtonStUnitOperation();
	afx_msg void OnBnClickedButtonManuProcCmdStop();
	afx_msg void OnBnClickedButtonManuProcCmdSkip();
	afx_msg void OnBnClickedCheckVisionLed();
};
