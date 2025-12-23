#pragma once
#include "afxwin.h"


// CDlgDeviceInfoEditor 대화 상자입니다.

class CDlgDeviceInfoEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDeviceInfoEditor)

public:
	CDlgDeviceInfoEditor( CWnd* pParent = NULL );   // 표준 생성자입니다.
	CDlgDeviceInfoEditor( int _device_stage, int _x, int _y );   // 표준 생성자입니다.
	virtual ~CDlgDeviceInfoEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DEVICE_INFO_EDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private :
	ST_DD_DEVICE m_device_data;
	int m_device_stage;
	int m_x;
	int m_y;
public:
	virtual BOOL OnInitDialog();
	void ShowTestCount( int _station_num );
	afx_msg void OnBnClickedOk();
	CComboBox m_combo_station_select;
	CSpreadSheet m_spread_test_count;
	CButton m_checkbox_exist;
	CEdit m_edit_bin_code;
	CEdit m_edit_max_test_cnt;
	DECLARE_EVENTSINK_MAP()
	void ChangeFpspreadDeviceInfoTestCnt( long Col, long Row );
	afx_msg void OnCbnSelchangeComboDeviceInfoStationSelect();
	void SetTestCnt( int _station_num, int _cnt );
	CEdit m_edit_total_station_cnt;
	void KeyDownFpspreadDeviceInfoTestCnt( short* KeyCode, short Shift );
	afx_msg void OnBnClickedButtonDeviceInfoSetAllStationTestCnt();
	CEdit m_edit_test_cnt;
	afx_msg void OnBnClickedButtonDeviceInfoSetLeftModuleTestCnt();
	afx_msg void OnBnClickedButtonDeviceInfoSetRightModuleTestCnt();
};
