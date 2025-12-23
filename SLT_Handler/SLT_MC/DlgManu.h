#pragma once
#include "afxcmn.h"
#include "stdarg.h"
#include "afxwin.h"
#include "ShadeButtonST.h"
#include "DlgHomeCheckStatus.h"
#include "DlgContactAndSafety.h"
#include "DlgManuTesterIF.h"

class CBaseTaskManager;

// CDlgManu 대화 상자입니다.	

class CDlgManu : public CDialogEx, public CBaseTaskManager
{
	DECLARE_DYNAMIC(CDlgManu)

public:
	CDlgManu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManu();

	void ThreadProc1();

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	int OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType=eEqpCtrlMode_Auto);
	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);
	void OnBnClickedBtnAixsManuLoadTbl(int nTableIdx);
	void OnBnClickedBtnAixsManuTestPp(int nTestPPIdx);
	void OnBnClickedBtnAixsManuStacker(int nStackerIdx );
	void OnBnClickedBtnAixsManuPress(int nPressIdx );

	void UpdateEventMessage(const char* fmt, ...);
	void GetTimeString(CString& strTimeString);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonManuPressUnit1();
	afx_msg void OnBnClickedButtonManuLoadTBLAndTestPp1();
	afx_msg void OnBnClickedButtonManuLoadTBLAndTestPp2();
	afx_msg void OnBnClickedButtonManuTrayPp1();
	afx_msg void OnBnClickedButtonManuSetplate();
	afx_msg void OnBnClickedButtonManuExStacker();
	afx_msg void OnBnClickedButtonManuServoMotorSetting();
	afx_msg void OnBnClickedButtonManuMotorSetupTeaching();
	afx_msg void OnBnClickedButtonManuTestInterface();
	afx_msg void OnBnClickedButtonManuIoAllCheck();
	afx_msg void OnBnClickedButtonManuEpRegulator();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonManuTransfer();
	afx_msg void OnBnClickedBtnAixsManuPressUnit9();
	afx_msg void OnBnClickedBtnAixsManuPressUnit1();
	afx_msg void OnBnClickedBtnAixsManuPressUnit2();
	afx_msg void OnBnClickedBtnAixsManuPressUnit3();
	afx_msg void OnBnClickedBtnAixsManuPressUnit4();
	afx_msg void OnBnClickedBtnAixsManuPressUnit5();
	afx_msg void OnBnClickedBtnAixsManuPressUnit6();
	afx_msg void OnBnClickedBtnAixsManuPressUnit7();
	afx_msg void OnBnClickedBtnAixsManuPressUnit8();
	afx_msg void OnBnClickedBtnAixsManuPressUnit10();
	afx_msg void OnBnClickedBtnAixsManuPressUnit11();
	afx_msg void OnBnClickedBtnAixsManuPressUnit12();
	afx_msg void OnBnClickedBtnAixsManuPressUnit13();
	afx_msg void OnBnClickedBtnAixsManuPressUnit14();
	afx_msg void OnBnClickedBtnAixsManuPressUnit15();
	afx_msg void OnBnClickedBtnAixsManuPressUnit16();
	afx_msg void OnBnClickedBtnAixsManuTestPp1();
	afx_msg void OnBnClickedBtnAixsManuTestPp2();
	afx_msg void OnBnClickedBtnAixsManuTestPp3();
	afx_msg void OnBnClickedBtnAixsManuTestPp4();
	afx_msg void OnBnClickedBtnAixsManuLoadTbl1();
	afx_msg void OnBnClickedBtnAixsManuLoadTbl2();
	afx_msg void OnBnClickedBtnAixsManuLoadTbl3();
	afx_msg void OnBnClickedBtnAixsManuLoadTbl4();
	afx_msg void OnBnClickedBtnAixsManuTrayPp(int nTrayPpInx);
	afx_msg void OnBnClickedBtnAixsManuTransfer();
	afx_msg void OnBnClickedButtonManuAioAllCheck();
	afx_msg void OnBnClickedButtonManuProcCmdRetry();
	afx_msg void OnBnClickedButtonManuProcCmdSkip();
	afx_msg void OnBnClickedButtonManuProcCmdStop();
	CEdit m_editManuOperEvent;
	afx_msg void OnBnClickedButtonManuVisionIf();

	CBitmap m_cBmpPress;
	CBitmap m_cBmpSetplateStacker;
	CBitmap m_cBmpLoadTestPP[4];
	CBitmap m_cBmpTrayPP1;
	CBitmap m_cBmpTrayPP2;
	CBitmap m_cBmpFeeder;
