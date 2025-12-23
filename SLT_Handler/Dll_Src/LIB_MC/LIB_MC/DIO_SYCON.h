#pragma  once

#include "DIO.h"
#include "SYCON_DRV.h"



//==============================================================================
// Digital Input Driver - CONTEC
//==============================================================================
class CDI_SYCON :	public CDI
{
public:
	CDI_SYCON(int nPortCount);
	~CDI_SYCON(void);

	void Read_DI();
private:
	int m_nDI_PortCount;
};

//==============================================================================
// Digital Output Driver - CONTEC
//==============================================================================
class CDO_SYCON :	public CDO
{
public:
	CDO_SYCON(int nPortCount);
	~CDO_SYCON(void);

	void Write_DO();
	void Reset();

private:
	int m_nDO_PortCount;
};
