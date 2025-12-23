#pragma once

#define MAX_RCV_BUFF				 (4096)
#define CLIENT_DISCONNECT_RETRY_TM	 (1000)

#define UM_TWIPCSOCK_RCV_EVENT		 (WM_USER + 1000)
#define UM_CLIENT_DISCONECT_RETRY	 (WM_USER + 1001)

#define IPC_EVT_CONNECTED			 (100)
#define IPC_EVT_DISCONNECTED		 (101)
#define IPC_EVT_RCV_EMPTY			 (102)
#define IPC_EVT_RCV_ERROR			 (103)
#define IPC_EVT_TM_OVER_MES			 (104)
#define IPC_EVT_CLIENT_CAN_NOT_CONNECT (105)

enum eSOCKET_INIT_TYPE
{
	eSocketTypeClient = 0,
	eSocketTypeServer,
};

enum e2dVision_TYPE{
	e2dVision_TYPE_TW =0,
	e2dVision_TYPE_Keyence,
};

/*
	0 = Acknowledge, command has been performed
	1 = Command does not exist
	2 = Cannot perform now
	3 = At least one parameter is invalid
	4 = Acknowledge, command will be performed with completion signaled later by an event
	5 = Rejected, Already in Desired Condition
	6 = No such object exists
	7-63 Reserved
*/
enum _eHCACK
{
	acknowledge = 0,
	cmd_does_not_exist,
	can_not_perform_now,
	parameter_is_invalid,
	acknowledge_cmd_will_be_performed,
	rejected,
	no_such_object_exists
};

//=================================================== define struct
typedef struct AFX_EXT_CLASS _ST_PACKET_HEAD{
	int         nMsgId;
	int			nS;
	int			nF;
	int			nLength;
	_ST_PACKET_HEAD(){	
		memset(this, 0x00, sizeof(_ST_PACKET_HEAD));	
	}
}ST_PACKET_HEAD;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S2F3{
	int nMsgID;
	int nStatus;
	int nDataCount;
	char szDataName[10][64];
	_ST_PACKET_BODY_S2F3(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S2F3));	
	}
}ST_PACKET_BODY_S2F3;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S2F4{
	int nMsgID;
	int nStatus;
	int nDataCount;
	char szDataValue[10][64];
	_ST_PACKET_BODY_S2F4(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S2F4));	
	}
}ST_PACKET_BODY_S2F4;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S2F41{
	int  nCmd;
	int  nParamCount;
	char szParam[10][64];
	_ST_PACKET_BODY_S2F41(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S2F41));	
	}
}ST_PACKET_BODY_S2F41;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S2F42{
	int nMsgID;
	int nIndex;
	int nRCMDACK;
	int nErrorDiscription;
	_ST_PACKET_BODY_S2F42(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S2F42));	
	}
}ST_PACKET_BODY_S2F42;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S5F1{
	int nALID;
	int nALST;
	_ST_PACKET_BODY_S5F1(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S5F1));	
	}
}ST_PACKET_BODY_S5F1;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S5F2{
	int nACK;
	_ST_PACKET_BODY_S5F2(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S5F2));	
	}
}ST_PACKET_BODY_S5F2;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S6F11{
	int nCEID;
    int nValCount;
    char szValue[10][64];
	_ST_PACKET_BODY_S6F11(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S6F11));	
	}
}ST_PACKET_BODY_S6F11;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S6F12{
	int nACK;
	_ST_PACKET_BODY_S6F12(){	
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S6F12));	
	}
}ST_PACKET_BODY_S6F12;

/////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_UNFORMATTED_DATA_LENGT     (4096)

// S107F1/F2 (Unformatted Data Load Request / Load Grant)
typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F1{
	int nDataID;
	int nDataLength; // [byte]

	_ST_PACKET_BODY_S107F1(){
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S107F1));
	}
}ST_PACKET_BODY_S107F1;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F2{
	int nDFDGNT; // Unformatted Data Grant
	/*
	0 = OK
	1 = Already have
	2 = No space
	3 = Invalid PPID
	4 = Busy, try later
	5 = Will not accept
	>5 = Other error
	6-63 Reserved
	*/
	_ST_PACKET_BODY_S107F2(){
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S107F2));
	}
}ST_PACKET_BODY_S107F2;

// S107F3/F4 (Unformatted Data Send / Ack)
typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F3{
	int  nDataID;
	char cData[MAX_UNFORMATTED_DATA_LENGT];
	_ST_PACKET_BODY_S107F3(){
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S107F3));
	}
}ST_PACKET_BODY_S107F3;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F4{
	int nACK7;
	/*
	0 = Accepted
	1 = Permission not granted
	2 = Length error
	3 = Matrix overflow
	4 = PPID not found
	5 = Mode unsupported
	6 = Command will be performed with completion signaled later
	>6 = Other error
	7-63 Reserved
	*/
	_ST_PACKET_BODY_S107F4(){
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S107F4));
	}
}ST_PACKET_BODY_S107F4;


