#pragma once

#include "../VAT_CommonData.h"
#include "OcxFiles/sscommand.h"
#include "ShadeButtonST.h"
#include "EditTrans.h"


#define LIMIT_COLOR_RED		(RED)
#define NORMAL_COLOR_BLACK	(BBLACK)

/*
Z Auto Focus, Cam Center Calibration, Picker Align, Z Auto Teaching, Vision Auto Teaching
모든 Main Dlg에서 공통 사용 함수 및 변수 정의.

함수 및 변수 _Com으로 시작.
*/

//Hand Type&Part Type 공통
typedef struct _stVAT_BASE_POS_SHEET_INFO
{
	int nSSType;
	int nHand;

	int nRow;
	int nCol;

	_stVAT_BASE_POS_SHEET_INFO() { clear(); }
	void clear() {
		nSSType = -1; 
		nHand = -1;
		nRow = -1;
		nCol = -1;
		//ZeroMemory(this, sizeof(_stVAT_BASE_POS_SHEET_INFO)); 
	}
};


#define DEF_PART_INDEX(Row)		(Row - 1) //Spread Sheet Row를 PartIndex로 변환.

class CVATMain_Common
{
public:
	CVATMain_Common();
	~CVATMain_Common();

public:
	BOOL m_bLastExecuteStatus; //컨트롤 Block을 위해 Auto 및 Cmd Execute 상태 저장.

//=========================================================================
// COMMENT : 모든 Mode 공통 사용
//=========================================================================
public:	
	//GUI LOG
	void _Com_MakeLog_GUI(LPCTSTR fmt, ...);

	std::vector<int> m_vAllHand;

	CSpreadSheet m_ssSelectSheet[VAT_HAND_MAX];

	void _Com_InitSSTitlePanel(CDSSPanel &panel);
	void _Com_InitSSDataPanel(CDSSPanel &panelTitle, CDSSPanel &panelSub1, CDSSPanel &panelSub2, _eVAT_SPREAD_SHEET_ACCESS_TYPE eAccessType = eVAT_SSACCESSTYPE_RW);
	void _Com_ChangeSSPanelColor(CDSSPanel &panelSub1, CDSSPanel &panelSub2, int nSSDisplayType, _eVAT_SPREAD_SHEET_ACCESS_TYPE eAccessType = eVAT_SSACCESSTYPE_RW);

	//Spread Sheet 동작 Button
	void _Com_InitOperBtn(CShadeButtonST &button, int nType);

	void _Com_InitTabControl(CTabCtrl &tab, int nTabType= eVatTabType_CellChange); //현재 사용하지 않음.

	void _Com_UpdateSheetCellColor(CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo, int nCol, int nRow);
	void _Com_UpdateLastSheetData(_stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo,int nCol, int nRow);
	void _Com_UpdateLastHandData(_stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo, int nHand);
	void _Com_DblClickSheetTextChange(CSpreadSheet &spread_sheet, int nHand, CWnd* cWnd, int nCol, int nRow);

	void _Com_BlockCell(CSpreadSheet &spread_sheet, int nHand);
	BOOL _Com_CheckBlockCell(int nHand, int nCol);
	BOOL _Com_CheckBlockCell(int nHand, int nCol, int nRow);

	int _Com_ConvertAxisToMotorPhysicalNo(int nHand, int nAxis);

	//Click Hand(Hand Type/Part Type)
	void _Com_ButtonClickedSpreadHand(long Col, long Row, int nHand, int nSSType, CEditTrans editPos[] = NULL);

	//Operate
	int _Com_Oper_EncoderRead(double dPos[], _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, int nSSDisplayType);
	void _Com_Oper_TeachingTableRead(double dPos[], int nHand, int nPart);
	int _Com_Oper_MakeBasePos(int nSSDisplayType);

	//Spread Sheet Data Clear(Parameter, Color)
	//void _Com_SpreadSheetData_Init(); //초기 Data Setting.
	//void _Com_SpreadSheetData_Clear(CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo);

	//void _Com_SpreadSheetData_Init();
	void _Com_Clear_SpreadSheet(CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO &stLastSheetInfo);

	//Resource Id to Hand Idx
	int _Com_ConvertResourceIdToHandIdx(UINT nID, int nStartId, int nEndId);

	//Interlock
	_eVAT_INTERLOCK _Com_SSInterlock_AlarmDeActivate();
	_eVAT_INTERLOCK _Com_SSInterlock_Origin(int nHand);
	_eVAT_INTERLOCK _Com_SSInterlock_DblClick(int nCol, int nRow, int nSSDisplayType);
	_eVAT_INTERLOCK _Com_SSInterlock_EncoderRead(_stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, int nSSDisplayType );
	_eVAT_INTERLOCK _Com_SSInterlock_MakeBasePos(int nSSDisplayType);

	//Hand Name Convert
	CString _Com_convert_hand_axis_to_name(int nAxis);

