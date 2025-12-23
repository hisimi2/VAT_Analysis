#include "StdAfx.h"
#include "LotInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLotInfo::CLotInfo(void)
{
	strLotNo="";
	strScheduleNo="";
	strOperID="";
	strDeviceName="";
	strProgName="";
	strCustomer="";
	nLotStartMode=0;
	nRCMode=0;
	nVisionUse=0;
	nLotState=0;
}

CLotInfo::CLotInfo(const CLotInfo& src)
{
	strLotNo      = src.strLotNo      ;
	strScheduleNo = src.strScheduleNo ;
	strOperID     = src.strOperID     ;
	strDeviceName = src.strDeviceName ;
	strProgName   = src.strProgName   ;
	strCustomer   = src.strCustomer   ;
	nLotStartMode = src.nLotStartMode ;
	nRCMode       = src.nRCMode       ;
	nVisionUse    = src.nVisionUse    ;
	nLotState     = src.nLotState     ;
}


CLotInfo::~CLotInfo(void)
{
}

CLotInfo& CLotInfo::operator=(const CLotInfo& src)
{
	if( this == &src )
	{
		return *this;
	}

	strLotNo      = src.strLotNo      ;
	strScheduleNo = src.strScheduleNo ;
	strOperID     = src.strOperID     ;
	strDeviceName = src.strDeviceName ;
	strProgName   = src.strProgName   ;
	strCustomer   = src.strCustomer   ;
	nLotStartMode = src.nLotStartMode ;
	nRCMode       = src.nRCMode       ;
	nVisionUse    = src.nVisionUse    ;
	nLotState     = src.nLotState     ;

	return *this;
}