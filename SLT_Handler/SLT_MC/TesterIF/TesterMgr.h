#pragma once

class CTesterMgr
{
public:
	CTesterMgr(void);
	~CTesterMgr(void);




public:
	CList<CTester*> m_listTester;
	int m_nTesterOption;
	std::vector<int> m_veIdentityNo[MAX_TESTER_DUAL];
public:
	BOOL RegTester( RecvData& recvData, CTesterClientSock* pSock );
	BOOL UnRegTester( RecvData& recvData );
	void AllUnRegTester();
	void UnRegTesterEach(int stationID, BOOL bUseIdentityNo = FALSE);
	void UnRegTesterIdentity(RecvData& recvData);

	std::vector<int> GetTesterSocketIndexRange(int nIdentityNo);

	CTester* GetTester(int stationID, BOOL bUseIdentityNo = FALSE);
	BOOL SendActionAll(int action);
	int  SendAction(int stationID, int action, WPARAM wParam = NULL, LPARAM lParam = NULL, int* pDirSiteOnOff = NULL, int nTesterMode = eTester_Single);
	int  GetStatus(int stationID, int* pDirSiteOnOff = NULL, int nTesterOption = eTester_Single, int* nTesterNum = NULL);
	int SetInformationStatus(int stationID, _eSetTesterInformation eStatus);
	BOOL SetStatus(int stationID, int status);
	BOOL SetTimeStamp(int stationID, CString timeStamp);
	BOOL SetEachTestResult( int stationID, int nResult, int nReserved, const char * szErrStr, const char * szDutSn, const char * szSocketID, const char * szErrCode, int nIdx );
	BOOL SetTestResult(int stationID, int nResult, int nReserved, const char * szErrStr, const char * szDutSn, const char * szSocketID, int nIdx);
	void DeleteListAll(CList<CTester*> &list);
	BOOL IsConnected(int stationID, int nTesterMode = eTester_Single, BOOL *pSocketOnffInfo = NULL);
	
	BOOL DisconnectSock(CTesterClientSock* sock);
	void DisconnectSockAll();

	void SendQueryInformation();
	void SetInformation(int stationID, CString info);

	void SendQueryScriptInfromation();

	void GetTesterInfoAll();
	void SendHeartBeat(int ntesterID, RecvData recvData, WPARAM wParam=NULL, LPARAM lParam=NULL);

	void HeartBeatOverCutClient(int stationID, BOOL bUseIdentityNo = FALSE);

	

	void SetTestScriptInfomation(int stationID, CString info);

	void SendSetTestMode(int nStationID);
	void SetTestMode(int stationID, CString info);

	void SendQuerySetTestMode(BOOL bManual = eEqpCtrlMode_Auto);
	void SendQuerySetAPLInfo(BOOL bManual = eEqpCtrlMode_Auto);
	void SendQuerySetLotStatus(eTesterLotStatus _eTesterLotStatus);

	int DualTesterIndex(int stationID, int nDir);
	int EachTesterIndex(int stationID, int nSocketNo);
	int LastResultReset(int stationID, int* nDirSiteOnOff);
	int GetDutIdentityNo(int stationID);
	int TesterNullPointerCheck(CTester* tester, int nMode);

	BOOL SetAPLInfo(int stationID, const char* szTestScriptInfo, const char* szTestSWVersion, const char* szTestRetest, const char* szTestHandlerID, const char* szTestDevice);


};

