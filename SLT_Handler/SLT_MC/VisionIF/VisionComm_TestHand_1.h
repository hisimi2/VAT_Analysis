#pragma once
#include "visioncomm.h"
#include "..\VisionAutoTeaching\VAT_CommonData.h"

class CVisionComm_TestHand_1 : public CVisionComm
{
public:
	CVisionComm_TestHand_1(int nVisionPcIdx, CString strVisionName);
	CVisionComm_TestHand_1(void);
	~CVisionComm_TestHand_1(void);

	static ST_MESSAGE_MAP MessageMap[];

	static int OnReceiveS2F3(CPacket* pPacket);
	static int OnReceiveS2F4(CPacket* pPacket);
	static int OnReceiveS2F41(CPacket* pPacket);
	static int OnReceiveS2F42(CPacket* pPacket);
	static int OnReceiveS5F1 (CPacket* pPacket);
	static int OnReceiveS5F2 (CPacket* pPacket);
	static int OnReceiveS6F11(CPacket* pPacket);
	static int OnReceiveS6F12(CPacket* pPacket);
	static int OnReceiveS107F1(CPacket* pPacket);
	static int OnReceiveS107F2(CPacket* pPacket);
	static int OnReceiveS107F3(CPacket* pPacket);
	static int OnReceiveS107F4(CPacket* pPacket);
	static int OnReceiveS107F5(CPacket* pPacket);
	static int OnReceiveS107F6(CPacket* pPacket);
	static int OnReceiveS107F9(CPacket* pPacket);



	LRESULT OnTWIPCEvent(WPARAM wParam, LPARAM lParam);
	int Initialize(CWnd* pParentWnd, char* szDriverFileName);
	void Finalize();


// 	void SetCamStatus(double bCamStatus[]);
// 	void GetCamStatus(double bCamStatus[]);
	BOOL m_bVisionComm;
	void SetVisionComm(BOOL bState);
	BOOL GetVisionComm();

//==================================================================PCD/BACKLASH
public:
	int m_nIsReceived[VAT_HAND_MAX + 1];
	//double m_dCamStatus[VAT_MAX_CAM_STATUS];

	std::vector<ST_PCD_BACKLASH_RCV_DATA> m_vRcvData;

public:
	int  SendMeasure_PcdBacklash(int nMeasureType, int nAreaVAT);
	static void OnReceive_PcdBacklash(CPacket* pPacket);

	void SetReceiveData(ST_PCD_BACKLASH_RCV_DATA vRcvData);
	void GetReceiveData(int nAreaVAT, ST_PCD_BACKLASH_RCV_DATA &vRcvData);

	int GetHandTypeIdx(_eTestPPIdx eTestPPIdx, eTestPPType eHandType);

	void SetReceiveState(int nHand, int nState);
	int GetReceiveState(int nHand);

//==================================================================Vision Auto Teaching
public:
	CCriticalSection m_csSend;
	CCriticalSection m_csRcv;
public:
	int SendMeasure_VAT(CPacket* pPacket);
	static void OnReceive_VAT(CPacket* pPacket);






};

