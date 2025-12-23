#pragma once

class CBaseTaskManager;

class CTaskTowerLight : public CBaseTaskManager
{
public:
	CTaskTowerLight(void);
	~CTaskTowerLight(void);

	int Initialize(char* szDriverFileName);
	void Finalize();

	void ThreadProc1();

	int OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType=eEqpCtrlMode_Auto);
	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

};

