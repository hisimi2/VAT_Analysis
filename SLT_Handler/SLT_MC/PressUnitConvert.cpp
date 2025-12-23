#include "StdAfx.h"
#include "PressUnitConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*
忙式式式式式式式式式成式式式式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式成式式式式式式式式式式成
弛         弛  MPa        弛 KPa      弛 bar      弛 psi      弛  kgf/cm2 弛  torr    弛 
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
弛 MPa     弛 1.0         弛  1000.0  弛 100.0    弛 145.04   弛 10.20    弛 7500.64  弛 
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
弛 KPa     弛 0.001       弛  1       弛 0.1      弛 0.14504  弛 0.01020  弛 7.50064  弛 
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
弛 bar     弛 0.1         弛 100      弛  1       弛 14.5038  弛 1.01972  弛 750.064  弛
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
弛 psi     弛 0.00689476  弛 6.89476  弛 0.06895  弛	  1      弛 0.07031  弛 51.7151  弛
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
弛 kgf/cm2 弛 0.0980665   弛 98.0665  弛 0.98067  弛 14.2233  弛  1       弛 735.561  弛
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
弛 torr    弛 0.00013332  弛 0.13332  弛 0.00133  弛 0.01934  弛 0.00136  弛 1        弛
戍式式式式式式式式式托式式式式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托式式式式式式式式式式托
*/
CPressUnitConvert::CPressUnitConvert(int nDefaultUnit/*=CPressUnitConvert::KPa*/, double dValue/*=0.0*/)
	:m_nDefUnit(nDefaultUnit), m_dValue(dValue)
{
	double dPressCvtTable[eMaxPressureUnitCount][eMaxPressureUnitCount] = {
	{1.0         ,1000.0    , 100.0    , 145.04   , 010.20   , 7500.64	},
	{0.001       ,   1.0    , 0.1      , 0.14504  , 0.01020  , 7.50064	},
	{0.1         , 100.0    , 1.0      , 14.5038  , 1.01972  , 750.064  },
	{ 0.00689476 , 6.89476  , 0.06895  ,	1.0   , 0.07031  , 51.7151  },
	{ 0.0980665  , 98.0665  , 0.98067  , 14.2233  ,  1.0     , 735.561  },
	{ 0.00013332 , 0.13332  , 0.00133  , 0.01934  , 0.00136  , 1.0      }};

	memcpy(m_dPressCvtTable, dPressCvtTable, sizeof(m_dPressCvtTable));
}

CPressUnitConvert::~CPressUnitConvert(void)
{
}

double CPressUnitConvert::ConvertTo(int nUnit)
{
	double dRetValue = 0.0;

	ASSERT( nUnit >= CPressUnitConvert::MPa && nUnit < CPressUnitConvert::eMaxPressureUnitCount );

	dRetValue = m_dValue * m_dPressCvtTable[m_nDefUnit][nUnit];

	return dRetValue;
}


