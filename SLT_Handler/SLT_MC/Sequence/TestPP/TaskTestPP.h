#pragma once

#include <vector>
#include <afxtempl.h>

//==============================================================================
//
//==============================================================================
class CBaseTaskManager;
class CDataMemCont;
class CPoint;

//==============================================================================
//
//==============================================================================


class CTaskTestPP :	public CBaseTaskManager
{
public:
	enum eTeachPosZ {
		eZPOS_PLACE = 0,
		eZPOS_PICK,
		eZPOS_SAFETY,
		eMAX_ZPOS_COUNT,
	};

	enum eMotorPosXYZ {
		ePosY_StationL1 = 0,
		ePosY_StationL2,
		ePosY_StationL3,
		ePosY_StationL4,
		ePosY_NoInStation,

		ePosX_StationAndPosiLimit = 0,
		ePosX_StationToTable,
		ePosX_TableToNegaLimit,

		ePosY_LoadTable1,
		ePosY_LoadTable2,
		ePosY_LoadTable3,
		ePosY_LoadTable4,

		ePosY_ArtOrCleanBuffer,
		ePosY_Etc,

		ePosZ_DownSafetyToNegLimit = 0,
		ePosZ_DownSafety,
		ePosZ_DownSafetyToPosiLimit,// 안전위치부터 limit까지 pick/place포함.

	};

	enum eDI {
		xVAC_SEN_R1_X1 = 0, xVAC_SEN_R2_X1, xVAC_SEN_R3_X1, xVAC_SEN_R4_X1, xVAC_SEN_F1_X1, xVAC_SEN_F2_X1, xVAC_SEN_F3_X1, xVAC_SEN_F4_X1,
		xVAC_SEN_R1_X2, xVAC_SEN_R2_X2, xVAC_SEN_R3_X2, xVAC_SEN_R4_X2, xVAC_SEN_F1_X2, xVAC_SEN_F2_X2, xVAC_SEN_F3_X2, xVAC_SEN_F4_X2,
		xX1AXIS_POSITION_SEN, xX2AXIS_POSITION_SEN,
		xART_BUFF_ARRIVED_SEN1, xART_BUFF_ARRIVED_SEN2,
		xMAX_COUNT,
	};

	enum eDO {
		yHAND_VAC_R1_X1 = 0,
		yHAND_VAC_R2_X1,
		yHAND_VAC_R3_X1,
		yHAND_VAC_R4_X1,
		yHAND_VAC_F1_X1,
		yHAND_VAC_F2_X1,
		yHAND_VAC_F3_X1,
		yHAND_VAC_F4_X1,

		yHAND_VAC_R1_X2,
		yHAND_VAC_R2_X2,
		yHAND_VAC_R3_X2,
		yHAND_VAC_R4_X2,
		yHAND_VAC_F1_X2,
		yHAND_VAC_F2_X2,
		yHAND_VAC_F3_X2,
		yHAND_VAC_F4_X2,

		yHAND_BLW_R1_X1,
		yHAND_BLW_R2_X1,
		yHAND_BLW_R3_X1,
		yHAND_BLW_R4_X1,
		yHAND_BLW_F1_X1,
		yHAND_BLW_F2_X1,
		yHAND_BLW_F3_X1,
		yHAND_BLW_F4_X1,

		yHAND_BLW_R1_X2,
		yHAND_BLW_R2_X2,
		yHAND_BLW_R3_X2,
		yHAND_BLW_R4_X2,
		yHAND_BLW_F1_X2,
		yHAND_BLW_F2_X2,
		yHAND_BLW_F3_X2,
		yHAND_BLW_F4_X2,

		yHAND_RESET_R1_X1,
		yHAND_RESET_R2_X1,
		yHAND_RESET_R3_X1,
		yHAND_RESET_R4_X1,
		yHAND_RESET_F1_X1,
		yHAND_RESET_F2_X1,
		yHAND_RESET_F3_X1,
		yHAND_RESET_F4_X1,

		yHAND_RESET_R1_X2,
		yHAND_RESET_R2_X2,
		yHAND_RESET_R3_X2,
		yHAND_RESET_R4_X2,
		yHAND_RESET_F1_X2,
		yHAND_RESET_F2_X2,
		yHAND_RESET_F3_X2,
		yHAND_RESET_F4_X2,

		yMAX_COUNT,
	};



	enum eMotor {
		eAxisAll = -1,
		eAxisX1 = 0,
		eAxisX2,
		eAxisY,
		eAxisZ1,
		eAxisZ2,


