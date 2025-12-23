#pragma once
#include <afxtempl.h>
#include "ParamList.h"
#include <vector>
#include <utility>

class CTesterClientSock;

//===================================================================
//
//===================================================================
class CTesterStatus
{
public:
	int            m_nStationID;							// Registraion 수신 시 갱신
	int            m_nRegistration;							// Registraion 수신 시 갱신
	int			   m_nRegiDutIdentityNo[STATION_MAX_PARA];	// Registartion 수신 할때 갱신. 
	int            m_nStatus;								// Tester Status 수신 시 갱신
	int            m_nReserved2;							// Tester Info를 통해 전달 받은 location 정보 저장
	int            m_nReserved3;							// Tester Info를 통해 전달 받은 occupied 정보 저장
	CString        m_sTimeStamp;							// heart bit 수신 시간 저장
	CString        m_sInfo[STATION_MAX_PARA];				// Tester Information을 통해 전달 받은 테스터 HW, FW, SW version 정보 저장
	int            m_nLastTestResult[ STATION_MAX_PARA ];
	int            m_nLastReserved[ STATION_MAX_PARA ];     // Test Result를 통해 받음
 	CString        m_sLastErrString[ STATION_MAX_PARA ];	// Test Result를 통해 받음
 	CString        m_sLastDutSN[ STATION_MAX_PARA ];		// Test Result를 통해 받음 Last Dut Serial Number
	BOOL           m_bHeartBit;								// heart bit 수신 시 마다 상태 변경.
	BOOL           m_bBeforeHeatBit;						// 이전의 heart bit flag
	CTime          m_cChkTimeStamp;

	// tester로부터 받은 것.
	CString			m_sTesterID;
	CString			m_sSocketID[ STATION_MAX_PARA ];
	CString         m_sHW_version;							 // 20230828 hyb Tester Information 수신시 갱신 한다.
	CString		    m_sPPO_FW_version;						 // 20231024 hyb Tester Information FW Spec 변경 수정
	CString		    m_sHID_FW_version;						 // 20231024 hyb Tester Information FW Spec 변경 수정
	CString			m_sSW_Version;							 // 20230828 hyb Tester Information 수신시 갱신 한다.
	CString			m_sTest_Script_Info;					// 20230828 hyb Tester Information 수신시 갱신 한다.
	CString			m_sTest_Script_Apl;						// APL 진행 시 Handler -> Tester 정보를 넘겨준 기반으로 다시 수신 한다 Carpenter 패키지 한정  [3/19/2025 donghyun.shin]
	int				m_nTest_Mode;							// APL 진행 시 Handler -> Tester 정보를 넘겨준 기반으로 Operation ID 다시 수신 한다 Watt 패키지 한정  [4/7/2025 donghyun.shin]
	CString			m_sIP;

	// information으로 받은 것.
	CString			m_sSocketID_Orginal[ STATION_MAX_PARA ];
	CString			m_sTesterID_Orginal;

	int            m_nYanMadeStatus[STATION_MAX_PARA];

	CString		   m_sTestScriptInfo;
	CString		   m_sTestSWVersion;
	CString			m_sTestRetest;
	CString			m_sTestHandlerID;
	CString			m_sTestDevice;
	_eSetTesterInformation	m_nTesterInformation; // 테스터 통신 소켓 연결 후(핸들러가 Registarion 수신) 테스터 소켓에 Tester Information 수신 하기 위해서. 
public:
	CTesterStatus();
	CTesterStatus(const CTesterStatus& ref);
	~CTesterStatus();

	CTesterStatus& operator=(const CTesterStatus& ref);
	void SetVersionInfo(CString sInfo, int nIdx);
	CString GetStatus();
	

	
	void PasingMsgType(CString sInfo, std::vector<std::pair<std::string, std::string>>& data_list, int MsgType = DEF_MSG_TYPE_REGIST); //each socket
	
	// "version1:0.0.1 version2:0.0.2 version3:0.0.3" 이런식으로 되어 있음. 첫번째 구분자 ";", 두번째 구분자 ":"
	void PasingInformation(CString sInfo, std::vector<std::pair<std::string, std::string>>& data_list); //single, dual
};

//===================================================================
//
//===================================================================
class CTester
{
public:
	CTester(void);
	~CTester(void);

public:
	CTesterClientSock* m_pSock;
	SendData m_SendData;

private:
	CTesterStatus m_TesterStatus;

public:
	BOOL SendAction(int action, WPARAM wParam=NULL, LPARAM lParam=NULL);
	void MakeSendData(BYTE* data, int action);

	void SetStatus(int a_nStatus);
	int GetStatus(){ return m_TesterStatus.m_nStatus; };
	
	void SetHeartBit(CString sTimeStamp);
	
	void SetRegiDutIdentityNo(int nDutIdentityNo);
	void SetRegistration(int nReg);
	void SetInformation( CString sTesterID, CString sSocketIDs, CString sSW_Version, CString sIP );
	
	void SetInformation(std::vector<std::pair<std::string, std::string>> tester_data);
	void SetStationID(int nID);
	int GetStationID(){ return m_TesterStatus.m_nStationID; };
	int GetRegiDutIdentityNo() { return m_TesterStatus.m_nRegiDutIdentityNo[0]; }
	

	void SetInfo(CString sInfo);

	void SetScriptInformation(CString strScriptAPL) { m_TesterStatus.m_sTest_Script_Apl = strScriptAPL; }
	CString GetScriptInformation() { return m_TesterStatus.m_sTest_Script_Apl; }

	void SetLocation(int nLocation);
    int  GetLocation(){ return m_TesterStatus.m_nReserved2; };

	void SetOccupied(int nOccupied);

	void SetEachTestResult( int nResult, int nReserved, const char * szErrStr, const char * szDutSN, const char * szSocketID, const char* szErrCode, int nIdx );
	void SetTestResult(int nResult, int nReserved, const char * szErrStr, const char * szDutSN, const char * szSocketID, int nIdx);

	CTesterStatus GetTesterStatus(){ return m_TesterStatus; };
	void SetTesterInformationStatus(_eSetTesterInformation eSetValue ) {  m_TesterStatus.m_nTesterInformation = eSetValue; };
	void SendHeartBeatAction(RecvData recvData, WPARAM wParam=NULL, LPARAM lParam=NULL);

	
	void SetTestScript(CString sTestScript);
	CString GetTestScript() {return m_TesterStatus.m_sTest_Script_Apl;};

	void CTester::SetTestMode(int nTest_Mode);
	CString GetTestMode();

	void Disconnection();
	void Connection();
	void LastResultReset();

	CString GetStringActionType(int nAction);
	int TesterNullPointerCheck(CTesterClientSock* pSock);
	void SetAPLInfo(const char* szTestScriptInfo, const char* szTestSWVersion, const char* szTestRetest, const char* szTestHandlerID, const char* szTestDevice);

	void SendTestAPLInfoAction();
	void SendTestLotStatusAction(eTesterLotStatus _eTesterLotStatus);

private:
};

