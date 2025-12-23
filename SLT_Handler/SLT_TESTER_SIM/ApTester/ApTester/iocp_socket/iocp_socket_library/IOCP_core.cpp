#include "StdAfx.h"
#include "IOCP_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>

CIocpCore::CIocpCore( void )
{
	m_hLinstenThread = INVALID_HANDLE_VALUE;
	m_hCompletionPort = INVALID_HANDLE_VALUE;
	m_hServSock = INVALID_SOCKET;
	m_nPort = 0;
	callbackAccept = nullptr;
}


CIocpCore::~CIocpCore( void )
{
	StopComm();
}

DWORD __stdcall ListenThread( LPVOID pCIocpCore );
DWORD __stdcall CompletionThread( LPVOID pComPort );
void ErrorHandling( LPCTSTR message );

#pragma comment(lib, "ws2_32.lib")

int CIocpCore::Initialize( int _nThreads /* = 0 */ )
{
	if (m_hCompletionPort != INVALID_HANDLE_VALUE)
		return 0;
		
	WSADATA wsaData;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
		ErrorHandling( _T( "WSAStartup() error!" ) );

	m_hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	m_hCompetionThreads.clear();
	SYSTEM_INFO SystemInfo;
	if( _nThreads == 0 )
		GetSystemInfo( &SystemInfo );
	else
		SystemInfo.dwNumberOfProcessors = _nThreads;

	for( DWORD i = 0; i < SystemInfo.dwNumberOfProcessors; ++i )
	{
		HANDLE thread_hanlde = NULL;
		thread_hanlde = ::CreateThread( NULL, 0, ::CompletionThread, ( LPVOID )this, 0, NULL );
		m_hCompetionThreads.push_back( thread_hanlde );
		::ResumeThread( thread_hanlde );
	}
	return 0;
}

int CIocpCore::StartServerComm( short _nPort, LPACCEPT_ROUTINE _callbackAccept )
{
	m_nPort = _nPort;

	this->callbackAccept = _callbackAccept;
	m_hLinstenThread = CreateThread( NULL, 0, ::ListenThread, ( LPVOID )this, 0, NULL );
	::ResumeThread( m_hLinstenThread );
	return 0;
}

CClientConnection * CIocpCore::StartClientComm( CString _sIp, short _nPort )
{
	CStringA mbsStr;
	m_nPort = _nPort;
	SOCKET hClntSock = WSASocket( PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if( hClntSock == INVALID_SOCKET )
		ErrorHandling( _T( "socket() error" ) );

	SOCKADDR_IN recvAddr;
	memset( &recvAddr, 0, sizeof( recvAddr ) );
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr( CStringA( _sIp ) );
	recvAddr.sin_port = htons( _nPort );

	if( connect( hClntSock, ( SOCKADDR* )&recvAddr, sizeof( recvAddr ) ) == SOCKET_ERROR )
	{
		ErrorHandling( _T( "connect() error!" ) );
		return nullptr;
	}

	CClientConnection* pClient = new CClientConnection( hClntSock, recvAddr );
	CString key = pClient->GetClientAddr();
	m_clients[ key ] = pClient;
	key += _T( " connected." );
	ErrorHandling( key );
	CreateIoCompletionPort( ( HANDLE )hClntSock, m_hCompletionPort, ( DWORD )pClient, 0 );

	int RecvBytes;
	int Flags = 0;

	pClient->m_rcvIO.alloc( PER_IO_DATA::read, BUFSIZE );
	WSARecv( pClient->GetSocket(), &pClient->m_rcvIO.wsaBuf, 1, ( LPDWORD )&RecvBytes, ( LPDWORD )&Flags, ( LPOVERLAPPED )&pClient->m_rcvIO, NULL );
	return pClient;
}

int CIocpCore::StopComm()
{
	if( m_hLinstenThread != INVALID_HANDLE_VALUE )
	{
		::TerminateThread( m_hLinstenThread, 0xdead );
		WaitForSingleObject( m_hLinstenThread, INFINITE );
		m_hLinstenThread = INVALID_HANDLE_VALUE;
	}

	if( m_hServSock != INVALID_SOCKET )
	{
		closesocket( m_hServSock );
		m_hServSock = INVALID_SOCKET;
	}

	for( auto i = m_hCompetionThreads.begin(); i != m_hCompetionThreads.end(); i++ )
	{
		::TerminateThread( *i, 0xdead );
		WaitForSingleObject( *i, INFINITE );
	}
	m_hCompetionThreads.clear();

	for( auto p = m_clients.GetStartPosition(); p;)
	{
		CString key;  void* ptr;
		m_clients.GetNextAssoc( p, key, ptr );
		CClientConnection* pClient = ( CClientConnection* )ptr;

		closesocket( pClient->GetSocket() );
		delete pClient;

		key += _T( " kill." );
		ErrorHandling( key );
	}
	m_clients.RemoveAll();

	if( m_hCompletionPort != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hCompletionPort );
		m_hCompletionPort = INVALID_HANDLE_VALUE;
	}

	WSACleanup();

	return 0;
}

