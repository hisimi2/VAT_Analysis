#pragma once

#include <vector>
struct partial_buffer_item;

#ifndef IOCP_DLL
#define IOCP_DLL __declspec(dllimport)
#endif

#define BUFSIZE 65535
#define PGET_BUFSIZE (BUFSIZE*3)

struct IOCP_DLL PER_IO_DATA
{
	enum RW_FLAG { read, write, nr };
	OVERLAPPED overlapped;
	RW_FLAG rw_flag;
	WSABUF wsaBuf;

	PER_IO_DATA();
	~PER_IO_DATA();
	PER_IO_DATA* alloc( RW_FLAG _rw, int _new_size );
	PER_IO_DATA* alloc( RW_FLAG _rw, void* _data, int _new_size );
	void free();
};

class CClientConnection;
typedef void( *LPCOMM_EVENT_ROUTINE )( CClientConnection* _client );

class IOCP_DLL CClientConnection
{
	CWnd* m_pParent;
	SOCKET m_hClntSock;
	SOCKADDR_IN m_clntAddr;
	WSAEVENT m_hSendComptEvent;
	std::vector<partial_buffer_item*> m_PartialBuffer;
	char* m_MergedBuff;
	int m_MergedBuffSize;
	CRITICAL_SECTION csBuffLock;
	//CALLBACK
	LPCOMM_EVENT_ROUTINE m_callbackRecv;
	LPCOMM_EVENT_ROUTINE m_callbackSend;
	LPCOMM_EVENT_ROUTINE m_callbackClosed;
public:
	PER_IO_DATA m_rcvIO;
	CClientConnection( SOCKET _hClntSock, SOCKADDR_IN& _clntAddr );
	~CClientConnection();
	CWnd* GetParentPtr();
	CString GetClientAddr();
	SOCKET GetSocket();
	void RegisterCallback(CWnd* _parent, LPCOMM_EVENT_ROUTINE _callbackRecv, LPCOMM_EVENT_ROUTINE _callbackSend, LPCOMM_EVENT_ROUTINE _callbackClosed );

	int Send( const char* buff, int size );
	int GetRcvSize();
	int GetRcvData( const char** buff );
	void ReleaseRcvData( int _consum_bytes );

	void _push_rcv_data( char* data, int len );
	void _raise_send_complete_event();
	void _raise_recv_complete_event();
	void _raise_closed_event();
	BOOL bClient = FALSE;
private:
	void _lock();
	void _unlock();
};

struct partial_buffer_item
{
	partial_buffer_item();
	~partial_buffer_item();

	int bytes;
	char* buffer;
	char* alloc( int bytes );
	void free();
};
