#include "StdAfx.h"
#include "Motion_Cont.h"


CMotion_Cont::CMotion_Cont(void)
{
}


CMotion_Cont::~CMotion_Cont(void)
{
}

int CMotion_Cont::GetAxisCount()
{
	return g_pMotion_Cont_Imp->GetAxisCount();
}

int CMotion_Cont::DB_Select_RobotTbl(CMyList& AxisInfoList)
{
	CDatabaseIF* pDbIf = g_SingleDB.GetObj();
	if( pDbIf ){
		pDbIf->SELECT_MOTORPARAM_TBL(AxisInfoList);
	}
	return 0;
}

OneAxis* CMotion_Cont::operator [](int nIndex)
{
	return g_pMotion_Cont_Imp->GetAxis(nIndex);
}

int CMotion_Cont:: Create(char* szIniFileName, int nFileLen)
{
	g_pMotion_Cont_Imp = new CMotion_Cont_Imp;
	int nCreateRespon = g_pMotion_Cont_Imp->Initialize(szIniFileName, nFileLen);

	return nCreateRespon;
}

void CMotion_Cont::Destroy()
{
	if( g_pMotion_Cont_Imp != NULL )
	{
		g_pMotion_Cont_Imp->Finalize();
		delete g_pMotion_Cont_Imp;
		g_pMotion_Cont_Imp = NULL;	
	}
}

void CMotion_Cont::ShowDialogSimul()
{
	g_pMotion_Cont_Imp->ShowDialogSimul();
}

int CMotion_Cont::GetDriverType()
{
	return g_pMotion_Cont_Imp->GetDriverType();
}

CBbxmnpClient* CMotion_Cont::GetbbxmnpClient()
{
	return &g_bbxmnpClient;
}
