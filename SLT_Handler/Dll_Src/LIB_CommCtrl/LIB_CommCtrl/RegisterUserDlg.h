#pragma once
#include "afxwin.h"
#include "resource.h"
#include "DEF_LIB_COMMCTRL.h"

// CRegisterUserDlg 대화 상자입니다.

class  CRegisterUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterUserDlg)

public:
	//CRegisterUserDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CRegisterUserDlg(char* szFilePath="", CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterUserDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOGIN_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRegisterOk();
	afx_msg void OnBnClickedButtonRegisterExit();
	afx_msg BOOL OnInitDialog();

	CComboBox m_userLevelCombo;
	CEdit m_userID;
	CEdit m_userPW;

	//ST_LOGIN_USER m_StLoginUser[1024];
	//void GetUser();

	CString m_strCreateID;
	bool m_bCrateSuccess;
private:
	char m_szFilePath[512];
public:
	afx_msg void OnBnClickedBtnRegisterUserDisplay();
};
