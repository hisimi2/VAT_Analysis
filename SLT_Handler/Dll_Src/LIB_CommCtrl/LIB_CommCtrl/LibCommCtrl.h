#pragma once

#include "DEF_LIB_COMMCTRL.h"

class AFX_EXT_CLASS CLibCommCtrl
{
public:
	CLibCommCtrl(void);
	~CLibCommCtrl(void);

	int  Initialize(CWnd* pParentWnd);
	void Close();

	int     GetNumberBox2(CWnd* cWnd, char* szRetrunNum ="", int maxchar=255, char* getnum="", char* title="input data");
	int     GetNumberBox(CWnd* cWnd, char* szRetrunNum  ="", int maxchar=255, char* getnum="", char* title="input data",char* cMax="100", char* cMin="0");
	BOOL    GetValueBox(char* szVal, int maxchar, char* getval, char* title, BOOL password);
	int     ShowLogInBox2(/*char* szFilePath ="",   */int nUserLv=0, int* nReturnLv=0, char* szGetUserID ="" , int* nLoginTime = 0); // 로그인한 User ID를 받아온다
	int     ShowLogInBox(/*char* szFilePath ="",   */int nUserLv=0, int* nReturnLv=0);
	void    ShowRegisterUser(/*char* szFilePath*/);

	void    ShowMessageBox(ST_SMB_INFO& MsgInfo);
	CString GetSelectDate();


	void ProgressBarCreate( bool _cancel_visible = false, void( *_cancel_callback )( int ) = nullptr, int _axis_no = -1 );
	void    ProgressBarDestroy();
	void    ProgressBarSetPos(int nPos);
	void    ProgressBarSetStatus(CString strStatus);	
	UINT    MyMsg3(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
		              UINT type=M_OK, char* p_szYes="Abort", char* p_szNo="Skip", char* Third="Retry", int nImageShow=0, HBITMAP hBitmap=NULL,int FontHeight=0, int FontWidth=0);
	UINT    MyMsg2(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
		UINT type=M_OK, char* p_szYes="Abort", char* p_szNo="Skip", int nImageShow=0,  HBITMAP hBitmap=NULL, int FontHeight=0, int FontWidth=0);

	void    ShowUserConfig(ST_USER_CONFIG_ARRAY StUserConfigSet);
	ST_USER_CONFIG_ARRAY GetUserConfig();

	CString m_strCreateID;

	void ProgressBarNewCreate();
	void ProgressBarShow();
	void ProgressBarNewDestroy();
	void ProgressBarNewSetPos(int nPos);
	void ProgressBarNewSetStatus(CString strStatus);
private:
	ST_USER_CONFIG_ARRAY m_UserConfigSet;
};

