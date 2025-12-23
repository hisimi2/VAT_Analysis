#pragma once

#include <deque>
#include "iocp_socket/iocp_socket_library/IOCP_core.h"

// CConectSocket 명령 대상입니다.
#define RET_ERROR			(-1)

#define TESTER_TYPE_TCP		(0)
#define TESTER_TYPE_GPIB	(1)

#define UNIT_MAX		(16)

// DATA LENGTH
#define MAX_MSG_ID		(4)
#define MAX_MSG_DATA	(1459)//(1456)
#define MSG_MAX			(1463)

// RECV DATA MAX LENGTH
#define REG_DATA_MAX		(668)
#define ACTION_DATA_MAX		(200)
#define STATUS_DATA_MAX		(8)
#define RESULT_DATA_MAX		(172)
#define HEART_DATA_MAX		(21)
//#define INFO_DATA_MAX		??

// RECV DATA LENGTH
#define REG_STR				(660)
#define TESTER_INFO_STR		(1451)
#define ERR_STR				(32)
#define ERR_CODE_STR		(32)

#define DUT_SN_STR			(128)
#define SOCKET_ID_STR		(64)
#define TIME_STAMP_STR		(14)
#define INFO_STR_STR		(1460)

#define RESULT_DATA_STR     (165)

// SEND DATA LENGTH
#define MODE_STR_MAX			(32	)
#define FULLSITES_STR_MAX		(32	)
#define SERIAL_STR_MAX			(128)//(128)
#define SEND_MSG_DATA_MAX		(1348)//(580)
#define SEND_HEART_BEAT_DATA_MAX (17)
#define SEND_HOST_NAME           (32)
#define CRC						(0x0A)

// Registration Type
#define REG_TYPE_NONE		(0)
#define REG_TYPE_PROJECT	(1)
#define REG_TYPE_SCRIPT		(2)

// DATA TYPE
#define TESTER_REG			(1) // RECV
#define ACTION				(2) // SEND
#define TESTER_STAUS		(3) // RECV
#define TEST_RESULT			(4) // RECV
#define ACKNOWLEDGEMENT		(5) // NOT USE
#define HEART_BEAT			(6) // RECV?
#define TESTER_INFORMATION	(7) // RECV
#define TESTER_SCRIPT		(9) // RECV
// ACTION TYPE
#define ACTION_START			(1)
#define ACTION_STOP				(2)
#define ACTION_RESET			(3)
#define ACTION_STATUS			(4)
#define ACTION_INFO				(5)
#define ACTION_SELF_TEST		(6)
#define ACTION_CALIBRATION		(7)
#define ACTION_SET_TEST_MODE	(8)
#define ACTION_TEST_SCRIPT		(6)
// STATUS TYPE
#define STATUS_INIT			(0)
#define STATUS_IDLE			(1)
#define STATUS_TESTING		(2)
#define STATUS_ERROR		(3)
#define STATUS_UNREGISTER	(4)

#define REG_TESTER			(1)
#define UNREG_TESTER		(0)

#define TEST_RESULT_FAIL    (0)
#define TEST_RESULT_PASS    (1)
#define STATION_MAX_PARA 8

typedef union DataHeaderInfo {
	struct
	{
		// Common
		unsigned int m_nSm : 1;
		unsigned int m_nSc : 1;
		unsigned int m_nStationID : 20;
		unsigned int m_nType : 4;
		unsigned int m_nReserved : 6;
	}m_bit;
	unsigned int m_nData;
}DataHeader;


// SWAP
#define SWAP16(s) (((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)))  
#define SWAP32(l) (((((l) & 0xff000000) >> 24) | (((l) & 0x00ff0000) >> 8)  | (((l) & 0x0000ff00) << 8)  | (((l) & 0x000000ff) << 24)))

