
#pragma once


// CDataAssignDlg 대화 상자입니다.

class CDataAssignDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataAssignDlg)

public:
	CDataAssignDlg(BOOL bModaless=FALSE, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDataAssignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DVC_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	int m_nPosNo;
	int m_nDeviceData[DEVICE_DATA_MAX_Y][DEVICE_DATA_MAX_X];

	int m_nDeviceDataBefore[DEVICE_DATA_MAX_Y][DEVICE_DATA_MAX_X];

	int m_nCntY;
	int m_nCntX;

	CString m_strTitle;
	CSpreadSheet m_CS_SET_DATA;

	BOOL m_bSkiptoAssign;
	BOOL m_bModeModaless;

	void SetDataSpread(int nData);
	void NumResetting();
	void UpdateDeviceData();
	void ChkLotState();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSitesetSave();
	afx_msg void OnBnClickedBtnSetplate1();
	afx_msg void OnBnClickedBtnSetplate2();
	afx_msg void OnBnClickedBtnTbl1();
	afx_msg void OnBnClickedBtnTbl2();
	afx_msg void OnBnClickedBtnTbl3();
	afx_msg void OnBnClickedBtnTbl4();
	afx_msg void OnBnClickedButtonDvcExist();
	afx_msg void OnBnClickedButtonDvcNoexist();
	void DataAssignChangeLog();
	void MakeLog(LPCTSTR fmt, ...);
	void SetPosNum(int nPos){m_nPosNo = nPos;}
	void SetSkiptoAssignFlag(BOOL bFlag){m_bSkiptoAssign = bFlag;}
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonDvcExistAll();
	afx_msg void OnBnClickedButtonDvcNoexistAll();
	void SetDataSpreadAll(int nData);
	afx_msg void OnBnClickedButtonTrayppSafetyMove();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
