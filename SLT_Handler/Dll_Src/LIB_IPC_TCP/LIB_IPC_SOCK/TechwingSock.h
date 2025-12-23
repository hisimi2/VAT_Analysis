#pragma once

class LBaseSock;
class CIPCSock_Imp;

class CTechwingSock : public LBaseSock
{
public:
	CTechwingSock(LBaseSock::eSocketEventHandling mode = LBaseSock::eWSAAsyncSelect);
	~CTechwingSock(void);

	void OnAccept(int nErrorCode) ;
	void OnClose(int nErrorCode)  ;
	void OnReceive(int nErrorCode);
	void OnConnect(int nErrorCode);
	void OnSend(int nErrorCode);

	int SetServerSock(CIPCSock_Imp* pServerSock);

protected:
	CIPCSock_Imp* m_pServerSock;
};

