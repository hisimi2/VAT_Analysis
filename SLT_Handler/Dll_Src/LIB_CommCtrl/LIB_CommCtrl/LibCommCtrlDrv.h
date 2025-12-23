#pragma once

#include "DEF_LIB_COMMCTRL.h"

class CLibCommCtrlDrv
{
public:
	CLibCommCtrlDrv(void);
	~CLibCommCtrlDrv(void);

	void AddShowMsgBox(ST_SMB_INFO& MsgInfo);
	void DelShowMsgBox(ST_SMB_INFO* pItem);
	void ClearShowMsgBox();
	void ShowProgressBar();

	UINT MyMsg3(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
		UINT type, char* p_szYes, char* p_szNo, char* p_szRety, int nImageShow=0, HBITMAP hBitMap=NULL, int FontHeight=0, int FontWidth=0);
	UINT MyMsg2(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
		UINT type, char* p_szYes, char* p_szNo, int nImageShow=0, HBITMAP hBitMap=NULL,int FontHeight=0, int FontWidth=0);

private:
	CList<ST_SMB_INFO*, ST_SMB_INFO*>  m_listSMB;

};

