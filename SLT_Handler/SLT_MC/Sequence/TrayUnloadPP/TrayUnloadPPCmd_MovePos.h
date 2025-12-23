#pragma once
class CBaseSeqCmd;

class CTrayUnloadPPCmd_MovePos_Param
{
public:
	CTrayUnloadPPCmd_MovePos_Param();
	CTrayUnloadPPCmd_MovePos_Param(const CTrayUnloadPPCmd_MovePos_Param& src);
	CTrayUnloadPPCmd_MovePos_Param& operator=(const CTrayUnloadPPCmd_MovePos_Param& src);
	CTrayUnloadPPCmd_MovePos_Param(
		eLocIdxTrayPP           _eStage,
		CPoint                  _Pocket,
		std::vector<CPoint>     _vPicker
	);
	~CTrayUnloadPPCmd_MovePos_Param();

	eLocIdxTrayPP           eStage;
	CPoint                  Pocket;
	std::vector<CPoint>     vPicker;
};

class CTrayUnloadPPCmd_MovePos : public CBaseSeqCmd
{
public:
	CTrayUnloadPPCmd_MovePos();
	CTrayUnloadPPCmd_MovePos( CTrayUnloadPPCmd_MovePos_Param _Prm );
	~CTrayUnloadPPCmd_MovePos( void );

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

	// Command Parameters
	CTrayUnloadPPCmd_MovePos_Param m_Cmd;

	CTimerTickCheck         m_tcTimeout;
	CTimerTickCheck         m_tcTimeoutPicker;
	int                     m_nRetryCount;

private:
	CTimerTickCheck	        m_TransferInterlock;

	std::vector<CPoint>		m_vErrPicker;

};

