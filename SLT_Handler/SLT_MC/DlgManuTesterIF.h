#pragma once

#include "DlgTesterIF_TesterStatus.h"
#include "../Include/OcxFiles/spreadsheet.h"
#include "../Include/OcxFiles/dsspanel.h"
#include "afxwin.h"
#include "EditTrans.h"
#include "../Include\OcxFiles\spreadsheet.h"

// CDlgManuTesterIF 대화 상자입니다.
class CBaseTaskManager;

class CDlgManuTesterIF : public CDialogEx, public CBaseTaskManager
{
	DECLARE_DYNAMIC(CDlgManuTesterIF)

private:
	CDlgTesterIF_TesterStatus* m_pDlgTesterStatus;
	int m_nSelectedStationID;
	int m_nMaxTesterCount;
	int m_nTesterOption;
private:
	void UpdateSelectedTester();
	void InitWndTesterStatus();
	void Init_Combobox_SetTestMode();
	void UserInterFaceInit();
public:
	CDlgManuTesterIF(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuTesterIF();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_TESTER_IF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonActionSend();
	int m_nSelectedAction;
	DECLARE_EVENTSINK_MAP()
	void ClickSpreadTesterSelectDn(long Col, long Row);
	void ClickSpreadTesterSelectUp(long Col, long Row);
	//CSpreadSheet m_spreadTesterSelect;
	CSpreadSheet m_spreadTesterSelectDn;
	CSpreadSheet m_spreadTesterSelectUp;




	CSpreadSheet* m_pSpreadTesterInfo;
	
	CSpreadSheet m_spreadTesterInfo;
	CSpreadSheet m_spreadTesterInfoSingle;


	CString GetActionName(int nSelectedAction);
	void MakeLog(LPCTSTR fmt, ...);

	//int m_nStationID;
	CSpreadSheet m_spreadTester2DID_Front;
	CSpreadSheet m_spreadTester2DID_Rear;

	CComboBox m_combo_SetTestMode;
	CComboBox m_combo_All_Station_TestMode;

	afx_msg void OnBnClickedButtonActionSendAllStation();
	afx_msg void OnBnClickedButtonManuStratAction();
	afx_msg void OnBnClickedButtonManuStopAction();

	BOOL m_bManualLoopTest;

	void ThreadProc1();
	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
	int OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType = eEqpCtrlMode_Auto);
	int OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList = NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnSetfocusEditTestCnt();
	afx_msg void OnEnSetfocusEditSotDelay();
	int CopyFolder(CString szFrom, CString szTo);

	void DisplayUI();
private:
	CEditTrans m_editLoopTestCount;
	CEditTrans m_editLoopSotDelay;
	int m_nLoopTestCount;
	double m_dLoopSotDelay;

	int m_TestSite_Div_X;
	int m_TestSite_Div_Y;

public:
	afx_msg void OnBnClickedOk();

	CSpreadSheet m_pSpreadTesterCommSts[MAX_TESTER_SINGLE];

	afx_msg void OnBnClickedButtonSimulTesterCopyFile();
	afx_msg void OnBnClickedButtonSimulTesterKill();
	afx_msg void OnBnClickedButtonSimulTesterExecute();
};
