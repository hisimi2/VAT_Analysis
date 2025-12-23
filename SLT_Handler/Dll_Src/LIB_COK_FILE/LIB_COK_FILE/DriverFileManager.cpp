#include "StdAfx.h"
#include "DriverFileManager.h"
#include "ParamList.h"
#include "ConfigData.h"
#include "DEF_COMMON.h"

CDriverFileManager::CDriverFileManager(void)
{
	ZeroMemory(m_szDriverFileName, sizeof(m_szDriverFileName));
	ZeroMemory(m_szCokName, sizeof(m_szCokName));
}


CDriverFileManager::~CDriverFileManager(void)
{
}

int  CDriverFileManager::Initialize(char* szDriverFileName, char* szCokName)
{
	strncpy_s(m_szDriverFileName, sizeof(m_szDriverFileName), szDriverFileName, strlen(szDriverFileName));
	strncpy_s(m_szCokName, sizeof(m_szCokName), szCokName, strlen(szCokName));

	return 0;
}

void CDriverFileManager::Finalize()
{

}


int CDriverFileManager::ReadRobotNameList(CMyList& RobotNameList)
{
	char szKey[128]={0,};
	char szValue[128]={0,};

	CConfigData ConfigData(m_szDriverFileName);

	int nRobotListCount = ConfigData.GetInt("ROBOT NAME LIST", szKey, "0");

	for(int i=0; i<nRobotListCount; i++)
	{
		memset(szKey, 0x00, sizeof(szKey));
		sprintf_s(szKey, "Robot_%d", i+1);

		memset(szValue, 0x00, sizeof(szValue));
		ConfigData.GetString("ROBOT LIST CONFIG", szKey, "0,robot", szValue);

		CParamList PrmList;
		PrmList.m_fnSetDelimiter(",", 1);
		PrmList.m_fnSetDelimitedText(szValue, strlen(szValue));
		TRobotName* pItem = new TRobotName;
		pItem->nRobotId = PrmList.m_fnGetItemInt(0);
		PrmList.m_fnGetItemString(1, pItem->szName);
		RobotNameList.m_fnAddData(pItem);
	}
	return 0;
}

int CDriverFileManager::ReadRobotName(int nRobotId, char* szBuff, int nBuffSize)
{
	CMyList RobotNameList;
	ReadRobotNameList(RobotNameList);
	int nCount = RobotNameList.m_fnGetCount();
	for(int i=0; i<nCount; i++)
	{
		TRobotName* pItem = (TRobotName*)RobotNameList.m_fnGetData(i);
		if( pItem != NULL )
		{
			if( nRobotId == pItem->nRobotId )
			{
				sprintf_s(szBuff, nBuffSize, pItem->szName);
				return 0;
			}
		}
	}
	return -1;
}


int CDriverFileManager::ReadPosNameList(int nRobotId, CStringList& PosNameList)
{
	char szRobotName[128]={0,};
	char szValue[512]={0,};

	ReadRobotName(nRobotId, szRobotName, sizeof(szRobotName));

	CConfigData ConfigData(m_szDriverFileName);
	ConfigData.GetString("ROBOT POSITION NAME LIST", szRobotName, "POS0,POS1,POS2,POS3,POS4,POS5,POS6,POS7,POS8,POS9,POS10", szValue);

	CParamList PrmList;
	PrmList.m_fnSetDelimiter(",", 1);
	PrmList.m_fnSetDelimitedText(szValue, strlen(szValue));

	int nCount = PrmList.m_fnGetCount();
	for(int i=0; i<nCount; i++)
	{
		char szName[128]={0,};
		PrmList.m_fnGetItemString(i, szName);
		PosNameList.AddTail(szName);
	}

	return 0;
}

