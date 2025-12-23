#pragma once


// CAutoGrrSetDlg 대화 상자입니다.

class CAutoGrrSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoGrrSetDlg)

// Variables
public:
private:
	CShadeButtonST m_btnSave;
	CShadeButtonST m_btnCancel;

	CSpreadSheet m_SprdGrrSite;

	CSpreadSheet m_SPRD_EnableSite;

	CEditTrans m_editUnitCount;
	CEditTrans m_editTestCount;
	CEditTrans m_editGrrSotDelay;

	CComboBox m_ComboBox;

	int m_nAutoGrrmode;

	// Shared Memory Data
	int m_nGrrSiteSocketOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	int m_nAutoGrrTestCount;
	int m_nAutoGrrUnitCount;
	double m_dGrrSotDelay;

	

	int m_Station_Div_X;
	int m_Station_Div_Y;

	int m_nProjectOption;
// Function
public:
	CAutoGrrSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoGrrSetDlg();

	void OnInitGUI();
	void LoadData();
	void InitSpreadHeadGrr();
	void OnUpdateSpreadGRRSite();
	//void SetSocketSite(int _Station, int _Row, int _Col);
	void SetComboSettingToSprd(BOOL bOnOff);
	void SetSocketOnOFF( BOOL bOnOff, int _nStation );
	BOOL ChkAllStationOFF();
	void DisableControl();

	void OnUpdateSpreadSite64Para();

	void InitSpreadHead();
	void ButtonClickedSpreadEnableSite(long Col, long Row/*, short ButtonDown*/);
	void DblClickSpreadEnableSite2(long Col, long Row);
	void DblClickSpreadEnableSite(long Col, long Row);
	void CompanySetting();

	void SiteSetSave();
	BOOL m_bInitSpread;

	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	void SetSocketSite(int _Station, int _Row, int _Col);
	BOOL IsSite_On_Of_All(int nStationNum); 
	BOOL IsSite_Reserve_Of_All(int nStationNum);
	BOOL IsSite_Off_Of_All(int nStationNum);
	BOOL IsSite_Hold_Of_All(int nStationNum);
	BOOL ChkModuleUseAble(int nCol);
	void ChkGUISet();
	DECLARE_EVENTSINK_MAP()
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_GRR_SET };
	enum _eGRRMODULE_NUM { STATION1 = 0, STATION2,STATION3,STATION4,STATION5,STATION6,STATION7,STATION8,ALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSitesetSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnSetfocusEditNumberOfUnits();
	afx_msg void OnEnSetfocusEditNumberOfTimes();
	void MakeLog( LPCTSTR fmt, ... );
	void SiteSetChangeLog( std::vector<std::pair<CString, std::pair<CString, CString>>> vData );
	BOOL m_bGRR_Device_swap;
	BOOL m_bGRR_Audit_loop;

	afx_msg void OnBnClickedButtonOn();
	afx_msg void OnBnClickedButtonOff();
	afx_msg void OnBnClickedRadioNormalMode();
	afx_msg void OnBnClickedRadioContiuneMode();
	afx_msg void OnBnClickedRadioePressReContactMode();
	afx_msg void OnEnSetfocusEditGrrSotDelayTime();
	afx_msg void OnBnClickedChkGrrModeAuditLoop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedChkGrrModeDeviceSwap();
};
