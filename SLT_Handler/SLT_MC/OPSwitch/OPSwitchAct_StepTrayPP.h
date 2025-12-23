#pragma once

#include "opswitchact.h"

class COPSwitchAct_StepTrayPP: public COPSwitchAct
{
public:
	COPSwitchAct_StepTrayPP(void);
	~COPSwitchAct_StepTrayPP(void);

	void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink=FALSE, int nLedType=0);

	void Switch_Operation();
	void SetChangeStatusOff();
	void SetChangeStatusOnOff(BOOL bOnOff);
	BOOL m_bOperationTrayPpStepLedOnoff;
	BOOL m_bOperationTrayppStepSwitchingLedOnoff;

	CWinThread* m_pTrayPpStepThread;
private:
	CTimerTickCheck m_tmChkPush;
};