typedef struct RecvDataInfo {
	DataHeader m_DataHeader;

	// Common
	unsigned int m_nReserved;
	unsigned int m_nType;
	unsigned int m_nStationID;
	unsigned int m_nSc;
	unsigned int m_nSm;

	int m_nDataLen;

	// Register
	int m_nRegister;
	char m_cRegStr[REG_STR];

	// Status
	int m_nStatus;

	// Test Result
	int m_nTestResult[STATION_MAX_PARA];
	int m_nReserved1[STATION_MAX_PARA];
	char m_cErrorStr[STATION_MAX_PARA][ERR_STR];
	char m_cDutSn[STATION_MAX_PARA][DUT_SN_STR];
	char m_cSocketID[STATION_MAX_PARA][SOCKET_ID_STR];

	// Heart Beat
	char m_cTimeStamp[ERR_STR];

	// Information
	int m_nReserved2;
	int m_nReserved3;
	char m_cInfoStr[INFO_STR_STR];

	int m_nCRC;
} RecvData;

typedef struct SendDataInfo {
	DataHeader m_DataHeader;

	// Common
	unsigned int m_nReserved;
	unsigned int m_nType;
	unsigned int m_nStationID;
	unsigned int m_nSc;
	unsigned int m_nSm;

	unsigned short m_nDataLen;
	BYTE m_nAction;
	char m_cMode[MODE_STR_MAX];
	char m_cFullsites[FULLSITES_STR_MAX];
	char m_cSerialNum[8][SERIAL_STR_MAX];

	BYTE m_nCRC;

	SendDataInfo() {
		ZeroMemory(this, sizeof(SendDataInfo));
	}
} SendData;

typedef struct SendDataInfoReg {
	DataHeader m_DataHeader;

	// Common
	unsigned int m_nReserved;
	unsigned int m_nType;
	unsigned int m_nStationID;
	unsigned int m_nSc;
	unsigned int m_nSm;

	unsigned short m_nDataLen;
	BYTE m_nAction;
	char m_cInfo[REG_STR];

	BYTE m_nCRC;

	SendDataInfoReg() {
		ZeroMemory(this, sizeof(SendDataInfoReg));
	}
} SendDataReg;

typedef struct SendDataInfoStat {
	DataHeader m_DataHeader;

	// Common
	unsigned int m_nReserved;
	unsigned int m_nType;
	unsigned int m_nStationID;
	unsigned int m_nSc;
	unsigned int m_nSm;

	unsigned short m_nDataLen;
	BYTE m_nAction;
	char m_cInfo;

	BYTE m_nCRC;

	SendDataInfoStat() {
		ZeroMemory(this, sizeof(SendDataInfoStat));
	}
} SendDataStat;

class CConectSocket 
{
public:
	CConectSocket();
	virtual ~CConectSocket();
	void OnClose(CClientConnection* _client);
	void OnReceive(CClientConnection* _client);

	void Initialize(int nTesterID);
	void MakeSendData(BYTE* data, int action, int nSubAct = 0);

	int  GetAction();
	int  GetState() { return m_nState; };
	// getter Function 추가
	int getStationId() { return m_nTesterID; };
	int getActionMode() { return m_nActionMode; };
	int getSocketId() { return m_nTesterID; };
	CString getIp() { return m_strIp; };
	CString getVersion() { return m_strVersion; };
	BOOL getIsConnect() { return m_bConnect; };
	//
	void setIp(CString strIp) { m_strIp = strIp; };
	void setActionMode(int nActionMode) { m_nActionMode = nActionMode; };
	void setConnect(BOOL connect) { m_bConnect = connect; };

	void CloseSocket();
	void Send(const void* lpBuf, int nBufLen );
	int Connect(CString ip, int port);
	int Reconnect();

public:
	//지울것
	void Create() {}

private:
	CClientConnection* m_pClient;
	int m_nTesterID;
	int m_nState;
	// variable 추가
	int m_nActionMode; // Normal, GRR, Loop, Audit
	int m_nPort;
	CString m_strIp;
	CString m_strVersion;
	BOOL m_bConnect;
	//
	int m_nSplitCount;
	char m_szSplit[10000];
	RecvData m_RecvData;
	SendData m_SendData;

	void ParserMsgID(BYTE* data, RecvData& recvData);
	void ParserMsgData(BYTE* data, RecvData& recvData, int nRevLength);

	std::deque<int> qDvcData;
	CCriticalSection m_cs;
};


