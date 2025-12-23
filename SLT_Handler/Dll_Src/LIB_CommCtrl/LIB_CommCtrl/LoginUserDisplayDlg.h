#pragma once


// CLoginUserDisplayDlg 대화 상자입니다.
#include "resource.h"

class CLoginUserDisplayDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginUserDisplayDlg)

public:
	CLoginUserDisplayDlg(char* szFilePath="", CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoginUserDisplayDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOGIN_USER_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	char m_szFilePath[512];
	void ShowUser();
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_UserList;
};
