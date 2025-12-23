#pragma once

//#include "../VAT_DataCopy.h"
// CVATMainTab_ZAutoTeachingSetting 대화 상자입니다.
#include "VAT_STATIC_ID.h"
#include "../VAT_ManuMotorJog.h"

class CVATMainTab_ZAutoTeachingSetting : public CDialogEx, public CVATMain_Common
{
	DECLARE_DYNAMIC(CVATMainTab_ZAutoTeachingSetting)

public:
	CVATMainTab_ZAutoTeachingSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMainTab_ZAutoTeachingSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_Z_AUTO_TEACHING_SETTING_DLG };

	CShadeButtonST m_BtnCancel;
public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	//=========================================================================TIMER
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	enum _eZAutoTeaching_Setting_Timer
	{
		//_ePickerAlignT_HandStatus
		//g_bVATStatusExecute
		//eTimer_CheckManualCmdStatus

		//상시 확인용 Timer
		_eZTeachSettingT_CheckExecuteStatus = 0,

		_eZTeachSettingT_Max,

	};

	afx_msg void OnDestroy();

	//control Enable/Disable
	void EnableControls_SettingTab(BOOL bStatus);

	void PageMoveInit_Tab();
	void PageMoveDispose_Tab();

	void InitDataLoad();

	void InitZTeachBasePosOperBtn();
	DECLARE_MESSAGE_MAP()

	void InitSSConstPanel();



public:
	////Gui에서 변경 된 Data 관리.==========================
	//Z Teach Base Position
	St_VAT_ZTeachBasePos m_stVATZTeachBasePosGui;
	//=====================================================


	CSpreadSheet m_ssZTeachBasePos;
	_stVAT_BASE_POS_SHEET_INFO m_stZTeachBasePosLastInfo;

	//Operate Button
	CShadeButtonST m_BtnZTeachBasePos_EncoderRead;
	CShadeButtonST m_BtnZTeachBasePos_MotorMove;
	CShadeButtonST m_BtnZTeachBasePos_Reload;
	CShadeButtonST m_BtnZTeachBasePos_Save;

	//Make Base Position
	CShadeButtonST m_BtnMakeBasePos;

	void DisplaySSData(int nHand, BOOL bMakeBasePos = FALSE);
	int CheckPosLimit(int nHand);

	void ButtonClickHandChange(long Col, long Row, int nHand);

	DECLARE_EVENTSINK_MAP()
	void ButtonClickedSpreadZAutoTeachingHand1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZAutoTeachingHand2(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZAutoTeachingHand3(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZAutoTeachingHand4(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZAutoTeachingHand5(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZAutoTeachingHand6(long Col, long Row, short ButtonDown);
	void ClickSpreadZTeachingBasePosition(long Col, long Row);
	void DblClickSpreadZTeachingBasePosition(long Col, long Row);
	afx_msg void OnBnClickedBtnZTeachingBasePosEncoderRead();
	afx_msg void OnBnClickedBtnZTeachingBasePosMotorMove();
	afx_msg void OnBnClickedBtnZTeachingBasePosReload();
	afx_msg void OnBnClickedBtnZTeachingBasePosDataSave();
	afx_msg void OnBnClickedBtnZTeachingMakeBasePos();
	void ZTeachingMakeBasePos(double dPos[], int nHand, int nPart);
public:
	//=========================================================================Parameter Setting(설계 치수)
	void InitStaticValue();	
	void InitParamSetBtn();

	
	//double m_dZPickOffset[VAT_HAND_MAX];
	//double m_dZPlaceOffset[VAT_HAND_MAX];
	
	//afx_msg void OnEnSetfocusEditZPickOffset(UINT nID);
	//afx_msg void OnEnSetfocusEditZPlaceOffset(UINT nID);
	

	//=================================Z Pick/Place Offset
	//Gui구조체는 사용 하지 않음.
	CShadeButtonST m_BtnParamSet_Save1;
	CSpreadSheet m_ssZPickPlaceOffset;
	_stVAT_BASE_POS_SHEET_INFO m_stZPickPlaceOffsetLastInfo;
	void InitZOffsetSpreadSheet(int nHand);
	void DisplayZOffsetData(int nHand);
	void ClickSpreadZTeachPickPlaceOffset(long Col, long Row);
	void DblClickSpreadZTeachPickPlaceOffset(long Col, long Row);
	afx_msg void OnBnClickedBtnZTeachParamSetSave1();
	

	//=================================Design Parameter
	BOOL m_bBlockEditFocus;
	CShadeButtonST m_BtnParamSet_Save2;
	double m_dDesignValue[VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING];
	void InitStaticCtrl_DesignValue();
	void DisplayDesignValue(); //Hand 상관 없음.
	afx_msg void OnEnSetFocusDesignValue(UINT nID);
	afx_msg void OnBnClickedBtnZTeachParamSetSave2();


public:
	//=========================================================================DATA
	//Z Base Position
	void DataCopyZBasePos_GuiToReal(int nHand);
	void DataCopyZBasePos_RealToGui(int nHand);

	//Parameter Position
	void LoadDesignParam(); //File -> Static
	void SaveDesignParam(); //Static -> File
	

public:
	//=========================================================================JOG
	CVAT_ManuMotorJog* m_pChildDlgJog;

	void InitMotorJog();
	

public:



	
		

	

};
