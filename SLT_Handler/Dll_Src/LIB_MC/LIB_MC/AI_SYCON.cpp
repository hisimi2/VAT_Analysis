#include "StdAfx.h"
#include "AI_SYCON.h"

using namespace std;

CAI_SYCON::CAI_SYCON()
{
}


CAI_SYCON::~CAI_SYCON(void)
{
}


void CAI_SYCON::ReadAll(std::vector<double>& vBuffer)
{
	if( g_SyconDrv.IsConnected() ){
		g_SyconDrv.READ_AI_All();
		for(std::vector<double>::size_type i=0; i<vBuffer.size(); i++){
			double dValue = 0.0;
			g_SyconDrv.GET_AI(i, &dValue);
			vBuffer[i] = dValue;
		}
	}
}
