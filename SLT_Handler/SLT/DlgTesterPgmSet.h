#pragma once


// CDlgTesterPgmSet 대화 상자입니다.

class CDlgTesterPgmSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTesterPgmSet)

public:
	CDlgTesterPgmSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTesterPgmSet();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TESTER_PGM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    CString m_strPgmName;
    int     m_nNewModifyType;
	CString m_strBeforePgmName;

private:
    CShadeButtonST m_BtnSave;
    CShadeButtonST m_BtnCancel;

	CShadeButtonST m_BtnInsert;
	CShadeButtonST m_BtnDelete;

    CEditTrans m_EditBootTmOut;
    CEditTrans m_EditTestTmOut;
	CEditTrans m_EditSotDelay;
    CEditTrans m_EditPreCooling;
    CEditTrans m_EditAC_PowerOnDelay;
    CEditTrans m_EditAC_PowerOffDelay;
    CEditTrans m_EditATX_PowerOnDelay;
    CEditTrans m_EditFailCodeDigital;
    CEditTrans m_EditMinPassTm;
    CEditTrans m_EditBiosBootCnt;
    CEditTrans m_EditBiosBootTmOut;
    CEditTrans m_EditBiosS3Cnt;
    CEditTrans m_EditBiosS3TmOut;
    CEditTrans m_EditBiosS3Delay;
    CEditTrans m_EditSetTemp;
    CEditTrans m_EditGuardBand;
    CEditTrans m_EditAGP_Delay;
	CEditTrans m_EditNotTestingTmOut;

    CString m_strPassString;
    CString m_strFailCodeHeader;
    CString m_strStartLine;
    CString m_strEndLine;
    BOOL m_bMD_HangReboot;
    BOOL m_bMD_BiosBoot;
    BOOL m_bMD_BiosS3;
    BOOL m_bMD_TempMonitor;
    BOOL m_bMD_ExtentionPwrAgp;
    BOOL m_bMD_BootTmOut   ; 
    BOOL m_bMD_PreCooling  ;
    BOOL m_bMD_AcPwrOnDly  ;
    BOOL m_bMD_AcPwrOff    ;
    BOOL m_bMD_AtxPowerOn  ;
    BOOL m_bMD_PassStr     ;
    BOOL m_bMD_FailCodeHead;
    BOOL m_bMD_FailCodeDig ;
    BOOL m_bMD_StartLine   ;
    BOOL m_bMD_EndLine     ;
    BOOL m_bMD_MinPassTm   ;
    BOOL m_bMD_BaudRate    ;

    double m_dBootTmOut;
    double m_dTestTmOut;
	double m_dSotDelay;
    double m_dPreCooling;
    double m_dAC_PowerOnDelay;
    double m_dAC_PowerOffDelay;
    double m_dATX_PowerOnDelay;
    int m_nFailCodeDigital;
    double m_dMinPassTm;
    int m_nBiosBootCnt;
    double m_dBiosBootTmOut;
    int m_nBiosS3Cnt;
    double m_dBiosS3TmOut;
    double m_dBiosS3Delay;
    int m_nSetTemp;
    int m_nGuardBand;
    double m_dAGP_Delay;
    int m_nBaudRate;

	double m_dNotTestingTmOut;
	ST_SLT_PGM m_stBeforePgmType;
	
    void OnEditBoxGUI();
    void OnBtnGUI();
    void OnUpdateData();

	void UpdateSpread();
	void SaveSpread();

	BOOL m_bMD_SiteOff_TestTimeOut;
	BOOL m_bMD_SiteOff_DefaultBin;
	BOOL m_bMD_Sort_as_DefaultBin;
	CSpreadSheet   m_SprdErrorString;
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnEnSetfocusEditBootTimeout();
    afx_msg void OnEnSetfocusEditTestTimeOut();
    afx_msg void OnEnSetfocusEditPreCooling();
    afx_msg void OnEnSetfocusEditAcPowerOnDelay();
    afx_msg void OnEnSetfocusEditAcPowerOffDelay();
    afx_msg void OnEnSetfocusEditAtxPowerDelay();
    afx_msg void OnEnSetfocusEditFailCodeDigNo();
    afx_msg void OnEnSetfocusEditMiniPassTm();
    afx_msg void OnEnSetfocusEditBiosBootCnt();
    afx_msg void OnEnSetfocusEditBiosBootTimeout();
    afx_msg void OnEnSetfocusEditBiosS3Cnt();
    afx_msg void OnEnSetfocusEditBiosS3Timeout();
    afx_msg void OnEnSetfocusEditBiosS3Delay();
    afx_msg void OnEnSetfocusEditTempSettemp();
    afx_msg void OnEnSetfocusEditTmpGuard();
    afx_msg void OnEnSetfocusEditExtCardDelay();
    afx_msg void OnBnClickedBtnPgmSave();
	afx_msg void OnEnSetfocusEditSotDelay();
	void MakeLog(LPCTSTR fmt, ...);
	void DeviceSetupChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<CString,CString>>> vData); 
	afx_msg void OnBnClickedBtnDefaultInsert();
	afx_msg void OnBnClickedBtnDefaultDelete();

	void DisableControl( CString strSltPgmName );
	afx_msg void OnEnSetfocusEditNotTestingTimeout();

};
