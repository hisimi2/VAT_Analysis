/*========================================================================================
	FILE	 : LBaseSock.cpp
	DESCRIPT : Implementation file of the base WinSock class
	COPYRIGHT: 2001 ~ 2005, IT Innovation Co., Ltd.
========================================================================================*/
#include "StdAfx.h"
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "LBaseSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//========================================================================================
//	GLOBALS/STATICS
//========================================================================================
int	LBaseSock::m_nRefCount = 0;


/*========================================================================================
	FUNCTION: LBaseSock::LBaseSock()
	DESCRIPT: Constructor
	RETURN	: None
	ARGUMENT:
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
LBaseSock::LBaseSock(eSocketEventHandling mode /* = eWSAAsyncSelect */)
{
	WSAData		wsaData;	//	WinSock data

	memcpy( m_szClsId, "CMYSOCK", 7 );

	if( LBaseSock::m_nRefCount == 0 ) {
		if( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) ) {
			// WSA Startup Error
			return;
		}
	}
	LBaseSock::m_nRefCount++;

	m_hSendBlockingEvent = NULL;
	m_Sock = INVALID_SOCKET;
	m_sSockClassName = "";
	m_nLastErr = EWS_NO_ERR;
	m_fListenSocket = false;
	
	m_hSockEvent = NULL;
	m_SocketHandlingMode = mode;
	m_thrSockThread = NULL;
	m_bKillSockThread = false;
	m_hWnd = NULL;
}


