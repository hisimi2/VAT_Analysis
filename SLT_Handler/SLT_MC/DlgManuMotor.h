#pragma once

#include "MyList.h"
#include "../Include/OcxFiles/dsspanel.h"


// CDlgManuMotor 대화 상자입니다.

class CDlgManuMotor : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuMotor)

public:
	CDlgManuMotor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuMotor();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_MOTOR };

private:
	void UpdateMotorStatus(int nAxisNo);
	void UpdateJogButtonName(int nAxisNo);
	void SetDlgItemOriginInterLock(BOOL bStatus);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickColorbtctrlServo();
	void ClickColorbtctrlZr();
	void ClickColorbtctrlReset();
	void MouseDownColorbtctrlJogPlus(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlJogPlus(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlJogMinus(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlJogMinus(short Button, short Shift, long X, long Y);
	afx_msg void OnCbnSelchangeComboRobotChoose();
	afx_msg void OnCbnSelchangeComboMSpeed();


public:
	double	    m_dJogSpeed;			// pps
	CString     m_strLocation;
	BOOL	    m_bPositiveLimitCheck;
	BOOL	    m_bNegativeLimitCheck;
	CComboBox   m_cbRobotChoose;
	CComboBox   m_cbMotorSpeed;
	CDSSPanel	m_ZrStat; 
	CDSSPanel	m_Servo;
	CDSSPanel	m_AmpFaultStat;
	CDSSPanel	m_MotorUsableStat;
	CDSSPanel	m_ServoStat;
	CDSSPanel	m_ctrlPlus;
	CDSSPanel	m_ctrlMinus;
	CString     m_strSelectMotorName;

private:
	CMyList     m_listMotorParam;
	int         m_nAxisNo;
	int			m_nMotorSpeed;

	int ComInterlock(int nAxisNo);
public:
	void ClickColorbtctrlStop();

	CBitmap m_cBmpExit;

	CButton m_btnExit;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonZaxisPicker();
	void ClickMotorTitle();
};
