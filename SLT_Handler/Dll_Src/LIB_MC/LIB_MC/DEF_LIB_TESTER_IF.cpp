#include "stdafx.h"
#include "DEF_LIB_TESTER_IF.h"

void CTesterIFCmd::TransReciveData( CString strReciveData )
{
	if( strReciveData.Find( TESTER_CMD_TTOH_BINON ) == 0 )
	{
		this->strCmd = TESTER_CMD_TTOH_BINON;
		this->strRcvData = strReciveData;
	}

	if( strReciveData.Find( TESTER_CMD_TTOH_CURRENTALM ) == 0 )
	{
		this->strCmd = TESTER_CMD_TTOH_CURRENTALM;
		this->strRcvData = strReciveData;
	}
}