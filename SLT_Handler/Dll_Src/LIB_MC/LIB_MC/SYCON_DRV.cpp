#include "StdAfx.h"
#include "SYCON_DRV.h"
#include "ConfigData.h"
#include "ParamList.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
//  SYCON DRIVER
//
//////////////////////////////////////////////////////////////////////////
CSYCON_DRV::CSYCON_DRV()
{
	m_nDI_BlockSize     = 0;
	m_nDO_BlockSize     = 0;
	m_nAI_BlockSize     = 0;
	m_nAO_BlockSize     = 0;

	m_hCifxDriver		= NULL;
	m_hChannel			= NULL;
	m_hSysDevice        = NULL;
	m_bConnected		= FALSE;

	m_pDI_Buffer        = NULL;
	m_pDO_Buffer	    = NULL;
	m_pAI_Buffer        = NULL;
	m_pAO_Buffer	    = NULL;
	m_nDioPointMax[0]   = 0;
	m_nDioPointMax[1]   = 0;
	m_nAioPointMax[0]   = 0;
	m_nAioPointMax[1]   = 0;

	/* Ethercat log param list */
	m_ptApp = NULL;
	m_tResult = TLR_S_OK;
	memset(&m_tSendPkt, NULL, sizeof(CIFX_PACKET));
	memset(&m_tRecvPkt, NULL, sizeof(CIFX_PACKET));	 
	m_ptRecvPkt = NULL;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CSYCON_DRV::~CSYCON_DRV()
{
	Finalize();

	if(m_pDI_Buffer){
		delete[]	m_pDI_Buffer;
		m_pDI_Buffer = NULL;
	}

	if(m_pDO_Buffer){
		delete[]	m_pDO_Buffer;
		m_pDO_Buffer = NULL;
	}
	if(m_pAI_Buffer){
		delete[]	m_pAI_Buffer;
		m_pAI_Buffer = NULL;
	}

	if(m_pAO_Buffer){
		delete[]	m_pAO_Buffer;
		m_pAO_Buffer = NULL;
	}

	if(m_ptApp) {
		App_FreeResources(m_ptApp);
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CSYCON_DRV::OpenDriver()
{
	unsigned int	unActState	= 0;
	unsigned int	unState		= 0;

	//////////////////////////////////////////////////////////////////////////
	//Master 및 Slave Status 를 확인하기 위한 Data packet 및 Buffer를 초기화 한다..
	m_tResult = App_CreateResources(&m_ptApp);
	if (m_tResult != TLR_S_OK)
	{
		//통신은 정상 연결되었는데 할당이 되지 않는 경우 return을 시키는것은 논리적이지 못하다.
		//일단은 리턴을 시키자..		
		App_FreeResources(m_ptApp);
		return -1;  //메모리 할당이 되지 않거나, 잘못 되는 경우..return
	}
	//////////////////////////////////////////////////////////////////////////

	//	Open Driver
	LONG lRet = xDriverOpen(&m_hCifxDriver);
	if(lRet != CIFX_NO_ERROR || m_hCifxDriver == NULL)
	{
		return -1;
	}

	m_ptApp->tCommon.hDriver = m_hCifxDriver; 

	CHAR szName[64]={0,};
	sprintf_s(szName, sizeof(szName), "Cifx0");

	lRet = xChannelOpen(m_hCifxDriver, szName, 0 , &m_hChannel);
	if(lRet != CIFX_NO_ERROR || m_hChannel == NULL)
	{
		return -2;
	}

	m_ptApp->tCommon.hChannel = m_hChannel; 

	lRet = xSysdeviceOpen(m_hCifxDriver, szName, &m_hSysDevice);
	if(lRet != CIFX_NO_ERROR || m_hSysDevice == NULL)
	{
		return -7;
	}

	//CIFX50-RE 의 경우 DriveClose할때 모든 IO OFF 되나 다시 DriverOpen하게 되면 이전 I/O 다시 설정됨.
	//ChannelReset을 할 경우 DriverOpen해도 모든 IO OFF로 설정됨
	lRet = xChannelReset(m_hChannel, CIFX_CHANNELINIT, 10000);
	if(lRet != CIFX_NO_ERROR)
	{
		return -3;
	}

	lRet = xChannelHostState(m_hChannel, CIFX_HOST_STATE_READ, &unActState, 100);
	//  Set HostState
	if(lRet != CIFX_NO_ERROR)
	{
		return -4;
	}

	// Stop Communication
	lRet = xChannelBusState(m_hChannel, CIFX_BUS_STATE_OFF, &unState, 2000);
	if(lRet != CIFX_NO_ERROR)
	{
		return -5;
	}

	// Stop 후 일정 시간 Delay 준다...
	Sleep(500); // test 결과 5초..

	lRet = xChannelBusState(m_hChannel, CIFX_BUS_STATE_ON, &unState, 10000);
	if(lRet != CIFX_NO_ERROR)
	{
		return -6;
	}

	//////////////////////////////////////////////////////////////////////////
	/* 어플리케이션 초기화 */
	m_tResult = App_Initialize(m_ptApp);
	if (CIFX_NO_ERROR == m_tResult){
		App_GetHandles(m_ptApp);
	}	
	//////////////////////////////////////////////////////////////////////////

	m_bConnected = TRUE;

	return 0;
}

void CSYCON_DRV::CloseDriver()
{
	// Close BusStage
	if( m_hChannel != NULL )
	{
		m_bConnected = FALSE;

		//Open시와는 다르게 Close일 때는 역순으로 변경 된다. 
		LONG lRet = xChannelReset(m_hChannel, CIFX_CHANNELINIT, 10000);
		if(lRet != CIFX_NO_ERROR)
		{
			return;
		}
		unsigned int unState	= 0;
		lRet = xChannelBusState(m_hChannel, CIFX_BUS_STATE_OFF, &unState , 10000);
		if(lRet != CIFX_NO_ERROR)
		{
			return;
		}
		//  Close HostState
		unsigned int unActState = 0;
		lRet = xChannelHostState(m_hChannel, CIFX_HOST_STATE_NOT_READY, &unActState, 10000);
		if(lRet != CIFX_NO_ERROR)
		{
			return;
		}

		//	Close Channel
		lRet = xChannelClose(m_hChannel);

		//	Close driver
		lRet = xDriverClose( m_hCifxDriver);

		m_hCifxDriver = NULL;
		m_hChannel = NULL;

		Sleep(3000);
	}
}

int CSYCON_DRV::InitialDriver(char* szIniFileName, int nFileNameLen)
{
	int nDriverOpened = OpenDriver();
	if( nDriverOpened != 0){
		return nDriverOpened;
	}

	if( m_pDI_Buffer != NULL ){
		delete [] m_pDI_Buffer;
		m_pDI_Buffer = NULL;
	}
	if( m_pDO_Buffer != NULL ){
		delete [] m_pDO_Buffer;
		m_pDO_Buffer = NULL;
	}
	if( m_pAI_Buffer != NULL ){
		delete [] m_pAI_Buffer;
		m_pAI_Buffer = NULL;
	}
	if( m_pAO_Buffer != NULL ){
		delete [] m_pAO_Buffer;
		m_pAO_Buffer = NULL;
	}

	CConfigData ConfigData(szIniFileName);

	char szDefault[512];
	memset(szDefault, 0x00, sizeof(szDefault));
	sprintf_s(szDefault, sizeof(szDefault), "HILSCHER_CIF.log");
	char szBuff[512]={0,};
	ConfigData.GetString("HILSCHER SYCON IO CONFIG", "Log File Name", szDefault, szBuff);
	char szLogFileName[512]={0,};
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);

	// Input
	int nInputMemBlockCount = ConfigData.GetInt("HILSCHER SYCON IO CONFIG", "Input Memory Block Count", "0");
	for(int i=0; i<nInputMemBlockCount; i++){
		char szValue[128]={0,};
		char szKey[128]={0,};
		sprintf_s(szKey, sizeof(szKey), "Input Memory Block %d", i+1);
		// "DIO" or "AIO", offset, block size
		ConfigData.GetString("HILSCHER SYCON IO CONFIG", szKey, "DIO,0,0", szValue);

		CParamList PrmList;
		PrmList.m_fnSetDelimiter(",", 1);
		PrmList.m_fnSetDelimitedText( szValue, strlen(szValue) );

		ST_SYCON_MEM_BLOCK SyconMemBlock;
		char szTemp[128] = {0,};
		PrmList.m_fnGetItemString(0, szTemp);
		if( strcmp(szTemp, "DIO") == 0 ){
			SyconMemBlock.nType = CSYCON_DRV::eDIO;
		}else{
			SyconMemBlock.nType = CSYCON_DRV::eAIO;
		}
		SyconMemBlock.nPhyOffset = PrmList.m_fnGetItemInt(1);
		SyconMemBlock.nBlockSize = PrmList.m_fnGetItemInt(2);

		m_vInMemBlock.push_back(SyconMemBlock);

		// 중간에 비어 있는 것이 있어서는 안됨.
		if( SyconMemBlock.nType == CSYCON_DRV::eDIO ){
			m_nDI_BlockSize += SyconMemBlock.nBlockSize; 
		}else{
			m_nAI_BlockSize += SyconMemBlock.nBlockSize;
		}
	}

	if( m_nDI_BlockSize > 0 ){
		m_pDI_Buffer	= new BYTE[ m_nDI_BlockSize ];
		memset(m_pDI_Buffer, 0x00, m_nDI_BlockSize);
		m_nDioPointMax[0] = m_nDI_BlockSize * SYCON_DIO_MAX_POINT_ONE_PORT;
	}

	if( m_nAI_BlockSize > 0 ){
		m_pAI_Buffer	= new BYTE[ m_nAI_BlockSize ];
		memset(m_pAI_Buffer, 0x00, m_nAI_BlockSize);
		m_nAioPointMax[0] = m_nAI_BlockSize / SYCON_AIO_SIZE_PER_POINT;
	}

	// Output
	int nOutputMemBlockCount = ConfigData.GetInt("HILSCHER SYCON IO CONFIG", "Output Memory Block Count", "0");
	for(int i=0; i<nOutputMemBlockCount; i++){
		char szValue[128]={0,};
		char szKey[128]={0,};
		sprintf_s(szKey, sizeof(szKey), "Output Memory Block %d", i+1);
		// "DIO" or "AIO", offset, block size
		ConfigData.GetString("HILSCHER SYCON IO CONFIG", szKey, "DIO,0,0", szValue);

		CParamList PrmList;
		PrmList.m_fnSetDelimiter(",", 1);
		PrmList.m_fnSetDelimitedText( szValue, strlen(szValue) );

		ST_SYCON_MEM_BLOCK SyconMemBlock;
		char szTemp[128] = {0,};
		PrmList.m_fnGetItemString(0, szTemp);
		if( strcmp(szTemp, "DIO") == 0 ){
			SyconMemBlock.nType = CSYCON_DRV::eDIO;
		}else{
			SyconMemBlock.nType = CSYCON_DRV::eAIO;
		}
		SyconMemBlock.nPhyOffset = PrmList.m_fnGetItemInt(1);
		SyconMemBlock.nBlockSize = PrmList.m_fnGetItemInt(2);

		m_vOutMemBlock.push_back(SyconMemBlock);

		// 중간에 비어 있는 것이 있어서는 안됨.
		if( SyconMemBlock.nType == CSYCON_DRV::eDIO ){
			m_nDO_BlockSize += SyconMemBlock.nBlockSize; 
		}else{
			m_nAO_BlockSize += SyconMemBlock.nBlockSize; 
		}
	}

	if( m_nDO_BlockSize > 0 ){
		m_pDO_Buffer	= new BYTE[ m_nDO_BlockSize ];
		memset(m_pDO_Buffer, 0x00, m_nDO_BlockSize);
		m_nDioPointMax[1] = m_nDO_BlockSize * SYCON_DIO_MAX_POINT_ONE_PORT;
	}

	if( m_nAO_BlockSize > 0 ){
		m_pAO_Buffer	= new BYTE[ m_nAO_BlockSize ];
		memset(m_pAO_Buffer, 0x00, m_nAO_BlockSize);	
		m_nAioPointMax[1] = m_nAO_BlockSize / SYCON_AIO_SIZE_PER_POINT;
	}

	m_bConnected = TRUE;
	
	return 1;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CSYCON_DRV::Finalize()
{
	CloseDriver();
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CSYCON_DRV::ReadChannelIO(int nAreaNum, int nOffset, BYTE* pBuffer, int nCount)
{
	if(m_bConnected)
	{
		//	read input data only
		LONG lRet = xChannelIORead(	m_hChannel,			// Channel
									nAreaNum,		    // Area Number
									nOffset,		    // Offset
									nCount,			    // DataLen
									pBuffer,		    // Data Pointer
									100L);				// Timeout

		if( lRet != CIFX_NO_ERROR )
		{
			// TODO STRIP : Log
			m_Logger.LOG("xChannelIORead error. [ERROR CODE:%x]", lRet);
			//////////////////////////////////////////////////////////////////////////
			App_GetHandles(m_ptApp);
			// acyclic 프로세스 진행
			m_tResult = xChannelGetPacket(m_hChannel, sizeof(m_tRecvPkt), &m_tRecvPkt, 5);

			// 패킷이나 에러 코드의 receive 여부 체크
			if(TLR_S_OK == m_tResult)
			{
				// 패킷 제어
				App_HandlePacket(m_ptApp, &m_tRecvPkt);
			}
			// 특정 상태에서 slave의 갯수가 변하는지 체크 및 출력 
			App_GetSlaveState(m_ptApp, &m_tSendPkt, &m_tRecvPkt);		
			//////////////////////////////////////////////////////////////////////////

			CloseDriver();
			OpenDriver();

			m_tResult = xChannelGetPacket(m_hChannel, sizeof(m_tRecvPkt), &m_tRecvPkt, 5);
			// 패킷이나 에러 코드의 receive 여부 체크
			if(TLR_S_OK == m_tResult)
			{
				// 패킷 제어
				App_HandlePacket(m_ptApp, &m_tRecvPkt);
			}
			// 특정 상태에서 slave의 갯수가 변하는지 체크 및 출력 
			App_GetSlaveState(m_ptApp, &m_tSendPkt, &m_tRecvPkt);
		}

		// 네트워크 통신 불량에 대한 에러 처리
		CHANNEL_INFORMATION stChannelInfo;
		lRet = xChannelInfo( m_hChannel, sizeof( CHANNEL_INFORMATION ), &stChannelInfo );
		if( CIFX_NO_ERROR != stChannelInfo.ulChannelError){
			m_Logger.LOG("xChannelInfo. [ERROR CODE:%x] [CHANNEL ERROR:%x]", lRet, stChannelInfo.ulChannelError);
			//////////////////////////////////////////////////////////////////////////
			App_GetHandles(m_ptApp);
			//Slave Status 체크 부분..
			m_tResult = xChannelGetPacket(m_hChannel, sizeof(m_tRecvPkt), &m_tRecvPkt, 5);

			// 패킷이나 에러 코드의 receive 여부 체크
			if(TLR_S_OK == m_tResult)
			{
				//패킷 제어
				App_HandlePacket(m_ptApp, &m_tRecvPkt);
			}
			//특정 상태에서 slave의 갯수가 변하는지 체크 및 출력
			App_GetSlaveState(m_ptApp, &m_tSendPkt, &m_tRecvPkt);		
			//////////////////////////////////////////////////////////////////////////

			CloseDriver();
			OpenDriver();
		}
	}
	return 1;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 

==============================================================================*/
int CSYCON_DRV::WriteChannelIO(int nAreaNum, int nOffset, BYTE* pBuffer, int nCount)
{
	if(m_bConnected)
	{
		LONG lRet = xChannelIOWrite(m_hChannel,			// Channel
									nAreaNum,			// Area Number
									nOffset,			// Offset
									nCount,			    // Data Len
									pBuffer,		    // Buffer Pointer
									100L);				// Timeout

		if( lRet != CIFX_NO_ERROR )
		{
			m_Logger.LOG("xChannelIOWrite error. [ERROR CODE:%x]", lRet);
			CloseDriver();
			OpenDriver();
			return -1;
		}
	}
	return 1;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CSYCON_DRV::READ_DI_All()
{
	int nReadSize = 0;
	for(std::vector<int>::size_type i=0; i<m_vInMemBlock.size(); ++i){
		if( m_vInMemBlock[i].nType == CSYCON_DRV::eDIO ){
			ReadChannelIO(0, m_vInMemBlock[i].nPhyOffset, m_pDI_Buffer+nReadSize, m_vInMemBlock[i].nBlockSize);
			nReadSize += m_vInMemBlock[i].nBlockSize;
		}
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CSYCON_DRV::WRITE_DO_All()
{
	int nWriteSize = 0;
	for(std::vector<int>::size_type i=0; i<m_vOutMemBlock.size(); ++i){
		if( m_vOutMemBlock[i].nType == CSYCON_DRV::eDIO ){
			WriteChannelIO(0, m_vOutMemBlock[i].nPhyOffset, m_pDO_Buffer+nWriteSize, m_vOutMemBlock[i].nBlockSize);
			nWriteSize += m_vOutMemBlock[i].nBlockSize;
		}
	}
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSYCON_DRV::GET_DI(INT nBitNum, INT& nData)
{
	if(m_pDI_Buffer == NULL)
		return FALSE;

	INT nIdx = nBitNum / 8;
	INT nPos = nBitNum % 8;

	if(nIdx < 0 || nIdx >= m_nDI_BlockSize)
		return FALSE;

	nData =  GETBIT(m_pDI_Buffer[nIdx], nPos);

	return TRUE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSYCON_DRV::GET_DO(INT nBitNum, INT& nData)
{
	if(m_pDO_Buffer == NULL)
		return FALSE;

	INT nIdx = nBitNum / 8;
	INT nPos = nBitNum % 8;

	if(nIdx < 0 || nIdx >= m_nDO_BlockSize)
		return FALSE;

	nData =  GETBIT(m_pDO_Buffer[nIdx], nPos);;

	return TRUE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSYCON_DRV::SET_DO(INT nBitNum, INT nData)
{
	if(m_pDO_Buffer == NULL)
		return FALSE;

	INT nIdx = nBitNum / 8;
	INT nPos = nBitNum % 8;

	if(nIdx < 0 || nIdx >= m_nDO_BlockSize)
		return FALSE;

	if(nData > 0)
		SETBIT(m_pDO_Buffer[nIdx], nPos);
	else
		CLRBIT(m_pDO_Buffer[nIdx], nPos);

	return TRUE;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CSYCON_DRV::READ_AI_All()
{
	int nReadSize = 0;
	for(std::vector<int>::size_type i=0; i<m_vInMemBlock.size(); ++i){
		if( m_vInMemBlock[i].nType == CSYCON_DRV::eAIO ){
			ReadChannelIO(0, m_vInMemBlock[i].nPhyOffset, m_pAI_Buffer+nReadSize, m_vInMemBlock[i].nBlockSize);			
			nReadSize += m_vInMemBlock[i].nBlockSize;
		}
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CSYCON_DRV::WRITE_AO_All()
{
	int nWriteSize = 0;
	for(std::vector<int>::size_type i=0; i<m_vOutMemBlock.size(); ++i){
		if( m_vOutMemBlock[i].nType == CSYCON_DRV::eAIO ){
			WriteChannelIO(0, m_vOutMemBlock[i].nPhyOffset, m_pAO_Buffer+nWriteSize, m_vOutMemBlock[i].nBlockSize);
			nWriteSize += m_vOutMemBlock[i].nBlockSize;
		}
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : [IN ] nIndex  : Analog 1 ch에 대한 인덱스 (1 채널 당 4바이트)
             [OUT] pdValue : Analog 값을 반환 한다. voltage (0~10v=0~3FFF)
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSYCON_DRV::GET_AI(int nIndex, double* pdValue)
{
	ASSERT( (nIndex >= 0) && (nIndex < (m_nAI_BlockSize / (int)sizeof(WORD))) );

	WORD nTemp = *(WORD*) &m_pAI_Buffer[nIndex * sizeof(WORD)];

	//if( nTemp < 0 ) nTemp = 0;
	if( nTemp > SYCON_AIO_CONVERT_RES ) nTemp = SYCON_AIO_CONVERT_RES;

/*
	WORD nValue = (nTemp & 0x00FF);
	nValue <<= 8;
	nTemp >>= 8;
	nValue |= nTemp;

	*pdValue = (double)((double)SYCON_AIO_CONVERT_MAX_VOLT_INPUT*((double)nValue/(double)SYCON_AIO_CONVERT_RES));
*/

	*pdValue = (double)((double)SYCON_AIO_CONVERT_MAX_VOLT_INPUT*((double)nTemp/(double)SYCON_AIO_CONVERT_RES));
	return TRUE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSYCON_DRV::GET_AO(int nIndex, double* pdValue)
{
	ASSERT( (nIndex >= 0) && (nIndex < (m_nAO_BlockSize / (int)sizeof(WORD))));

	
	WORD nTemp = *(WORD*) &m_pAO_Buffer[nIndex * sizeof(WORD)];

	//if( nTemp < 0 ) nTemp = 0;
	if( nTemp > SYCON_AIO_CONVERT_RES ) nTemp = SYCON_AIO_CONVERT_RES;

/*
 	WORD nValue = (nTemp & 0x00FF);
 	nValue <<= 8;
 	nTemp >>= 8;
 	nValue |= nTemp;
	
	*pdValue = (double)((double)SYCON_AIO_CONVERT_MAX_VOLT_OUTPUT*((double)nValue/(double)SYCON_AIO_CONVERT_RES));
*/
	*pdValue = (double)((double)SYCON_AIO_CONVERT_MAX_VOLT_OUTPUT*((double)nTemp/(double)SYCON_AIO_CONVERT_RES));

	return TRUE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-08]
  NAME     : 
  PARAMS   : dValue : 셋팅 하고자 하는 전압. [v]
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CSYCON_DRV::SET_AO(int nIndex, double   dValue)
{
	ASSERT( (nIndex >= 0) && (nIndex < (m_nAO_BlockSize / (int)sizeof(WORD))));

	int nValue = (int)((dValue/SYCON_AIO_CONVERT_MAX_VOLT_OUTPUT)*SYCON_AIO_CONVERT_RES);
	
	if( nValue < 0 ) nValue = 0;
	if( nValue > SYCON_AIO_CONVERT_RES ) nValue = SYCON_AIO_CONVERT_RES;

	*(WORD*) &m_pAO_Buffer[nIndex * sizeof(WORD)] = nValue;
/*
	BYTE* pMem = (BYTE*) &m_pAO_Buffer[nIndex * sizeof(WORD)];
	*(pMem+1)   = nValue & 0x00FF;
	*(pMem)     = (nValue & 0xFF00) >> 8;
*/

	return TRUE;
}

TLR_RESULT CSYCON_DRV::App_CreateResources( APPLICATION_T** pptApp )
{
	//TRACE("Allocating resources...\n");

	/* 함수 반환 값 */
	TLR_RESULT tResult      = TLR_S_OK;

	/* 함 수 마지막에 포인터 반환 예정 */
	*pptApp = NULL;

	/* 어플리케이션 데이터를 위한 메모리 할당 */
	APPLICATION_T* ptApp = (APPLICATION_T*)malloc(sizeof(APPLICATION_T));
	memset(ptApp, 0, sizeof(APPLICATION_T));

	/* read / write 버퍼를 위한 메모리 할당 */
	ptApp->tCommon.ulReadBufferSize  = 8 * sizeof(TLR_UINT8);
	ptApp->tCommon.ulWriteBufferSize = 8 * sizeof(TLR_UINT8);

	ptApp->tCommon.pabReadBuffer  =  (TLR_UINT8*) malloc(ptApp->tCommon.ulReadBufferSize);
	ptApp->tCommon.pabWriteBuffer =  (TLR_UINT8*) malloc(ptApp->tCommon.ulWriteBufferSize);

	/* read / write 버퍼를 "0"으로 초기화 */
	memset(ptApp->tCommon.pabReadBuffer,  0, ptApp->tCommon.ulReadBufferSize);
	memset(ptApp->tCommon.pabWriteBuffer, 0, ptApp->tCommon.ulWriteBufferSize);

	/* 포인터 반환 */
	*pptApp = ptApp;

	//TRACE("Successful.\n");
	return tResult;
}

TLR_RESULT CSYCON_DRV::App_FreeResources( APPLICATION_T* ptApp )
{
	//TRACE("Free resources...\n");

	/* 함수의 반환 값 */
	TLR_RESULT tResult = TLR_S_OK;

	/* 버퍼 리소스 반환 */
	free(ptApp->tCommon.pabReadBuffer);
	free(ptApp->tCommon.pabWriteBuffer);

	/* 어플리케이션 데이터 컨테이너 반환 */
	free(ptApp);

	//ptApp = NULL;
	//TRACE("Successful.\n");
	return tResult;
}

TLR_RESULT CSYCON_DRV::App_SendRecvPkt( APPLICATION_T* ptApp, CIFX_PACKET* ptSendPkt, CIFX_PACKET* ptRecvPkt )
{
	TLR_RESULT tResult = TLR_S_OK; 

	/* 패킷 보냄 */
	tResult = xChannelPutPacket(ptApp->tCommon.hChannel, ptSendPkt, ptApp->tCommon.ulTimeout);
	if (TLR_S_OK != tResult)
	{
		return tResult;
	}

	/* ok, 패킷 전송 성공 */

	/* 받을 패킷 확인 */
	/* the ulCnfCmd is always: ulReqCmd | 0x01 */
	TLR_UINT32 ulCnfCmd = ptSendPkt->tHeader.ulCmd | 0x01;  

	while(CIFX_NO_ERROR == (tResult = xChannelGetPacket(ptApp->tCommon.hChannel, sizeof(*ptRecvPkt), ptRecvPkt, ptApp->tCommon.ulTimeout)))
	{
		/* 패킷 체크 */
		if(ptRecvPkt->tHeader.ulCmd == ulCnfCmd)
		{
			/* 해당 패킷이 맞는 경우 자체 상태를 결과로 리턴 */
			tResult = ptRecvPkt->tHeader.ulState;

			/* Note: 받은 패킷 리턴 (by reference, see signature of function) */

			/* 패킷 받았으므로 여기서 break */
			break;
		}
		else
		{
			/* it is something else, so place it in the application packet handler */
			App_HandlePacket(ptApp, ptRecvPkt);
		}
	}

	return tResult;
}

TLR_RESULT CSYCON_DRV::App_SendRecvEmptyPkt( APPLICATION_T* ptApp, TLR_UINT32 ulCmd )
{
	CIFX_PACKET tSendPkt = {{0}};
	CIFX_PACKET tRecvPkt = {{0}};

	TLR_RESULT tResult = TLR_S_OK;

	/* 모든 등록된 패킷은 empty 패킷으로 데이터가 없음을 의미하기 때문에 특정 타입으로의 캐스트 불필요 */
	TLR_EMPTY_PACKET_T* ptReq = (TLR_EMPTY_PACKET_T*) &tSendPkt;

	/* ulSrc와 ulDest는 GetPacket / SendPacket에 의해 설정 */
	ptReq->tHead.ulCmd    = ulCmd;
	ptReq->tHead.ulDest   = 0x20;
	ptReq->tHead.ulDestId = 0;
	ptReq->tHead.ulExt    = 0;
	ptReq->tHead.ulId     = 0;
	ptReq->tHead.ulLen    = 0;
	ptReq->tHead.ulRout   = 0;
	ptReq->tHead.ulSrc    = 0;
	ptReq->tHead.ulSrcId  = 0;
	ptReq->tHead.ulSta    = 0;

	tResult = App_SendRecvPkt(ptApp, &tSendPkt, &tRecvPkt);
	return tResult;
}

void CSYCON_DRV::App_GetSlaveState( APPLICATION_T* ptApp, CIFX_PACKET* ptSendPkt, CIFX_PACKET* ptRecvPkt )
{
	// MasterStatusBlock을 읽어서 configuration되어 있는 slave와 활성화되어 있는 slave의 갯수 출력
	NETX_COMMON_STATUS_BLOCK tCommonStatus = {0};

	xChannelCommonStatusBlock(ptApp->tCommon.hChannel, CIFX_CMD_READ_DATA, 0, sizeof(tCommonStatus), &tCommonStatus);

	if (ptApp->tCommon.ulConfigSlaveAmount != tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfConfigSlaves)
	{
		//TRACE("Configured Slaves amount changed from %u to %u.\n", ptApp->tCommon.ulConfigSlaveAmount, tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfConfigSlaves);
		m_Logger.LOG("Configured Slaves amount changed from %u to %u.\n", ptApp->tCommon.ulConfigSlaveAmount, tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfConfigSlaves);
		ptApp->tCommon.ulConfigSlaveAmount = tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfConfigSlaves;
	}

	if (ptApp->tCommon.ulActiveSlaveAmount != tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfActiveSlaves)
	{
		//TRACE("Active Slave amount changed from %u to %u.\n", ptApp->tCommon.ulActiveSlaveAmount, tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfActiveSlaves);
		m_Logger.LOG("Active Slave amount changed from %u to %u.\n", ptApp->tCommon.ulActiveSlaveAmount, tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfActiveSlaves);
		ptApp->tCommon.ulActiveSlaveAmount = tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfActiveSlaves;
	}

	if (ptApp->tCommon.ulFaultySlaveAmount != tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfDiagSlaves)
	{
		//TRACE("Faulty Slaves amount changed from %u to %u.\n", ptApp->tCommon.ulFaultySlaveAmount, tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfDiagSlaves);
		m_Logger.LOG("Faulty Slaves amount changed from %u to %u.\n", ptApp->tCommon.ulFaultySlaveAmount, tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfDiagSlaves);
		ptApp->tCommon.ulFaultySlaveAmount = tCommonStatus.uStackDepended.tMasterStatusBlock.ulNumOfDiagSlaves;
	}
}

TLR_RESULT CSYCON_DRV::App_Initialize( APPLICATION_T* ptApp )
{
	/* return value of function */
	TLR_RESULT tResult = TLR_S_OK;

	/* register on firmware for indications */
	tResult = App_SendRecvEmptyPkt(ptApp, RCX_REGISTER_APP_REQ);
	if(TLR_S_OK != tResult)
	{
		return tResult;
	}  

	return tResult;
}

TLR_RESULT CSYCON_DRV::App_Finalize( APPLICATION_T* ptApp )
{
	//TRACE("Shutdown application...\n");

	/* return value of function */
	TLR_RESULT tResult = TLR_S_OK;

	/* packets for sending and receiving */
	CIFX_PACKET tSendPkt = {{0}};
	CIFX_PACKET tRecvPkt = {{0}};

	/* at first we empty the queue, than we start with the shutdown sequence below */
	while(TLR_S_OK == (tResult = xChannelGetPacket(ptApp->tCommon.hChannel, sizeof(tRecvPkt), &tRecvPkt, ptApp->tCommon.ulTimeout)))
	{
		App_HandlePacket(ptApp, &tRecvPkt);
	}

	/* unregister on firmware */
	App_SendRecvEmptyPkt(ptApp, RCX_UNREGISTER_APP_REQ);

	/* perform ChannelInit on firmware to stop any communication and delete configuration */
	App_SendRecvEmptyPkt(ptApp, RCX_CHANNEL_INIT_REQ);

	return tResult;
}

void CSYCON_DRV::App_HandlePacket( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	TLR_PACKET_HEADER_T* ptPck = (TLR_PACKET_HEADER_T*)ptPacket;

	switch(ptPck->ulCmd)
	{
		// Slave에 대한 handle 얻어옴.
	case RCX_GET_SLAVE_HANDLE_CNF:
		App_HandleGetSlaveHandleCnf(ptApp, ptPacket);
		break;

		// 각 slave에 대한 diagnosis 정보.
	case RCX_GET_SLAVE_CONN_INFO_CNF:
		App_HandleGetSlaveConnInfoCnf(ptApp, ptPacket);
		break;

		// SDO Upload
	case ETHERCAT_MASTER_CMD_SDO_UPLOAD_CNF:
		App_HandleSdoUploadCnf(ptApp, ptPacket);
		break;

		// SDO Download
	case ETHERCAT_MASTER_CMD_SDO_DOWNLOAD_CNF:
		App_HandleSdoDownloadCnf(ptApp, ptPacket);
		break;

		// Get OD List
	case ETHERCAT_MASTER_CMD_GET_ODLIST_CNF:
		App_HandleGetODListCnf(ptApp, ptPacket);
		break;

		// Get Entry Description
	case ETHERCAT_MASTER_CMD_GET_ENTRYDESC_CNF:
		App_HandleGetEntryDescCnf(ptApp, ptPacket);
		break;

		// Get DC Deviation
	case ETHERCAT_MASTER_CMD_GET_DC_DEVIATION_CNF:
		App_HandleGetDCDeviationCnf(ptApp, ptPacket);
		break;

		// Get EC State
	case ETHERCAT_MASTER_CMD_GET_ECSTATE_CNF:
		App_HandleGetECStateCnf(ptApp, ptPacket);
		break;

		// bus scan req
	case ETHERCAT_MASTER_CMD_START_BUS_SCAN_CNF:
		App_HandleStartBusScanCnf(ptApp, ptPacket);
		break;

		// Bus Scan Info
	case ETHERCAT_MASTER_CMD_GET_BUS_SCAN_INFO_CNF:
		App_HandleGetBusScanInfoCnf(ptApp, ptPacket);
		break;

	default:
		//TRACE("Unknown packet. ulCmd=0x%x\n", ptPacket->tHeader.ulCmd);
		break;
	}
}

void CSYCON_DRV::App_HandleGetSlaveHandleCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	RCX_PACKET_GET_SLAVE_HANDLE_CNF_T* ptPck = (RCX_PACKET_GET_SLAVE_HANDLE_CNF_T*)ptPacket;

	/* 패킷 내부의 핸들 양 계산 */
	/* 첫번째 핸들: ptPck->tData.aulHandle[0] */
	/* 두번째 핸들 부터: ptPck->tData.aulHandle[1] */
	TLR_UINT32 ulHandleCnt = ptPck->tHead.ulLen - sizeof(ptPck->tData);
	ulHandleCnt = ulHandleCnt / 4;
	ulHandleCnt++;
	TLR_UINT32 ulTmp;

	//TW322JQ의 경우 HandleCnt는 57
	if (16 > ulHandleCnt)
	{
		for(ulTmp = 0; ulTmp < ulHandleCnt; ulTmp++)
		{
			/* 특정 핸들에 대한 디바이스 정보 get */
			App_GetDeviceInfo(ptApp, ptPck->tData.aulHandle[ulTmp]);
		}
	}
	else
	{
		//TRACE("%u devices configured. This needs more precise handling as only 16 message can be put in the mailbox per time.\n", ulHandleCnt);
	}
}

void CSYCON_DRV::App_HandleGetSlaveConnInfoCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	RCX_PACKET_GET_SLAVE_CONN_INFO_CNF_T* ptPck = (RCX_PACKET_GET_SLAVE_CONN_INFO_CNF_T*)ptPacket;
	ETHERCAT_MASTER_DIAG_GET_SLAVE_DIAG_T* ptSlaveDiag;

	if (ptPck->tData.ulStructID == 5938)
	{
		ptSlaveDiag = (ETHERCAT_MASTER_DIAG_GET_SLAVE_DIAG_T*)(&ptPck->tData +1);
		/*
		TRACE("Handle %u: \n\t StationAddress: %d, \n\t AutoIncAddress: 0x%X,  \n\t CurrentState: 0x%X, \n\t LastError: 0x%X, \n\t Name: %s\n", 
			ptPck->tData.ulHandle,
			ptSlaveDiag->ulStationAddress,
			ptSlaveDiag->ulAutoIncAddress,
			ptSlaveDiag->ulCurrentState,
			ptSlaveDiag->ulLastError,
			ptSlaveDiag->szSlaveName);
		*/
		m_Logger.LOG("Handle %u: \n\t StationAddress: %d, \n\t AutoIncAddress: 0x%X,  \n\t CurrentState: 0x%X, \n\t LastError: 0x%X, \n\t Name: %s",
			ptPck->tData.ulHandle,
			ptSlaveDiag->ulStationAddress,
			ptSlaveDiag->ulAutoIncAddress,
			ptSlaveDiag->ulCurrentState,
			ptSlaveDiag->ulLastError,
			ptSlaveDiag->szSlaveName);
	}
	else
	{
		TRACE("unexpected StructId %u returned.\n", ptPck->tData.ulStructID);
	}
}

void CSYCON_DRV::App_HandleSdoUploadCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_SDO_UPLOAD_CNF_T*)ptPacket;
	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("SDO Upload: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("SDO Upload: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		/* 4 byte 단위로 변수 temp에 복사 후 출력 */
		UINT32 ulTemp;

		memcpy(&ulTemp, ptPck->tData.abSdoData, sizeof(ulTemp));
		TRACE("SDO Upload: nodeID: %d, index: 0x%X, subindex: 0x%X, value: %d\n", ptPck->tData.ulNodeId, ptPck->tData.ulIndex, ptPck->tData.ulSubIndex, ulTemp);
	}
}

void CSYCON_DRV::App_HandleGetODListCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_GET_ODLIST_CNF_T*)ptPacket;

	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("SDO Upload: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("Get OD List: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		/* 4 byte 단위로 변수 temp에 복사 후 출력 */
		UINT32 ulTemp;

		memcpy(&ulTemp, ptPck->tData.ausObjectList, sizeof(ulTemp));


		for( TLR_UINT32 i=0; i<ptPck->tData.ulDataCnt; i++)
		{
			TRACE("OD %d: 0x%X\n", i, ptPck->tData.ausObjectList[i]);
		}
	}
}

void CSYCON_DRV::App_HandleGetEntryDescCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_CNF_T*)ptPacket;

	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		/* 4 byte 단위로 변수 temp에 복사 후 출력 */
		UINT32 ulTemp;

		memcpy(&ulTemp, ptPck->tData.abObjData, sizeof(ulTemp));
		TRACE("Get Entry Description \n");

		TRACE("Node ID: %d\n", ptPck->tData.ulNodeId);
		TRACE("Index: %d\n", ptPck->tData.ulIndex);
		TRACE("Sub-Index: %d\n", ptPck->tData.ulSubIndex);
		TRACE("Bit Mask: %d\n", ptPck->tData.ulValueInfo);
		TRACE("Data Type: %d\n", ptPck->tData.ulDataType);
		TRACE("Number of Bit: %d\n", ptPck->tData.ulBitLen);
		TRACE("Access Right: %d\n", ptPck->tData.ulObAccess);
		TRACE("Unit: %d\n", ptPck->tData.ulUnitType);

		for( TLR_UINT32 i=0; i< ptPck->tData.ulDataLen; i++)
			TRACE("Remained Data: %d\n", ptPck->tData.abObjData[i]);
	}
}

void CSYCON_DRV::App_HandleGetDCDeviationCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_CNF_T*)ptPacket;

	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		/* 4 byte 단위로 변수 temp에 복사 후 출력 */
		UINT32 ulTemp;

		memcpy(&ulTemp, ptPck->tData.aulSlaveDeviation, sizeof(ulTemp));
		TRACE("Get DC Deviation \n");

		TRACE("Broadcast System Time Difference: %d\n", ptPck->tData.ulBroadcastDeviation);

		for( TLR_UINT32 i=0; i< ptApp->tCommon.ulActiveSlaveAmount; i++)
			TRACE("Remained Data: %d\n", ptPck->tData.aulSlaveDeviation[i]);
	}
}

void CSYCON_DRV::App_HandleGetECStateCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_GET_ECSTATE_CNF_T*)ptPacket;


	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		/* 4 byte 단위로 변수 temp에 복사 후 출력 */
		//UINT16 ulTemp;

		//memcpy(&ulTemp, ptPck->tData.usCurrentEcState, sizeof(ulTemp));
		TRACE("Get EC State: %d\n", ptPck->tData.usCurrentEcState);
	}
}

void CSYCON_DRV::App_HandleStartBusScanCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_START_BUS_SCAN_CNF_T*)ptPacket;

	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		TRACE("Found Slaves: %d\n", ptPck->tData.ulFoundSlaves);
	}
}

