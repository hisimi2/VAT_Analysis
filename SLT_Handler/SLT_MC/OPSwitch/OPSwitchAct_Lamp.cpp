#include "StdAfx.h"
#include "OPSwitchAct_Lamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COPSwitchAct_Lamp::COPSwitchAct_Lamp(void)
{
}


COPSwitchAct_Lamp::~COPSwitchAct_Lamp(void)
{
}


void COPSwitchAct_Lamp::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink/*=FALSE*/, int nLedType/*=0*/)
{
    COPSwitchAct::OPSwitchSet(strName, nSw_IO_Addr, nLed_IO_Addr, eOut_Attrib, bBlink, nLedType);
	COPSwitchAct::Add_In(DI_REAR_LEFT_LAMP_SW_SEN);
	COPSwitchAct::Add_In(DI_REAR_RIGHT_LAMP_SW_SEN);
	COPSwitchAct::Add_Out(DO_REAR_LEFT_LAMP_SW_LAMP);
	COPSwitchAct::Add_Out(DO_REAR_RIGHT_LAMP_SW_LAMP);
}

void COPSwitchAct_Lamp::Switch_Operation()
{
    switch( m_nOperationStep )
    {
    case 0:
        {
//             if( !m_bLockMode )	//Lock되어 있으면 아무것도 하지 않는다.
//             {
                if(GetSWStatus())
                {
                    if( m_bTemp_key == FALSE )
                    {
                        m_bTemp_key = TRUE;
                        if( !GetStatus())
                        {
                            m_bTemp_Memo_Flag = TRUE;
                            m_nOperationStep = 1000;
                        }
                        else
                        {
                            m_bTemp_Memo_Flag = FALSE;
                            m_nOperationStep = 2000;
                        }
                    }
                }
                else
                {
                    m_bTemp_key = FALSE;
                }
//            }
        }break;

    case 1000: // STACKER ON
        {
            m_nOperationStep = 1001;
        }break;

    case 1001:
        {
            m_nOperationStep = 1002;
        }break;

    case 1002:
        {
 			g_IO.out(DO_FRONT_LAMP_ON, DEF_ON);
			g_IO.out(DO_LED_LIGHT_ON, DEF_ON);
            SetStatus(DEF_ON);
            m_nOperationStep = 0;
        }break;

    case 2000: // STACKER OFF // 시정
        {	
/*			if( g_START_SW.GetStatus() != DEF_ON ){*/
				g_IO.out(DO_FRONT_LAMP_ON, DEF_OFF);
				g_IO.out(DO_LED_LIGHT_ON, DEF_OFF);
				SetStatus(DEF_OFF);
/*			} */			
            m_nOperationStep = 0;
        }break;
    }
}

void COPSwitchAct_Lamp::LAMP_ON()
{
    m_nOperationStep = 1000;
}
void COPSwitchAct_Lamp::LAMP_OFF()
{
    m_nOperationStep = 2000;
}
