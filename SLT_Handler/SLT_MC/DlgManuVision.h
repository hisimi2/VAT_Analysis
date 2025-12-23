#pragma once
#include "afxwin.h"


// CDlgManuVision 대화 상자입니다.

class CDlgManuVision : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuVision)

public:
	CDlgManuVision(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuVision();
	void OnConnection(int nVisionPc);
	void GetTimeString(CString& strTimeString);
	void UpdateEventMessage(const char* fmt, ...);
	void UpdateVisionState();

	std::vector<CVisionComm*> m_vVisionCommObj;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_VISION_IF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConn1();
	CComboBox m_cbMessageList;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonConn2();
	afx_msg void OnBnClickedButtonConn3();
	CComboBox m_cbTargetSel1;
	CComboBox m_cbTargetSel2;
	afx_msg void OnCbnSelchangeComboTarget1();
	afx_msg void OnCbnSelchangeComboTarget2();
	afx_msg void OnBnClickedButtonCmdSetCok();
	afx_msg void OnBnClickedButtonCmdCleanDisk();
	afx_msg void OnBnClickedButtonCmdReqResult();
	afx_msg void OnBnClickedButtonCmdInspReady();
	afx_msg void OnBnClickedButtonCmdVisionShutDown();
	afx_msg void OnBnClickedButtonCmdReqMeasure();
	CEdit m_editManuOperEvent;
	afx_msg LRESULT OnVisionSockEvent(WPARAM wParma, LPARAM lParam);
	afx_msg LRESULT OnVisionLogMsg(WPARAM wParma, LPARAM lParam);

	void ButtonClickLog(int nBtnNum);
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedButtonCmdVisionReadyStart();
	afx_msg void OnBnClickedButtonCmdVisionReadyEnd();
	afx_msg void OnBnClickedOk();
};
