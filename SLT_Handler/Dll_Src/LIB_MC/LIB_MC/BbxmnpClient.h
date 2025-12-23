#pragma once
// By pkn [170418] Apply Hand picker(Z-axis motor)

#include "BbxmnpWnd.h"
#include "DefineBbxmnp.h"
#include "TimerTickCheck.h"

typedef struct _MOTOR_PARAM
{
	// Move
	double dSpeed;				//pps
	double dAccTime;			//msec
	double dDecTime;			//msec

	// Origin
	double dOrgSpeed;			//pps
	double dOrgRevSpeed;		//Origin 진행 시에 센서감지 후에 역으로 움직일 경우 [pps]
	double dOrgAccDecTime;		//msec

	double dScaleFactor;		

	// SW Limit
	double dSwLimitP;		// um
	double dSwLimitN;		// um

	_MOTOR_PARAM(){ clear(); };
	void clear() { ZeroMemory(this, sizeof(_MOTOR_PARAM)); };
}MOTOR_PARAM;

typedef struct _PICK_OFFSET {
	// Pos Offset
	double dFlatOffset[eBMAxis_Picker_MAX];



	_PICK_OFFSET(){ clear(); };
	void clear() { ZeroMemory(this, sizeof(_PICK_OFFSET)); };
}PICK_OFFSET;

// typedef struct _Z_TEACHING_DATA
// {
//  	//Hand 각 Parts Pick Position, 먼저 Vacuum 감지된 Position
//  	double dLoadTrayPP_PickPos[MAX_TRAY_PP_LOC_COUNT];
//  	double dUnloadTrayPP_PickPos[MAX_TRAY_PP_LOC_COUNT];
//  	double dTestPp1_PickPos[TESTPP_HAND_TYPE][MAX_TEST_PP_LOC_COUNT];
//  	double dTestPp2_PickPos[TESTPP_HAND_TYPE][MAX_TEST_PP_LOC_COUNT];
//  
//  	//Picker간의 높이 Gap
//  	double dLoadTrayPp_Loc_PickerGap[MAX_TRAY_PP_LOC_COUNT][eBMAxis_Picker_MAX];
//  	double dUnloadTrayPp_Loc_PickerGap[MAX_TRAY_PP_LOC_COUNT][eBMAxis_Picker_MAX];
//  	double dTestPp1_Loc_PickerGap[TESTPP_HAND_TYPE][MAX_TEST_PP_LOC_COUNT][eBMAxis_Picker_MAX];
//  	double dTestPp2_Loc_PickerGap[TESTPP_HAND_TYPE][MAX_TEST_PP_LOC_COUNT][eBMAxis_Picker_MAX];
// 
// 	_Z_TEACHING_DATA() { clear(); };
// 	void clear() { ZeroMemory(this, sizeof(_Z_TEACHING_DATA)); };
// }Z_TEACHING_DATA;


// 초기화 시에 Pulse type, Encoder Mode 같은 것을 설정해야 하는지 확인할 것
// 주의!!! -> 함수에서 _ePICK_MOTOR 타입은 해당 Hand의 Z축 전체(picker 8개)를 의미하고 _eBbxmnpAxis 타입은 개별축을 의미한다.
//			  여러 축을 묶어서 사용할 경우에 반드시 _eBbxmnpAxis로 정의된 것을 묶어서 사용하세요.
//			  단위 => speed[pps], Accel Time[msec], Decel Time[msec]

class CBbxmnpClient : public CWnd
{
public:
	enum _ePICK_MOTOR { PICK_MOTOR_LOAD_TRAY,PICK_MOTOR_LOAD_TEST_1,PICK_MOTOR_UNLOAD_TEST_1,
		PICK_MOTOR_UNLOAD_TRAY, PICK_MOTOR_LOAD_TEST_2, PICK_MOTOR_UNLOAD_TEST_2, PICK_MOTOR_MAX };
	enum _eMOTOR_STS_TYPE { eMotorSts_Org, eMotorSts_EZ, eMotorSts_EMG, eMotorSts_INP, eMotorSts_ALM, 
							eMotorSts_LimitP, eMotorSts_LimitN, eMotorSts_CRC, eMotorSts_ServoOn, eMotorSts_Reset,
							eMotorSts_HomeOK};