// S107F5/F6 (Unformatted Data Request / Send)
typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F5{
	int nDataID;
	_ST_PACKET_BODY_S107F5(){
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S107F5));
	}
}ST_PACKET_BODY_S107F5;


typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F6{
	int  nDataID;
	char cData[MAX_UNFORMATTED_DATA_LENGT];
	_ST_PACKET_BODY_S107F6(){
		memset(this, 0x00, sizeof(_ST_PACKET_BODY_S107F6));
	}
}ST_PACKET_BODY_S107F6;

typedef struct AFX_EXT_CLASS _ST_PACKET_BODY_S107F9							
{							
	int	 nDataID;					
	int	 nStatus;					
	char szData[10][64];

	_ST_PACKET_BODY_S107F9(){						
		memset (this, 0x00, sizeof(_ST_PACKET_BODY_S107F9));
	}						
}ST_PACKET_BODY_S107F9;


//=================================================== define class head & body
class AFX_EXT_CLASS  CPacketHead
{
public:
	CPacketHead(){};
	CPacketHead(int nS, int nF){
		stData.nS = nS;
		stData.nF = nF;
		stData.nMsgId = MakeMsgId();
	};
	~CPacketHead(){};

	static int MakeMsgId(){
		m_nTheMsgid++;
		if( m_nTheMsgid > 99999 ){
			m_nTheMsgid = 1;
		}
		int nRefDebugValue = m_nTheMsgid;
		return m_nTheMsgid;
	}

	int GetSize(){	return sizeof(ST_PACKET_HEAD); }

	void SetPacketSize(int nSize){
		stData.nLength = nSize;
	}
	int  GetPacketSize(){ return stData.nLength;}

	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};


	ST_PACKET_HEAD stData;
public:
	static int  m_nTheMsgid;	
};


class AFX_EXT_CLASS  CPacketBody
{
public:
	virtual int   GetSize()=0;
	virtual BYTE* GetDataPtr()=0;
};
/****************************************************************** define class :: public body */
class AFX_EXT_CLASS  CPacketBody_S2F3 : public CPacketBody
{
public:
	int GetSize(){
		return sizeof(ST_PACKET_BODY_S2F3);
	}
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S2F3 stData;
};

class AFX_EXT_CLASS  CPacketBody_S2F4 : public CPacketBody
{
public:
	int   GetSize(){	return sizeof(ST_PACKET_BODY_S2F4);	}
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S2F4 stData;
};

class AFX_EXT_CLASS  CPacketBody_S2F41 : public CPacketBody
{
public:
	int   GetSize(){	return sizeof(ST_PACKET_BODY_S2F41); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S2F41 stData;
};

class AFX_EXT_CLASS  CPacketBody_S2F42 : public CPacketBody
{
public:
	int   GetSize(){	return sizeof(ST_PACKET_BODY_S2F42); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S2F42 stData;
};

class AFX_EXT_CLASS  CPacketBody_S5F1 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S5F1); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S5F1 stData;
};

class AFX_EXT_CLASS  CPacketBody_S5F2 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S5F2); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S5F2 stData;
};

class AFX_EXT_CLASS  CPacketBody_S6F11 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S6F11); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S6F11 stData;
};

class AFX_EXT_CLASS  CPacketBody_S6F12 : public CPacketBody
{
public:
	int   GetSize(){	return sizeof(ST_PACKET_BODY_S6F12); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S6F12 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F1 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F1); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F1 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F2 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F2); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F2 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F3 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F3); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F3 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F4 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F4); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F4 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F5 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F5); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F5 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F6 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F6); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F6 stData;
};

class AFX_EXT_CLASS  CPacketBody_S107F9 : public CPacketBody
{
public:
	int   GetSize(){ return sizeof(ST_PACKET_BODY_S107F9); }
	BYTE* GetDataPtr(){
		return (BYTE*)&stData;
	};
	ST_PACKET_BODY_S107F9 stData;
};

/******************************************************************************/

class AFX_EXT_CLASS  CPacketBodyFactory
{
public:
	CPacketBodyFactory(){};
	~CPacketBodyFactory(){};

	static CPacketBody* Create(int nS, int nF)
	{
		CPacketBody* pBody = NULL;
		switch( nS )
		{
		case 2:
			{
				switch( nF )
				{
				case 3: pBody = new CPacketBody_S2F3; break;
				case 4: pBody = new CPacketBody_S2F4; break;
				case 41: pBody = new CPacketBody_S2F41; break;
				case 42: pBody = new CPacketBody_S2F42; break;
				}
			}break;
		case 5:
			{
				switch( nF )
				{
				case 1: pBody = new CPacketBody_S5F1; break;
				case 2: pBody = new CPacketBody_S5F2; break;
				}
			}break;
		case 6:
			{
				switch( nF )
				{
				case 11: pBody = new CPacketBody_S6F11; break;
				case 12: pBody = new CPacketBody_S6F12; break;
				}
			}break;
		case 107:
			{
				switch( nF )
				{
				case 1: pBody = new CPacketBody_S107F1; break;
				case 2: pBody = new CPacketBody_S107F2; break;
				case 3: pBody = new CPacketBody_S107F3; break;
				case 4: pBody = new CPacketBody_S107F4; break;
				case 5: pBody = new CPacketBody_S107F5; break;
				case 6: pBody = new CPacketBody_S107F6; break;
				case 9: pBody = new CPacketBody_S107F9; break;
				}

			}break;
		}
		return pBody;
	}
};


