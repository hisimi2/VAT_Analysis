#include "StdAfx.h"
#include "DIO_SIMUL.h"
#include "SharedMem.h"
#include "DEF_COMMON.h"

////////////////////////////////////////////////////////////////
//
//			::CDI_SIMUL::
//
////////////////////////////////////////////////////////////////
CDI_SIMUL::CDI_SIMUL(int nPointCount)
{
	m_nDI_PointCount = nPointCount;
	m_pDI_PointBuffer = new BYTE [nPointCount];
	memset(m_pDI_PointBuffer, 0x00, nPointCount);

}


CDI_SIMUL::~CDI_SIMUL(void)
{
	if( m_pDI_PointBuffer ){
		delete [] m_pDI_PointBuffer;
		m_pDI_PointBuffer = NULL;
	}
}

void CDI_SIMUL::Read_DI()
{
	int nMaxPoint = g_pDioSimDrv->GetPointMax(_tDIO_LS::eDI);
	for(int i=0; i<nMaxPoint; i++){
		m_pDI_PointBuffer[i] = g_pDioSimDrv->GetSimDataDi(i);
	}
}



////////////////////////////////////////////////////////////////
//
//			::CDO_SIMUL::
//
////////////////////////////////////////////////////////////////
CDO_SIMUL::CDO_SIMUL(int nPointCount)
{
	m_nDO_PointCount = nPointCount;
	m_pDO_PointBuffer = new BYTE [nPointCount];
	memset(m_pDO_PointBuffer, 0x00, nPointCount);
}


CDO_SIMUL::~CDO_SIMUL(void)
{
	if( m_pDO_PointBuffer ){
		delete [] m_pDO_PointBuffer;
		m_pDO_PointBuffer = NULL;
	}
}

void CDO_SIMUL::Write_DO()
{
	int nMaxPoint = g_pDioSimDrv->GetPointMax(_tDIO_LS::eDO);
	for(int i=0; i<nMaxPoint; i++){
		g_pDioSimDrv->SetSimDataDo(i,m_pDO_PointBuffer[i]);
	}
}


////////////////////////////////////////////////////////////////
//
//			::CDIO_SIMUL_DRV::
//
////////////////////////////////////////////////////////////////
CDIO_SIMUL_DRV::CDIO_SIMUL_DRV()
{
	m_pSimMemDi = NULL;
	m_pSimMemDo = NULL;
	m_pDlgSimul = NULL;
	m_hSimulWnd = NULL;
	m_nMaxPointDo = 0;
	m_nMaxPointDi = 0;
}

CDIO_SIMUL_DRV::~CDIO_SIMUL_DRV()
{
	if( m_pDlgSimul ){
		m_pDlgSimul->DestroyWindow();
		delete m_pDlgSimul;
		m_pDlgSimul = NULL;
	}
}

int CDIO_SIMUL_DRV::DIO_INIT(const char* szIniFileName, int nDiPointMax, int nDoPointMax)
{
	BOOL bCreated = 0;
	char szFileName[MAX_PATH]={0,};
	sprintf_s(szFileName, sizeof(szFileName), "%s\\%s.map", SZ_DATA_MEM_PATH, TW_DI_SIM_MAP);
	bCreated = m_smDI.Create(szFileName, TW_DI_SIM_MAP, sizeof(_ST_DIO_MEM), _MODE_READWRITE);
	m_pSimMemDi = (BYTE*)m_smDI.GetPoint();

	memset(szFileName, 0x00, sizeof(szFileName));
	sprintf_s(szFileName, sizeof(szFileName), "%s\\%s.map", SZ_DATA_MEM_PATH, TW_DO_SIM_MAP);
	bCreated = m_smDO.Create(szFileName, TW_DO_SIM_MAP, sizeof(_ST_DIO_MEM), _MODE_READWRITE);
	m_pSimMemDo = (BYTE*)m_smDO.GetPoint();
	
	m_nMaxPointDi = nDiPointMax;
	m_nMaxPointDo = nDoPointMax;

	m_pDlgSimul = new CDlgDIOSimul;
	m_pDlgSimul->Create(IDD_DIO_SIMUL);
	m_pDlgSimul->ShowWindow(SW_HIDE);
	
	return 0;
}


