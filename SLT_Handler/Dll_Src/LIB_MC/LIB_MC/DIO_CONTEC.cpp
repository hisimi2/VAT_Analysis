#include "StdAfx.h"
#include "DIO_CONTEC.h"
#include "ConfigData.h"
#include "CDio.h"


CDIO_CONTEC_DRV* g_pContecDrv = NULL;

//////////////////////////////////////////////////////////////////////////
//
//  DI Techwing
//
//////////////////////////////////////////////////////////////////////////
CDI_CONTEC::CDI_CONTEC(int nBoardCount)
{
	m_nDI_BoardCount	= nBoardCount;
	m_nDI_PointCount	= CONTEC_DIO_MAX_PORT_ONE_MODULE * nBoardCount;
	m_nDI_PortCount		= CONTEC_DIO_MAX_POINT_ONE_PORT  * nBoardCount;

	m_pDI_PointBuffer       = new BYTE[m_nDI_PointCount];
	memset(m_pDI_PointBuffer, 0x00, m_nDI_PointCount);

	m_pDI_PortBuffer = new BYTE[m_nDI_PortCount];
	memset(m_pDI_PortBuffer, 0x00, sizeof(BYTE)*m_nDI_PortCount);
}

CDI_CONTEC::~CDI_CONTEC(void)
{
}

void CDI_CONTEC::read_DI()
{
	int nIndex   = 0;
	int nBitCnt  = 0;
	int nByteNo  = 0;
	int nBitNo   = 0;
	int nPortMax = 0;

	nPortMax = m_nDI_BoardCount * CONTEC_DIO_MAX_PORT_ONE_MODULE;

	for(int i = 0; i < nPortMax; i++)
	{
		g_pContecDrv->GetDI(i, (BYTE*)&m_pDI_PortBuffer[i]);
	}

	for (nIndex=0;nIndex<m_nDI_PointCount;nIndex++){
		nByteNo = nIndex / CONTEC_DIO_MAX_POINT_ONE_PORT;
		nBitNo  = nIndex % CONTEC_DIO_MAX_POINT_ONE_PORT;
		nBitCnt = 1;
		nBitCnt <<= nBitNo;
		if ((m_pDI_PortBuffer[nByteNo]&nBitCnt)==0)	m_pDI_PointBuffer[nIndex]= 0;
		else									    m_pDI_PointBuffer[nIndex]= 1;
	}
}



