#include "StdAfx.h"
#include "CokFileManager.h"
#include "CokFileManager_Imp.h"


CCokFileManager_Imp* g_pCokFileMgrImp = NULL;


CCokFileManager::CCokFileManager(void)
{
	g_pCokFileMgrImp = new CCokFileManager_Imp;
}


CCokFileManager::~CCokFileManager(void)
{
	if( g_pCokFileMgrImp != NULL ){
		//g_pCokFileMgrImp->Finalize(szLastPkgName);
		delete g_pCokFileMgrImp;
		g_pCokFileMgrImp = NULL;
	}
}

int  CCokFileManager::Initialize(char* szExeDir)
{
	return g_pCokFileMgrImp->Initialize(szExeDir);
}

void CCokFileManager::Finalize(char* szLastPkgName)
{
	g_pCokFileMgrImp->Finalize(szLastPkgName);
}


int CCokFileManager::LoadCokInfo( char* szCokName, ST_COK_INFO& lpstData )
{
	return g_pCokFileMgrImp->LoadCokInfo(szCokName, lpstData);
}

int CCokFileManager::SaveCokInfo( char* szCokName, ST_COK_INFO* lpstData )
{
	return g_pCokFileMgrImp->SaveCokInfo(szCokName, lpstData);
}

int CCokFileManager::LoadCategoryInfo( char* szCokName, ST_CATE_INFO& lpstData )
{
	return g_pCokFileMgrImp->LoadCategoryInfo(szCokName, lpstData);
}

int CCokFileManager::SaveCategoryInfo( char* szCokName, ST_CATE_INFO* lpstData )
{
	return g_pCokFileMgrImp->SaveCategoryInfo(szCokName, lpstData);
}


int CCokFileManager::Load2dMarkInfo(char* szCokName, ST_2D_MARK_INFO& lpstData)
{
	return g_pCokFileMgrImp->Load2dMarkInfo(szCokName, lpstData);
}

int CCokFileManager::Save2dMarkInfo(char* szCokName, ST_2D_MARK_INFO* lpstData)
{
	return g_pCokFileMgrImp->Save2dMarkInfo(szCokName, lpstData);
}

int CCokFileManager::LoadSltPgm(char* szCokName, ST_SLT_PGM& lpstData)
{
    return g_pCokFileMgrImp->LoadSltPgm(szCokName, lpstData);
}
int CCokFileManager::SaveSltPgm(char* szCokName, ST_SLT_PGM* lpstData)
{
    return g_pCokFileMgrImp->SaveSltPgm(szCokName, lpstData);
}

int CCokFileManager::LoadSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin& lpstData)
{
	return g_pCokFileMgrImp->LoadSltPgmDefaultBin(szCokName, lpstData);
}

int CCokFileManager::SaveSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin* lpstData)
{
	return g_pCokFileMgrImp->SaveSltPgmDefaultBin(szCokName, lpstData);
}


int CCokFileManager::LoadBasicInfo( char* szCokName, ST_BASIC_INFO& lpstData )
{
	return g_pCokFileMgrImp->LoadBasicInfo(szCokName, lpstData);
}

int CCokFileManager::SaveBasicInfo( char* szCokName, ST_BASIC_INFO* lpstData )
{
	return g_pCokFileMgrImp->SaveBasicInfo(szCokName, lpstData);
}

int CCokFileManager::SetCurrentDirectoryDLL( char* szExeDir )
{
	return g_pCokFileMgrImp->SetCurrentDirectoryDLL(szExeDir);
}

int CCokFileManager::LoadSltSysFile(ST_SLT_SYSTEM& lpstData)
{
	return g_pCokFileMgrImp->LoadSltSysFile(lpstData);
}

int CCokFileManager::SaveSltSysFile(ST_SLT_SYSTEM* lpstData)
{
	return g_pCokFileMgrImp->SaveSltSysFile(lpstData);
}

int CCokFileManager::SaveSignalTower( ST_SLT_SIGNAL_TOWER* lpstData )
{
	return g_pCokFileMgrImp->SaveSignalTower(lpstData);
}

int CCokFileManager::LoadSignalTower( ST_SLT_SIGNAL_TOWER& lpstData )
{
	return g_pCokFileMgrImp->LoadSignalTower(lpstData);
}

int CCokFileManager::SaveAppleTestIFidx(std::vector<int> vIFidx)
{
	return g_pCokFileMgrImp->SaveAppleTestIFidx(vIFidx);
}
int CCokFileManager::LoadAppleTestIFidx(std::vector<int>& vIFidx)
{
	return g_pCokFileMgrImp->LoadAppleTestIFidx(vIFidx);
}

// int CCokFileManager::SaveUserOffSetData(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP* lpstData )
// {
// 	return g_pCokFileMgrImp->SaveUserOffSetData(szCokName, nTestPpIdx, lpstData);
// }
// 
// int CCokFileManager::LoadUserOffSetData(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP& lpstData )
// {
// 	return g_pCokFileMgrImp->LoadUserOffSetData(szCokName, nTestPpIdx, lpstData);
// }

// int CCokFileManager::LoadPickerOffSetTrayPp(ST_USER_OFFSET_TRAYPP& lpstData)
// {
// 	return g_pCokFileMgrImp->LoadPickerOffSetTrayPp(lpstData);
// }
// 
// int CCokFileManager::SavePickerOffSetTrayPp(ST_USER_OFFSET_TRAYPP* lpstData)
// {
// 	return g_pCokFileMgrImp->SavePickerOffSetTrayPp(lpstData);
// }

// int CCokFileManager::LoadVatData(ST_VAT_3POINT& stVatTeachData)
// {
// 	return g_pCokFileMgrImp->LoadVatData(stVatTeachData);
// }
// 
// int CCokFileManager::SaveVatData(ST_VAT_3POINT* lpstVatTeachData)
// {
// 	return g_pCokFileMgrImp->SaveVatData(lpstVatTeachData);
// }

int CCokFileManager::LoadTrayAssign(ST_TRAY_ASSIGN &lpstData)
{
	return g_pCokFileMgrImp->LoadTrayAssign(lpstData);
}

int CCokFileManager::SaveTrayAssign(ST_TRAY_ASSIGN *lpstData)
{
	return g_pCokFileMgrImp->SaveTrayAssign(lpstData);
}


 int CCokFileManager::SaveUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP* lpstData )
 {
 	return g_pCokFileMgrImp->SaveUserOffSetDataTestPp_MP(szCokName, nTestPpIdx, lpstData);
 }
 
 int CCokFileManager::LoadUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP& lpstData )
 {
 	return g_pCokFileMgrImp->LoadUserOffSetDataTestPp_MP(szCokName, nTestPpIdx, lpstData);
 }

 int CCokFileManager::SaveSiteOnOffInfo(char* szCokName, int nStation, int nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE])
 {
	 return g_pCokFileMgrImp->SaveSiteOnOffInfo(szCokName, nStation, nSiteOnOff);
 }
 int CCokFileManager::LoadSiteOnOffInfo(char* szCokName, int* nSiteOnOff)
 {
	 return g_pCokFileMgrImp->LoadSiteOnOffInfo(szCokName, nSiteOnOff);
 }