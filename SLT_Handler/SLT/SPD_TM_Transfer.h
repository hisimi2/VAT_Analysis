#pragma once


// CSPD_TM_Transfer 대화 상자입니다.

class CSPD_TM_Transfer : public CDialog
{
	DECLARE_DYNAMIC(CSPD_TM_Transfer)

public:
	CSPD_TM_Transfer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_TM_Transfer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPD_TM_TRANSFER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void MakeLog(LPCTSTR fmt, ...);
	void TransferSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData);

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	void OnBtnGUI();
	void OnEditBoxGUI();

	int m_nSPD_Trans_X;
	int m_nACC_Trans_X;
	int m_nDCC_Trans_X;
	double m_dTransfer_CylDownWait;

	double m_dStacker_MainWait;
	double m_dStacker_SubWait;
	double m_dStacker_ClampWait;
	double m_dConveyorUnload_StopWait;
	double m_dConveyorload_StopWait;

	int m_nSlowUnloadStackerOption;

	CButton m_rbChkSlowUnStkUse;
	CButton m_rbChkSlowUnStkNotUse;

	CEditTrans m_editSPD_Trans_X;
	CEditTrans m_editACC_Trans_X;
	CEditTrans m_editDCC_Trans_X;
	CEditTrans m_editTransfer_CylDownWait;

	CEditTrans m_editStacker_CylMainUpDownWait;
	CEditTrans m_editStacker_CylSubUpDownWait;
	CEditTrans m_editStacker_CylClampUnclampWait;
	CEditTrans m_editConveyorUnload_StopWait;
	CEditTrans m_editConveyorload_StopWait;


public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnEnSetfocusEditSpdTransX();
	afx_msg void OnEnSetfocusEditAccTransX();
	afx_msg void OnEnSetfocusEditDccTransX();
	afx_msg void OnEnSetfocusEditTransferCylDownWait();
	afx_msg void OnEnSetfocusEditStackerCylMainWait();
	afx_msg void OnEnSetfocusEditStackerCylSubWait();
	afx_msg void OnEnSetfocusEditStackerCylClampWait();
	afx_msg void OnEnSetfocusEditStackerVeyorWait();
	afx_msg void OnEnSetfocusEditStackerVeyorLoadWait();
	afx_msg void OnBnClickedRadioSlowUnloadStackerOn();
	afx_msg void OnBnClickedRadioSlowUnloadStackerOff();
};