void CSYCON_DRV::App_HandleGetBusScanInfoCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_CNF_T*)ptPacket;

	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("Get Entry Description: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		/* 4 byte 단위로 변수 temp에 복사 후 출력 */
		//		UINT32 ulTemp;

		//		memcpy(&ulTemp, ptPck->tData.ulFoundSlaves, sizeof(ulTemp));
		TRACE("VendorID: %d\n", ptPck->tData.ulVendorId);
		TRACE("ProductCode: %d\n", ptPck->tData.ulProductCode);
		TRACE("RevisionNumber: %d\n", ptPck->tData.ulRevisionNumber);
		TRACE("SerialNumber: %d\n", ptPck->tData.ulSerialNumber);
		TRACE("PortState: %d\n", ptPck->tData.ulPortState);

	}
}

void CSYCON_DRV::App_HandleSdoDownloadCnf( APPLICATION_T* ptApp, CIFX_PACKET* ptPacket )
{
	ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_CNF_T* ptPck = (ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_CNF_T*)ptPacket;

	if (TLR_E_ETHERCAT_MASTER_COE_NO_MBX_SUPPORT == ptPck->tHead.ulSta)
	{
		TRACE("SDO Upload: Slave does not supports Mailbox!\n");
	}
	else if (TLR_S_OK != ptPck->tHead.ulSta)
	{
		TRACE("SDO Upload: error returned 0x%X returned.\n", ptPck->tHead.ulSta);
	}
	else
	{
		TRACE("SDO Download Success\n");
	}
}