/*========================================================================================
	FUNCTION: LBaseSock::~LBaseSock()
	DESCRIPT: Destructor
	RETURN	: None
	ARGUMENT: None
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
LBaseSock::~LBaseSock()
{
	Close();
	
	LBaseSock::m_nRefCount--;
	if(LBaseSock::m_nRefCount <= 0)
		WSACleanup();
}


/*========================================================================================
	FUNCTION: LBaseSock::Open()
	DESCRIPT: Create the socket event handler window.
	RETURN	:
		EWS_NO_ERR: Success!
		Others	  : System error code
	ARGUMENT:
		sSockClass: (i)Socket class name
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::Open( CString sSockClass )
{
	m_sSockClassName = sSockClass;
	if(m_SocketHandlingMode == eWSAAsyncSelect)
	{
	WNDCLASS	WndClass;	//	Window class structure

	WndClass.style = 0;
	WndClass.lpfnWndProc = LBaseSock::WindowProcEx;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = sizeof( LONG );
	WndClass.hInstance = NULL;
	WndClass.hIcon = NULL;
	WndClass.hCursor = NULL;
	WndClass.hbrBackground = NULL;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = m_sSockClassName;

	RegisterClass( &WndClass );

		m_hWnd = ::CreateWindow( m_sSockClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL );
	if( m_hWnd == NULL ) {
		DEBUG_LOG( "LBaseSock.OpenFailed: create handler failed\n" );
		return (m_nLastErr = GetLastError());
	}

	::SetWindowLong( m_hWnd, 0, (long)this );
	}
	else
	{
		DWORD dwThrID = 0;
		m_bKillSockThread = false;
		m_hSendBlockingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hSockEvent = WSACreateEvent();
		m_thrSockThread = CreateThread(NULL, 0, SockEventThread, this, NULL, &dwThrID);
		if(m_thrSockThread == NULL)
			return (-1);
		ResumeThread(m_thrSockThread);

		AfxTrace("LBaseSock::SockEventThread Start (%d)\n", dwThrID);
	}
	return EWS_NO_ERR;
}


/*========================================================================================
	FUNCTION: LBaseSock::GetListenPort()
	DESCRIPT: Get the listening port #.
	RETURN	:
	ARGUMENT: None
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::GetListenPort()
{
	CString		sAddr;	//	IP address
	unsigned int	nPort;	//	Port #

	if( !GetSockName( sAddr, nPort ) ) { return nPort; }
	else {
		//TRACE( "Socket get peer name failed: %d - %s\n", m_nLastErr, 
		//	   GetLastErrorString() );
		return m_nLastErr;
	}
}


/*========================================================================================
	FUNCTION: LBaseSock::Listen()
	DESCRIPT: Listen connections for the server socket.
	RETURN	:
		EWS_NO_ERR: Success!
		Others	  : System error code
	ARGUMENT:
		szAddr	   : (i)IP address
		nPort	   : (i)Port #
		nMaxConnect: (i)Max. # of connections accepted (Default = 1)
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::Listen( char *szAddr, int nPort, int nMaxConnect )
{
	SOCKADDR_IN		SockAddr;	//	Socket address structure
	BOOL	bOpt = true;
	DWORD	dwResult;	//	Function result
	int		nResult;	//	Function result

	//	Get the socket descriptor.
	if( (m_Sock = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET ) {
		return (m_nLastErr = WSAGetLastError());
	}

	//	Set the socket option.
	setsockopt( m_Sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&bOpt, sizeof( bOpt ) );

	//	Bind socket.
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons( (u_short)nPort );
	if( szAddr == NULL ) { SockAddr.sin_addr.s_addr = htonl( INADDR_ANY ); }
	else {
		dwResult = inet_addr( szAddr );
		if( dwResult == INADDR_NONE ) {
			m_nLastErr = WSAGetLastError();
			closesocket(m_Sock);
			m_Sock = INVALID_SOCKET;
			return m_nLastErr; 
		}
		SockAddr.sin_addr.s_addr = dwResult;
	}
	if( bind( m_Sock, (LPSOCKADDR)&SockAddr, sizeof( SOCKADDR ) ) == SOCKET_ERROR ) {
		m_nLastErr = WSAGetLastError();
		closesocket(m_Sock);
		m_Sock = INVALID_SOCKET;
		return m_nLastErr;
	}

	//	Select the socket event.
	if(m_SocketHandlingMode == eWSAAsyncSelect)
	{
	nResult = WSAAsyncSelect( m_Sock, m_hWnd, WM_SOCK_SOCKET,
							  FD_ACCEPT | FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE );
	}
	else
	{
		nResult = WSAEventSelect(m_Sock, m_hSockEvent, 
			FD_ACCEPT | FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	}
	if( nResult == SOCKET_ERROR ) { return (m_nLastErr = WSAGetLastError()); }

	//	Listen connections.
	if( listen( m_Sock, nMaxConnect ) == SOCKET_ERROR ) {
		m_nLastErr = WSAGetLastError();
		closesocket( m_Sock );
		m_Sock = INVALID_SOCKET;
		return m_nLastErr;
	}
	m_fListenSocket = true;

	return EWS_NO_ERR;
}


/*========================================================================================
	FUNCTION: LBaseSock::Connect()
	DESCRIPT: Connect to the server socket.
	RETURN	:
		EWS_NO_ERR: Success!
		Others	  : System error code
	ARGUMENT:
		szAddr: (i)IP address
		nPort : (i)Port #
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::Connect( char *szAddr, int nPort )
{
	SOCKADDR_IN		SockAddr;	//	Socket address structure
	DWORD	dwResult;	//	Function result
	int		nResult;	//	Function result

	//	Get the socket descriptor.
	if( (m_Sock = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET ) {
		return (m_nLastErr = WSAGetLastError());
	}

	BOOL	bOpt = true;
	setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&bOpt, sizeof(bOpt));

	int nBufSize = 1024 * 120;
	int nBufSize_Size = sizeof(nBufSize);
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char*)&nBufSize, nBufSize_Size);

	if(m_SocketHandlingMode == eWSAAsyncSelect)
	{
	//	Select the socket event.
	nResult = WSAAsyncSelect( m_Sock, m_hWnd, WM_SOCK_SOCKET,
							  FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE );
	if( nResult == SOCKET_ERROR ) { return (m_nLastErr = WSAGetLastError()); }
	}
	else
	{
		nResult = WSAEventSelect(m_Sock, m_hSockEvent, 
			FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
		if( nResult == SOCKET_ERROR ) { return (m_nLastErr = WSAGetLastError()); }
	}

	//	Connect to the server socket.
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons( nPort );
	if( szAddr == NULL ) { SockAddr.sin_addr.s_addr = htonl( INADDR_ANY ); }
	else {
		dwResult = inet_addr( szAddr );
		if( dwResult == INADDR_NONE ) { return (m_nLastErr = WSAGetLastError()); }
		SockAddr.sin_addr.s_addr = dwResult;
	}
	nResult = connect( m_Sock, (sockaddr *)&SockAddr, sizeof( sockaddr_in ) );
	if( nResult == SOCKET_ERROR ) {
		if( (m_nLastErr = WSAGetLastError()) == WSAEWOULDBLOCK ) {
			::SetTimer( m_hWnd, IDT_SOCK_CONNECT, TIMEOUT_SOCK_CONNECT, NULL );
		}
		else { return m_nLastErr; }
	}
	m_fListenSocket = false;

	return EWS_NO_ERR;
}


/*========================================================================================
	FUNCTION: LBaseSock::WindowProc()
	DESCRIPT: Event handler window procedure.
	RETURN	:
	ARGUMENT:
		uMsg  : (i)Message ID
		wParam: (i)Message parameter 1
		lParam: (i)Message parameter 2
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
LRESULT LBaseSock::WindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	int		nErr;	//	Socket error code
	
	if( uMsg == WM_SOCK_SOCKET ) {
		nErr =  WSAGETASYNCERROR( lParam );
		switch( WSAGETSELECTEVENT( lParam ) ) {
			case FD_ACCEPT:
				OnAccept( nErr );
				break;
			case FD_CONNECT: 
				::KillTimer( m_hWnd, IDT_SOCK_CONNECT );
				OnConnect( nErr ); 
				break;
			case FD_CLOSE:
				OnClose( nErr );
				break;
			case FD_READ:
				OnReceive( nErr );
				break;
			case FD_WRITE:
				OnSend( nErr );
				break;
		}
		return 0L;
	}
	else if( uMsg == WM_TIMER ) {
		if( wParam == IDT_SOCK_CONNECT ) {
			if( m_Sock != INVALID_SOCKET ) {
				closesocket( m_Sock );
				m_Sock = INVALID_SOCKET;
			}
			m_nLastErr = WSAETIMEDOUT;
			OnConnect( WSAETIMEDOUT );
			return 0L;
		}
		
	}

	return ::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
}


void LBaseSock::OnConnect(int nErrorCode)
{

}

void LBaseSock::OnReceive(int nErrorCode)
{

}

void LBaseSock::OnSend(int nErrorCode)
{
	if(m_hSendBlockingEvent)
		SetEvent(m_hSendBlockingEvent);
}

void LBaseSock::OnClose(int nErrorCode)
{

}

void LBaseSock::OnAccept(int nErrorCode)
{

}


/*========================================================================================
	FUNCTION: LBaseSock::Accept()
	DESCRIPT: Accept the socket connection.
	RETURN	:
		EWS_NO_ERR: Success!
		Others	  : System error code
	ARGUMENT:
		Sock: (i)Socket descriptor
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::Accept(LBaseSock& lbListenSock)
{
	SOCKADDR_IN		SockAddr;	//	Socket information structure
	int		nResult;	//	Function result
	int		nLen;	//	Length of the block

	//	Accept the socket connection.
	nLen = sizeof( SOCKADDR );
	SOCKET client_socket = accept( lbListenSock.m_Sock, (LPSOCKADDR)&SockAddr, &nLen );
	if( client_socket == INVALID_SOCKET ) { return (m_nLastErr = WSAGetLastError()); }

	BOOL	bOpt = true;
	setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&bOpt, sizeof(bOpt));
	int nBufSize = 1024 * 1000;
	int nBufSize_Size = sizeof(nBufSize);
	setsockopt(client_socket, SOL_SOCKET, SO_SNDBUF, (const char*)&nBufSize, nBufSize_Size);

	//	Select the socket event.
	m_Sock = client_socket;
	if(m_SocketHandlingMode == eWSAAsyncSelect)
	{
	nResult = WSAAsyncSelect( m_Sock, m_hWnd, WM_SOCK_SOCKET,
							  FD_CLOSE | FD_READ | FD_WRITE );
	}
	else
	{
		nResult = WSAEventSelect( m_Sock, m_hSockEvent, 
			FD_CLOSE | FD_READ | FD_WRITE );
	}
	if( nResult == SOCKET_ERROR ) { return (m_nLastErr = WSAGetLastError()); }

	return EWS_NO_ERR;
}


/*========================================================================================
	FUNCTION: LBaseSock::Close()
	DESCRIPT: Close the socket and the event handler window.
	RETURN	: None
	ARGUMENT: None
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
void LBaseSock::Close()
{
	if( m_Sock != INVALID_SOCKET ) {
		shutdown( m_Sock, 2 );
		closesocket( m_Sock );
		m_Sock = INVALID_SOCKET;
	}

	if(m_SocketHandlingMode == eWSAAsyncSelect)
	{
	if( m_hWnd ) {
		::DestroyWindow( m_hWnd );
		::UnregisterClass( m_sSockClassName, NULL );
		m_hWnd = NULL;
	}
	}
	else
	{
		m_bKillSockThread = true;
		if(m_hSockEvent)
		{
			WSASetEvent(m_hSockEvent); /*event 발생 시켜 스레드를 깨운다*/
			Sleep(100);
		}
		if(m_hSendBlockingEvent)
			CloseHandle(m_hSendBlockingEvent);
		WSACloseEvent(m_hSockEvent);
		m_hSockEvent = NULL;
		m_hSendBlockingEvent = NULL;
		m_thrSockThread = NULL;
		m_fListenSocket = false;
	}
}


