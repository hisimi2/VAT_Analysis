#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCategorySetupDlg 대화 상자입니다.

class CCategorySetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CCategorySetupDlg)

public:
	CCategorySetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCategorySetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CATEGORY_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CEdit     m_EditCategoryText;
	CListBox  m_ListCategory;
	CListCtrl m_ListDataDisplay;

	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnModify;
	CShadeButtonST m_BtnConnect;
	CShadeButtonST m_BtnNew;
	CShadeButtonST m_BtnDel;

	void UpdateListData();
	void OnBtnGUI();
	void UpdateDataDisplay(ST_CATE_INFO lpstData);
	BOOL CheckDataSave();

	ST_CATE_INFO m_stCateData;
	CString		 m_strListSelName;

	int m_nProjectName;
public:
	afx_msg void OnBnClickedButtonModify();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListCate();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDel();
	void MakeLog(LPCTSTR fmt, ...);
};
