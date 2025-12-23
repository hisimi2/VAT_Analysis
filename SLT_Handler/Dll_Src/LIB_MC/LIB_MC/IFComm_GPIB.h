#pragma once

#include "IFComm.h"

class CIFComm_GPIB : public CIFComm
{
public:
	CIFComm_GPIB(void);
	~CIFComm_GPIB(void);

private:
	virtual int		Init_Board();
	virtual void	Final_Board();

	virtual void 	GetBoardRcvCMD();
	virtual int		do_SendCMD(CTesterIFCmd& Packet);

	virtual int					SRQLineCheck();

private:
	int				m_nKindGPIB;
	int				m_nGpibID;
};

//extern CIFComm_GPIB g_IFComm_GPIB;
