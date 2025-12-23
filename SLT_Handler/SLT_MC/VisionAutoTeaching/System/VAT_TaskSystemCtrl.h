#pragma once

#include "VatBaseTask.h"

class CBaseTaskManager;

class CVAT_TaskSystemCtrl : public CBaseTaskManager
{
public:
	CVAT_TaskSystemCtrl();
	~CVAT_TaskSystemCtrl();

private:
	CCriticalSection m_cs;
	CCriticalSection m_csTblOwner;
	CCriticalSection m_csCTrayJigOwner;
	CCriticalSection m_csTestPpOwner;
	int m_reserved_table[VAT_TABLE_OWNER_MAX]; //All Hand
	int m_reserved_test_pp[eMaxTestPPCount];
	int m_reserved_tray_jig[2]; //Tray Load Pp, Tray Unload Pp

	HWND m_hVatMainPage;
public:
	int Initialize(/*HWND hOwner*/);
	void Finalize();

	void ThreadProc1();

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
public:
	int SetLotCommand(eAutoLotCmd eLotCmd);

	void SetSeqCurMode(int nMode);
	int GetSeqCurMode();
	int GetSeqCurModeType();

	//Table Reserve 
	bool reserve_tbl_work_hand(int table_idx, int nHand);
	bool release_tbl_work_hand(int table_idx, int nHand);
	bool get_tbl_reserved_hand(int table_idx, int nHand);

	//C-Tray Jig Reserve
	bool reserve_tray_jig_work_hand(int table_idx, int nHand);
	bool release_tray_jig_work_hand(int table_idx, int nHand);
	bool get_tray_jig_reserved_hand(int table_idx, int nHand);

	//Test PP Reserve
	void reserve_test_pp_work_hand(int nTestPpIdx, int nType);
	//void release_test_pp_work_hand(int nTestPpIdx);
	int get_test_pp_work_hand(int nTestPpIdx);
	void ReserveTestPpWorkIfSelected(int nTestPpIdx);
	BOOL IsLoadTestPpAlreadyDone(int nTestPpIdx);



//GUI Notify
public:
	void ClearMainGuiMeasureStatus();

//Interlock
public:
	int CheckEqpCondition(CString &strErrAxis);
	
	void SetVatMainPageHandle(HWND hOwner);
	HWND GetVatMainPageHandle();

//TaskSystemCtrl에서 모든 Auto Parameter 관리.
public:
	std::vector<CVatBaseTask*> m_vAllTaskAuto;

	//Hand Type
	void ClearAllMeasureHand();
	void SetMeasureHandToEachAuto(int nHand, BOOL bIsSelected);

	//Picker Type
	void ClearAllMeasurePicker();
	void SetMeasurePickerToEachAuto(int nHand, int nPicker, BOOL bIsSelected);

	//Part Type
	void ClearAllMeasurePart();
	void SetMeasurePartToEachAuto(int nHand, int nPart, BOOL bIsSelected);
};

