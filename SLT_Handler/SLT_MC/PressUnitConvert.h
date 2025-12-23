#pragma once
class CPressUnitConvert
{
public:
	CPressUnitConvert(int nDefaultUnit=CPressUnitConvert::MPa, double dValue=0.0);
	~CPressUnitConvert(void);

	double ConvertTo(int nUnit);

	enum ePressureUnit{
		MPa=0, KPa, bar, psi, KgFPcm2, Torr, eMaxPressureUnitCount		
	};


private:
	int m_nDefUnit;
	double m_dValue;
	double m_dPressCvtTable[eMaxPressureUnitCount][eMaxPressureUnitCount];

};