	enum _eMOTION_STS_TYPE {
		eMotionSts_DRV, eMotionSts_WAIT_DR, eMotionSts_WAIT_STA, eMotionSts_WAIT_INSYNC, eMotionSts_WAIT_OTHER, eMotionSts_WAIT_ERC,
		eMotionSts_WAIT_DIR, eMotionSts_BACKLASH, eMotionSts_WAIT_PE, eMotionSts_IN_FA, eMotionSts_IN_FL, eMotionSts_IN_FUR,
		eMotionSts_IN_FH, eMotionSts_IN_FDR, eMotionSts_WAIT_INP, eMotionSts_IN_CMP, eMotionSts_WAIT_SYNC, eMotionSts_WAIT_GANT,
	};
	enum _ePICKER_POS { ePICKER_REAR, ePICKER_FRONT, ePICKER_MAX,}; // Slave Board에 포함된 축들끼리만 동시제어 가능
// Function
public:
	CBbxmnpClient(void);
	~CBbxmnpClient(void);

	CLogger m_MtLog;

	int m_nSimul;
	double m_dActualPos[PICK_MOTOR_MAX * eBMAxis_Picker_MAX];
	udtCardStatus m_SimStatus[PICK_MOTOR_MAX * eBMAxis_Picker_MAX];

	int Initialize(char* szFilePath);
	void Finalize();
	void LoadPickOffset();
	void SavePickOffset();
	void SetInitAxisInfo(_ePICK_MOTOR eMotor, double dScaleFactor, int nPulseDir = 1, int nMoveDir = 1,int nSubZAxisNum = 0);
	void SetInitParameter(_ePICK_MOTOR eMotor);
	void SetInitMotorParam(_ePICK_MOTOR eMotor, double dSpeed, double dAcc, double dDec, 
						double dOrgSpeed, double dOrgAcc, double dSwLimitP, double dSwLimitN, double dScaleFactor);
	void SetPickOffset(_ePICK_MOTOR eMotor, PICK_OFFSET stPickOffset);
	PICK_OFFSET GetPickOffset(_ePICK_MOTOR eMotor);
	void SetOriginMode(_ePICK_MOTOR eMotor, int nMode);

	int SetSwLimitPos(_ePICK_MOTOR eMotor);
	int SetSwLimitAction(_ePICK_MOTOR eMotor, int nAction);
	int SetSwLimitEnable(_ePICK_MOTOR eMotor, int nEnable);
	BOOL GetMotorStatus(_ePICK_MOTOR eMotor,_eBbxmnpAxis eAxis, _eMOTOR_STS_TYPE eStatusType); // 해당 Arm 해당 축의 특정 Status 반환용
	BOOL GetMotorStatus(_eBbxmnpAxis eAxis, _eMOTOR_STS_TYPE eStatusType);					// 해당 축에 대한 특정 Status 반환용
	void GetMotorStatus(_ePICK_MOTOR eMotor, _eMOTOR_STS_TYPE eStatusType, BOOL* pStatus);	// 해당 Arm의 특정 Status 반환용
	BOOL GetMotionStatus(_eBbxmnpAxis eAxis, _eMOTION_STS_TYPE eType);
	void GetMotionStatus(_ePICK_MOTOR eMotor, _eMOTION_STS_TYPE eType, BOOL *pStatus);

	// Servo On/Off
	int ServoOn(_eBbxmnpAxis eAxis, bool bEnable);
	int ServoOn(_ePICK_MOTOR eMotor, bool bEnable);
	int Reset(_ePICK_MOTOR eMotor);
	// Origin Function
	int  Origin(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);
	BOOL OriginWhile(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, BOOL bMsgDisplay = FALSE);
	int  MultiOrigin(_ePICK_MOTOR eMotor);
	BOOL MultiOriginWhile(_ePICK_MOTOR eMotor);
	
