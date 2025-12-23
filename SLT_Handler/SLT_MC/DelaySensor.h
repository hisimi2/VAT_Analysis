#pragma once

class CDelaySensor
{
public:
	enum eDelaySenSts{ eDelaySenSts_Stablizing = -1, eDelaySenSts_Off=0, eDelaySenSts_On };

public:
	CDelaySensor();
	CDelaySensor(int nDI_ID, CDIO_Cont* pIO, double dStableTime);
	~CDelaySensor(void);

	int Initialize(int nDI_ID, CDIO_Cont* pIO, double dStableTime);
	void do_CheckStable();
	int GetSensorState(){ return m_nSensorState; };

private:
	CDIO_Cont* m_pIO;
	int m_nDI_ID;
	double m_dStableTime; // msec
	double m_dStartTime;
	int m_nSensorState;
	int m_nSensorState_Pre;
};

