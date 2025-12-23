#pragma once
class CVAT_TaskAutoTestPP : public CBaseTaskManager, public CBaseAutoSeqManager, public CVatBaseTask
{

private:
	enum
	{
		eTHREAD_1 = 0, // SeqCmd running
	};

public:
	int m_nTestPpIdx;  //Auto Initial에서 저장.(고정값)
	int m_nTestPpType; //GUI에서 선택된 Type 저장.(사용자 선택에 따라 상시 변경)

public:
	CVAT_TaskAutoTestPP();
	~CVAT_TaskAutoTestPP();

	//=================================================================================
	// Basic Auto Sequence Management Overide Function
	//=================================================================================
	int  Initialize(int nHand);
	void Finalize();

	int  OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList = NULL);
	int  OnCommandRsp(CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>());
	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	void ThreadProc1();

	void MakeLog(LPCTSTR fmt, ...);

	//VAT Auto Run Mode
	void AutoState_InitNeed();
	void AutoState_Initialize();
	void AutoState_Idle();

	void AutoState_Pause();
	void AutoState_Execute();

	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();

	void AutoState_CleanOut();

	//Excute Mode
	void AutoState_Execute_Z_Auto_Focus();
	void AutoState_Execute_Cam_Center_Calibration();
	void AutoState_Execute_Picker_Align();
	void AutoState_Execute_Z_Auto_Teaching();
	//void AutoState_Execute_Vision_Auto_Teaching();

	void SetProcessCmd(base_seq_cmd::eProcessCommand eCommand);

public:
	void SetHandType(int nType) { m_nTestPpType = nType; }	//필요 없음.
	int GetHandType() { return m_nTestPpType; }				//필요 없음.

	int IsAllCmdReady();
	//CBbxmnpClient::_ePICK_MOTOR GetBbxMnpMotorIdx();


//Vision
public:
	int GET_DATA_ID_UPPER();
	int GET_DATA_ID_LOWER();

	int GET_UPPER_CAM_NUM();
	int GET_LOWER_CAM_NUM();


private:
	//===================================================================================================[Z Auto Focus]
	enum _eZAutoFocusExcutingStep {
		eExcutingStep_ZFocus_Start = 0,
		eExcutingStep_ZFocus_Check_PlateJig,
		eExcutingStep_ZFocus_Move_Start_Pos, //Move

		eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF, //Cam Auto Focus

		// Plate Jig 하단 Target 측정.
		eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure,

		// Plate Jig 상단 Target 측정.
		eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos,
		eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos,
		eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure,			//상단 Target 측정.
		eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk,		//상단 Target Center 확인.

		eExcutingStep_ZFocus_UpperCam_AF,

		eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos,
		eExcutingStep_ZFocus_Find_PlateJig_Z_Fast,
		eExcutingStep_ZFocus_Find_PlateJig_Z_Slow,
		eExcutingStep_ZFocus_Find_PlateJig_Z_End,

		eExcutingStep_ZFocus_End,
	};

	//===================================================================================================[Cam Center Calibration]
	enum _eCamCenterCalExcutingStep
	{
		eExcutingStep_CamCenterCal_Start = 0,
		eExcutingStep_CamCenterCal_Check_PlateJig,

		eExcutingStep_CamCenterCal_Move_Pre_Pos,
		eExcutingStep_CamCenterCal_Move_Target_Pos,

		eExcutingStep_CamCenterCal_Std_Picker_Measure,
		eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk,

		eExcutingStep_CamCenterCal_Move_Z_Safety_Pos,

		eExcutingStep_CamCenterCal_End,
	};


	//===================================================================================================[Picker Align]
	enum _ePickerAlignExcutingStep
	{
		eExcutingStep_PickerAlign_Start = 0,
		eExcutingStep_PickerAlign_Check_PlateJig,

		eExcutingStep_PickerAlign_Select_Measure_Pkr,

		//Picker 초점 맞는 높이 Find
		eExcutingStep_PickerAlign_Move_Picker_Setting_Pos,

		//====loop====
		eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure,
		eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk,
		eExcutingStep_PickerAlign_Move_Picker_Z_Focus_Pos,
		//=============
		eExcutingStep_PickerAlign_Picker_Z_Focus_Pos_Find,


		//Picker Hole 검출
		//====loop====
		eExcutingStep_PickerAlign_Move_Pre_Pos,
		eExcutingStep_PickerAlign_Move_Target_Pos,

		eExcutingStep_PickerAlign_Move_Cur_Picker_Measure,
		eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk,
		//=============

		eExcutingStep_PickerAlign_Move_Safety_Pos,

		eExcutingStep_PickerAlign_Each_Picker_Offset_Cal,

		eExcutingStep_PickerAlign_End,
	};

	//===================================================================================================[Z Auto Teaching]
	enum _eZAutoTeachingExcutingStep {
		eExcutingStep_ZTeaching_Start = 0,
		//eExcutingStep_ZTeaching_Check_Hand_Owner,

		eExcutingStep_ZTeaching_CheckCondition,
		eExcutingStep_ZTeaching_Table_Release,
		eExcutingStep_ZTeaching_Check_All_Part_Complete,
		eExcutingStep_ZTeaching_Select_Part,
		eExcutingStep_ZTeaching_Move_Loading_Tbl,

		eExcutingStep_ZTeaching_Wait_Parts_Move,
		//eExcutingStep_ZTeaching_Move_To_Part,
		eExcutingStep_ZTeaching_Move_Target_Pos,

		eExcutingStep_ZTeaching_Find_Target_Z_Fast,
		eExcutingStep_ZTeaching_Find_Target_Z_Slow,

		//eExcutingStep_ZTeaching_Check_More_Part,
		eExcutingStep_ZTeaching_Find_Target_Z_End,

		eExcutingStep_ZTeaching_End,
		//eExcutingStep_ZTeaching_Wait_Other_Hand,
	};



};

