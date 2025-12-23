#pragma once
#include "afxwin.h"


// CSPD_TM_SHUTTLE 대화 상자입니다.

class CSPD_TM_SHUTTLE : public CDialog
{
	DECLARE_DYNAMIC(CSPD_TM_SHUTTLE)

public:
	CSPD_TM_SHUTTLE(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_TM_SHUTTLE();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPD_TM_TBL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//GUI Data Chang Log
	void MakeLog(LPCTSTR fmt, ...);
	// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	void LoadTableSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData);

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	void OnBtnGUI();
	void OnEditBoxGUI();

	int m_nSPD_LoadTbl1_Y;
	int m_nACC_LoadTbl1_Y;
	int m_nDCC_LoadTbl1_Y;
	int m_nSPD_LoadTbl2_Y;
	int m_nACC_LoadTbl2_Y;
	int m_nDCC_LoadTbl2_Y;
	int m_nSPD_LoadTbl3_Y;
	int m_nACC_LoadTbl3_Y;
	int m_nDCC_LoadTbl3_Y;
	int m_nSPD_LoadTbl4_Y;
	int m_nACC_LoadTbl4_Y;
	int m_nDCC_LoadTbl4_Y;

	int m_nSPD_LoadTbl1_Rotate;
	int m_nACC_LoadTbl1_Rotate;
	int m_nDCC_LoadTbl1_Rotate;
	int m_nSPD_LoadTbl2_Rotate;
	int m_nACC_LoadTbl2_Rotate;
	int m_nDCC_LoadTbl2_Rotate;
	int m_nSPD_LoadTbl3_Rotate;
	int m_nACC_LoadTbl3_Rotate;
	int m_nDCC_LoadTbl3_Rotate;
	int m_nSPD_LoadTbl4_Rotate;
	int m_nACC_LoadTbl4_Rotate;
	int m_nDCC_LoadTbl4_Rotate;


	int m_nBarcodeRetry;

	int m_nVibratorConut;
	double m_dVibratorDelayTime;
	int m_nViratorOption;

	double m_dLoadTableWaitTime;

	int m_nNumberOfLoadTable;

	CEditTrans m_editSPD_LoadTbl1_X;
	CEditTrans m_editACC_LoadTbl1_X;
	CEditTrans m_editDCC_LoadTbl1_X;
	CEditTrans m_editSPD_LoadTbl1_Y;
	CEditTrans m_editACC_LoadTbl1_Y;
	CEditTrans m_editDCC_LoadTbl1_Y;
	CEditTrans m_editSPD_LoadTbl2_Y;
	CEditTrans m_editACC_LoadTbl2_Y;
	CEditTrans m_editDCC_LoadTbl2_Y;
	CEditTrans m_editSPD_LoadTbl3_Y;
	CEditTrans m_editACC_LoadTbl3_Y;
	CEditTrans m_editDCC_LoadTbl3_Y;
	CEditTrans m_editSPD_LoadTbl4_Y;
	CEditTrans m_editACC_LoadTbl4_Y;
	CEditTrans m_editDCC_LoadTbl4_Y;

	CEditTrans m_editSPD_LoadTbl1_Rotate;
	CEditTrans m_editACC_LoadTbl1_Rotate;
	CEditTrans m_editDCC_LoadTbl1_Rotate;
	CEditTrans m_editSPD_LoadTbl2_Rotate;
	CEditTrans m_editACC_LoadTbl2_Rotate;
	CEditTrans m_editDCC_LoadTbl2_Rotate;
	CEditTrans m_editSPD_LoadTbl3_Rotate;
	CEditTrans m_editACC_LoadTbl3_Rotate;
	CEditTrans m_editDCC_LoadTbl3_Rotate;
	CEditTrans m_editSPD_LoadTbl4_Rotate;
	CEditTrans m_editACC_LoadTbl4_Rotate;
	CEditTrans m_editDCC_LoadTbl4_Rotate;

	CEditTrans m_editBarcodRetry;

	CEditTrans m_EditReCnt;
	CEditTrans m_EditDelayTime;

	CEditTrans m_EditWaitTime;
	
	int m_bChkVibUse;


	int m_nSPD_Y_Barcode_Limit;
	CEditTrans m_editSPD_Y_Barcode_Limit;


	void OnBnClickedRadioVibratorUse();
	void OnBnClickedRadioVibratorNotUse();

public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnEnSetfocusEditSpdSh1Y();
	afx_msg void OnEnSetfocusEditAccSh1Y();
	afx_msg void OnEnSetfocusEditDccSh1Y();
	afx_msg void OnEnSetfocusEditSpdSh2Y();
	afx_msg void OnEnSetfocusEditAccSh2Y();
	afx_msg void OnEnSetfocusEditDccSh2Y();
    CButton m_CtrlArrivedSen;
	CButton m_CtrlAfterMoveArrivedSen;
	CButton m_BtnImpactTbl;
	CButton m_Btn2dTestSiteMove;

	afx_msg void OnClickedVibratorUse(UINT nID);
private:
	int m_n2D_BarcodeBak;
public:
	afx_msg void OnEnSetfocusEditBarcodeRetry();
	afx_msg void OnEnSetfocusEditVibratorCnt();
	afx_msg void OnEnSetfocusEditVibratorDelayTime();
	afx_msg void OnEnSetfocusEditLoadTableWaitTime();
	afx_msg void OnEnSetfocusEditSpdSh3Y();
	afx_msg void OnEnSetfocusEditAccSh3Y();
	afx_msg void OnEnSetfocusEditDccSh3Y();
	afx_msg void OnEnSetfocusEditSpdSh4Y();
	afx_msg void OnEnSetfocusEditAccSh4Y();
	afx_msg void OnEnSetfocusEditDccSh4Y();
	afx_msg void OnEnSetfocusEditSpdSh1Rotate();
	afx_msg void OnEnSetfocusEditAccSh1Rotate();
	afx_msg void OnEnSetfocusEditDccSh1Rotate();
	afx_msg void OnEnSetfocusEditSpdSh2Rotate();
	afx_msg void OnEnSetfocusEditAccSh2Rotate();
	afx_msg void OnEnSetfocusEditDccSh2Rotate();
	afx_msg void OnEnSetfocusEditSpdSh3Rotate();
	afx_msg void OnEnSetfocusEditAccSh3Rotate();
	afx_msg void OnEnSetfocusEditDccSh3Rotate();
	afx_msg void OnEnSetfocusEditSpdSh4Rotate();
	afx_msg void OnEnSetfocusEditAccSh4Rotate();
	afx_msg void OnEnSetfocusEditDccSh4Rotate();
	afx_msg void OnEnSetfocusEditBarcodeLimitSpd();
	afx_msg void OnBnClickedButtonTableVacuumCheck();

	afx_msg void OnBnClickedCheckArrivedSen();

	afx_msg void OnBnClickedRadioNumberOfLoadtable4();
	afx_msg void OnBnClickedRadioNumberOfLoadtable8();
};
