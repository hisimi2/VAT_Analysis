#pragma once
#include "VAT_PickerAlignDlgCom.h"
#include "afxwin.h"

// CVATMainTab_PickerAlignSetting 대화 상자입니다.

class CVATMainTab_PickerAlignSetting : public CDialogEx, public CVATMain_Common, public CVAT_PickerAlignDlgCom
{
	DECLARE_DYNAMIC(CVATMainTab_PickerAlignSetting)

public:
	CVATMainTab_PickerAlignSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMainTab_PickerAlignSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_PICKER_ALIGN_SETTING_DLG };


public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Tab();
	void PageMoveDispose_Tab();

public: //OnTimer
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	enum _ePickerAlignTimer
	{
		_ePickerAlignT_CheckExecuteStatus = 0,
		_ePickerAlignT_VisionMeasureLive,

		_ePickerAlignT_Max,
	};
	void EnableControls(BOOL bStatus);
	BOOL CheckVisionConnect(int nHand);
public: //OnPaint
	afx_msg void OnPaint();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
// 	CSpreadSheet m_ssPkrOffsetPos[VAT_HAND_MAX];
// 	CSpreadSheet m_ssPkrSettingPos[VAT_HAND_MAX];
	enum _eHandType
	{
		eHandType_TrayPp = 0,
		eHandType_TestPp,
	};
	int GetHandType(int nHand);

	enum _ePosSpreadSheetType
	{
		eSSType_EachPkrPos = 0,
		eSSType_PkrSettingPos,
		eSSType_Max,
	};

	CSpreadSheet m_ssEachPkrTargetPos[VAT_HAND_MAX];
	CSpreadSheet m_ssEachPkrSettingPos[VAT_HAND_MAX];

	void InitPosSpreadSheet(CSpreadSheet &spread_sheet);
	void BlockSpreadSheet(int nType);
	void DblClickSheetTextChange(int nHand, int nCol, int nRow);
	int ConvertSSCellToMotorAxisNo(int nHand, int nRow);

public:
	int m_nSelectedPkr[VAT_HAND_MAX][VAT_MAX_AXIS_XY];

	void InitSelectedPicker();
	void SelectedPickerColor(int nHand);
	void SetSelectedPicker(int nHand, int nCol, int nRow);
	void DisplaySelectedPicker(int nHand);

	//Offset Grid
	void InitSettingTabRect();

public:
	DECLARE_EVENTSINK_MAP()

	void ButtonClickedPkrSelectSpread1(long Col, long Row, short ButtonDown);
	void ButtonClickedPkrSelectSpread2(long Col, long Row, short ButtonDown);
	void ButtonClickedPkrSelectSpread3(long Col, long Row, short ButtonDown);
	void ButtonClickedPkrSelectSpread4(long Col, long Row, short ButtonDown);
	void ButtonClickedPkrSelectSpread5(long Col, long Row, short ButtonDown);
	void ButtonClickedPkrSelectSpread6(long Col, long Row, short ButtonDown);

	//Spread Sheet Double Click
	void DblClickedPkrPrePositionSpread1(long Col, long Row);
	void DblClickedPkrPrePositionSpread2(long Col, long Row);
	void DblClickedPkrPrePositionSpread3(long Col, long Row);
	void DblClickedPkrPrePositionSpread4(long Col, long Row);
	void DblClickedPkrPrePositionSpread5(long Col, long Row);
	void DblClickedPkrPrePositionSpread6(long Col, long Row);

//[Operate Button]
public:
	enum _eMotorMoveType
	{
		eMoveType_SelectedPkrPos = 0,
		eMoveType_PrePosition,
		eMoveTypeMax,
	};


	CShadeButtonST m_BtnTargetMove[VAT_HAND_MAX];

	CShadeButtonST m_BtnPreMove[VAT_HAND_MAX];
	CShadeButtonST m_BtnRead[VAT_HAND_MAX];
	CShadeButtonST m_BtnSave[VAT_HAND_MAX];

	CShadeButtonST m_BtnVisionLive[VAT_HAND_MAX];

	void InitOperBtn();
	void DisplayTargetPos(int nHand);
	void DisplayPrePos(int nHand);

	enum eVisionLiveStatus
	{
		eLive_Off = 0,
		eLive_On,

		eLive_Status_Max,
	};

	BOOL m_bVisionLiveStatus[VAT_HAND_MAX];
	void DisplayVisionLiveStaus(int nHand, int nStatus);

	//Move
	void MotorMove(int nHand, _eMotorMoveType eMoveType);
	_eVAT_INTERLOCK ChkMotorMoveInterlock(int nHand);
	afx_msg void OnBtnClickedTargetPosMove(UINT nID);
	afx_msg void OnBtnClickedPrePosMove(UINT nID);

	//Read
	void ReadEncoder(int nHand);
	afx_msg void OnBtnClickedPrePosRead(UINT nID);

	//Save
	void SaveEncoder(int nHand);
	afx_msg void OnBtnClickedPrePosSave(UINT nID);

	//Vision Live
	afx_msg void OnBtnClickedVisionLive(UINT nID);

//Real Time GUI Update
public:
	afx_msg LRESULT OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam);
};