void CSYCON_DRV::App_GetHandles( APPLICATION_T* ptApp )
{
	RCX_PACKET_GET_SLAVE_HANDLE_REQ_T tPck;

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = RCX_GET_SLAVE_HANDLE_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = sizeof(tPck.tData);

	tPck.tData.ulParam = RCX_LIST_CONF_SLAVES;


	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_GetDeviceInfo( APPLICATION_T* ptApp, TLR_UINT32 ulHandle )
{
	RCX_PACKET_GET_SLAVE_CONN_INFO_REQ_T tPck;
	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = RCX_GET_SLAVE_CONN_INFO_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = sizeof(tPck.tData);

	tPck.tData.ulHandle = ulHandle;

	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleProcessData( APPLICATION_T* ptApp )
{
	/* 함수 반환 값 */
	TLR_RESULT tResult = TLR_S_OK;

	/* cyclic 입출력 핸들링 */
	/* read 입력 데이터 */
	tResult = xChannelIORead(ptApp->tCommon.hChannel, 0, 0, ptApp->tCommon.ulReadBufferSize, ptApp->tCommon.pabReadBuffer, ptApp->tCommon.ulReadBufferSize);

	if ((CIFX_DEV_EXCHANGE_FAILED == tResult) || (CIFX_DEV_EXCHANGE_TIMEOUT == tResult))
	{
		TRACE("Read failed with 0x%08x\n", tResult);
	}
	else if (CIFX_DEV_NO_COM_FLAG == tResult)
	{
		//TRACE("Read failed. Device is not communicating\n");
	}
	else if (CIFX_NO_ERROR == tResult)
	{
		//TRACE("Read succeeded. Copy Inputs back to Outputs.\n");

		/* 데이터 미러링을 위해서 read한 메모리를 보낼 메모리에 복사 */
		memcpy (ptApp->tCommon.pabWriteBuffer, ptApp->tCommon.pabReadBuffer, ptApp->tCommon.ulWriteBufferSize);     

		// write 출력 데이터 
		tResult = xChannelIOWrite(ptApp->tCommon.hChannel, 0, 0, ptApp->tCommon.ulWriteBufferSize, ptApp->tCommon.pabWriteBuffer, ptApp->tCommon.ulWriteBufferSize);
	}
	else
	{
		/* received unexpected failure */
		TRACE("Read failed unexpected with 0x%08x\n", tResult);
	}
}

void CSYCON_DRV::App_HandleSdoUploadReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_SDO_UPLOAD_REQ_T tPck;

	char cBuf[128];

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = ETHERCAT_MASTER_CMD_SDO_UPLOAD_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = sizeof(tPck.tData);

	TRACE("Node Address 입력, 예. \"256\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulNodeId = atol(cBuf);

	TRACE("Index 입력, 예. \"4096\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulIndex = atol(cBuf);

	TRACE("Subindex 입력, 예. \"1\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulSubIndex = atol(cBuf);

	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleGetODListReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_GET_ODLIST_REQ_T  tPck;
	char cBuf[128];

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = ETHERCAT_MASTER_CMD_GET_ODLIST_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = sizeof(tPck.tData);



	TRACE("Node Address 입력, 예. \"256\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulNodeId = atol(cBuf);

	TRACE("List Type 입력, \"1에서 5중 선택\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulListType = atol(cBuf);


	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleGetEntryDescReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_GET_ENTRYDESC_REQ_T tPck;


	char cBuf[128];

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = ETHERCAT_MASTER_CMD_GET_ENTRYDESC_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = sizeof(tPck.tData);

	TRACE("Node Address 입력, 예. \"256\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulNodeId = atol(cBuf);

	TRACE("Index 입력, 예. \"4096\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulIndex = atol(cBuf);

	TRACE("Subindex 입력, 예. \"1\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulSubIndex = atol(cBuf);

	TRACE("BitMask 입력, 예. \"Hex: 0x01~0x40\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulAccessBitMask = atoi(cBuf);


	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleGetDCDeviationReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_GET_DC_DEVIATION_REQ_T tPck;


	//	char cBuf[128];

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = ETHERCAT_MASTER_CMD_GET_DC_DEVIATION_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = 0;


	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleGetECStateReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_GET_ECSTATE_REQ_T tPck;

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd = ETHERCAT_MASTER_CMD_GET_ECSTATE_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = 0;

	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleStartBusScanReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_START_BUS_SCAN_REQ_T tPck;

	char cBuf[128];
	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd = ETHERCAT_MASTER_CMD_START_BUS_SCAN_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen = 4;

	TRACE("TimeOut 입력: ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulTimeout = atol(cBuf);

	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleGetBusScanInfoReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_GET_BUS_SCAN_INFO_REQ_T tPck;

	char cBuf[128];
	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd = ETHERCAT_MASTER_CMD_GET_BUS_SCAN_INFO_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen = 2;

	TRACE("Auto Increment Address: ");
	fgets(cBuf, 128, stdin);
	tPck.tData.usAutoIncAddr = static_cast<TLR_UINT16>( atol(cBuf) );

	if (CIFX_NO_ERROR != xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed.\n");
	}
}

void CSYCON_DRV::App_HandleSdoDownloadReq( APPLICATION_T* ptApp )
{
	ETHERCAT_MASTER_PACKET_SDO_DOWNLOAD_REQ_T tPck;

	char cBuf[128];
	TLR_RESULT tResult = TLR_S_OK;

	memset(&tPck, 0, sizeof(tPck));

	tPck.tHead.ulCmd  = ETHERCAT_MASTER_CMD_SDO_DOWNLOAD_REQ;
	tPck.tHead.ulDest = 0x20;
	tPck.tHead.ulLen  = 16+ETHERCAT_MASTER_COE_MAX_SDO_DOWNLOAD_DATA;

	// Data size가 정의되어 있는 Max size보다 큰 경우 == SDO data size 출력
	TRACE("DATA SIZE: %d, MAX: %d", tPck.tHead.ulLen, ETHERCAT_MASTER_COE_MAX_SDO_DOWNLOAD_DATA);

	TRACE("Node Address 입력, 예. \"256\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulNodeId = atol(cBuf);

	TRACE("Index 입력, 예. \"4096\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulIndex = atol(cBuf);

	TRACE("Subindex 입력, 예. \"1\"  ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulSubIndex = atol(cBuf);

	TRACE("Data Count 입력. \"바이트수\" ");
	fgets(cBuf, 128, stdin);
	tPck.tData.ulDataCnt = atoi(cBuf);

	TRACE("SDO data 입력, 단위. \"바이트\" ");
	for( TLR_UINT32 i=0;i < tPck.tData.ulDataCnt; i++)
	{
		fgets(cBuf, 128, stdin);
		tPck.tData.abSdoData[i] = atoi(cBuf);
	}

	tResult = xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout);

	if (CIFX_NO_ERROR != tResult)//xChannelPutPacket(ptApp->tCommon.hChannel, (CIFX_PACKET *)&tPck, ptApp->tCommon.ulTimeout))
	{
		TRACE("Sending packet failed: 0x%08x\n", tResult);
	}
}

BOOL CSYCON_DRV::IsConnected()
{
	return m_bConnected; 
}
