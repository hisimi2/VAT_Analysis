// ConsoleApplication1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ConsoleApplication1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../iocp_socket_library/IOCP_core.h"
#pragma comment(lib, "../iocp_socket_library/Debug/iocp_socket_library.lib")


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;

void OnAccept( CClientConnection* _client );
void OnRecieve( CClientConnection* _client );
void OnClosed( CClientConnection* _client );
void OnRecieveFromServer( CClientConnection * _client );

void server_run();
void client_run();

#define _N _T("\n")
int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle( nullptr );

	if( hModule != nullptr )
	{
		// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
		if( !AfxWinInit( hModule, nullptr, ::GetCommandLine(), 0 ) )
		{
			// TODO: 오류 코드를 필요에 따라 수정합니다.
			wprintf( L"심각한 오류: MFC를 초기화하지 못했습니다.\n" );
			nRetCode = 1;
		}
		else
		{
			CString ch;
			do {
				_tprintf( _T( "0:exit, 1:Server, 2:Client>" ) );
				_tscanf( _T( "%s" ), ch.GetBuffer( 100 ) ); ch.ReleaseBuffer();
				switch( ch[ 0 ] )
				{
				case _T( '1' ):
					server_run();
					continue;
				case _T( '2' ):
					client_run();
					continue;
				case _T( '0' ):
					break;
				default: continue;
				}
				break;
			} while( 1 );
		}
	}
	else
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		wprintf( L"심각한 오류: GetModuleHandle 실패\n" );
		nRetCode = 1;
	}

	return nRetCode;
}



void OnAccept( CClientConnection* _client )
{
	_client->RegisterCallback( OnRecieve, nullptr, OnClosed );
}



void OnRecieve( CClientConnection * _client )
{
	char* buf = NULL;
	int bytes = _client->GetRcvData( &buf );
	CString s1, s2;
	memcpy( s1.GetBuffer( bytes / 2 ), buf, bytes );
	s1.ReleaseBuffer();
	_client->ReleaseRcvData( buf );
	_putts( s1 );

	s2 = _T( "svr:" ) + s1;
	_client->Send( ( char* )s2.GetString(), s2.GetLength() * 2 + 2 );
}


void OnRecieveFromServer( CClientConnection * _client )
{
	char* buf = NULL;
	int bytes = _client->GetRcvData( &buf );
	CString str;
	memcpy( str.GetBuffer( bytes / 2 ), buf, bytes );
	str.ReleaseBuffer();
	_client->ReleaseRcvData( buf );
	_putts( str );
}


void OnClosed( CClientConnection * _client )
{
	_putts( _T( "bye~" ) );
}



void server_run()
{
	CIocpCore core;
	CString text;

	_putts( _N _T( "server start!" )_N );
	core.Initialize();
	core.StartServerComm( 80, ::OnAccept );
	do
	{
		_tprintf( _T( "0:stop server, 1:list up>" _N ) );
		_tscanf( _T( "%s" ), text.GetBuffer( 100 ) ); text.ReleaseBuffer();
		switch( text[ 0 ] )
		{
		case _T( '1' ):
			_putts( _T( "" ) );
			{
				CLIENT_ARRAY list;
				core.GetClients( list );
				for( int i = 0; i < ( int )list.size(); i++ )
				{
					auto c = list[ i ];
					_tprintf( _T( "%d : %s\n" ), i + 1, c->GetClientAddr() );
				}
				if( list.size() <= 0 )
				{
					_putts( _T( "no clients." ) );
					continue;
				}
				_tprintf( _T( "chois>" ) );
				_tscanf( _T( "%s" ), text.GetBuffer( 100 ) ); text.ReleaseBuffer();
				if( text.IsEmpty() )
					continue;

				CClientConnection* c = list[ _ttoi( text ) - 1 ];
				_tprintf( _T( "msg>" ) );
				_tscanf( _T( "%s" ), text.GetBuffer( 10240 ) ); text.ReleaseBuffer();
				c->Send( ( char* )text.GetString(), text.GetLength() * 2 + 2 );
			}
			continue;
		case _T( '0' ):
			break;
		default: continue;
		}
		break;
	} while( 1 );

	core.StopComm();
	return;
}


void client_run()
{
	CString input;
	CIocpCore core;
	CClientConnection* conn;
	_tprintf( _N _T( "connect to server..." ) );
	core.Initialize( );
	conn = core.StartClientComm( _T( "127.0.0.1" ), 80 );
	if( conn == nullptr )
	{
		_putts( _T( "fail" )_N );
		return;
	}
	conn->RegisterCallback( OnRecieveFromServer, nullptr, OnClosed );

	do
	{
		_tprintf( _N _T( "0:disconnect>" ) );
		_tscanf( _T( "%s" ), input.GetBuffer( 100 ) ); input.ReleaseBuffer();
		switch( input[ 0 ] )
		{
		default:
			{
				CString text = input;
				int count;
				_tprintf( _T( "repeat>" ) );
				_tscanf( _T( "%s" ), input.GetBuffer( 10240 ) ); input.ReleaseBuffer();
				count = _ttoi( input );
				for( int i = 0; i < count; i++ )
				{
					CString bf;
					bf.Format( _T( "%d:%s" ), i, text );
					conn->Send( ( char* )bf.GetString(), bf.GetLength() * 2 + 2 );
				}
			}
			continue;
		case _T( '0' ):
			break;
		case _T( '\0' ): continue;
		}
		break;
	} while( 1 );

	core.StopComm();
	return;
}
