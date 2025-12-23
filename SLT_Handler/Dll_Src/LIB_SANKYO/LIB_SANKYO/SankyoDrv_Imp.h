#pragma once

#include "ThreadBase.h"
#include "WinRs232c.h"
#include <string>
#include <vector>
#include <deque>
#include <list>
#include "SankyoMotorDrv.h"
#include "DlgSankyoManual.h"
#include "DlgSankyoSimul.h"

#ifndef UM_SANKYO_LOG
	#define UM_SANKYO_LOG (WM_USER + 1000)
#endif // UM_SANKYO_LOG

#ifndef sankyo_err_no_error
	#define sankyo_err_no_error                        (0)
	#define sankyo_err_port_open_fail                  (1)
	#define sankyo_err_send_fail                       (2)
	#define sankyo_err_reply_timeout                   (3)
	#define sankyo_err_op_mode                         (4)
#endif //sankyo_err_no_error
/*=====================================================
  SANKYO MOTOR의 정보를 제공 한다.
  통신을 통해서 정보를 능동적으로 취득하고 있다가 필요시 제공
=====================================================*/
class CSankyoDrv_Imp : public CWnd, public CThreadBase
{
	DECLARE_DYNAMIC(CSankyoDrv_Imp)

public:
	enum eFlowState{
		eComm_Manual_Idle=0, eSend_0000, ePreSend_1000, eSend_1000, eWaitReply,
		eReplyTimeout_1000, eReplyTimeout_Wait_Retry_Skip,eCommSendFail_1000, eCommSendFail_1100,
	};

public:
	CSankyoDrv_Imp(void);
	~CSankyoDrv_Imp(void);

	// Interface Function
	int Initialize(Sankyo::ST_SANKYO_INIT_INFO stInitInfo);
	void Finalize();
	BOOL IsConnected();
	BOOL IsAutoRunning();
	double GetTorque(int nStation); // 메모리에서 꺼내서 데이터를 넘겨준다.
	void GetTorqueAll(std::vector<double>& vCont);
	void ShowManualDialog();
	BOOL GetAutoRunningHeartBit(){ return m_bRunningHeartBit; }
	void Op_Action(int nAction);
	void Op_RunMode(int nRunMode);
	
	// set-up
	int Setup_Subscription(int nStation, bool bEnable);
	int Setup_Subscription(std::list<int> vSubscriptionList);	
	bool Setup_GetSubscription(int nStation);
	void Setup_GetSubscriptionAll(std::list<int>& vSubscriptedStation);

	// Work Procedure
	void ThreadProc1();
	static  void cbReceiveEvent(CWnd* pWnd, LPARAM lParam, WPARAM wParam);
	void OnReceivePacket(LPARAM lParam, WPARAM wParam);

	// 
	void AddCommLogView(HWND hWnd);
	void RemoveLogView(HWND hWnd);
	void ClearLogView();
	int GetLogViewCount();
	void BroadcastLogView(char* szAscii, int nLen);
	void BroadcastLogView(const char* fmt, ...);

private:
	int SendPacket(int nStation, int nCommand, std::vector<unsigned short> sParam);
	bool CheckReceiveReply();
	int CheckOpAction();

private:
	char                  m_szSplit[10000];
	int                   m_nSplitCount;
	char                  m_szRcvBuff[1024];

	std::vector<Sankyo::ST_SANKYO_DATA> m_vData; // 통신을 통해 취득된 데이터 저장소, address에 따라서 고정하여 사용 (0~31)

	CWinRs232c*           m_pWinRs232c;
	std::list<int>        m_vSubscriptedStationNumber; // 데이터를 폴링하는 스테이션 리스트
	std::list<int>		  m_vStationNumTemp;
	int                   m_nStateMain;
	int                   m_nRunMode;
	int                   m_nSendRetryCount;
	int                   m_nMaxSendRetryCount;
	double                 m_dStartTime;
	int                   m_nReplyTimeoutRetryCount;
	int                   m_nMaxReplyTimeoutRetryCount;
	int                   m_nSelectedOperatorAction;
	int                   m_nTagNo;
	BOOL                  m_bRunningHeartBit;
	DWORD                 m_dwRcvLen;
	bool                  m_bReceiveReply;

	CRITICAL_SECTION      m_csRcvMsg;
	CRITICAL_SECTION      m_csLogObserver;
	std::vector<HWND>     m_vLogObserverList;
	Sankyo::ST_SANKYO_INIT_INFO m_stInitInfo;

	CDlgSankyoSimul*	  m_pDlgSimul;

	friend CDlgSankyoManual;
	friend CDlgSankyoSimul;

public:
	DECLARE_MESSAGE_MAP()
};