/*========================================================================================
	FUNCTION: LBaseSock::Send()
	DESCRIPT: Send data.
	RETURN	: Bytes to be sent
	ARGUMENT:
		pBytes: (i)Data buffer for sending
		nLen  : (i)Length of the data in bytes
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::Send( void *pBytes, int nLen )
{
	int		nSentBytes=0;	//	Bytes to be sent

	//	Send data.
	if( (nSentBytes = send( m_Sock, (char *)pBytes, nLen, 0 )) == SOCKET_ERROR ) {
		m_nLastErr = WSAGetLastError();
		//DEBUG_LOG( "LBaseSock.Send req=%d, sent=%d (err=%d)\n", nLen, nSentBytes,
		//		   m_nLastErr );
	}
	else 
	{ 
		//DEBUG_LOG( "LBaseSock.Send req=%d, sent=%d\n", nLen, nSentBytes );
	}

	return nSentBytes;
}


/*========================================================================================
	FUNCTION: LBaseSock::Receive()
	DESCRIPT: Receive data.
	RETURN	: Bytes to be received
	ARGUMENT:
		pBytes: (i)Data buffer for receiving
		nLen  : (i)Length of the buffer in bytes
	UPDATE	: 2004/07/01, In-hyeok Paek; First work!
========================================================================================*/
int LBaseSock::Receive( void *pBytes, int nLen )
{
	int		nRecvBytes;	//	Byte to be received

	//	Receive data.
	if( (nRecvBytes= recv( m_Sock, (char *)pBytes, nLen, 0 )) == SOCKET_ERROR ) {
		m_nLastErr = WSAGetLastError();
		DEBUG_LOG( "LBaseSock.Recv read=%d (err=%d)\n", nRecvBytes, m_nLastErr );
	}
	else { DEBUG_LOG( "LBaseSock.Recv read=%d\n", nRecvBytes ); }

	return nRecvBytes;
}

