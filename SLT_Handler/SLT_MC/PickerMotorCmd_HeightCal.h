#pragma once

class CPickerMotorCmd_HeightCal :public CBaseSeqCmd
{
public:
	CPickerMotorCmd_HeightCal();
	~CPickerMotorCmd_HeightCal();

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

	int                 m_nCmdStage;
	CPoint              m_CmdPocket;
	std::vector<CPoint> m_vCmdPicker;
	std::vector<CPoint> m_vSpecOutPicker;
	std::vector<CPoint> m_vFullCmdPkr;
	int                 m_nCmdPick;	    // 0=Pick, 1=Place
	BOOL				m_bNormalPkrEnd;
	BOOL				m_bSpeckOutPkrEnd;
	int				    m_nCntSpecOut;
	BOOL				m_bSpecOutPkrUse;

private:
	// 제어상수
	double           m_dWaitTime;        // msec
	BOOL             m_bDownCheckEnable; // 0:Up 상태에서 Vac 점검. 1:Down 상태에서 Vac 점검
	double           m_dDownCheckTimeout;

	double           m_tcTimeout;
	double           m_tcTimeoutPicker;
	int              m_nRetryCount;
	int              m_nSetRetryCount;

	double           m_dInPosRange[CTaskTrayPP::eAxisMax];


	eCheckAbnormalCase check_abnormal_case(int _PickPlace, CString& error_msg);

	CTimerTickCheck	m_TransferInterlock;

	enum eExcuteStep {
		eExcuteStep_Do_Blow = 12000,
	};
};

