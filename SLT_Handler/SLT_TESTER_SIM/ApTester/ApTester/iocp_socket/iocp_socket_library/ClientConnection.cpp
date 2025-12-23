#include "StdAfx.h"
#include "ClientConnection.h"


CClientConnection::CClientConnection( SOCKET _hClntSock, SOCKADDR_IN& _clntAddr )
{
	m_pParent = nullptr;
	m_rcvIO.free();
	m_hSendComptEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	m_hClntSock = _hClntSock;
	memcpy( &( this->m_clntAddr ), &_clntAddr, sizeof( _clntAddr ) );
	::InitializeCriticalSection( &csBuffLock );
	m_callbackRecv = nullptr;
	m_callbackSend = nullptr;
	m_callbackClosed = nullptr;
}


CClientConnection::~CClientConnection( void )
{
	GetRcvData( NULL ); //free all buffers

	if( m_hClntSock != INVALID_SOCKET )
		closesocket( m_hClntSock );
	m_hClntSock = INVALID_SOCKET;

	if( m_hSendComptEvent != INVALID_HANDLE_VALUE )
		WSACloseEvent( m_hSendComptEvent );
	m_hSendComptEvent = INVALID_HANDLE_VALUE;

	::DeleteCriticalSection( &csBuffLock );

	_raise_closed_event();
}

CWnd * CClientConnection::GetParentPtr()
{
	return this->m_pParent;
}

CString CClientConnection::GetClientAddr()
{
	CString strAddr;
	strAddr.Format( _T( "%d.%d.%d.%d:%d" ),
					m_clntAddr.sin_addr.S_un.S_un_b.s_b1,
					m_clntAddr.sin_addr.S_un.S_un_b.s_b2,
					m_clntAddr.sin_addr.S_un.S_un_b.s_b3,
					m_clntAddr.sin_addr.S_un.S_un_b.s_b4,
					m_clntAddr.sin_port );
	return strAddr;
}

SOCKET CClientConnection::GetSocket()
{
	return m_hClntSock;
}

void CClientConnection::_push_rcv_data( char* _data, int _len )
{
	partial_buffer_item* pItem = new partial_buffer_item();
	memcpy( pItem->alloc( _len ), _data, _len );
	_lock();
	m_PartialBuffer.push_back( pItem );
	_unlock();

	/*callback suported*/
	_raise_recv_complete_event();
}

int CClientConnection::Send( const char* buff, int size )
{
	int retv = SOCKET_ERROR;
	DWORD send_bytes = 0;
	PER_IO_DATA send_io;
	send_io.alloc( PER_IO_DATA::write, ( void* )buff, size );
	//send_io.overlapped.hEvent = m_hSendComptEvent;
	if( WSASend( m_hClntSock, &( send_io.wsaBuf ), 1, &send_bytes, 0, &send_io.overlapped, nullptr ) == SOCKET_ERROR )
	{
		int Err = WSAGetLastError();
		if( Err != WSA_IO_PENDING )
		{
			_tprintf( _T( "WSASend() error" ) );
			retv = SOCKET_ERROR;
		}
		else
			AfxTrace( _T( "WSA_IO_PENDING\n" ) );
	}
	WaitForSingleObject( m_hSendComptEvent, INFINITE );
	AfxTrace( _T( "%x,%d bytes ok\n" ), send_io.wsaBuf.buf, send_bytes );
	return retv;
}

void CClientConnection::_raise_send_complete_event()
{
	AfxTrace( _T( "_raise_send_complete_event\n" ) );
	if( m_callbackSend )
		m_callbackSend( this );
	SetEvent( m_hSendComptEvent );
}

void CClientConnection::_raise_recv_complete_event()
{
	AfxTrace( _T( "_raise_recv_complete_event\n" ) );
	if( m_callbackRecv )
		m_callbackRecv( this );
}

