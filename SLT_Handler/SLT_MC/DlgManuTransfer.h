#pragma once
#include "afxwin.h"

#include "BtnST.h"
#include "ShadeButtonST.h"
// CDlgManuTransfer 대화 상자입니다.

enum eManualCyl_Position{
	eManualCyl_Position_Trans=0,
	eManualCyl_Position_Ev_L,
	eManualCyl_Position_Ev_R,
};

class CDlgManuTransfer : public CDialogEx, public CThreadBase
{
	DECLARE_DYNAMIC(CDlgManuTransfer)

public:
	CDlgManuTransfer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuTransfer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_TRANSFER };

	enum{
		eClickBtn_Clamp =0,
		eClickBtn_UnClamp,
		eClickBtn_Up,
		eClickBtn_Down,
		eClickBtn_Stop,
	};

	ST_CYLINDER_SPEC m_dCylinderSpec[eClickBtn_Stop];

	void CylinderSpecUpdate();
	void CylinderSpecInOut(CDC* pDC);
	void CylinderSpecIndex(int& nCmdManualCyl);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();

	void CylinderManualAction();

	void CylManualOper_None();
	void CylManualOper_Only_One_Action_and_No_Check_Sensor();
	void CylManualOper_Only_One_Action_and_Check_Sensor();
	void CylManualOper_Continue_Action_and_No_Check_Sensor();
	void CylManualOper_Continue_Action_and_Check_Sensor();
	void CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
	void ThreadProc1();

	int m_nCylManualOpActState;
	int m_nCylManualActionStep;

	void SetCommand(int nCmd, int nPosition = 0,int nHeadIdx = 0);
	void ClearCommand();
	int do_CylCommand(BOOL bUndo=FALSE);
	int Check_CylSensor(BOOL bUndo=FALSE);

	void DisplayCylinderActionTime(BOOL bClear=FALSE);
	void CylinderNum(BOOL bUpDown);
	int m_nCmdManualCyl; // picker up/down, vacuum/blow
	int m_nCmdOperation;
	int m_nCmdParam_Time;
	int m_nCmdPos;
	int m_nCmdLoc;
	int m_nCmdCycleCount;

	double m_dTimeCheck;
	double m_dCylActionTimeRecord;
	int m_nCylActionTimeRetry;
	int m_nCylinderNum;

private:
	CMFCButton m_BtnTrans_Up;
	CMFCButton m_BtnTrans_Down;
	CMFCButton m_BtnTrans_Clamp;
	CMFCButton m_BtnTrans_Unclamp;	

	CShadeButtonST m_BtnStop;

	CComboBox m_cbOperationSel;
	CComboBox m_cbTimeSel;


	void UpdateSensorSts(int nCtrlId, int nDiId);
	void UpdateCylinderSts(int nCtrlId, int nStatus);
public:
	afx_msg void OnBnClickedButtonTransCylActStop();

	CBitmap m_cBmpExit;

	CButton m_btnExit;
	
	
	void MakeLog(LPCTSTR fmt, ...);
	void ButtonClickLog(int nBtnNum);

	afx_msg void OnBnClickedBtnTransferUp1();
	afx_msg void OnBnClickedBtnTransferDown1();
	afx_msg void OnBnClickedBtnTransferClamp1();
	afx_msg void OnBnClickedBtnTransferUnclamp1();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