		eAxisPitch_HW_Y1_SW_X1,
		eAxisPitch_HW_Y2_SW_X2,
		eAxisPitch_HW_X1_SW_Y1,
		eAxisPitch_HW_X2_SW_Y2,


		eAxisMax
	};


	enum eAxisPos {
		ePos_AxisX1,
		ePos_AxisY1,
		ePos_AxisZ1,
		ePos_AxisPitchX1,
		ePos_AxisPitchY1,
		ePos_AxisX2,
		ePos_AxisY2,
		ePos_AxisZ2,
		ePos_AxisPitchX2,
		ePos_AxisPitchY2,
		ePos_Max

	};

	enum eTblStepMoveCase {
		eTblStepMoveCase_Normal = 0,
		eTblStepMoveCase_Abnormal,

		eTblStepMoveCase_LDHand_LDTbl = 0, //
		eTblStepMoveCase_LDHand_UDTbl,
		eTblStepMoveCase_UDHand_LDTbl,
		eTblStepMoveCase_UDHand_UDTbl,


	};

	enum eVATLED {
		eUpperCam,
		eDownCam
	};

public:
	CTaskTestPP(void);
	~CTaskTestPP(void);

	// Common
	int					Initialize(int nTestPPIdx, char* szDriverFileName);
	void				Finalize();

	void				ThreadProc1();

	int					OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	void				OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	// Motor
	OneAxis*			GetAxis(int nAxisIdx);
	int					GetPickerOffset(int nLocStage, std::vector<CPoint> vPicker, double* pdOffsetX, double* pdOffsetY);
	int					GetTeachPos(int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ,
							double* pdPosX1, double* pdPosY1, double* pdPosZ1, double* pdPitchX1, double* pdPitchY1,
							double* pdPosX2, double* pdPosY2, double* pdPosZ2, double* pdPitchX2, double* pdPitchY2, int nTestHandType, BOOL bCleanOffsetUse = eCleanOffsetNotUse);
	int					GetTeachPos(int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ,double *dTPos,int nTestHandType, BOOL bCleanOffsetUse = eCleanOffsetNotUse);
	double				GetTeachPos(int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int nUpDown, int nAxisIdx, int nTestHandType, BOOL bCleanOffsetUse = eCleanOffsetNotUse);
	int					ChkMotorPos(int nLocStage, int nTestHandType, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ, int nAxisIdx=eAxisAll);
	int					ChkMotorPos(int nLocStage, int nTestHandType, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ, double dOffset, int nAxisIdx=eAxisAll);
	int					do_MovePos(int nLocStage, int nTestHandType, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ, int nAxisIdx, BOOL bCleanOffsetUse = eCleanOffsetNotUse, int nSpdRate=100, double dPosOffset=0.0);
	int					do_MovePos_Pcd_backlash(double dAbsPosition, std::vector<CPoint> vPicker, int nAxisIdx, int nSeqSpdRate = 5, double dPosOffset = 0.0, BOOL bIsApplyBcklsh = FALSE, int nTestHandType = 0);
	void				GetTeachingPos();
	BOOL				ChkTableMove(int nHandType, int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int &nStep); //TRUE, FALSE
	int					Interlock_MovePos(int nAxisIdx, int nLocStage);
	int					WhereIsMotorPos(int nHandType, std::vector<int>& vLocList, int nAxisIdx=eAxisAll);

	// Inter Lock
	int					Interlock_Move_X(double dTargetX, int nAxis, int nHandType = 0);
	int					IsYRangeRelatedPress_Z_Safety(double dTPosX, double dTPosY, int nHandType, std::vector<int> &nRelatedStationNo);
	int					Interlock_X_Related_Clean_Table_Range(); // Clean Table <-> Station 사이에 영역을 Test Load Hand가 해당 위치에 있는지 확인 하는 인터락 함수
	void				Interlock_Y_Range_Related_Press_Z_Safety(double dCurPos, int nHandType, std::vector<int> &nRelatedStationNo);
	int					Check_X_Related_Press_Z_Safety();
	BOOL				CompareXData(double dData1, double dData2); //Module1, 2 X축 Station 진입 방향이 다름. Interlock에서 사용.
	BOOL				IsHandStationArea(double dTPos);

