#pragma once

#include "opswitchact.h"

class COPSwitchAct_Cover : public COPSwitchAct
{
public:
	COPSwitchAct_Cover(void);
	~COPSwitchAct_Cover(void);
	
	void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink=FALSE, int nLedType=0);

	void Switch_Operation();

	void Cover_Lock();
	void Cover_Unlock();

	void Switch_CoverSleep(int nSleep);

	void Initialize();

	std::vector<int> m_vLockSen;
	std::vector<int> m_vLockOut;
};

