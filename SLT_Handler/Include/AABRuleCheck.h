#pragma once

#include "DEF_DEVICE_DATA.h"
#include "DEF_COMMON.h"
#include "DEF_DB_TBL.h"
#include "DatabaseIF.h"
#include <afxmt.h>
#include <deque>
#include <vector>
#include <stdlib.h>

// VLTD = VIOLATION DATA

enum eAAB_RULE_STEP{
	eAAB_RULE_STEP_1st=0,
	eAAB_RULE_STEP_2st,
	eAAB_RULE_STEP_3st,
};

enum eAAB_TOK_DATA{
	eAAB_TOK_DATA_ALL =0,
	eAAB_TOK_DATA_Bin,
};

class AFX_EXT_CLASS AABRuleCheck : public CWnd
{
public:
	AABRuleCheck(void);
	~AABRuleCheck(void);

	// Initialize & Finalize
	int Initialize(int nTypeART, int nSortType);
	void Finalize();

	// option
	void ChangeRuleType(int nTypeART/*enum eAUTO_RETEST_TEST_PP*/);
	void ChangeSortType(int nSortType);

	// insert 
	void Insert_VLTD(ST_DD_DEVICE stDvcData);
	// delete
	void Delete_VLTD(char *szBarcode);

	// detecting data check
	void Chk_VLTD(ST_AAB_CHECK_DATA stVLTD);
	
	// check complete data clear
	void CompDataClear(char *szBarcode);

	// occurd data push db
	void Insert_VLTDtoDB(ST_AAB_CHECK_DATA stVLTD);
	void GetVLTD_FromDB(std::deque<ST_AAB_CHECK_DATA> &qDvcData);

	// Process req data count & barcode
	void ReqVLTD(std::deque<ST_AAB_CHECK_DATA> &qDvcData);

	// recovery data
	void RecoveryDB();

	// all data delete, deque, db
	void DeleteAllData();

	// 문자 자르기
	void TokData_RuleChk(int nTokDataType, CString &strTokSit, CString &strTokErCode, CString &strTokBin, int nIndex, ST_AAB_CHECK_DATA stVLTD);
	void TokData_DataChk(CString &strSortType, int nIndex, ST_AAB_CHECK_DATA stVLTD);
	
	CString TestCount2_CHK(ST_AAB_CHECK_DATA stVLTD);
	CString TestCount3_CHK(ST_AAB_CHECK_DATA stVLTD);

	BOOL Data_CHK(ST_AAB_CHECK_DATA stVLTD);
private:
	CCriticalSection m_cs;

	BOOL        m_bRunStop;
	CWinThread*	m_pThread;
	static UINT ProcVLTD(LPVOID lpVoid);
	
	CDatabaseIF m_DB;

	std::deque<ST_AAB_CHECK_DATA> m_qData;

	CString m_strRemark;

	int m_nTypeART;
	int m_nSortType;	
	int m_nFirstBin;
	int m_nSecondBin;
	
// 	void InsertDvcDataDB();
// 	int m_nAutoRetestKind;
// 	std::deque<ST_DD_DEVICE> m_qDvcData;
// 
// 	BOOL m_bStopRuleCheck;
// 	BOOL m_bStopInsertDB;
// 	BOOL m_bCsIsLock;
// 
// 	CDatabaseIF m_DBAABRule; // add by SJI [160919] Add AAB Retest Rule Check
};

 