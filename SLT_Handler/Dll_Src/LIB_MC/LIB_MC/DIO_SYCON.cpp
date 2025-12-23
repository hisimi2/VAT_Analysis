#include "stdafx.h"
#include "DIO_SYCON.h"
#include "DEF_COMMON.h"

//////////////////////////////////////////////////////////////////////////
//
//  DI Sycon
//
//////////////////////////////////////////////////////////////////////////
CDI_SYCON::CDI_SYCON(int nPortCount)
{
	m_nDI_PortCount = nPortCount;
	m_nDI_PointCount = nPortCount * SYCON_DIO_MAX_POINT_ONE_PORT;

	m_pDI_PointBuffer = new BYTE[m_nDI_PointCount];
	memset(m_pDI_PointBuffer, 0x00, sizeof(BYTE)*m_nDI_PointCount);

	m_pDI_PortBuffer = new BYTE[m_nDI_PortCount];						
	memset(m_pDI_PortBuffer, 0x00, sizeof(BYTE)*m_nDI_PortCount);
}

CDI_SYCON::~CDI_SYCON( void )
{

}

void CDI_SYCON::Read_DI()
{
	if( g_SyconDrv.IsConnected() )
	{
		g_SyconDrv.READ_DI_All();
		int nData = 0;
		for(int i=0; i<m_nDI_PointCount; i++){
			g_SyconDrv.GET_DI(i, nData);
			m_pDI_PointBuffer[i] = (BYTE)nData;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//  DO Sycon
//
//////////////////////////////////////////////////////////////////////////
CDO_SYCON::CDO_SYCON(int nPortCount)
{
	int i = 0;

	m_nDO_PortCount  = nPortCount;
	m_nDO_PointCount = nPortCount * SYCON_DIO_MAX_POINT_ONE_PORT;

	m_pDO_PointBuffer		= new BYTE[m_nDO_PointCount];
	m_pDO_PortBuffer		= new BYTE[m_nDO_PortCount];
	m_pDO_PortBuffer_Old	= new BYTE[m_nDO_PortCount];

	for(i = 0; i < m_nDO_PortCount;i++){
		m_pDO_PortBuffer[i]     = 0;
		m_pDO_PortBuffer_Old[i] = 0xFF;
	}

	for(i = 0; i < m_nDO_PointCount; i++){
		m_pDO_PointBuffer[i] = 0;
	}
}

CDO_SYCON::~CDO_SYCON( void )
{

}

void CDO_SYCON::Write_DO()
{
	if( g_SyconDrv.IsConnected() )
	{
		for(int nBit=0; nBit<m_nDO_PointCount; nBit++){
			g_SyconDrv.SET_DO(nBit, m_pDO_PointBuffer[nBit]);
		}
		g_SyconDrv.WRITE_DO_All();
	}
}

void CDO_SYCON::Reset()
{
/*
	int i=0;
	for(i = 0; i < m_nDO_PortCount;i++){
		m_pDO_PortBuffer[i]     = 0;
		m_pDO_PortBuffer_Old[i] = 0xFF;
	}

	for(i = 0; i < m_nDO_PointCount; i++){
		m_pDO_PointBuffer[i] = 0;
	}
*/

	for(int nBit=0; nBit<m_nDO_PointCount; nBit++){
		g_SyconDrv.SET_DO(nBit, 0);
	}

	Write_DO();
}
