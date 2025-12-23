#pragma once

class CTesterIF;
//class CTesterServerSock;

class CTesterClientSock : public LBaseSock
{
public:
	CTesterClientSock(void);
	~CTesterClientSock(void);

public:
	CTesterIF*			m_pTesterIF;
	//CTesterServerSock*	m_pServerSock;

	//int m_nIdx;
	BOOL m_bConnect;

	RecvData m_RecvData;
	SendData m_SendData;

	int m_nSplitCount;
	char m_szSplit[10000];

public:
	void OnAccept(int nErrorCode) {}
	void OnClose(int nErrorCode);
	void OnSend(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnConnect(int nErrorCode) {}

	void ResetData();

	void ParserMsgID(BYTE* data, RecvData& recvData);
	void ParserMsgData(BYTE* data, RecvData& recvData, int nRevLength);

};
