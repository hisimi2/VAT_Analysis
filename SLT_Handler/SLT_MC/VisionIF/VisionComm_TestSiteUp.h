#pragma once
#include "visioncomm.h"

class CVisionComm_TestSiteUp : public CVisionComm
{
public:
	CVisionComm_TestSiteUp(int nVisionPcIdx, CString strVisionName);
	CVisionComm_TestSiteUp(void);
	~CVisionComm_TestSiteUp(void);

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

	LRESULT OnTWIPCEvent(WPARAM wParam, LPARAM lParam);

	int Initialize(CWnd* pParentWnd, char* szDriverFileName);
	void Finalize();
};

