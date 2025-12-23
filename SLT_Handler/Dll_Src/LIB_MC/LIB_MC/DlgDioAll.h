#pragma once

#include "resource.h"
#include "StaticTrans.h"
#include "ShadeButtonST.h"

#define MAX_DIO_ONE_PAGE      (24)

// CDlgDioAll 대화 상자입니다.

class CDlgDioAll : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDioAll)

public:
	CDlgDioAll(int nDlgUsage, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDioAll();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IO_DIALOG };

	enum { eDlgUsageDIO = 0, eDlgUsageAIO};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	CShadeButtonST m_btnDO		  [MAX_DIO_ONE_PAGE];
	CShadeButtonST m_btnDI		  [MAX_DIO_ONE_PAGE];
	CStaticTrans   m_staticDO_Name[MAX_DIO_ONE_PAGE];
	CStaticTrans   m_staticDI_Name[MAX_DIO_ONE_PAGE];
	CStaticTrans   m_staticDO_Title;
	CStaticTrans   m_staticDI_Title;
	CShadeButtonST m_btnDO_UP;
	CShadeButtonST m_btnDO_DOWN;
	CShadeButtonST m_btnDI_UP;
	CShadeButtonST m_btnDI_DOWN;
	CShadeButtonST m_btnExit;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void InitControl();
	void Update_IO_Name(int nDO_PageNum=-1, int nDI_PageNum=-1);
	void Update_IO_Status(int nDO_PageNum=-1, int nDI_PageNum=-1);

private:
	int     m_nDlgUsage;            // 0:DIO, 1=AIO
	int		m_nCur_PageNum_DO;		// 0 base
	int		m_nCur_PageNum_DI;		// 0 base
	int     m_nDO_ButtonID[MAX_DIO_ONE_PAGE];
	int     m_nDI_ButtonID[MAX_DIO_ONE_PAGE];
	int     m_nDO_StaticID[MAX_DIO_ONE_PAGE];
	int     m_nDI_StaticID[MAX_DIO_ONE_PAGE];

public:
	afx_msg void OnBnClickedBtnOutUp();
	afx_msg void OnBnClickedBtnOutDown();
	afx_msg void OnBnClickedBtnInUp();
	afx_msg void OnBnClickedBtnInDown();
	afx_msg void OnBnClickedBtnOut(UINT nCtrlID);
	afx_msg void OnBnClickedOk();
};
