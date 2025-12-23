#pragma once
#include "../../Dll_Src/LIB_MC/LIB_MC/BbxmnpClient.h"
#include <unordered_map>


#define arr (0)

#define DEF_HAND_UNSELECTED (-1)


// static const std::unordered_map<int, std::pair<int, int>> cHandPitchAxisMap = {
// 	{ VAT_HAND_LOAD_TRAYPP,{ eAXIS_TRAY_PP_1_X_PITCH,  eAXIS_TRAY_PP_1_Y_PITCH } },
// 	{ VAT_HAND_UNLOAD_TRAYPP,{ eAXIS_TRAY_PP_2_X_PITCH,  eAXIS_TRAY_PP_2_Y_PITCH } },
// 	{ VAT_HAND_LOAD_TESTPP1,{ eAXIS_TEST_PP_1_X_PITCH,  eAXIS_TEST_PP_1_Y_PITCH } },
// 	{ VAT_HAND_UNLOAD_TESTPP1,{ eAXIS_TEST_PP_1_X2_PITCH, eAXIS_TEST_PP_1_Y2_PITCH } },
// 	{ VAT_HAND_LOAD_TESTPP2,{ eAXIS_TEST_PP_2_X_PITCH,  eAXIS_TEST_PP_2_Y_PITCH } },
// 	{ VAT_HAND_UNLOAD_TESTPP2,{ eAXIS_TEST_PP_2_X2_PITCH, eAXIS_TEST_PP_2_Y2_PITCH } },
// };

// typedef struct _stHandPitchInfo
// {
// 	int robot_id;
// 
// 	int xp_pos_idx;
// 	int yp_pos_idx;
// 
// 	int xp_axis_no;
// 	int yp_axis_no;
// 
// 	_stHandPitchInfo(int param1, int param2, int param3, int param4, int param5):
// 		robot_id(param1), 
// 		xp_pos_idx(param2), 
// 		yp_pos_idx(param3), 
// 		xp_axis_no(param4), 
// 		yp_axis_no(param5) 
// 	{}
// 
// 
// 	_stHandPitchInfo() { clear(); };
// 	void clear() {
// 		robot_id = -1; xp_pos_idx = -1;  yp_pos_idx = -1; xp_axis_no = -1; yp_axis_no = -1;
// 	};
// }stHandPitchInfo;
// 
// 
// static const std::unordered_map<int, stHandPitchInfo> cHandPitchParamMap = {
// 	{ VAT_HAND_LOAD_TRAYPP,{ RID_TRAY_PP_(RIDX_TRAY_PP_1),  POS_IDX_TRAY_LOAD_PP_X_PITCH, POS_IDX_TRAY_LOAD_PP_Y_PITCH,  eAXIS_TRAY_PP_1_X_PITCH,  eAXIS_TRAY_PP_1_Y_PITCH } },
// 	{ VAT_HAND_UNLOAD_TRAYPP,{ RID_TRAY_PP_(RIDX_TRAY_PP_2), POS_IDX_TRAY_PP_X_PITCH,  POS_IDX_TRAY_PP_Y_PITCH, eAXIS_TRAY_PP_2_X_PITCH,  eAXIS_TRAY_PP_2_Y_PITCH } },
// 	{ VAT_HAND_LOAD_TESTPP1,{ RID_TEST_PP_(RIDX_TEST_PP_1), POS_IDX_TEST_PP_X1_PITCH,  POS_IDX_TEST_PP_Y1_PITCH, eAXIS_TEST_PP_1_X_PITCH,  eAXIS_TEST_PP_1_Y_PITCH } },
// 	{ VAT_HAND_UNLOAD_TESTPP1,{ RID_TEST_PP_(RIDX_TEST_PP_1), POS_IDX_TEST_PP_X2_PITCH, POS_IDX_TEST_PP_Y2_PITCH, eAXIS_TEST_PP_1_X2_PITCH, eAXIS_TEST_PP_1_Y2_PITCH } },
// 	{ VAT_HAND_LOAD_TESTPP2,{ RID_TEST_PP_(RIDX_TEST_PP_2), POS_IDX_TEST_PP_X1_PITCH,  POS_IDX_TEST_PP_Y1_PITCH, eAXIS_TEST_PP_2_X_PITCH,  eAXIS_TEST_PP_2_Y_PITCH } },
// 	{ VAT_HAND_UNLOAD_TESTPP2,{ RID_TEST_PP_(RIDX_TEST_PP_2), POS_IDX_TEST_PP_X2_PITCH, POS_IDX_TEST_PP_Y2_PITCH, eAXIS_TEST_PP_2_X2_PITCH, eAXIS_TEST_PP_2_Y2_PITCH } },
// };

