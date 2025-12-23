#pragma once

//#include "DEF_COMMON.h"
#include "DEF_LIB_IPC_SOCK.h"
//#include "IPCSock.h"
#include "Logger.h"

class AFX_EXT_CLASS CIPCSock : public CWnd
{
public:
	CIPCSock(void);
	~CIPCSock(void);

	int  Initialize(CWnd* pParentWnd, int nSocketType, char* szIp, int nPort, ST_MESSAGE_MAP* msg_map, char* szLogName );
	void Finalize();
	
	int  SocketConnect();
	int  SocketConnect(int nSocketType, char* szIp, int nPort);
	int  GetSocketConnect(int& nSocketType, CString& sIp, int& nPort);
	int  SocketClose();

	BOOL IsConnected();
	BOOL IsPing(char* szIp);

	int  Send(CPacket* pPacket, BOOL bTmReplyCheck);
	void SetTmOutLimit(int nMilliSec);

	CWnd* GetParentWnd() { return m_pParentWnd; };
	void ChangeSetting(char* szIp, int nPort);

	virtual afx_msg LRESULT OnTWIPCEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()	

protected:
	CWnd*			  m_pParentWnd;
	ST_MESSAGE_MAP	  m_MessageMap[1000];
	int               m_nMapCount;
	int               m_nSocketType;
	char              m_szIp[128];
	int               m_nPort;
	void*			  m_pIPCSock_Imp;
	char              m_szLogName[512];

private:
	CLogger m_Logger;
	void SendLogMessage(const char* fmt, ...);
};