	// Move Function
	int  Move(_eMoveType eMoveType, _eBbxmnpAxis eAxis, double dMovePos, double dSpeed = 0, double dAccTime = 0, double dDecTime = 0, int nBoardType = eBoardDir_Forward, BOOL bNoUseOffset = FALSE);
	BOOL MoveWhile(_eMoveType eMoveType, _eBbxmnpAxis eAxis, double dMovePos, double dSpeed = 0, double dAccTime = 0, double dDecTime = 0,  BOOL bNoUseOffset = FALSE);
	int  Move(_eMoveType eMoveType, _ePICK_MOTOR eMotor, std::vector<CPoint> vPicker, double dMovePos, double dSpeed = 0, double dAccTime = 0, double dDecTime = 0, int nBoardType = eBoardDir_Forward, BOOL bNoUseOffset = FALSE/*, BOOL bVatOffsetUse = FALSE*/);
	BOOL MoveWhile(_eMoveType eMoveType, _ePICK_MOTOR eMotor, std::vector<CPoint> vPicker, double dMovePos, double dSpeed = 0, double dAccTime = 0, double dDecTime = 0, BOOL bNoUseOffset = FALSE);
	BOOL MoveWhile(_eMoveType eMoveType, _ePICK_MOTOR eMotor, double dMovePos, double dSpeed = 0, double dAccTime = 0, double dDecTime = 0, BOOL bNoUseOffset = FALSE);
	BOOL MoveWhiletoLastPos(_eMoveType eMoveType, _ePICK_MOTOR eMotor, double dSpeed = 0, double dAccTime = 0, double dDecTime = 0);

	// JOG Function
	int JogMove(_eJogDir eJogDir, _eBbxmnpAxis eAxis, double dSpeed, double dAccDecTime);
	int JogMove(_eJogDir eJogDir, int nNumOfAxes, int* anAxes, double dSpeed, double dAccDecTime);

	// Position Get/Set
	double GetCommandPos(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);
	void SetCommandPos(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, double dPos);
	double GetActualPos(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);
	void SetActiaulPos(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, double dPos);

	// Check Move End
	int CheckMoveEnd(_eBbxmnpAxis eAxis);
	int CheckMoveEnd(int nNumOfAxes, int* anAxes);
	int CheckMoveEnd(_ePICK_MOTOR eMotor);

	// Stop
	int Stop(_ePICK_MOTOR eMotor );
	int Stop(_eBbxmnpAxis eAxis);
	int Stop(int nNumOfAxes, int* anAxes );
	int EStop(_ePICK_MOTOR eMotor);
	int EStop(_eBbxmnpAxis eAxis);
	int EStop(int nNumOfAxes, int* anAxes );

	void SetOriginFlag(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, bool bFlag);
	void SetOriginFlag(_ePICK_MOTOR eMotor, bool bFlag);
	bool GetOriginFlag(_ePICK_MOTOR eMotor);
	bool GetOriginFlag(_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);

	void SetTargetPos(_ePICK_MOTOR eMotor, double dTargetPos);
	double GetTargetPos(_ePICK_MOTOR eMotor);
	void SetLastPos(_ePICK_MOTOR eMotor, int nPickerNo, double dLastPos);
	double GetLastPos(_ePICK_MOTOR eMotor, int nPickerNo );

	void AxisTransToName(_eBbxmnpAxis eAxis, CString& strText);
	void AxisTransToName(_ePICK_MOTOR eMotor, CString& strText);
	void AxisTransToPickMotor(_eBbxmnpAxis eAxis, _ePICK_MOTOR& eMotor);

	void AxisTransToVatHand(_ePICK_MOTOR eMotor, int &nHand);
 	//void TransToRamdataMotorParam(_ePICK_MOTOR eMotor, St_MotorParameter& ramdataMotorParam);
 	//void TransToBbxmnpClientMotorParam(_ePICK_MOTOR eMotor, const St_MotorParameter& ramdataMotorParam);
	int SetPulseType(_eBbxmnpAxis eAxis, int nType);
	int SetOrgLevel(_eBbxmnpAxis eAxis, int nLevel);
	int SetOrgLevel(_ePICK_MOTOR eMotor, int nLevel);
	void SetOperAxes(_ePICK_MOTOR eMotor, std::vector<CPoint> vPicker, int nOperAxes[2][4], int nOperAxesCnt[PICK_MOTOR_MAX]);
	void SetHandMatrix();
	void  SetMaxVel(_ePICK_MOTOR eMotor);

