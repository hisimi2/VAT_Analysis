#pragma once

#include "afxwin.h"
// CLotInfoDlg 대화 상자입니다.

class CLotInfoAmkorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLotInfoAmkorDlg)

public:
	CLotInfoAmkorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLotInfoAmkorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOT_INFO_AMKOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetLogInLevel(int nLogInLevel);
private:
	// Current Data
	CString m_strLotNo;
	CString m_strOperID;
	CString m_strPGM_Name;
	CString m_strScheduleNo;
	CString m_strDeviceName;
	CString m_strCustomName;
	CString m_strProcess;
	CString m_strHostName;
	CString m_strProcessID;
	CString m_strTemp;
	CString m_strRetestCode;
	CString m_strQty;
	CString m_strArt;
	CString m_strTpRev;
	CString m_strSoakTime;
	CString m_strFuse;
	CString m_strHandler;
	CString m_strLoadBdNo;
	CString m_strSocketId;
	CString m_strCKitID;
	CString m_strFoundryLotId;
	CString m_strAssemblyStartdate;
	CString m_strAssemblyEnddate;
	CString m_StrAssyVendorLotid;
	CString m_strTestVendorLotid;
	CString m_strBotSubVendor;
	CString m_strNickname;
	CString m_str2DIDSortPath;
	CString m_strTheoreticalUPH;
	int m_nTheoreticalUPH;

	BOOL m_bLotNormalRC;
	BOOL m_bVisionUse;
	BOOL m_bVisionUseNoUseBak;
	int  m_nModeTesterIF;
	int  m_nFTRCQC;
	int  m_nLogInLevel;

	
	// btn ctrl
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnGetInfo;
	CShadeButtonST m_BtnLock;

	// member function
	void OnBtnGUI();
	void SetDisplayUserData(BOOL bFlag);
	void OnCopyLastButtonEvent(int nEventNo);
	
	void LotInfoLock(BOOL bLockState);
	BOOL SaveInrolock();
	BOOL SummaryInterlock();

	char m_szCompany[16];
	void CompanySetting();
	void BarcodeSortShowHide(int nShow);
	BOOL FileNameLimit(CString strName);
	BOOL ShowLogInBox();
	void CheckHandlerMode(int nID);

public:
	afx_msg void OnBnClickedButtonSave();	
	afx_msg void OnBnClickedRadioVision();
	afx_msg void OnBnClickedRadioNotVision();
	afx_msg void OnBnClickedGetInfo();
	void MakeLog(LPCTSTR fmt, ...);
	void LotInfoChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData);
	afx_msg void OnEnChangeEditLotNo();
	afx_msg void OnBnClickedLock();
	afx_msg void OnBnClickedRadioQa();
	afx_msg void OnBnClickedRadioQaContinue();

	afx_msg void OnRadioLotModeCtrl(UINT nID);
	afx_msg void OnEnSetfocusEditQaCnt();

	CSLTView* m_pSltView;
	int m_nQaSampleCnt;
	CEditTrans m_editQaSample;
	afx_msg void OnBnClickedRadioProduction();
	afx_msg void OnBnClickedRadioAudit();
	afx_msg void OnBnClickedRadioLoop();
	afx_msg void OnBnClickedRadioGrr();
	afx_msg void OnBnClickedButton2didSortPath();
};
