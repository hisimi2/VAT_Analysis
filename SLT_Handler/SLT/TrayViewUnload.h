#pragma once


// CTrayViewUnload 대화 상자입니다.

class CTrayViewUnload : public CDialog
{
	DECLARE_DYNAMIC(CTrayViewUnload)

public:
	CTrayViewUnload(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTrayViewUnload();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRAYVIEW_UNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CSpreadSheet m_SprdStacker;
	CSpreadSheet m_SprdBarcode;
	CSpreadSheet m_SprdBarcode2;

	void InitSpread();
	void UpdateSpreadUnLoadSetplate();

	void ClassifyDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);
	void SetDisplayGrr( CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows );
	void SetDisplayBinColor(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);
	void SetDisplayBarcode( CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows );
	void SetDisplayBarcode2(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	CComboBox m_CtrlComboPos;
	CComboBox m_CtrlComboKind;
};
