#pragma once

#include "../VisionAutoTeaching/Simul/VATSimul.h"

#define BEGIN_CASE_TO_STR(act) switch(act){
#define CASE_TO_STR(step_n) case step_n: return _T(#step_n)
#define ELSE_CASE(str) default: return _T(str);
#define END_CASE_TO_STR() }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 


namespace base_seq_cmd
{
	enum eProcessCommand{ 
		eProcCmd_Pause = 0, 
		eProcCmd_Resume, 
		eProcCmd_Skip, 
		eProcCmd_Retry, 
		eProcCmd_Stop,
		eProcCmd_Cancel,
		eProcCmd_OK,
		eProcCmd_CleanOut,
	};

	enum eCmdExecutorStatus{ 
		eCmdExecutorStatus_Idle=0, 
		eCmdExecutorStatus_Executing, 
		eCmdExecutorStatus_Complete
	};

	enum eSeqCmdEndState{
		_seq_cmd_not_end_ = 0,
		_seq_cmd_end_     = 1,
	};
}

//==============================================================================
//
//==============================================================================
class CBaseTaskManager;
class CParamList;


//==============================================================================
//
//==============================================================================
class CBaseSeqCmd
{
public:
	CBaseSeqCmd(void);
	CBaseSeqCmd(const CBaseSeqCmd& src);
	virtual ~CBaseSeqCmd(void);

public:
	enum eSeqCmdStatus {
		eSeqCmdStatus_Ready=0, 
		eSeqCmdStatus_Queue,
		eSeqCmdStatus_Setup, 
		eSeqCmdStatus_Executing, 
		eSeqCmdStatus_Pause, 
		eSeqCmdStatus_Recovery, 
		eSeqCmdStatus_NormalComplete, 
		eSeqCmdStatus_Canceled, 
		eSeqCmdStatus_Alarm, 
		eSeqCmdStatus_AlarmComplete, 
		eSeqCmdStatus_Aborting, 
		eSeqCmdStatus_AbortComplete
	};

public:
	CBaseSeqCmd& operator = (const CBaseSeqCmd& src);

	virtual int CheckParam() = 0;

	BOOL Running();	
	void ReportCmdEnd(eSeqCmdStatus a_eEndCode = eSeqCmdStatus_NormalComplete);
	void ReportAlarm( int nAlid, CParamList AlarmData, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>() );

	int GetCmdStatus();
	int SetCmdStatus(eSeqCmdStatus a_eNewStatus, int nNewStep=0);
	int GetLastError();
	TCHAR*	GetCmdName();
	int GetWorkCmdStage();

	static void MakeCmdSerial(CBaseSeqCmd* pSelf);

	// Stop Command를 받았을 때 호출 됩니다.
	virtual int  OnStop()           {return 0;};
	// Retry Command를 받았을 때 호출 됩니다.
	virtual int  OnRetry()          {return 0;};
	// Skip Command를 받았을 때 호출 됩니다.
	virtual int  OnSkip()           {return 0;};
	// PAUSE Command를 받았을 때 호출 됩니다.
	virtual int  OnPause()          {return 0;};
	// RESUME Command를 받았을 때 호출 됩니다.
	virtual int  OnResume()         {return 0;};

	/*
	* 명령의 상태에 해당 될 때 함수들이 호출 됩니다. 
	* 함수들은 상태가 바꾸거나, return 값을 TRUE로 하기 전까지 계속 호출 됩니다.
	* Retur의 의미는 명령의 종료 입니다. CBaseTaskManager에서 호출 함.
	*/
	virtual BOOL State_Setup();
	virtual BOOL State_Executing();
	virtual BOOL State_Recovery();
	virtual BOOL State_Pause();
	virtual BOOL State_NormalComplete();
	virtual BOOL State_Alarm();
	virtual BOOL State_AlarmComplete();
	virtual BOOL State_Aborting();
	virtual BOOL State_AbortComplete();

	// 명령을 보낸 Task의 정보
	CBaseTaskManager*  m_pSender;
	// 명령을 수신한 Task의 정보
	CBaseTaskManager*  m_pReceive;

	// 명령의 일련번호
	int                m_nSerialNo;
	// 명령의 고유 ID
	int                m_nCmdId;
	// 명령 실행을 위한 Parameters
	CParamList         m_ParamList;

	// Last Errr Data
	int                m_nLastError;		// ex> ERR_STACKER_CLAMP_TO
	CParamList         m_LastErrorData;	// ex> "LOC=STACKER1, Set Timeout=1000"

	int                m_nCmdSeqMode;		// enum eSeqMode

protected:
	eSeqCmdStatus      m_eCmdStatus;  // enum eSeqCmdStatus 
	int                m_nStep;
	int                m_nStep_New;   // step을 변경 하기 위한 Step 번호를 저장해 둔다. Step을 변경 한 후에는 -1로 초기화 한다.
	int                m_nStep_Pre;   // 바로 이전 제어 주기에 실행 하였던 값을 저장 해 둔다.
	char			   m_szCmdName[128];
	int                m_nLast_Executing_Step;
	CCriticalSection   m_csSeqCmdStatus;

	int                m_nWorkCmdStage;

//VAT CMD 관련 함수
	VATSimul m_simulCtrl;
// public:
// 	//Simulation
// 	void _Simul_SeqControl(int control_time);
// 	BOOL _Simul_IsRemainSeqCycle();
// 	void _Simul_ClearParam();
// 
// 	BOOL m_bIsSimul;
// 	int m_nActionCnt;

};
