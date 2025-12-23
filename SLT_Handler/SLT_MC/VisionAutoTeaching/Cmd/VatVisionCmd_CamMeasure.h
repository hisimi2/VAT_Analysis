#pragma once
class CVatVisionCmd_CamMeasure : public CBaseSeqCmd, public CVAT_VisionComm
{
public:
	CVatVisionCmd_CamMeasure(int _nHand);
	~CVatVisionCmd_CamMeasure();

	int CheckParam();

	int  OnStop();
	int  OnRetry();
	int  OnSkip();
	int  OnPause();
	int  OnResume();

	BOOL State_Setup();
	BOOL State_Executing();
	BOOL State_Pause();
	BOOL State_Recovery();
	BOOL State_NormalComplete();
	BOOL State_Alarm();
	BOOL State_AlarmComplete();
	BOOL State_Aborting();
	BOOL State_AbortComplete();


private:
	enum _eStep_Vision_Measure
	{
		vision_target_measure_start = 0,

		vision_target_send_measure,
		vision_target_receive_wait,
		vision_target_receive_error_chk,
		vision_data_save,

		vision_target_measure_end = 99999,
	};

public:
	CString act_string(int act);

private:
	//int m_nVisionPC;
	CTimerTickCheck m_tcTimeout;

//Vision Measure Parameter
public:
	//외부 입력 Parameter
	//int m_nDataId;
	std::vector<int> m_vDataId;

	int m_nStatus;
	int m_nHand;//Data Save시 필요.

};


// nCamNo : 실제 Vision Cam Offset Data는 Cam 별로 관리. 
// nHand : 측정 완료 후, 가공된 Data는 Hand별로 관리. 

