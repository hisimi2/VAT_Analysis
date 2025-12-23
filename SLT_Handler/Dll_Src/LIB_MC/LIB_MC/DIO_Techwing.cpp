#include "StdAfx.h"
#include "DIO_Techwing.h"
#include <conio.h>


//////////////////////////////////////////////////////////////////////////
//
//  DI Techwing
//
//////////////////////////////////////////////////////////////////////////
CDI_Techwing::CDI_Techwing(int nBaseAddress, int nDiBoardCount)
{
	m_nBaseAddress	= nBaseAddress;
	m_nDi_BoardCount= nDiBoardCount;
	m_nDI_PointCount= TW_DIO_MAX_POINT_ONE_BD * nDiBoardCount;
	m_nDI_PortCount = TW_DIO_MAX_PORT_ONE_BD  * nDiBoardCount;	// Board 한개에 16개의 Port 존재, Port 한개에 8 Point

	if( !m_nBaseAddress ){
		return;
	}

	m_pDI_PointBuffer       = new BYTE[m_nDI_PointCount];
	memset(m_pDI_PointBuffer, 0x00, sizeof(BYTE)*m_nDI_PointCount);

	m_pDI_PortBuffer = new BYTE[m_nDI_PortCount];						
	memset(m_pDI_PortBuffer, 0x00, sizeof(BYTE)*m_nDI_PortCount);
}

CDI_Techwing::~CDI_Techwing(void)
{
	if( m_pDI_PointBuffer != NULL ){
		delete [] m_pDI_PointBuffer;
		m_pDI_PointBuffer = NULL;
	}
	if( m_pDI_PortBuffer != NULL ){
		delete [] m_pDI_PortBuffer;
		m_pDI_PortBuffer = NULL;
	}
}


void CDI_Techwing::Read_DI()
{
	int indx     = 0;
	int bitcnt   = 0;
	int byteno   = 0;
	int bitno    = 0;
	int port_max = 0;

	port_max = m_nDi_BoardCount * TW_DIO_MAX_PORT_ONE_BD;

	for(int i = 0, j = 0; i < port_max; i++)
	{
		m_pDI_PortBuffer[ i ] = ( BYTE )PortIn( m_nBaseAddress + j );
		j++;
	}

	for (indx=0;indx<m_nDI_PointCount;indx++){
		byteno = indx / TW_DIO_MAX_POINT_ONE_PORT;
		bitno  = indx % TW_DIO_MAX_POINT_ONE_PORT;
		bitcnt = 1;
		bitcnt <<= bitno;
		if ((m_pDI_PortBuffer[byteno]&bitcnt)==0)	m_pDI_PointBuffer[indx]= 0;
		else										m_pDI_PointBuffer[indx]= 1;
	}
}



//////////////////////////////////////////////////////////////////////////
//
//  DO Techwing
//
//////////////////////////////////////////////////////////////////////////

CDO_Techwing::CDO_Techwing(int nBaseAddress, int nDoBoardCount)
{
	int i = 0;

	m_nBaseAddress   = nBaseAddress;
	m_nDo_BoardCount = nDoBoardCount;
	m_nDO_PointCount = TW_DIO_MAX_POINT_ONE_BD * nDoBoardCount;
	m_nDO_PortCount  = TW_DIO_MAX_PORT_ONE_BD  * nDoBoardCount;
	if(!m_nBaseAddress){
		return;
	}

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


CDO_Techwing::~CDO_Techwing(void)
{
	if( m_pDO_PointBuffer != NULL ){
		delete [] m_pDO_PointBuffer;
		m_pDO_PointBuffer = NULL;
	}

	if( m_pDO_PortBuffer != NULL ){
		delete [] m_pDO_PortBuffer;
		m_pDO_PortBuffer = NULL;
	}

	if( m_pDO_PortBuffer_Old != NULL ){
		delete [] m_pDO_PortBuffer_Old;
		m_pDO_PortBuffer_Old = NULL;
	}
}


void CDO_Techwing::Write_DO()
{
	int indx   = 0;
	int bitcnt = 0;
	int byteno = 0;
	int bitno  = 0;

	int i=0, j=0, port_max=0;

	port_max = m_nDo_BoardCount * TW_DIO_MAX_PORT_ONE_BD;

	for (indx=0;indx<m_nDO_PointCount;indx++)
	{
		bitcnt=1;
		byteno=indx / TW_DIO_MAX_POINT_ONE_PORT;
		bitno=indx  % TW_DIO_MAX_POINT_ONE_PORT;
		bitcnt <<= bitno;
		if (m_pDO_PointBuffer[indx] == 1 )
		{
			bitcnt = ~bitcnt;
			m_pDO_PortBuffer[byteno] &= bitcnt;
		}
		else{
			m_pDO_PortBuffer[byteno] |= bitcnt;
		}
	}

	for( i = 0; i < port_max; i++)
	{
// 		if( memcmp(&m_pDO_PortBuffer_Old[i], &m_pDO_PortBuffer[i], sizeof(BYTE)) == 0 )
// 		{
// 			continue;
// 		}
		PortOut(m_nBaseAddress+i, m_pDO_PortBuffer[i]);
		memcpy(&m_pDO_PortBuffer_Old[i], &m_pDO_PortBuffer[i], sizeof(BYTE));
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDO_Techwing::Reset()
{
	if(!m_nBaseAddress){
		return;
	}

	int i = 0;

	memset(m_pDO_PointBuffer, 0x00, sizeof(BYTE)*m_nDO_PointCount);

	// Board 한개의 16개 Port 존재
	for(i = 0; i < m_nDO_PortCount;i++){
		m_pDO_PortBuffer[i] = 0;
	}

	Write_DO();
}
