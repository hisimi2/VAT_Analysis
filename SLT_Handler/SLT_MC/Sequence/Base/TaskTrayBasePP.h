/*****************************************************************************
* CTaskTrayBasePP는 TWSL301N에서 사용되었던 CTaskTrayPP를 일반화한 클래스 입니다.
* Hand Hand와 Unload Hand에서 공통으로 사용 할 수 있도록 일반화 시킵니다.
* Load Hand에 Y축이 없는 것을 제외하면 모든 구성 요소가 동일 합니다.
* 작업 위치에 따른 티칭 테이블은 다음과 같다. https://postimg.cc/HVnK7YKT
* Load Hand가 Y축이 없고 Unload Hand는 Feeder Unit에서 작업하지 않지만 공통으로
* 사용하기 위해 위와 같은 테이블 구조를 갖는다.
******************************************************************************/
#pragma once
#include "../../Dll_Src/LIB_MC/LIB_MC/BbxmnpClient.h"

enum eCheckAbnormalCase {
	invaild_case = -1,
	cmd_end = 0, // 모든 picker 상태 검사 해야 함.
	abnormal,
	alarm_drop,
	alarm_device_exist,
	situation,
	normal_start, // 모든 picker 상태 검사 해야 함.
};

class CHw_Bin_Sort_Item {
public:
	eDeviceDataStage eHwBin;
	char szSwBin[16];
	char szErrStr[128];
	double dLoadingTime;

	CHw_Bin_Sort_Item() {
		ZeroMemory(szSwBin, sizeof(szSwBin));
		ZeroMemory(szErrStr, sizeof(szErrStr));
		dLoadingTime = 0.0;
		eHwBin = eDeviceStage_STACKER_CONV_1;
	}


	//     CHw_Bin_Sort_Item& operator=(const CHw_Bin_Sort_Item& operand){
	//         eHwBin = operand.eHwBin;
	//         strSwBin = operand.strSwBin.GetString();//deep copy
	//         dLoadingTime = operand.dLoadingTime;
	//         return (*this);
	//     }
};

typedef struct _ST_PICKER_INFO {
	int nPlusX;
	int nPlusY;

	int nRow;
	int nCol;

	_ST_PICKER_INFO()
	{
		memset(this, 0, sizeof(_ST_PICKER_INFO));
	}
}ST_PICKER_INFO;


class CTaskTrayBasePP : public CBaseTaskManager
{
public:
	enum eVATLED {
		eUpperCam,
		eDownCam,

		eVATLED_MAX,
	};

	enum eTeachPosZ {
		eZPOS_Undefined = -1,
		eZPOS_PICK = 0,
		eZPOS_PLACE,
		eZPOS_SAFETY,
		eMAX_ZPOS_COUNT,
	};

	enum eMotor {
		eAxisAll = -1,
		eAxisX = 0,
		eAxisY,
		eAxisZ,
		eAxisPitchX,
		eAxisPitchY,
		eAxisMax
	};

	enum eFeederImpactPos {
		eNone = -1,
		
	};

	// base
	CTaskTrayBasePP();
	~CTaskTrayBasePP();

			void		ASSERT_AXIS_IDX( eMotor eAxisIdx );
// Motor
// Motor Meta data
			int			GetMotorCount();
			bool		EmptyMotor( eMotor eAxisIdx );

// motor control object
			OneAxis*	GetAxis( eMotor nAxisIdx );

// motor teaching data
			int			GetPickerOffset( eLocIdxTrayPP nLocStage, std::vector<CPoint> vPicker, double* pdOffsetX, double* pdOffsetY );
			int			GetTeachPos( eLocIdxTrayPP nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ nLocZ, double* pdPosX, double* pdPosY, double* pdPosZ, double* pdPitchX, double* pdPitchY );
			double		GetTeachPos( eLocIdxTrayPP nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ nLocZ, eMotor nAxisIdx );

// motor position state
			int			ChkMotorPos( eLocIdxTrayPP nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ nLocZ, eMotor nAxisIdx = eAxisAll );
			int			ChkMotorPos( eLocIdxTrayPP nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ nLocZ, double dOffset, eMotor nAxisIdx = eAxisAll );
	virtual int			WhereIsMotorPosXY( eLocIdxTrayPP& pos_xy ) = 0;
			bool		WhereIsMotorPosZ();
			bool		WhereIsMotorPosZEach(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);
			int			WhereIsMotorPos( eLocIdxTrayPP& pos_xy, bool& pos_z_safety );
			int         Chk_Z_Safety_Sen_All();
			int         Chk_Z_Safety_Sen(std::vector<CPoint> vPicker);
			int         Chk_Z_Safety_Sen_Check_Type(std::vector<CPoint> vPicker, ePickerSafetySensorCheckType _ePickerHomeSensorCheckType = ePickerSafetySensorCheckType::eSafetySensorCheck);
	virtual bool        is_safety_state()=0;

// motor move command
			int			do_MovePos( eLocIdxTrayPP nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ nLocZ, eMotor nAxisIdx, int nSeqSpdRate = 100, double dPosOffset = 0.0 );
			int			do_MovePos_Pcd_backlash(std::vector<CPoint> vPicker, double dAbsPosition, eMotor nAxisIdx, int nSeqSpdRate = 5, double dPosOffset = 0.0, BOOL bIsApplyBcklsh = FALSE );
			

// Vac/Blow
			int			do_VacBlow( eVacBlowState nVacBlow, CPoint Picker );
			int			do_VacBlow( eVacBlowState nVacBlow, std::vector<CPoint> vPicker );
			int			do_VacBlow_All( eVacBlowState nVacBlow );
			int			do_NoDeviceResetAll();
			eOnOffState	GetVacuum( CPoint Picker );
			int			GetVacuum_All( eOnOffState bOnOff );
			int			ChkVacuum( eOnOffState bOnOff, CPoint Picker );
			int			ChkVacuum( eOnOffState bOnOff, std::vector<CPoint> vPicker );
			int			ChkVacuum_All( eOnOffState bOnOff );

