#pragma once

#include "../Include/OcxFiles/dsspanel.h"

#define UM_SITE_YIELD_HOLD			(WM_USER + 1001)
// CMainLower_TabYield 대화 상자입니다.
class CMainLower_TabYield : public CDialogEx
{
	DECLARE_DYNAMIC( CMainLower_TabYield )

public:
	enum eYIELD_MODE
	{
		YM_PERCENTAGE,
		YM_COUNT,
		YM_1ST_FAIL,
		YM_2DID_FAIL,
		YM_nr,
	};

	CMainLower_TabYield( eYIELD_MODE mode, CWnd* pParent = NULL );   // 표준 생성자입니다.
	virtual ~CMainLower_TabYield();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_PG_YIELD };
private:
	CStaticTrans m_nTestTm[ eMaxPressUnitCount ];
	double m_dTestTime[ eMaxPressUnitCount ];
	int m_nTestTmCtrlID[ eMaxPressUnitCount ];
	int m_nIndexID[ eMaxPressUnitCount ];
	int m_nSocketOnOff[ STATION_MAX_PARA*eMaxPressUnitCount ];
//	int m_nSpreadTestSiteID[ eMaxPressUnitCount ];
	int m_nSpreadTestSiteID[eMaxPressUnitCount][eMaxSpreadIdx];


	int    m_nGoodBinCnt[ STATION_MAX_PARA*eMaxPressUnitCount ];
	int    m_nFailBinCnt[ STATION_MAX_PARA*eMaxPressUnitCount ];
	int    m_nClsAfterGoodBinCnt[ STATION_MAX_PARA*eMaxPressUnitCount ];
	int    m_nClsAfterFailBinCnt[ STATION_MAX_PARA*eMaxPressUnitCount ];
	int    m_n1stFailBinCnt[STATION_MAX_PARA*eMaxPressUnitCount];
	int    m_n2DIDFailBinCnt[STATION_MAX_PARA*eMaxPressUnitCount];
	char m_szCompany[16];
	int m_nProjectOption;
	std::vector <int> m_veSiteIndex;
	CShadeButtonST cButtonST_1;
	CShadeButtonST cButtonST_2;
	CShadeButtonST cButtonST_3;
	CShadeButtonST cButtonST_4;
	CShadeButtonST cButtonST_5;
	CShadeButtonST cButtonST_6;
	CShadeButtonST cButtonST_7;
	CShadeButtonST cButtonST_8;
// 	CShadeButtonST cButtonST_9;
// 	CShadeButtonST cButtonST_10;
// 	CShadeButtonST cButtonST_11;
// 	CShadeButtonST cButtonST_12;
// 	CShadeButtonST cButtonST_13;
// 	CShadeButtonST cButtonST_14;
// 	CShadeButtonST cButtonST_15;
// 	CShadeButtonST cButtonST_16;
	//control variables
	eYIELD_MODE m_eYieldMode;
private:
	void position_setup();
	CDSSPanel	m_CtrlConnect[MAX_TESTER_DUAL];
	int			m_nConntct[MAX_TESTER_DUAL];
	int			m_nStationOnOff[eMaxPressUnitCount];
	int			m_nTesterCount;

	CString m_strOnOffSpreadType[eMaxOnOffSpreadType][4][2] = {
		{
			{ _T("4"), _T("8") },
			{ _T("3"), _T("7") },
			{ _T("2"), _T("6") },
			{ _T("1"), _T("5") }
		},
		{
			{ _T("5"), _T("1") },
			{ _T("6"), _T("2") },
			{ _T("7"), _T("3") },
			{ _T("8"), _T("4") }
		}
	};
	CString m_strOnOffSpreadTypeY1[eMaxOnOffSpreadType][4][1] = {
		{
			{ _T("4"), },
			{ _T("3"), },
			{ _T("2"), },
			{ _T("1"), }
		},
		{
			{ _T("1") },
			{ _T("2") },
			{ _T("3") },
			{ _T("4") }
		}
	};
protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void Refresh();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void RefreshConnection();
	void RefreshSiteOnOff();
	void StationParaSetting();

	int m_nTestSite_Div_X  ;
	int m_nTestSite_Div_Y  ;

	int m_nBefore_TestSite_Div_X;
	int m_nBefore_TestSite_Div_Y;

	void SetRevClean(int nPressIndex);
	void RefreshCleaning();
	void CompanySetting();
