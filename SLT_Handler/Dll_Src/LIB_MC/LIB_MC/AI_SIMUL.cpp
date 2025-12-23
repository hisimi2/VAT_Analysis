#include "StdAfx.h"
#include "AI_SIMUL.h"

using namespace std;

CAI_SIMUL::CAI_SIMUL(void)
{
}


CAI_SIMUL::~CAI_SIMUL(void)
{
}


void CAI_SIMUL::ReadAll(std::vector<double>& vBuffer)
{
	int nMaxPoint = g_pAioSimDrv->GetPointMax(_tAIO_LS::eAI);
	for(int i=0; i<nMaxPoint;i++){
		vBuffer[i] = g_pAioSimDrv->GetSimDataAi(i);
	}
}
