#pragma once

#include "opswitchact.h"

class COPSwitchAct_START : public COPSwitchAct
{
public:
	COPSwitchAct_START(void);
	~COPSwitchAct_START(void);

	void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib);

	void Switch_Operation();

	BOOL m_bSwitchStatus_Bak;

	CTimerTickCheck m_tcMotionDone;

	int m_nBackStep;
	std::vector<int> m_vLockSen;

private:
	int m_nOrgRetryCount;
	CTimerTickCheck m_tcTimeout;

};

