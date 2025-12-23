#pragma once

#include "ShadeButtonST.h"
// CSocketCount 대화 상자입니다.

class CSocketCount : public CDialog
{
	DECLARE_DYNAMIC(CSocketCount)

public:
	CSocketCount(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSocketCount();

	enum { IDD = IDD_DIALOG_SOCKET_COUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CSpreadSheet   m_SprdSocketCount;
	CSpreadSheet   m_SprdSocketCount_Offline;
	int m_nMaxOnlineRowNum;
	ST_SOCKET_CONTACT_COUNT m_stCountInfo;
	int m_nLastSel_Row;
	BOOL m_bDataDelete;
	BOOL m_bClick;
	BOOL m_bAddLimit;

	CShadeButtonST m_BtnSelectDelete;
	CShadeButtonST m_BtnAllDelete;
	CShadeButtonST m_BtnFind;
	CShadeButtonST m_BtnAllDataShow;
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnExit;
	CShadeButtonST m_BtnAddLimit;
	CShadeButtonST m_BtnAllDataUpdate;

	int m_nLimit;
	int m_nAddLimit;

	char m_szPathFileName[MAX_PATH];

	int m_nTesterCount;
	int m_nSocketCount;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void Initialize();
	afx_msg void OnBnClickedButtonSelectDelete();
	afx_msg void OnBnClickedButtonAllDelete();
	afx_msg void OnBnClickedButtonSave();

	afx_msg void OnBnClickedButtonFindId();

	void DisplaySpread(ST_SOCKET_CONTACT_COUNT stCountInfo);
	DECLARE_EVENTSINK_MAP()
	void ClickSpreadCount(long Col, long Row);
	afx_msg void OnBnClickedButtonAllDataShow();
	void OnBtnGUI();
	void MakeLog(LPCTSTR fmt, ...);

	void StationNumSetting();
	afx_msg void OnEnSetfocusEditLimitCount();
	afx_msg void OnEnSetfocusEditAddLimitCount();
	afx_msg void OnBnClickedButtonSetAddLimit();
	afx_msg void OnBnClickedButtonAllDataUpdate();
	afx_msg void OnDestroy();
};
