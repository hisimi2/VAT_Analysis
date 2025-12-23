#include "stdafx.h"
#include "VATSimul.h"

VATSimul::VATSimul()
{
	m_bIsSimul = FALSE;

	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ || VAT_VISION_OS_BY_PASS) {
		m_bIsSimul = TRUE;
  	}
	m_nActionCnt = 0;
	//Random Offset
// 	std::random_device rd;
// 	offset = std::mt19937(rd());  
// 	dist = std::uniform_real_distribution<>(150.0, 200.0);  //Offset (0.15 ~ 0.2)

	offset = std::mt19937(std::random_device{}());
	dist = std::uniform_real_distribution<double>(-150.0, 150.0);
}


VATSimul::~VATSimul()
{
}

void VATSimul::_WaitSeqControl(int wait_time)
{
	if (m_bIsSimul)
		Sleep(wait_time);
}

BOOL VATSimul::_IsRemainSeqCycle()
{
	if (m_bIsSimul && m_nActionCnt < VAT_SIM_MAX_ACTION_COUNT) {
		m_nActionCnt++;
		return TRUE;
	}

	return FALSE;
}

void VATSimul::_ClearParam()
{
	if (m_bIsSimul)
		m_nActionCnt = 0;
}


void VATSimul::_SetVisionRcvData(int nCam, int nStatus)
{
	if (!m_bIsSimul)
		return;

	//Cam Auto Focus는 Vision Offset 없음.
	if (nStatus == VISION_STATUS_AUTO_FOCUS || nStatus == VISION_STATUS_FIND_Z_MEASURE_PICKER || nStatus == VISION_STATUS_FIND_Z_RESULT_PICKER)
		return;

	ASSERT(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER);

	int nCycle = g_DMCont.m_dmVAT.GN(NDM14_VAT_SIMUL_CAM_1_RCV_CNT + nCam);
	ST_VAT_RCV_DATA stRcvData;

	
	nCycle++; //Retry 증가.

	if (nCycle < VAT_SIM_MAX_ACTION_COUNT) {
		stRcvData.dX = dist(offset);
		stRcvData.dY = dist(offset);
	}
	else {
		stRcvData.dX = 0.0;
		stRcvData.dY = 0.0;
	}
	g_VatVisionMgr[nCam].SetReceiveData(stRcvData);


	//초기화 진행.
	if (nCycle >= VAT_SIM_MAX_ACTION_COUNT) {
		g_DMCont.m_dmVAT.SN(NDM14_VAT_SIMUL_CAM_1_RCV_CNT + nCam, 0);
	}
	else {
		
		g_DMCont.m_dmVAT.SN(NDM14_VAT_SIMUL_CAM_1_RCV_CNT + nCam, nCycle);
	}
}