#pragma once
#include "..\Dll_Src\LIB_IPC_TCP\LIB_IPC_SOCK\PerformanceTime.h"

class CBaseSeqCmd;
class CLoadTblCmd_MovePos :	public CBaseSeqCmd
{
public:
	CLoadTblCmd_MovePos(void);
	~CLoadTblCmd_MovePos(void);

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

	BOOL ChkErrArrivedSen(CString& strErr, std::vector<CPoint>& vFailPocket, std::vector<CString>& vErrHistor);

	void InvertIdxCurPosToTeachPos(int nCurPos, int &nCmdStage);

	int			    m_nCmdStage;// 0 : LOC_IDX_LD_TBL_LD_SIDE  1 :LOC_IDX_LD_TBL_TEST_SIDE
	int				m_nMoveStep;
	//int				m_nStepRow;

	CTimerTickCheck m_tcTimeout;
	int             m_nRetryCount;
	_tVisionMsg     m_tVisionMsgMeasure;
	_tVisionMsg     m_tVisionMsgMeasureResult;

	int m_nVibCurTryCnt;

	int	m_nBackVibratorStep;


	CPerformanceTime tmr;

	enum eSeqStep
	{
		_eSeqStep_Move_Load_Site_do_move_rotate_dir_test_side = 13000,
		_eSeqStep_Move_Load_Site_chk_rotate_dir_test_side,

	};


	enum eImpactStep
	{
		_eSeqStep_Impact_Move_CurPos			= 30000,
		_eSeqStep_Impact_Move_CurPos_Axisdone,
		_eSeqStep_Impact_Move_First_Pos,
		_eSeqStep_Impact_Move_First_Pos_Axisdone,
		_eSeqStep_Impact_Move_Second_Pos,
		_eSeqStep_Impact_Move_Second_Pos_Axisdone,
	};

	enum eVibratorStep
	{
		do_Vibrator_On = 35000,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		Vibrator_complete,
	};

private:
    int m_nTableIdx;
	int m_nImpactMoveCnt;
	std::vector<ST_2D_BARCODE_INFO> m_v2DBARCODE_FOR_CANCEL;
	//BOOL m_bRetryVision;
	std::vector<CPoint> m_v2D_DUP_COORDINATE;
	std::vector<CPoint> m_v2DErrorData;

	std::vector<CPoint> m_v2DMarkErrorData; // 2D Mark Error

	BOOL m_bIsBarcodeDup;

	int m_nPocketCntY;
	int m_nPocketCntX;

	int m_nBarcodeMaxRetry;
};

