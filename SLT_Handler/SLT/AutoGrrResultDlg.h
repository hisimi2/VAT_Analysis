#pragma once
#include "afxwin.h"

// CAutoGrrResultDlg 대화 상자입니다.

class CAutoGrrResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoGrrResultDlg)

public:
	CAutoGrrResultDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoGrrResultDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_AUTOGRR_RESULT };

	void Initialize();
	void Finalize();

	//DB에 있는 Data를 들고 온다
	void GetDataToDB();

	//Spread에 Data를 뿌려준다
	void ViewDatafromSpread();
	void ViewStationDatafromSpread();

	void InitSpread();
	void InitSpreadStation();
	void DropSpread();

	CMyList m_List;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	CCriticalSection m_cs;
	CSpreadSheet m_SprdResult;
	CSpreadSheet m_SprdStationPF;
	int m_nAutoGrrTestCount;
	int m_nAutoGrrUnitCount;
	int m_nTestSiteNum;
	int m_nBeforeDataCnt;
	int m_nDataCnt;

	CComboBox m_CtrlCombo;

	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	DECLARE_EVENTSINK_MAP()
	void ClickSpreadGrrResult(long Col, long Row);
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedCombo();
	void InitSpreadResult2(int nStationNo);
};
