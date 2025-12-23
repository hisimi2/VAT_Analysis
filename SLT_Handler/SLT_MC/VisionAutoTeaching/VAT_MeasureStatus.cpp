#include "stdafx.h"
#include "VAT_MeasureStatus.h"


CVAT_MeasureStatus::CVAT_MeasureStatus()
{
	// 	::InitializeCriticalSection(m_cs);
	ClearStatus();
	m_hVatMainPage = NULL;


}


CVAT_MeasureStatus::~CVAT_MeasureStatus()
{
	// 	::DeleteCriticalSection(m_cs);
}

CVAT_MeasureStatus& CVAT_MeasureStatus::GetInstance()
{
	static CVAT_MeasureStatus instance;
	return instance;
}

void CVAT_MeasureStatus::ClearStatus()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_nHandStatus[nHand] = VAT_STATUS_MEASURE_INIT;
		m_dProgressPercent[nHand] = 0.0;
	}

	for (int nCam = 0; nCam < VAT_HAND_MAX; nCam++) {
		m_nCamStatus[nCam] = VAT_RCV_STATUS_INIT;
		m_nCamRetryCnt[nCam] = 0;
	}

	ZeroMemory(m_nVisionOffset, sizeof(m_nVisionOffset));
	ZeroMemory(m_nPickerStatus, sizeof(m_nPickerStatus));
}


void CVAT_MeasureStatus::UpdateHandStatus(int nHand, int nStatus)
{
	m_csUnitStatus.Lock();

	m_nHandStatus[nHand] = nStatus;

	m_csUnitStatus.Unlock();

	if (m_hVatMainPage && ::IsWindow(m_hVatMainPage))
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_HAND_STATUS, (WPARAM)nHand, (LPARAM)NULL);
}


void CVAT_MeasureStatus::UpdatePickerStatus(int nHand, int nPitchType, int nPickerIdx, int nStatus)
{
	m_csUnitStatus.Lock();

	m_nPickerStatus[nHand][nPitchType][nPickerIdx] = nStatus;
	
	WPARAM wParam = SET_BIT_VAT_PICKER_INFO(nHand, nPitchType, nPickerIdx);

	m_csUnitStatus.Unlock();
	
	if (m_hVatMainPage && ::IsWindow(m_hVatMainPage)) 
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_PICKER_STATUS, (WPARAM)wParam, (LPARAM)NULL);
}

void CVAT_MeasureStatus::UpdateProgressPercent(int nHand, double dCurPercent)
{
	m_csProgress.Lock();

	m_dProgressPercent[nHand] = dCurPercent;

	m_csProgress.Unlock();

	if(m_hVatMainPage && ::IsWindow(m_hVatMainPage))
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_PROGRESS, (WPARAM)nHand, (LPARAM)NULL);
}

void CVAT_MeasureStatus::UpdateVisionStatus(int nCam, int nStatus)
{
	m_csVisionStatus.Lock();

	m_nCamStatus[nCam] = nStatus;

	m_csVisionStatus.Unlock();

	if (m_hVatMainPage && ::IsWindow(m_hVatMainPage))
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_VISION_STATUS, (WPARAM)nCam, (LPARAM)NULL);
}

void CVAT_MeasureStatus::UpdateVisionOffset(int nCam, double dOffsetX, double nOffsetY)
{
	m_csVisionOffset.Lock();

	m_nVisionOffset[nCam][VAT_VISION_OFFSET_X] = dOffsetX;
	m_nVisionOffset[nCam][VAT_VISION_OFFSET_Y] = nOffsetY;

	m_csVisionOffset.Unlock();

	if (m_hVatMainPage && ::IsWindow(m_hVatMainPage))
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_VISION_OFFSET, (WPARAM)nCam, (LPARAM)NULL);
}


void CVAT_MeasureStatus::UpdateVisionMeasureCnt(int nCam, int nMeasureCnt)
{
	m_csVisionRetryCnt.Lock();

	m_nCamRetryCnt[nCam] = nMeasureCnt;

	m_csVisionRetryCnt.Unlock();

	if (m_hVatMainPage && ::IsWindow(m_hVatMainPage))
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_VISION_RETRY_CNT, (WPARAM)nCam, (LPARAM)NULL);
}

void CVAT_MeasureStatus::UpdateSaveData()
{
	if (m_hVatMainPage && ::IsWindow(m_hVatMainPage))
		::PostMessage(m_hVatMainPage, UM_VAT_SEQ_MSG_UPDATE_SAVE_DATA, (WPARAM)NULL, (LPARAM)NULL);
}


int CVAT_MeasureStatus::GetHandStatus(int nHand)
{
	m_csUnitStatus.Lock();

	int nRet = 0;
	if (VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2)
		nRet = m_nHandStatus[nHand];
	
	m_csUnitStatus.Unlock();

	return nRet;
}


int CVAT_MeasureStatus::GetPickerStatus(int nHand, int nPitchType, int nPickerIdx)
{
	m_csUnitStatus.Lock();

	int nRet = 0;
	if ((VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2) &&
		(nPitchType == VAT_PITCH_TYPE_WIDE || nPitchType == VAT_PITCH_TYPE_NARROW) &&
		(VAT_PICKER_01 <= nPickerIdx && nPickerIdx <= VAT_PICKER_08))
	{
		nRet = m_nPickerStatus[nHand][nPitchType][nPickerIdx];
	}

	m_csUnitStatus.Unlock();

	return nRet;
}


double CVAT_MeasureStatus::GetProgressPercent(int nHand)
{
	m_csProgress.Lock();

	double dRet = 0.0;
	if (VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2)
		dRet = m_dProgressPercent[nHand];

	m_csProgress.Unlock();

	return dRet;
}


int CVAT_MeasureStatus::GetVisionStatus(int nCam)
{
	m_csVisionStatus.Lock();

	int nRet = VAT_RCV_STATUS_ERROR;

	if (VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER)
		nRet = m_nCamStatus[nCam];

	m_csVisionStatus.Unlock();

	return nRet;
}


std::pair<double, double> CVAT_MeasureStatus::GetVisionOffset(int nCam)
{
	m_csVisionOffset.Lock();

	std::pair<double, double> p;
	p.first = m_nVisionOffset[nCam][VAT_VISION_OFFSET_X];
	p.second = m_nVisionOffset[nCam][VAT_VISION_OFFSET_Y];

	m_csVisionOffset.Unlock();

	return p;
}

int CVAT_MeasureStatus::GetVisionMeasureCnt(int nCam)
{
	m_csVisionRetryCnt.Lock();

	int nCnt = 0;
	if (VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER) {
		nCnt = m_nCamRetryCnt[nCam];
	}

	m_csVisionRetryCnt.Unlock();

	return nCnt;
}

