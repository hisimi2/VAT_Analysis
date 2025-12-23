#pragma once

#include "../Include/OcxFiles/spreadsheet.h"
#include "afxwin.h"
#include "TeachingFile.h"
#include "RamTeaching.h"
#include "../VisionAutoTeaching/MainDlg/VAT_MainDlg.h"
//#include "../VisionAutoTeaching/Z Auto Teaching/VAT_ZAutoTeachingDlg.h"

// CDlgManuMotorTeaching 대화 상자입니다.

class CDlgManuMotorTeaching : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuMotorTeaching)

public:
	CDlgManuMotorTeaching(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuMotorTeaching();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_MOTOR_TEACHING };

private:
	void InitControl();
	void UpdateTeachingData(int nRobotId, BOOL bSaveAndValidate=TRUE);
	void SaveTeachingData();
	void SelchangeCbRobotChoose();

	CLogger m_EncoderLog;


private:
	//CTeachingFile m_TeachingFile;
    //CRamTeaching  m_TeachingData;   // decrease by PAGESIZE Error로 불가
	CMyList		  m_RobotNameList;
	int           m_nCurRobotId;
	int           m_nLastSel_Row ;
	int           m_nLastSel_Col ;

	int			  m_nLastLocCount;
	int			  m_nLastPosCount;

	int  ComInterlock();
	BOOL ChkCellBlock(int nRow, int nCol, int nRobotid);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCommandbuttonManuMotor();
	afx_msg void OnBnClickedCommandbuttonTeachingJog();
	static void callback_progressbar_cancel_event( int _axis_no );
	afx_msg void OnBnClickedCommandbuttonMove();
	afx_msg void OnBnClickedCommandbuttonBase();
	afx_msg void OnBnClickedCommandbuttonParameter();
	afx_msg void OnBnClickedCommandbuttonSave();
	afx_msg void OnBnClickedOk();
	CSpreadSheet m_sprLocTable;
	CComboBox m_cbRobotName;
	afx_msg void OnCbnSelchangeCbRobotChoose();
	DECLARE_EVENTSINK_MAP()
	void Click_RobotTeachingTable(long Col, long Row);
	void DblClick_RobotTeachingTable(long Col, long Row);
	afx_msg void OnBnClickedCommandbuttonReload();
	afx_msg void OnBnClickedCommandbuttonTestpp();
	afx_msg void OnBnClickedCommandbuttonTraypp();
	afx_msg void OnBnClickedCommandbuttonPress();
	afx_msg void OnBnClickedCommandbuttonSetplateStacker();
	afx_msg void OnBnClickedCommandbuttonTransEvstacker();
	afx_msg void OnBnClickedCommandbuttonReadEncoder();
	afx_msg void OnBnClickedCommandbuttonNewPcdbacklash();
	CBitmap m_cBmpServoMotor;
	CBitmap m_cBmpMotorTeaching;
	CBitmap m_cBmpEncoderRead;
	CBitmap m_cBmpMotorMoving;
	CBitmap m_cBmpBasePoint;
	CBitmap m_cBmpParameterSetup;
	CBitmap m_cBmpReload;
	CBitmap m_cBmpSave;
	CBitmap m_cBmpExit;
	CBitmap m_cBmpPressUnit;
	CBitmap m_cBmpTrayPP;
	CBitmap m_cBmpTestPP;
	CBitmap m_cBmpTransfer;
	CBitmap m_cBmpSetPlateStacker;
	CBitmap m_cBmpPcdBacklash;
	CBitmap m_cBmpTestPpOffset;
	CBitmap m_cBmpPickerAlign;
	CBitmap m_cBmpNewBacklash;
	CBitmap m_cBmpVisionAutoTeaching;
	CBitmap m_cBmpPickerHeightCal;
	CBitmap m_cBmpVAT;

	CButton m_btnServoMotor;
	CButton m_btnMotorTeaching;
	CButton m_btnEncoderRead;
	CButton m_btnMotorMoving;
	CButton m_btnBasePoint;
	CButton m_btnParameterSetup;
	CButton m_btnReload;
	CButton m_btnSave;
	CButton m_btnExit;
	CButton m_btnPressUnit;
	CButton m_btnTrayPP;
	CButton m_btnTestPP;
	CButton m_btnTransfer;
	CButton m_btnSetPlateStacker;
	CButton m_btnPcdNBacklash;

	CButton m_btnTestPpOffset;
	CButton m_btnPickerAlign;
	CButton m_btnNewBacklash;
	CButton m_btnVisionAutoTeaching;
	CButton m_btnPickerHeightCal;

	CButton m_btnVAT;
	CVAT_MainDlg* m_pDlgVAT;
	_tTEACH_LOG tTeachLog;

	int m_nTestPpType;
	
	void DisplayTestPpSSPanel();

	void MakeLog(LPCTSTR fmt, ...);

private:
	void WriteChangeTeachLog();

	void SetFlatnessOffset(int nAxisNo, int nLocStage);
	int ConvertTeachLocStagetoVatPartIdx(int nAxisNo, int nLocStage);
public:
	afx_msg void OnBnClickedCommandbuttonPcdbacklash();
	afx_msg void OnBnClickedCommandbuttonTestppOffset();
	afx_msg void OnBnClickedCommandbuttonTransfer();
	afx_msg void OnBnClickedBtnVisionAutoTeaching();
	afx_msg void OnBnClickedCommandbuttonTrayppOffset();
	afx_msg void OnBnClickedCommandbuttonNewPickerMotor();
	afx_msg void OnBnClickedVisionAutoTeaching();
	void ClickSspanelTestppType();

//Group Teaching Point 추가.
public:
	std::vector<std::pair<int, int>> m_vYAxisGroupPoint[4];

	enum _eGroup_TeachPos_Col {
		eCOL1_X = 1,
		eCOL2_Y = 2,
		eCOL5_Z_Safety = 5,
		eCOL6_X_Pitch = 6,
		eCOL7_Y_Pitch = 7,
	};

	enum _eGroup_TeachPos_Row {
		eRow1_Station1 = 1,
		eRow2_Station2,
		eRow3_Station3,
		eRow4_Station4,
		eRow5_Loading_Table_1_LD,
		eRow6_Loading_Table_2_LD,
		eRow7_Loading_Table_3_LD,
		eRow8_Loading_Table_4_LD,
		eRow9_Loading_Table_1_UD,
		eRow10_Loading_Table_2_UD,
		eRow11_Loading_Table_3_UD,
		eRow12_Loading_Table_4_UD,
		eRow13_Socket_Clean,
		eRow14_Home,
	};


	std::vector<std::pair<int, int>> m_vGroupPoint;
	BOOL IsGroupTeachPosition(long Col, long Row);
	void SetGroupTeachPosition(long Col, long Row);

	void ChangeSingleTeachingColor(long Col, long Row, OLE_COLOR color);
	void ChangeGroupTeachingColor(long Col, long Row, OLE_COLOR color);

	void ChangeGroupTeachingData(long Col, long Row, LPCTSTR strValue);


	//Teaching Type SSPanel
	enum _TeachType {
		_eTeachType_Single = 0,
		_eTeachType_Group,
	};

	int m_nTeachType;
	void ClickSspanelSingleTeaching();
	void ClickSspanelGroupTeaching();

	void DisplayGroupTeachSSPanel();
	
	BOOL ChkShowFlatnessOffset(int nAxisNo, int nLoc, int nPos);

	int ChkInterlockBeforeMove(int nAxisNo, double dTPos);

	void AutoCalculateValue(int nRobotId, long Col, long Row);
};
