/*========================================================================================
	FILE	 : LBaseSock.h
	DESCRIPT : Definition file of the base WinSock class
	COPYRIGHT: 2001 ~ 2005, IT Innovation Co., Ltd.
========================================================================================*/
#if !defined(AFX_MYSOCKET_H__74E4ED83_4EBC_4263_B317_955699AA42F6__INCLUDED_)
#define AFX_MYSOCKET_H__74E4ED83_4EBC_4263_B317_955699AA42F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>


//========================================================================================
//	ERROR CODES
//========================================================================================
#ifndef RV_SUCCESS
	#define RV_SUCCESS		0				//	Success!
#endif	//	RV_SUCCESS
#define EWS_NO_ERR			RV_SUCCESS		//	No error
#define EWS_				-21000			//	Base error


//========================================================================================
//	MACROS
//========================================================================================
//	Length or size of the variables
#ifndef LEN_COMMON
	#define LEN_COMMON			64					//	Common string length
	#define LEN_COMMON2			(LEN_COMMON * 2)	//	Common string length * 2
	#define LEN_GENERAL			(LEN_COMMON * 4)	//	General string length
	#define LEN_GENERAL2		(LEN_GENERAL * 2)	//	General string length * 2
	#define LEN_GENERAL4		(LEN_GENERAL * 4)	//	General string length * 4
	#define LEN_GENERAL8		(LEN_GENERAL * 8)	//	General string length * 8
#endif		//	LEN_COMMON

//	Etc
#define WM_SOCK_SOCKET				(WM_USER + 100)	//	Socket event message
#define IDT_SOCK_CONNECT			100		//	Timer ID for connection
#define TIMEOUT_SOCK_CONNECT		3000	//	Timeout for connection


//========================================================================================
//	CLASS	: LBaseSock
//	DESCRIPT: Base WinSock class
//========================================================================================
class AFX_EXT_CLASS LBaseSock 
{
private:
	int		m_nLastErr;	//	Last error code
	HWND	m_hWnd;		//	Window handle for handling the socket events
	bool	m_fListenSocket;
	CString m_sSockClassName;
public:
	char	m_szClsId[LEN_COMMON];	//	Class ID

// Construction
public:
	/*evnet select mode supported*/
	enum eSocketEventHandling{eWSAAsyncSelect,eWSAEventSelect};
	eSocketEventHandling m_SocketHandlingMode;
	WSAEVENT	m_hSockEvent;
	HANDLE		m_thrSockThread;
	bool		m_bKillSockThread;
	LBaseSock(eSocketEventHandling mode = eWSAAsyncSelect);

// Attributes
public:
	HANDLE	m_hSendBlockingEvent;
	SOCKET	m_Sock;	//	Socket descriptor

// Operations
public:
	static int		m_nRefCount;

protected:
	virtual LRESULT WindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	static LRESULT WINAPI WindowProcEx(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI SockEventThread(LPVOID);
// Implementation
public:
	//	Create the socket event handler window.
	int Open( CString sSockClass );
	//	Listen connections for the server socket.
	int Listen( char* szAddr, int nPort, int nMaxConnect = 1 );
	//	Connect to server
	int Connect(char* szAddr, int nPort);
	//	return true is listen mode else send or receive mode
	bool IsListeningSocket() { return m_fListenSocket; }

	static void DEBUG_LOG(char* fmt, ...);
	int GetLastError() { return m_nLastErr; }
	char* GetLastErrorString() { return GetErrorString( m_nLastErr ); }

	int GetSockName(CString& sAddr, unsigned int& nPort);
	static char* LBaseSock::GetErrorString(long nErrorCode);
	
	int Receive(void* pBytes, int nBuffLen);
	int Send(void* pBytes, int nLen);
	//	please call OnAccept, made client comm socket
	int Accept(LBaseSock& lbListenSock);
	//	force disconnect this socket
	void Close();
	//	event for new connect from client
	virtual void OnAccept(int nErrorCode);
	//	event for close the this socket
	virtual void OnClose(int nErrorCode);
	//	event for ready to send
	virtual void OnSend(int nErrorCode);
	//	event for reached the packet
	virtual void OnReceive(int nErrorCode);
	//	event for connect to server (please check nErrorCode)
	virtual void OnConnect(int nErrorCode);

	int GetListenPort();
	virtual ~LBaseSock();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__74E4ED83_4EBC_4263_B317_955699AA42F6__INCLUDED_)
