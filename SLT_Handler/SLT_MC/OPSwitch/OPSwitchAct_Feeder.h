#pragma once

#include "opswitchact.h"

class COPSwitchAct_Feeder : public COPSwitchAct
{
public:
	COPSwitchAct_Feeder();
	~COPSwitchAct_Feeder();

	void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink = FALSE, int nLedType = 0);

	void Switch_Operation();
	void SetChangeStatusOff();
	void SetChangeStatusOnOff(BOOL bOnOff);

private:
	CTimerTickCheck m_tmChkPush;
	BOOL	m_bFeederSwitchOnOff;
};

