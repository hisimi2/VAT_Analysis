#pragma once

class CBaseTaskManager;

class CTestHandWorkArea
{
public:
	CTestHandWorkArea(void);
	~CTestHandWorkArea(void);

private:
	bool m_bWorking[eMaxPressUnitCount];
	int  m_nTestIdx;
	std::vector<int>  m_veLoadTableIdx;
	CCriticalSection m_cs;
	CBaseTaskManager* m_reserved_task[eMaxPressUnitCount];

public:
	void SetTestPpIdx(int nIdx);
	void SetLoadTableIdx(int nIdx);
	bool is_work_enable_state(int nPressIdx, CBaseTaskManager* _this);
	void release_work_area(int nPressIdx, CBaseTaskManager* _this);
	bool reserve_work_area(int nPressIdx, CBaseTaskManager* _this);
	bool get_reserved_task(int nPressIdx, CBaseTaskManager* _this);
};

