#pragma once


//==============================================================================
//
//==============================================================================
#include "DIO.h"
#include "SharedMem.h"
#include "DlgDIOSimul.h"

//==============================================================================
//
//==============================================================================
#ifndef SZ_DATA_MEM_PATH
	#define SZ_DATA_MEM_PATH  _T(".\\sys_data\\DATA_MEM")
#endif

#define TW_DI_SIM_MAP_FILE		"C:\\TWDISIM.map"
#define TW_DO_SIM_MAP_FILE		"C:\\TWDOSIM.map"
#define TW_DI_SIM_MAP			"TWDISIM"	
#define TW_DO_SIM_MAP			"TWDOSIM"	

#define DIO_SIMUL_MAX_PORT_COUNT_ONE_BD			(16)
#define DIO_SIMUL_MAX_BD_COUNT					(8)
#define DIO_SIMUL_MAX_PORT_COUNT_SYS			(DIO_SIMUL_MAX_PORT_COUNT_ONE_BD*DIO_SIMUL_MAX_BD_COUNT)	// Max 128 Ports
#define DIO_SIMUL_MAX_IO_COUNT_SYS				(DIO_SIMUL_MAX_PORT_COUNT_SYS*8)							// 1024 bits
#define DIO_SIMUL_MAX_POINT_ONE_PORT			(8)

#ifndef UM_DIO_SIM_DO_CHANGED
	#define UM_DIO_SIM_DO_CHANGED       (WM_USER + 1000)
#endif


typedef struct _ST_DIO_MEM{
	unsigned char byPort[DIO_SIMUL_MAX_PORT_COUNT_SYS];
	_ST_DIO_MEM()
	{
		memset(this, 0x00, sizeof(_ST_DIO_MEM));
	}
}ST_DIO_MEM, *LPST_DIO_MEM;


//==============================================================================
// Digital Input Driver - Shared Memory
//==============================================================================
class CDI_SIMUL :	public CDI
{
public:
	CDI_SIMUL(int nPointCount);
	~CDI_SIMUL(void);
	void Read_DI();
};

//==============================================================================
// Digital Output Driver - Shared Memory
//==============================================================================
class CDO_SIMUL :	public CDO
{
public:
	CDO_SIMUL(int nPointCount);
	~CDO_SIMUL(void);
	void Write_DO();
};

//==============================================================================
//
//==============================================================================
class CDIO_SIMUL_DRV
{
public:
	CDIO_SIMUL_DRV();
	~CDIO_SIMUL_DRV();

	int  DIO_INIT(const char* szIniFileName, int nDiPointMax, int nDoPointMax);

	BOOL GetSimDataDi(int nIndex);
	void SetSimDataDi(int nIndex, BOOL bOnOff);
	BOOL GetSimDataDo(int nIndex);
	void SetSimDataDo(int nIndex, BOOL bOnOff);

	int GetPointMax(int nInOrOut);
	void ShowDialogSimul();
	int SendSimulMsg_DoChanged(int nIndex, BOOL bChangedValue);
	int SetSimulWnd(HWND hSimulWnd);

public:
	CSharedMem m_smDI;
	CSharedMem m_smDO;
	BYTE* m_pSimMemDi;
	BYTE* m_pSimMemDo;
	CDlgDIOSimul* m_pDlgSimul;
	int m_nMaxPointDi;
	int m_nMaxPointDo;
	HWND m_hSimulWnd;
};