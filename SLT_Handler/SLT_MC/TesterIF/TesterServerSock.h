#pragma once

class CTesterServerSock : public LBaseSock
{
public:
	CTesterServerSock(void);
	~CTesterServerSock(void);

public:
	CTesterIF* m_pTesterIF;

	CList<CTesterClientSock*> m_listClient;

	BOOL m_bConnect;

public:
	void OnAccept(int nErrorCode);
	void OnClose(int nErrorCode);
	void OnSend(int nErrorCode){}
	void OnReceive(int nErrorCode){}
	void OnConnect(int nErrorCode){}

	BOOL DeleteAllClientSock();
	CTesterClientSock* GetNotConnectSock();

};

