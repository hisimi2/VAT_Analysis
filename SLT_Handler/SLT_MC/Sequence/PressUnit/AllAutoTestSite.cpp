#include "StdAfx.h"
#include "AllAutoTestSite.h"


CAllAutoTestSite::CAllAutoTestSite(void)
{
}


CAllAutoTestSite::~CAllAutoTestSite(void)
{
}

int CAllAutoTestSite::GetSeqCmdStatusAllPart()
{
	for( int i=0; i < eMaxTestPPCount; i++ )
	{
		int nState = g_TaskTestPP[i].GetSeqCmdStatus();
		if( nState != CBaseSeqCmd::eSeqCmdStatus_Ready )
			return -1;

		nState = g_TaskLoadTbl[i].GetSeqCmdStatus();
		if( nState != CBaseSeqCmd::eSeqCmdStatus_Ready )
			return -1;
	}

	for( int i=0; i < eMaxPressUnitCount; i++ )
	{
		int nState = g_TaskPressUnit[i].GetSeqCmdStatus();
		if( nState != CBaseSeqCmd::eSeqCmdStatus_Ready )
			return -1;
	}

	return ERR_NO_ERROR;
}

int CAllAutoTestSite::OnLotCommand(eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam)
{
	int nErr = ERR_NO_ERROR;

	for (int i = 0; i<eMaxTestPPCount; i++)
	{
		nErr |= g_TaskAutoTestPp[eTestPP_1 + i].OnLotCommand(eLotCmd, NULL, NULL);
	
	}
	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		nErr |= g_TaskAutoLdTable[eLoadTbl_1 + i].OnLotCommand(eLotCmd, NULL, NULL);
	}
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		nErr |= g_TaskAutoPress[i].OnLotCommand(eLotCmd, NULL, NULL);
	}

	return nErr;
}

