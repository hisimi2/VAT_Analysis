#pragma once

#include <afxmt.h>
#include <queue>
#include "IPCSock.h"
#include "Logger.h"

using namespace std;

class CPacket;
class CThreadBase;
class LBaseSock;
class CTechwingSock;

class CIPCSock_Imp : public LBaseSock
{
public:
	CIPCSock_Imp(void);
	~CIPCSock_Imp(void);

	int Initialize(CWnd* pClientWnd, ST_MESSAGE_MAP* msg_map, char* szIp );
	void Finallize();

	int SocketConnect(int nSocketType, char* szIp, int nPort );

	void OnAccept(int nErrorCode) ;
	void OnClose(int nErrorCode)  ;
	void OnReceive(int nErrorCode);
	void OnConnect(int nErrorCode);

	void OnReceive(char* sData, int nRdLength);

	void PopSndPacket();
	void PopRcvPacket();
	void AddReceivePacket(CPacket* pPacket);

	BOOL IsConnected();
	void SetTmOutLimit(int nLimitTmMilSec/*Milli Second*/);
		
	int  SendPacket(CPacket& pPacket);
	int  AddSendPacket(CPacket* pPacket, BOOL bTmReplyCheck);	

	int  PopTmOutPakcetChkTmOut();
	void DeleteTmOutPacket(int a_nMsgId);
	void AddTmOutPacket(CPacket* pPacket, double dTickCount);
protected:
	int	 do_DataParse(char sData[]);

public:
// Send Packet Buffer
	static UINT ThrSndPacketProc(LPVOID lpVoid);
	CWinThread*	         m_pThreadSnd;
	CCriticalSection     m_csSndQ;
	std::deque<CPacket*> m_DqSnd;

// Receive Packet Buffer
	static UINT ThrRcvPacketProc(LPVOID lpVoid);
	CWinThread*	         m_pThreadRcv;
	CCriticalSection     m_csRcvQ;
	std::deque<CPacket*> m_DqRcv;

// Timeout Process	
	static UINT ThrPacketTmOutProc(LPVOID lpVoid);
	CWinThread*	                m_pThreadTimeOut;
	std::deque<CTimeOutPacket*> m_DqTmOut;
	std::deque<CTimeOutPacket*>::iterator DqTmOutIter;
	CCriticalSection            m_csTmOutQ;
	
	FN_CALLBACK_EVENT    m_fnEventCallback;
	ST_MESSAGE_MAP	     m_MessageMap[1000];
					     
	BOOL                 m_bStatusConnect;
	BOOL                 m_bStopRcvPacketProc;
	BOOL                 m_bStopSndPacketProc;
	BOOL                 m_bTimeOutProc;
	BOOL                 m_bTerminatedRcvPacketProc;
	BOOL                 m_bTerminatedSndPacketProc;
	BOOL                 m_bTerminatedTimeOutProc;
					     
	CEvent               m_evtThreadTerminatedRcv;
	CEvent               m_evtThreadTerminatedSnd;
	CEvent               m_evtThreadTerminatedTimeOut;
	HWND                 m_ClientHWnd;
	int					 m_nTmOutMilliSec;
					     
protected:
	CTechwingSock*       m_pClientSock;
	int                  m_nSocketType;
	int			         m_nSplitCount;
	char		         m_szSplit[10000];
	char		         m_strDataBuf[127];
	

private:
	BOOL CriticalRcvLock(BOOL bLockUnlock);
	BOOL CriticalRcvIsLock();
	BOOL m_bCsIsLock;

	CLogger m_Logger;
	char    m_szLogName[512];
};

extern CIPCSock_Imp* g_pIPCSock_Imp;