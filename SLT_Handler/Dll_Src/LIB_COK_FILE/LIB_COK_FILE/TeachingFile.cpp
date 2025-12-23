#include "StdAfx.h"
#include "TeachingFile.h"
#include "ParamList.h"
#include "ConfigData.h"
#include "DEF_COMMON.h"


CTeachingFile::CTeachingFile(void)
{
	ZeroMemory(m_szDriverFileName, sizeof(m_szDriverFileName));
	ZeroMemory(m_szCokName, sizeof(m_szCokName));
}


CTeachingFile::~CTeachingFile(void)
{
}


int  CTeachingFile::Initialize(char* szDriverFileName, char* szCokName)
{
	strncpy_s(m_szDriverFileName, sizeof(m_szDriverFileName), szDriverFileName, strlen(szDriverFileName));
	strncpy_s(m_szCokName, sizeof(m_szCokName), szCokName, strlen(szCokName));

	return 0;
}

void CTeachingFile::Finalize()
{

}


int CTeachingFile::ReadRobotNameList(CMyList& RobotNameList)
{
	char szKey[128]={0,};
	char szValue[128]={0,};

	CConfigData ConfigData(m_szDriverFileName);

	int nRobotListCount = ConfigData.GetInt("ROBOT NAME LIST", "Robot Name Count", "0");

	for(int i=0; i<nRobotListCount; i++)
	{
		memset(szKey, 0x00, sizeof(szKey));
		sprintf_s(szKey, "Robot_%d", i+1);

		memset(szValue, 0x00, sizeof(szValue));
		ConfigData.GetString("ROBOT NAME LIST", szKey, "0,robot", szValue);

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

int CTeachingFile::ReadRobotName(int nRobotId, char* szBuff, int nBuffSize)
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


int CTeachingFile::ReadPosNameList(int nRobotId, ST_LOCATION_HEAD &tPosNameList)
{
	char szRobotName[128]={0,};
	char szValue[512]={0,};

	ReadRobotName(nRobotId, szRobotName, sizeof(szRobotName));

	CConfigData ConfigData(m_szDriverFileName);
	ConfigData.GetString("ROBOT POSITION NAME LIST", szRobotName, "Name=pos1;AxisNo=0[!]Name=pos2;AxisNo=1", szValue);

	CParamList PosList;
	PosList.m_fnSetDelimiter("[!]", 3);
	PosList.m_fnSetDelimitedText(szValue, strlen(szValue));

	int nCount = PosList.m_fnGetCount();
	for(int i=0; i<nCount; i++)
	{
		char szItemString[128]={0,};
		PosList.m_fnGetItemString(i, szItemString);

		CParamList PosItem;
		PosItem.m_fnSetDelimiter(",", 1);
		PosItem.m_fnSetDelimitedText(szItemString, strlen(szItemString));

		PosItem.m_fnGetItemString(0, tPosNameList.aszPosName[i]);
		tPosNameList.anPosAxisNo[i] = PosItem.m_fnGetItemInt(1);

		tPosNameList.nPosCount++;
	}

	return 0;
}

int CTeachingFile::ReadLocNameList(int nRobotId, CStringList& LocNameList)
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



int CTeachingFile::ReadTeachingData(char* szCokName, int nRobotId, int nLocIdx, double* pdPosList, int nPosMax, int* pnListCount)
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
	// sys_data/HandlerRecipe/TrayFile/cok_name
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CString strDir = _T("");
	strDir.Format("%s%s\\%s\\%s", szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);

	sprintf_s(szSec, sizeof(szSec), "LOCATION TEACHING TABLE - %s", szRobotName);

	POSITION pos = LocNameList.FindIndex(nLocIdx);
	sprintf_s(szKey, sizeof(szKey), "%s", LocNameList.GetAt(pos).GetString());
	ConfigData.GetString(szSec, szKey, "0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00", szVal);

	CParamList PrmList;
	PrmList.m_fnSetDelimiter(",", 1);
	PrmList.m_fnSetDelimitedText(szVal, strlen(szVal));

	int nPosCount = PrmList.m_fnGetCount();
	for(int i=0; i<nPosCount && i<nPosMax; i++)
	{
		*(pdPosList+i) = PrmList.m_fnGetItemDouble(i);
	}
	*(pnListCount) = nPosCount;

	return 0;
}

int CTeachingFile::WriteTeachingData(char* szCokName, int nRobotId, int nLocIdx, double* pdPosList, int nListCount)
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
	// sys_data/HandlerRecipe/TrayFile/cok_name
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CString strDir = _T("");
	strDir.Format("%s%s\\%s\\%s", szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);

	sprintf_s(szSec, sizeof(szSec), "LOCATION TEACHING TABLE - %s", szRobotName);

	POSITION pos = LocNameList.FindIndex(nLocIdx);
	sprintf_s(szKey, sizeof(szKey), "%s", LocNameList.GetAt(pos).GetString() );

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


int CTeachingFile::ReadTeachingData(char* szCokName, int nRobotId, int nLocIdx, ST_LOCATION* lpstData)
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
	// sys_data/HandlerRecipe/TrayFile/cok_name
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CString strDir = _T("");
	strDir.Format("%s%s\\%s\\%s", szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);

	sprintf_s(szSec, sizeof(szSec), "LOCATION TEACHING TABLE - %s", szRobotName);

	POSITION pos = LocNameList.FindIndex(nLocIdx);
	CString strLocName = LocNameList.GetAt(pos);
	sprintf_s(szKey, sizeof(szKey), "%s", strLocName.GetString() );
	ConfigData.GetString(szSec, szKey, "0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00", szVal);

	sprintf_s(lpstData->szName, sizeof(lpstData->szName), "%s", strLocName.GetString() );

	CParamList PrmList;
	PrmList.m_fnSetDelimiter(",", 1);
	PrmList.m_fnSetDelimitedText(szVal, strlen(szVal));

	int nPosCount = PrmList.m_fnGetCount();
	for(int i=0; i<nPosCount && i<MAX_POS_IDX_COUNT; i++)
	{
		lpstData->dPos[i] = PrmList.m_fnGetItemDouble(i);
	}

	return 0;
}

