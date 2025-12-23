#pragma once

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
#define ERR_STR				(32)
#define ERR_CODE_STR		(4)
//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
#define DUT_SN_STR_EACH			(128)	// yanmade communiaction protocol spec 128byte
//#else
#define DUT_SN_STR			(64)
//#endif
#define SOCKET_ID_STR		(64)
#define TIME_STAMP_STR		(14)
#define INFO_STR_STR		(1460)
#define RESULT_DATA_STR     (165)

#define TEST_APL_SCRIPT_INFO_STR				(32)
#define TEST_APL_SW_VERSION_INFO_STR			(32)
#define TEST_APL_RETEST_STR						(32)
#define TEST_APL_HANDLER_ID_STR					(32)
#define TEST_APL_DEVICE_STR						(32)

// SEND DATA LENGTH
#define MODE_STR_MAX			(32	)
#define FULLSITES_STR_MAX		(32	)
#define SERIAL_STR_MAX			(128)
#define SERIAL_SINGLE_STR_MAX			(160)

//Test APL Info - Watt 패키지 전용
#define APL_INFO_DATA_MAX			(163)
#define TEST_SCRIPT_STR_MAX			(32)
#define TEST_SW_VERSION_STR_MAX		(32)
#define TEST_RETEST_STR_MAX			(32)
#define TEST_HANDLER_ID_STR_MAX		(32)
#define TEST_DEVICE_STR_MAX			(32)
//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET

#define TEST_SCRIPT_STR_MAX			(32)

#define TEST_LOT_STATUS_STR_MAX		(5)


	#define SEND_MSG_DATA_EACH_MAX		(195)//(580)
//#else
	#define SEND_MSG_DATA_MAX		(1348)//(580)
//#endif

#define SEND_HEART_BEAT_DATA_MAX (17)
#define SEND_HOST_NAME           (32)
#define SEND_DATA_CRC						(0x0A)

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
#define TESTER_REG_FOR_AMKOR (8) // RECV
#define TESTER_TEST_SCRIPT (9) // RECV
#define SET_TESTER_MODE			(10) // RECV
#define TESTER_APL_INFO			(11) // SEND & RECV
#define TESTER_LOT_STATUS		(12) // SEND 
// ACTION TYPE
#define ACTION_START			(1)
#define ACTION_STOP				(2)
#define ACTION_LOT_END			(3)
#define ACTION_RESET			(3)	// TWSL421 Yanmade 통신 사양
#define ACTION_STATUS			(4)
#define ACTION_INFO				(5)
#define ACTION_TEST_SCRIPT_INFO	(6)
#define ACTION_SET_TEST_MODE	(8)


// STATUS TYPE
#define STATUS_INIT			(0)
#define STATUS_IDLE			(1)
#define STATUS_TESTING		(2)
#define STATUS_ERROR		(3)
#define STATUS_UNREGISTER	(4)
#define STATUS_RESULT_WAIT	(5)

#define REG_TESTER			(1)
#define UNREG_TESTER		(0)

#define TEST_RESULT_FAIL    (0)
#define TEST_RESULT_PASS    (1)

#define UNIT_INDEX_PATH		"sys_data\\UnitIndex.ini"
#define RECV_TESTER_ID_LIST "sys_data\\RecvTesterID.ini"

#define REG_SOCKET_ID_ERROR "ERROR_SOCKET_ID"
// SWAP
#define SWAP16(s) (((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)))  
#define SWAP32(l) (((((l) & 0xff000000) >> 24) | (((l) & 0x00ff0000) >> 8)  | (((l) & 0x0000ff00) << 8)  | (((l) & 0x000000ff) << 24)))




// STRUCT
typedef union DataHeaderInfo {
	struct
	{
	// Common
	unsigned int m_nSm:1;
	unsigned int m_nSc:1;
	unsigned int m_nStationID:20;
	unsigned int m_nType:4;
	unsigned int m_nReserved:6;
	}m_bit;
	unsigned int m_nData;
}DataHeader;

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
	int m_nReserved1[ STATION_MAX_PARA ];
	char m_cErrorStr[ STATION_MAX_PARA ][ERR_STR];
	char m_cDutSn[ STATION_MAX_PARA ][DUT_SN_STR_EACH]; //최대로 선언.
	char m_cSocketID[ STATION_MAX_PARA ][ SOCKET_ID_STR ];
	char m_cErrorCode[STATION_MAX_PARA][ERR_CODE_STR];

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
	char m_cMode[ MODE_STR_MAX ];
	char m_cFullsites[ FULLSITES_STR_MAX ];
	char m_cSerialNum[ STATION_MAX_PARA ][ SERIAL_STR_MAX ];
	char m_cSingleSerialNum[STATION_MAX_PARA][SERIAL_SINGLE_STR_MAX];
	char m_cTpRev[STATION_MAX_PARA][SERIAL_STR_MAX];
	BYTE m_nCRC;

	SendDataInfo() {
		ZeroMemory( this, sizeof( SendDataInfo ) );
	}
} SendData;

