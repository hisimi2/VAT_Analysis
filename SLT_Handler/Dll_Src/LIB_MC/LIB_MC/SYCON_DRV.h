#pragma once


//==============================================================================
//
//==============================================================================
#include <vector>
#include "../../../include/cifX32DLL/cifXUser.h"
#include "../../../include/cifX32DLL/cifXErrors.h"
#pragma comment(lib, "../../../LibFiles/cifx32dll.lib")

#include "Logger.h"

//////////////////////////////////////////////////////////////////////////
//==============================================================================
// Add EtherCat Log
//==============================================================================
#include "TLR_Types.h"
#include "TLR_Results.h"
#include "TLR_Packet.h"
#include "rcx_error.h"
#include "rcX_Public.h"
#include "rcX_User.h"
#include "EtherCATMaster_AP_Public.h"
#include "EtherCATMaster_Public.h"
#include "EtherCATMaster_DiagStructDef.h"
#include "EtherCATMaster_error.h"
#include "EtherCATMasterApTask_error.h"
typedef struct APPLICATION_COMMON_Ttag
{
	/* handle from driver api */
	TLR_HANDLE hDriver;

	/* channel handle from driver api */
	TLR_HANDLE hChannel;

	/* default timeout */
	TLR_UINT32 ulTimeout;

	/* receive buffers */
	TLR_UINT8* pabReadBuffer;
	TLR_UINT32 ulReadBufferSize;

	/* transmit buffers */
	TLR_UINT8* pabWriteBuffer;
	TLR_UINT32 ulWriteBufferSize;

	TLR_UINT32 ulConfigSlaveAmount;
	TLR_UINT32 ulActiveSlaveAmount;
	TLR_UINT32 ulFaultySlaveAmount;

} APPLICATION_COMMON_T;

typedef struct APPLICATION_Ttag
{
	APPLICATION_COMMON_T tCommon;
} APPLICATION_T;

//==============================================================================
//
//==============================================================================
#define SYCON_DIO_MAX_POINT_ONE_PORT		(8)			// Sycon IO Board의 Port 하나당 IO 수량
#define SYCON_AIO_SIZE_PER_POINT            (2)
#define SYCON_AIO_CONVERT_RES               (0x3FFF)    // 0v ~ 10v = 0 ~ 3FFF
#define SYCON_AIO_CONVERT_MAX_VOLT_OUTPUT   (10.0)
#define SYCON_AIO_CONVERT_MAX_VOLT_INPUT    (10.0)
 

//==============================================================================
//
//==============================================================================
typedef struct _ST_SYCON_MEM_BLOCK{
	int nType;  // 0:digital, 1:analog
	int nPhyOffset;
	int nBlockSize;
	_ST_SYCON_MEM_BLOCK(){ memset(this, 0x00, sizeof(_ST_SYCON_MEM_BLOCK));		};
}ST_SYCON_MEM_BLOCK;


//==============================================================================
//
//==============================================================================
class CSYCON_DRV
{
public:
	CSYCON_DRV(void);
	~CSYCON_DRV(void);

	int		InitialDriver(char* szIniFileName, int nFileNameLen);

	int     OpenDriver();
	void    CloseDriver();

	int		ReadChannelIO(int nAreaNum, int nOffset, BYTE* pBuffer, int nCount);
	int		WriteChannelIO(int nAreaNum, int nOffset, BYTE* pBuffer, int nCount);

	// DIO
	void	READ_DI_All();
	void	WRITE_DO_All();

	BOOL	GET_DI(INT nBitNum, INT& nData);
	BOOL	GET_DO(INT nBitNum, INT& nData);
	BOOL	SET_DO(INT nBitNum, INT nData);

	// AIO
	void    READ_AI_All();
	void    WRITE_AO_All();

	BOOL    GET_AI(int nIndex, double* pdValue);
	BOOL    GET_AO(int nIndex, double* pdValue);
	BOOL    SET_AO(int nIndex, double   dValue);

