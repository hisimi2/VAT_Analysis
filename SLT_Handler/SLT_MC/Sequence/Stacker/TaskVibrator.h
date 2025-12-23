#pragma once

class CBaseTaskManager;

class CTaskVibrator : public CBaseTaskManager
{
public:
	CTaskVibrator(void);
	~CTaskVibrator(void);

	int Initialize(char* szDriverFileName);
	void Finalize();

	void ThreadProc1();

	int OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType=eEqpCtrlMode_Auto);
	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);


	// yVIBRATOR_8 (8bit) : 0 = action mode      1 = set mode
	// yVIBRATOR_7 (7bit) : 0 = position         1 = level
	// yVIBRATOR_6 (6bit) : 0 = not save         1 = save
	// DATA 0 ~ 4(5bit) : position or level
	int do_Vibration(int nCh, BOOL bOnOff);
	int do_SetIntensity(int nCh, int nValue);
	int GetIntensity();
	int IsAction(int nCh, BOOL bOnOff);

private:
	std::vector<int> m_vX;           // DI id list
	std::vector<int> m_vY;           // DO id list
	CMap<int,int,CString,CString> m_mapDI;
	CMap<int,int,CString,CString> m_mapDO;
	int m_nCurIntensity;

public:
	enum eDI{
		xVIBRATOR_SEN1                 = 0,
		xVIBRATOR_SEN2,
		xVIBRATOR_SEN3,
		xVIBRATOR_SEN4,
		xMAX_COUNT,
	};
	enum eDO{
		yVIBRATOR_1   = 0,
		yVIBRATOR_2,
		yVIBRATOR_3,
		yVIBRATOR_4,
		yVIBRATOR_5,
		yVIBRATOR_6,
		yVIBRATOR_7,
		yVIBRATOR_8,
		yMAX_COUNT
	};

    void MakeLog(LPCTSTR fmt, ...);
};

