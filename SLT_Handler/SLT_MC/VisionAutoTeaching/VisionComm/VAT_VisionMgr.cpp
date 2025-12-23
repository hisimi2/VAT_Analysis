#include "stdafx.h"
#include "VAT_VisionMgr.h"


CVAT_VisionMgr::CVAT_VisionMgr()
{
	m_nCam = -1;

	m_nRcvStatus = VAT_RCV_STATUS_INIT;
	m_vRcvData.clear();
}


CVAT_VisionMgr::~CVAT_VisionMgr()
{
}


void CVAT_VisionMgr::Initialize(int nCam)
{
	m_nCam = nCam;

	CString strFileName = _T("");
	switch (nCam)
	{
	case VAT_CAM_LD_TRAY_PP_UPPER:			
	case VAT_CAM_LD_TRAY_PP_LOWER:			
	{
		strFileName = _T("VisionComm_TrayPP_1.log");
	}break;
	case VAT_CAM_UD_TRAY_PP_UPPER:			
	case VAT_CAM_UD_TRAY_PP_LOWER:			
	{
		strFileName = _T("VisionComm_TrayPP_2.log");
	}break;
	case VAT_CAM_LD_TEST_PP_1_UPPER:		
	case VAT_CAM_UD_TEST_PP_1_UPPER:		
	case VAT_CAM_TEST_PP_1_LOWER: 			
	{
		strFileName = _T("VisionComm_TestPP_1.log");
	}break;
	case VAT_CAM_LD_TEST_PP_2_UPPER:		
	case VAT_CAM_UD_TEST_PP_2_UPPER:		
	case VAT_CAM_TEST_PP_2_LOWER: 			
	{
		strFileName = _T("VisionComm_TestPP_2.log");
	}break;
	default:
		ASSERT("Initialize Fail Cam invalid" == 0);
	}

	//Log 위치 저장.
	char pszFilePath[128] = { 0, };
	sprintf_s(pszFilePath, sizeof(pszFilePath), "D:\\techwing\\VAT_LOG\\%s", strFileName);
	m_Logger.SetFileName(pszFilePath);
	m_Logger.LOG("Initialize Done.");
}


void CVAT_VisionMgr::SetReceiveData(ST_VAT_RCV_DATA vRcvData)
{
	m_vRcvData = vRcvData;

	//Gui Update
	CVAT_MeasureStatus::GetInstance().UpdateVisionOffset(m_nCam, vRcvData.dX, vRcvData.dY);
}


void CVAT_VisionMgr::GetReceiveData(/*int nCam, */ST_VAT_RCV_DATA &vRcvData)
{
	vRcvData = m_vRcvData;
}


void CVAT_VisionMgr::SetReceiveState(/*int nCam, */int nState)
{
	m_nRcvStatus = nState;
	
	//Gui Update
	CVAT_MeasureStatus::GetInstance().UpdateVisionStatus(m_nCam, nState);
}


int CVAT_VisionMgr::GetReceiveState()
{
	return m_nRcvStatus;
}


int CVAT_VisionMgr::SendMeasure(int nDataID, int nStatus)
{
	CPacket* pPacket = new CPacket(107, 9);
	CPacketBody_S107F9* pBody = (CPacketBody_S107F9*)pPacket->pBody;

	//1. DataID
	pBody->stData.nDataID = nDataID;

	//2. Status
	pBody->stData.nStatus = nStatus;

	//3. cData(Cam Number) 
	// 	int nCamNo = ConvertVatHandIdxToCamNo(nHand);
	// 	sprintf_s(pBody->stData.szData[0], sizeof(pBody->stData.szData[0]), "%d", nCamNo);

	//4. SEND MEASURE
	int nCam = ConvertDataIdToVatCam(nDataID);
	if (VISION_REQ_DATAID_VAT_LD_UPPER <= nDataID && nDataID <= VISION_REQ_DATAID_VAT_TEST_1_LOWER) {
		g_pVisionTestHand_1->SendMeasure_VAT(pPacket);
	}
	else {
		g_pVisionTestHand_2->SendMeasure_VAT(pPacket);
	}	

	SetReceiveState(VAT_RCV_STATUS_IDLE);

	//LOG
	CString strLog = _T("");
	strLog.Format("[SND] [Cam = %s] [Status = %s]", ConvertCamIdToString(nCam), ConvertStatusToString(nStatus));
	m_Logger.LOG(strLog);

	return 0;

}


void CVAT_VisionMgr::OnReceive(CPacket* pPacket)
{
	CPacketBody_S107F9* pRcvBody = (CPacketBody_S107F9*)pPacket->pBody;

	if (pRcvBody->stData.nStatus != VAT_RCV_STATUS_SUCCESS) {
		ST_VAT_RCV_DATA stRcvData;
		stRcvData.nState = pRcvBody->stData.nStatus;
		stRcvData.nError = atoi(pRcvBody->stData.szData[0]);

		SetReceiveData(stRcvData);
		SetReceiveState(VAT_RCV_STATUS_ERROR);

		m_Logger.LOG("[Rcv Error] [Cam = %s] [Status = %s] [Error = %d]", ConvertCamIdToString(m_nCam), ConvertStatusToString(stRcvData.nState), ConvertVisionErrIdxToAlarmIdx(stRcvData.nError));
		return;
	}

	//RCV Success
	ST_VAT_RCV_DATA stRcvData;
	stRcvData.nState = pRcvBody->stData.nStatus;
	stRcvData.nError = ERR_VAT_NO_ERROR;
	stRcvData.dX = atof(pRcvBody->stData.szData[1])*1000.0;
	stRcvData.dY = atof(pRcvBody->stData.szData[2])*1000.0;
	stRcvData.nFocusedZIdx = atoi(pRcvBody->stData.szData[3]);

	SetReceiveData(stRcvData);
	SetReceiveState(VAT_RCV_STATUS_SUCCESS);
		
	m_Logger.LOG("[Rcv] [Cam = %s] [Status = %s] [X Offset:%.2f mm, Y Offset:%.2f mm]", ConvertCamIdToString(m_nCam), ConvertStatusToString(stRcvData.nState), stRcvData.dX / 1000.0, stRcvData.dY / 1000.0);
}
