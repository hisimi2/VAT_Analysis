#pragma once
#include "BaseSeqCmd.h"

class CSystemCmd_AxisJogMove : public CBaseSeqCmd
{
public:
	CSystemCmd_AxisJogMove(int nAxisIdx, int nDir, double dSpeed/*um/s*/);
	~CSystemCmd_AxisJogMove();

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

	void ClearMotorCmd();
private:
	int m_nAxisIdx; //eAxisNo
	int m_dir;	// Negative = -1, Positive = 1;
	double m_spd_ums;	// /*um/s*/

	CTimerTickCheck m_tcTimeout;
};

