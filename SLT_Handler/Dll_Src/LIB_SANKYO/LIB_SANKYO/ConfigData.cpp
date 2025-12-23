// ConfigData.cpp: implementation of the CConfigData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CConfigData::CConfigData()
{
	memset(m_szFileName,	0x00, sizeof(m_szFileName)	);
	memset(m_szSection,		0x00, sizeof(m_szSection)	);
	memset(m_szKey,			0x00, sizeof(m_szKey)		);
	memset(m_szDefault,		0x00, sizeof(m_szDefault)	);
}

CConfigData::CConfigData(char* szFileName)
{
	memset(m_szFileName,	0x00, sizeof(m_szFileName)	);
	memset(m_szSection,		0x00, sizeof(m_szSection)	);
	memset(m_szKey,			0x00, sizeof(m_szKey)		);
	memset(m_szDefault,		0x00, sizeof(m_szDefault)	);
	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
}

CConfigData::CConfigData(char* szFileName,char* szSection, char* szKey, char* szDefault)
{
	memset(m_szFileName,	0x00, sizeof(m_szFileName)	);
	memset(m_szSection,		0x00, sizeof(m_szSection)	);
	memset(m_szKey,			0x00, sizeof(m_szKey)		);
	memset(m_szDefault,		0x00, sizeof(m_szDefault)	);

	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
	sprintf_s(m_szSection, sizeof(m_szSection), "%s", szSection);
	sprintf_s(m_szKey, sizeof(m_szKey), "%s", szKey);
	sprintf_s(m_szDefault, sizeof(m_szDefault), "%s", szDefault);
}


CConfigData::~CConfigData()
{

}

void CConfigData::Initialize(char* szFileName, char* szSection, char* szKey, char* szDefault)
{
	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
	sprintf_s(m_szSection , sizeof(m_szSection ), "%s", szSection);
	sprintf_s(m_szKey     , sizeof(m_szKey     ), "%s", szKey);
	sprintf_s(m_szDefault , sizeof(m_szDefault ), "%s", szDefault);
}

void CConfigData::Initialize(char* szSection, char* szKey, char* szDefault)
{
	sprintf_s(m_szSection, sizeof(m_szSection), "%s", szSection);
	sprintf_s(m_szKey    , sizeof(m_szKey    ), "%s", szKey);
	sprintf_s(m_szDefault, sizeof(m_szDefault), "%s", szDefault);
}

void CConfigData::SetFileName(char* szFileName)
{
	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
}

int CConfigData::GetInt()
{
	int nRet=0;
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	nRet = atoi(szBuff);
	return nRet;
}

int CConfigData::GetInt(char* szSection, char* szKey, char* szDefault)
{
	int nRet=0;
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	nRet = atoi(szBuff);
	return nRet;
}

double CConfigData::GetDouble()
{
	double dRet=0.0;
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	dRet = atof(szBuff);
	return dRet;
}

double CConfigData::GetDouble(char* szSection, char* szKey, char* szDefault)
{
	double dRet=0.0;
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	dRet = atof(szBuff);
	return dRet;
}


void CConfigData::GetString(char* pData, int nSize)
{
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	sprintf_s(pData, nSize, "%s", szBuff);
}

void CConfigData::GetString(char* szSection, char* szKey, char* szDefault, char* pData, int nSize)
{
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	sprintf_s(pData, nSize, "%s", szBuff);
}


void CConfigData::Set(int nData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%d", nData);
	WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
}

void CConfigData::Set(char* szSection, char* szKey, int nData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%d", nData);
	WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
}


void CConfigData::Set(double dData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%f", dData);
	WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
}

void CConfigData::Set(char* szSection, char* szKey, double dData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%f", dData);
	WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
}

void CConfigData::Set(char* szData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%s", szData);
	WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
}

void CConfigData::Set(char* szSection, char* szKey, char* szData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%s", szData);
	WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
}

