#pragma once
#include "MX16LIB/PCOMM.H"

class CDotMatrix_Comm
{
public:
	CDotMatrix_Comm();
	~CDotMatrix_Comm();
	int Initialize(int nPort);
	int ComOpen();
	int ComClose();
	int SendData(char* pData);
	int ReciveData();
	unsigned char MakeCheckSum(char *pMsg, int nSize);
private:
	int m_nPort;
};

