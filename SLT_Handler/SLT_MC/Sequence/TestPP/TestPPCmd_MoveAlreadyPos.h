#pragma once

//==============================================================================
//
//==============================================================================
#include <vector>


//==============================================================================
//
//==============================================================================
class CBaseSeqCmd;

class CTestPPCmd_MoveAlreadyPos : public CBaseSeqCmd
{
public:
    CTestPPCmd_MoveAlreadyPos(void);
    ~CTestPPCmd_MoveAlreadyPos(void);

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
	int                 m_nCmdStage;

	CPoint              m_cCmdPocket;
	std::vector<CPoint> m_vCmdPicker;
	int					m_nHandType;

	BOOL                m_bXmoveTarget;	// 미디 대기 시 X축까지 할 것인지

	BOOL				m_bAutoRunXHomePos; // Auto Executing Run 진행 시 처음 Home 위치로 대기
    // 제어상수

    // 제어변수
	double           m_tcTimeout;
	double           m_tcTimeoutPicker;
	int              m_nMaxRetryCnt;
	int              m_nRetryCount;

    double           m_dLastPos[CTaskTestPP::eAxisMax];
    double           m_dLastPosErrRange[CTaskTestPP::eAxisMax];
    double           m_dInPosRange[CTaskTestPP::eAxisMax];

private:
    int m_nTestPpIdx;

	CString m_strStageName;

	OneAxis* m_pMotor[ CTaskTestPP::eAxisMax ];

	std::vector<CPoint> m_vErrPicker;
};

