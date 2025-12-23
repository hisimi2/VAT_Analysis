#pragma once

class CBaseSeqCmd;

class CTrayUnloadPPCmd_MovePickPlace_Param
{
public:
	CTrayUnloadPPCmd_MovePickPlace_Param();
	CTrayUnloadPPCmd_MovePickPlace_Param(const CTrayUnloadPPCmd_MovePickPlace_Param& src);
	CTrayUnloadPPCmd_MovePickPlace_Param& operator=(const CTrayUnloadPPCmd_MovePickPlace_Param& src);
	CTrayUnloadPPCmd_MovePickPlace_Param(
		eLocIdxTrayPP       _eStage,
		CPoint              _Pocket,
		std::vector<CPoint> _vPicker,
		std::vector<CPoint> _vSpecOutPicker,
		std::vector<CPoint> _vFullCmdPkr,
		ePPCmd              _ePick,	    // 0=Pick, 1=Place
		BOOL				_bNormalPkrEnd,
		BOOL				_bSpeckOutPkrEnd,
		int				    _nCntSpecOut,
		BOOL				_bSpecOutPkrUse,
		BOOL				_bVacuumCheckFuction
	);
	~CTrayUnloadPPCmd_MovePickPlace_Param();

	eLocIdxTrayPP       eStage;
	CPoint              Pocket;
	std::vector<CPoint> vPicker;
	std::vector<CPoint> vSpecOutPicker;
	std::vector<CPoint> vFullCmdPkr;
	ePPCmd              ePickPlace;	    // 0=Pick, 1=Place
	BOOL				bNormalPkrEnd;
	BOOL				bSpeckOutPkrEnd;
	int				    nCntSpecOut;
	BOOL				bSpecOutPkrUse;
	BOOL				bVacuumCheckFuction;

};

class CTrayUnloadPPCmd_MovePickPlace : public CBaseSeqCmd
{
public:
	CTrayUnloadPPCmd_MovePickPlace();
	CTrayUnloadPPCmd_MovePickPlace( CTrayUnloadPPCmd_MovePickPlace_Param _Prm );
	~CTrayUnloadPPCmd_MovePickPlace( void );

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
	CTrayUnloadPPCmd_MovePickPlace_Param m_Cmd;

private:
	// 제어상수
	double           m_dWaitTime;        // msec
	BOOL             m_bDownCheckEnable; // 0:Up 상태에서 Vac 점검. 1:Down 상태에서 Vac 점검
	double           m_dDownCheckTimeout;

	double           m_tcTimeout;
	double           m_tcTimeoutPicker;
	int              m_nRetryCount;
	int              m_nSetRetryCount;

	double           m_dInPosRange[CTaskTrayBasePP::eAxisMax];


	eCheckAbnormalCase check_abnormal_case(int _PickPlace, CString& error_msg);

	CTimerTickCheck	m_TransferInterlock;

	std::vector<CPoint> m_vErrPicker;

	enum eExcuteStep {
		eExcuteStep_Do_Blow = 12000,
		eExcuteStep_Table_Vacuum_Check =25000,
	};
};