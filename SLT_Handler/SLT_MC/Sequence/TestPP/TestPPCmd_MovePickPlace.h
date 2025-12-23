#pragma once
#include "..\Dll_Src\LIB_IPC_TCP\LIB_IPC_SOCK\PerformanceTime.h"

class CBaseSeqCmd;

//==============================================================================
// xyz축은 Pick을 할 수 있는 위치에 있는 상태에서 pick 동작만 수행
//==============================================================================
class CTestPPCmd_MovePickPlace : public CBaseSeqCmd
{
public:
	CTestPPCmd_MovePickPlace(void);
	~CTestPPCmd_MovePickPlace(void);

	int CheckParam();

	int  OnStop();
	int  OnRetry();
	int  OnSkip();
	int  OnPause();
	int  OnResume();

	enum eVibratorStep
	{
		do_Vibrator_On = 25000,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		Vibrator_complete,
	};

	CPerformanceTime tmr;

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
    int              m_nCmdStage;
	CPoint			 m_cCmdPocket;
	std::vector<CPoint> m_vCmdPicker;
	int              m_nCmdPick;	    // 0=Pick, 1=Place
	//int              m_nCmdHandType;	    // 0=LoadHand, 1=UnlondHand

	int				 m_cCmdManualPickPlace;
	int				 m_nHandType;		//// 0=LoadHand, 1=UnlondHand
	BOOL			 m_bAlreadyPick;
	// 제어상수
	int              m_nMaxRetryCnt;

	int              m_nVacuumWait;
	int              m_nBlowWait;
	int				 m_nBackVibratorStep;
	int				 m_nVibCurTryCnt;

	BOOL			 m_bVib_Retry_Complete;

	BOOL			 m_bCleanOffsetUse;

	// 제어변수
	double           m_tcTimeout;
	double           m_tcTimeoutPicker;
	int              m_nRetryCount;
	
	double           m_dLastPos[CTaskTestPP::eAxisMax];
	double           m_dLastPosErrRange[CTaskTestPP::eAxisMax];
	double           m_dInPosRange[CTaskTestPP::eAxisMax];	



private:
	_tVisionMsg m_tVisionMsgMeasure;
	_tVisionMsg m_tVisionMsgMeasureResult;

	OneAxis* m_pMotor[ CTaskTestPP::eAxisMax ];

	int m_nTestPpIdx;
	int m_nPressIdx;

	CString m_strWorkPkrName;
	CString m_strStageName;
	CString m_strPickPlace;
	BOOL m_bVisionErrSkip[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];

	std::vector<CPoint> m_vErrPicker;

	int	m_nPickRetry;
	CParamList m_BackAlarmData;
	
};

