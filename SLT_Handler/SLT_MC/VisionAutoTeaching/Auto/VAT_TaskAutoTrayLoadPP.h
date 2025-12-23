#pragma once
class CVAT_TaskAutoTrayLoadPP : public CBaseTaskManager, public CBaseAutoSeqManager, public CVatBaseTask
{

private:
	enum 
	{
		eTHREAD_1 = 0, // SeqCmd running
	};

public:
	int IsAllCmdReady();

	BOOL IsCurSeqCmdReady();
public:
	CVAT_TaskAutoTrayLoadPP();
	~CVAT_TaskAutoTrayLoadPP();

	//=================================================================================
	// Basic Auto Sequence Management Overide Function
	//=================================================================================
	int  Initialize();
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

	//Execute Mode
	void AutoState_Execute_Z_Auto_Focus();
	void AutoState_Execute_Cam_Center_Calibration();
	void AutoState_Execute_Picker_Align();
	void AutoState_Execute_Vision_Auto_Teaching();
	void AutoState_Execute_Z_Auto_Teaching();


	void SetProcessCmd(base_seq_cmd::eProcessCommand eCommand);


private:
	int GET_STATUS_ID_UPPER(); //vision

	int GET_VAT_PART_IDX(); //VAT 측정 시, Part Idx 
	int GET_VAT_PART_POS(); //VAT 측정 시, Part Pos


/*	void GET_VAT_PART_*/

//AUTO SCHEDULE STEP
private:
	//===================================================================================================[Z Auto Focus]
	enum _eZAutoFocusExcutingStep
	{
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

		 eExcutingStep_ZFocus_UpperCam_AF,  //Cam Final Auto Focus

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
	
		eExcutingStep_PickerAlign_End,
	};

	//===================================================================================================[Vision Auto Teaching]
	enum _eVisionAutoTeachingExcutingStep {
		eExcutingStep_VAT_Start = 0,
		eExcutingStep_VAT_Check_PlateJig,
		eExcutingStep_VAT_Check_C_Tray_Jig,

		eExcutingStep_VAT_Select_Measure_Part,	//Part Select

		//============================================================ Feeder
		eExcutingStep_VAT_Feeder_Measure_Start,

		//[Plate Jig로 Feeder Pick 위치 선정 방식] --- Test 필요함/현재 사용 안함.
// 		eExcutingStep_VAT_Move_Feeder_Pick_Pre_Pos,
// 		eExcutingStep_VAT_Move_Feeder_Pick_Target_Pos,
// 		eExcutingStep_VAT_Feeder_Pick_Target_Measure,
// 		eExcutingStep_VAT_Feeder_Pick_Target_Measure_Chk,
		

		eExcutingStep_VAT_Move_Stacker_Down,
		eExcutingStep_VAT_Move_Stacker_Load,
		eExcutingStep_VAT_Move_Feeder_Pick_C_Tray_Jig,


		eExcutingStep_VAT_Feeder_Place_C_Tray_Jig,
		eExcutingStep_VAT_Feeder_Move_Safety,
		eExcutingStep_VAT_Check_C_Tray_Jig_Measure_Remain,
		eExcutingStep_VAT_Move_Stacker_Unload,
		eExcutingStep_VAT_Move_Stacker_Up,

		//============================================================= Table
		eExcutingStep_VAT_Table_Measure_Start,

		eExcutingStep_VAT_Calculate_Table_Rotate_Pos, 


		//============================================= Measure Common [Feeder, Table]
		eExcutingStep_VAT_Std_Picker_Move_Target_Pos,
		eExcutingStep_VAT_Find_Unit_Target_Z_Fast,
		eExcutingStep_VAT_Find_Unit_Target_Z_Slow,

		eExcutingStep_VAT_Move_Hand_Pre_Pos_1, 
		eExcutingStep_VAT_Move_Hand_Target_Pos_1,

		eExcutingStep_VAT_Move_Hand_Pre_Pos_2,
		eExcutingStep_VAT_Move_Hand_Target_Pos_2,

		eExcutingStep_VAT_Target_Measure,
		eExcutingStep_VAT_Target_Measure_Chk,
		eExcutingStep_VAT_Target_Measure_Done,

		eExcutingStep_VAT_Table_Release,

		eExcutingStep_VAT_Check_All_Part_Complete,

		eExcutingStep_VAT_End,
	};


	//===================================================================================================[Z Auto Teaching]
	enum _eZAutoTeachingExcutingStep {
		eExcutingStep_ZTeaching_Start = 0,
		eExcutingStep_ZTeaching_CheckCondition,
		eExcutingStep_ZTeaching_Table_Release,
		eExcutingStep_ZTeaching_Check_All_Part_Complete,
		eExcutingStep_ZTeaching_Select_Part,

		//Prepare Measure FEEDER
		eExcutingStep_ZTeaching_Move_Stacker_Down,
		eExcutingStep_ZTeaching_Move_Stacker_Load,
		eExcutingStep_ZTeaching_Feeder_Pick_Stacker,
		eExcutingStep_ZTeaching_Move_Feeder,

		//Prepare Measure TABLE
		eExcutingStep_ZTeaching_Move_Loading_Tbl,
		eExcutingStep_ZTeaching_Wait_Parts_Move,
		//eExcutingStep_ZTeaching_Move_To_Part,

		//Move Target Position
		eExcutingStep_ZTeaching_Move_Target_Pos,

		//Measure Start
		eExcutingStep_ZTeaching_Find_Target_Z_Fast,
		eExcutingStep_ZTeaching_Find_Target_Z_Slow,
		eExcutingStep_ZTeaching_Find_Target_Z_End,

		eExcutingStep_ZTeaching_Feeder_Place_Stacker,
		eExcutingStep_ZTeaching_Feeder_Move_Safety,
		eExcutingStep_ZTeaching_Check_Feeder_Measure_Remain,
		eExcutingStep_ZTeaching_Move_Stacker_Unload,
		eExcutingStep_ZTeaching_Move_Stacker_Up,

		eExcutingStep_ZTeaching_End,
		//eExcutingStep_ZTeaching_Wait_Other_Hand,
	};




}; 

