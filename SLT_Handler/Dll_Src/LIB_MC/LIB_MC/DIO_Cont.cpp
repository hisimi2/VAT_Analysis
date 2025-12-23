#include "StdAfx.h"
#include "DIO_Cont.h"
#include "DIO_Cont_Imp.h"
#include "DlgDioAll.h"


//==============================================================================
//
//==============================================================================
BYTE CDI_Cont::operator[]( int nIndex )
{
	ASSERT( nIndex >= 0 );

	return g_pDIOContImp->_in[ nIndex ];
}

BYTE CDI_Cont::operator()( int nIndex )
{
	return this->operator[]( nIndex );
}

//==============================================================================
//
//==============================================================================

BYTE CDO_Cont::operator()( int nIndex ) const
{
	return g_pDIOContImp->GET_DO( nIndex );
}

void CDO_Cont::operator()( int nIndex, int nOnOff )
{
	g_pDIOContImp->SET_DO( nIndex, nOnOff );
}



CDIO_Cont::CDIO_Cont(void)
{
}


CDIO_Cont::~CDIO_Cont(void)
{
}

int CDIO_Cont::Create(const char* szDriverInitFileName, int nFileNameLen)
{
	g_pDIOContImp = new CDIO_Cont_Imp;
	return g_pDIOContImp->Initialize(szDriverInitFileName, nFileNameLen);
}

void CDIO_Cont::Destroy()
{
	if( g_pDIOContImp != NULL ){
		g_pDIOContImp->Finalize();
		delete g_pDIOContImp;
		g_pDIOContImp = NULL;
	}
}

BOOL CDIO_Cont::IsInitBoard()
{
	return g_pDIOContImp->IsInitBoard();
}

void CDIO_Cont::_out(int nIndex, char cValue)
{
	ASSERT(nIndex>=0);
	g_pDIOContImp->SET_DO(nIndex, cValue);
}

char CDIO_Cont::_out(int nIndex)
{
	ASSERT(nIndex>=0);
	return g_pDIOContImp->GET_DO(nIndex);
}

void CDIO_Cont::RESET_DO()
{
	g_pDIOContImp->RESET_DO_DRV_CALL();
}

char CDIO_Cont::_in(int nIndex)
{
	ASSERT(nIndex>=0);
	return g_pDIOContImp->GET_DI(nIndex);
}

int CDIO_Cont::GetDiCount()
{ 
	return g_pDIOContImp->GetDIPointCount(); 
}

int CDIO_Cont::GetDoCount()
{
	return g_pDIOContImp->GetDOPointCount(); 
}

void CDIO_Cont::ShowDialog_IO_All()
{
	g_pDIOContImp->ShowDialog_IO_All();
}
void CDIO_Cont::ShowDialogSimul()
{
	g_pDIOContImp->ShowDialogSimul();
}
void CDIO_Cont::SetSimulDi(int nIndex, BOOL bValue)
{
	g_pDIOContImp->SetSimulDi(nIndex, bValue);
}
void CDIO_Cont::SetSimulWnd(CWnd* pSimulWnd)
{
	g_pDIOContImp->SetSimulWnd( pSimulWnd );
}

int CDIO_Cont::GetDriverType()
{
	return g_nDIO_Type;
}
