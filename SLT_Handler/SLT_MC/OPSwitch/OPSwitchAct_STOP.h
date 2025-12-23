#pragma once

#include "opswitchact.h"

class COPSwitchAct_STOP : public COPSwitchAct
{
public:
	COPSwitchAct_STOP(void);
	~COPSwitchAct_STOP(void);

	void COPSwitchAct_STOP::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib);

	void Switch_Operation();
	BOOL m_bSwitchStatus_Bak;
};