DWORD CIocpCore::ListenThread()
{
	m_hServSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	//servAddr.sin_port = htons(atoi("2738"));
	servAddr.sin_port = htons( m_nPort );

	bind( m_hServSock, ( SOCKADDR* )&servAddr, sizeof( servAddr ) );
	listen( m_hServSock, 5 );

	CClientConnection* pClient;

	int RecvBytes;
	int Flags;

	while( TRUE )
	{
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof( clntAddr );

		/*wait for client*/
		SOCKET hClntSock = accept( m_hServSock, ( SOCKADDR* )&clntAddr, &addrLen );
		if( hClntSock == SOCKET_ERROR )
		{
			StopComm();
			return 0;
		}
		pClient = new CClientConnection( hClntSock, clntAddr );
		CString key = pClient->GetClientAddr();
		m_clients[ key ] = pClient;
		key += _T( " connected." );
		ErrorHandling( key );
		CreateIoCompletionPort( ( HANDLE )hClntSock, m_hCompletionPort, ( DWORD )pClient, 0 );

		if( callbackAccept )
			callbackAccept( pClient );

		Flags = 0;

		pClient->m_rcvIO.alloc( PER_IO_DATA::read, BUFSIZE );
		WSARecv( pClient->GetSocket(), &pClient->m_rcvIO.wsaBuf, 1, ( LPDWORD )&RecvBytes, ( LPDWORD )&Flags, ( LPOVERLAPPED )&pClient->m_rcvIO, NULL );
	}
	return 0;
}

DWORD CIocpCore::CompletionThread()
{
	DWORD BytesTransferred = 0;
	CClientConnection* pClient = nullptr;
	PER_IO_DATA* io;
	DWORD flags = 0;

	while( 1 ) {
		GetQueuedCompletionStatus( m_hCompletionPort, &BytesTransferred, ( LPDWORD )&pClient, ( LPOVERLAPPED* )&io, INFINITE );
		
		if( BytesTransferred == 0 )
		{
			CString key = pClient->GetClientAddr();
			m_clients.RemoveKey( pClient->GetClientAddr() );
			delete pClient;

			key += _T( " disconnected." );
			ErrorHandling( key );
			continue;
		}

		if( io->rw_flag == PER_IO_DATA::write )
		{
			AfxTrace( _T( "%d send - " ),BytesTransferred );
			pClient->_raise_send_complete_event();
			continue;
		}

		pClient->_push_rcv_data( pClient->m_rcvIO.wsaBuf.buf, BytesTransferred );

		flags = 0;
		pClient->m_rcvIO.alloc( PER_IO_DATA::read, BUFSIZE );
		WSARecv( pClient->GetSocket(), &pClient->m_rcvIO.wsaBuf, 1, ( LPDWORD )&BytesTransferred, ( LPDWORD )&flags, ( LPOVERLAPPED )&pClient->m_rcvIO, NULL );
	}

	return 0;
}

int CIocpCore::GetClients( CLIENT_ARRAY& _clients )
{
	_clients.clear();
	for( auto i = m_clients.GetStartPosition(); i; )
	{
		CString key;
		void* some_client;
		m_clients.GetNextAssoc( i, key, some_client );
		_clients.push_back( ( CClientConnection* )some_client );
	}
	return _clients.size();
}

/************************************************************************/
/* socket thread                                                        */
/************************************************************************/
DWORD __stdcall ListenThread( LPVOID _pCIocpCore )
{
	return ( ( CIocpCore* )_pCIocpCore )->ListenThread();
}

DWORD __stdcall CompletionThread( LPVOID _pCIocpCore )
{
	return ( ( CIocpCore* )_pCIocpCore )->CompletionThread();
}

void ErrorHandling( LPCTSTR message )
{
	_fputts( message, stderr );
	_fputtc( _T( '\n' ), stderr );
}
