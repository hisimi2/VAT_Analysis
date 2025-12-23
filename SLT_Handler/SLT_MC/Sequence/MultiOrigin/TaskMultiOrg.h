#pragma once

class CBaseTaskManager;

class CTaskMultiOrg : public CBaseTaskManager
{
public:
	CTaskMultiOrg(void);
	~CTaskMultiOrg(void);

	int Initialize(char* szDriverFileName);
	void Finalize();

	void ThreadProc1();

	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
};

