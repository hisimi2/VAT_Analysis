#pragma once

#include "VisionComm.h"
#include "DEF_LIB_IPC_SOCK.h"
#include "DualTCP_Sock.h"
#include "DEF_COMMON.h"


typedef std::vector<CString>	TokenArray;

class CVisionComm_Barcode :	public CVisionComm
{
public:
	CVisionComm_Barcode(int nVisionPcIdx, CString strVisionName, int nCamCompanyType=e2dVision_TYPE_TW);
	CVisionComm_Barcode();
	~CVisionComm_Barcode(void);

	static ST_MESSAGE_MAP MessageMap[];

	static int OnReceiveS2F3 (CPacket* pPacket);
	static int OnReceiveS2F4 (CPacket* pPacket);
	static int OnReceiveS2F41 (CPacket* pPacket);
	static int OnReceiveS2F42 (CPacket* pPacket);
	static int OnReceiveS5F1  (CPacket* pPacket);
	static int OnReceiveS5F2  (CPacket* pPacket);
	static int OnReceiveS6F11 (CPacket* pPacket);
	static int OnReceiveS6F12 (CPacket* pPacket);
	static int OnReceiveS107F1(CPacket* pPacket);
	static int OnReceiveS107F2(CPacket* pPacket);
	static int OnReceiveS107F3(CPacket* pPacket);
	static int OnReceiveS107F4(CPacket* pPacket);
	static int OnReceiveS107F5(CPacket* pPacket);
	static int OnReceiveS107F6(CPacket* pPacket);

	LRESULT OnTWIPCEvent(WPARAM wParam, LPARAM lParam);
	int Initialize(CWnd* pParentWnd, char* szDriverFileName);
	void Finalize();

	int Cmd_SetCok(_tVisionMsg* pVisionMsg=NULL);
	int Cmd_ReqCleanDisk(_tVisionMsg* pVisionMsg=NULL);
	int Cmd_ReqVisionShutDown(_tVisionMsg* pVisionMsg=NULL);
	int Cmd_ReqInspReady(int nCamNo,_tVisionMsg* pVisionMsg=NULL);
	int Cmd_ReqMeasure(int nCamNo,_tVisionMsg* pVisionMsg=NULL, int nDirPocket=e2dPocketDirection_REAR_LEFT/*enum e2dPocketDirection*/);
	int Cmd_Req2DMeasure(int nCamNo);
	int Cmd_ReqMeasureExist(int nCamNo,_tVisionMsg* pVisionMsg=NULL);
	int Cmd_VisionUseNoUse( int nUseNoUse, _tVisionMsg* pVisionMsg=NULL );
	int GetCmdDone(_tVisionMsg* pVisionMsg);

	int Cmd_KeyenceLOff(int nCamNo); //int nDirPocket/*enum e2dPocketDirection*/);

	int Cmd_ReqMeasureResult(int nCamNo,_tVisionMsg* pVisionMsg=NULL ,int nDirPocket=e2dPocketDirection_REAR_LEFT/*enum e2dPocketDirection*/);
	int Cmd_Req2DMeasureResult(int nCamNo, int nTblIdx, TokenArray* str2DErrorData, _tVisionMsg* pVisionMsg = NULL);
	int Cmd_ShuttleMoveComplte(int nLoadTblIdx, _tVisionMsg* pVisionMsg=NULL);
	void ChangePocketNum(int nCamNo, int &nPocketX, int &nPocketY, int &nPocketNum);

	void Erase2dPicture(int nTblIdx);

	void AddRetryCountCurLot(int nBarcodeNo);
	void AddTriggerTotalCount(int nBarcodeNo, int nTrrigerCnt);
private:
	CDualTCP_Sock* m_pDualSock1;
	CDualTCP_Sock* m_pDualSock2;
	CDualTCP_Sock* m_pDualSock3;
	CDualTCP_Sock* m_pDualSock4;
	int  m_nRecvLen_L;
	int  m_nRecvLen_R;


	CString m_strLeft2dIp[2];	//Left 영역 Barcode 2ea
	CString m_strRight2dIp[2];	//Right 영역 Barcode 2ea
};

