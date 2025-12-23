#pragma once
#include "DIO.h"

#define SOFT_SERVOE_DIO_MAX_PORT_ONE_BD			(4)		// TechWing IO Board의 Board 한장당 최대 Port 수
#define SOFT_SERVOE_DIO_MAX_POINT_ONE_PORT		(8)			// TechWing IO Board의 Port 하나당 IO 수량
#define SOFT_SERVOE_DIO_MAX_POINT_ONE_BD			(32)		// TechWing IO Board의 Board 한장당 최대 IO 수량

//==============================================================================
// Digital Input Driver - Techwing Original
//==============================================================================
class CDI_SoftServo : public CDI
{
public:
	CDI_SoftServo(int nBaseAddress, int nDiBoardCount);
	~CDI_SoftServo(void);

	void Read_DI();
	//void SetDriverContext(DIOUM_DRIVER_CONTEXT *DriverContext);

private:
	//DIOUM_DRIVER_CONTEXT *m_DriverContext;
	int				m_nBaseAddress;
	int				m_nDi_BoardCount;
	int				m_nDI_PortCount;
	int				*nEtherCAT_IN;
};


//==============================================================================
// Digital Output Driver - Techwing Original
//==============================================================================
class CDO_SoftServo : public CDO
{
public:
	CDO_SoftServo(int nBaseAddress, int nDoBoardCount);
	~CDO_SoftServo(void);

	void Write_DO();
	virtual int Write_DO_Bit(int _idx, int _on_off) override;
	void Reset();
	void Read_DO_Data();
	

private:
	int				*nEtherCAT_OUT;
	int				m_nBaseAddress;
	int				m_nDo_BoardCount;
	int				m_nDO_PortCount;
};