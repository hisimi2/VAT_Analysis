#pragma once
#include "afxwin.h"


// CDlgAxisActionMove 대화 상자입니다.

class CDlgAxisActionMove : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAxisActionMove)

public:
	CDlgAxisActionMove(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAxisActionMove();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_AXIS_ACTION };
	enum _ePICKER_NUM { eMAX_X_PICKER = 8, eMAX_Y_PICKER = 2};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int m_nStartPoint;
	int m_nSelectAction;
	int m_nSelectDevice;
	//Test PP 관련.
	int m_nSelectHandType;
	int ConvertTestPPCmdStage(int nEndPointIdx);

	//Tray 및 Feeder 연동하여 움직이기 위함.
	int m_nCmdStage_auto;
	std::vector<CPoint> m_vPicker_auto;
	
	BOOL			m_bChkTime;
	double           m_tcTimeout;
private:
	void OnUpdateStartPointText();
	void OnUpdateItemList();
	void OnUpdateEndPointCombo();

	CComboBox m_CmbEndPoint;
	CFont m_CmbFont;
	CButton m_ChkHand1_1;
	CButton m_ChkHand1_2;
	CButton m_ChkHand1_3;
	CButton m_ChkHand1_4;
	CButton m_ChkHand1_5;
	CButton m_ChkHand1_6;
	CButton m_ChkHand1_7;
	CButton m_ChkHand1_8;
	CButton m_ChkHand2_1;
	CButton m_ChkHand2_2;
	CButton m_ChkHand2_3;
	CButton m_ChkHand2_4;
	CButton m_ChkHand2_5;
	CButton m_ChkHand2_6;
	CButton m_ChkHand2_7;
	CButton m_ChkHand2_8;
	CButton m_ChkPickPlaceEnd;

	BOOL CheckUI_InterLock(int nUseHand[ eMAX_Y_PICKER ][ eMAX_X_PICKER ]);
	CPoint Chk_Select( int _nUseHand[ eMAX_Y_PICKER ][ eMAX_X_PICKER ] );

	double m_dDelayTime;
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonSetY1();
	afx_msg void OnBnClickedButtonSetY2();
	afx_msg void OnBnClickedButtonSetAll();
	afx_msg void OnBnClickedButtonManuMove();
    afx_msg void OnBnClickedButtonSetLoad();
    afx_msg void OnBnClickedButtonSetUnload();
    afx_msg void OnBnClickedButtonSetClear();
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnEnSetfocusEditDelayTime();
	afx_msg void OnRadioSeletTestHandType(UINT id);
	afx_msg void OnBnClickedRadioSelDeviceNormal(UINT id);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioSelLoadHand();
	afx_msg void OnCbnSelchangeComboEndMovePos();

};
