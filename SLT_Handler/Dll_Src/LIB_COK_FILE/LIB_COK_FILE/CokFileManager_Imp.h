#pragma once

#include "DEF_COMMON.h"
#include "MyList.h"

class CCokFileManager_Imp
{
public:
	CCokFileManager_Imp(void);
	~CCokFileManager_Imp(void);

	int  Initialize(char* szExeDir);
	void Finalize(char* szLastPkgName);

	int SetCurrentDirectoryDLL(char* szExeDir);
	int LoadSltSysFile(ST_SLT_SYSTEM& lpstData);
	int SaveSltSysFile(ST_SLT_SYSTEM* lpstData);

	int LoadCokInfo(char* szCokName, ST_COK_INFO& lpstData);
	int SaveCokInfo(char* szCokName, ST_COK_INFO* lpstData);

	int LoadCategoryInfo(char* szCokName, ST_CATE_INFO& lpstData);
	int SaveCategoryInfo(char* szCokName, ST_CATE_INFO* lpstData);

	int Load2dMarkInfo(char* szCokName, ST_2D_MARK_INFO& lpstData);
	int Save2dMarkInfo(char* szCokName, ST_2D_MARK_INFO* lpstData);

    int SaveSltPgm(char* szCokName, ST_SLT_PGM* lpstData);
    int LoadSltPgm(char* szCokName, ST_SLT_PGM& lpstData);

	int LoadSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin& lpstData);
	int SaveSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin* lpstData);

	int LoadBasicInfo( char* szCokName, ST_BASIC_INFO& lpstData );
	int SaveBasicInfo( char* szCokName, ST_BASIC_INFO* lpstData );

	int SaveSignalTower( ST_SLT_SIGNAL_TOWER* lpstData );
	int LoadSignalTower( ST_SLT_SIGNAL_TOWER& lpstData );

	int SaveAppleTestIFidx(std::vector<int> vIFidx);
	int LoadAppleTestIFidx(std::vector<int>& vIFidx);

	
	int LoadTrayAssign(ST_TRAY_ASSIGN &lpstData);
	int SaveTrayAssign(ST_TRAY_ASSIGN *lpstData);

	int SaveUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP* lpstData);
	int LoadUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP& lpstData);

	int SaveSiteOnOffInfo(char* szCokName, int nStation, int nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE]);
	int LoadSiteOnOffInfo(char* szCokName, int* nSiteOnOff);

private:
	char               m_szCurrentDir[512];
};

extern CCokFileManager_Imp*	g_pCokFileMgrImp;