			void        DivSpecOutPicker(std::vector<CPoint> &vPickerNormal, std::vector<CPoint> &vPickerSpecOut);
	virtual	BOOL        GetPickerAlignStatus(int nX, int nY) = 0;
	virtual void        GetPickerOffset(double &dOffsetX, double &dOffsetY, int nX, int nY) = 0;

			BOOL		IsInitNeed();

// etc
			int			GetDeviceDataExist( CPoint Picker, eExistState* pnExist );
			int			SetDeviceDataExist( CPoint Picker, eExistState nExist );
			void		MakeAllPicker( std::vector<CPoint>& vPicker );
			int			GetTrayPpDvcCnt();
			int			GetTrayPpFrontPickerDvcCnt();
public:
			void	    AddLoadTrayPickFailCnt(int nCnt);
			BOOL	    ChkLoadTrayPickFailCntIsOver();
			void	    ResetLoadTrayPickFailCnt();

			CString		GetPickerName(CPoint Picker);
			CString     GetPickerName(std::vector<CPoint> vPicker);

			int			GetPickerErrorCase(int nAxisDone, int nErrSen, std::vector<CPoint> vAxisDonePkr, std::vector<CPoint> vErrSen, std::vector<CPoint>& vErrPicker, CString& sAlarmData);

private:
			int         m_nLoadTrayAutoSkipCount;

public:

// Device Data
			int         GetDeviceData_Picker(int nX, int nY, ST_DD_DEVICE* lpstBuff);
			int         SetDeviceData_Picker(int nX, int nY, ST_DD_DEVICE* lpstBuff);
			int			ProcDeviceData( eLocIdxTrayPP eDestStage, int nDestPocektX, int nDestPocektY, std::vector<CPoint> vCmdPicker, ePPCmd ePickPlace, std::vector<CString>& vErrString, int nMisMatchErase = eDeviceData_Nothing );
			int			ConvertCmdIdxToStageIdx( eLocIdxTrayPP nCmdStage, eDeviceDataStage* nStageIdx );
			int			GetTargetCmdStageDvcData(int nX, int nY, ST_DD_DEVICE* lpstBuff);
			int			SetTargetCmdStageDvcData(int nX, int nY, ST_DD_DEVICE* lpstBuff);
			int			GetTargetCmdStageDvcData( eDeviceDataStage nStageIdx, int nX, int nY, ST_DD_DEVICE* lpstBuff );
			int			SetTargetCmdStageDvcData( eDeviceDataStage nStageIdx, int nX, int nY, ST_DD_DEVICE* lpstBuff );

// lost device
			CString		ChkLostDevice( eLocIdxTrayPP nCmdStage, int nTargetX, int nTargetY, std::vector<CPoint> vCmdPicker, ePPCmd nPickPlace );
			int			GetLostDvcCount( std::vector<CPoint> vCmdPicker, ePPCmd nPickPlace );
			CString		GetLocName( eLocIdxTrayPP nCmdStage );
			int		    CheckDevice(CString& strMsg, std::vector<CString>& vErrString);
			int		    CheckDevice(CString& strMsg, std::vector<CPoint> vCmdPicker, std::vector<CString>& vErrString);
	virtual	void        LossDevice();

// vat vision led
			void		VAT_VisionLED_ONOFF( eOnOffState nCmdOnOff, eVATLED nCamUpDown );
			void		VAT_VisionLED_AllOff();

			int         GetHandPitchType(int nAxis, int nLocIdx);
			std::vector< std::pair< CPoint, CPoint > > change_picker_to_pocket_point(int nCmdStage, int nTargetX, int nTargetY, std::vector< CPoint > vCmdPicker);


			int         GetTrayPpUnloadDvcCnt();
			int         GetTrayPpLoadDvcCnt();
			int         GetBufferDeviceCnt();