BOOL CDIO_SIMUL_DRV::GetSimDataDi(int nIndex)
{
	if( nIndex < 0 || nIndex > m_nMaxPointDi ){
		return FALSE;
	}

	int nByt = nIndex / DIO_SIMUL_MAX_POINT_ONE_PORT;
	int nBit  = nIndex % DIO_SIMUL_MAX_POINT_ONE_PORT;
	BOOL bValue = GETBIT(m_pSimMemDi[nByt], nBit);
	return bValue;
}

void CDIO_SIMUL_DRV::SetSimDataDi(int nIndex, BOOL bOnOff)
{
	if( nIndex < 0 || nIndex > m_nMaxPointDi ){
		return;
	}
	int nByt = nIndex / DIO_SIMUL_MAX_POINT_ONE_PORT;
	int nBit  = nIndex % DIO_SIMUL_MAX_POINT_ONE_PORT;
	if( bOnOff ){
		SETBIT(m_pSimMemDi[nByt], nBit);
	}else{
		CLRBIT(m_pSimMemDi[nByt], nBit);
	}
}

BOOL CDIO_SIMUL_DRV::GetSimDataDo(int nIndex)
{
	if( nIndex < 0 || nIndex > m_nMaxPointDo ){
		return FALSE;
	}
	int nByt = nIndex / DIO_SIMUL_MAX_POINT_ONE_PORT;
	int nBit  = nIndex % DIO_SIMUL_MAX_POINT_ONE_PORT;
	BOOL bValue = GETBIT(m_pSimMemDo[nByt], nBit);
	return bValue;
}

void CDIO_SIMUL_DRV::SetSimDataDo(int nIndex, BOOL bOnOff)
{
	if( nIndex < 0 || nIndex > m_nMaxPointDo ){
		return;
	}
	int nByt = nIndex / DIO_SIMUL_MAX_POINT_ONE_PORT;
	int nBit  = nIndex % DIO_SIMUL_MAX_POINT_ONE_PORT;
	BOOL bCurVal = GETBIT(m_pSimMemDo[nByt], nBit);
	if( bCurVal != bOnOff )
	{
		if( bOnOff ){
			SETBIT(m_pSimMemDo[nByt], nBit);
		}else{
			CLRBIT(m_pSimMemDo[nByt], nBit);
		}
		//SendSimulMsg_DoChanged(nIndex, bOnOff);
	}
}

int CDIO_SIMUL_DRV::GetPointMax(int nInOrOut)
{
	if( nInOrOut == 0 ){
		return m_nMaxPointDi;
	}else{
		return m_nMaxPointDo;
	}
}

void CDIO_SIMUL_DRV::ShowDialogSimul()
{
	if( m_pDlgSimul ){
		m_pDlgSimul->ShowWindow(SW_SHOW);
	}
}

int CDIO_SIMUL_DRV::SendSimulMsg_DoChanged(int nIndex, BOOL bChangedValue)
{
	MSG message;
	if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}

	int nResult = 0;
	if( m_hSimulWnd != NULL ){
		try
		{
			nResult = ::PostMessage( m_hSimulWnd, UM_DIO_SIM_DO_CHANGED, ( WPARAM )nIndex, ( LPARAM )bChangedValue );
		}
		catch( const std::exception& e )
		{
			nResult = GetLastError();;
		}
		
	}
	return nResult;
}

int CDIO_SIMUL_DRV::SetSimulWnd(HWND hSimulWnd)
{
	m_hSimulWnd = hSimulWnd;
	return 0;
}
