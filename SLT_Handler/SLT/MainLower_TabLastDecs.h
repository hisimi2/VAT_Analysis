#pragma once

#include "../Include/OcxFiles/dsspanel.h"
// CMainLower_TabDesc 대화 상자입니다.
class CMainLower_TabDesc : public CDialogEx
{
	DECLARE_DYNAMIC(CMainLower_TabDesc)

public:

	CMainLower_TabDesc( CWnd* pParent = NULL );   // 표준 생성자입니다.
	virtual ~CMainLower_TabDesc();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_PG_LASTDESC };
private:
	CStaticTrans m_nTestTm[ eMaxPressUnitCount ];
	double m_dTestTime[ eMaxPressUnitCount ];
	int m_nTestTmCtrlID[ eMaxPressUnitCount ];
	int m_nIndexID[ eMaxPressUnitCount ];
	int m_nSocketOnOff[ STATION_MAX_PARA*eMaxPressUnitCount ];
//	int m_nSpreadTestSiteID[ eMaxPressUnitCount ];
	int m_nSpreadTestSiteID[eMaxPressUnitCount][eMaxSpreadIdx];

	int m_nTesterCount;
private:
	void position_setup();
	CDSSPanel	m_CtrlConnect[MAX_TESTER_DUAL];
	int			m_nConntct[MAX_TESTER_DUAL];
	int m_nProjectOption;
	std::vector <int> m_veSiteIndex;
protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void Refresh();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void RefreshConnection();

	void StationParaSetting();

	int m_nTestSite_Div_X;
	int m_nTestSite_Div_Y;

	int m_nBefore_TestSite_Div_X;
	int m_nBefore_TestSite_Div_Y;
};
