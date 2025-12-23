#include "StdAfx.h"
#include "DotMatrix_Comm.h"


CDotMatrix_Comm::CDotMatrix_Comm()
{
	m_nPort = 0;
}


CDotMatrix_Comm::~CDotMatrix_Comm()
{
}

int CDotMatrix_Comm::Initialize(int nPort)
{
	m_nPort = nPort;
	int nOpenRes = ComOpen();
	
	if (nOpenRes == -1)
	{
		return -1;
	}

	return 0;
}

int CDotMatrix_Comm::ComOpen()
{
	CString strMsg;

	ComClose();

	if (sio_open(m_nPort) != SIO_OK)
	{
		strMsg.Format("COM%d OPEN FAIL", m_nPort);
		AfxMessageBox(strMsg, MB_TOPMOST);
		return -1;
	}

	if (sio_ioctl(m_nPort, B9600, P_NONE | BIT_8 | STOP_1) != SIO_OK)
	{
		strMsg.Format("COM%d INIT FAIL", m_nPort);
		AfxMessageBox(strMsg, MB_TOPMOST);
		return -1;
	}

	return 0;
}

int CDotMatrix_Comm::ComClose()
{
	int nSts = sio_data_status(m_nPort);
	int nRes = SIO_OK;
	if (nSts != SIO_BADPORT)
	{
		nRes = sio_close(m_nPort);
		if (nRes != SIO_OK)
		{
			return nRes;
		}
	}

	return nRes;
}

int CDotMatrix_Comm::SendData(char* pData)
{
	if (sio_lstatus(m_nPort) == SIO_BADPORT)
	{
		return -1;
	}

	char szData[128] = { 0, };
	strcpy_s(szData, sizeof(szData), (LPCTSTR)pData);
	
	sio_write(m_nPort, szData, strlen(szData));

	int nOverTime = 10;
	double dStartTime = (double)GetTickCount64();//clock();
	
	while (1)
	{
		int nInputCnt = sio_iqueue(m_nPort);
		if (nInputCnt > 0)
		{
			if (ReciveData() == -1)
			{
				sio_write(m_nPort, szData, strlen(szData));
				continue;
			}
			break;
		}
		else
		{
			double dEndTime = (double)GetTickCount64();//clock();
			double tbuf = ((double)dEndTime - (double)dStartTime) / (double)CLK_TCK;
			
			if (nOverTime < tbuf)
			{
				return -2;
			}
		}
		Sleep(1);
	}

	return 0;
}

int CDotMatrix_Comm::ReciveData()
{
	unsigned char csum;
	char CheckSum[20] = { 0 };
	char Addr[20] = { 0 };
	char RvTmp[20] = { 0 };

	char cSTX = '*';
	char cETX = 0x0D;

	char szRvData[20] = { 0, };

	sio_read(m_nPort, szRvData, 19);

	if (*szRvData != '*')
	{
		Initialize(m_nPort);
		return -1;
	}

	strncpy_s(RvTmp, sizeof(RvTmp), szRvData, 8);

	Addr[0] = RvTmp[1];
	Addr[1] = RvTmp[2];
	
	
	//csum = MakeCheckSum(RvTmp + 1, sizeof(RvTmp + 1));
	csum = MakeCheckSum(RvTmp + 1, sizeof(&RvTmp));
	sprintf_s(CheckSum, sizeof(CheckSum), "%02X", csum);

	if (	0 == strncmp(CheckSum, szRvData + 5, 2)
		&&	0 == strncmp(szRvData + 3, "00", 2)
		&&	0 == strncmp(szRvData + 1, Addr, 2))
	{
		sio_flush(m_nPort, 3);
	}
	else
	{
		return -2;
	}

	return 0;
}

unsigned char CDotMatrix_Comm::MakeCheckSum(char * pMsg, int nSize)
{
	unsigned char csum = 0xFF;

	for (int i = 0; i < nSize; i = i + 2)
	{
		if (('A' <= *(pMsg + i + 1)) && ('F' >= *(pMsg + i + 1)))
		{
			switch (*(pMsg + i + 1))
			{
			case 'A': *(pMsg + i + 1) = 0x0A; break;
			case 'B': *(pMsg + i + 1) = 0x0B; break;
			case 'C': *(pMsg + i + 1) = 0x0C; break;
			case 'D': *(pMsg + i + 1) = 0x0D; break;
			case 'E': *(pMsg + i + 1) = 0x0E; break;
			case 'F': *(pMsg + i + 1) = 0x0F; break;
			default: return NULL;
			}
		}
		*(pMsg + i) &= 0x0F;
		*(pMsg + i + 1) &= 0x0F;
		*(pMsg + i) <<= 4;
		csum += *(pMsg + i) + *(pMsg + i + 1);
	}
	csum = ~csum;
	*pMsg = NULL;

	return csum;
}
