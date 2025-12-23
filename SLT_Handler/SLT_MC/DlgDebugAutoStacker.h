#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "StaticTrans.h"
#include "../Include/OcxFiles/spreadsheet.h"


// CDlgDebugAutoStacker 대화 상자입니다.
class CBaseTaskManager;

class CDlgDebugAutoStacker : public CDialogEx, public CBaseTaskManager
{
	DECLARE_DYNAMIC(CDlgDebugAutoStacker)

public:
	CDlgDebugAutoStacker(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDebugAutoStacker();

	void ThreadProc1();

	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);
	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	void UpdateEventMessage(const char* fmt, ...);
	void GetTimeString(CString& strTimeString);

	void UpdateStatus();
	void UpdateTrayDataExist();

	void OnBnClickedBtnZip();

	CWnd* m_pInputValueTargetWnd;

	int m_nWorkSetplate;
	std::vector<int> m_nOnOffDi_Idx;
	int m_nOnOffDi_Val;
	
	int m_nTrayExistCtrlId[eMaxStackerCount];
	int m_nTrayFullCtrlId[eMaxStackerCount];

	int m_nFrontConveyorExistCtrlId[ eMaxStackerCount ];
	int m_nFrontConveyorDataCtrlId[eMaxStackerCount];
	//int m_nFrontConveyorSlowCtrlId[ eMaxStackerCount ];
	//int m_nRearConveyorSlowCtrld[ eMaxStackerCount ];
	int m_nRearConveyorExistCtrlId[ eMaxStackerCount ];
	int m_nRearConveyorDataCtrlId[ eMaxStackerCount ];

    CStaticTrans m_StaticTowerR;
    CStaticTrans m_StaticTowerG;
    CStaticTrans m_StaticTowerY;

	CStaticTrans m_StaticSwitchCover;

	CStaticTrans m_StaticTblTestSidePos[MAX_LD_TBL_CNT][MAX_LD_TBL_CNT];
	CStaticTrans m_StaticTblLoadSidePos[MAX_LD_TBL_CNT];
	CStaticTrans m_StaticReserve[3];
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DEBUG_AUTO_STACKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonAutoStackerInitial();
	afx_msg void OnBnClickedButtonAutoStackerLotStart();
	afx_msg void OnBnClickedButtonAutoStackerLotPause();
	afx_msg void OnBnClickedButtonAutoStackerLotResume();
	afx_msg void OnBnClickedButtonAutoStackerLotOnecycle();
	afx_msg void OnBnClickedButtonAutoStackerLotRetry();
	afx_msg void OnBnClickedButtonAutoStackerLotSkip();
	afx_msg void OnBnClickedButtonAutoStackerLotCleanout();
	CEdit m_editManuOperEvent;
	afx_msg void OnBnClickedButtonAutoStackerAuto();
	afx_msg void OnBnClickedButtonAutoStackerManual();
	afx_msg void OnBnClickedButtonAutoStackerStart();
	afx_msg void OnBnClickedButtonAutoStackerStop();
	afx_msg void OnSimulationInputsimulationvalue();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSimulationOn();
	afx_msg void OnSimulationOff();
	afx_msg void OnSimulationConveyorRearAllOn();
	afx_msg void OnSimulationConveyorRearAllOff();
	afx_msg void OnSimulationConveyorFrontAllOn();
	afx_msg void OnSimulationConveyorFrontAllOff();
	afx_msg void OnSimulationUnloadExistAllOn();
	afx_msg void OnSimulationUnloadExistAllOff();

	afx_msg void OnBnClickedButtonOpPanelLampSw();
	afx_msg void OnBnClickedButtonOpPanelSoundSw();
	afx_msg void OnBnClickedButtonOpPanelCoverSw();

private:
	SYSTEMTIME m_SystemTime;
	BOOL m_bUPH_Start;
	std::deque< ST_DD_DEVICE > m_DeviceDataClipBoard;
	void do_device_data_clip( int _device_stage_idx, CRect _ltrb, int _key );
public:
	afx_msg void OnBnClickedBtnUph();
	afx_msg void OnBnClickedBtnUphStop();
	afx_msg void OnBnClickedBtnJamLotDataClear();
	afx_msg void OnBnClickedOk();
private:
	CSpreadSheet m_SpreadClrInfo;
	void InitSpreadSheet();
	void SetDisplayStageInfo(int spreadCtrlID, int stage, int columnLength, int rowLength, int rowstart = 0);
	void SetDisplayPressStageInfo(int spreadCtrlID, int site, int stage, int columnLength, int rowLength);
	void SetDisplaySOTEOTInfo(int spreadCtrlID, int columnLength, int rowLength, int nModule);
	int CompSystemTime(SYSTEMTIME SysTime1, SYSTEMTIME SysTime2); 
public:
	CSpreadSheet m_SpreadClrTest;
	CSpreadSheet m_SPRD_TestPP1_L;
	CSpreadSheet m_SPRD_TestPP1_U;
	CSpreadSheet m_SPRD_TestPP2_L;
	CSpreadSheet m_SPRD_TestPP2_U;
	CSpreadSheet m_SPRD_Load_Table1;
	CSpreadSheet m_SPRD_Load_Table2;
	CSpreadSheet m_SPRD_Load_Table3;
	CSpreadSheet m_SPRD_Load_Table4;

	CSpreadSheet m_SPRD_Retest_Buff_Table1;
	CSpreadSheet m_SPRD_Socketclean_Buff_Table1;
	CSpreadSheet m_SPRD_Retest_Buff_Table2;
	CSpreadSheet m_SPRD_Socketclean_Buff_Table2;
	afx_msg void OnBnClickedButtonMotionDbReload();
	void ClickSpreadLoadTbl( _eLoadTblIdx _idx, int _col, int _row );
//	void ClickSpreadLoadTbl1( long Col, long Row );
//	void ClickSpreadLoadTbl2( long Col, long Row );
	afx_msg void OnBnClickedButtonAutoStackerLoadTable1InitIdleReserve();
	afx_msg void OnBnClickedButtonAutoStackerLoadTable2InitIdleReserve();
	void KeyDownSpreadLoadTbl1( short* KeyCode, short Shift );
	void KeyDownSpreadLoadTbl2( short* KeyCode, short Shift );
	void RightClickSpreadLoadTbl1( short ClickType, long Col, long Row, long MouseX, long MouseY );
	void RightClickSpreadLoadTbl2( short ClickType, long Col, long Row, long MouseX, long MouseY );
	afx_msg void OnBnClickedButtonEventLogAllDelete();

	afx_msg void OnBnClickedButtonAutoStackerManual3();
	afx_msg void OnBnClickedButtonAutoStackerManual4();

	afx_msg void OnBnClickedButtonAutoStackerLoadTable3InitIdleReserve();
	afx_msg void OnStnDblclickStaticTaskLoadtbl1CmdStage();
	afx_msg void OnStnDblclickStaticTaskLoadtbl4CmdStage();
	afx_msg void OnBnClickedButtonAutoStackerLoadTable4InitIdleReserve();
	afx_msg void OnBnClickedButtonAutoStackerManual5();
};