// 	CBitmap m_cBmpPressUp[8];
// 	CBitmap m_cBmpPressDown[8];
	CBitmap m_cBmpTestPP[4];
	CBitmap m_cBmpLoadTable[4];
	CBitmap m_cBmpTrayPP1Axis;
	CBitmap m_cBmpTrayPP2Axis;
	CBitmap m_cBmpFeederAxis;
	CBitmap m_cBmpStackerEVLeft;
	CBitmap m_cBmpStackerEVRight;
	CBitmap m_cBmpTransferEV;
	CBitmap m_cBmpTransfer;
	CBitmap m_cBmpServoMotorSetting;
	CBitmap m_cBmpMotorSetupTeaching;
	CBitmap m_cBmpEPReglator;
	CBitmap m_cBmpDIOAllCheck;
	CBitmap m_cBmpAIOAllCheck;
	CBitmap m_cBmpStop;
	CBitmap m_cBmpRetry;
	CBitmap m_cBmpSkip;
	CBitmap m_cBmpCommunicationTest;
	CBitmap m_cBmpExit;
	CBitmap m_cBmpVisionInterface;

	CBitmap m_cBmpCylinderTime;

	CButton m_btnPress;
	CButton m_btnSetplateStacker;
	CButton m_btnLoadTestPP[4];
	CButton m_btnTrayPP1;
	CButton m_btnTrayPP2;
	CButton m_btnFeeder;
// 	CButton m_btnPressUp[8];
// 	CButton m_btnPressDown[8];
	CButton m_btnTestPP[4];
	CButton m_btnLoadTable[4];
	CButton m_btnTrayPP1Axis;
	CButton m_btnTrayPP2Axis;
	CButton m_btnFeederAxis;
	CButton m_btnStackerEVLeft;
	CButton m_btnStackerEVRight;
	CButton m_btnTransferEV;
	CButton m_btnTransfer;
	CButton m_btnServoMotorSetting;
	CButton m_btnMotorSetupTeaching;
	CButton m_btnEPReglator;
	CButton m_btnDIOAllCheck;
	CButton m_btnAIOAllCheck;
	CButton m_btnStop;
	CButton m_btnRetry;
	CButton m_btnSkip;
	CButton m_btnCommunicationTest;
	CButton m_btnExit;
	CButton m_btnVisionInterface;
	CButton m_btnCylinderTime;

private:
	BOOL m_bManualCmdExcute;

	CShadeButtonST m_btnPress1 ;
	CShadeButtonST m_btnPress2 ;
	CShadeButtonST m_btnPress3 ;
	CShadeButtonST m_btnPress4 ;
	CShadeButtonST m_btnPress5 ;
	CShadeButtonST m_btnPress6 ;
	CShadeButtonST m_btnPress7 ;
	CShadeButtonST m_btnPress8 ;
	CShadeButtonST m_btnPress9 ;
	CShadeButtonST m_btnPress10;
	CShadeButtonST m_btnPress11;
	CShadeButtonST m_btnPress12;
	CShadeButtonST m_btnPress13;
	CShadeButtonST m_btnPress14;
	CShadeButtonST m_btnPress15;
	CShadeButtonST m_btnPress16;
	CShadeButtonST m_btnTbl1_2;
	CShadeButtonST m_btnTbl3_4;
	CShadeButtonST m_btnStacker[7];

	void OnUpdatePressUI();
public:
	afx_msg void OnBnClickedButtonManuLoadTbl12();
	afx_msg void OnBnClickedButtonManuLoadTbl13();
	afx_msg void OnBnClickedBtnAixsManuPressUp();
	afx_msg void OnBnClickedBtnAixsManuPressDn();
	afx_msg void OnBnClickedBtnAixsManuLoadTbl();
	afx_msg void OnBnClickedBtnAixsManuMod1();
	afx_msg void OnBnClickedBtnAixsManuMod2();
	afx_msg void OnBnClickedBtnAixsManuMod3();
	afx_msg void OnBnClickedBtnAixsManuMod4();
	afx_msg void OnBnClickedBtnAixsManuMod5();
	afx_msg void OnBnClickedBtnAixsManuMod6();
	afx_msg void OnBnClickedBtnAixsManuAll();
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedBtnAixsManuTestpp();
	afx_msg void OnBnClickedBtnAixsManuStacker1();
	afx_msg void OnBnClickedBtnAixsManuStacker2();
	afx_msg void OnBnClickedBtnAixsManuStacker3();
	afx_msg void OnBnClickedBtnAixsManuStacker4();
	afx_msg void OnBnClickedBtnAixsManuStacker5();
	afx_msg void OnBnClickedBtnAixsManuStacker6();
	afx_msg void OnBnClickedBtnAixsManuStacker7();

private:
	CDlgHomeCheckStatus*      m_pDlgHomeCheckStatus;
	CDlgContactAndSafety*     m_pDlgContactAndSafety;

	CDlgManuTesterIF* m_pDlgManuTesterIF;
	//CDlgManuMotorTeaching*      m_pDlgManuMotorTeaching;
public:
	afx_msg void OnBnClickedButtonManuCylinderCheck();
	afx_msg void OnBnClickedBtnAixsManuTrayPp1();
	afx_msg void OnBnClickedBtnAixsManuTrayPp2();
	afx_msg void OnBnClickedButtonManuTrayPp2();
	afx_msg void OnBnClickedBtnAxisManualFeeder();
	afx_msg void OnBnClickedButtonManuFeeder();
	afx_msg void OnBnClickedButtonTrigger();
};
