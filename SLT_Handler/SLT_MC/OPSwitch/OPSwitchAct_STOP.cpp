#include "StdAfx.h"
#include "OPSwitchAct_STOP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COPSwitchAct_STOP::COPSwitchAct_STOP(void)
{
	m_bSwitchStatus_Bak = FALSE;
}


COPSwitchAct_STOP::~COPSwitchAct_STOP(void)
{
}

void COPSwitchAct_STOP::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib)
{
	COPSwitchAct::OPSwitchSet(strName, nSw_IO_Addr, nLed_IO_Addr, eOut_Attrib);
	COPSwitchAct::Add_In(DI_REAR_LEFT_STOP_SW_SEN);
	COPSwitchAct::Add_In(DI_REAR_RIGHT_STOP_SW_SEN);
 	COPSwitchAct::Add_Out(DO_REAR_LEFT_STOP_SW_LAMP);
	COPSwitchAct::Add_Out(DO_REAR_RIGHT_STOP_SW_LAMP);
	COPSwitchAct::OutPutLed(DEF_ON);
}

void COPSwitchAct_STOP::Switch_Operation()
{
	switch( m_nOperationStep )
	{
	case 0:
		{
			if( !m_bLockMode )	//Lock되어 있으면 아무것도 하지 않는다.
			{
				if((GetSWStatus()==TRUE) && (m_bSwitchStatus_Bak == FALSE))
				{
					m_nOperationStep = 1000;
				}

				m_bSwitchStatus_Bak = GetSWStatus();
			}
		}break;

	case 1000: // on
		{
			int nErr = g_TaskSystemCtrl.OnPanelButtonStop();
			if( nErr == ERR_NO_ERROR )
			{
				m_nOperationStep = 1001;
			}
			else
			{
				m_nOperationStep = 0;
			}
		}break;
	case 1001:
		{
			SetStatus(TRUE);
			if(!m_bBlinkFlag){
				OutPutLed(1);
			}
			ASSERT(m_pcGroup);
			if(m_pcGroup)	{	//본키가 눌렸을때 꺼져야할 스위치의 포인터
				m_pcGroup->SetStatus(FALSE);
			}

// 			BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
// 			if (bUseNoUse == eOPTION_USE)
// 			{
// 				g_pVisionTestHand_1->Cmd_VisionReady_HandCam(eVisionLotStateEnd);
// 			}
			g_TaskSystemCtrl.MakeLog("Push Stop Switch");
			m_nOperationStep = 0;
		}break;
	}// end of switch
}
