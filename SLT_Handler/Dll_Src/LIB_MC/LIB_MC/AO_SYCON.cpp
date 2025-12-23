#include "StdAfx.h"
#include "AO_SYCON.h"

using namespace std;


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-10]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CAO_SYCON::CAO_SYCON(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-10]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CAO_SYCON::~CAO_SYCON(void)
{
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-10]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CAO_SYCON::WriteAll(vector<double>& vBuff)
{
	if( g_SyconDrv.IsConnected() ){
		for(vector<double>::size_type i=0; i<vBuff.size(); i++){
			double dValue = 0.0;
			g_SyconDrv.SET_AO(i, vBuff[i]);
		}
		g_SyconDrv.WRITE_AO_All();
	}
}