void CClientConnection::_raise_closed_event()
{
	AfxTrace( _T( "_raise_closed_event\n" ) );
	if( m_callbackClosed )
		m_callbackClosed( this );
}

int CClientConnection::GetRcvSize()
{
	int count = 0;
	_lock();
	for( auto i = m_PartialBuffer.begin(); i != m_PartialBuffer.end(); i++ )
		count += ( *i )->bytes;
	_unlock();
	return count;
}

int CClientConnection::GetRcvData( char** _ppBuff )
{
	int merged_bytes = GetRcvSize();
	_lock();
	if( _ppBuff )
	{
		char* p = ( *_ppBuff ) = new char[ merged_bytes ];
		for( auto i = m_PartialBuffer.begin(); i != m_PartialBuffer.end(); i++ )
		{
			memcpy( p, ( *i )->buffer, ( *i )->bytes );
			p += ( *i )->bytes;
			delete ( *i );
		}
		m_PartialBuffer.clear();
	}
	/*clear*/
	else
	{
		for( auto i = m_PartialBuffer.begin(); i != m_PartialBuffer.end(); i++ )
			delete ( *i );
		m_PartialBuffer.clear();
	}
	_unlock();

	return merged_bytes;
}

void CClientConnection::ReleaseRcvData( char* _buff )
{
	if( _buff )
		delete[] _buff;
}

void CClientConnection::RegisterCallback(
	CWnd* _parent, 
	LPCOMM_EVENT_ROUTINE _callbackRecv,
	LPCOMM_EVENT_ROUTINE _callbackSend, 
	LPCOMM_EVENT_ROUTINE _callbackClosed )
{
	m_pParent = _parent;
	m_callbackRecv = _callbackRecv;
	m_callbackSend = _callbackSend;
	m_callbackClosed = _callbackClosed;
}

void CClientConnection::_lock() { ::EnterCriticalSection( &csBuffLock ); }
void CClientConnection::_unlock() { ::LeaveCriticalSection( &csBuffLock ); }

/************************************************************************/
/*                                                                      */
/************************************************************************/

partial_buffer_item::partial_buffer_item()
{
	bytes = 0;
	buffer = NULL;
}

partial_buffer_item::~partial_buffer_item()
{
	free();
}

char* partial_buffer_item::alloc( int bytes )
{
	this->bytes = bytes;
	this->buffer = new char[ bytes ];
	ZeroMemory( this->buffer, bytes );
	return this->buffer;
}

void partial_buffer_item::free()
{
	if( buffer )
		delete[] buffer;
	buffer = NULL;
	bytes = 0;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
PER_IO_DATA::PER_IO_DATA()
{
	wsaBuf.buf = nullptr;
	free();
}
PER_IO_DATA::~PER_IO_DATA()
{
	free();
}
PER_IO_DATA* PER_IO_DATA::alloc( RW_FLAG _rw, int _new_size )
{
	memset( &( overlapped ), 0, sizeof( OVERLAPPED ) );
	rw_flag = _rw;
	wsaBuf.len = _new_size;
	if( wsaBuf.buf )
		delete[] wsaBuf.buf;

	ZeroMemory(
		wsaBuf.buf = new char[ _new_size ],
		_new_size );
	
	return this;
}

PER_IO_DATA * PER_IO_DATA::alloc( RW_FLAG _rw, void * _data, int _new_size )
{
	memset( &( overlapped ), 0, sizeof( OVERLAPPED ) );
	rw_flag = _rw;
	wsaBuf.len = _new_size;
	if( wsaBuf.buf )
		delete[] wsaBuf.buf;

	memcpy(
		wsaBuf.buf = new char[ _new_size ],
		_data,
		_new_size );

	return this;
}

void PER_IO_DATA::free()
{
	memset( &( overlapped ), 0, sizeof( OVERLAPPED ) );
	rw_flag = read;
	wsaBuf.len = 0;
	if( wsaBuf.buf )
		delete[] wsaBuf.buf;
	wsaBuf.buf = nullptr;
}