	virtual void        ApplyVATOffset()=0;	        
			int			TrayHandLocIdxConvertor(int nRobotID, int nLocIdx);
			int			TrayHandManuLocIdxConvertor(int nRobotID, int nLocIdx);

			void		SetPkrImpactFeeder(int nPkrIdx);
			void		GetPkrImpactFeeder(std::vector<int> &vPkr);
			void		ClearImpactFeeder();

			int			ProcVacuumCheckMem(eLocIdxTrayPP eDestStage, int nDestPocektX, int nDestPocketY, std::vector<CPoint> vCmdPicker, ePPCmd ePickPlace);

			void		SetRobotIdIdx(int nRobotID) { m_nRobotId_Idx = nRobotID; }
			int         GetRobotIdIdx() { return m_nRobotId_Idx; };

			std::vector<int> m_vImpactPkr;
public:
	BOOL                               m_bInitialized;
	std::vector<CPoint>                m_vFullPkr;

	std::vector<CPoint> GetFailPicker() { return m_vFailPicker; }
	std::vector<CPoint>				   m_vFailPicker;
	stMatrix<char>                     m_TrayPP_picker_setting;
	std::vector<CPoint>				   m_vSafetyFailPicker[ePickerSafetySensorCheckType::eMaxPickerSafetySensorCheckType];
	std::vector<CPoint> GetSafetyFailPicker(ePickerSafetySensorCheckType _ePickerHomeSensorCheckType) { return m_vSafetyFailPicker[_ePickerHomeSensorCheckType]; }
	
	CString							   m_sSaftyFailPicker[ePickerSafetySensorCheckType::eMaxPickerSafetySensorCheckType];

	//Cycle Pause 구간 사용.
	BOOL							   m_bCyclePause;
private:
	int                                m_nRobotId_Idx; // 0 : Load PP, 1 : Unload PP

protected:					

	enum class eDO : int {};
	enum class eDI : int {};


	int                                NDM_TrayPP_RetryCnt;
	int                                NDM_TM_TrayPP_CylDownWait;
	int                                NDM_TM_TrayPP_Vacuum;
	int                                NDM_TM_TrayPP_Blow;

	std::vector<int>                   m_vDevLossX;
	std::vector<int>                   m_vDevLossY;

	eDeviceDataStage                   m_eMyDeviceStageNum;
					                   
	eDI                                m_nHand_i_VacSen[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];
	eDO                                m_nHand_o_Vac   [ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];
	eDO                                m_nHand_o_Blw   [ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];
	eDO                                m_nHand_o_Pick  [ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];
	eDO                                m_nHand_o_Reset [ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];
	eDO                                m_nVATvisionLED_o_ONOFF[ eVATLED_MAX ];

	// Physical Information
	std::vector<int>                   m_vX;
	std::vector<int>                   m_vY;
	std::vector<OneAxis*>              m_vMotor; // eMotr로 구성된 motor object에 대한 포인터가 모두 포함되어 있다.
									             // Load Hand는 Y축이 없어서 null로 채워 넣는다.

	CMap<int, int, CString, CString>   m_mapDI;
	CMap<int, int, CString, CString>   m_mapDO;
	CMap<int, int, CString, CString>   m_mapMotor; // motor 설정 데이터로 driver.ini에 설정되어 있는 축 명칭에 따른 축번호를 읽어 들인 값. index에 따른 축 정보 key name
	
	// ↑↑↑↑↑ 개별적으로 설정 해야 하는 멤버들 ↑↑↑↑↑

	// ↓↓↓↓↓↓↓↓↓ 공통으로 사용하는 멤버들 ↓↓↓↓↓↓↓↓↓

	enum eThreadNum : int
	{
		eTHREAD_1,
	};

	// common
	virtual void		MakeLog( LPCTSTR fmt, ... ) = 0;
	

	// DIO
	virtual int			ChkDI( eDI nIndex, eOnOffState bValue = eOn );
	virtual eOnOffState GetDI( eDI nIndex );
	virtual int			SetDO( eDO nIndex, eOnOffState eOnOff );

//VAT
public:
// Make Log	
	//virtual void		MakeVatLog(LPCTSTR fmt, ...) = 0;

	//void MakeVatLog(LPCTSTR fmt, ...) {};


// Vat Move Command
	//Speed, Acc, Dcc 외부 설정 의존도 X
	//do_MovePosVAT에 Z축 제거 후, do_MovePosZVAT()로 분리 
	//	=> 해당 함수에서 여러 Picker또는 단축 피커 Move 제어.
	//	=> 단축 Picker 제어의 경우, 파라미터 동일하게 받은 후, 함수내에서 _eBbxmnpAxis로 변경.
	int			do_VATMovePos(eMotor nAxisIdx, double dTargetPos, double dPosOffset = 0.0);
	int			do_VATMovePos_Z(std::vector<CPoint> vPicker, double dTargetPos, double dPosOffset = 0.0);
	int			m_nBbxmnpAxis[VAT_PICKER_MAX];

	void		_SetFlatnessOffset(int nStage);


};

