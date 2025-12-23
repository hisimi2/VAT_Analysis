#pragma once
#include "stdafx.h"

// CManuMotorZAxisDlg 대화 상자입니다.

class CManuMotorZAxisDlg : public CDialog
{
	DECLARE_DYNAMIC(CManuMotorZAxisDlg)

public:
	CManuMotorZAxisDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManuMotorZAxisDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANU_MOTOR_ZAXIS };
#endif
private:
	int m_nAxisNum;

	int m_nJogSpeed;
	int m_ZrStat[MAX_PICKER_Z_AXIS_CNT];
	int m_MotorStat[MAX_PICKER_Z_AXIS_CNT];
	int m_ServoOnOff[MAX_PICKER_Z_AXIS_CNT];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_Combo_RobotChoose;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboAxisChoose();
	afx_msg void OnCbnSelchangeComboMSpeedZ();
	CComboBox m_motor_speed;

	CDSSPanel	m_ZrStat01;
	CDSSPanel	m_ZrStat02;
	CDSSPanel	m_ZrStat03;
	CDSSPanel	m_ZrStat04;
	CDSSPanel	m_ZrStat05;
	CDSSPanel	m_ZrStat06;
	CDSSPanel	m_ZrStat07;
	CDSSPanel	m_ZrStat08;

	CDSSPanel  m_MotorStat01;
	CDSSPanel  m_MotorStat02;
	CDSSPanel  m_MotorStat03;
	CDSSPanel  m_MotorStat04;
	CDSSPanel  m_MotorStat05;
	CDSSPanel  m_MotorStat06;
	CDSSPanel  m_MotorStat07;
	CDSSPanel  m_MotorStat08;

	CDSSPanel m_ServoOnOff01;
	CDSSPanel m_ServoOnOff02;
	CDSSPanel m_ServoOnOff03;
	CDSSPanel m_ServoOnOff04;
	CDSSPanel m_ServoOnOff05;
	CDSSPanel m_ServoOnOff06;
	CDSSPanel m_ServoOnOff07;
	CDSSPanel m_ServoOnOff08;

	CDSSPanel	m_ctrlPlus;
	CDSSPanel  m_ctrlMinus;

	int m_nAxisNo;
	BOOL m_nBoardDir;

	void ShowZRStatus();
	void ShowAmpFaultStatus();
	void ShowServoStatus();
	int PickerStartInx();
	int PickerAxisNumber();
	_eBbxmnpAxis CHANGE(int eAxis); //Board Type에 따라 Picker Index변경 

	std::vector<int> m_vPkrIdx;

	void MouseDownColorbtctrlZaxisJogPlus(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlZaxisJogPlus(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlZaxisJogMinus2(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlZaxisJogMinus2(short Button, short Shift, long X, long Y);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonZr();
	afx_msg void OnBnClickedButtonServoOnoff();
	afx_msg void OnBnClickedButtonStop();
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnDestroy();
};
