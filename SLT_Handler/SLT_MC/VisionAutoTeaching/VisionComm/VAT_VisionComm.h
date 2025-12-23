#pragma once
class CVAT_VisionComm
{
public:
	CVAT_VisionComm();
	~CVAT_VisionComm();

public:
	void SaveMeasureData(int nHand, int nDataId, int nStatus);
	
	void SaveMeasureData_PlateJig(int nHand, int nDataId, ST_VAT_RCV_DATA stRcvData);
	void SaveMeasureData_Picker(int nHand, ST_VAT_RCV_DATA stRcvData);
	void SaveMeasureData_C_Tray();
	void SaveMeasureData_Table(int nHand, ST_VAT_RCV_DATA stRcvData);
	void SaveMeasureData_Sockets();

	int m_nMeasurePkrIdx;
	void GetBbxInfo(int nHand, CBbxmnpClient::_ePICK_MOTOR &eMotor, int &nBbxStartIdx);
	void SaveMeasureData_PickerZPos(int nHand);
	void SaveMeasureData_FocusedPickerZIdx(int nHand, ST_VAT_RCV_DATA stRcvData);

	int ChkCamType(int nDataId);
	BOOL IsOffsetOKRange(int nHand, double dOffsetX, double dOffsetY);

	enum _eCamType
	{
		_eCamType_Upper = 0,
		_eCamType_Lower,
	};

	int GetVisionPcNo(int nDataId);

public:
	//Data ID -> VAT CAM
	int ConvertDataIdToVatCam(int nDataId);

	int ConvertVatCamToDataId(int nCam);

	//VAT Mode -> String
	CString ConvertVATModeToString(int nMode);

	//Cam ID -> String
	CString ConvertCamIdToString(int nCam);

	CString ConvertStatusToString(int nStatus);

	//Vision Error Idx -> Alarm Error Idx
	int ConvertVisionErrIdxToAlarmIdx(int nErr);
};

