#pragma once

#include "shadebuttonst.h"
#include "spreadsheet.h"
#include "afxwin.h"



// CSiteSetDlg_128Para 대화 상자입니다.

class CSiteSetDlg_128Para : public CDialogEx
{
	DECLARE_DYNAMIC(CSiteSetDlg_128Para)

public:
	CSiteSetDlg_128Para(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSiteSetDlg_128Para();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SITE_SET_128PARA };
	enum _eMODULE_NUM { STATION1 = 0, STATION2,STATION3,STATION4,STATION5,STATION6,STATION7,STATION8,MODULE1,MODULE2, ALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
private:
	CShadeButtonST m_BtnOnline;
	CShadeButtonST m_BtnOffLine;
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnExistDvc;
	CShadeButtonST m_BtnDemo;
	CShadeButtonST m_BtnNoDvcExistTray;
	CShadeButtonST m_BtnNoDvcNoTray;

	CSpreadSheet m_SPRD_EnableSite;

	int m_nRunMode;
	int m_nDeviceMode;

	int m_nProjectOption;

	int m_TestSite_Div_X;
	int m_TestSite_Div_Y;
	int  m_nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	BOOL m_bInitSpread;

	void OnBtnGUI();
	void OnEditBoxGUI();
	void OnUpdateSpreadSite64Para();
	void SetSocketSite(int _Station, int _Row, int _Col);
	void ChkLotState();
	void SetComboSettingToSprd(BOOL bOnOff);
	void SetSocketOnOFF( BOOL bOnOff, int _nStation );

	BOOL ChkAllStationOFF();
	BOOL ChkSocketOnOffInterlock();
	BOOL Chk8StationSocketOff(std::vector<int> vDown);
	BOOL ChkEachOneMoreSocketOn(std::vector<int> vDown);
	BOOL ChkRetestTwoMoreStaionOn( std::vector<int> vDown);
	void ChkErrEachOneMoreSocketOn( std::vector<int> vDown, std::vector<int> &vSocket );
	void ChkEachOneMoreSocketState(int _Downstation,int& _Socket_number);
	void ChkRetestTwoMoreStaionState(int _index, std::vector<int> vDown, int& _DownStation);
	void ChkErrEachOneMoreSocketState( int _index, std::vector<int> _vDown, int &Socket );

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioRunmode1();
	afx_msg void OnBnClickedRadioRunmode2();
	afx_msg void OnBnClickedRadioRunmode3();
	afx_msg void OnBnClickedRadioDvcmode1();
	afx_msg void OnBnClickedRadioDvcmode2();
	afx_msg void OnBnClickedRadioDvcmode3();
	afx_msg void OnBnClickedSitesetSave();
	afx_msg void OnEnSetfocusEditOfflineTestTime();
	BOOL m_bAutoRetest_TestPP;
	BOOL m_bRetest_Skip_HW1;
	BOOL m_bRetest_Skip_HW2;
	BOOL m_bRetest_Skip_HW3;
	BOOL m_bRetest_Skip_HW4;
	BOOL m_bOffLineBinRandom;
	BOOL m_bOffLineYieldCut;
	BOOL m_bAlwayBin1Sorting;
	double m_dOffLineYieldCutPer;

	afx_msg void OnBnClickedChkAutoRetest();
	int m_nAutoRetestKind;
	void MakeLog(LPCTSTR fmt, ...);
	void SiteSetChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData);
	void InitSpreadHead();
	afx_msg void OnBnClickedSiteUp();
	afx_msg void OnBnClickedSiteDown();
	afx_msg void OnBnClickedRadioUpSite();
	afx_msg void OnBnClickedRadioDownSite();
	int m_nTestSite;
	CComboBox m_ComboBox;
	afx_msg void OnBnClickedButtonOn();
	afx_msg void OnBnClickedButtonOff();
	CEditTrans m_editOffLineTestTime;
	CEditTrans m_editOffLineYieldCut;

	int m_nOffLineTT;
	afx_msg void OnEnSetfocusEditOfflineTestTime2();
	DECLARE_EVENTSINK_MAP()
	void ButtonClickedSpreadEnableSite( long Col, long Row/*, short ButtonDown*/ );


	afx_msg void OnEnSetfocusEditYieldCutPer();

	void SiteSetSave();

	BOOL ChkModuleUseAble(int nCol);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DblClickSpreadEnableSite2(long Col, long Row);
	void DblClickSpreadEnableSite(long Col, long Row);

	//  [9/15/2020 jaehoon.jung]
	BOOL IsSite_On_Of_All(int nStationNum); //스테이션에서 하나라도 on인지?
	BOOL IsSite_Reserve_Of_All(int nStationNum);
	BOOL IsSite_Off_Of_All(int nStationNum);
	BOOL IsSite_Hold_Of_All(int nStationNum); //하나라도 Hold 이면 TRUE;

	int TesterConnectChk(int nStation, int nX, int nY);
};