	//=========================================================================================================
	//Command 동작 관련 공통
	void _Com_ManualMove(int nMode, CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, int nSSDisplayType = eVAT_SSDISPLAYTYPE_DATA);
	void _Com_MakeCmdPicker(int nMode, int nHand, std::vector<CPoint> &vPicker);
	void _Com_MakeCmdPosition(CSpreadSheet &spread_sheet, _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, double dCmdPos[]);
	void _Com_MakeCmdPosition_Name(_stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, CString &strCmdStage);
	
	//NDM0_TEST_SW_ENGINEER_OPTION
	void _Com_MakeCmdPositionForEngOpt(int nMode, _stVAT_BASE_POS_SHEET_INFO stLastSheetInfo, double dCmdPos[]);

	//Position Limit Check
	std::pair<int, int>  _Com_CmdPositionLimitCheck(int nHand, const double dPos[]);
	BOOL _Com_BasePositionLimitCheck(int nHand, int nAxis, double dPos);
	

	//_eVAT_MeasureStatus _Com_GetMeasureUnit_Status_DM(int nMode, int nHand, int nPart);

//=========================================================================================================
//COMMENT : 측정 진행 간, GUI 실시간 상태 표기 관련 변수 및 함수.
//=========================================================================================================
public:	
	//Single Tone Instance
	CVAT_MeasureStatus* GuiNotifier = &CVAT_MeasureStatus::GetInstance(); 


	//Hand Status
	CEditTrans m_editPosition[VAT_HAND_MAX]; //Gui 표기 Edit
	int m_nGuiHandStatus[VAT_HAND_MAX];      //실제 상태 값

	//Part Status
	struct ST_PART_EDIT_POS
	{
		CEditTrans* editPosition;
		//int nArrayCount;
	};
	ST_PART_EDIT_POS m_editPositionEachHand[VAT_HAND_MAX] = {
		m_editPos_LDTrayPp, m_editPos_UDTrayPp,
		m_editPos_LDTestPp1, m_editPos_UDTestPp1,
		m_editPos_LDTestPp2, m_editPos_UDTestPp2,
	};
	CEditTrans m_editPos_LDTrayPp[VAT_MAX_TRAY_PP_LOC_COUNT];
	CEditTrans m_editPos_UDTrayPp[VAT_MAX_TRAY_PP_LOC_COUNT];
	CEditTrans m_editPos_LDTestPp1[VAT_MAX_TEST_PP_LOC_COUNT];
	CEditTrans m_editPos_UDTestPp1[VAT_MAX_TEST_PP_LOC_COUNT];
	CEditTrans m_editPos_LDTestPp2[VAT_MAX_TEST_PP_LOC_COUNT];
	CEditTrans m_editPos_UDTestPp2[VAT_MAX_TEST_PP_LOC_COUNT];
	


	//Progress
	CProgressCtrl	m_Progress[VAT_HAND_MAX];
	CSSCommand	    m_Percent[VAT_HAND_MAX];
	void _Com_InitProgressBar();

	//Cam Status
	int _Com_ConvertCamStatusToFrameIdx(int nStatus);
	int _Com_ConvertLowerCamToHandIdx(int nCam);

	//Blink Toggle
	BOOL m_bBlinkToggle[VAT_HAND_MAX];

	//[Timer]
	//Hand Status
	void _Com_tDisplayHandStatus(CEditTrans editPos[]);

//===============================================================================
// COMMENT : Hand 선택 관련 변수 및 함수
//           1. Z Auto Focus
//			 2. Cam Center Calibration
//			 3. Z Auto Teaching & Vison Auto Teaching Base Position Dlg에서 사용
//===============================================================================
protected:
	BOOL m_bSelectedHand[VAT_HAND_MAX];

//Dlg Init
public: 
	void _Com_InitHandSelectSheet();

	void _Com_InitPosSpreadSheet_HandType(CSpreadSheet &spread_sheet);
public:
	CString _Com_convert_hand_idx_to_name(int nHand);


public:
	//Hand Selected
	BOOL _Com_IsHandSelected(int nHand);
	BOOL _Com_IsAnyHandSelected();


//===============================================================================
// COMMENT : Picker 선택 관련 변수 및 함수
//			 1. Picker Align
//===============================================================================
//추후, 측정하고자 하는 Picker 한개씩 선택 할 때 추가.
//protected:
	//BOOL m_bSelectedPicker[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX][VAT_PICKER_MAX];


//public:
	//BOOL _Com_IsPickerSelected(int nHand);



//=========================================================================
// COMMENT : Part 선택 관련 변수 및 함수
//           1. Z Auto Teaching
//			 2. Vision Auto Teaching

//=========================================================================

protected:
	