public:
	DECLARE_EVENTSINK_MAP()
	void DblClickSpreadStation1(long Col, long Row);
	void DblClickSpreadStation2(long Col, long Row);
	void DblClickSpreadStation3(long Col, long Row);
	void DblClickSpreadStation4(long Col, long Row);
	void DblClickSpreadStation5(long Col, long Row);
	void DblClickSpreadStation6(long Col, long Row);
	void DblClickSpreadStation7(long Col, long Row);
	void DblClickSpreadStation8(long Col, long Row);
	void DblClickSpreadStation9(long Col, long Row);
	void DblClickSpreadStation10(long Col, long Row);
	void DblClickSpreadStation11(long Col, long Row);
	void DblClickSpreadStation12(long Col, long Row);
	void DblClickSpreadStation13(long Col, long Row);
	void DblClickSpreadStation14(long Col, long Row);
	void DblClickSpreadStation15(long Col, long Row);
	void DblClickSpreadStation16(long Col, long Row);

	void RightClickSpreadStation1(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation2(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation3(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation4(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation5(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation6(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation7(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation8(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation9(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation10(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation11(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation12(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation13(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation14(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation15(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation16(short ClickType, long Col, long Row, long MouseX, long MouseY);

	int  m_nSiteOnOff[eMaxPressUnitCount][4][2]; //해당 Station의 onoff 데이터
	int m_nRow;
	int m_nCol;

	void CheckRunningAndSwitchOnOff(int nStation, long Col, long Row, BOOL bTypeAll = FALSE, BOOL bHoldType = FALSE);
	int getCurUserLv();
	BOOL ChkModuleUseAble(int nStation);
	void switchOnOff(int nStation, int Col, int Row, BOOL bTypeAll = FALSE);
	void switchSiteHold(int nStation, int Col, int Row, BOOL bTypeAll /* = FALSE */);
	BOOL ShowLoginBox();
	void setAllStationOnOffFromDM();
	void ChangeAllSocketStatus(int nStation);
	int TesterConnectChk(int nStation, int nX, int nY);
	BOOL ChkSocketOnOffInterlock();
	void ChangeAllSocketHoldStatus(int nStation);
	void SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData);
	BOOL Chk4StationSocketOff(std::vector<int> vSocket);
	BOOL ChkEachOneMoreSocketOn(std::vector<int> vSocket);
	BOOL ChkRetestTwoMoreStaionOn(std::vector<int> vSocket);
	void ChkEachOneMoreSocketState(int _station, int& _Socket_number);
	void ChkErrEachOneMoreSocketOn(std::vector<int> vChkSocket, std::vector<int> &vSocket);
	void ChkRetestTwoMoreStaionState(int _index, std::vector<int> _vSocket, int& _nStation);
	void ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vSocket, int &_Socket);
	void MakeLog(LPCTSTR fmt, ...);
	BOOL ChkAllStationOFF();
	afx_msg LRESULT	OnSiteHold(WPARAM wParam, LPARAM lparam);

	afx_msg void OnBnClickedBtnYeClean1();
	afx_msg void OnBnClickedBtnYeClean2();
	afx_msg void OnBnClickedBtnYeClean3();
	afx_msg void OnBnClickedBtnYeClean4();
	afx_msg void OnBnClickedBtnYeClean5();
	afx_msg void OnBnClickedBtnYeClean6();
	afx_msg void OnBnClickedBtnYeClean7();
	afx_msg void OnBnClickedBtnYeClean8();
	afx_msg void OnBnClickedBtnYeClean9();
	afx_msg void OnBnClickedBtnYeClean10();
	afx_msg void OnBnClickedBtnYeClean11();
	afx_msg void OnBnClickedBtnYeClean12();
	afx_msg void OnBnClickedBtnYeClean13();
	afx_msg void OnBnClickedBtnYeClean14();
	afx_msg void OnBnClickedBtnYeClean15();
	afx_msg void OnBnClickedBtnYeClean16();

	afx_msg void OnStnDblclickStation1();
	afx_msg void OnStnDblclickStation2();
	afx_msg void OnStnDblclickStation3();
	afx_msg void OnStnDblclickStation4();
	afx_msg void OnStnDblclickStation5();
	afx_msg void OnStnDblclickStation6();
	afx_msg void OnStnDblclickStation7();
	afx_msg void OnStnDblclickStation8();
	afx_msg void OnStnDblclickStation9();
	afx_msg void OnStnDblclickStation10();
	afx_msg void OnStnDblclickStation11();
	afx_msg void OnStnDblclickStation12();
	afx_msg void OnStnDblclickStation13();
	afx_msg void OnStnDblclickStation14();
	afx_msg void OnStnDblclickStation15();
	afx_msg void OnStnDblclickStation16();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
