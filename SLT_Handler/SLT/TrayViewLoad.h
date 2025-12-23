#pragma once


// CTrayViewLoad 대화 상자입니다.

class CTrayViewLoad : public CDialog
{
	DECLARE_DYNAMIC(CTrayViewLoad)

public:
	CTrayViewLoad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTrayViewLoad();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRAYVIEW_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CSpreadSheet m_SprdSetplateLD1;
	CSpreadSheet m_SprdFeeder;
	CSpreadSheet m_SprdLoadTbl1;
	CSpreadSheet m_SprdLoadTbl2;
	CSpreadSheet m_SprdLoadTbl3;
	CSpreadSheet m_SprdLoadTbl4;

	CSpreadSheet m_SprdLoadTrayPP;
	CSpreadSheet m_SprdUnloadTrayPP;
	CSpreadSheet m_SprdTestPP1_LD;
	CSpreadSheet m_SprdTestPP1_UD;
	CSpreadSheet m_SprdTestPP2_LD;
	CSpreadSheet m_SprdTestPP2_UD;


	CSpreadSheet m_SprdTestPP1_Clean;
	CSpreadSheet m_SprdTestPP2_Clean;

	void InitSpread();
	void SetDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows, int nTestPPType = -1);
	void SetDisplayGrr(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows, int nTestPPType = -1);
	void ClassifyDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);
	void UpdateSpreadLoadSetplate();
	void UpdateSpreadFeeder();
	void UpdateSpreadLoadTbl();
	void UpdateSpreadTrayPP();
	void UpdateSpreadTestPP();
    void UpdateSpreadBuffTbl();
	void UpdateSpreadCleanTbl();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	CComboBox m_CtrlComboKindView;
	void InitColorView();
};