LRESULT WINAPI LBaseSock::WindowProcEx(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (hWnd) {
		LBaseSock* pSock = (LBaseSock*)GetWindowLong(hWnd, 0);

		LRESULT ret = 0;

		if (pSock!=NULL && memcmp(pSock->m_szClsId, "CMYSOCKET", 7)==0) {
			try {
				ret = pSock->WindowProc(message, wParam, lParam);
			} catch(...) {
				DEBUG_LOG("LBaseSock.WinProcEx: Exception: msg=%d, w=%d, l=%d\n", message, wParam, lParam);
			}
			return ret;
		} 
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

int LBaseSock::GetSockName( CString &sAddr, unsigned int &nPort )
{
	SOCKADDR_IN sa;
	int nlen = sizeof(sa);

	int ret=getsockname(m_Sock, (SOCKADDR*)&sa, &nlen);
	if (!ret) {
		sAddr = CString( inet_ntoa(sa.sin_addr) );
		nPort = ntohs(sa.sin_port);
		return 0;
	}
	m_nLastErr = WSAGetLastError();
	return m_nLastErr;
}

char* LBaseSock::GetErrorString(long nErrorCode)
{
	static char msgBuff[1024];

	memset(msgBuff, 0, sizeof(msgBuff));

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		msgBuff,
		sizeof(msgBuff),
		NULL
	);


	for(char* p = msgBuff; *p; p++) {
		if (*p == '\n' || *p == '\r') {
			*p = 0;
		}
	}

	return msgBuff;
}

void LBaseSock::DEBUG_LOG(char* fmt, ...)
{
#if 0
	try {
		va_list ap;
		SYSTEMTIME mtime;
		FILE* fp;
		fopen_s( &fp, "_err.log", "a");

		GetLocalTime(&mtime);

		va_start(ap, fmt);

		if (fp) {
			fprintf(fp, "%04d.%02d.%02d %02d:%02d:%02d.%03d [th=%x]",
				mtime.wYear, mtime.wMonth, mtime.wDay,
				mtime.wHour, mtime.wMinute, mtime.wSecond,
				mtime.wMilliseconds,
				GetCurrentThreadId());
			vfprintf(fp, fmt, ap);
			fflush(fp); 
			fclose(fp);
		}

		va_end(ap);
	} catch(...) {}
#endif
}







/*WSAEventSelect supported*/
DWORD LBaseSock::SockEventThread(LPVOID pParam)
{
	LBaseSock* pSelf = (LBaseSock*)pParam;

	WSANETWORKEVENTS evtCode;

	while(pSelf->m_bKillSockThread == false)
	{
		DWORD dwEventIdx = WSAWaitForMultipleEvents(1, &pSelf->m_hSockEvent, FALSE, 100, FALSE);
		
		if(	dwEventIdx == WSA_WAIT_TIMEOUT || 
			dwEventIdx == WSA_WAIT_IO_COMPLETION){
			continue;
		}
		else if(dwEventIdx == WSA_WAIT_EVENT_0)
		{
			if(pSelf->m_Sock == INVALID_SOCKET)
			{
				break;
			}
			else
			{
				WSAResetEvent(pSelf->m_hSockEvent);

				WSAEnumNetworkEvents(pSelf->m_Sock, pSelf->m_hSockEvent, &evtCode);
				if(evtCode.lNetworkEvents & FD_ACCEPT)
					pSelf->OnAccept(evtCode.iErrorCode[FD_ACCEPT_BIT]);
				if(evtCode.lNetworkEvents & FD_READ)
					pSelf->OnReceive(evtCode.iErrorCode[FD_READ_BIT]);
				if(evtCode.lNetworkEvents & FD_WRITE)
					pSelf->OnSend(evtCode.iErrorCode[FD_WRITE_BIT]);
				if(evtCode.lNetworkEvents & FD_CLOSE)
				{
					pSelf->OnClose(evtCode.iErrorCode[FD_CLOSE_BIT]);
					break;
				}
			}
		}
		else
		{
			pSelf->OnClose(dwEventIdx);
			break; //thread abort
		}
	}

	return 0;
}