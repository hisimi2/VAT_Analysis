#pragma once
#include "afxwin.h"


// CSPD_TM_CONTACTOR 대화 상자입니다.

class CSPD_TM_CONTACTOR : public CDialog
{
	DECLARE_DYNAMIC(CSPD_TM_CONTACTOR)

public:
	CSPD_TM_CONTACTOR(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_TM_CONTACTOR();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPD_TM_PRESS_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void MakeLog(LPCTSTR fmt, ...);
	// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	void PressUnitSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData);

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	void OnBtnGUI();
	void OnEditBoxGUI();

	int m_nSPD_PressUnit1;//1
	int m_nACC_PressUnit1;
	int m_nDCC_PressUnit1;
	int m_nSPD_PressUnit2;//2
	int m_nACC_PressUnit2;
	int m_nDCC_PressUnit2;
	int m_nSPD_PressUnit3;//3
	int m_nACC_PressUnit3;
	int m_nDCC_PressUnit3;
	int m_nSPD_PressUnit4;//4
	int m_nACC_PressUnit4;
	int m_nDCC_PressUnit4;
	int m_nSPD_PressUnit5;//5
	int m_nACC_PressUnit5;
	int m_nDCC_PressUnit5;
	int m_nSPD_PressUnit6;//6
	int m_nACC_PressUnit6;
	int m_nDCC_PressUnit6;
	int m_nSPD_PressUnit7;//7
	int m_nACC_PressUnit7;
	int m_nDCC_PressUnit7;
	int m_nSPD_PressUnit8;//8
	int m_nACC_PressUnit8;
	int m_nDCC_PressUnit8;
	
	double m_dBlow_PressUnit1;
	double m_dBlow_PressUnit2;
	double m_dBlow_PressUnit3;
	double m_dBlow_PressUnit4;
	double m_dBlow_PressUnit5;
	double m_dBlow_PressUnit6;
	double m_dBlow_PressUnit7;
	double m_dBlow_PressUnit8;

	double m_dPressUnit_Blow_Pos1;
	double m_dPressUnit_Blow_Pos2;
	double m_dPressUnit_Blow_Pos3;
	double m_dPressUnit_Blow_Pos4;
	double m_dPressUnit_Blow_Pos5;
	double m_dPressUnit_Blow_Pos6;
	double m_dPressUnit_Blow_Pos7;
	double m_dPressUnit_Blow_Pos8;

	double m_dPressUnit_Clean_Offset1;
	double m_dPressUnit_Clean_Offset2;
	double m_dPressUnit_Clean_Offset3;
	double m_dPressUnit_Clean_Offset4;
	double m_dPressUnit_Clean_Offset5;
	double m_dPressUnit_Clean_Offset6;
	double m_dPressUnit_Clean_Offset7;
	double m_dPressUnit_Clean_Offset8;

	int m_nVibratorConut;
	double m_dVibratorDelayTime;

	int m_nAlwayVibratorConut;
	double m_dAlwayVibratorDelayTime;

	int m_nReContactConut;
	double m_dReContactOffset;
	double m_dReContactDelayTime;

	double m_dContactPosOffset;
	int m_nContact2stSpeed;
	double m_dCleaning2ndZOffset;

	double m_dSocketCleanBlowPos;
	double m_dSocketCleanBlowTime;
	
	CEditTrans m_editSPD_PressUnit1;//1
	CEditTrans m_editACC_PressUnit1;
	CEditTrans m_editDCC_PressUnit1;
	CEditTrans m_editSPD_PressUnit2;//2
	CEditTrans m_editACC_PressUnit2;
	CEditTrans m_editDCC_PressUnit2;
	CEditTrans m_editSPD_PressUnit3;//3
	CEditTrans m_editACC_PressUnit3;
	CEditTrans m_editDCC_PressUnit3;
	CEditTrans m_editSPD_PressUnit4;//4
	CEditTrans m_editACC_PressUnit4;
	CEditTrans m_editDCC_PressUnit4;
	CEditTrans m_editSPD_PressUnit5;//5
	CEditTrans m_editACC_PressUnit5;
	CEditTrans m_editDCC_PressUnit5;
	CEditTrans m_editSPD_PressUnit6;//6
	CEditTrans m_editACC_PressUnit6;
	CEditTrans m_editDCC_PressUnit6;
	CEditTrans m_editSPD_PressUnit7;//7
	CEditTrans m_editACC_PressUnit7;
	CEditTrans m_editDCC_PressUnit7;
	CEditTrans m_editSPD_PressUnit8;//8
	CEditTrans m_editACC_PressUnit8;
	CEditTrans m_editDCC_PressUnit8;

	CEditTrans m_editBlow_PressUnit1;
	CEditTrans m_editBlow_PressUnit2;
	CEditTrans m_editBlow_PressUnit3;
	CEditTrans m_editBlow_PressUnit4;
	CEditTrans m_editBlow_PressUnit5;
	CEditTrans m_editBlow_PressUnit6;
	CEditTrans m_editBlow_PressUnit7;
	CEditTrans m_editBlow_PressUnit8;


	CEditTrans m_edit_PressUnit_Blow_Pos1;
	CEditTrans m_edit_PressUnit_Blow_Pos2;
	CEditTrans m_edit_PressUnit_Blow_Pos3;
	CEditTrans m_edit_PressUnit_Blow_Pos4;
	CEditTrans m_edit_PressUnit_Blow_Pos5;
	CEditTrans m_edit_PressUnit_Blow_Pos6;
	CEditTrans m_edit_PressUnit_Blow_Pos7;
	CEditTrans m_edit_PressUnit_Blow_Pos8;

	CEditTrans m_edit_PressUnit_Clean_Offset1;
	CEditTrans m_edit_PressUnit_Clean_Offset2;
	CEditTrans m_edit_PressUnit_Clean_Offset3;
	CEditTrans m_edit_PressUnit_Clean_Offset4;
	CEditTrans m_edit_PressUnit_Clean_Offset5;
	CEditTrans m_edit_PressUnit_Clean_Offset6;
	CEditTrans m_edit_PressUnit_Clean_Offset7;
	CEditTrans m_edit_PressUnit_Clean_Offset8;


	CEditTrans m_EditReCnt;
	CEditTrans m_EditDelayTime;

	CEditTrans m_EditAlwayReCnt;
	CEditTrans m_EditAlwayDelayTime;

	CEditTrans m_EditReContactCnt;
	CEditTrans m_EditReContactOffset;
	CEditTrans m_EditReContactDelayTime;

	CEditTrans m_EditContactPosOffset;
	CEditTrans m_EditContact2stSpeed;
	CEditTrans m_EditCleaning2ndZOffset;

	CEditTrans m_EditSocketCleanBlowPos;
	CEditTrans m_EditSocketCleanBlowTime;

	CStatic m_stContact; 
	CStatic m_stClean;


public:		
	afx_msg void OnBnClickedButtonSave();
	
	afx_msg void OnEnSetfocusEditSpdContact1();
	afx_msg void OnEnSetfocusEditAccContact1();
	afx_msg void OnEnSetfocusEditDccContact1();
	afx_msg void OnEnSetfocusEditSpdContact2();
	afx_msg void OnEnSetfocusEditAccContact2();
	afx_msg void OnEnSetfocusEditDccContact2();
	afx_msg void OnEnSetfocusEditSpdContact3();
	afx_msg void OnEnSetfocusEditAccContact3();
	afx_msg void OnEnSetfocusEditDccContact3();
	afx_msg void OnEnSetfocusEditSpdContact4();
	afx_msg void OnEnSetfocusEditAccContact4();
	afx_msg void OnEnSetfocusEditDccContact4();
	afx_msg void OnEnSetfocusEditSpdContact5();
	afx_msg void OnEnSetfocusEditAccContact5();
	afx_msg void OnEnSetfocusEditDccContact5();
	afx_msg void OnEnSetfocusEditSpdContact6();
	afx_msg void OnEnSetfocusEditAccContact6();
	afx_msg void OnEnSetfocusEditDccContact6();
	afx_msg void OnEnSetfocusEditSpdContact7();
	afx_msg void OnEnSetfocusEditAccContact7();
	afx_msg void OnEnSetfocusEditDccContact7();
	afx_msg void OnEnSetfocusEditSpdContact8();
	afx_msg void OnEnSetfocusEditAccContact8();
	afx_msg void OnEnSetfocusEditDccContact8();


	afx_msg void OnEnSetfocusEditBlowTime1();
	afx_msg void OnEnSetfocusEditBlowTime2();
	afx_msg void OnEnSetfocusEditBlowTime3();
	afx_msg void OnEnSetfocusEditBlowTime4();
	afx_msg void OnEnSetfocusEditBlowTime5();
	afx_msg void OnEnSetfocusEditBlowTime6();
	afx_msg void OnEnSetfocusEditBlowTime7();
	afx_msg void OnEnSetfocusEditBlowTime8();

	afx_msg void OnEnSetfocusEditBlowPos1();
	afx_msg void OnEnSetfocusEditBlowPos2();
	afx_msg void OnEnSetfocusEditBlowPos3();
	afx_msg void OnEnSetfocusEditBlowPos4();
	afx_msg void OnEnSetfocusEditBlowPos5();
	afx_msg void OnEnSetfocusEditBlowPos6();
	afx_msg void OnEnSetfocusEditBlowPos7();
	afx_msg void OnEnSetfocusEditBlowPos8();

	afx_msg void OnEnSetfocusEditVibratorCnt();
	afx_msg void OnEnSetfocusEditVibratorDelayTime();

	void NumberBox(char* szVal, int nControlID, char* szTitle, char* szMax, char* szMin);
	void NumberBox(double dVal, int nControlID, char* szTitle, char* szMax, char* szMin);
private:
// 	UINT m_nRadioDeviceCheck;
// 	UINT m_nRadioCheckOption;
	int m_nDeviceCheckStatus;
	int m_nDeviceCheckOption;

	int m_nViratorOption;

	int m_nAlwayViratorOption;

	int m_nBlowPosOption;

	int m_nCleanOffsetOption;

	int m_nReContactOption;

	char m_szCompanyVibratorMode[64];

	int m_nVibrationOption;

	int m_nContactLogOption;

	int m_nSocketCleanFunctionOption;

	void SetVisibleSocketOption(BOOL enable);
	void SetPressDeviceOption(
		int nEnable, int nOpt, int nUse, int nReCnt,int nContactOption,
		int nCleanOffsetOtn, double dDelayTime,int nAlwayVibOtn, int nAlwayRetryCnt,
		double dAlwayDelayTime, int nReContactEnable,int nReContactCnt, double dReContactOffset,double dReContactDelayTime);

	void ContactPosEnable(BOOL bEnable);
	void SocketCleanFunctionEnable(BOOL bEnable);
	void CleanOffsetEnable(BOOL bEnable);

	CButton m_rbChkDvcEnable;
	CButton m_rbChkDvcDisable;
	CButton m_rbAllSocketOpt;
	CButton m_rbOnlySocketOpt;
	CButton m_rbChkVibUse;
	CButton m_rbChkVibNotUse;

	CButton m_rbChkAlwayVibUse;
	CButton m_rbChkAlwayVibNotUse;

	CButton m_rbChkContactUse;
	CButton m_rbChkContactNotUse;



	CButton m_rbChkCleanOffsetUse;
	CButton m_rbChkCleanOffsetNotUse;

	CButton m_rbChkReContactEnable;
	CButton m_rbChkReContactDisable;

	CButton m_rbChkReContactLogEnable;
	CButton m_rbChkReContactLogDisable;

	CButton m_rbChkSocketCleanFunctionEnable;
	CButton m_rbChkSocketCleanFunctionDisable;

	CButton m_rbChkVibrationUse;
	CButton m_rbChkVibrationNotUse;

	afx_msg void OnBnClickedPressRadioUse();
	afx_msg void OnBnClickedPressRadioNotUse();
	afx_msg void OnBnClickedPressRadioAllSocket();
	afx_msg void OnBnClickedPressRadioOnlySocketOpen();
public:
	
	afx_msg void OnBnClickedRadioVibratorUse();
	afx_msg void OnBnClickedRadioVibratorNotUse();

	afx_msg void OnBnClickedRadioBlowPosUse();
	afx_msg void OnBnClickedRadioBlowPosNotUse();
	afx_msg void OnBnClickedRadioContactPosOffsetUse();
	afx_msg void OnBnClickedRadioCleanPosOffsetUse();

	afx_msg void OnBnClickedRadioAlwayVibratorUse();
	afx_msg void OnBnClickedRadioAlwayVibratorNotUse();
	afx_msg void OnEnSetfocusEditAlwayVibReCnt();
	afx_msg void OnEnSetfocusEditAlwayVibDelayTime();
	afx_msg void OnBnClickedRadioRecontactUse();
	afx_msg void OnBnClickedRadioRecontactNotUse();
	afx_msg void OnEnSetfocusEditRecontactCnt();
	afx_msg void OnEnSetfocusEditRecontactOffset();
	afx_msg void OnEnSetfocusEditRecontactDelayTime();
	afx_msg void OnEnSetfocusEditContactPosOffset();
	afx_msg void OnBnClickedRadioVibrationUse();
	afx_msg void OnBnClickedRadioVibrationNotUse();
	afx_msg void OnEnSetfocusEditContact2stSpeed();
	afx_msg void OnBnClickedRadioContactLogUse();
	afx_msg void OnBnClickedRadioContactLogNotUse();
	afx_msg void OnBnClickedStaticSocketCleanFunctionUse();
	afx_msg void OnBnClickedStaticSocketCleanFunctionNotUse();
	afx_msg void OnEnSetfocusEditSocketCleanBlowPos();
	afx_msg void OnEnSetfocusEditSocketCleanBlowTime();
	afx_msg void OnEnSetfocusEditCleaning2ndZOffset();
	afx_msg void OnEnSetfocusEditContactCleanOffset1();
	afx_msg void OnEnSetfocusEditContactCleanOffset2();
	afx_msg void OnEnSetfocusEditContactCleanOffset3();
	afx_msg void OnEnSetfocusEditContactCleanOffset4();
	afx_msg void OnEnSetfocusEditContactCleanOffset5();
	afx_msg void OnEnSetfocusEditContactCleanOffset6();
	afx_msg void OnEnSetfocusEditContactCleanOffset7();
	afx_msg void OnEnSetfocusEditContactCleanOffset8();
	afx_msg void OnBnClickedRadioCleanOffUse();
	afx_msg void OnBnClickedRadioCleanOffsetNotUse();
};
