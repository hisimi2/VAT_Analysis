#include "StdAfx.h"
#include "DotMatrix_Drv.h"


CDotMatrix_Drv::CDotMatrix_Drv()
{
	m_Comm = NULL;
}


CDotMatrix_Drv::~CDotMatrix_Drv()
{
	if (m_Comm != NULL)
	{
		m_Comm->ComClose();
		delete m_Comm;
		m_Comm = NULL;
	}
}

int CDotMatrix_Drv::Initialize(const char * szConfigFileName)
{
	char szBuff[128] = { 0, };
	int nGetFileLengh = strlen(szConfigFileName);
	GetPrivateProfileString("DotMatrix", "Port_Number", "", szBuff, sizeof(szBuff) - 1, szConfigFileName);
	int nlen = strlen(szBuff);
	if (nlen <= 0)
	{
		WritePrivateProfileString("DotMatrix", "Port_Number", "5", szConfigFileName);
		sprintf_s(szBuff, sizeof(szBuff), "3");
	}

	int nPortNum = atoi(szBuff);

	if (m_Comm != NULL)
	{
		m_Comm->ComClose();
		delete m_Comm;
		m_Comm = NULL;
	}
	else
	{
		m_Comm = new CDotMatrix_Comm;
		m_Comm->Initialize(nPortNum);
	}

	return 0;
}

int CDotMatrix_Drv::ComOpen()
{
	if (m_Comm != NULL)
		m_Comm->ComOpen();

	return 0;
}

int CDotMatrix_Drv::ComClose()
{
	if (m_Comm != NULL)
		m_Comm->ComClose();

	return 0;
}

int CDotMatrix_Drv::SendData(stDotMatrix stData)
{
	if ( m_Comm != NULL)
	{
		unsigned char cCheckSum = 0x00;
		char	cSTX = '*';
		char	cETX = 0x0D;
		char	szAddress[3] = { 0, };
		char	szAuxData[3] = { 0, };
		char	szData[64] = { 0, };
		char	szDataCnt[3] = { 0, };
		char	szBuffer[3] = { 0, };
		char	szCheckSum[3] = { 0, };
		char	szPacket[128] = { 0, };
		char	szCheckSumData[128] = { 0, };
		int		nCheckSumDataLength = 0;

		sprintf_s(szAddress, sizeof(szAddress), "%02X", stData.nAddress);
		sprintf_s(szAuxData, sizeof(szAuxData), "%02X", stData.DotColor);
		sprintf_s(szDataCnt, sizeof(szDataCnt), "%02X", (unsigned int)stData.vData.size());
		for each (auto BinData in stData.vData)
		{
			sprintf_s(szBuffer, sizeof(szBuffer), "%02X", BinData);
			strcat_s(szData, szBuffer);
		}

		sprintf_s(szCheckSumData, sizeof(szCheckSumData), "%s%s%s%s", szAddress, szAuxData, szDataCnt, szData);
		nCheckSumDataLength = strlen(szCheckSumData);

		cCheckSum = m_Comm->MakeCheckSum(szCheckSumData, nCheckSumDataLength);

		sprintf_s(szCheckSum, sizeof(szCheckSum), "%02X", cCheckSum);
		sprintf_s(szPacket, sizeof(szPacket), "%c%s%s%s%s%s%c", cSTX, szAddress, szAuxData, szDataCnt, szData, szCheckSum, cETX);

		m_Comm->SendData(szPacket);
	}	

	return 1;
}
