#pragma once

#include "MyList.h"
#include "DEF_COMMON.h"

class AFX_EXT_CLASS CTeachingFile
{
public:
	CTeachingFile(void);
	~CTeachingFile(void);

	virtual int  Initialize(char* szDriverFileName, char* szCokName);
	virtual void Finalize();

	int ReadRobotNameList(CMyList& RobotNameList);
	int ReadRobotName(int nRobotId, char* szBuff, int nBuffSize);
	int ReadPosNameList(int nRobotId, ST_LOCATION_HEAD &tPosNameList);
	int ReadLocNameList(int nRobotId, CStringList& LocNameList);

	int ReadTeachingData(char* szCokName, int nRobotId, int nLocIdx, double* pdPosList, int nPosMax, int* pnListCount);
	int WriteTeachingData(char* szCokName, int nRobotId, int nLocIdx, double* pdPosList, int nListCount);

	int ReadTeachingData(char* szCokName, int nRobotId, int nLocIdx, ST_LOCATION* lpstData);
	int WriteTeachingData(char* szCokName, int nRobotId, int nLocIdx, ST_LOCATION* lpstData);

	int ReadTeachingData(char* szCokName, int nRobotId, ST_ROBOT_TEACHING* lpstData);
	int WriteTeachingData(char* szCokName, int nRobotId, ST_ROBOT_TEACHING* lpstData);

protected:
	char m_szDriverFileName[512];
	char m_szCokName[128];
};

