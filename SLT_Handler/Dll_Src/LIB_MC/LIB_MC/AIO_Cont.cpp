#include "StdAfx.h"
#include "AIO_Cont.h"
#include "DlgDioAll.h"

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CAIO_Cont::CAIO_Cont(void)
{
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CAIO_Cont::~CAIO_Cont(void)
{
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont::Initialize(const char* szDriverInitFileName, int nFileNameLen)
{
	g_pAioContImp = new CAIO_Cont_Imp;
	return g_pAioContImp->Initialize(szDriverInitFileName, nFileNameLen);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CAIO_Cont::Finalize()
{
	if( g_pAioContImp != NULL ){
		g_pAioContImp->Finalize();
		delete g_pAioContImp;
		g_pAioContImp = NULL;
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CAIO_Cont::IsInitBoard()
{
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CAIO_Cont::out(int nIndex, double dValue)
{

}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double CAIO_Cont::out(int nIndex)
{
	return 0.0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CAIO_Cont::RESET_AO()
{

}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double CAIO_Cont::in(int nIndex)
{
	return 0.0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont::GetAiCount()
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont::GetAoCount()
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CAIO_Cont::ShowDialog_IO_All()
{
	g_pAioContImp->ShowDialog_IO_All();	
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont::GetPhysicalValue(int nIoType, int nIndex, double* pdValue, CString& strUnit)
{
	return g_pAioContImp->GetPhysicalValue(nIoType, nIndex, pdValue, strUnit);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont::SetPhysicalValue(int nIndex, double dValue)
{
	return g_pAioContImp->SetPhysicalValue(nIndex, dValue);
}


void CAIO_Cont::ShowDialogSimul()
{
	g_pAioContImp->ShowDialogSimul();
}
