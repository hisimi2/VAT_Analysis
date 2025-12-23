#include "StdAfx.h"
#include "DelaySensor.h"


CDelaySensor::CDelaySensor()
{
	m_nDI_ID = 0;
	m_pIO = NULL;
	m_dStableTime = 3000.0;
	m_dStartTime = (double)GetTickCount64();
	m_nSensorState = -1;
	m_nSensorState_Pre = -1;
}

CDelaySensor::CDelaySensor(int nDI_ID, CDIO_Cont* pIO, double dStableTime)
	:m_nDI_ID(nDI_ID), m_pIO(pIO), m_dStableTime(dStableTime)
{
	m_dStartTime = (double)GetTickCount64();
	m_nSensorState = -1;
	m_nSensorState_Pre = -1;
}


CDelaySensor::~CDelaySensor(void)
{
}

int CDelaySensor::Initialize(int nDI_ID, CDIO_Cont* pIO, double dStableTime)
{
	m_nDI_ID= nDI_ID;
	m_pIO = pIO;
	m_dStableTime = dStableTime;
	m_dStartTime = (double)GetTickCount64();
	return 0;
}

void CDelaySensor::do_CheckStable()
{
	if( m_pIO == NULL ){
		return;
	}

	int nSensorState_Cur = m_pIO->in(m_nDI_ID);
	if (nSensorState_Cur != m_nSensorState_Pre) {
		m_dStartTime = (double)GetTickCount64();
	}

	double dSpanTime = (double)GetTickCount64() - m_dStartTime;
	if( dSpanTime > 60*60*1000 ){ // 오버 플로우가 되면 안정화 되지 않은 것으로 볼 수 있음.
		dSpanTime = 60*60*1000;
	}

	if (dSpanTime > m_dStableTime) {
		m_nSensorState = nSensorState_Cur;
	}else {
		m_nSensorState = CDelaySensor::eDelaySenSts_Stablizing;
	}

	m_nSensorState_Pre = nSensorState_Cur;
}
