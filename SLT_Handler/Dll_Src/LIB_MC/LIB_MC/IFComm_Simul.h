#pragma once

#include "IFComm.h"

class CIFComm_Simul : public CIFComm
{
public:
	CIFComm_Simul(void);
	~CIFComm_Simul(void);

	virtual int		Init_Board();
	virtual void	Final_Board();

	virtual void 	GetBoardRcvCMD();
	virtual int		do_SendCMD(CTesterIFCmd& Packet);

	virtual int		SRQLineCheck();

private:
	CString MakeBINON( CString strFullsites );

	CString m_strBINON;
};

