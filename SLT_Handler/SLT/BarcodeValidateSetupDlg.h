#pragma once


// CBarcodeValidateSetupDlg 대화 상자입니다.

class CBarcodeValidateSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBarcodeValidateSetupDlg)

public:
	CBarcodeValidateSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBarcodeValidateSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_2D_BARCODE_VALIDATE };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void MakeLog(LPCTSTR fmt, ...);

	virtual BOOL OnInitDialog();
	
private:
	//================================GUI Button
	CShadeButtonST m_BtnNew;
	CShadeButtonST m_BtnDel;
	CShadeButtonST m_BtnModify;

	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;
	
	void InitBtnGUI();
	

	//============================Option Check 
	CButton m_CkBtn2dBarcode;
	CButton m_CkBtnDuplication;
	CButton m_CkBtn2dLength;
	CButton m_CkBtn2dMark;

	void InitChkBtnStatus();


	//=========================Setting Data
	//Length
	int m_n2DLength;
	CEditTrans m_Edit2DLength;

	//Mark
	CEdit m_EditDeviceName;

	int m_n2DMarkStartIdx;
	CEditTrans m_Edit2DMarkStartIdx;

	//CString m_strCurDeviceName;
	ST_2D_MARK_INFO m_stBarcodeMark;

	CListBox  m_ListDeviceName;
	void UpdateDeviceNameList();

	CListCtrl m_ListMark;
	void UpdateMarkList();

	BOOL IsLotOnGoing(); //현재 사용 안함.

	void LoadCurMarkInfo();

	void DisplayCurDeviceName();

	//Init
	void InitEditDataGUI();

public:
	void ChkOptionAuthority(CButton *btnOption);

	//Check Box Clicked
	afx_msg void OnBnClickedCheck2dBarcodeUse();
	afx_msg void OnBnClickedCheckDup2dBarcode();
	afx_msg void OnBnClickedCheck2dLength();
	afx_msg void OnBnClickedCheck2dBarcodeMark();

	//2d Length 입력
	afx_msg void OnEnSetfocusEditDeviceLength();

	//2d Mark
	afx_msg void OnEnSetfocusEditDeviceMarkStartIdx();
	afx_msg void OnLbnSelchangeListDeviceName();
	afx_msg void OnBnClicked2dValidateBtnNew();
	afx_msg void OnBnClicked2dValidateBtnDel();
	afx_msg void OnBnClicked2dValidateBtnModify();


	afx_msg void OnBnClicked2dValidateBtnSave();
	void BarcodeValidateOptChangeLog(std::vector<std::pair<CString, std::pair<double, double>>> vData);
	void BarcodeMarkChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData);

	afx_msg void OnEnSetfocusEditDeviceName();

public:
	enum _eValidateType
	{
		_eValidateType_Duplication = 0,
		_eValidateType_Length,
		_eValidateType_Mark,

		_eValidateType_Max,
	};

	void EnableGUI_MainOpt();
	void EnableGUI_SubOpt(int eType, BOOL bTR_FL);
};
