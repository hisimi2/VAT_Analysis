#pragma once

#include "opswitchact.h"

class COPSwitchAct_StepTestPP: public COPSwitchAct
{
public:
	COPSwitchAct_StepTestPP(void);
	~COPSwitchAct_StepTestPP(void);

	void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink=FALSE, int nLedType=0, int nIdx = -1);

	void Switch_Operation();
	void SetChangeStatusOff();
	void SetChangeStatusOnOff(BOOL bOnOff);
	BOOL m_bOperationTestPpStepLedOnoff;
	BOOL m_bOperationTestPpStepSwitchingLedOnoff;

	CWinThread* m_pTestPpStepThread;
private:
	CTimerTickCheck m_tmChkPush;
	int m_nTestPPIdx;
};

