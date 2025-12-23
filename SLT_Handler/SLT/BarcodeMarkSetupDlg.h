#pragma once


// CBarcodeMarkSetupDlg 대화 상자입니다.

class CBarcodeMarkSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBarcodeMarkSetupDlg)

public:
	CBarcodeMarkSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBarcodeMarkSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_2D_MARK_SETUP_BIN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	CShadeButtonST m_BtnInsert;
	CShadeButtonST m_BtnDelete;
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	CSpreadSheet   m_SprdMark;

	
	int m_nMaxRow;

	void OnBtnGUI();
	void UpdateSpread();

//외부에서 받는 Data
public:
	ST_2D_MARK_INFO m_stMarkData;

public:
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedBtnMarkInsert();
	afx_msg void OnBnClickedBtnMarkDel();
	afx_msg void OnBnClickedBtnMarkSave();

	void BarcodeMarkSetupChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData);
};
