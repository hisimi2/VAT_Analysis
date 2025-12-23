#pragma once

#include <vector>
#include "ClientConnection.h"

#ifndef IOCP_DLL
#define IOCP_DLL __declspec(dllimport)
#endif

typedef bool( *LPACCEPT_ROUTINE )( CClientConnection* newClient );
typedef std::vector<CClientConnection*> CLIENT_ARRAY;
class IOCP_DLL CIocpCore
{
	CList<CClientConnection*> m_clients;
	HANDLE m_hLinstenThread;
	std::vector<HANDLE> m_hCompetionThreads;
	/* socket ฐüทร */
	HANDLE m_hCompletionPort;
	SOCKET m_hServSock;
	short m_nPort;
	LPACCEPT_ROUTINE callbackAccept;
public:
	CIocpCore( void );
	~CIocpCore( void );
public:
	int Initialize( int _nThreads = 0 );
	int StartServerComm( short _nPort, LPACCEPT_ROUTINE _callbackAccept );
	CClientConnection* StartClientComm( CString _sIp, short _nPort );
	void Kill( CClientConnection* _client );
	int StopComm();
	DWORD ListenThread();
	DWORD CompletionThread();
	int GetClients( CLIENT_ARRAY& _clients );
};



