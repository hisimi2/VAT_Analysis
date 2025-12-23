#pragma once

#include "opswitchact.h"

class COPSwitchAct_Lamp : public COPSwitchAct
{
public:
    COPSwitchAct_Lamp(void);
    ~COPSwitchAct_Lamp(void);

    void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink=FALSE, int nLedType=0);

    void Switch_Operation();

    void LAMP_ON();
    void LAMP_OFF();
};