	std::vector<int> m_vHandMatrix; // 1 ~ 48 Z Axis Motor  1차원 Vetor
	std::vector<int> m_vHandAxis[PICK_MOTOR_MAX];  // [Motor Type][ 1~8 Z Axis Motor]   2차원 Vetor


	BOOL GetBoardCommSts(int *pBoardConnectSts);
	static void OnReceiveAlarm(CWnd* pWnd, LPARAM lParam, WPARAM wParam );
	void OnAlarmDisplay();

	BOOL GetHomeSensorStatus(_ePICK_MOTOR eMotor);
	BOOL CheckHomeSensor(int nOrigin[MAX_PICKER_CNT_EACH_SLAVE]);

	BOOL GetHomeSensorStatusEach(_ePICK_MOTOR eMotor, int nPickerAxis);
	BOOL GetHomeSensorStatus(_ePICK_MOTOR eMotor, int nPickerAxis);

	BOOL CheckHomeSensorEach(int nOrigin[MAX_PICKER_CNT_EACH_SLAVE], int nPickerAxis);
	
	double ApplyZMotorFlatOffset(_eBbxmnpAxis eAxis);
	int BoardSlaveNumber(_ePICK_MOTOR eMotor, int nPickerAxis);
	int BoardSlavePickerNumber(_ePICK_MOTOR eMotor, int nPickerAxis);
	int ZAxisStartPicker(_ePICK_MOTOR eMotor);
	void BoardSlaveIndex(_ePICK_MOTOR eMotor, int &nBDStartNum, int &nBDEndNum);
	int BoardNumber(_ePICK_MOTOR eMotor);
	int GetZPickerMotorNumber(int nAxisNumber);
	void MakeBMAxisPicker(std::vector<int>& vPicker, CBbxmnpClient::_ePICK_MOTOR eMotor);

	double m_All_Last_Pos[PICK_MOTOR_MAX][ eBMAxis_Picker_MAX ];

	PICK_OFFSET m_stPickOffset[PICK_MOTOR_MAX]; // Picker Offset

	double GetPickerOffset(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis);

	int ChangePkrNum(_ePICK_MOTOR eMotor, int nPickerAxis);

	CString GetPickerName(CPoint Picker);

	CBbxmnpWnd m_bbxmnpWnd;
	// Variable
private:

	
	bool m_bOriginFlag[PICK_MOTOR_MAX][eBMAxis_Picker_MAX];
	MOTOR_PARAM m_stMotorParam[PICK_MOTOR_MAX]; // 순수 MotorParam으로 외부통해서 받는다
	

	double m_adTargetPos[PICK_MOTOR_MAX];
	double m_adLastPos[PICK_MOTOR_MAX][ eBMAxis_Picker_MAX ];

	// Scale Factor
	double m_adScaleFactor[PICK_MOTOR_MAX];	// um/pulse
	int m_anMoveDir[PICK_MOTOR_MAX];
	int m_anPulseDir[PICK_MOTOR_MAX];

	std::vector<int> m_veBDSlaveNum;

	CTimerTickCheck	m_swTimeout;
	int m_nBoardType;

	std::vector<int> vePickerDirIdx;


public:
	
	//void SetZAutoTeachingData(double dHandGap[]);

	//Z_TEACHING_DATA m_stZAutoTeachingData; // Picker Offset
	std::vector<CPoint> VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR eMotor);
	void MakeAllPicker(std::vector<CPoint>& vPicker);

//Vision Auto Teaching Data 
public:
	//신규작성.
	double m_dHandGap[PICK_MOTOR_MAX][eBMAxis_Picker_MAX];
	
	void SetFlatnessOffset(int nHand, double dHandGap[]);

	int ReConnection();
};

