#pragma once
#include "afxwin.h"


// CSPD_TM_TRAY_PP 대화 상자입니다.

class CSPD_TM_TRAY_PP : public CDialog
{
	DECLARE_DYNAMIC(CSPD_TM_TRAY_PP)

public:
	CSPD_TM_TRAY_PP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_TM_TRAY_PP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPD_TM_TRAYPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void MakeLog(LPCTSTR fmt, ...);
	void TryPPSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>>& vData );

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	//Load Tray PP
	CEditTrans m_editSPD_LDTray_X;
	CEditTrans m_editACC_LDTray_X;
	CEditTrans m_editDCC_LDTray_X;
	CEditTrans m_editSPD_Gripper_Y;
	CEditTrans m_editACC_Gripper_Y;
 	CEditTrans m_editDCC_Gripper_Y;
	CEditTrans m_editSPD_LDTray_Z;
	CEditTrans m_editACC_LDTray_Z;
	CEditTrans m_editDCC_LDTray_Z;
	CEditTrans m_editSPD_LDTray_X_Pitch;
	CEditTrans m_editACC_LDTray_X_Pitch;
	CEditTrans m_editDCC_LDTray_X_Pitch;
	CEditTrans m_editSPD_LDTray_Y_Pitch;
	CEditTrans m_editACC_LDTray_Y_Pitch;
	CEditTrans m_editDCC_LDTray_Y_Pitch;

	//Unload Tray PP
	CEditTrans m_editSPD_ULTray_X;
	CEditTrans m_editACC_ULTray_X;
	CEditTrans m_editDCC_ULTray_X;
	CEditTrans m_editSPD_ULTray_Y;
	CEditTrans m_editACC_ULTray_Y;
	CEditTrans m_editDCC_ULTray_Y;
	CEditTrans m_editSPD_ULTray_Z;
	CEditTrans m_editACC_ULTray_Z;
	CEditTrans m_editDCC_ULTray_Z;
	CEditTrans m_editSPD_ULTray_X_Pitch;
	CEditTrans m_editACC_ULTray_X_Pitch;
	CEditTrans m_editDCC_ULTray_X_Pitch;
	CEditTrans m_editSPD_ULTray_Y_Pitch;
	CEditTrans m_editACC_ULTray_Y_Pitch;
	CEditTrans m_editDCC_ULTray_Y_Pitch;

	CEditTrans m_editTrayPP_Vacuum_LD;
	CEditTrans m_editTrayPP_Blow_LD;
	CEditTrans m_editTrayPP_Vacuum_UD;
	CEditTrans m_editTrayPP_Blow_UD;

	CEditTrans m_editTrayPP_Retry;
	CEditTrans m_editTrayPP_CylDownWait;
	CEditTrans m_editTrayPP_Pick_1Stage_Offset;
	CEditTrans m_editTrayPP_Pick_1Stage_Speed;
	CEditTrans m_editULTrayPP_Linear_Offset;

	void OnBtnGUI();
	void OnEditBoxGUI();

	//LD Tray PP
	int m_nSPD_LDTrayPP_X;
	int m_nACC_LDTrayPP_X;
	int m_nDCC_LDTrayPP_X;
 	int m_nSPD_Gripper_Y;
 	int m_nACC_Gripper_Y;
 	int m_nDCC_Gripper_Y;
	int m_nSPD_LDTrayPP_Z;
	int m_nACC_LDTrayPP_Z;
	int m_nDCC_LDTrayPP_Z;
	int m_nSPD_LDTrayPP_X_Pitch;
	int m_nACC_LDTrayPP_X_Pitch;
	int m_nDCC_LDTrayPP_X_Pitch;
	int m_nSPD_LDTrayPP_Y_Pitch;
	int m_nACC_LDTrayPP_Y_Pitch;
	int m_nDCC_LDTrayPP_Y_Pitch;

	//Unload Tray PP
	int m_nSPD_ULTrayPP_X;
	int m_nACC_ULTrayPP_X;
	int m_nDCC_ULTrayPP_X;
	int m_nSPD_ULTrayPP_Y;
	int m_nACC_ULTrayPP_Y;
	int m_nDCC_ULTrayPP_Y;
	int m_nSPD_ULTrayPP_Z;
	int m_nACC_ULTrayPP_Z;
	int m_nDCC_ULTrayPP_Z;
	int m_nSPD_ULTrayPP_X_Pitch;
	int m_nACC_ULTrayPP_X_Pitch;
	int m_nDCC_ULTrayPP_X_Pitch;
	int m_nSPD_ULTrayPP_Y_Pitch;
	int m_nACC_ULTrayPP_Y_Pitch;
	int m_nDCC_ULTrayPP_Y_Pitch;

	double m_dTrayPP_Vacuum_LD;
	double m_dTrayPP_Blow_LD;
	double m_dTrayPP_Vacuum_UD;
	double m_dTrayPP_Blow_UD;

	double m_dTrayPP_CylDownWait;
	int m_nRetryCnt;
	double m_dTrayPP_Pick_1Stage_Offset;
	double m_dTrayPP_Pick_1Stage_Speed;

	double m_dULTrayPP_Linear_Offset;

	CButton m_ChkPreVacuum;