int CTeachingFile::WriteTeachingData(char* szCokName, int nRobotId, int nLocIdx, ST_LOCATION* lpstData)
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
	// sys_data/HandlerRecipe/TrayFile/cok_name
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CString strDir = _T("");
	strDir.Format("%s%s\\%s\\%s", szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);

	sprintf_s(szSec, sizeof(szSec), "LOCATION TEACHING TABLE - %s", szRobotName);

	POSITION pos = LocNameList.FindIndex(nLocIdx);
	sprintf_s(szKey, sizeof(szKey), "%s", LocNameList.GetAt(pos).GetString() );

	ST_LOCATION_HEAD tPositionNames;
	ReadPosNameList(nRobotId, tPositionNames);
	int nPosCount = tPositionNames.nPosCount;

	for(int i=0; i<nPosCount; i++)
	{
		memset(szTemp, 0x00, sizeof(szTemp));
		sprintf_s(szTemp, sizeof(szTemp), "%.7f", (lpstData->dPos[i]));
		strcat_s(szVal, sizeof(szVal), szTemp);
		if( i < nPosCount-1 ){
			strcat_s(szVal, sizeof(szVal), ",");
		}
	}
	ConfigData.Set(szSec, szKey, szVal);

	return 0;
}

int CTeachingFile::ReadTeachingData(char* szCokName, int nRobotId, ST_ROBOT_TEACHING* lpstData)
{
	char szRobotName[128]={0,};
	char szSec[256]={0,};
	char szKey[256]={0,};
	char szDef[256]={0,};
	char szVal[512]={0,};

	ReadRobotName(nRobotId, szRobotName, sizeof(szRobotName));

	lpstData->nRobotID = nRobotId;
	sprintf_s(lpstData->szRobotName, sizeof(lpstData->szRobotName), "%s", szRobotName);

	char szTeachFile[512]={0,};
	char szTemp[512]={0,};
	strcpy_s(szTemp, sizeof(szTemp), m_szDriverFileName);
	char* p = strrchr(szTemp, '\\');
	*(p+1) = '\0';
	// sys_data/HandlerRecipe/TrayFile/cok_name
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CString strDir = _T("");
	strDir.Format("%s%s\\%s\\%s", szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	int i;

	// Position Name
	ReadPosNameList(nRobotId, lpstData->stLocHead);	

	// Position Data
	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);
	for(i=0; i<LocNameList.GetCount(); i++){
		ReadTeachingData(szCokName, nRobotId, i, &lpstData->aLoc[i] );
	}

	return 0;
}

// Teaching Position ¸¸ Write ÇÔ.
int CTeachingFile::WriteTeachingData(char* szCokName, int nRobotId, ST_ROBOT_TEACHING* lpstData)
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
	// sys_data/HandlerRecipe/TrayFile/cok_name
	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CString strDir = _T("");
	strDir.Format("%s%s\\%s\\%s", szTemp, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData ConfigData(szTeachFile);

	CStringList LocNameList;
	ReadLocNameList(nRobotId, LocNameList);
	int nLocCount = LocNameList.GetCount();

	for(int i=0; i<nLocCount; i++){
		WriteTeachingData(szCokName, nRobotId, i, &lpstData->aLoc[i]);
	}

	return 0;
}