class AFX_EXT_CLASS CPacket
{
public:
	static CPacket* MakePacket(int nS, int nF)
	{
		return new CPacket(nS,nF);
	}
	static void FreePacket(CPacket* pPacket)
	{
		if(pPacket)
			delete pPacket;
	}
public: 
	CPacket() {
		pHead = NULL;
		pBody = NULL;
	};
	CPacket(const CPacket& src)
	{
		pHead = NULL;
		pBody = NULL;
		int nS = src.pHead->stData.nS;
		int nF = src.pHead->stData.nF;
		pHead = new CPacketHead(nS, nF);
		pBody = CPacketBodyFactory::Create(nS, nF);
		CopyFromObject(src);
	};
	CPacket(int nS, int nF)
	{
		pHead = new CPacketHead(nS, nF);
		pBody = CPacketBodyFactory::Create(nS, nF);

		int nSize = pHead->GetSize() + pBody->GetSize();
		pHead->SetPacketSize(nSize);
	};
	~CPacket()
	{
		if( pHead ){
			delete pHead;
			pHead = NULL;
		}
		if( pBody ){
			delete pBody;
			pBody = NULL;
		}
	};

	CPacket& operator = (const CPacket& src)
	{
		if( this == &src ){
			return *this;
		}
		if( pHead ){
			delete pHead;
			pHead = NULL;
		}
		if( pBody ){
			delete pBody;
			pBody = NULL;
		}
		int nS = src.pHead->stData.nS;
		int nF = src.pHead->stData.nF;
		pHead = new CPacketHead(nS, nF);
		pBody = CPacketBodyFactory::Create(nS, nF);
		CopyFromObject(src);

		return *this;
	}

	CPacketHead*   pHead;
	CPacketBody*   pBody;

	void CopyFromByte(char* pByte)
	{
		memcpy(&pHead->stData, pByte, pHead->GetSize());
		memcpy(pBody->GetDataPtr(), pByte+pHead->GetSize(), pBody->GetSize());
	}

	void CopyFromObject(const CPacket& src)
	{
		memcpy(pHead->GetDataPtr(), src.pHead->GetDataPtr(), pHead->GetSize());
		memcpy(pBody->GetDataPtr(), src.pBody->GetDataPtr(), pBody->GetSize());
	}
};

class AFX_EXT_CLASS CTimeOutPacket
{
public:
	CTimeOutPacket()
	{
		m_pPacket = NULL;
		m_dTickCountStart = 0;
	};
	CTimeOutPacket(double dTickCountStart, CPacket* pPacket)
	{
		m_dTickCountStart = dTickCountStart;

		m_pPacket = new CPacket(pPacket->pHead->stData.nS, pPacket->pHead->stData.nF);
		m_pPacket->CopyFromObject( *pPacket );
	};
	~CTimeOutPacket()
	{
		if( m_pPacket ){
			delete m_pPacket;
			m_pPacket = NULL;
		}		
	};

	CPacket* m_pPacket;
	double    m_dTickCountStart;

	void CopyFromObject(CTimeOutPacket& src)
	{
		m_dTickCountStart = src.m_dTickCountStart;
		memcpy(m_pPacket->pHead->GetDataPtr(), src.m_pPacket->pHead->GetDataPtr(), m_pPacket->pHead->GetSize());
		memcpy(m_pPacket->pBody->GetDataPtr(), src.m_pPacket->pBody->GetDataPtr(), m_pPacket->pBody->GetSize());
	}
};

#ifndef __FN_CALLBACK_EVENT__
#define __FN_CALLBACK_EVENT__
	typedef void (*FN_CALLBACK_EVENT)(int,long);
#endif //__FN_CALLBACK_EVENT__

#ifndef __FN_CALLBACK_TWIPC_RECEIVE__
#define __FN_CALLBACK_TWIPC_RECEIVE__
	typedef int (*FN_CALLBACK_TWIPC_RECEIVE)(CPacket*);
#endif //__FN_CALLBACK_TWIPC_RECEIVE__

typedef struct _ST_MESSAGE_MAP{
	int nS;
	int nF;
	FN_CALLBACK_TWIPC_RECEIVE fnReceiveCallback;

}ST_MESSAGE_MAP;

extern UINT g_nChildWndID;
UINT g_fnMakeChildWndID();