	// Vac/Blow
	int					do_VacBlow(int nVacBlow, int nHandType, CPoint Picker);
	int					do_VacBlow(int nVacBlow, int nHandType, std::vector<CPoint> vPicker);
	int					do_VacBlow_All(int nVacBlow, int nHandType);
	int					do_NoDeviceResetAll();
	int					Interlock_VacBlow(int nVacBlow, CPoint Picker);
	int					Interlock_VacBlow(int nVacBlow, std::vector<CPoint> vPicker);
	int					GetVacuum(int nHandType, CPoint nPicker);
	int					GetVacuum_All(int nHandType, BOOL bOnOff);
	int					ChkVacuum(BOOL bOnOff, int nHandType, CPoint nPicker);
	int					ChkVacuum(BOOL bOnOff, int nHandType, std::vector<CPoint> vPicker);
	int					ChkVacuum_All(BOOL bOnOff);

	CString				GetErrPickerName(int nUpDown, int nHandtype, std::vector<CPoint> vPicker);
	int					GetHandTypeIndex(int nHandType);
	// DIO
	int					ChkDI(int nIndex, BOOL bValue=TRUE);
	BOOL				GetDI(int nIndex);

	// etc
	void				MakeAllPicker(std::vector<CPoint>& vPicker);
	int					ConvertStageIdx(int nStageCmd);
	
	// hand device data & etc
	int					GetHandDvcData(CPoint Picker,int nHandType, ST_DD_DEVICE* lpstBuff);
	int					SetHandDvcData(CPoint Picker, int nHandType, ST_DD_DEVICE* lpstBuff);
	int					GetHandPkrDvcExist(CPoint Picker, int nHandType);
	int					SetHandPkrDvcExist(CPoint Picker, int nExist);
	int					GetLoadPickerDeviceCnt();
	int					GetUnloadPickerHaveDvcCnt();
	int					GetLoadHandDeviceCnt();
	int					GetUnLoadHandDeviceCnt();
	int					ChkLostDvcCnt(std::vector<CPoint> vCmdPicker, int nHandtype, int nPickPlace);
	int					GetLoadPickerLoadDeviceCnt();
	int					GetUnLoadHandRetestBDeviceCnt();
	BOOL				GetTestPPCleanDevice();


	// shuttle/Art buffer/clean buffer/ station site -> device data
	int					GetCmdStageDvcData(int nCmdStage, CPoint Target, int nHandType, ST_DD_DEVICE* lpstBuff);
	int					SetCmdStageDvcData(int nCmdStage, CPoint Target, int nHandType, ST_DD_DEVICE* lpstBuff);
	int					GetBufferTableCnt();
	int					GetBufferTablePocketCnt();
	int					CheckDeviceType(int nCmdStage, int nHandType, int nTargetX, int nTargetY, std::vector<CPoint> vCmdPicker, int nPickPlace);
	int					DeviceType(int nCmdStage, ST_DD_DEVICE stSourceData, ST_DD_DEVICE stTargetDevice);
	BOOL				TestSdHasDvc();

	BOOL				IsInitNeed();
	// processing device data
	int					ProcDeviceData(int nCmdStage, int nHandType, int nTargetX, int nTargetY, std::vector<CPoint> vCmdPicker, int nPickPlace, std::vector<CString>& vErrstring, int& nRetDeviceType, BOOL bMisMatchErase=FALSE);

	// Get Location Name
	CString				GetLocName(int nLoc);
	CString				GetPickerName(CPoint Picker);
	CString				GetPickerName(std::vector<CPoint> vPicker);
	CString				GetHandTypeName(int nHandType);
	//Get Press Unit Number
	int					GetPressNum(int nCmdStage);

	int					GetHandPitchType(int nAxis, int nLocIdx);

	// arrive sen buffer table
	int					ChkBuffArrivedSen_All(CString &strErrMsg, std::vector<CPoint> &vErrPocket);
	int					ChkBuffArrivedSen(int nY, CString &strErrMsg, std::vector<CPoint> &vErrPocket, std::vector<CString> &vErrHistory);
	
	//backlash
	void				ApplyBacklash(int nHandType);

	BOOL				ChkPkrArt1StBin(std::vector<CPoint> vPicker);

	void				GetUserOffsetData();

	int					ChkBeforeBlowDevice(std::vector<CPoint> vCmdPicker, int nHandType, CString& strErrorMsg, std::vector<CString>& vErrString);
	int					GetPickerErrorCase(int nAxisDone, int nErrSen, std::vector<CPoint> vAxisDonePkr, std::vector<CPoint> vErrSen, int nHandType, std::vector<CPoint>& vErrPicker, CString& sAlarmData);

	void				SetFailPicker(CPoint Picker, int nHandType);

	std::vector<CPoint> GetFailPicker(int nHandType);
	std::vector<CPoint> GetSafetyFailPicker(int nHandType, ePickerSafetySensorCheckType _ePickerHomeSensorCheckType) { return m_vSafetyFailPicker[nHandType][_ePickerHomeSensorCheckType]; }
	std::vector<CPoint> GetLostPicker(int nHandType);