public:	
	afx_msg void OnBnClickedButtonSave();
	
	afx_msg void OnEnSetfocusEditSpdX();
	afx_msg void OnEnSetfocusEditAccX();
	afx_msg void OnEnSetfocusEditDccX();
	afx_msg void OnEnSetfocusEditSpdY();
	afx_msg void OnEnSetfocusEditAccY();
	afx_msg void OnEnSetfocusEditDccY();
	afx_msg void OnEnSetfocusEditSpdZ();
	afx_msg void OnEnSetfocusEditAccZ();
	afx_msg void OnEnSetfocusEditDccZ();
	afx_msg void OnEnSetfocusEditSpdXPitch();
	afx_msg void OnEnSetfocusEditAccXPitch();
	afx_msg void OnEnSetfocusEditDccXPitch();
	afx_msg void OnEnSetfocusEditSpdYPitch();
	afx_msg void OnEnSetfocusEditAccYPitch();
	afx_msg void OnEnSetfocusEditDccYPitch();
	afx_msg void OnEnSetfocusEditTrayVacuumDelay();
	afx_msg void OnEnSetfocusEditTrayBlowDelay();
	afx_msg void OnEnSetfocusEditTrayRetry();
	afx_msg void OnEnSetfocusEditTrayCylDownWait();
	afx_msg void OnEnSetfocusEditSpdXUlTray();
	afx_msg void OnEnSetfocusEditAccXUlTray();
	afx_msg void OnEnSetfocusEditDccXUlTray();
	afx_msg void OnEnSetfocusEditSpdYUlTray();
	afx_msg void OnEnSetfocusEditAccYUlTray();
	afx_msg void OnEnSetfocusEditDccYUlTray();
	afx_msg void OnEnSetfocusEditSpdZUlTray();
	afx_msg void OnEnSetfocusEditAccZUlTray();
	afx_msg void OnEnSetfocusEditDccZUlTray();
	afx_msg void OnEnSetfocusEditSpdXPitchUlTray();
	afx_msg void OnEnSetfocusEditAccXPitchUlTray();
	afx_msg void OnEnSetfocusEditDccXPitchUlTray();
	afx_msg void OnEnSetfocusEditSpdYPitchUlTray();
	afx_msg void OnEnSetfocusEditAccYPitchUlTray();
	afx_msg void OnEnSetfocusEditDccYPitchUlTray();
	afx_msg void OnEnSetfocusEditSpdYGripper();
	afx_msg void OnEnSetfocusEditAccYGripper();
	afx_msg void OnEnSetfocusEditDccYGripper();
	afx_msg void OnEnSetfocusEditTrayVacuumDelay2();
	afx_msg void OnEnSetfocusEditTrayBlowDelay2();
	afx_msg void OnEnSetfocusEditTrayPick1stageOffset();
	afx_msg void OnEnSetfocusEditTrayPick1stageSpeed();
	afx_msg void OnEnSetfocusEditTrayUnloadTrayPpLinearOffset();
};
