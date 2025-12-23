/********************************************************************
	created:	2015/07/14
	created:	14:7:2015   21:04
	filename: 	D:\Work\99.Repositories\09.tw-strip\trunk\src\Strip Handler\Dll_Src\LIB_MC\LIB_MC\DIO_CONTEC.h
	file path:	D:\Work\99.Repositories\09.tw-strip\trunk\src\Strip Handler\Dll_Src\LIB_MC\LIB_MC
	file base:	DIO_CONTEC
	file ext:	h
	author:		나명진
	
	purpose:	CONTEC USB Type용 DIO Driver
*********************************************************************/
#pragma once

#include "DIO.h"

#pragma comment(lib, "../../../LibFiles/CONTEC/CDIO.lib")


#define CONTEC_DIO_MAX_PORT_ONE_MODULE		(4)			// TechWing IO Board의 Board 한장당 최대 Port 수
#define CONTEC_DIO_MAX_POINT_ONE_PORT		(8)			// TechWing IO Board의 Port 하나당 IO 수량
#define CONTEC_DIO_MAX_POINT_ONE_MODULE		(32)		// TechWing IO Board의 Board 한장당 최대 IO 수량

//==============================================================================
// Digital Input Driver - CONTEC
//==============================================================================
class CDI_CONTEC :	public CDI
{
public:
	CDI_CONTEC(int nBoardCount);
	~CDI_CONTEC(void);

	void read_DI();
private:
	int m_nDI_BoardCount;
	int m_nDI_PortCount;
};

//==============================================================================
// Digital Output Driver - CONTEC
//==============================================================================
class CDO_CONTEC :	public CDO
{
public:
	CDO_CONTEC(int nBoardCount);
	~CDO_CONTEC(void);

	void Write_DO();
	void Reset();
private:
	int m_nDO_BoardCount;
	int m_nDO_PortCount;
};


//==============================================================================
//  USB CONTEC Device
//==============================================================================
#define MAX_CONTEC_DEV					(2)					// 장착된 디바이스 갯수.
#define MAX_PORT_COUNT					(2)					// 모듈 한개달 가질 수 있는 최대 포드 갯수.
#define CONTEC_DEV_INFO_FILE			"CONTEC.INI"

//==============================================================================
//
//==============================================================================
typedef struct _ST_CONTEC_DEV{
	short sId;
	char  szDevName[128];

	_ST_CONTEC_DEV()
	{
		memset(this, 0x00, sizeof(_ST_CONTEC_DEV));
	}
}ST_CONTEC_DEV, *LPST_CONTEC_DEV;


//==============================================================================
//
//==============================================================================
class CDIO_CONTEC_DRV
{
public:
	CDIO_CONTEC_DRV();
	~CDIO_CONTEC_DRV();

	int Initialize(char* szIniFileName, int nFileNameLen);

	int GetDI(unsigned short nPortNo, BYTE* pbyData);
	int SetDO(unsigned short nPortNo, BYTE byData);

private:
	char			m_szInitFileName[MAX_PATH];
	int				m_nNumOfModule;
	int				m_nMaxPortCout;
	ST_CONTEC_DEV	m_aDevice[MAX_CONTEC_DEV];
};


extern CDIO_CONTEC_DRV* g_pContecDrv;