#pragma once
#include "afxwin.h"


// CSocketCleaning 대화 상자입니다.

class CSocketCleaning : public CDialog
{
	DECLARE_DYNAMIC(CSocketCleaning)

public:
	CSocketCleaning(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSocketCleaning();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SOCKET_CLEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedCleaningSave();
    afx_msg void OnEnSetfocusEditContactCnt();
    afx_msg void OnEnSetfocusEditAlarmCnt();
    afx_msg void OnEnSetfocusEditIntervalCnt();
	afx_msg void OnEnSetfocusEditContactDelayTime();
	void MakeLog(LPCTSTR fmt, ...);
	void SocketCleaningChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData);
	void SocketCleaningDataUpdate();
private:
    void OnBtnGUI();
    void OnEditBoxGUI();
	void SocketCleanDeviceCountClear(int nResourceID);
	void SocketCleanDataSave();
	void ChkLotState();
	int SocketCleanOnOffInterlock();
	std::vector<int> m_vModuleSetPressIdx[eMaxTestPPCount];
	int nSiteOnOff[eMaxPressUnitCount];

    CShadeButtonST m_BtnSave;
    CShadeButtonST m_BtnCancel;    

    int m_nCleaningOff;  
    int m_nContactCnt;
    int m_nAlarmCnt;
    int m_nIntervalCnt;
	double m_dContactDelayTime;
	BOOL m_bEventType;

    double m_dContactTime;

    CButton m_CtrlChkInitialStart;
    CButton m_CtrlChkInitialRetestStart;
    CButton m_CtrlChkInterval;

    CEditTrans m_EditAlarmCnt;
    CEditTrans m_EditContactCnt;
    CEditTrans m_EditIntervalCnt;
	CEditTrans m_EditContactTime;

	CSpreadSheet m_SprdClnDvcPos1;
	CSpreadSheet m_SprdClnDvcPos2;

	CSpreadSheet m_SprdClnDvcTestCnt1;
	CSpreadSheet m_SprdClnDvcTestCnt2;

	CSpreadSheet m_SprdRevManuCleanUp;
	CSpreadSheet m_SprdRevManuCleanDown;
public:
	afx_msg void OnBnClickedBtnResetCln1();
	afx_msg void OnBnClickedBtnResetCln2();
};
