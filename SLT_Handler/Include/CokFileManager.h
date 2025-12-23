#pragma once

#include "DEF_COMMON.h"

class AFX_EXT_CLASS CCokFileManager
{
public:
	CCokFileManager(void);
	~CCokFileManager(void);

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

	int LoadSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin& lpstData);
	int SaveSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin* lpstData);

    int LoadSltPgm(char* szCokName, ST_SLT_PGM& lpstData);
    int SaveSltPgm(char* szCokName, ST_SLT_PGM* lpstData);

	int LoadBasicInfo(char* szCokName, ST_BASIC_INFO& lpstData);
	int SaveBasicInfo(char* szCokName, ST_BASIC_INFO* lpstData);

	int SaveSignalTower( ST_SLT_SIGNAL_TOWER* lpstData );
	int LoadSignalTower( ST_SLT_SIGNAL_TOWER& lpstData );

	int SaveAppleTestIFidx(std::vector<int> vIFidx);
	int LoadAppleTestIFidx(std::vector<int>& vIFidx);

// 	int LoadUserOffSetData(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP& lpstData);
// 	int SaveUserOffSetData(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP* lpstData);	
// 
// 	int LoadPickerOffSetTrayPp(ST_USER_OFFSET_TRAYPP& lpstData);
// 	int SavePickerOffSetTrayPp(ST_USER_OFFSET_TRAYPP* lpstData);


	
	int LoadTrayAssign(ST_TRAY_ASSIGN &lpstData);
	int SaveTrayAssign(ST_TRAY_ASSIGN *lpstData);

 	int LoadUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP& lpstData);
 	int SaveUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP* lpstData);

	int SaveSiteOnOffInfo(char* szCokName, int nStation, int nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE]);
	int LoadSiteOnOffInfo(char* szCokName, int* nSiteOnOff);
};

