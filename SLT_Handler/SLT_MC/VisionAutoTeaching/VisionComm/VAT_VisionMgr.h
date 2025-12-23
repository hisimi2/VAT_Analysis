#pragma once

//VAT Vision Data 및 상태 독립적으로 관리 하기 위한 클래스.
//VAT에 관련된 상/하단 Cam 10개

class CVAT_VisionMgr : public CVAT_VisionComm
{
public:
	CVAT_VisionMgr();
	~CVAT_VisionMgr();

	CLogger m_Logger;
	int m_nCam;
	void Initialize(int nCam);
public:
	int m_nRcvStatus;
	ST_VAT_RCV_DATA m_vRcvData;
	

	void SetReceiveData(ST_VAT_RCV_DATA vRcvData);
	void GetReceiveData(/*int nCam, */ST_VAT_RCV_DATA &vRcvData);

	void SetReceiveState(/*int nCam, */int nState);
	int GetReceiveState();

// [Send/Receive]
public:
	int SendMeasure(int nDataID, int nStatus);
	void OnReceive(CPacket* pPacket);

};