typedef struct ResultDataInfo {
	unsigned int m_nStationID;
	unsigned int m_PhysicalNo[STATION_MAX_PARA];
	int m_nTestResult[ STATION_MAX_PARA ];
	//int m_nErrorCode;
	char m_cErrorCode[ STATION_MAX_PARA ][ERR_CODE_STR+1];
	char m_cErrorStr[ STATION_MAX_PARA ][ERR_STR];
	char m_cDutSn[ STATION_MAX_PARA ][DUT_SN_STR_EACH]; /*DUT_SN_STR*/

	ResultDataInfo(){
		ZeroMemory(this, sizeof(ResultDataInfo));
	}
} ResultData;

typedef struct UnitIdxInfo {
	int  m_nStationID;
	int  m_nUnitIdx;
    BOOL m_bRegDataConnected;

	int  m_nReserved2;
	char m_cInfoStr[INFO_STR_STR];
	

    UnitIdxInfo(){
        m_nStationID = -1;
        m_nUnitIdx  = -1;
        m_bRegDataConnected = FALSE;

		m_nReserved2 = -1;
		ZeroMemory(m_cInfoStr, sizeof(m_cInfoStr));
    }
} UnitIdx;

//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
typedef struct _ST_TESTER_ACTION_PARAM{
	bool m_bSiteOn;
	char m_cLotNum[ 64 ];
	char m_cTesterID[ 32 ];
	char m_cSocketID[ 32 ];
	char m_c2DID[ 128 ];
	char m_cFTRT[8];
	char m_cRTCode[8];

	_ST_TESTER_ACTION_PARAM(){
		ZeroMemory(this, sizeof(_ST_TESTER_ACTION_PARAM));
	}
}ST_TESTER_ACTION_PARAM;

//#else
// typedef struct _ST_TESTER_ACTION_PARAM {
// 	bool m_bSiteOn;
// 	char m_cLotNum[32];
// 	char m_cTesterID[32];
// 	char m_cSocketID[32];
// 	char m_c2DID[128];
// 	char m_cFTRT[32];
// 
// 	_ST_TESTER_ACTION_PARAM() {
// 		ZeroMemory(this, sizeof(_ST_TESTER_ACTION_PARAM));
// 	}
// }ST_TESTER_ACTION_PARAM;
//#endif

#include "Tester.h"
#include "TesterMgr.h"
#include "lbasesock.h"
#include "TesterClientSock.h"
#include "TesterServerSock.h"
#include <afxtempl.h>
#include "Logger.h"

// CTesterIF

class CTesterIF : public CWnd
{
	DECLARE_DYNAMIC(CTesterIF)

public:
	CTesterIF();
	virtual ~CTesterIF();

public:
	int m_nType;
	CTesterServerSock m_ServerSock;
	
	CTesterMgr			m_TesterMgr;
	//CList<ResultData*>	m_listResult;
	//CList<UnitIdx*>		m_listUnitIdx;

	//CRITICAL_SECTION*	m_pCSTesterIF;
 	//CLogger             m_Logger;
	
public:
	BOOL ServerStart(int type = TESTER_TYPE_TCP);
	BOOL SendActionAll( int action );
	int  SendAction( int stationID, int action, WPARAM wParam = NULL, LPARAM lParam = NULL, int* pDirSiteOnOff = NULL,  int nTesterMode = eTester_Single);
	int  GetStatus(int stationID, int* pDirSiteOnOff = NULL, int nTesterMode = eTester_Single, int* nTesterNum=NULL);
	BOOL IsConnected(int stationID, int nTesterMode = eTester_Single, BOOL *pSocketOnffInfo = NULL);

	//void AddResultData(RecvData& recvData);
	//BOOL GetResultData(int stationID, ResultData* rd);
	BOOL GetResultDataFromTester(int stationID, ResultData* rd, int* nDirSiteOnOff = NULL, int nTesterMode = eTester_Single);

	CTesterStatus GetTesterStatus(int stationID,int nMode=eTester_Single );
	CTime GetTesterTimeStamp(int stationaID );
	int GetStationID(int stationID );
    //int GetUnitIndex(int stationID );

	void MakeLog( LPCTSTR fmt, ... );
	void MakeHexLog( BYTE * data, int len, LPCTSTR header_fmt, ... );

    //void SetUnitDataClear();

	// Get location information 
	//void GetTesterInfoAll();
	//void SetUnitData(int unitIdx, int stationID);
	//void GetUnitData();

	void HeartBeatOverCutClient(int stationID, BOOL bUseIdentityNo = FALSE);

private:
	BOOL ServerStartTcp();
	BOOL ServerStartGpib();

	BOOL SendActionTcp(int action);
	BOOL SendActionGpib(int action);
	//void DeleteListAll(CList<ResultData*> &list);
	//void DeleteListAll(CList<UnitIdx*> &list);	

	std::vector<UnitIdx> m_vTesterId;
protected:
	DECLARE_MESSAGE_MAP()
};


