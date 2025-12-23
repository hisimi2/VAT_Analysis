#pragma once
#include "afxwin.h"


// CTrayViewTestSite 대화 상자입니다.
class CTrayViewTestSite : public CDialogEx
{
	DECLARE_DYNAMIC(CTrayViewTestSite)

public:
	CTrayViewTestSite(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTrayViewTestSite();

private:
	CSpreadSheet m_SprdTestSite[eMaxPressUnitCount];

	void InitSpread();
	void UpdateSpreadTestSite(int nStationNo);
	void SetDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);
	void SetDisplayGrr(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);
	void ClassifyDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRAYVIEW_TESTSITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	CComboBox m_CtrlComboKindView;
	void InitColorView();
};