// static const stGetTeachPosParam cGetTeachPosParam[VAT_HAND_MAX] = {
// 
// }



typedef std::pair<int, _eVAT_MeasureStatus> UNITCOMPONENT;
typedef std::vector<UNITCOMPONENT> UNITCOMPONENTLIST;

//Measure 진행 중인 Picker 정보(Picker Align)
typedef struct _ST_VAT_PICKER
{
	int nPickerIdx;
	int nPitchType;
	
	_ST_VAT_PICKER() { clear(); };
	void clear() {
		nPickerIdx = -1;
		nPitchType = -1; 
	};

}ST_VAT_PICKER;

class CVatBaseTask
{
public:
	CVatBaseTask();
	~CVatBaseTask();

public:
	HWND m_hVatMainGui;
	void SetVatMainGuiHandle(HWND hOwner);
	HWND GetVatMainGuiHandle();


	int m_nZAutoTeachingSpeed;
	void _Base_SetZAutoTeachingSpd(int eSpd);
	int _Base_GetZAutoTeachingSpd();

//set
public:

	void SetHandStatus(int nStatus);
	int GetHandStatus();

// 	void SetPartStatus(int nCurLoc, int part_status);
// 	int GetPartStatus(int nCurLoc);

	//void ClearSelectedPart() { m_vSelectedParts.clear(); }
//get
public:
	//int GetSeqMode();

	//int GetSelectedHand(int nPart);

	CString GetExcuteModeStr(int nMode);

	

//==================신규 작성
private:
	//BOOL m_bSelectedHand;

	//std::vector<std::pair<int, _eVAT_MeasureStatus>> m_cSelectedHand;	
	//std::vector<std::pair<int, _eVAT_MeasureStatus>> m_vSelectedPartPairs;

	UNITCOMPONENT		m_cSelectedHand;
	UNITCOMPONENTLIST	m_cSelectedPicker[VAT_PITCH_TYPE_MAX];
	//UNITCOMPONENTLIST	m_cSelectedPicker;
	UNITCOMPONENTLIST	m_cSelectedPart;

public:
	//====================Common
	int m_nSeqMode;

	int m_nHand;
	int m_nCurLoc;

	//[Picker]
	CPoint _Base_GetStdPicker();
	CPoint _Base_ConvertPickerIdxToPoint(int nPkrIdx);
	std::pair<double, double> _Base_CalPitchTargetPosXY(int nPitchType, int nPickerIdx);
	std::pair<double, double> _Base_GetPitchXYTeachPos(int nHand, int nPitchType);

	//[Measure]
	double m_dPrevPosX; //Target Center Move를 위한 Hand 이전 Position 저장.
	double m_dPrevPosY; //Target Center Move를 위한 Hand 이전 Position 저장.
	int m_nVisionRetryCnt;
	//int m_nPkrAutoFocusCnt;

	void _Base_GetSeqTargetPos(double dTPos[], _eVAT_MODE_EACH_DATA_TYPE _eType = eModeEachDataType_None);
	void _Base_GetSeqTargetPos_Unit(double dTPos[], int nVATPart, _eVAT_PART_ONLY_POS _ePos);
	void _Base_GetSafetyPos(double dTPos[]);
	void _Base_MakePrePos(double dTPos[]);


	void _Base_CheckandCreateLimitPos(double dTPos[], int nAxis = VAT_MAX_AXIS_XY);
	void _Base_CheckandCreateLimitPos_Unit(double dTPos[], int nVATPart);
	void _Base_CheckandCreateZHeightCalPrePos(double &dZPos);