	std::vector<CPoint> AxisDoneFailPickerIdxConverter(std::vector<CPoint> vAxisDonePkr);

	int					FromTestPPToLoadTableIdx(int nCmdStage);
	int					FromTestPPToLoadTableCmdStageConvetor(int nCmdStage);

	int					TransIdxMotorAxisToTeachPos(int nMotorAxis);
	int					CmdStageHistoryStageConvertor(int nCmdStage);
	int					TeachPosIdxConvertor(int nAxisIdx, int nTestHandType);

	int					Chk_Z_Safety_Sen_All(int nHandType);
	int					Chk_Z_Safety_Sen(std::vector<CPoint> vPicker, int nHandType);
	int					Chk_Z_Safety_Sen_Check_Type(std::vector<CPoint> vPicker, int nHandType, ePickerSafetySensorCheckType _ePickerHomeSensorCheckType = ePickerSafetySensorCheckType::eSafetySensorCheck);

	void				MakeLog(LPCTSTR fmt, ...);
public:

	BOOL								m_bInitialized;

	int									m_nTestPPIdx;
	int									m_nTestPPType;

	// Vacuum / Blow
	int									m_nHand_i_VacSen[NAX_TEST_PP_HEAD_COUNT][MAX_TEST_PP_PKR_CNT_Y][MAX_TEST_PP_PKR_CNT_X];

	int									m_nHand_o_Vac[NAX_TEST_PP_HEAD_COUNT][MAX_TEST_PP_PKR_CNT_Y][MAX_TEST_PP_PKR_CNT_X];
	int									m_nHand_o_Blw[NAX_TEST_PP_HEAD_COUNT][MAX_TEST_PP_PKR_CNT_Y][MAX_TEST_PP_PKR_CNT_X];
	int									m_nHand_o_Reset[NAX_TEST_PP_HEAD_COUNT][MAX_TRAY_PP_CNT_Y][MAX_TEST_PP_PKR_CNT_X];

	//Cycle Pause 구간 사용.
	BOOL								m_bCyclePause;

	//GetTeachPos시, Pitch계산 유무.
	BOOL								m_bCalBasePoint;

	// Physical Information
	std::vector<int>					m_vX;           // DI id list
	std::vector<int>					m_vY;           // DO id list
	std::vector<OneAxis*>				m_vMotor;       // Motor Pointer list x, z
	std::vector<CPoint>					m_vFailPicker[eMaxTestPPTypeCount];  
	std::vector<CPoint>					m_vSafetyFailPicker[eMaxTestPPTypeCount][ePickerSafetySensorCheckType::eMaxPickerSafetySensorCheckType];

	CString								m_sSaftyFailPicker[eMaxTestPPTypeCount][ePickerSafetySensorCheckType::eMaxPickerSafetySensorCheckType];

	CMap<int,int,CString,CString>		m_mapDI;
	CMap<int,int,CString,CString>		m_mapDO;
	CMap<int,int,CString,CString>		m_mapMotor;

	// 제어 상수
	double								m_dMotorErrRange[eAxisMax];

	std::vector<int>					m_vModuleSetLoadTableIdx;
	std::vector<CPoint>					m_vLostBlowDvc[eMaxTestPPTypeCount];
   
	ST_USER_OFFSET_TESTPP_MP			m_stTeachPp;		// test site user off set data

	CRamTeaching						m_CTeaching_Data;

	//VAT
public:
	HWND			m_hLog;

	int				m_nBbxmnpAxis[VAT_TESTPP_HAND_TYPE][VAT_PICKER_MAX];

	void			SetVatLogHandle(HWND hOwner);

	// Vat Move Command
	//Speed, Acc, Dcc 외부 설정 의존도 X
	//do_MovePosVAT에 Z축 제거 후, do_MovePosZVAT()로 분리 
	//	=> 해당 함수에서 여러 Picker또는 단축 피커 Move 제어.
	//	=> 단축 Picker 제어의 경우, 파라미터 동일하게 받은 후, 함수내에서 _eBbxmnpAxis로 변경.
	int				do_VATMovePos(int nAxisIdx, double dTargetPos, double dPosOffset = 0.0);
	int				do_VATMovePos_Z(int nAxisIdx, std::vector<CPoint> vPicker, double dTargetPos, double dPosOffset = 0.0);

	void			_SetFlatnessOffset(int nHandType, int nStage);

	void			WriteDisplayLog(const char * fmt, ...);
	void			MakeVatLog(LPCTSTR fmt, ...);
};