	//Part Select
	//구조체 내부에서만 Selected Part 관리.
	typedef struct _ST_PART_SELECT
	{
		BOOL load_tray_pp_part_select[VAT_MAX_TRAY_PP_LOC_COUNT];
		BOOL unload_tray_pp_part_select[VAT_MAX_TRAY_PP_LOC_COUNT];
		BOOL load_test_pp_1_part_select[VAT_MAX_TEST_PP_LOC_COUNT];
		BOOL unload_test_pp_1_part_select[VAT_MAX_TEST_PP_LOC_COUNT];
		BOOL load_test_pp_2_part_select[VAT_MAX_TEST_PP_LOC_COUNT];
		BOOL unload_test_pp_2_part_select[VAT_MAX_TEST_PP_LOC_COUNT];

		
		_ST_PART_SELECT() {
			memset(this, 0x00, sizeof(_ST_PART_SELECT));
		};

		BOOL getPartSelected(int nHand, int nPart) {
			BOOL bRet =FALSE;
			switch (nHand)
			{
			case VAT_HAND_LOAD_TRAYPP:	  bRet = load_tray_pp_part_select[nPart]; break;
			case VAT_HAND_UNLOAD_TRAYPP:  bRet = unload_tray_pp_part_select[nPart]; break;
			case VAT_HAND_LOAD_TESTPP1:   bRet = load_test_pp_1_part_select[nPart]; break;
			case VAT_HAND_UNLOAD_TESTPP1: bRet = unload_test_pp_1_part_select[nPart]; break;
			case VAT_HAND_LOAD_TESTPP2:   bRet = load_test_pp_2_part_select[nPart]; break;
			case VAT_HAND_UNLOAD_TESTPP2: bRet = unload_test_pp_2_part_select[nPart]; break;
			default:
				break;
			}
			return bRet;
		};

		void setPartSelected(int nHand, int nPart, BOOL TR_FL) {
			switch (nHand)
			{
			case VAT_HAND_LOAD_TRAYPP:	  load_tray_pp_part_select[nPart] = TR_FL;		break;
			case VAT_HAND_UNLOAD_TRAYPP:  unload_tray_pp_part_select[nPart] = TR_FL;	break;
			case VAT_HAND_LOAD_TESTPP1:   load_test_pp_1_part_select[nPart] = TR_FL;	break;
			case VAT_HAND_UNLOAD_TESTPP1: unload_test_pp_1_part_select[nPart] = TR_FL;	 break;
			case VAT_HAND_LOAD_TESTPP2:   load_test_pp_2_part_select[nPart] = TR_FL;	 break;
			case VAT_HAND_UNLOAD_TESTPP2: unload_test_pp_2_part_select[nPart] = TR_FL;	break;
			default:
				break;
			}
		}

	}ST_PART_SELECT;
	

	ST_PART_SELECT m_stPartSelected;

public:
	//Part Type의 Hand Panel 종류 6가지
 	CDSSPanel m_ssPanel_LD_TrayPP;
 	CDSSPanel m_ssPanel_UD_TrayPP;
 	CDSSPanel m_ssPanel_LD_TestPP1;
 	CDSSPanel m_ssPanel_UD_TestPP1;
 	CDSSPanel m_ssPanel_LD_TestPP2;
 	CDSSPanel m_ssPanel_UD_TestPP2;

	BOOL _Com_IsPartHidden(int nHand, int nPart);

	void _Com_SSDisplaySelectedPart(int nHand, int nCol, int nRow);
	void _Com_SSDisplayPartStatusByDM(int nMode, int nHand, int nPart);
	int _Com_GetPartStatusFirstIdx_DM(int nMode, int nHand);

	void _Com_InitSSHandPanel_PartType(); //Hand Panel 초기화

	void _Com_InitPartSelectSheet();											   //모든 Hand Part Select Spread Sheet 초기화
	
	void _Com_InitPosSpreadSheet_PartType(CSpreadSheet &spread_sheet, int nHand); //Base Position Spread Sheet 초기화.

	CComboBox m_ComboPos[VAT_HAND_MAX];
	void _Com_InitComboPos();
	int _Com_GetPartIdxfromComboBox(int nHand);

	BOOL _Com_IsNecessaryPart(int nHand, int nPartIdx);
	CString _Com_convert_part_idx_to_name(int nHand, int nPartIdx);
protected:

	//Part Status
	int GetSelectedPartStatus(int nHandIdx, int nPart); //현재 사용 안하고 있는데 사용시 _Com으로 변경.


//Part Measure Tab 객체에서만 호출 가능하도록 public 변경 필요.
//선택 Part는 외부에서도 호출이 가능함으로 m_pSettingTab에서는 호출을 방지하고자 함.
protected:
	//=======================================Picker Type



	//=======================================Part Type
	void _Com_SetSelectedPart(int nHand, int nPart, BOOL bIsPainting = TRUE); //1Hand, 1Part
	void _Com_SetSelectedAllPart(int nHand);//1Hand, All Part

	void _Com_SetSelectedAll();//All Hand, All Part //[현재 사용 안함 -> 기능 추가 가능.]

	//혼동을 방지하기 위해 _Com_GetSelectedPart는 선택된 Part를 가져오는 함수로 구성해야 함.

	BOOL _Com_IsPartSelected(int nHand, int nPart);// 1Hand, 1part 확인. 

	BOOL _Com_IsAnyPartSelected();

	BOOL _Com_IsPartDisable(int nHand, int nPart);
	
	
};
