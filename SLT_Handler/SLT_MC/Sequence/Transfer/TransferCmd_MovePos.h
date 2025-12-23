#pragma once

class CTransferCmd_MovePos : public CBaseSeqCmd
{	
public:
	CTransferCmd_MovePos( eLocIdxTransfer eTeachPos, int nSpeedRate = 100);
	~CTransferCmd_MovePos();

	int CheckParam();

	int  OnStop();
	int  OnRetry();
	int  OnSkip();
	int  OnPause();
	int  OnResume();

	BOOL State_Setup();
	BOOL State_Executing();
	BOOL State_Pause();
	BOOL State_Recovery();
	BOOL State_NormalComplete();
	BOOL State_Alarm();
	BOOL State_AlarmComplete();
	BOOL State_Aborting();
	BOOL State_AbortComplete();

	CTimerTickCheck	m_swTimeout;

private:
	int m_nLast_Do_Step;

	eLocIdxTransfer			m_eTeachPos;		// 이동할려고 하는 Teach Pos
	int m_nSpdRate;

	CTimerTickCheck	m_TrayInterlock;
};


