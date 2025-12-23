#pragma once
class CVAT_TaskAutoSystem : public CBaseTaskManager, public CBaseAutoSeqManager
{
public:
	CVAT_TaskAutoSystem();
	~CVAT_TaskAutoSystem();

	int Initialize(/*HWND hOwner*/);
	void Finalize();

	int OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList = NULL);
	int OnCommandRsp(CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>());
	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	void ThreadProc1();

	void MakeLog(LPCTSTR fmt, ...);

	//Auto Run Mode
	void AutoState_InitNeed();
	void AutoState_Initialize();
	void AutoState_Idle();
	
	void AutoState_Execute();
	void AutoState_Pause();

	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();

	void AutoState_CleanOut();

	enum _eExcutingStep {
		eExcutingStep_Vat_Start = 0,
		eExcutingStep_Vat_Measure_Wait,
		eExcutingStep_Vat_Data_Save,
		eExcutingStep_Vat_Data_Save_Wait,
		eExcutingStep_Vat_End,
	};



public:
	//HWND m_hVatMainGui;
	std::vector< CBaseAutoSeqManager* > m_vVatAllAutoTasks;
	int m_nDataSaveCase;
	
	int m_nCurModeType;
};

