#pragma once
// SingleTone Class 생성. [9/4/2025 dohyeong.kim] 
// GUI에서 측정 간, 상태 관리를 위함.
// 공유메모리 사용 X
//  -. 모든 Mode 상태에 대해 작성해야 하는데(Hand, Part, Vision Cam등) 관리가 어려움.
//  -. 여러 쓰레드에서 상시 호출에 문제가 있음.
//
// Vision Data의 경우 g_VATVisionMgr로 따로 관리 되지만, GUI에서 측정 간 상태 관리에 사용 하지 않음.
//  -. Auto 및 Cmd에서 실시간으로 Data 가공 및 save되며, GUI에서도 g_VATVisionMgr 지속 호출될 경우, Data관리에 문제가 있음.
//  -. Critical Section을 사용하더라도 임계 영역 접근 빈도가 많아 Vison Send/Rcv에 Delay가 발생됨.
//  -. 이에 GUI 상태 표기를 위한 Vision 관련 Data들은 SingleTone 인스턴스에서 따로 관리.
#define VAT_VISION_MEASURE_SINGLE		(1)
#define VAT_VISION_MEASURE_AUTO_FOCUS	(-1)

class CVAT_MeasureStatus
{
private:
	CVAT_MeasureStatus();
	~CVAT_MeasureStatus();

	
	//복사 생성자 및 대입 연산 불가능
	//인스턴스 한개로 Single Tone관리
	CVAT_MeasureStatus(const CVAT_MeasureStatus&) = delete;
	CVAT_MeasureStatus& operator=(const CVAT_MeasureStatus&) = delete;

public:
	static CVAT_MeasureStatus& GetInstance();

private:
	//mfc critical_section
	CCriticalSection m_csUnitStatus;
	CCriticalSection m_csProgress; 
	CCriticalSection m_csVisionStatus;
	CCriticalSection m_csVisionOffset;
	CCriticalSection m_csVisionRetryCnt;

	int m_nHandStatus[VAT_HAND_MAX];
	int m_nPickerStatus[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX][VAT_PICKER_MAX];
	double m_dProgressPercent[VAT_HAND_MAX];
	int m_nCamStatus[VAT_CAM_MAX];
	double m_nVisionOffset[VAT_CAM_MAX][VAT_VISION_OFFSET_MAX];
	int m_nCamRetryCnt[VAT_CAM_MAX];


public:
	HWND m_hVatMainPage;
	void SetVatMainPageHandle(HWND hOwner) { m_hVatMainPage = hOwner; };
	

public:
	void ClearStatus();

	//Update Data
	void UpdateHandStatus(int nHand, int nStatus);
	void UpdatePickerStatus(int nHand, int nPitchType, int nPickerIdx, int nStatus);
	void UpdateProgressPercent(int nHand, double dCurPercent);
	void UpdateVisionStatus(int nCam, int nStatus);
	void UpdateVisionOffset(int nCam, double nOffsetX, double nOffsetY);
	void UpdateVisionMeasureCnt(int nCam, int nMeasureCnt);

	void UpdateSaveData();

	//Get Data
	int GetHandStatus(int nHand);
	int GetPickerStatus(int nHand, int nPitchType, int nPickerIdx);
	double GetProgressPercent(int nHand);
	int GetVisionStatus(int nCam);
	std::pair<double, double> GetVisionOffset(int nCam);
	int GetVisionMeasureCnt(int nCam);


};