int CDriverFileManager::ReadLocNameList(int nRobotId, CStringList& LocNameList)
{
	char szRobotName[128]={0,};
	char szSec[256]={0,};
	char szKey[256]={0,};
	char szDef[256]={0,};
	char szVal[512]={0,};

	ReadRobotName(nRobotId, szRobotName, sizeof(szRobotName));

	CConfigData ConfigData(m_szDriverFileName);
	sprintf_s(szSec, sizeof(szSec), "TEACHING LOCATION NAME LIST - %s", szRobotName);
	int nCount = ConfigData.GetInt(szSec, "Location Count", "0");
	for(int i=0; i<nCount; i++)
	{
		memset(szVal, 0x00, sizeof(szVal));
		memset(szDef, 0x00, sizeof(szDef));
		sprintf_s(szKey, sizeof(szKey), "LOC %d", i+1);
		sprintf_s(szDef, sizeof(szDef), "LOC %d", i+1);
		ConfigData.GetString(szSec, szKey, szDef, szVal);
		LocNameList.AddTail( szVal );
	}
	return 0;
}



int CDriverFileManager::ReadTeachingData(char* szCokName, int nRobotId, int nLocIdx, double* pdPosList, int nPosMax, int* pnListCount)
{
	char szRobotName[128]={0,};
	char szSec[256]={0,};
	char szKey[256]={0,};
	char szDef[256]={0,};
	char szVal[512]={0,};

	ReadRobotName(nRobotId, szRobotName, sizeof(szRobotName));

	char szTeachFile[512]={0,};
	char szTemp[512]={0,};
	strcpy_s(szTemp, sizeof(szTemp), m_szDriverFileName);
	char* p = strrchr(szTemp, '\\');
	*(p+1) = '\0';
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s", szTemp, m_szCokName, SZ_TEACHING_FILE_NAME);
	
	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);

	int nLocCount = LocNameList.GetCount();
	
	sprintf_s(szSec, sizeof(szSec), "LOCATION TEACHING TABLE - %s", szRobotName);

	sprintf_s(szKey, sizeof(szKey), "LOC %d", nLocIdx);
	ConfigData.GetString(szSec, szKey, "0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00", szVal);

	CParamList PrmList;
	PrmList.m_fnSetDelimiter(",", 1);
	PrmList.m_fnSetDelimitedText(szVal, strlen(szVal));

	int nPosCount = PrmList.m_fnGetCount();
	for(int i=0; i<nPosCount && i<nPosMax; i++)
	{
		*(pdPosList+i) = PrmList.m_fnGetItemDouble(i);
		*(pnListCount)++;
	}

	return 0;
}

int CDriverFileManager::WriteTeachingData(char* szCokName, int nRobotId, int nLocIdx, double* pdPosList, int nListCount)
{
	char szRobotName[128]={0,};
	char szSec[256]={0,};
	char szKey[256]={0,};
	char szDef[256]={0,};
	char szVal[512]={0,};

	ReadRobotName(nRobotId, szRobotName, sizeof(szRobotName));

	char szTeachFile[512]={0,};
	char szTemp[512]={0,};
	strcpy_s(szTemp, sizeof(szTemp), m_szDriverFileName);
	char* p = strrchr(szTemp, '\\');
	*(p+1) = '\0';
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s", szTemp, m_szCokName, SZ_TEACHING_FILE_NAME);

	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);

	int nLocCount = LocNameList.GetCount();

	sprintf_s(szSec, sizeof(szSec), "LOCATION TEACHING TABLE - %s", szRobotName);

	sprintf_s(szKey, sizeof(szKey), "LOC %d", nLocIdx);

	for(int i=0; i<nListCount; i++)
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		sprintf_s(szTemp, sizeof(szTemp), "%.7f", *(pdPosList+i));
		strcat_s(szVal, sizeof(szVal), szTemp);
		if( i < nListCount-1 ){
			strcat_s(szVal, sizeof(szVal), ",");
		}
	}
	ConfigData.Set(szSec, szKey, szVal);

	return 0;
}