//////////////////////////////////////////////////////////////////////////
//
//  DI Techwing
//
//////////////////////////////////////////////////////////////////////////
CDO_CONTEC::CDO_CONTEC(int nBoardCount)
{
	int i = 0;

	m_nDO_BoardCount = nBoardCount;
	m_nDO_PointCount = CONTEC_DIO_MAX_POINT_ONE_MODULE * nBoardCount;
	m_nDO_PortCount  = CONTEC_DIO_MAX_PORT_ONE_MODULE  * nBoardCount;

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


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDO_CONTEC::~CDO_CONTEC(void)
{
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDO_CONTEC::Write_DO()
{
	int nIndex    = 0;
	int nBIT_CNT  = 0;
	int nBYTE_NUM = 0;
	int nBIT_NUM  = 0;

	int i=0, j=0, port_max=0;

	port_max = m_nDO_BoardCount * CONTEC_DIO_MAX_PORT_ONE_MODULE;

	for (nIndex=0;nIndex<m_nDO_PointCount;nIndex++)
	{
		nBIT_CNT  = 1;
		nBYTE_NUM = nIndex / CONTEC_DIO_MAX_POINT_ONE_PORT;
		nBIT_NUM  = nIndex % CONTEC_DIO_MAX_POINT_ONE_PORT;
		nBIT_CNT <<= nBIT_NUM;
		if (m_pDO_PointBuffer[nIndex] == 1 )
		{
			nBIT_CNT = ~nBIT_CNT;
			m_pDO_PortBuffer[nBYTE_NUM] &= nBIT_CNT;
		}
		else
		{
			m_pDO_PortBuffer[nBYTE_NUM] |= nBIT_CNT;
		}
	}

	for( i = 0; i < port_max; i++)
	{
		if( memcmp(&m_pDO_PortBuffer_Old[i], &m_pDO_PortBuffer[i], sizeof(BYTE)) == 0 )
		{
			continue;
		}
		g_pContecDrv->SetDO(i, m_pDO_PortBuffer[i]);
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
void CDO_CONTEC::Reset()
{
	int i=0;
	for(i=0; i<m_nDO_PointCount; i++){
		m_pDO_PointBuffer[i] = 0x00;
	}

	for(i=0; i<m_nDO_PortCount; i++){
		m_pDO_PortBuffer[i] = 0x00;
		m_pDO_PortBuffer_Old[i] = 0xFF;
	}
	Write_DO();
}



//////////////////////////////////////////////////////////////////////////
//
//  CONTEC Device Driver
//
//////////////////////////////////////////////////////////////////////////

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDIO_CONTEC_DRV::CDIO_CONTEC_DRV()
{
	memset(m_szInitFileName, 0x00, sizeof(m_szInitFileName));
	m_nNumOfModule = MAX_CONTEC_DEV;
	m_nMaxPortCout = MAX_PORT_COUNT;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDIO_CONTEC_DRV::~CDIO_CONTEC_DRV()
{

}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CDIO_CONTEC_DRV::Initialize(char* szIniFileName, int nFileNameLen)
{
	sprintf_s(m_szInitFileName, sizeof(m_szInitFileName), "%s", szIniFileName);

	CConfigData ConfigData(m_szInitFileName);

	m_nNumOfModule = ConfigData.GetInt("DIO CONFIG CONTEC", "Number Of Module", "2");

	m_nMaxPortCout = ConfigData.GetInt("DIO CONFIG CONTEC", "Max Port Count", "2");

	char szSec[MAX_PATH]={0,};
	char szTemp[512]={0,};
	int i=0;
	for(i=0; i<m_nNumOfModule; ++i)
	{
		memset(szSec, 0x00, sizeof(szSec));
		sprintf_s(szSec, sizeof(szSec), "DIO CONFIG CONTEC MODULE INFO - %d", i);

		// Device Name
		memset(szTemp, 0x00, sizeof(szTemp));
		sprintf_s(szTemp, sizeof(szTemp), "DIO CONFIG CONTEC DIO%03d", i);
		ConfigData.GetString(szSec, "Device Name", szTemp, m_aDevice[i].szDevName);
	}

	int nRet = 0;
	for(i=0; i<m_nNumOfModule; ++i)
	{
		nRet = DioInit(m_aDevice[i].szDevName, &m_aDevice[i].sId);
		if( nRet != DIO_ERR_SUCCESS )
		{
			char	szErrMsg[512]={0,};
			char	szError[256];		// Error String
			DioGetErrorString(nRet , szError);
			sprintf_s(szErrMsg, sizeof(szErrMsg), "CONTEC I/O initialize fail!!! \r\n[ERR=%d:]\r\n[%s]", nRet, szError);
			AfxMessageBox(szErrMsg);
			return -1;
		}
	}
	return 0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-03-23]
  NAME     : 
  PARAMS   : nPortNo : 0~Max Port
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CDIO_CONTEC_DRV::GetDI(unsigned short nPortNo, BYTE* pbyData)
{
	int   nRet = 0;
	ASSERT(m_nMaxPortCout!=0);
	int nIndex  = nPortNo / m_nMaxPortCout;
	short sPort = nPortNo % m_nMaxPortCout;
	BYTE  InpPortData=0;

	nRet = DioInpByte(m_aDevice[nIndex].sId, sPort, &InpPortData);
	if( nRet != DIO_ERR_SUCCESS )
	{
		char	szErrMsg[256];
		char	szError[256];		// Error String
		DioGetErrorString(nRet , szError);
		sprintf_s(szErrMsg, sizeof(szErrMsg), "CONTEC I/O read input fail!!! [ERR=%d:%s]", nRet, szError);
		// Write Log
	}

	*pbyData = InpPortData;
	return nRet;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CDIO_CONTEC_DRV::SetDO(unsigned short nPortNo, BYTE byData)
{
	int nRet = 0;
	ASSERT(m_nMaxPortCout!=0);
	int   nIndex = nPortNo / m_nMaxPortCout;
	short sPort  = nPortNo % m_nMaxPortCout;

	nRet = DioOutByte(m_aDevice[nIndex].sId, sPort, byData);
	if( nRet != DIO_ERR_SUCCESS )
	{
		char	szErrMsg[256];
		char	szError[256];		// Error String
		DioGetErrorString(nRet , szError);
		sprintf_s(szErrMsg, sizeof(szErrMsg), "CONTEC I/O write output fail!!! [ERR=%d:%s]", nRet, szError);
		// Write Log
	}
	return nRet;
}
