#pragma once
#include "afxwin.h"
#include "resource.h"

// CLogInUserDlg 대화 상자입니다.

class  CLogInUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogInUserDlg)

public:
	//CLogInUserDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogInUserDlg(char* szFilePath="", int szUseLv=0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogInUserDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	BOOL	m_bTempLogOn;
	BOOL	m_bIsShift;
	int		m_nEditFocus;
	char	m_szUserID[128];
	char	m_szUserPW[128];

	

	BOOL	IsMaxCharLength();
	void	AddInputedValue(char *pcInputedValue);
	void	ChangeKey();

public:
	afx_msg void OnBnClickedButtonLoginOk();
	afx_msg void OnBnClickedButtonLoginExit();
	afx_msg void OnBnClickedKey(UINT ID);
	afx_msg	void OnBnClicked_Shift();
	afx_msg	void OnBnClicked_Delete();
	afx_msg	void OnBnClicked_Clear();

	afx_msg void OnEnSetFocusEditID();
	afx_msg void OnEnSetFocusEditPW();

	void	SetTempLogOn(BOOL bTempLogOn) {m_bTempLogOn = bTempLogOn;}
	BOOL	GetTempLogOn() { return m_bTempLogOn; }
	BOOL    FileOpenCheck();
	BOOL    CompareUser(char* szUserId, char* szUserPw);

private:
	char    m_szFilePath[512];
	CEdit   m_userID;
	CEdit   m_userPW;

public:
	int  m_nUserLv;
	char m_szGetUserID[10];
	int		m_nLoginTime; //초 
	afx_msg void OnEnChangeEditLoginUserTime();
};
