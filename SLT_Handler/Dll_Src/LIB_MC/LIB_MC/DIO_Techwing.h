#pragma once

#include "DIO.h"
#include "io.h"

#define TW_DIO_MAX_PORT_ONE_BD			(16)		// TechWing IO Board의 Board 한장당 최대 Port 수
#define TW_DIO_MAX_POINT_ONE_PORT		(8)			// TechWing IO Board의 Port 하나당 IO 수량
#define TW_DIO_MAX_POINT_ONE_BD			(128)		// TechWing IO Board의 Board 한장당 최대 IO 수량

//==============================================================================
// Digital Input Driver - Techwing Original
//==============================================================================
class CDI_Techwing : public CDI
{
public:
	CDI_Techwing(int nBaseAddress, int nDiBoardCount);
	~CDI_Techwing(void);
	
	void Read_DI();
private:
	int				m_nBaseAddress;
	int				m_nDi_BoardCount;
	int				m_nDI_PortCount;
};


//==============================================================================
// Digital Output Driver - Techwing Original
//==============================================================================
class CDO_Techwing : public CDO
{
public:
	CDO_Techwing(int nBaseAddress, int nDoBoardCount);
	~CDO_Techwing(void);

	void Write_DO();
	void Reset();

private:
	int				m_nBaseAddress;
	int				m_nDo_BoardCount;
	int				m_nDO_PortCount;
};
