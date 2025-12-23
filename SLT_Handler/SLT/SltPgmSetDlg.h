#pragma once
#include "afxwin.h"


// CSltPgmSetDlg 대화 상자입니다.

class CSltPgmSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSltPgmSetDlg)

public:
	CSltPgmSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSltPgmSetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SLT_PGM_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CString m_strPgmName;

	CListBox m_ListPGM;

//	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnModify;
	CShadeButtonST m_BtnDelete;
	CShadeButtonST m_BtnNew;

	CString m_strGetSelectListbox;
	int     m_nListIndex;

	void OnBtnGUI();
	void OnRadioBtnDisplayUpdate();
	void InitListData();
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnLbnSelchangeListPgm();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonModify();
	void MakeLog(LPCTSTR fmt, ...);
};
