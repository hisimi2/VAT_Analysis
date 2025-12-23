#include "StdAfx.h"
#include "OPSwitchAct_StepTestPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT OpStepTestPpSwitchThreadProc(LPVOID lpVoid);

COPSwitchAct_StepTestPP::COPSwitchAct_StepTestPP(void)
{
	m_nTestPPIdx = -1;
	m_bOperationTestPpStepLedOnoff = FALSE;
	m_bOperationTestPpStepSwitchingLedOnoff = FALSE;
	m_pTestPpStepThread = NULL;
}


COPSwitchAct_StepTestPP::~COPSwitchAct_StepTestPP(void)
{
	if (m_pTestPpStepThread->m_hThread != NULL)
		WaitForSingleObject(m_pTestPpStepThread->m_hThread, NULL);
}

void COPSwitchAct_StepTestPP::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink/*=FALSE*/, int nLedType/*=0*/, int nINdexNum)
{
	m_nTestPPIdx = nINdexNum;
	COPSwitchAct::OPSwitchSet(strName, nSw_IO_Addr, nLed_IO_Addr, eOut_Attrib, bBlink, nLedType);
	m_pTestPpStepThread = AfxBeginThread(OpStepTestPpSwitchThreadProc, this);
}

void COPSwitchAct_StepTestPP::Switch_Operation()
{
	switch( m_nOperationStep )
	{
	case 0:
		{
			if( !m_bLockMode )	//Lock되어 있으면 아무것도 하지 않는다.
			{
				int nChkOpSwitch = 0;
				for(int i=0; i < eMaxTestPPCount; i++){
					if( g_StepTestPP_SW[i].GetStatus() == DEF_ON && i != m_nTestPPIdx){
						nChkOpSwitch++;
					}
				}

				if (g_StepTrayPP_SW[eTrayPP_1_LD].GetStatus() == DEF_ON || g_StepTrayPP_SW[eTrayPP_2_UD].GetStatus() == DEF_ON)
					nChkOpSwitch++;			

				//BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
				//if( bAutoGrrUse == TRUE ){
				//	nChkOpSwitch++;
				//}

				if(GetSWStatus())
				{
					if( m_bTemp_key == FALSE )
					{
						m_bTemp_key = TRUE;
						if( !GetStatus() && nChkOpSwitch == 0)
						{
							if( m_tmChkPush.IsChecking() == FALSE ){
								m_tmChkPush.SetCheckTime(0.05);
								m_tmChkPush.StartTimer();
							}
						}
						else
						{
							m_tmChkPush.SetTimerCheckEnd();
							m_bTemp_Memo_Flag = FALSE;
							m_nOperationStep = 2000;
						}
					}else{
						if( m_tmChkPush.IsChecking() == TRUE && m_tmChkPush.CheckOverTime() == TRUE && GetStatus() != DEF_ON && nChkOpSwitch == 0){
							m_bTemp_Memo_Flag = TRUE;
							m_nOperationStep = 1000;
						}
					}
				}
				else
				{
					m_bTemp_key = FALSE;
				}
			}
		}break;

	case 1000: // STEP FUNCTION ON 
		{
			SetChangeStatusOnOff(DEF_OFF);
			Sleep(100);
			SetStatus(DEF_ON);
			g_TaskSystemCtrl.MakeLog("Push Step Test PP [ON]");
			m_nOperationStep = 0;
		}break;

	case 2000: // STEP FUNCTION OFF
		{	
			SetChangeStatusOnOff(DEF_OFF);
			Sleep(100);
            SetStatus(DEF_OFF);
            g_TaskSystemCtrl.MakeLog("Push Step Test PP [Off]");
            m_nOperationStep = 0;
		}break;
	}
}

void COPSwitchAct_StepTestPP::SetChangeStatusOff()
{
	SetStatus(DEF_OFF);
	m_tmChkPush.SetTimerCheckEnd();
}

void COPSwitchAct_StepTestPP::SetChangeStatusOnOff(BOOL bOnOff)
{
	m_bOperationTestPpStepLedOnoff = bOnOff;

	if (bOnOff == DEF_OFF)
	{
		SwitchLEDOnOff(DEF_OFF);
	}
}
UINT OpStepTestPpSwitchThreadProc(LPVOID lpVoid)
{
	COPSwitchAct_StepTestPP* pSelf = (COPSwitchAct_StepTestPP*)lpVoid;

	while (1) {
		if (pSelf->m_bOperationTestPpStepLedOnoff == TRUE) {
			if (pSelf->m_bOperationTestPpStepSwitchingLedOnoff == DEF_ON) {
				pSelf->SwitchLEDOnOff(DEF_ON);
				pSelf->m_bOperationTestPpStepSwitchingLedOnoff = FALSE;
			}
			else {
				pSelf->SwitchLEDOnOff(DEF_OFF);
				pSelf->m_bOperationTestPpStepSwitchingLedOnoff = TRUE;
			}
		}
		Sleep(300);
	}
	return 0;
}