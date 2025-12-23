#pragma once

#include "../Include/OcxFiles/dsspanel.h"

// CMainLower_TabPassFail 대화 상자입니다.
#define UM_SITE_HOLD			(WM_USER + 1000)

class CMainLower_TabPassFail : public CDialogEx
{
	DECLARE_DYNAMIC( CMainLower_TabPassFail )

public:
	CMainLower_TabPassFail( CWnd* pParent = NULL );   // 표준 생성자입니다.
	virtual ~CMainLower_TabPassFail();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_PG_PASSFAIL };

// 	enum _eSpreadIdx {
// 		eFront,
// 		eRear,
// 		eMaxSpreadIdx
// 	};
// 
// 	enum _eSpreadRowIdx {
// 		eRow1 = 1, 
// 		eRow2,
// 		eRow3,
// 		eRow4
// 	};
// 
// 	enum _eOnOffSpreadType {
// 		eOnOff_Spread_Type1,
// 		eOnOff_Spread_Type2,
// 		eMaxOnOffSpreadType
// 	};
private:
	CStaticTrans m_nTestTm[ eMaxPressUnitCount ];
	double m_dTestTime[ eMaxPressUnitCount ];
	int m_nTestTmCtrlID[ eMaxPressUnitCount ];
	int m_nIndexID[ eMaxPressUnitCount ];
	int m_nSocketOnOff[ STATION_MAX_PARA*eMaxPressUnitCount ];
	int m_nSpreadTestSiteID[ eMaxPressUnitCount][eMaxSpreadIdx];

	//int  m_nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE]; //해당 Station의 onoff 데이터
	int  m_nSiteOnOff[eMaxPressUnitCount][4][2]; //해당 Station의 onoff 데이터
	int  m_nCol;
	int  m_nRow;

	int m_nBefore_TestSite_Div_X;
	int m_nBefore_TestSite_Div_Y;

	int m_nTestSite_Div_X;
	int m_nTestSite_Div_Y;

	int m_nProjectOption;

	std::vector<int> m_veSiteIndex;
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
			{ _T("4"),  },
			{ _T("3"),  },
			{ _T("2"),  },
			{ _T("1"),  }
		},
		{
			{ _T("1") },
			{ _T("2") },
			{ _T("3") },
			{ _T("4") }
		}
	};



//	CString m_strSpreadOnOffIdx[eMaxOnOffSpreadType][4][2] ={}

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()
public:
	void Refresh();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void RefreshConnection();
	void RefreshSiteOnOff();

	void CheckRunningAndSwitchOnOff(int nStation, long Col, long Row , BOOL bTypeAll = FALSE, BOOL bHoldType = FALSE);
	BOOL ChkModuleUseAble(int nStation);
	void setAllStationOnOffFromDM();
	void switchOnOff(int nStation , int Col, int Row, BOOL bTypeAll = FALSE);
	BOOL ChkAllStationOFF();
	BOOL ChkSocketOnOffInterlock();
	BOOL Chk4StationSocketOff(std::vector<int> vSocket);
	BOOL ChkEachOneMoreSocketOn(std::vector<int> vSocket);
	BOOL ChkRetestTwoMoreStaionOn(std::vector<int> vSocket);
	void ChkEachOneMoreSocketState(int _station, int& _Socket_number);
	void ChkErrEachOneMoreSocketOn(std::vector<int> vChkSocket, std::vector<int> &vSocket);
	void ChkRetestTwoMoreStaionState(int _index, std::vector<int> _vSocket, int& _nStation);
	void ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vSocket, int &_Socket);
	void SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData);
	void MakeLog(LPCTSTR fmt, ...);
	void ChkLotState();
	void ChangeAllSocketStatus(int nStation);
	void ChangeAllSocketHoldStatus(int nStation);
	int getCurUserLv();
	BOOL ShowLoginBox();
	void switchSiteHold(int nStation, int Col, int Row, BOOL bTypeAll /* = FALSE */);
	int TesterConnectChk(int nStation, int nX, int nY);

	afx_msg void OnBnClickedBtnClean1();
	afx_msg void OnBnClickedBtnClean2();
	afx_msg void OnBnClickedBtnClean3();
	afx_msg void OnBnClickedBtnClean4();
	afx_msg void OnBnClickedBtnClean5();
	afx_msg void OnBnClickedBtnClean6();
	afx_msg void OnBnClickedBtnClean7();
	afx_msg void OnBnClickedBtnClean8();

private:
	void SetRevClean(int nPressIndex);
	void RefreshCleaning();
	CShadeButtonST cButtonST_1;
	CShadeButtonST cButtonST_2;
	CShadeButtonST cButtonST_3;
	CShadeButtonST cButtonST_4;
	CShadeButtonST cButtonST_5;
	CShadeButtonST cButtonST_6;
	CShadeButtonST cButtonST_7;
	CShadeButtonST cButtonST_8;
	CShadeButtonST cButtonST_9;
	CShadeButtonST cButtonST_10;
	CShadeButtonST cButtonST_11;
	CShadeButtonST cButtonST_12;
	CShadeButtonST cButtonST_13;
	CShadeButtonST cButtonST_14;
	CShadeButtonST cButtonST_15;
	CShadeButtonST cButtonST_16;

	void CompanySetting();
	char m_szCompany[16];
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

	void StationParaSetting();
	
	afx_msg void OnStnDblclickStation1();
	afx_msg void OnStnDblclickStation2();
	afx_msg void OnStnDblclickStation3();
	afx_msg void OnStnDblclickStation4();
	afx_msg void OnStnDblclickStation5();
	afx_msg void OnStnDblclickStation6();
	afx_msg void OnStnDblclickStation7();
	afx_msg void OnStnDblclickStation8();
	afx_msg LRESULT	OnSiteHold(WPARAM wParam, LPARAM lparam);

	void RightClickSpreadStation1(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation2(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation3(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation4(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation5(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation6(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation7(short ClickType, long Col, long Row, long MouseX, long MouseY);
	void RightClickSpreadStation8(short ClickType, long Col, long Row, long MouseX, long MouseY);

//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ConvertRowColLogicalIdx(int nSpreadType, int nPhyRow, int nPhyCol, int &nLogRow, int &nLogCol);
	
};