	//Hand/Picker/Part Type 통합 상태 변경 함수.
	void _Base_SetMeasure_Status(_eVAT_MeasureStatus nUpdateStatus, std::vector<int> vStatus = {});
	BOOL _Base_IsAllMeasureSameStatus(_eVAT_MeasureStatus status);


	//Auto Schedule Parameter Clear
	void _Base_ClearParam();

	//====================Hand Type
	void _Base_ClearMeasureHand() {
		m_cSelectedHand.first = DEF_HAND_UNSELECTED; 
		m_cSelectedHand.second = VAT_STATUS_MEASURE_INIT;
	};
	void _Base_SetMeasureHand(BOOL bSelect);

	void _Base_SetMeasureHand_Status(_eVAT_MeasureStatus nStatus);
	int _Base_GetMeasureHand_Status();

	BOOL _Base_IsHandSameStatus(_eVAT_MeasureStatus hand_status);
	BOOL _Base_IsHandSelected() { return (m_cSelectedHand.first != DEF_HAND_UNSELECTED) ? TRUE : FALSE; };

	//====================Picker Type
	ST_VAT_PICKER m_stCurPicker;

	void _Base_ClearMeasurePicker() { 
		m_cSelectedPicker[VAT_PITCH_TYPE_WIDE].clear();
		m_cSelectedPicker[VAT_PITCH_TYPE_NARROW].clear();
	}
	void _Base_SetMeasurePicker_Selected(int nPicker, BOOL bSelect);

	ST_VAT_PICKER _Base_GetMeasurePicker_Each(_eVAT_MeasureStatus nStatus);

	void _Base_SetMeasureAllPicker_Status(_eVAT_MeasureStatus nStatus);
	void _Base_SetMeasurePicker_Status(int nPitchType, int nPicker, _eVAT_MeasureStatus nStatus);
	int _Base_GetMeasurePicker_Status(int nPitchType, int nPicker);

	BOOL _Base_IsAllPickerSameStatus(int nPitchType, int part_status);
	BOOL _Base_IsAnyPickerSelected();

	//====================Part Type
	int m_nTableTargetIdx;
	double m_dTblTargetPos[eTableTargetMax][VAT_MAX_AXIS_XY]; //Table Rotate 계산을 위한 Target Position1, 2
	

	void _Base_ClearMeasurePart() { m_cSelectedPart.clear(); }					//[선택 Part 상태 초기화]
	void _Base_SetMeasurePart_Selected(int nPart, BOOL bSelect);					//[선택 Part Set]
	int _Base_GetMeasurePart_Selected(_eVAT_MeasureStatus nStatus);					//[선택 Part 중 nStatus인 Part]
	
	UNITCOMPONENT _Base_GetMeasurePart_Each(int nPart);

	void _Base_SetMeasurePart_Status(int nPart, _eVAT_MeasureStatus nStatus);		//[Part 상태 Set]
	int _Base_GetMeasurePart_Status(int nPart);										//[Part 상태 get]
	
	
	void _SetMeasurePart_Status_DM(int nPart, _eVAT_MeasureStatus nStatus);

	BOOL _Base_IsAllPartSameStatus(int part_status);
	BOOL _Base_IsAnyPartSelected() { return (m_cSelectedPart.size() > 0) ? TRUE : FALSE; };

	int _Base_GetVATMeasureType(int nHand);

	//Calculate Rotate
	void _Base_CalRotate();
	double _Base_CalSlopeDegree(double dX, double dY);

public:
	//Alarm Report
	void _Base_AlarmReport_VAT(int nErr, CString strAlarmData = _T(""));

	//Calculate Progress Rate
	double m_dEachCaseRate;
	double m_dThresholdProgressRate;
	void _Base_UpdateProgressRate(int nCurMainStep, int nLastMainStep, int nTotalCase); //Auto Case 계산.(일반적인 방법)
	
	int   m_nLastPickerForProgress;														//Progress 계산을 위한 이전 Picker 저장.
	void _Base_UpdateProgressRate_Picker();												//Picker 갯수로 계산.(Picker Align만 사용)

	

	CTimerTickCheck m_tcAutoSchedule;


};