	BOOL    IsConnected();
	int     GetMaxDioPointCount(int nInOrOut){	ASSERT( nInOrOut>=0 && nInOrOut < 2); return m_nDioPointMax[nInOrOut];	};
	int     GetMaxAioPointCount(int nInOrOut){	ASSERT( nInOrOut>=0 && nInOrOut < 2); return m_nAioPointMax[nInOrOut];	};

private:
	void	Finalize();

protected:
	enum {eDIO=0, eAIO};

	HANDLE			m_hCifxDriver;
	HANDLE			m_hChannel;
	HANDLE          m_hSysDevice;
	BOOL			m_bConnected;

	// Buffer
	BYTE*			m_pDI_Buffer;
	BYTE*			m_pDO_Buffer;
	BYTE*			m_pAI_Buffer;
	BYTE*			m_pAO_Buffer;

	std::vector<ST_SYCON_MEM_BLOCK>  m_vInMemBlock;
	std::vector<ST_SYCON_MEM_BLOCK>  m_vOutMemBlock;

	int             m_nDI_BlockSize;
	int             m_nDO_BlockSize;
	int             m_nAI_BlockSize;
	int             m_nAO_BlockSize;

	int             m_nDioPointMax[2];
	int             m_nAioPointMax[2];
	CLogger         m_Logger;

protected:	// Sycon Err Log
	/* 패킷용 공통 변수 */
	CIFX_PACKET m_tSendPkt;
	CIFX_PACKET m_tRecvPkt;
	CIFX_PACKET* m_ptRecvPkt;
	APPLICATION_T* m_ptApp;

	TLR_RESULT m_tResult;
	TLR_RESULT App_CreateResources(APPLICATION_T** pptApp);
	TLR_RESULT App_FreeResources(APPLICATION_T* ptApp);
	TLR_RESULT App_SendRecvPkt(APPLICATION_T* ptApp, CIFX_PACKET* ptSendPkt, CIFX_PACKET* ptRecvPkt);
	TLR_RESULT App_SendRecvEmptyPkt(APPLICATION_T* ptApp, TLR_UINT32 ulCmd);

	/* configuration */
	void App_GetSlaveState(APPLICATION_T* ptApp, CIFX_PACKET* ptSendPkt, CIFX_PACKET* ptRecvPkt);	

	/* initialize / finalize application */
	TLR_RESULT App_Initialize(APPLICATION_T* ptApp); //사용안할 수도 있음...
	TLR_RESULT App_Finalize(APPLICATION_T* ptApp);

	/* default packet handler */
	void App_HandlePacket(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);

	/* 펌웨어에서 받은 패킷을 제어하는 함수 */
	void App_HandleGetSlaveHandleCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleGetSlaveConnInfoCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleSdoUploadCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleGetODListCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleGetEntryDescCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleGetDCDeviationCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleGetECStateCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleStartBusScanCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleGetBusScanInfoCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket);
	void App_HandleSdoDownloadCnf(APPLICATION_T* ptApp, CIFX_PACKET* ptPacket); 	

	void App_GetHandles(APPLICATION_T* ptApp);
	void App_GetDeviceInfo(APPLICATION_T* ptApp, TLR_UINT32 ulHandle);

	/* 아래는 현재 사용하지 않음.*/

	/* 입출력 데이터 제어 */
	void App_HandleProcessData(APPLICATION_T* ptApp); 

	void App_HandleSdoUploadReq(APPLICATION_T* ptApp);
	void App_HandleGetODListReq(APPLICATION_T* ptApp);
	void App_HandleGetEntryDescReq(APPLICATION_T* ptApp);
	void App_HandleGetDCDeviationReq(APPLICATION_T* ptApp);
	void App_HandleGetECStateReq(APPLICATION_T* ptApp);
	void App_HandleStartBusScanReq(APPLICATION_T* ptApp);
	void App_HandleGetBusScanInfoReq(APPLICATION_T* ptApp);
	void App_HandleSdoDownloadReq(APPLICATION_T* ptApp); 	
};


