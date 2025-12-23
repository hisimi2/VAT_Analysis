#include "StdAfx.h"
#include "AO_SIMUL.h"

using namespace std;


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-10]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CAO_SIMUL::CAO_SIMUL(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-10]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CAO_SIMUL::~CAO_SIMUL(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-10]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CAO_SIMUL::WriteAll(vector<double>& vBuff)
{
	int nPointMax = g_pAioSimDrv->GetPointMax(_tAIO_LS::eAO);
	for(int i=0; i<nPointMax; i++){
		g_pAioSimDrv->SetSimDataAo(i, vBuff[i]);
	}
}
