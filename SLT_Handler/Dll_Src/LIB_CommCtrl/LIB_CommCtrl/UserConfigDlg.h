#pragma once


// CUserConfigDlg 대화 상자입니다.
#include "DEF_LIB_COMMCTRL.h"

#pragma once

#include "resource.h"
#include "../Include/OcxFiles/nokey.h"
#include "../Include/OcxFiles/spreadsheet.h"

class CUserConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserConfigDlg)

public:
	CUserConfigDlg(ST_USER_CONFIG_ARRAY stUserConfig, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_OPER_CONFIG };

	ST_USER_CONFIG_ARRAY  m_StUserConfigSet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CSpreadSheet	m_FlexGrid;
	int             m_nSpreadHeader;
	void            InitSpread();
	void			SetItemList();
	void            GetItemList();	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSystem();
};
