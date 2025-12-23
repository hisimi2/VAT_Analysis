#include "StdAfx.h"
#include "CokFileManager_Imp.h"
#include "ConfigData.h"
#include "DEF_COMMON.h"
#include "ParamList.h"
#include "DEF_LIB_COMMCTRL.h"

CCokFileManager_Imp::CCokFileManager_Imp(void)
{
	ZeroMemory( m_szCurrentDir,	sizeof(m_szCurrentDir));
}


CCokFileManager_Imp::~CCokFileManager_Imp(void)
{
}

int  CCokFileManager_Imp::Initialize(char* szExeDir)
{
	SetCurrentDirectoryDLL(szExeDir);
	return 0;
}

void CCokFileManager_Imp::Finalize(char* szLastPkgName)
{
	char szFilePath[MAX_PATH] = {0,};
	sprintf_s(szFilePath, sizeof( szFilePath ), "%s\\sys_data\\%s",m_szCurrentDir, SZ_SLT_SYS_FILE_NAME);

	CConfigData CConfig(szFilePath);
	CConfig.Set("Last Package used", "Name",  szLastPkgName);
}


int CCokFileManager_Imp::LoadSltSysFile(ST_SLT_SYSTEM& lpstData)
{
	char szFilePath[MAX_PATH] = {0,};
	sprintf_s(szFilePath, sizeof( szFilePath ), "%s\\sys_data\\%s",m_szCurrentDir, SZ_SLT_SYS_FILE_NAME);

	CConfigData CConfig(szFilePath);
	CConfig.GetString("Last Package used", "Name", "Test", lpstData.szLastPackageName);

	LoadCokInfo     (lpstData.szLastPackageName           , lpstData.lpstDataCOK);
	LoadBasicInfo   (lpstData.szLastPackageName           , lpstData.lpstDataBasic);
	LoadCategoryInfo(lpstData.lpstDataBasic.szCategoryName, lpstData.lpstDataCategory);

	char szSltPgmPath[MAX_PATH] = {0,};
	//char szSltPgmName[MAX_PATH] = {0,};
	//sprintf_s( szSltPgmPath, sizeof( szSltPgmPath ), "%s\\sys_data\\handlerRecipe\\%s\\%s.dat",m_szCurrentDir, SZ_SLT_PGM_FOLDER, lpstData.lpstDataBasic.szSltPgmName );
	sprintf_s(szSltPgmPath, sizeof(szSltPgmPath), "%s\\sys_data\\handlerRecipe\\%s\\%s\\Tester.dat", m_szCurrentDir, SZ_SLT_PGM_FOLDER, lpstData.lpstDataBasic.szSltPgmName);

    ST_SLT_PGM lpstDataPgm;
    LoadSltPgm(lpstData.lpstDataBasic.szSltPgmName, lpstData.lpstDataSltPgm);

	return 0;
}

int CCokFileManager_Imp::SaveSltSysFile(ST_SLT_SYSTEM* lpstData)
{
	char szFilePath[MAX_PATH] = {0,};
	sprintf_s(szFilePath, sizeof( szFilePath ), "%s\\sys_data\\%s",m_szCurrentDir, SZ_SLT_SYS_FILE_NAME);

	CConfigData CConfig(szFilePath);
	CConfig.Set("Last Package used", "Name",  lpstData->szLastPackageName);

	return 0;
}

int CCokFileManager_Imp::LoadCokInfo( char* szCokName, ST_COK_INFO& lpstData )
{
	char szCokFullName[512] = {0,};
										    // Ex) root\sysdata\handlerRecipe\TrayFiles\Package\PackageName.dat
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s\\CokInfo.dat",m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);//, szCokName );
	
	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe",m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s",m_szCurrentDir,SZ_TRAY_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s",m_szCurrentDir,SZ_TRAY_FOLDER,szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData CConfigdata(szCokFullName);
	char szMainTitle[256] = {0,};
	{//SPD, ACC, DCC <Start>
		// == Tray PP SPD, ACC, DCC
		//Load Tray PP
		for (int i = RIDX_TRAY_PP_1; i <= RIDX_TRAY_PP_2; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Tray PP %d", i + 1);

			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nSPD = CConfigdata.GetInt(szMainTitle, "X SPD", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nACC = CConfigdata.GetInt(szMainTitle, "X ACC", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nDCC = CConfigdata.GetInt(szMainTitle, "X DCC", "5");

			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nSPD = CConfigdata.GetInt(szMainTitle, "Y SPD", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nACC = CConfigdata.GetInt(szMainTitle, "Y ACC", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nDCC = CConfigdata.GetInt(szMainTitle, "Y DCC", "5");

			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nSPD = CConfigdata.GetInt(szMainTitle, "Z SPD", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nACC = CConfigdata.GetInt(szMainTitle, "Z ACC", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nDCC = CConfigdata.GetInt(szMainTitle, "Z DCC", "5");

			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nSPD = CConfigdata.GetInt(szMainTitle, "X Pitch SPD", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nACC = CConfigdata.GetInt(szMainTitle, "X Pitch ACC", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nDCC = CConfigdata.GetInt(szMainTitle, "X Pitch DCC", "5");

			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nSPD = CConfigdata.GetInt(szMainTitle, "Y Pitch SPD", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nACC = CConfigdata.GetInt(szMainTitle, "Y Pitch ACC", "5");
			lpstData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nDCC = CConfigdata.GetInt(szMainTitle, "Y Pitch DCC", "5");
		}


		// == Test #1~#4 PP SPD, ACC, DCC
		for(int i=RIDX_TEST_PP_1; i<=RIDX_TEST_PP_2; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Test PP %d", i+1);

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nSPD = CConfigdata.GetInt(szMainTitle, "X1 SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nACC = CConfigdata.GetInt(szMainTitle, "X1 ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nDCC = CConfigdata.GetInt(szMainTitle, "X1 DCC", "5");

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nSPD = CConfigdata.GetInt(szMainTitle, "X2 SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nACC = CConfigdata.GetInt(szMainTitle, "X2 ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nDCC = CConfigdata.GetInt(szMainTitle, "X2 DCC", "5");

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nSPD = CConfigdata.GetInt(szMainTitle, "Y SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nACC = CConfigdata.GetInt(szMainTitle, "Y ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nDCC = CConfigdata.GetInt(szMainTitle, "Y DCC", "5");


			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nSPD = CConfigdata.GetInt(szMainTitle, "Z1 SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nACC = CConfigdata.GetInt(szMainTitle, "Z1 ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nDCC = CConfigdata.GetInt(szMainTitle, "Z1 DCC", "5");

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nSPD = CConfigdata.GetInt(szMainTitle, "Z2 SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nACC = CConfigdata.GetInt(szMainTitle, "Z2 ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nDCC = CConfigdata.GetInt(szMainTitle, "Z2 DCC", "5");

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nSPD = CConfigdata.GetInt(szMainTitle, "X1 Pitch SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nACC = CConfigdata.GetInt(szMainTitle, "X1 Pitch ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nDCC = CConfigdata.GetInt(szMainTitle, "X1 Pitch DCC", "5");

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nSPD = CConfigdata.GetInt(szMainTitle, "X2 Pitch SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nACC = CConfigdata.GetInt(szMainTitle, "X2 Pitch ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nDCC = CConfigdata.GetInt(szMainTitle, "X2 Pitch DCC", "5");

			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nSPD = CConfigdata.GetInt(szMainTitle, "Y1 Pitch SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nACC = CConfigdata.GetInt(szMainTitle, "Y1 Pitch ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nDCC = CConfigdata.GetInt(szMainTitle, "Y1 Pitch DCC", "5");
														   												 
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nSPD = CConfigdata.GetInt(szMainTitle, "Y2 Pitch SPD", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nACC = CConfigdata.GetInt(szMainTitle, "Y2 Pitch ACC", "5");
			lpstData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nDCC = CConfigdata.GetInt(szMainTitle, "Y2 Pitch DCC", "5");

		}
	
		// == PRESS Unit #1~#16
		for(int i=RIDX_PRESS_UNIT_1; i<= RIDX_PRESS_UNIT_8; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Press Unit %d", i+1);

			lpstData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD = CConfigdata.GetInt(szMainTitle, "Z SPD", "5");
			lpstData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC = CConfigdata.GetInt(szMainTitle, "Z ACC", "5");
			lpstData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC = CConfigdata.GetInt(szMainTitle, "Z DCC", "5");
			lpstData.aPress_Tm[i].nBlowTm = CConfigdata.GetInt(szMainTitle, "Blow Time", "0");
			lpstData.aPress_Tm[i].dBlowPos = CConfigdata.GetInt(szMainTitle, "Blow Pos", "0");
			lpstData.aPress_Tm[i].dCleanOffset = CConfigdata.GetInt(szMainTitle, "Clean Offset", "0");
		}

		// == Loading Table #1 ~ #4
// 		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_X].nSPD = CConfigdata.GetInt("LoadTbl 1", "X SPD", "5");
// 		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_X].nACC = CConfigdata.GetInt("LoadTbl 1", "X ACC", "5");
// 		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_X].nDCC = CConfigdata.GetInt("LoadTbl 1", "X DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Y].nSPD = CConfigdata.GetInt("LoadTbl 1", "Y SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Y].nACC = CConfigdata.GetInt("LoadTbl 1", "Y ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Y].nDCC = CConfigdata.GetInt("LoadTbl 1", "Y DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Y].nSPD = CConfigdata.GetInt("LoadTbl 2", "Y SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Y].nACC = CConfigdata.GetInt("LoadTbl 2", "Y ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Y].nDCC = CConfigdata.GetInt("LoadTbl 2", "Y DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Y].nSPD = CConfigdata.GetInt("LoadTbl 3", "Y SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Y].nACC = CConfigdata.GetInt("LoadTbl 3", "Y ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Y].nDCC = CConfigdata.GetInt("LoadTbl 3", "Y DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Y].nSPD = CConfigdata.GetInt("LoadTbl 4", "Y SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Y].nACC = CConfigdata.GetInt("LoadTbl 4", "Y ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Y].nDCC = CConfigdata.GetInt("LoadTbl 4", "Y DCC", "5");

		//Loading Tbl Rotate
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Rotate].nSPD = CConfigdata.GetInt("LoadTbl 1", "Rotate SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Rotate].nACC = CConfigdata.GetInt("LoadTbl 1", "Rotate ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Rotate].nDCC = CConfigdata.GetInt("LoadTbl 1", "Rotate DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Rotate].nSPD = CConfigdata.GetInt("LoadTbl 2", "Rotate SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Rotate].nACC = CConfigdata.GetInt("LoadTbl 2", "Rotate ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Rotate].nDCC = CConfigdata.GetInt("LoadTbl 2", "Rotate DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Rotate].nSPD = CConfigdata.GetInt("LoadTbl 3", "Rotate SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Rotate].nACC = CConfigdata.GetInt("LoadTbl 3", "Rotate ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Rotate].nDCC = CConfigdata.GetInt("LoadTbl 3", "Rotate DCC", "5");

		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Rotate].nSPD = CConfigdata.GetInt("LoadTbl 4", "Rotate SPD", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Rotate].nACC = CConfigdata.GetInt("LoadTbl 4", "Rotate ACC", "5");
		lpstData.aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Rotate].nDCC = CConfigdata.GetInt("LoadTbl 4", "Rotate DCC", "5");

		//CConfigdata.Set("LoadTbl Barcode Limit", "Y SPD", lpstData->nLoadTbl_SPD_Barcode_Limit);
		lpstData.nLoadTbl_SPD_Barcode_Limit = CConfigdata.GetInt("LoadTbl Barcode Limit", "Y SPD", "20");

		// == Feeder
		lpstData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD = CConfigdata.GetInt("Feeder", "Y SPD", "5");
		lpstData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC = CConfigdata.GetInt("Feeder", "Y ACC", "5");
		lpstData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC = CConfigdata.GetInt("Feeder", "Y DCC", "5");

		// == Transfer x/z1/z2 (Center)
		lpstData.aTransfer.aAxisForm[eAxis_X].nSPD     = CConfigdata.GetInt("Transfer", "X SPD", "5");
		lpstData.aTransfer.aAxisForm[eAxis_X].nACC     = CConfigdata.GetInt("Transfer", "X ACC", "5");
		lpstData.aTransfer.aAxisForm[eAxis_X].nDCC     = CConfigdata.GetInt("Transfer", "X DCC", "5");
	}//SPD, ACC, DCC <end>

	{//Vacuum, Blow Time <start>
		lpstData.aTranser_Tm.nCylDownWaitTm = CConfigdata.GetInt("Transfer", "Cylinder Down Wait", "0");

		lpstData.aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm = CConfigdata.GetInt("Tray Load PP", "Vacuum Time", "0");
		lpstData.aTrayPP_Tm[eTrayPP_1_LD].nBlowTm   = CConfigdata.GetInt("Tray Load PP", "Blow Time"  , "0");
		lpstData.aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt = CConfigdata.GetInt("Tray PP", "Retry Count", "0");
		lpstData.aTrayPP_Tm[eTrayPP_1_LD].nCylDownWaitTm = CConfigdata.GetInt("Tray PP", "Cylinder Down Wait", "0");
		lpstData.aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Offset = CConfigdata.GetInt("Tray PP", "Pick 1Stage Offset", "0");
		lpstData.aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Speed = CConfigdata.GetInt("Tray PP", "Pick 1Stage Speed", "0");

		lpstData.aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm = CConfigdata.GetInt("Tray Unload PP", "Vacuum Time", "0");
		lpstData.aTrayPP_Tm[eTrayPP_2_UD].nBlowTm = CConfigdata.GetInt("Tray Unload PP", "Blow Time", "0");
		lpstData.aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt = CConfigdata.GetInt("Tray PP", "Retry Count", "0");
		lpstData.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm = CConfigdata.GetInt("Tray Load PP", "Vacuum Time", "0");
		lpstData.aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset = CConfigdata.GetInt("Tray Unload PP", "Linear Offset", "0");

		for(int i=RIDX_TEST_PP_1; i<=RIDX_TEST_PP_2; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Test PP %d", i+1);

			lpstData.aTestPP_Tm[i].nVacuumTm = CConfigdata.GetInt(szMainTitle, "Vacuum Time", "0");
			lpstData.aTestPP_Tm[i].nBlowTm   = CConfigdata.GetInt(szMainTitle, "Blow Time"  , "0");
			lpstData.aTestPP_Tm[i].nRetryCnt = CConfigdata.GetInt(szMainTitle, "Retry Count", "0");
			lpstData.aTestPP_Tm[i].nCylDownWaitTm = CConfigdata.GetInt(szMainTitle, "Cylinder Down Wait", "0");

			lpstData.aTestPP_Tm[i].nPick_1Stage_Offset = CConfigdata.GetInt(szMainTitle, "Pick 1Stage Offset", "0");
			lpstData.aTestPP_Tm[i].nPick_1Stage_Speed = CConfigdata.GetInt(szMainTitle, "Pick 1Stage Speed", "0");
			lpstData.aTestPP_Tm[i].dPickRetryOffset= CConfigdata.GetDouble(szMainTitle, "Pick Retry Offset", "0");
			lpstData.aTestPP_Tm[i].dPickerWaitTm = CConfigdata.GetDouble(szMainTitle, "Picker Wait Time", "0");
			
		}
	}//Vacuum, Blow Time <end>

	{// Tray, Plate, Load Table Pitch Data <start>
		lpstData.aTrayForm.nStart_PitchX = CConfigdata.GetInt("Tray Form", "Start X Pitch", "0");
		lpstData.aTrayForm.nStart_PitchY = CConfigdata.GetInt("Tray Form", "Start Y Pitch", "0");
		lpstData.aTrayForm.nPitchX       = CConfigdata.GetInt("Tray Form", "X Pitch"   , "0");
		lpstData.aTrayForm.nPitchY       = CConfigdata.GetInt("Tray Form", "Y Pitch"   , "0");
		lpstData.aTrayForm.nCnt_X		 = CConfigdata.GetInt("Tray Form", "X Count"   , "0");
		lpstData.aTrayForm.nCnt_Y		 = CConfigdata.GetInt("Tray Form", "Y Count"   , "0");
		lpstData.aTrayForm.nThickness    = CConfigdata.GetInt("Tray Form", "Thickness" , "0");

		lpstData.aPlateForm.nStart_PitchX = CConfigdata.GetInt("Plate Form", "Start X Pitch", "0");
		lpstData.aPlateForm.nStart_PitchY = CConfigdata.GetInt("Plate Form", "Start Y Pitch", "0");
		lpstData.aPlateForm.nPitchX       = CConfigdata.GetInt("Plate Form", "X Pitch", "0");
		lpstData.aPlateForm.nPitchY       = CConfigdata.GetInt("Plate Form", "Y Pitch", "0");
		lpstData.aPlateForm.nCnt_X		  = CConfigdata.GetInt("Plate Form", "X Count", "0");
		lpstData.aPlateForm.nCnt_Y		  = CConfigdata.GetInt("Plate Form", "Y Count", "0");

        lpstData.nLoadTbl_Xpitch		  = CConfigdata.GetInt("LoadTable", "X Pitch", "0");
        lpstData.nLoadTbl_Ypitch		  = CConfigdata.GetInt("LoadTable", "Y Pitch", "0");
        lpstData.nLoadTbl_XCnt		      = CConfigdata.GetInt("LoadTable", "X Count", "0");
        lpstData.nLoadTbl_YCnt		      = CConfigdata.GetInt("LoadTable", "Y Count", "0");

		lpstData.nLoadTbl_Xpitch_Ver		  = CConfigdata.GetInt("LoadTable", "X Pitch Ver", "0");
		lpstData.nLoadTbl_Ypitch_Ver		  = CConfigdata.GetInt("LoadTable", "Y Pitch Ver", "0");

        lpstData.nBuffTbl_Xpitch		  = CConfigdata.GetInt("BuffTable", "X Pitch", "0");
        lpstData.nBuffTbl_Ypitch		  = CConfigdata.GetInt("BuffTable", "Y Pitch", "0");
        lpstData.nBuffTbl_XCnt		      = CConfigdata.GetInt("BuffTable", "X Count", "0");
        lpstData.nBuffTbl_YCnt		      = CConfigdata.GetInt("BuffTable", "Y Count", "0");

		lpstData.nTestPP_ST_XPitch		= CConfigdata.GetInt("TestPP_ST_XPitch", "X Pitch", "0");
		lpstData.nTestPP_ST_YPitch		= CConfigdata.GetInt("TestPP_ST_XPitch", "Y Pitch", "0");

		lpstData.nTestBuffTbl_Xpitch = CConfigdata.GetInt("Test BuffTable", "X Pitch", "0");
		lpstData.nTestBuffTbl_Ypitch = CConfigdata.GetInt("Test BuffTable", "Y Pitch", "0");
		lpstData.nTestBuffTbl_XCnt = CConfigdata.GetInt("Test BuffTable", "X Count", "0");
		lpstData.nTestBuffTbl_YCnt = CConfigdata.GetInt("Test BuffTable", "Y Count", "0");

		lpstData.nTestSite_Xpitch		  = CConfigdata.GetInt("Test Site", "X Pitch", "0");
		lpstData.nTestSite_Ypitch		  = CConfigdata.GetInt("Test Site", "Y Pitch", "0");
		lpstData.nTestSite_XCnt		      = CConfigdata.GetInt("Test Site", "X Count", "0");
		lpstData.nTestSite_YCnt		      = CConfigdata.GetInt("Test Site", "Y Count", "0");

		lpstData.nTestClean_Xpitch		  = CConfigdata.GetInt("Test Clean", "X Pitch", "0");
		lpstData.nTestClean_Ypitch		  = CConfigdata.GetInt("Test Clean", "Y Pitch", "0");
		lpstData.nTestClean_XCnt		  = CConfigdata.GetInt("Test Clean", "X Count", "0");
		lpstData.nTestClean_YCnt		  = CConfigdata.GetInt("Test Clean", "Y Count", "0");

		lpstData.nTrayPPLtPitchX		  = CConfigdata.GetInt("Tray PP Load Table", "X Pitch", "0");
		lpstData.nTrayPPLtPitchY		  = CConfigdata.GetInt("Tray PP Load Table", "Y Pitch", "0");

		lpstData.nTestPP_Xpitch		  = CConfigdata.GetInt("Test PP", "X Pitch", "0");
		lpstData.nTestPP_Ypitch		  = CConfigdata.GetInt("Test PP", "Y Pitch", "0");
	}// Tray, Plate, Load Table Pitch Data <end>
	{// Stacker 
		lpstData.nStacker_Main_WaitTm = CConfigdata.GetInt( "Stacker", "Main Wait", "0" );
		lpstData.nStacker_Sub_WaitTm = CConfigdata.GetInt( "Stacker", "Sub Wait", "0" );
		lpstData.nStacker_Clamp_WaitTm = CConfigdata.GetInt( "Stacker", "Clamp Wait", "0" );
		lpstData.nStacker_Conveyor_StopWaitTm = CConfigdata.GetInt("Stacker", "Conveyor unload wait", "0");
		lpstData.nSlowUnloadStackerOption = CConfigdata.GetInt("Stacker", "Slow Unload Stacker Option", "0");
	}// Stacker Data <end>
	{// Feeder
		lpstData.nFeeder_UpDn_WaitTm = CConfigdata.GetInt("Feeder", "Up/Down Wait", "0");
		lpstData.nFeeder_Clamp_WaitTm = CConfigdata.GetInt("Feeder", "Clamp Wait", "0");
		lpstData.dFeeder_StepOffset = CConfigdata.GetDouble("Feeder", "Step Offset", "0.0");
	}// Feeder Data <end>

	{// 2D Barcode Validate
		lpstData.b2dBarcodeUse = CConfigdata.GetInt("2d Barcode", "MD Use", "1");

		lpstData.b2dDuplicateOptUse = CConfigdata.GetInt("2d Barcode Duplication", "MD Use", "1");

		lpstData.b2dLengthOptUse = CConfigdata.GetInt("2d Barcode Length", "MD Use", "1");
		lpstData.n2dDeviceLength = CConfigdata.GetInt("2d Barcode Length", "Length", "0");
			
		lpstData.b2dMarkOptUse = CConfigdata.GetInt("2d Barcode Mark", "MD Use", "1");
		lpstData.n2dMarkStartIdx = CConfigdata.GetInt("2d Barcode Mark", "Start Idx", "0");

		CConfigdata.GetString("2d Barcode Mark", "Device Name", "", lpstData.sz2dDeviceName);
		CConfigdata.GetString("2d Barcode Mark", "Mark List", "", lpstData.sz2dMarkList);
	}// 2D Barcode Valiate<end>
	

	{//Press Auto Height Cal Data
		for (int i = RIDX_PRESS_UNIT_1; i <= RIDX_PRESS_UNIT_8; i++)
		{
			char szSubTitle[MAX_PATH] = { 0, };
			sprintf_s(szSubTitle, sizeof(szSubTitle), "Press Unit %d", i + 1);

			lpstData.dPressHeightCalData[i] = CConfigdata.GetDouble("Press Auto Height Cal Data", szSubTitle, "0.0");
		}
	}//Press Auto Height Cal Data<end>

	return 0;
}

int CCokFileManager_Imp::SaveCokInfo( char* szCokName, ST_COK_INFO* lpstData )
{
	char szCokFullName[512] = {0,};
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s\\CokInfo.dat",m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);/*, szCokName );*/

	CConfigData CConfigdata(szCokFullName);
	char szMainTitle[256] = {0,};
	{//SPD, ACC, DCC <Start>
		// == Tray PP SPD, ACC, DCC
		for (int i = RIDX_TRAY_PP_1; i <= RIDX_TRAY_PP_2; i++) {
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Tray PP %d", i + 1);
			//Load Tray PP
			CConfigdata.Set(szMainTitle, "X SPD", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_X].nSPD);
			CConfigdata.Set(szMainTitle, "X ACC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_X].nACC);
			CConfigdata.Set(szMainTitle, "X DCC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_X].nDCC);

			CConfigdata.Set(szMainTitle, "Y SPD", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Y].nSPD );
			CConfigdata.Set(szMainTitle, "Y ACC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Y].nACC );
			CConfigdata.Set(szMainTitle, "Y DCC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Y].nDCC );

			CConfigdata.Set(szMainTitle, "Z SPD", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Z].nSPD);
			CConfigdata.Set(szMainTitle, "Z ACC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Z].nACC);
			CConfigdata.Set(szMainTitle, "Z DCC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Z].nDCC);

			CConfigdata.Set(szMainTitle, "X Pitch SPD", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nSPD);
			CConfigdata.Set(szMainTitle, "X Pitch ACC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nACC);
			CConfigdata.Set(szMainTitle, "X Pitch DCC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nDCC);

			CConfigdata.Set(szMainTitle, "Y Pitch SPD", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nSPD);
			CConfigdata.Set(szMainTitle, "Y Pitch ACC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nACC);
			CConfigdata.Set(szMainTitle, "Y Pitch DCC", lpstData->aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nDCC);
		}

		CConfigdata.Set("Tray PP Load Table", "X Pitch", lpstData->nTrayPPLtPitchX);
		CConfigdata.Set("Tray PP Load Table", "Y Pitch", lpstData->nTrayPPLtPitchY);

		// == Test #1~#4 PP SPD, ACC, DCC
		for(int i=RIDX_TEST_PP_1; i<=RIDX_TEST_PP_2; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Test PP %d", i+1);

			CConfigdata.Set(szMainTitle, "X1 SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nSPD );
			CConfigdata.Set(szMainTitle, "X1 ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nACC );
			CConfigdata.Set(szMainTitle, "X1 DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nDCC );

			CConfigdata.Set(szMainTitle, "X2 SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nSPD);
			CConfigdata.Set(szMainTitle, "X2 ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nACC);
			CConfigdata.Set(szMainTitle, "X2 DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nDCC);

			CConfigdata.Set(szMainTitle, "Y SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nSPD );
			CConfigdata.Set(szMainTitle, "Y ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nACC );
			CConfigdata.Set(szMainTitle, "Y DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nDCC );


			CConfigdata.Set(szMainTitle, "Z1 SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nSPD );
			CConfigdata.Set(szMainTitle, "Z1 ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nACC );
			CConfigdata.Set(szMainTitle, "Z1 DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nDCC );

			CConfigdata.Set(szMainTitle, "Z2 SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nSPD);
			CConfigdata.Set(szMainTitle, "Z2 ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nACC);
			CConfigdata.Set(szMainTitle, "Z2 DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nDCC);

			CConfigdata.Set(szMainTitle, "X1 Pitch SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nSPD);
			CConfigdata.Set(szMainTitle, "X1 Pitch ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nACC);
			CConfigdata.Set(szMainTitle, "X1 Pitch DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nDCC);
			CConfigdata.Set(szMainTitle, "X2 Pitch SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nSPD);
			CConfigdata.Set(szMainTitle, "X2 Pitch ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nACC);
			CConfigdata.Set(szMainTitle, "X2 Pitch DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nDCC);

			CConfigdata.Set(szMainTitle, "Y1 Pitch SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nSPD);
			CConfigdata.Set(szMainTitle, "Y1 Pitch ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nACC);
			CConfigdata.Set(szMainTitle, "Y1 Pitch DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nDCC);
			CConfigdata.Set(szMainTitle, "Y2 Pitch SPD", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nSPD);
			CConfigdata.Set(szMainTitle, "Y2 Pitch ACC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nACC);
			CConfigdata.Set(szMainTitle, "Y2 Pitch DCC", lpstData->aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nDCC);

		}

		CConfigdata.Set("Test PP", "X Pitch", lpstData->nTestPP_Xpitch);
		CConfigdata.Set("Test PP", "Y Pitch", lpstData->nTestPP_Ypitch);

		// == PRESS Unit #1~#16
		for(int i=RIDX_PRESS_UNIT_1; i<= RIDX_PRESS_UNIT_8; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Press Unit %d", i+1);

			CConfigdata.Set(szMainTitle, "Z SPD", lpstData->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD);
			CConfigdata.Set(szMainTitle, "Z ACC", lpstData->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC);
			CConfigdata.Set(szMainTitle, "Z DCC", lpstData->aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC);
			CConfigdata.Set(szMainTitle, "Blow Time", lpstData->aPress_Tm[i].nBlowTm);
			CConfigdata.Set(szMainTitle, "Blow Pos", lpstData->aPress_Tm[i].dBlowPos);
			CConfigdata.Set(szMainTitle, "Clean Offset", lpstData->aPress_Tm[i].dCleanOffset);
		}

		// == Loading Table #1 ~ #4
// 		CConfigdata.Set("LoadTbl 1", "X SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_X].nSPD);
// 		CConfigdata.Set("LoadTbl 1", "X ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_X].nACC);
// 		CConfigdata.Set("LoadTbl 1", "X DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_X].nDCC);

		CConfigdata.Set("LoadTbl 1", "Y SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Y].nSPD);
		CConfigdata.Set("LoadTbl 1", "Y ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Y].nACC);
		CConfigdata.Set("LoadTbl 1", "Y DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Y].nDCC);

		CConfigdata.Set("LoadTbl 2", "Y SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Y].nSPD);
		CConfigdata.Set("LoadTbl 2", "Y ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Y].nACC);
		CConfigdata.Set("LoadTbl 2", "Y DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Y].nDCC);

		CConfigdata.Set("LoadTbl 3", "Y SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Y].nSPD);
		CConfigdata.Set("LoadTbl 3", "Y ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Y].nACC);
		CConfigdata.Set("LoadTbl 3", "Y DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Y].nDCC);

		CConfigdata.Set("LoadTbl 4", "Y SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Y].nSPD);
		CConfigdata.Set("LoadTbl 4", "Y ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Y].nACC);
		CConfigdata.Set("LoadTbl 4", "Y DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Y].nDCC);

		CConfigdata.Set("LoadTbl 1", "Rotate SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Rotate].nSPD);
		CConfigdata.Set("LoadTbl 1", "Rotate ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Rotate].nACC);
		CConfigdata.Set("LoadTbl 1", "Rotate DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_1].aAxisForm[eAxis_Tbl_Rotate].nDCC);

		CConfigdata.Set("LoadTbl 2", "Rotate SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Rotate].nSPD);
		CConfigdata.Set("LoadTbl 2", "Rotate ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Rotate].nACC);
		CConfigdata.Set("LoadTbl 2", "Rotate DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_2].aAxisForm[eAxis_Tbl_Rotate].nDCC);

		CConfigdata.Set("LoadTbl 3", "Rotate SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Rotate].nSPD);
		CConfigdata.Set("LoadTbl 3", "Rotate ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Rotate].nACC);
		CConfigdata.Set("LoadTbl 3", "Rotate DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_3].aAxisForm[eAxis_Tbl_Rotate].nDCC);

		CConfigdata.Set("LoadTbl 4", "Rotate SPD", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Rotate].nSPD);
		CConfigdata.Set("LoadTbl 4", "Rotate ACC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Rotate].nACC);
		CConfigdata.Set("LoadTbl 4", "Rotate DCC", lpstData->aLoadTbl_SPD[RIDX_LD_TBL_4].aAxisForm[eAxis_Tbl_Rotate].nDCC);

		CConfigdata.Set("LoadTbl Barcode Limit", "Y SPD", lpstData->nLoadTbl_SPD_Barcode_Limit);

		// == Feeder
		CConfigdata.Set("Feeder", "Y SPD", lpstData->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD);
		CConfigdata.Set("Feeder", "Y ACC", lpstData->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC);
		CConfigdata.Set("Feeder", "Y DCC", lpstData->aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC);

		// == Transfer 
		CConfigdata.Set("Transfer", "X SPD"	   , lpstData->aTransfer.aAxisForm[eAxis_X].nSPD    );
		CConfigdata.Set("Transfer", "X ACC"	   , lpstData->aTransfer.aAxisForm[eAxis_X].nACC    );
		CConfigdata.Set("Transfer", "X DCC"	   , lpstData->aTransfer.aAxisForm[eAxis_X].nDCC    );
	}//SPD, ACC, DCC <end>

	{//Vacuum, Blow Time <start>
		CConfigdata.Set("Transfer", "Cylinder Down Wait", lpstData->aTranser_Tm.nCylDownWaitTm);

		CConfigdata.Set("Tray Load PP", "Vacuum Time" , lpstData->aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm);
		CConfigdata.Set("Tray Load PP", "Blow Time"   , lpstData->aTrayPP_Tm[eTrayPP_1_LD].nBlowTm  );
		CConfigdata.Set("Tray PP", "Retry Count" , lpstData->aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt  );

		CConfigdata.Set("Tray PP", "Pick 1Stage Offset", lpstData->aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Offset);
		CConfigdata.Set("Tray PP", "Pick 1Stage Speed", lpstData->aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Speed);

		CConfigdata.Set("Tray Unload PP", "Vacuum Time", lpstData->aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm);
		CConfigdata.Set("Tray Unload PP", "Blow Time", lpstData->aTrayPP_Tm[eTrayPP_2_UD].nBlowTm);
		CConfigdata.Set("Tray PP", "Retry Count", lpstData->aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt);


		CConfigdata.Set("Tray Unload PP", "Linear Offset", lpstData->aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset);;

		for(int i=RIDX_TEST_PP_1; i<=RIDX_TEST_PP_2; i++)
		{
			ZeroMemory(szMainTitle, sizeof(szMainTitle));
			sprintf_s(szMainTitle, sizeof(szMainTitle), "Test PP %d", i+1);

			CConfigdata.Set(szMainTitle, "Vacuum Time", lpstData->aTestPP_Tm[i].nVacuumTm);
			CConfigdata.Set(szMainTitle, "Blow Time"  , lpstData->aTestPP_Tm[i].nBlowTm  );
			CConfigdata.Set(szMainTitle, "Retry Count", lpstData->aTestPP_Tm[i].nRetryCnt);
			CConfigdata.Set(szMainTitle, "Pick 1Stage Offset", lpstData->aTestPP_Tm[i].nPick_1Stage_Offset);
			CConfigdata.Set(szMainTitle, "Pick 1Stage Speed", lpstData->aTestPP_Tm[i].nPick_1Stage_Speed);
			CConfigdata.Set(szMainTitle, "Pick Retry Offset", lpstData->aTestPP_Tm[i].dPickRetryOffset);
			CConfigdata.Set(szMainTitle, "Picker Wait Time", lpstData->aTestPP_Tm[i].dPickerWaitTm);
		}
	}//Vacuum, Blow Time <end>

	{// Tray, Plate Pitch Data <start>
        CConfigdata.Set("Tray Form" , "Start X Pitch"	, lpstData->aTrayForm.nStart_PitchX );
        CConfigdata.Set("Tray Form" , "Start Y Pitch"	, lpstData->aTrayForm.nStart_PitchY );
        CConfigdata.Set("Tray Form" , "X Pitch"			, lpstData->aTrayForm.nPitchX       );
        CConfigdata.Set("Tray Form" , "Y Pitch"			, lpstData->aTrayForm.nPitchY       );
        CConfigdata.Set("Tray Form" , "X Count"			, lpstData->aTrayForm.nCnt_X		);
        CConfigdata.Set("Tray Form" , "Y Count"			, lpstData->aTrayForm.nCnt_Y		);
        CConfigdata.Set("Tray Form" , "Thickness"		, lpstData->aTrayForm.nThickness	);

        CConfigdata.Set("Plate Form", "Start X Pitch"	, lpstData->aPlateForm.nStart_PitchX);
        CConfigdata.Set("Plate Form", "Start Y Pitch"	, lpstData->aPlateForm.nStart_PitchY);
        CConfigdata.Set("Plate Form", "X Pitch"			, lpstData->aPlateForm.nPitchX      );
        CConfigdata.Set("Plate Form", "Y Pitch"			, lpstData->aPlateForm.nPitchY      );
        CConfigdata.Set("Plate Form", "X Count"			, lpstData->aPlateForm.nCnt_X		);
        CConfigdata.Set("Plate Form", "Y Count"			, lpstData->aPlateForm.nCnt_Y		);

        CConfigdata.Set("LoadTable" , "X Pitch"         , lpstData->nLoadTbl_Xpitch         );
        CConfigdata.Set("LoadTable" , "Y Pitch"         , lpstData->nLoadTbl_Ypitch         );
        CConfigdata.Set("LoadTable" , "X Count"         , lpstData->nLoadTbl_XCnt	        );
        CConfigdata.Set("LoadTable" , "Y Count"         , lpstData->nLoadTbl_YCnt	        );

		CConfigdata.Set("LoadTable" , "X Pitch Ver"     , lpstData->nLoadTbl_Xpitch_Ver     );
		CConfigdata.Set("LoadTable" , "Y Pitch Ver"     , lpstData->nLoadTbl_Ypitch_Ver     );

        CConfigdata.Set("BuffTable" , "X Pitch"         , lpstData->nBuffTbl_Xpitch         );
        CConfigdata.Set("BuffTable" , "Y Pitch"         , lpstData->nBuffTbl_Ypitch         );
        CConfigdata.Set("BuffTable" , "X Count"         , lpstData->nBuffTbl_XCnt	        );
        CConfigdata.Set("BuffTable" , "Y Count"         , lpstData->nBuffTbl_YCnt	        );

		CConfigdata.Set("TestPP_ST_XPitch", "X Pitch", lpstData->nTestPP_ST_XPitch);
		CConfigdata.Set("TestPP_ST_XPitch", "Y Pitch", lpstData->nTestPP_ST_YPitch);

		CConfigdata.Set("Test BuffTable" , "X Pitch"         , lpstData->nTestBuffTbl_Xpitch        );
		CConfigdata.Set("Test BuffTable" , "Y Pitch"         , lpstData->nTestBuffTbl_Ypitch        );
		CConfigdata.Set("Test BuffTable" , "X Count"         , lpstData->nTestBuffTbl_XCnt	        );
		CConfigdata.Set("Test BuffTable" , "Y Count"         , lpstData->nTestBuffTbl_YCnt	        );

		CConfigdata.Set("Test Site" , "X Pitch"         , lpstData->nTestSite_Xpitch        );
		CConfigdata.Set("Test Site" , "Y Pitch"         , lpstData->nTestSite_Ypitch        );
		CConfigdata.Set("Test Site" , "X Count"         , lpstData->nTestSite_XCnt	        );
		CConfigdata.Set("Test Site" , "Y Count"         , lpstData->nTestSite_YCnt	        );

		CConfigdata.Set("Test Clean" , "X Pitch"         , lpstData->nTestClean_Xpitch        );
		CConfigdata.Set("Test Clean" , "Y Pitch"         , lpstData->nTestClean_Ypitch        );
		CConfigdata.Set("Test Clean" , "X Count"         , lpstData->nTestClean_XCnt	      );
		CConfigdata.Set("Test Clean" , "Y Count"         , lpstData->nTestClean_YCnt	      );
	}// Tray, Plate Pitch Data <end>

	{// Stacker
		CConfigdata.Set( "Stacker", "Main Wait", lpstData->nStacker_Main_WaitTm );
		CConfigdata.Set( "Stacker", "Sub Wait", lpstData->nStacker_Sub_WaitTm );
		CConfigdata.Set( "Stacker", "Clamp Wait", lpstData->nStacker_Clamp_WaitTm );
		CConfigdata.Set( "Stacker", "Conveyor unload wait", lpstData->nStacker_Conveyor_StopWaitTm);
		CConfigdata.Set("Stacker", "Slow Unload Stacker Option", lpstData->nSlowUnloadStackerOption);
	}// Stacker Data <end>
	{// Feeder
		CConfigdata.Set("Feeder", "Up/Down Wait", lpstData->nFeeder_UpDn_WaitTm);
		CConfigdata.Set("Feeder", "Clamp Wait", lpstData->nFeeder_Clamp_WaitTm);
		CConfigdata.Set("Feeder", "Step Offset", lpstData->dFeeder_StepOffset);
	}// Feeder Data <end>

	{// 2d Barcode Validate
		CConfigdata.Set("2d Barcode", "MD Use", lpstData->b2dBarcodeUse); 

		CConfigdata.Set("2d Barcode Duplication", "MD Use", lpstData->b2dDuplicateOptUse); 

		CConfigdata.Set("2d Barcode Length", "MD Use", lpstData->b2dLengthOptUse); 
		CConfigdata.Set("2d Barcode Length", "Length", lpstData->n2dDeviceLength);
		
		CConfigdata.Set("2d Barcode Mark", "MD Use", lpstData->b2dMarkOptUse); 
		CConfigdata.Set("2d Barcode Mark", "Start Idx", lpstData->n2dMarkStartIdx);

		CConfigdata.Set("2d Barcode Mark", "Device Name", lpstData->sz2dDeviceName);
		CConfigdata.Set("2d Barcode Mark", "Mark List", lpstData->sz2dMarkList);
	}// 2d Barcode Validate<end>


	{//Press Auto Height Cal Data
		for (int i = RIDX_PRESS_UNIT_1; i <= RIDX_PRESS_UNIT_8; i++)
		{
			char szSubTitle[MAX_PATH] = { 0, };
			sprintf_s(szSubTitle, sizeof(szSubTitle), "Press Unit %d", i + 1);

			CConfigdata.Set("Press Auto Height Cal Data", szSubTitle, lpstData->dPressHeightCalData[i]);
		}
	}//Press Auto Height Cal Data<end>


	return 0;
}


int CCokFileManager_Imp::LoadCategoryInfo( char* szCokName, ST_CATE_INFO& lpstData )
{
	char szCokFullName[512] = {0,};

	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s.csv",m_szCurrentDir, SZ_CATEGORY_FOLDER, szCokName );

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe",m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s",m_szCurrentDir,SZ_CATEGORY_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	/*파일 유무 체크 방식 2*/
	if(PathFileExists(szCokFullName) == FALSE)
	{
		CString strLastError;
		DWORD dwLastError = GetLastError();
		switch(dwLastError)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			{
				AfxMessageBox("Category file does not exist.It creates a temporary data!!!!!\r\nPlease check your data!!");

				CString strDefaultData = _T("");
                strDefaultData += _T("99,4,NOERRORSTRING,0\n");
                strDefaultData += _T("99,4,The OTHER REJECTS,0");
				CFile f;
				f.Open(szCokFullName, (CFile::modeCreate|CFile::modeWrite));
				f.SeekToEnd();
				f.Write(strDefaultData, strlen(strDefaultData));
				f.Close();
			}break;
		default:
			strLastError.Format("Category file access denied. (GetLastErrorCode=[%lu])", dwLastError);
			AfxMessageBox(strLastError);
			break;
		}
	}

	CString strGetText  = _T("");
	CString strGetSWBin = _T("");
	CString strHWBin    = _T("");
	CString strCode     = _T("");
	CString strDesc     = _T("");
	CString strPassFail = _T("");
	CString strIncorrectText = _T("");
	CString strHWBin_Tester = _T("");
	CStdioFile sf(szCokFullName, CFile::modeRead | CFile::shareDenyNone);
	int nCount = 0;

	while(sf.ReadString(strGetText) != NULL)
	{
		AfxExtractSubString(strGetSWBin, strGetText, 0, ',');
		AfxExtractSubString(strHWBin   , strGetText, 1, ',');
		AfxExtractSubString(strDesc    , strGetText, 2, ',');
		AfxExtractSubString(strPassFail, strGetText, 3, ',');
		AfxExtractSubString(strCode    , strGetText, 4, ',');
		AfxExtractSubString(strHWBin_Tester, strGetText, 5, ',');
		AfxExtractSubString(strIncorrectText, strGetText, 6, ',');

		if (strIncorrectText.IsEmpty() == FALSE) {
			AfxMessageBox("Unable to load because there are more than four items!");
			break;
		};

		sprintf_s(lpstData.aCateInfo[nCount].szSWBin, sizeof(lpstData.aCateInfo[nCount].szSWBin), "%s", strGetSWBin.GetString() );
		if(strGetSWBin.IsEmpty() == TRUE){ continue; };

		sprintf_s(lpstData.aCateInfo[nCount].szHWBin, sizeof(lpstData.aCateInfo[nCount].szHWBin), "%s", strHWBin.GetString() );
		if(strHWBin.IsEmpty() == TRUE){ continue; };

		//if(strCode.IsEmpty() == TRUE){ continue; };

		sprintf_s(lpstData.aCateInfo[nCount].szDesc, sizeof(lpstData.aCateInfo[nCount].szDesc), "%s", strDesc.GetString() );
		sprintf_s(lpstData.aCateInfo[nCount].szPassFail, sizeof(lpstData.aCateInfo[nCount].szPassFail), "%s", strPassFail.GetString() );
		sprintf_s(lpstData.aCateInfo[nCount].szCode, sizeof(lpstData.aCateInfo[nCount].szCode), "%s", strCode.GetString());
		sprintf_s(lpstData.aCateInfo[nCount].szHWBin_Tester, sizeof(lpstData.aCateInfo[nCount].szHWBin_Tester), "%s", strHWBin_Tester.GetString());
		
		if(strPassFail.IsEmpty() == TRUE){ continue; };

		nCount++;
	}
	sf.Close();

	lpstData.nIndexCount = nCount;

	return RETURN_OK;
}


int CCokFileManager_Imp::SaveCategoryInfo( char* szCokName, ST_CATE_INFO* lpstData )
{
	char szCokFullName[512] = {0,};
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s.csv",m_szCurrentDir, SZ_CATEGORY_FOLDER, szCokName );

 	CFile f;

	f.Open(szCokFullName, CFile::modeCreate);
	f.Close();

	f.Open(szCokFullName, CFile::modeWrite);
	f.SeekToEnd();
	
	CString strData = _T("");
	int nCount = lpstData->nIndexCount;
	for(int i=0;i<nCount;i++)
	{
		strData.Format("%s,%s,%s,%s,%s,%s",lpstData->aCateInfo[i].szSWBin,lpstData->aCateInfo[i].szHWBin,lpstData->aCateInfo[i].szDesc,lpstData->aCateInfo[i].szPassFail, lpstData->aCateInfo[i].szCode, lpstData->aCateInfo[i].szHWBin_Tester);
		strData+= "\r\n";
		f.Write(strData, strlen(strData));		
	}
	
	f.Close();

	return 0;
}


int CCokFileManager_Imp::Load2dMarkInfo(char* szCokName, ST_2D_MARK_INFO& lpstData)
{
	char sz2dMarkFullName[512] = { 0, };

	sprintf_s(sz2dMarkFullName, sizeof(sz2dMarkFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\%s\\%s.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName, SZ_2D_MARK_FOLDER, lpstData.szDeviceName);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe", m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s", m_szCurrentDir, SZ_TRAY_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	if (PathFileExists(sz2dMarkFullName) == FALSE)
	{
		CString strLastError;
		DWORD dwLastError = GetLastError();
		switch (dwLastError)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		{
			AfxMessageBox("2d Mark file does not exist!!");
		}break;
		default:
			strLastError.Format("2d Mark file access denied. (GetLastErrorCode=[%lu])", dwLastError);
			AfxMessageBox(strLastError);
			break;
		}
	}

	CString strGetText = _T("");
	CString str2dMark = _T("");
	CString strIncorrectText = _T("");
	CStdioFile sf(sz2dMarkFullName, CFile::modeRead | CFile::shareDenyNone);

	while (sf.ReadString(strGetText) != NULL)
	{
		AfxExtractSubString(str2dMark, strGetText, 0, ',');
		AfxExtractSubString(strIncorrectText, strGetText, 1, ',');

		if (strIncorrectText.IsEmpty() == FALSE) {
			AfxMessageBox("Unable to load because there are more than four items!");
			break;
		};

		lpstData.v2dMarkList.push_back(str2dMark);

	}
	sf.Close();

	return RETURN_OK;
}


int CCokFileManager_Imp::Save2dMarkInfo(char* szCokName, ST_2D_MARK_INFO* lpstData)
{
	char sz2dMarkFullName[512] = { 0, };

	sprintf_s(sz2dMarkFullName, sizeof(sz2dMarkFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\%s\\%s.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName, SZ_2D_MARK_FOLDER, lpstData->szDeviceName);

	CFile f;

	f.Open(sz2dMarkFullName, CFile::modeCreate);
	f.Close();

	f.Open(sz2dMarkFullName, CFile::modeWrite);
	f.SeekToEnd();

	CString strData = _T("");
	int nCount = lpstData->v2dMarkList.size();
	for (int i = 0; i < nCount; i++)
	{
		strData.Format("%s", lpstData->v2dMarkList[i]);
		strData += "\r\n";
		f.Write(strData, strlen(strData));
	}

	f.Close();

	return 0;
}


int CCokFileManager_Imp::LoadSltPgm(char* szCokName, ST_SLT_PGM& lpstData)
{
	char szCokFullName[512] = {0,};
    //sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s.dat",m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\Tester.dat", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);
    //CConfigData CConfig(szCokFullName);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe", m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s", m_szCurrentDir, SZ_SLT_PGM_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	CConfigData CConfig(szCokFullName);

    lpstData.nBootTmOut         = CConfig.GetInt("Time Out"          , "Boot Time"        , "0");
    lpstData.nTestTmOut         = CConfig.GetInt("Time Out"          , "Test Time"        , "0");
    lpstData.nAcPowerOnDelayTm  = CConfig.GetInt("Delay Time"        , "AC Power On"      , "0");
    lpstData.nAcPowerOffDelayTm = CConfig.GetInt("Delay Time"        , "AC Power Off"     , "0");
    lpstData.nAtxPowerOnDelayTm = CConfig.GetInt("Delay Time"        , "ATX Power On"     , "0");
    lpstData.nBaudRate          = CConfig.GetInt("Baud Rate"         , "Baud Rate"        , "0");
    lpstData.nPreCoolingTm      = CConfig.GetInt("Cooling"           , "Pre Cooling Time" , "0");    
    lpstData.nAgpDelayTm        = CConfig.GetInt("AGP"               , "AGP Delay Time"   , "0");    
    lpstData.nBiosBootCnt       = CConfig.GetInt("BIOS Boot"         , "Count"            , "0");
    lpstData.nBiosTmOut         = CConfig.GetInt("BIOS Boot"         , "Time Out"         , "0");    
    lpstData.nBiosS3Cnt         = CConfig.GetInt("BIOS S3"           , "Count"            , "0");
    lpstData.nBiosS3TmOut       = CConfig.GetInt("BIOS S3"           , "Time Out"         , "0");
    lpstData.nBiosS3DelayTm     = CConfig.GetInt("BIOS S3"           , "Delay Time"       , "0");    
    lpstData.nSetTemp           = CConfig.GetInt("Temperature"       , "Setting Temp"     , "0");
    lpstData.nTempGuardBand     = CConfig.GetInt("Temperature"       , "Guard Band"       , "0");
    lpstData.nMinPassTm         = CConfig.GetInt("Min PassTime"      , "Min PassTime"     , "0");
    lpstData.nFailCodeDigitalNo = CConfig.GetInt("Fail Code"         , "Digital"          , "0");
	lpstData.nSotDelayTm        = CConfig.GetInt("Delay Time"        , "SotDelay"         , "0");
	lpstData.nNotTestingTmOut    = CConfig.GetInt("Not Testing Time Out", "Not Testing Time Out", "0");

    CConfig.GetString("Fail Code"      , "Header"      , "", lpstData.szFailCodeHeader);
    CConfig.GetString("Pass String"    , "String"      , "", lpstData.szPassString);
    CConfig.GetString("Line"           , "Start Line"  , "", lpstData.szStartLine);
    CConfig.GetString("Line"           , "End Line"    , "", lpstData.szEndLine);

    // mode
    lpstData.bMD_BootTmOut	    = (BOOL)CConfig.GetInt("Mode"         , "BootTimeOut"            , "0"); 
    lpstData.bMD_PreCooling     = (BOOL)CConfig.GetInt("Mode"         , "PreCooling"             , "0"); 
    lpstData.bMD_AcPwrOnDly     = (BOOL)CConfig.GetInt("Mode"         , "AcPowerOn"              , "0"); 
    lpstData.bMD_AcPwrOff       = (BOOL)CConfig.GetInt("Mode"         , "AcPowerOff"             , "0"); 
    lpstData.bMD_AtxPowerOn     = (BOOL)CConfig.GetInt("Mode"         , "AtxPowerOn"             , "0"); 
    lpstData.bMD_PassStr        = (BOOL)CConfig.GetInt("Mode"         , "PassString"             , "0"); 
    lpstData.bMD_FailCodeHead   = (BOOL)CConfig.GetInt("Mode"         , "FailCodeHeader"         , "0"); 
    lpstData.bMD_FailCodeDig    = (BOOL)CConfig.GetInt("Mode"         , "FailCodeDigital"        , "0"); 
    lpstData.bMD_StartLine      = (BOOL)CConfig.GetInt("Mode"         , "StartLine"              , "0"); 
    lpstData.bMD_EndLine        = (BOOL)CConfig.GetInt("Mode"         , "EndLine"                , "0"); 
    lpstData.bMD_MinPassTm      = (BOOL)CConfig.GetInt("Mode"         , "MinPassTime"            , "0"); 
    lpstData.bMD_BaudRate       = (BOOL)CConfig.GetInt("Mode"         , "BaudRate"               , "0"); 
    lpstData.bMD_HangReboot     = (BOOL)CConfig.GetInt("Mode"         , "HangReboot"             , "0"); 
    lpstData.bMD_BiosBoot       = (BOOL)CConfig.GetInt("Mode"         , "BiosBoot"               , "0"); 
    lpstData.bMD_BiosS3         = (BOOL)CConfig.GetInt("Mode"         , "BiosS3"                 , "0"); 
    lpstData.bMD_TempMonitor    = (BOOL)CConfig.GetInt("Mode"         , "TempMonitor"            , "0"); 
    lpstData.bMD_ExtentionPwrAGP= (BOOL)CConfig.GetInt("Mode"         , "ExtentionPowerAGP"      , "0");

	lpstData.bMD_SiteOffTestTimeOut = (BOOL)CConfig.GetInt("Site Off", "TestTimeOut", "0");
	lpstData.bMD_SiteOffDefaultBin = (BOOL)CConfig.GetInt("Site Off", "DefaultBin", "0");
	lpstData.bMD_Sort_as_DefaultBin = (BOOL)CConfig.GetInt("Sort", "DefaultBin", "1");

    return 0;
}

int CCokFileManager_Imp::SaveSltPgm(char* szCokName, ST_SLT_PGM* lpstData)
{
	char szCokFullName[512] = {0,};
    //sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s.dat",m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);
    //CConfigData CConfig(szCokFullName);
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\Tester.dat", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe", m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s", m_szCurrentDir, SZ_SLT_PGM_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	CConfigData CConfig(szCokFullName);

    CConfig.Set("Time Out"          , "Boot Time"        , lpstData->nBootTmOut        );
    CConfig.Set("Time Out"          , "Test Time"        , lpstData->nTestTmOut        );
    CConfig.Set("Delay Time"        , "AC Power On"      , lpstData->nAcPowerOnDelayTm );
    CConfig.Set("Delay Time"        , "AC Power Off"     , lpstData->nAcPowerOffDelayTm);
    CConfig.Set("Delay Time"        , "ATX Power On"     , lpstData->nAtxPowerOnDelayTm);
    CConfig.Set("Baud Rate"         , "Baud Rate"        , lpstData->nBaudRate         );
    CConfig.Set("Cooling"           , "Pre Cooling Time" , lpstData->nPreCoolingTm     );    
    CConfig.Set("AGP"               , "AGP Delay Time"   , lpstData->nAgpDelayTm       );    
    CConfig.Set("BIOS Boot"         , "Count"            , lpstData->nBiosBootCnt      );
    CConfig.Set("BIOS Boot"         , "Time Out"         , lpstData->nBiosTmOut        );    
    CConfig.Set("BIOS S3"           , "Count"            , lpstData->nBiosS3Cnt        );
    CConfig.Set("BIOS S3"           , "Time Out"         , lpstData->nBiosS3TmOut      );
    CConfig.Set("BIOS S3"           , "Delay Time"       , lpstData->nBiosS3DelayTm    );    
    CConfig.Set("Temperature"       , "Setting Temp"     , lpstData->nSetTemp          );
    CConfig.Set("Temperature"       , "Guard Band"       , lpstData->nTempGuardBand    );
    CConfig.Set("Min PassTime"      , "Min PassTime"     , lpstData->nMinPassTm        );
    CConfig.Set("Fail Code"         , "Digital"          , lpstData->nFailCodeDigitalNo);
    CConfig.Set("Fail Code"         , "Header"           , lpstData->szFailCodeHeader  );
    CConfig.Set("Pass String"       , "String"           , lpstData->szPassString      );
    CConfig.Set("Line"              , "Start Line"       , lpstData->szStartLine       );
    CConfig.Set("Line"              , "End Line"         , lpstData->szEndLine         );
	CConfig.Set("Delay Time"        , "SotDelay"         , lpstData->nSotDelayTm       );
	CConfig.Set("Not Testing Time Out", "Not Testing Time Out", lpstData->nNotTestingTmOut);

    // mode
    CConfig.Set("Mode"         , "BootTimeOut"            , lpstData->bMD_BootTmOut	    ); 
    CConfig.Set("Mode"         , "PreCooling"             , lpstData->bMD_PreCooling     ); 
    CConfig.Set("Mode"         , "AcPowerOn"              , lpstData->bMD_AcPwrOnDly     ); 
    CConfig.Set("Mode"         , "AcPowerOff"             , lpstData->bMD_AcPwrOff       ); 
    CConfig.Set("Mode"         , "AtxPowerOn"             , lpstData->bMD_AtxPowerOn     ); 
    CConfig.Set("Mode"         , "PassString"             , lpstData->bMD_PassStr        ); 
    CConfig.Set("Mode"         , "FailCodeHeader"         , lpstData->bMD_FailCodeHead   ); 
    CConfig.Set("Mode"         , "FailCodeDigital"        , lpstData->bMD_FailCodeDig    ); 
    CConfig.Set("Mode"         , "StartLine"              , lpstData->bMD_StartLine      ); 
    CConfig.Set("Mode"         , "EndLine"                , lpstData->bMD_EndLine        ); 
    CConfig.Set("Mode"         , "MinPassTime"            , lpstData->bMD_MinPassTm      ); 
    CConfig.Set("Mode"         , "BaudRate"               , lpstData->bMD_BaudRate       ); 
    CConfig.Set("Mode"         , "HangReboot"             , lpstData->bMD_HangReboot     ); 
    CConfig.Set("Mode"         , "BiosBoot"               , lpstData->bMD_BiosBoot       ); 
    CConfig.Set("Mode"         , "BiosS3"                 , lpstData->bMD_BiosS3         ); 
    CConfig.Set("Mode"         , "TempMonitor"            , lpstData->bMD_TempMonitor    ); 
    CConfig.Set("Mode"         , "ExtentionPowerAGP"      , lpstData->bMD_ExtentionPwrAGP);

	CConfig.Set("Site Off", "TestTimeOut", lpstData->bMD_SiteOffTestTimeOut);
	CConfig.Set("Site Off", "DefaultBin", lpstData->bMD_SiteOffDefaultBin);
	CConfig.Set("Sort", "DefaultBin", /*lpstData->bMD_Sort_as_DefaultBin*/TRUE);

    return 0;
}

int CCokFileManager_Imp::LoadSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin& lpstData)
{
	char szCokFullName[512] = { 0, };

	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\DefaultBin.csv", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe", m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s", m_szCurrentDir, SZ_SLT_PGM_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	if (PathFileExists(szCokFullName) == FALSE)
	{
		CString strLastError;
		DWORD dwLastError = GetLastError();
		switch (dwLastError)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		{
			CFile f;
			f.Open(szCokFullName, (CFile::modeCreate | CFile::modeWrite));
			f.SeekToEnd();
			f.Close();
		}break;
		default:
			strLastError.Format("DefaultBin file access denied. (GetLastErrorCode=[%lu])", dwLastError);
			AfxMessageBox(strLastError);
			break;
		}
	}
	CString strGetText = _T("");

	CStdioFile sf(szCokFullName, CFile::modeRead | CFile::shareDenyNone);
	while (sf.ReadString(strGetText) != NULL)
	{
		lpstData.vDefaultBin.push_back(strGetText);
		Sleep(1);
	}
	sf.Close();

	return RETURN_OK;
}

int CCokFileManager_Imp::SaveSltPgmDefaultBin(char* szCokName, ST_Set_DefaultBin* lpstData)
{
	char szCokFullName[512] = { 0, };
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\DefaultBin.csv", m_szCurrentDir, SZ_SLT_PGM_FOLDER, szCokName);

	CFile f;

	f.Open(szCokFullName, CFile::modeCreate);
	f.Close();

	f.Open(szCokFullName, CFile::modeWrite);
	f.SeekToEnd();

	CString strData = _T("");
	int nCount = (int)lpstData->vDefaultBin.size();
	for (int i = 0; i<nCount; i++)
	{
		strData.Format(_T("%s"), lpstData->vDefaultBin[i]);
		strData += "\r\n";
		f.Write(strData, strlen(strData));
	}

	f.Close();

	return 0;
}

int CCokFileManager_Imp::LoadBasicInfo( char* szCokName, ST_BASIC_INFO& lpstData )
{
	char szCokFullName[512] = {0,};
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s\\BasicAction.dat",m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);//, szCokName );

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe",m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s",m_szCurrentDir,SZ_TRAY_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s",m_szCurrentDir,SZ_TRAY_FOLDER,szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}

	CConfigData CConfig(szCokFullName);

	lpstData.bAutoSkipMode      = (BOOL)CConfig.GetInt("User Config"     , "AutoSkip"      , "0");
	lpstData.bContactMode       = (BOOL)CConfig.GetInt("User Config"     , "Contact"       , "0");
	lpstData.bTempMode          = (BOOL)CConfig.GetInt("User Config"     , "Temperature"   , "0");
    lpstData.nVision		    = CConfig.GetInt("User Config"           , "TestSiteVision", "0");

	lpstData.nAutoSkipCount      = (BOOL)CConfig.GetInt("User Config"     , "AutoSkipCount"      , "0");
							    
	lpstData.bMD_TestSite_Retouch = (BOOL)CConfig.GetInt("User Config", "TestSiteRetouch", "0");
	lpstData.dTestSite_Retouch_Dis = CConfig.GetInt("User Config", "TestSiteRetouch_Distance", "0");

// 	lpstData.bContinueFailMode  = (BOOL)CConfig.GetInt("Continue Fail"   , "Use Mode"		, "0");
// 	lpstData.nContiFailCnt      = CConfig.GetInt("Continue Fail"		 , "Fail Count"     , "0");
// 	lpstData.nAContiResetCnt     = CConfig.GetInt("Continue Fail"		 , "Reset Count"    , "0");
								 									 
	lpstData.nDropHeight        = CConfig.GetInt("Drop Mode"			 , "Drop Height"   , "0");
	lpstData.nDropTimer         = CConfig.GetInt("Drop Mode"			 , "Drop Timer"    , "0");
								 									 
	lpstData.nOffLineTestTm	    = CConfig.GetInt("Off Line"			     , "Test Time"     , "0");
	lpstData.bOffLineBinRandom  = (BOOL)CConfig.GetInt("Off Line"		 , "BinRandom"	   , "0");
	

	lpstData.nRunMode		    = CConfig.GetInt("Machine Config"	     , "Run Mode"      , "0");
	lpstData.nStartMode		    = CConfig.GetInt("Machine Config"	     , "Start Mode"    , "0");
    lpstData.nFTRTQC		    = CConfig.GetInt("Machine Config"	     , "Lot Control"    , "0");

	lpstData.nContactForcePins  = CConfig.GetInt("Contact Force"		 , "Pin Ball"	   , "0");
	lpstData.dContactcForceGF   = CConfig.GetDouble("Contact Force"      , "GF"			   , "0");
	lpstData.bAutoRetestSiteUse = CConfig.GetInt("Machine Config"        , "Auto Retest Test PP", "0");
    lpstData.nAutoRetestKind    = CConfig.GetInt("Machine Config"        , "Auto Retest Kind Test PP", "0");
	lpstData.nDeviceMode        = CConfig.GetInt("Machine Config"        , "Device Mode"        , "0");

	
	// Vibrator
	lpstData.nVibUdDvcActCount			= CConfig.GetInt("Vibrator"		 , "UnLoad Device Action Count", "0");
	lpstData.dVibActDelayTime			= CConfig.GetDouble("Vibrator"	 , "Action Delay Time", "0.0");
	lpstData.nVibActCnt				    = CConfig.GetInt("Vibrator"		 , "Active Count", "0");
	lpstData.nPressVibratorChkOpt = CConfig.GetInt("Vibrator", "Vibrator Option",  "0");
	lpstData.nPressVibratorRetryCnt = CConfig.GetInt("Vibrator", "Vibrator Retry Count", "0");
	lpstData.dPressVibratorDelayTime = CConfig.GetDouble("Vibrator", "Vibrator Delay Time", "0.0");
	// Alway Vibrator
	lpstData.nPressAlwayVibratorChkOpt = CConfig.GetInt("Vibrator", "Alway Vibrator Option", "0");
	lpstData.nPressAlwayVibratorRetryCnt = CConfig.GetInt("Vibrator", "Alway Vibrator Retry Count", "0");
	lpstData.dPressAlwayVibratorDelayTime = CConfig.GetDouble("Vibrator", "Alway Vibrator Delay Time", "0.0");

	// Yield Monitoring
	lpstData.nContinueFailureCnt  = CConfig.GetInt("Yield Monitoring"	, "Continue Failure Count", "0");
	lpstData.bYieldMonitor        = (BOOL)CConfig.GetInt("Yield Monitoring"   , "Yield Monitor", "0");
	lpstData.bEventType           = (BOOL)CConfig.GetInt("Yield Monitoring"   , "Event Type", "0");
	lpstData.bRetestYield         = (BOOL)CConfig.GetInt("Yield Monitoring"   , "Retest Yield", "0");
	lpstData.nSampleQuantity	  = CConfig.GetInt("Yield Monitoring"   , "Sample Quantity", "0");
	lpstData.nPresetCounter		  = CConfig.GetInt("Yield Monitoring"   , "Preset Counter", "0");
	lpstData.dLowYield			  =	CConfig.GetDouble("Yield Monitoring", "Low Yield", "0");
	lpstData.dVariance            =	CConfig.GetDouble("Yield Monitoring", "Variance", "0");
	lpstData.nContinueFailureCnt_Station = CConfig.GetInt("Yield Monitoring", "Continue Failure Count Station", "0");

    lpstData.bMD_Autokill         = (BOOL)CConfig.GetInt("Yield Monitoring", "ModeAutoKill", "0");
	lpstData.bMD_Consecutive_Fail_AutoKill = (BOOL)CConfig.GetInt("Yield Monitoring", "Consecutive Fail AutoKill", "0");
	lpstData.bMD_Consecutive_Fail_AutoKill_Station = (BOOL)CConfig.GetInt("Yield Monitoring", "Consecutive Fail AutoKill Station", "0");
	lpstData.bMD_1stFailAutokill = (BOOL)CConfig.GetInt("Yield Monitoring", "1st Fail AutoKill", "0");

	lpstData.bMD_VisionExist_AAB  = (BOOL)CConfig.GetInt("Machine Config"  , "ExistAAB"    , "0");	
	lpstData.bMD_TestPP_DevExist_Check = (BOOL)CConfig.GetInt("Machine Config","ExistTestPP", "0");
	lpstData.bMD_TestPP_After_DevExist_Check = (BOOL)CConfig.GetInt("Machine Config", "After ExistTestPP", "0");
	lpstData.bMD_TestPP_DevExist_Check_Sensor = (BOOL)CConfig.GetInt("Machine Config","ExistTestPP_Sensor", "0");
	lpstData.bPressDeviceChk	  = (BOOL)CConfig.GetInt("Machine Config", "PressDeviceCheck", "1"); //기본 값 사용함.
	lpstData.nPressDeviceChkOpt	  = (BOOL)CConfig.GetInt("Machine Config", "PressDeviceCheckOption", "0"); //기본 값 All Socket
	lpstData.bMD_TestPP_ArtDevice_PickPlace = (BOOL)CConfig.GetInt("Machine Config","TestPP_ArtDevice_PickPlace", "1");
	lpstData.bMD_TestPP_PreVacuum = (BOOL)CConfig.GetInt("Machine Config", "TestPP_PreVacuum", "0");
	lpstData.bMD_TrayPP_PreVacuum = (BOOL)CConfig.GetInt("Machine Config", "TrayPP_PreVacuum", "0");
	lpstData.bMD_TestPP_PNP_SiteArrived_Sen = (BOOL)CConfig.GetInt("Machine Config", "PnPSiteArrivedSen", "0");

	lpstData.bMD_TestPP_PNP_SiteMoveArrived_Sen = (BOOL)CConfig.GetInt("Machine Config", "PnPSiteMoveArrivedSen", "0");
	lpstData.bMD_TestppStationFR_PNP = (BOOL)CConfig.GetInt("Machine Config", "TestppStationFR_PNP", "0");
	lpstData.bMD_LoadHandAlreadyPick = (BOOL)CConfig.GetInt("Machine Config", "TestppLoadHandAlreadyPick", "0");

	//?? lpstData.nPressVibratorChkOpt = (BOOL)CConfig.GetInt("Machine Config", "PressVibratorOption", "1"); //기본

    lpstData.bLoadTblArrived      = CConfig.GetInt("Load Table", "ArrivedSen", "0");
	lpstData.bLoadTblAfterMoveArrived = CConfig.GetInt("Load Table", "AfterArrivedSen", "0");
	lpstData.bImpactTbl		      = CConfig.GetInt("Load Table", "ImpactTbl", "0");
//	lpstData.bMD_Dvc_HorVer       = CConfig.GetInt("Load Table", "HorVerType", "0");
	lpstData.b2DTestSiteMove	  = CConfig.GetInt("Load Table", "2D Test Site Move", "0");

	lpstData.nBarcodeRetryCnt     = CConfig.GetInt("Load Table", "Barcode Retry", "0");
	lpstData.nLoadTableVibratorChkOpt = CConfig.GetInt("Load Table", "Vibrator Option", "0");
	lpstData.nLoadTableVibratorRetryCnt = CConfig.GetInt("Load Table", "Vibrator Retry Count", "0");
	lpstData.dLoadTableVibratorDelayTime = CConfig.GetDouble("Load Table", "Vibrator Delay Time", "0.0");
	lpstData.dLoadTableWaitTime = CConfig.GetDouble("Load Table", "Wait Time", "0.0");
	lpstData.nNumberOfLoadTable = CConfig.GetInt("Load Table", "Number Of LoadTable", "0");

	lpstData.nDivision = CConfig.GetInt("Station", "Div_Y", "2");

	char szSubTitle[ 256 ] = { 0, };
	char szSocketOnOff[ 2056 ] = { 0, };
	//CString SocketOnOff = _T( "" );
	//CString tkn = _T( "" );
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++ )
	{
		ZeroMemory( szSubTitle, sizeof( szSubTitle ) );
		sprintf_s( szSubTitle, sizeof( szSubTitle ), "Station%d_SocketOnOff", nStation + 1 );
		CConfig.GetString( "Machine Config", szSubTitle, "0,0,0,0,0,0,0,0", szSocketOnOff );

		CParamList PrmList;
		PrmList.m_fnSetDelimiter( ",", 1 );
		PrmList.m_fnSetDelimitedText( szSocketOnOff, strlen( szSocketOnOff ) );
		int nPosCount = PrmList.m_fnGetCount();
		int index = 0;

		//int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

		for( int nCol = 0; nCol < 2; nCol++ )
		{
			for( int nRow = 0; nRow < 4; nRow++ )
			{
				if (lpstData.nDivision == 1)
				{
					if (nStation <= ePressUnit_Down_4)
						lpstData.nSiteOnOff[nStation][nRow][abs(nCol)] = atoi(PrmList.m_fnGetItemString(index++));						
					else
						lpstData.nSiteOnOff[nStation][abs(nRow - 3)][nCol] = atoi(PrmList.m_fnGetItemString(index++));
				}
				else {
					if (nStation <= ePressUnit_Down_4)
						lpstData.nSiteOnOff[nStation][nRow][abs(nCol - 1)] = atoi(PrmList.m_fnGetItemString(index++));					
					else
						lpstData.nSiteOnOff[nStation][abs(nRow - 3)][nCol] = atoi(PrmList.m_fnGetItemString(index++));
// 					if (nStation <= ePressUnit_Down_4)
// 						lpstData.nSiteOnOff[nStation][abs(nRow - 3)][nCol] = atoi(PrmList.m_fnGetItemString(index++));
// 					else
//  						lpstData.nSiteOnOff[nStation][nRow][abs(nCol - 1)] = atoi(PrmList.m_fnGetItemString(index++));
				}
				
			}
		}
	}

// 	lpstData.bYieldControlMode = (BOOL)CConfig.GetInt("Yield Control", "Use Mode" , "0");
// 	lpstData.nYield_Cnt        = CConfig.GetInt("Yield Control"      , "Count"    , "0");
// 	lpstData.dYield_Percent    = CConfig.GetDouble("Yield Control"   , "Percent"  , "0");

	CConfig.GetString("Category"      , "Name"  , "Test", lpstData.szCategoryName);
	CConfig.GetString("SLT PGM"       , "Name"  , "Test", lpstData.szSltPgmName);

	CConfig.GetString("Lot Info"      , "LotNo"             , "None", lpstData.szLotNo);
	CConfig.GetString("Lot Info"      , "ScheduleNo"        , "None", lpstData.szScheduleNo);
	CConfig.GetString("Lot Info"      , "OperID"            , "None", lpstData.szOperID);
	CConfig.GetString("Lot Info"      , "DeviceName"        , "None", lpstData.szDeviceName);
	CConfig.GetString("Lot Info"      , "PgmName"           , "None", lpstData.szPgmName);
	CConfig.GetString("Lot Info"      , "CustomerName"      , "None", lpstData.szCustomerName);

	lpstData.nDvcDimensionX = CConfig.GetInt("Device Info"      , "Dimension X"  , "1");
	lpstData.nDvcDimensionY = CConfig.GetInt("Device Info"      , "Dimension Y"  , "1");

    // socket cleaning
    lpstData.bMD_SocketClean            = CConfig.GetInt("Socket Clean"     , "MD_SocketClean"         , "0");
    lpstData.bMD_CleanLoadTblArrivedSen = CConfig.GetInt("Socket Clean"     , "MD_CleanLoadTblArrived" , "0");
    lpstData.nCleanContacTm             = CConfig.GetInt("Socket Clean"     , "SockClnContactTm"       , "0");
    lpstData.nCleanContacCnt            = CConfig.GetInt("Socket Clean"     , "SockClnContactCnt"      , "0");
    lpstData.nCleanAlarmCnt             = CConfig.GetInt("Socket Clean"     , "SockClnAlarmCnt"        , "0");
    lpstData.nCleanPiecesCnt            = CConfig.GetInt("Socket Clean"     , "SockClnPiecesCnt"       , "0");
    lpstData.nCleanIntervalCnt          = CConfig.GetInt("Socket Clean"     , "SockClnIntervalCnt"     , "0");
    lpstData.bMD_CleanInitStart         = CConfig.GetInt("Socket Clean"     , "MD_CleanInitStart"      , "0");
    lpstData.bMD_CleanInitRetestStart   = CConfig.GetInt("Socket Clean"     , "MD_CleanInitRetestStart", "0");
    lpstData.bMD_CleanIntervalCnt       = CConfig.GetInt("Socket Clean"     , "MD_CleanIntervalCnt"    , "0");
    lpstData.bMD_CleanLowYield          = CConfig.GetInt("Socket Clean"     , "MD_CleanLowYield"       , "0");
    lpstData.dCleanLowYield             = CConfig.GetDouble("Socket Clean"  , "SockClnLowYield"        , "0");
	lpstData.bMD_EventType              = CConfig.GetInt("Socket Clean"     , "MD_EventType"           , "0");
	lpstData.dContactDelayTime =		CConfig.GetDouble("Socket Clean", "SockClnContactDelayTime", "0");
	lpstData.dCleaning2ndZOffset		= CConfig.GetDouble("Socket Clean", "2nd Contact Z Offset", "0");

	// GRR OPTION
	lpstData.nAutoGrrTestCount = CConfig.GetInt( "GRR", "TestCount", "0" );
	lpstData.nAutoGrrUnitCount = CConfig.GetInt( "GRR", "UnitCount", "0" );
	lpstData.nGrrSotDelay = CConfig.GetInt("GRR", "SOTDelayTime", "0");
	lpstData.bGRR_Device_swap = ( BOOL )CConfig.GetInt( "GRR", "Device_swap", "0" );
	lpstData.bGRR_Audit_loop = ( BOOL )CConfig.GetInt( "GRR", "Audit_loop", "0" );
	lpstData.nAutoGrrMode = CConfig.GetInt("GRR", "AutoGrrMode", "0");

	char szGRRSubTitle[ 256 ] = { 0, };
	char szGRRSocketOnOff[ 2056 ] = { 0, };
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++ ) {
		ZeroMemory( szGRRSubTitle, sizeof( szGRRSubTitle ) );
		sprintf_s( szGRRSubTitle, sizeof( szGRRSubTitle ), "GRRStation%d_SocketOnOff", nStation + 1 );
		CConfig.GetString( "GRR", szGRRSubTitle, "0,0,0,0,0,0,0,0", szGRRSocketOnOff );

		CParamList PrmList;
		PrmList.m_fnSetDelimiter( ",", 1 );
		PrmList.m_fnSetDelimitedText( szGRRSocketOnOff, strlen( szGRRSocketOnOff ) );
		int nPosCount = PrmList.m_fnGetCount();
		int index = 0;
		for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ ) 
		{
			for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ ) 
			{
				if( index < nPosCount )
					lpstData.nGRRSiteOnOff[ nStation ][ nY ][ nX ] = atoi( PrmList.m_fnGetItemString( index++ ) );
				else
					lpstData.nGRRSiteOnOff[ nStation ][ nY ][ nX ] = 0;
			}
		}
	}

	// QAMode 
	lpstData.nAutoQADefaultCount = CConfig.GetInt("QA", "DefaultCount", "0");

	// Loop OPTION

	lpstData.nAutoLoopTestCount = CConfig.GetInt("Loop", "TestCount", "0");
	lpstData.nAutoLoopUnitCount = CConfig.GetInt("Loop", "UnitCount", "0");
	lpstData.nLoopSotDelay = CConfig.GetInt("Loop", "SOTDelayTime", "0");
	lpstData.nAutoLoopmode = CConfig.GetInt("Loop", "AutoLoopMode", "0");
	lpstData.nAutoLoopRePickPlaceCount = CConfig.GetInt("Loop", "AutoLoopRePickPlaceCount", "0");
	lpstData.nAutoLoopReContactCount = CConfig.GetInt("Loop", "AutoLoopReContactCount", "0");



	// AuditMode 

	lpstData.nAutoAuditTestCount = CConfig.GetInt("Audit", "TestCount", "0");
	lpstData.nAutoAuditUnitCount = CConfig.GetInt("Audit", "UnitCount", "0");
	lpstData.nAuditSotDelay = CConfig.GetInt("Audit", "SOTDelayTime", "0");
	lpstData.nAutoAuditMode = CConfig.GetInt("Audit", "AutoAuditMode", "0");

	char szAuditSubTitle[256] = { 0, };
	char szAuditSocketOnOff[2056] = { 0, };
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		ZeroMemory(szAuditSubTitle, sizeof(szAuditSubTitle));
		sprintf_s(szAuditSubTitle, sizeof(szAuditSubTitle), "AuditStation%d_SocketOnOff", nStation + 1);
		CConfig.GetString("Audit", szAuditSubTitle, "0,0,0,0,0,0,0,0", szAuditSocketOnOff);

		CParamList PrmList;
		PrmList.m_fnSetDelimiter(",", 1);
		PrmList.m_fnSetDelimitedText(szAuditSocketOnOff, strlen(szAuditSocketOnOff));
		int nPosCount = PrmList.m_fnGetCount();
		int index = 0;
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				if (index < nPosCount)
					lpstData.nAuditSiteOnOff[nStation][nY][nX] = atoi(PrmList.m_fnGetItemString(index++));
				else
					lpstData.nAuditSiteOnOff[nStation][nY][nX] = 0;
			}
		}
	}



	return 0;
}

int CCokFileManager_Imp::SaveBasicInfo( char* szCokName, ST_BASIC_INFO* lpstData )
{
	char szCokFullName[ 512 ] = { 0, };
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\handlerRecipe\\%s\\%s\\BasicAction.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName );//, szCokName );
	CConfigData CConfig( szCokFullName );

	CConfig.Set( "User Config", "AutoSkip", ( int )lpstData->bAutoSkipMode );
	CConfig.Set( "User Config", "Contact", ( int )lpstData->bContactMode );
	CConfig.Set( "User Config", "Temperature", ( int )lpstData->bTempMode );
	CConfig.Set( "User Config", "TestSiteVision", ( int )lpstData->nVision );
	
	CConfig.Set("User Config", "TestSiteRetouch", (int)lpstData->bMD_TestSite_Retouch);
	CConfig.Set("User Config", "TestSiteRetouch_Distance", (int)lpstData->dTestSite_Retouch_Dis);

	CConfig.Set( "User Config", "AutoSkipCount", lpstData->nAutoSkipCount );

	// 	CConfig.Set("Continue Fail"  , "Use Mode"		      , (int)lpstData->bContinueFailMode);
	// 	CConfig.Set("Continue Fail"  , "Fail Count"           , lpstData->nContiFailCnt      );
	// 	CConfig.Set("Continue Fail"  , "Reset Count"          , lpstData->nContiResetCnt     );

	CConfig.Set( "Drop Mode", "Drop Height", lpstData->nDropHeight );
	CConfig.Set( "Drop Mode", "Drop Timer", lpstData->nDropTimer );

	CConfig.Set( "Contact Force", "Pin Ball", lpstData->nContactForcePins );
	CConfig.Set( "Contact Force", "GF", lpstData->dContactcForceGF );

	CConfig.Set( "Off Line", "Test Time", lpstData->nOffLineTestTm );
	CConfig.Set("Off Line", "BinRandom", lpstData->bOffLineBinRandom);

	CConfig.Set( "Machine Config", "Run Mode", lpstData->nRunMode );
	CConfig.Set( "Machine Config", "Start Mode", lpstData->nStartMode );
	CConfig.Set( "Machine Config", "Lot Control", lpstData->nFTRTQC );
	CConfig.Set( "Machine Config", "Device Mode", lpstData->nDeviceMode );
	CConfig.Set( "Machine Config", "Auto Retest Test PP", lpstData->bAutoRetestSiteUse );
	CConfig.Set( "Machine Config", "Auto Retest Kind Test PP", lpstData->nAutoRetestKind );

	// Vibrator
	CConfig.Set( "Vibrator", "UnLoad Device Action Count", lpstData->nVibUdDvcActCount );
	CConfig.Set( "Vibrator", "Action Delay Time", lpstData->dVibActDelayTime );
	CConfig.Set( "Vibrator", "Active Count", lpstData->nVibActCnt );
	CConfig.Set("Vibrator", "Vibrator Option", lpstData->nPressVibratorChkOpt);
	CConfig.Set("Vibrator", "Vibrator Retry Count", lpstData->nPressVibratorRetryCnt);
	CConfig.Set("Vibrator", "Vibrator Delay Time", lpstData->dPressVibratorDelayTime);
	//Alway Vibrator
	CConfig.Set("Vibrator", "Alway Vibrator Option", lpstData->nPressAlwayVibratorChkOpt);
	CConfig.Set("Vibrator", "Alway Vibrator Retry Count", lpstData->nPressAlwayVibratorRetryCnt);
	CConfig.Set("Vibrator", "Alway Vibrator Delay Time", lpstData->dPressAlwayVibratorDelayTime);
	

	// Yield Monitoring
	CConfig.Set( "Yield Monitoring", "Continue Failure Count", lpstData->nContinueFailureCnt );
	CConfig.Set( "Yield Monitoring", "Yield Monitor", lpstData->bYieldMonitor );
	CConfig.Set( "Yield Monitoring", "Event Type", lpstData->bEventType );
	CConfig.Set( "Yield Monitoring", "Retest Yield", lpstData->bRetestYield );
	CConfig.Set( "Yield Monitoring", "Sample Quantity", lpstData->nSampleQuantity );
	CConfig.Set( "Yield Monitoring", "Preset Counter", lpstData->nPresetCounter );
	CConfig.Set( "Yield Monitoring", "Low Yield", lpstData->dLowYield );
	CConfig.Set( "Yield Monitoring", "Variance", lpstData->dVariance );
	CConfig.Set( "Yield Monitoring", "Continue Failure Count Station", lpstData->nContinueFailureCnt_Station);
	
	// yield auto socket off (auto kill)
	CConfig.Set( "Yield Monitoring", "ModeAutoKill", lpstData->bMD_Autokill );
	CConfig.Set( "Yield Monitoring", "Consecutive Fail AutoKill", lpstData->bMD_Consecutive_Fail_AutoKill);
	CConfig.Set( "Yield Monitoring", "Consecutive Fail AutoKill Station", lpstData->bMD_Consecutive_Fail_AutoKill_Station);
	CConfig.Set("Yield Monitoring", "1st Fail AutoKill", lpstData->bMD_1stFailAutokill);

	CConfig.Set( "Machine Config", "ExistAAB", lpstData->bMD_VisionExist_AAB );
	CConfig.Set( "Machine Config", "ExistTestPP", lpstData->bMD_TestPP_DevExist_Check );
	CConfig.Set("Machine Config", "After ExistTestPP", lpstData->bMD_TestPP_After_DevExist_Check);
	CConfig.Set( "Machine Config", "ExistTestPP_Sensor", lpstData->bMD_TestPP_DevExist_Check_Sensor );
	CConfig.Set( "Machine Config", "PressDeviceCheck", lpstData->bPressDeviceChk );
	CConfig.Set( "Machine Config", "PressDeviceCheckOption", lpstData->nPressDeviceChkOpt );
	CConfig.Set( "Machine Config", "TestPP_ArtDevice_PickPlace", lpstData->bMD_TestPP_ArtDevice_PickPlace );
	CConfig.Set( "Machine Config", "TestPP_PreVacuum", lpstData->bMD_TestPP_PreVacuum);
	CConfig.Set( "Machine Config", "TrayPP_PreVacuum", lpstData->bMD_TrayPP_PreVacuum);
	CConfig.Set("Machine Config", "PnPSiteArrivedSen", lpstData->bMD_TestPP_PNP_SiteArrived_Sen);
	CConfig.Set("Machine Config", "PnPSiteMoveArrivedSen", lpstData->bMD_TestPP_PNP_SiteMoveArrived_Sen);
	CConfig.Set("Machine Config", "TestppStationFR_PNP", lpstData->bMD_TestppStationFR_PNP);
	CConfig.Set("Machine Config", "TestppLoadHandAlreadyPick", lpstData->bMD_LoadHandAlreadyPick);


	// arrived sensor
	CConfig.Set( "Load Table", "ArrivedSen", lpstData->bLoadTblArrived );
	CConfig.Set("Load Table", "AfterArrivedSen", lpstData->bLoadTblAfterMoveArrived);
	CConfig.Set( "Load Table", "ImpactTbl", lpstData->bImpactTbl );
	//    CConfig.Set("Load Table"  , "HorVerType"   , lpstData->bMD_Dvc_HorVer);
	CConfig.Set("Load Table", "2D Test Site Move", lpstData->b2DTestSiteMove);
	CConfig.Set("Load Table", "Barcode Retry", lpstData->nBarcodeRetryCnt);
	CConfig.Set("Load Table", "Vibrator Option", lpstData->nLoadTableVibratorChkOpt);
	CConfig.Set("Load Table", "Vibrator Retry Count", lpstData->nLoadTableVibratorRetryCnt);
	CConfig.Set("Load Table", "Vibrator Delay Time", lpstData->dLoadTableVibratorDelayTime);
	CConfig.Set("Load Table", "Wait Time", lpstData->dLoadTableWaitTime);
	CConfig.Set("Load Table", "Number Of LoadTable", lpstData->nNumberOfLoadTable);

	CConfig.Set("Station", "Div_Y", lpstData->nDivision);

	char szSubTitle[ 256 ] = { 0, };
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++ ) {
		ZeroMemory( szSubTitle, sizeof( szSubTitle ) );
		sprintf_s( szSubTitle, sizeof( szSubTitle ), "Station%d_SocketOnOff", nStation + 1 );
		CString SocketOnOff = _T( "" );
		CString tmp = _T( "" );
		for( int nCol = 0; nCol < 2; nCol++ )
		{
			for( int nRow = 0; nRow < 4; nRow++ )
			{
				if (lpstData->nDivision == 1)
				{
					if (nStation <= ePressUnit_Down_4)
						tmp.Format(_T("%d,"), lpstData->nSiteOnOff[nStation][nRow][abs(nCol)]);					
					else
						tmp.Format(_T("%d,"), lpstData->nSiteOnOff[nStation][abs(nRow - 3)][nCol]);
				}
				else {
					if (nStation <= ePressUnit_Down_4)
						tmp.Format(_T("%d,"), lpstData->nSiteOnOff[nStation][nRow][abs(nCol - 1)]);						
					else
						tmp.Format(_T("%d,"), lpstData->nSiteOnOff[nStation][abs(nRow - 3)][nCol]);
// 					if (nStation <= ePressUnit_Down_4)
// 						tmp.Format(_T("%d,"), lpstData->nSiteOnOff[nStation][abs(nRow - 3)][nCol]);
// 					else
// 						tmp.Format(_T("%d,"), lpstData->nSiteOnOff[nStation][nRow][abs(nCol - 1)]);
				}

				SocketOnOff += tmp;
			}
		}
		SocketOnOff.TrimRight( "," );
		CConfig.Set( "Machine Config", szSubTitle, ( LPSTR )( LPCTSTR )SocketOnOff );
	}

	// 	CConfig.Set("Yield Control" , "Use Mode" , (int)lpstData->bYieldControlMode);
	// 	CConfig.Set("Yield Control" , "Count"    , lpstData->nYield_Cnt       );
	// 	CConfig.Set("Yield Control" , "Percent"  , lpstData->dYield_Percent   );

	CConfig.Set( "Category", "Name", lpstData->szCategoryName );
	CConfig.Set( "SLT PGM", "Name", lpstData->szSltPgmName );

	CConfig.Set( "Lot Info", "LotNo", lpstData->szLotNo );
	CConfig.Set( "Lot Info", "ScheduleNo", lpstData->szScheduleNo );
	CConfig.Set( "Lot Info", "OperID", lpstData->szOperID );
	CConfig.Set( "Lot Info", "DeviceName", lpstData->szDeviceName );
	CConfig.Set( "Lot Info", "PgmName", lpstData->szPgmName );
	CConfig.Set( "Lot Info", "CustomerName", lpstData->szCustomerName );

	CConfig.Set( "Device Info", "Dimension X", lpstData->nDvcDimensionX );
	CConfig.Set( "Device Info", "Dimension Y", lpstData->nDvcDimensionY );

	// socket cleaning
	CConfig.Set( "Socket Clean", "MD_SocketClean", lpstData->bMD_SocketClean );
	CConfig.Set( "Socket Clean", "MD_CleanLoadTblArrived", lpstData->bMD_CleanLoadTblArrivedSen );
	CConfig.Set( "Socket Clean", "SockClnContactTm", lpstData->nCleanContacTm );
	CConfig.Set( "Socket Clean", "SockClnContactCnt", lpstData->nCleanContacCnt );
	CConfig.Set( "Socket Clean", "SockClnAlarmCnt", lpstData->nCleanAlarmCnt );
	CConfig.Set( "Socket Clean", "SockClnPiecesCnt", lpstData->nCleanPiecesCnt );
	CConfig.Set( "Socket Clean", "SockClnIntervalCnt", lpstData->nCleanIntervalCnt );
	CConfig.Set( "Socket Clean", "MD_CleanInitStart", lpstData->bMD_CleanInitStart );
	CConfig.Set( "Socket Clean", "MD_CleanInitRetestStart", lpstData->bMD_CleanInitRetestStart );
	CConfig.Set( "Socket Clean", "MD_CleanIntervalCnt", lpstData->bMD_CleanIntervalCnt );
	CConfig.Set( "Socket Clean", "MD_CleanLowYield", lpstData->bMD_CleanLowYield );
	CConfig.Set( "Socket Clean", "SockClnLowYield", lpstData->dCleanLowYield );
	CConfig.Set( "Socket Clean", "MD_EventType", lpstData->bMD_EventType );
	CConfig.Set("Socket Clean", "SockClnContactDelayTime", lpstData->dContactDelayTime);
	CConfig.Set("Socket Clean", "2nd Contact Z Offset", lpstData->dCleaning2ndZOffset);

	//GRR
	CConfig.Set( "GRR", "TestCount", lpstData->nAutoGrrTestCount );
	CConfig.Set( "GRR", "UnitCount", lpstData->nAutoGrrUnitCount );
	CConfig.Set("GRR", "SOTDelayTime", lpstData->nGrrSotDelay);
	CConfig.Set( "GRR", "Device_swap", lpstData->bGRR_Device_swap );
	CConfig.Set( "GRR", "Audit_loop", lpstData->bGRR_Audit_loop );
	CConfig.Set("GRR", "AutoGrrMode", lpstData->nAutoGrrMode);

	char szGRRSubTitle[ 256 ] = { 0, };
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++ ) {
		ZeroMemory( szGRRSubTitle, sizeof( szGRRSubTitle ) );
		sprintf_s( szGRRSubTitle, sizeof( szGRRSubTitle ), "GRRStation%d_SocketOnOff", nStation + 1 );
		CString GRRSocketOnOff = _T( "" );
		CString GRRtmp = _T( "" );
		for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ ) {
			for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ ) {
				GRRtmp.Format( _T( "%d," ), lpstData->nGRRSiteOnOff[ nStation ][ nY ][ nX ] );
				GRRSocketOnOff += GRRtmp;
			}
		}
		GRRSocketOnOff.TrimRight( "," );
		CConfig.Set( "GRR", szGRRSubTitle, ( LPSTR )( LPCTSTR )GRRSocketOnOff );
	}

	// QAMode 
	CConfig.Set("QA", "DefaultCount", lpstData->nAutoQADefaultCount);

	// LoopMode 
	CConfig.Set("Loop", "TestCount", lpstData->nAutoLoopTestCount);
	CConfig.Set("Loop", "UnitCount", lpstData->nAutoLoopUnitCount);
	CConfig.Set("Loop", "SOTDelayTime", lpstData->nLoopSotDelay);
	CConfig.Set("Loop", "AutoLoopMode", lpstData->nAutoLoopmode);
	CConfig.Set("Loop", "AutoLoopRePickPlaceCount", lpstData->nAutoLoopRePickPlaceCount);
	CConfig.Set("Loop", "AutoLoopReContactCount", lpstData->nAutoLoopReContactCount);

	// AuditMode 

	CConfig.Set("Audit", "TestCount", lpstData->nAutoAuditTestCount);
	CConfig.Set("Audit", "UnitCount", lpstData->nAutoAuditUnitCount);
	CConfig.Set("Audit", "SOTDelayTime", lpstData->nAuditSotDelay);
	CConfig.Set("Audit", "AutoAuditMode", lpstData->nAutoAuditMode);

	char szAuditSubTitle[256] = { 0, };
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		ZeroMemory(szAuditSubTitle, sizeof(szAuditSubTitle));
		sprintf_s(szAuditSubTitle, sizeof(szAuditSubTitle), "AuditStation%d_SocketOnOff", nStation + 1);
		CString AuditSocketOnOff = _T("");
		CString Audittmp = _T("");
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++) {
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++) {
				Audittmp.Format(_T("%d,"), lpstData->nAuditSiteOnOff[nStation][nY][nX]);
				AuditSocketOnOff += Audittmp;
			}
		}
		AuditSocketOnOff.TrimRight(",");
		CConfig.Set("Audit", szAuditSubTitle, (LPSTR)(LPCTSTR)AuditSocketOnOff);
	}

	
	return 0;
}

int CCokFileManager_Imp::SetCurrentDirectoryDLL( char* szExeDir )
{
	strcpy_s(m_szCurrentDir,sizeof(m_szCurrentDir), szExeDir);
	return 0;
}


int CCokFileManager_Imp::SaveSignalTower( ST_SLT_SIGNAL_TOWER* lpstData )
{
	char szDir[MAX_PATH] = {0,};
	sprintf_s( szDir, sizeof( szDir ), "%s\\sys_data\\SignalTower.ini",m_szCurrentDir);
	CConfigData CConfig(szDir);
	
	CConfig.Set("Running", "Priority"        , lpstData->nTower[eSIGNAL_RUNNING][eSIGNAL_PRIORTY]);
	CConfig.Set("Running", "Red"             , lpstData->nTower[eSIGNAL_RUNNING][eSIGNAL_RED]);
	CConfig.Set("Running", "Yellow"          , lpstData->nTower[eSIGNAL_RUNNING][eSIGNAL_YELLOW]);
	CConfig.Set("Running", "Green"           , lpstData->nTower[eSIGNAL_RUNNING][eSIGNAL_GREEN]);
	CConfig.Set("Running", "Sound"           , lpstData->nTower[eSIGNAL_RUNNING][eSIGNAL_SOUND]);
	CConfig.Set("Jam Call", "Priority"       , lpstData->nTower[eSIGNAL_JAM_CALL][eSIGNAL_PRIORTY]);
	CConfig.Set("Jam Call", "Red"            , lpstData->nTower[eSIGNAL_JAM_CALL][eSIGNAL_RED]);
	CConfig.Set("Jam Call", "Yellow"         , lpstData->nTower[eSIGNAL_JAM_CALL][eSIGNAL_YELLOW]);
	CConfig.Set("Jam Call", "Green"          , lpstData->nTower[eSIGNAL_JAM_CALL][eSIGNAL_GREEN]);
	CConfig.Set("Jam Call", "Sound"          , lpstData->nTower[eSIGNAL_JAM_CALL][eSIGNAL_SOUND]);
	CConfig.Set("Cycle Stop", "Priority"     , lpstData->nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_PRIORTY]);
	CConfig.Set("Cycle Stop", "Red"          , lpstData->nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_RED]);
	CConfig.Set("Cycle Stop", "Yellow"       , lpstData->nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_YELLOW]);
	CConfig.Set("Cycle Stop", "Green"        , lpstData->nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_GREEN]);
	CConfig.Set("Cycle Stop", "Sound"        , lpstData->nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_SOUND]);
	CConfig.Set("Finish", "Priority"         , lpstData->nTower[eSIGNAL_FINISH][eSIGNAL_PRIORTY]);
	CConfig.Set("Finish", "Red"              , lpstData->nTower[eSIGNAL_FINISH][eSIGNAL_RED]);
	CConfig.Set("Finish", "Yellow"           , lpstData->nTower[eSIGNAL_FINISH][eSIGNAL_YELLOW]);
	CConfig.Set("Finish", "Green"            , lpstData->nTower[eSIGNAL_FINISH][eSIGNAL_GREEN]);
	CConfig.Set("Finish", "Sound"            , lpstData->nTower[eSIGNAL_FINISH][eSIGNAL_SOUND]);
	CConfig.Set("Reset", "Priority"          , lpstData->nTower[eSIGNAL_RESET][eSIGNAL_PRIORTY]);
	CConfig.Set("Reset", "Red"               , lpstData->nTower[eSIGNAL_RESET][eSIGNAL_RED]);
	CConfig.Set("Reset", "Yellow"            , lpstData->nTower[eSIGNAL_RESET][eSIGNAL_YELLOW]);
	CConfig.Set("Reset", "Green"             , lpstData->nTower[eSIGNAL_RESET][eSIGNAL_GREEN]);
	CConfig.Set("Reset", "Sound"             , lpstData->nTower[eSIGNAL_RESET][eSIGNAL_SOUND]);
	CConfig.Set("Pause", "Priority"          , lpstData->nTower[eSIGNAL_PAUSE][eSIGNAL_PRIORTY]);
	CConfig.Set("Pause", "Red"               , lpstData->nTower[eSIGNAL_PAUSE][eSIGNAL_RED]);
	CConfig.Set("Pause", "Yellow"            , lpstData->nTower[eSIGNAL_PAUSE][eSIGNAL_YELLOW]);
	CConfig.Set("Pause", "Green"             , lpstData->nTower[eSIGNAL_PAUSE][eSIGNAL_GREEN]);
	CConfig.Set("Pause", "Sound"             , lpstData->nTower[eSIGNAL_PAUSE][eSIGNAL_SOUND]);
	CConfig.Set("Request", "Priority"        , lpstData->nTower[eSIGNAL_REQUEST][eSIGNAL_PRIORTY]);
	CConfig.Set("Request", "Red"             , lpstData->nTower[eSIGNAL_REQUEST][eSIGNAL_RED]);
	CConfig.Set("Request", "Yellow"          , lpstData->nTower[eSIGNAL_REQUEST][eSIGNAL_YELLOW]);
	CConfig.Set("Request", "Green"           , lpstData->nTower[eSIGNAL_REQUEST][eSIGNAL_GREEN]);
	CConfig.Set("Request", "Sound"           , lpstData->nTower[eSIGNAL_REQUEST][eSIGNAL_SOUND]);
	CConfig.Set("Temp Waiting", "Priority"   , lpstData->nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_PRIORTY]);
	CConfig.Set("Temp Waiting", "Red"        , lpstData->nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_RED]);
	CConfig.Set("Temp Waiting", "Yellow"     , lpstData->nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_YELLOW]);
	CConfig.Set("Temp Waiting", "Green"      , lpstData->nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_GREEN]);
	CConfig.Set("Temp Waiting", "Sound"      , lpstData->nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_SOUND]);
	CConfig.Set("Test Running", "Priority"   , lpstData->nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_PRIORTY]);
	CConfig.Set("Test Running", "Red"        , lpstData->nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_RED]);
	CConfig.Set("Test Running", "Yellow"     , lpstData->nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_YELLOW]);
	CConfig.Set("Test Running", "Green"      , lpstData->nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_GREEN]);
	CConfig.Set("Test Running", "Sound"      , lpstData->nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_SOUND]);
	CConfig.Set("Start", "Priority"          , lpstData->nTower[eSIGNAL_START][eSIGNAL_PRIORTY]);
	CConfig.Set("Start", "Red"               , lpstData->nTower[eSIGNAL_START][eSIGNAL_RED]);
	CConfig.Set("Start", "Yellow"            , lpstData->nTower[eSIGNAL_START][eSIGNAL_YELLOW]);
	CConfig.Set("Start", "Green"             , lpstData->nTower[eSIGNAL_START][eSIGNAL_GREEN]);
	CConfig.Set("Start", "Sound"             , lpstData->nTower[eSIGNAL_START][eSIGNAL_SOUND]);
	CConfig.Set("Temp Stabilize", "Priority" , lpstData->nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_PRIORTY]);
	CConfig.Set("Temp Stabilize", "Red"      , lpstData->nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_RED]);
	CConfig.Set("Temp Stabilize", "Yellow"   , lpstData->nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_YELLOW]);
	CConfig.Set("Temp Stabilize", "Green"    , lpstData->nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_GREEN]);
	CConfig.Set("Temp Stabilize", "Sound"    , lpstData->nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_SOUND]);

	return 0;
}

int CCokFileManager_Imp::LoadSignalTower( ST_SLT_SIGNAL_TOWER& lpstData )
{
	char szDir[MAX_PATH] = {0,};
	sprintf_s( szDir, sizeof( szDir ), "%s\\sys_data\\SignalTower.ini",m_szCurrentDir);
	CConfigData CConfig(szDir);

	lpstData.nTower[eSIGNAL_RUNNING][eSIGNAL_PRIORTY]        = CConfig.GetInt("Running", "Priority"        , "0");
	lpstData.nTower[eSIGNAL_RUNNING][eSIGNAL_RED]            = CConfig.GetInt("Running", "Red"             , "0");
	lpstData.nTower[eSIGNAL_RUNNING][eSIGNAL_YELLOW]         = CConfig.GetInt("Running", "Yellow"          , "0");
	lpstData.nTower[eSIGNAL_RUNNING][eSIGNAL_GREEN]          = CConfig.GetInt("Running", "Green"           , "0");
	lpstData.nTower[eSIGNAL_RUNNING][eSIGNAL_SOUND]          = CConfig.GetInt("Running", "Sound"           , "0");
	lpstData.nTower[eSIGNAL_JAM_CALL][eSIGNAL_PRIORTY]       = CConfig.GetInt("Jam Call", "Priority"       , "0");
	lpstData.nTower[eSIGNAL_JAM_CALL][eSIGNAL_RED]           = CConfig.GetInt("Jam Call", "Red"            , "0");
	lpstData.nTower[eSIGNAL_JAM_CALL][eSIGNAL_YELLOW]        = CConfig.GetInt("Jam Call", "Yellow"         , "0");
	lpstData.nTower[eSIGNAL_JAM_CALL][eSIGNAL_GREEN]         = CConfig.GetInt("Jam Call", "Green"          , "0");
	lpstData.nTower[eSIGNAL_JAM_CALL][eSIGNAL_SOUND]         = CConfig.GetInt("Jam Call", "Sound"          , "0");
	lpstData.nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_PRIORTY]     = CConfig.GetInt("Cycle Stop", "Priority"     , "0");
	lpstData.nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_RED]         = CConfig.GetInt("Cycle Stop", "Red"          , "0");
	lpstData.nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_YELLOW]      = CConfig.GetInt("Cycle Stop", "Yellow"       , "0");
	lpstData.nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_GREEN]       = CConfig.GetInt("Cycle Stop", "Green"        , "0");
	lpstData.nTower[eSIGNAL_CYCLE_STOP][eSIGNAL_SOUND]       = CConfig.GetInt("Cycle Stop", "Sound"        , "0");
	lpstData.nTower[eSIGNAL_FINISH][eSIGNAL_PRIORTY]         = CConfig.GetInt("Finish", "Priority"         , "0");
	lpstData.nTower[eSIGNAL_FINISH][eSIGNAL_RED]             = CConfig.GetInt("Finish", "Red"              , "0");
	lpstData.nTower[eSIGNAL_FINISH][eSIGNAL_YELLOW]          = CConfig.GetInt("Finish", "Yellow"           , "0");
	lpstData.nTower[eSIGNAL_FINISH][eSIGNAL_GREEN]           = CConfig.GetInt("Finish", "Green"            , "0");
	lpstData.nTower[eSIGNAL_FINISH][eSIGNAL_SOUND]           = CConfig.GetInt("Finish", "Sound"            , "0");
	lpstData.nTower[eSIGNAL_RESET][eSIGNAL_PRIORTY]          = CConfig.GetInt("Reset", "Priority"          , "0");
	lpstData.nTower[eSIGNAL_RESET][eSIGNAL_RED]              = CConfig.GetInt("Reset", "Red"               , "0");
	lpstData.nTower[eSIGNAL_RESET][eSIGNAL_YELLOW]           = CConfig.GetInt("Reset", "Yellow"            , "0");
	lpstData.nTower[eSIGNAL_RESET][eSIGNAL_GREEN]            = CConfig.GetInt("Reset", "Green"             , "0");
	lpstData.nTower[eSIGNAL_RESET][eSIGNAL_SOUND]            = CConfig.GetInt("Reset", "Sound"             , "0");
	lpstData.nTower[eSIGNAL_PAUSE][eSIGNAL_PRIORTY]          = CConfig.GetInt("Pause", "Priority"          , "0");
	lpstData.nTower[eSIGNAL_PAUSE][eSIGNAL_RED]              = CConfig.GetInt("Pause", "Red"               , "0");
	lpstData.nTower[eSIGNAL_PAUSE][eSIGNAL_YELLOW]           = CConfig.GetInt("Pause", "Yellow"            , "0");
	lpstData.nTower[eSIGNAL_PAUSE][eSIGNAL_GREEN]            = CConfig.GetInt("Pause", "Green"             , "0");
	lpstData.nTower[eSIGNAL_PAUSE][eSIGNAL_SOUND]            = CConfig.GetInt("Pause", "Sound"             , "0");
	lpstData.nTower[eSIGNAL_REQUEST][eSIGNAL_PRIORTY]        = CConfig.GetInt("Request", "Priority"        , "0");
	lpstData.nTower[eSIGNAL_REQUEST][eSIGNAL_RED]            = CConfig.GetInt("Request", "Red"             , "0");
	lpstData.nTower[eSIGNAL_REQUEST][eSIGNAL_YELLOW]         = CConfig.GetInt("Request", "Yellow"          , "0");
	lpstData.nTower[eSIGNAL_REQUEST][eSIGNAL_GREEN]          = CConfig.GetInt("Request", "Green"           , "0");
	lpstData.nTower[eSIGNAL_REQUEST][eSIGNAL_SOUND]          = CConfig.GetInt("Request", "Sound"           , "0");
	lpstData.nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_PRIORTY]   = CConfig.GetInt("Temp Waiting", "Priority"   , "0");
	lpstData.nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_RED]       = CConfig.GetInt("Temp Waiting", "Red"        , "0");
	lpstData.nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_YELLOW]    = CConfig.GetInt("Temp Waiting", "Yellow"     , "0");
	lpstData.nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_GREEN]     = CConfig.GetInt("Temp Waiting", "Green"      , "0");
	lpstData.nTower[eSIGNAL_TEMP_WAITING][eSIGNAL_SOUND]     = CConfig.GetInt("Temp Waiting", "Sound"      , "0");
	lpstData.nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_PRIORTY]   = CConfig.GetInt("Test Running", "Priority"   , "0");
	lpstData.nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_RED]       = CConfig.GetInt("Test Running", "Red"        , "0");
	lpstData.nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_YELLOW]    = CConfig.GetInt("Test Running", "Yellow"     , "0");
	lpstData.nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_GREEN]     = CConfig.GetInt("Test Running", "Green"      , "0");
	lpstData.nTower[eSIGNAL_TEST_RUNNING][eSIGNAL_SOUND]     = CConfig.GetInt("Test Running", "Sound"      , "0");
	lpstData.nTower[eSIGNAL_START][eSIGNAL_PRIORTY]          = CConfig.GetInt("Start", "Priority"          , "0");
	lpstData.nTower[eSIGNAL_START][eSIGNAL_RED]              = CConfig.GetInt("Start", "Red"               , "0");
	lpstData.nTower[eSIGNAL_START][eSIGNAL_YELLOW]           = CConfig.GetInt("Start", "Yellow"            , "0");
	lpstData.nTower[eSIGNAL_START][eSIGNAL_GREEN]            = CConfig.GetInt("Start", "Green"             , "0");
	lpstData.nTower[eSIGNAL_START][eSIGNAL_SOUND]            = CConfig.GetInt("Start", "Sound"             , "0");
	lpstData.nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_PRIORTY] = CConfig.GetInt("Temp Stabilize", "Priority" , "0");
	lpstData.nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_RED]     = CConfig.GetInt("Temp Stabilize", "Red"      , "0");
	lpstData.nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_YELLOW]  = CConfig.GetInt("Temp Stabilize", "Yellow"   , "0");
	lpstData.nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_GREEN]   = CConfig.GetInt("Temp Stabilize", "Green"    , "0");
	lpstData.nTower[eSIGNAL_TEMP_STABILIZE][eSIGNAL_SOUND]   = CConfig.GetInt("Temp Stabilize", "Sound"    , "0");
	

	return 0;
}

int CCokFileManager_Imp::SaveAppleTestIFidx(std::vector<int> vIFidx)
{
	char szDir[MAX_PATH] = {0,};
	sprintf_s( szDir, sizeof( szDir ), "%s\\sys_data\\UnitIndex.ini",m_szCurrentDir);
	CConfigData CConfig(szDir);

	char szNumber[32] = {0,};
	int nCntUnitIdx = 0;
	for(std::vector<int>::size_type i=0; i < vIFidx.size(); i++ ){
		sprintf_s(szNumber, sizeof(szNumber), "%d", nCntUnitIdx);
		CConfig.Set("UNIT_INDEX", szNumber, vIFidx[i]);
		nCntUnitIdx++;
	}

	return 0;
}

int CCokFileManager_Imp::LoadAppleTestIFidx(std::vector<int>& vIFidx)
{
	char szDir[MAX_PATH] = {0,};
	sprintf_s( szDir, sizeof( szDir ), "%s\\sys_data\\UnitIndex.ini",m_szCurrentDir);
	CConfigData CConfig(szDir);

	vIFidx.clear();

	char szNumber[32] = {0,};
	for(int i=0; i < eMaxPressUnitCount; i++ ){
		sprintf_s(szNumber, sizeof(szNumber), "%d", i);
		vIFidx.push_back(CConfig.GetInt("UNIT_INDEX", szNumber , "0"));
	}

	return 0;
}


// int CCokFileManager_Imp::LoadVatData(ST_VAT_3POINT& stVatTeachData)
// {
// 	char szCokFullName[512] = {0,};
// 	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\VisionAutoTeaching.dat",m_szCurrentDir);
// 	CConfigData CConfig(szCokFullName);
// 
// 	// Web Camera position
// 	stVatTeachData.dCameraPosition[VAT_TRAY_PP][VAT_MANUAL_CAM_CENTER][VAT_X]= CConfig.GetInt("TRAYPP"    , "Cam Manual Center X", "362500");
// 	stVatTeachData.dCameraPosition[VAT_TRAY_PP][VAT_MANUAL_CAM_CENTER][VAT_Y]= CConfig.GetInt("TRAYPP"    , "Cam Manual Center Y", "566000");
// 	stVatTeachData.dCameraPosition[VAT_TRAY_PP][VAT_MANUAL_CAM_CENTER][VAT_Z]= CConfig.GetInt("TRAYPP"    , "Cam Manual Center Z", "0");//??
// 	stVatTeachData.dCameraPosition[VAT_TRAY_PP][VAT_CAM_CENTER][VAT_X]       = CConfig.GetInt("TRAYPP"    , "Cam Find Center X"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TRAY_PP][VAT_CAM_CENTER][VAT_Y]       = CConfig.GetInt("TRAYPP"    , "Cam Find Center Y"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TRAY_PP][VAT_CAM_CENTER][VAT_Z]       = CConfig.GetInt("TRAYPP"    , "Cam Find Center Z"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP1][VAT_MANUAL_CAM_CENTER][VAT_X]= CConfig.GetInt("TESTPP_1" , "Cam Manual Center X", "19000");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP1][VAT_MANUAL_CAM_CENTER][VAT_Y]= CConfig.GetInt("TESTPP_1" , "Cam Manual Center Y", "161200");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP1][VAT_MANUAL_CAM_CENTER][VAT_Z]= CConfig.GetInt("TESTPP_1" , "Cam Manual Center Z", "0");//??
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP1][VAT_CAM_CENTER][VAT_X]       = CConfig.GetInt("TESTPP_1" , "Cam Find Center X"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP1][VAT_CAM_CENTER][VAT_Y]       = CConfig.GetInt("TESTPP_1" , "Cam Find Center Y"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP1][VAT_CAM_CENTER][VAT_Z]       = CConfig.GetInt("TESTPP_1" , "Cam Find Center Z"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP2][VAT_MANUAL_CAM_CENTER][VAT_X]= CConfig.GetInt("TESTPP_2" , "Cam Manual Center X", "29000");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP2][VAT_MANUAL_CAM_CENTER][VAT_Y]= CConfig.GetInt("TESTPP_2" , "Cam Manual Center Y", "161200");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP2][VAT_MANUAL_CAM_CENTER][VAT_Z]= CConfig.GetInt("TESTPP_2" , "Cam Manual Center Z", "0");//??
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP2][VAT_CAM_CENTER][VAT_X]       = CConfig.GetInt("TESTPP_2" , "Cam Find Center X"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP2][VAT_CAM_CENTER][VAT_Y]       = CConfig.GetInt("TESTPP_2" , "Cam Find Center Y"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP2][VAT_CAM_CENTER][VAT_Z]       = CConfig.GetInt("TESTPP_2" , "Cam Find Center Z"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP3][VAT_MANUAL_CAM_CENTER][VAT_X]= CConfig.GetInt("TESTPP_3" , "Cam Manual Center X", "413000");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP3][VAT_MANUAL_CAM_CENTER][VAT_Y]= CConfig.GetInt("TESTPP_3" , "Cam Manual Center Y", "161200");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP3][VAT_MANUAL_CAM_CENTER][VAT_Z]= CConfig.GetInt("TESTPP_3" , "Cam Manual Center Z", "0");//??
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP3][VAT_CAM_CENTER][VAT_X]       = CConfig.GetInt("TESTPP_3" , "Cam Find Center X"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP3][VAT_CAM_CENTER][VAT_Y]       = CConfig.GetInt("TESTPP_3" , "Cam Find Center Y"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP3][VAT_CAM_CENTER][VAT_Z]       = CConfig.GetInt("TESTPP_3" , "Cam Find Center Z"  , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP4][VAT_MANUAL_CAM_CENTER][VAT_X]= CConfig.GetInt("TESTPP_4" , "Cam Manual Center X", "423000");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP4][VAT_MANUAL_CAM_CENTER][VAT_Y]= CConfig.GetInt("TESTPP_4" , "Cam Manual Center Y", "161200");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP4][VAT_MANUAL_CAM_CENTER][VAT_Z]= CConfig.GetInt("TESTPP_4" , "Cam Manual Center Z", "0");//??
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP4][VAT_CAM_CENTER][VAT_X]       = CConfig.GetInt("TESTPP_4" , "Find Center X"      , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP4][VAT_CAM_CENTER][VAT_Y]       = CConfig.GetInt("TESTPP_4" , "Find Center Y"      , "0");
// 	stVatTeachData.dCameraPosition[VAT_TEST_PP4][VAT_CAM_CENTER][VAT_Z]       = CConfig.GetInt("TESTPP_4" , "Find Center Z"      , "0");
// 
// 	// 기구적 원점으로부터 계산되어지는 각 part postion 의 기준 위치 == Edge point
// 	stVatTeachData.dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_1][VAT_X]     = CConfig.GetInt("TRAYPP" , "Standard Edge Load Table 1 X", "130500"); // 566 - 435.5 ( WebCam - Pocket Dist )
// 	stVatTeachData.dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_1][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Standard Edge Load Table 1 Y", "494000"); // 362.5 +131.5 ( WebCam + Pocket Dist )
// 	stVatTeachData.dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_2][VAT_X]     = CConfig.GetInt("TRAYPP" , "Standard Edge Load Table 2 X", "297500"); // 566 - 268.5 ( WebCam - Pocket Dist )
// 	stVatTeachData.dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_2][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Standard Edge Load Table 2 Y", "494000");
// 	
// 
// 	// Standard 기구 position
// 	// 하단 진입 가능 높이 494.35mm+Gap mm // 상단 진입 가능 높이 6.35mm+Gap mm
// 	stVatTeachData.dStd3PEdgePosTest[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_X]         = CConfig.GetInt("TESTPP_1" , "Standard Edge Load Table X", "295550");// 293.77
// 	stVatTeachData.dStd3PEdgePosTest[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]         = CConfig.GetInt("TESTPP_1" , "Standard Edge Load Table Y", "56190"); //46.7
// 	stVatTeachData.dStd3PEdgePosTest[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]         = CConfig.GetInt("TESTPP_1" , "Standard Edge Load Table Z", "500000");//500.22
// 	
// 
// 	stVatTeachData.dStd3PEdgePosTest[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_X]         = CConfig.GetInt("TESTPP_2" , "Standard Edge Load Table X", "318000");//320
// 	stVatTeachData.dStd3PEdgePosTest[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]         = CConfig.GetInt("TESTPP_2" , "Standard Edge Load Table Y", "56190");//39.97
// 	stVatTeachData.dStd3PEdgePosTest[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]         = CConfig.GetInt("TESTPP_2" , "Standard Edge Load Table Z", "500000");//501.42
// 	
// 
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_TOP][VAT_X]     = CConfig.GetInt("TRAYPP" , "Measure TopLeft Load Table 1 X", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_TOP][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Measure TopLeft Load Table 1 Y", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_TOP][VAT_Z]     = CConfig.GetInt("TRAYPP" , "Measure TopLeft Load Table 1 Z", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_TOP][VAT_X]     = CConfig.GetInt("TRAYPP" , "Measure TopLeft Load Table 2 X", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_TOP][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Measure TopLeft Load Table 2 Y", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_TOP][VAT_Z]     = CConfig.GetInt("TRAYPP" , "Measure TopLeft Load Table 2 Z", "0");
// 	
// 
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_RIGHT_TOP][VAT_X]     = CConfig.GetInt("TRAYPP" , "Measure TopRight Load Table 1 X", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_RIGHT_TOP][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Measure TopRight Load Table 1 Y", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_RIGHT_TOP][VAT_Z]     = CConfig.GetInt("TRAYPP" , "Measure TopRight Load Table 1 Z", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_RIGHT_TOP][VAT_X]     = CConfig.GetInt("TRAYPP" , "Measure TopRight Load Table 2 X", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_RIGHT_TOP][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Measure TopRight Load Table 2 Y", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_RIGHT_TOP][VAT_Z]     = CConfig.GetInt("TRAYPP" , "Measure TopRight Load Table 2 Z", "0");
// 
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_BOTTOM][VAT_X]     = CConfig.GetInt("TRAYPP" , "Measure BottomLeft Load Table 1 X", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Measure BottomLeft Load Table 1 Y", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Z]     = CConfig.GetInt("TRAYPP" , "Measure BottomLeft Load Table 1 Z", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_BOTTOM][VAT_X]     = CConfig.GetInt("TRAYPP" , "Measure BottomLeft Load Table 2 X", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Measure BottomLeft Load Table 2 Y", "0");
// 	stVatTeachData.dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Z]     = CConfig.GetInt("TRAYPP" , "Measure BottomLeft Load Table 2 Z", "0");
// 
// 	// Find Test PP position
// 	stVatTeachData.dTest3P_MeasureLoc[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_X]         = CConfig.GetInt("TESTPP_1" , "Measure Load Table X", "0");
// 	stVatTeachData.dTest3P_MeasureLoc[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]         = CConfig.GetInt("TESTPP_1" , "Measure Load Table Y", "0");
// 	stVatTeachData.dTest3P_MeasureLoc[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]         = CConfig.GetInt("TESTPP_1" , "Measure Load Table Z", "0");
// 
// 	stVatTeachData.dTest3P_MeasureLoc[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_X]         = CConfig.GetInt("TESTPP_2" , "Measure Load Table X", "0");
// 	stVatTeachData.dTest3P_MeasureLoc[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]         = CConfig.GetInt("TESTPP_2" , "Measure Load Table Y", "0");
// 	stVatTeachData.dTest3P_MeasureLoc[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]         = CConfig.GetInt("TESTPP_2" , "Measure Load Table Z", "0");
// 
// 	// tray pp gradient
// 	stVatTeachData.dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_1][VAT_X]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 1 X", "0");
// 	stVatTeachData.dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_1][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 1 Y", "0");
// 	stVatTeachData.dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_2][VAT_X]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 2 X", "0");
// 	stVatTeachData.dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_2][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 2 Y", "0");
// 
// 	// tray pp scale factor
// 	stVatTeachData.dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_1][VAT_X]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 1 X", "0");
// 	stVatTeachData.dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_1][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 1 Y", "0");
// 	stVatTeachData.dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_2][VAT_X]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 2 X", "0");
// 	stVatTeachData.dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_2][VAT_Y]     = CConfig.GetInt("TRAYPP" , "Gradient Load Table 2 Y", "0");
// 
// 	return 0;
// }
// 
// int CCokFileManager_Imp::SaveVatData(ST_VAT_3POINT* lpstVatTeachData)
// {
// 	char szCokFullName[512] = {0,};
// 	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\VisionAutoTeaching.dat",m_szCurrentDir);
// 	CConfigData CConfig(szCokFullName);
// 
// 	// Web Camera position
// 	CConfig.Set("TRAYPP"    , "Cam Manual Center X", lpstVatTeachData->dCameraPosition[VAT_TRAY_PP][VAT_MANUAL_CAM_CENTER][VAT_X]); 
// 	CConfig.Set("TRAYPP"    , "Cam Manual Center Y", lpstVatTeachData->dCameraPosition[VAT_TRAY_PP][VAT_MANUAL_CAM_CENTER][VAT_Y]); 
// 	CConfig.Set("TRAYPP"    , "Cam Manual Center Z", lpstVatTeachData->dCameraPosition[VAT_TRAY_PP][VAT_MANUAL_CAM_CENTER][VAT_Z]);    
// 	CConfig.Set("TRAYPP"    , "Cam Find Center X"  , lpstVatTeachData->dCameraPosition[VAT_TRAY_PP][VAT_CAM_CENTER][VAT_X]);
// 	CConfig.Set("TRAYPP"    , "Cam Find Center Y"  , lpstVatTeachData->dCameraPosition[VAT_TRAY_PP][VAT_CAM_CENTER][VAT_Y]);
// 	CConfig.Set("TRAYPP"    , "Cam Find Center Z"  , lpstVatTeachData->dCameraPosition[VAT_TRAY_PP][VAT_CAM_CENTER][VAT_Z]);
// 
// 	CConfig.Set("TESTPP_1" , "Cam Manual Center X", lpstVatTeachData->dCameraPosition[VAT_TEST_PP1][VAT_MANUAL_CAM_CENTER][VAT_X]);
// 	CConfig.Set("TESTPP_1" , "Cam Manual Center Y", lpstVatTeachData->dCameraPosition[VAT_TEST_PP1][VAT_MANUAL_CAM_CENTER][VAT_Y]);
// 	CConfig.Set("TESTPP_1" , "Cam Manual Center Z", lpstVatTeachData->dCameraPosition[VAT_TEST_PP1][VAT_MANUAL_CAM_CENTER][VAT_Z]);
// 	CConfig.Set("TESTPP_1" , "Cam Find Center X"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP1][VAT_CAM_CENTER][VAT_X]       );
// 	CConfig.Set("TESTPP_1" , "Cam Find Center Y"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP1][VAT_CAM_CENTER][VAT_Y]       );
// 	CConfig.Set("TESTPP_1" , "Cam Find Center Z"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP1][VAT_CAM_CENTER][VAT_Z]       );
// 	CConfig.Set("TESTPP_2" , "Cam Manual Center X", lpstVatTeachData->dCameraPosition[VAT_TEST_PP2][VAT_MANUAL_CAM_CENTER][VAT_X]);
// 	CConfig.Set("TESTPP_2" , "Cam Manual Center Y", lpstVatTeachData->dCameraPosition[VAT_TEST_PP2][VAT_MANUAL_CAM_CENTER][VAT_Y]);
// 	CConfig.Set("TESTPP_2" , "Cam Manual Center Z", lpstVatTeachData->dCameraPosition[VAT_TEST_PP2][VAT_MANUAL_CAM_CENTER][VAT_Z]);
// 	CConfig.Set("TESTPP_2" , "Cam Find Center X"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP2][VAT_CAM_CENTER][VAT_X]       );
// 	CConfig.Set("TESTPP_2" , "Cam Find Center Y"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP2][VAT_CAM_CENTER][VAT_Y]       );
// 	CConfig.Set("TESTPP_2" , "Cam Find Center Z"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP2][VAT_CAM_CENTER][VAT_Z]       );
// 	CConfig.Set("TESTPP_3" , "Cam Manual Center X", lpstVatTeachData->dCameraPosition[VAT_TEST_PP3][VAT_MANUAL_CAM_CENTER][VAT_X]);
// 	CConfig.Set("TESTPP_3" , "Cam Manual Center Y", lpstVatTeachData->dCameraPosition[VAT_TEST_PP3][VAT_MANUAL_CAM_CENTER][VAT_Y]);
// 	CConfig.Set("TESTPP_3" , "Cam Manual Center Z", lpstVatTeachData->dCameraPosition[VAT_TEST_PP3][VAT_MANUAL_CAM_CENTER][VAT_Z]);
// 	CConfig.Set("TESTPP_3" , "Cam Find Center X"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP3][VAT_CAM_CENTER][VAT_X]       );
// 	CConfig.Set("TESTPP_3" , "Cam Find Center Y"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP3][VAT_CAM_CENTER][VAT_Y]       );
// 	CConfig.Set("TESTPP_3" , "Cam Find Center Z"  , lpstVatTeachData->dCameraPosition[VAT_TEST_PP3][VAT_CAM_CENTER][VAT_Z]       );
// 	CConfig.Set("TESTPP_4" , "Cam Manual Center X", lpstVatTeachData->dCameraPosition[VAT_TEST_PP4][VAT_MANUAL_CAM_CENTER][VAT_X]);
// 	CConfig.Set("TESTPP_4" , "Cam Manual Center Y", lpstVatTeachData->dCameraPosition[VAT_TEST_PP4][VAT_MANUAL_CAM_CENTER][VAT_Y]);
// 	CConfig.Set("TESTPP_4" , "Cam Manual Center Z", lpstVatTeachData->dCameraPosition[VAT_TEST_PP4][VAT_MANUAL_CAM_CENTER][VAT_Z]);
// 	CConfig.Set("TESTPP_4" , "Find Center X"      , lpstVatTeachData->dCameraPosition[VAT_TEST_PP4][VAT_CAM_CENTER][VAT_X]       );
// 	CConfig.Set("TESTPP_4" , "Find Center Y"      , lpstVatTeachData->dCameraPosition[VAT_TEST_PP4][VAT_CAM_CENTER][VAT_Y]       );
// 	CConfig.Set("TESTPP_4" , "Find Center Z"      , lpstVatTeachData->dCameraPosition[VAT_TEST_PP4][VAT_CAM_CENTER][VAT_Z]       );
//  
// 
// 	// 기구적 원점으로부터 계산되어지는 각 part postion 의 기준 위치 == Edge point
// 	CConfig.Set("TRAYPP" , "Standard Edge Load Table 1 X", lpstVatTeachData->dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_1][VAT_X]     ); 
// 	CConfig.Set("TRAYPP" , "Standard Edge Load Table 1 Y", lpstVatTeachData->dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_1][VAT_Y]     ); 
// 	CConfig.Set("TRAYPP" , "Standard Edge Load Table 2 X", lpstVatTeachData->dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_2][VAT_X]     ); 
// 	CConfig.Set("TRAYPP" , "Standard Edge Load Table 2 Y", lpstVatTeachData->dStd3PEdgePosTray[LOC_IDX_TRAY_PP_SHT_2][VAT_Y]     );
// 	
// 	// Standard 기구 position
// 	// 하단 진입 가능 높이 494.35mm+Gap mm // 상단 진입 가능 높이 6.35mm+Gap mm
// 	CConfig.Set("TESTPP_1" , "Standard Edge Load Table X", lpstVatTeachData->dStd3PEdgePosTest[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_X]        );
// 	CConfig.Set("TESTPP_1" , "Standard Edge Load Table Y", lpstVatTeachData->dStd3PEdgePosTest[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]        ); 
// 	CConfig.Set("TESTPP_1" , "Standard Edge Load Table Z", lpstVatTeachData->dStd3PEdgePosTest[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]        );
// 
// 	CConfig.Set("TESTPP_2" , "Standard Edge Load Table X", lpstVatTeachData->dStd3PEdgePosTest[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_X]        );
// 	CConfig.Set("TESTPP_2" , "Standard Edge Load Table Y", lpstVatTeachData->dStd3PEdgePosTest[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]        );
// 	CConfig.Set("TESTPP_2" , "Standard Edge Load Table Z", lpstVatTeachData->dStd3PEdgePosTest[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]        );
// 
// 	CConfig.Set("TRAYPP" , "Measure TopLeft Load Table 1 X", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_TOP][VAT_X]);
// 	CConfig.Set("TRAYPP" , "Measure TopLeft Load Table 1 Y", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_TOP][VAT_Y]);
// 	CConfig.Set("TRAYPP" , "Measure TopLeft Load Table 1 Z", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_TOP][VAT_Z]);
// 	CConfig.Set("TRAYPP" , "Measure TopLeft Load Table 2 X", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_TOP][VAT_X]);
// 	CConfig.Set("TRAYPP" , "Measure TopLeft Load Table 2 Y", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_TOP][VAT_Y]);
// 	CConfig.Set("TRAYPP" , "Measure TopLeft Load Table 2 Z", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_TOP][VAT_Z]);
// 
// 	CConfig.Set("TRAYPP" , "Measure TopRight Load Table 1 X", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_RIGHT_TOP][VAT_X]);
// 	CConfig.Set("TRAYPP" , "Measure TopRight Load Table 1 Y", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_RIGHT_TOP][VAT_Y]);
// 	CConfig.Set("TRAYPP" , "Measure TopRight Load Table 1 Z", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_RIGHT_TOP][VAT_Z]);
// 	CConfig.Set("TRAYPP" , "Measure TopRight Load Table 2 X", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_RIGHT_TOP][VAT_X]);
// 	CConfig.Set("TRAYPP" , "Measure TopRight Load Table 2 Y", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_RIGHT_TOP][VAT_Y]);
// 	CConfig.Set("TRAYPP" , "Measure TopRight Load Table 2 Z", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_RIGHT_TOP][VAT_Z]);
// 
// 	CConfig.Set("TRAYPP" , "Measure BottomLeft Load Table 1 X", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_BOTTOM][VAT_X]);
// 	CConfig.Set("TRAYPP" , "Measure BottomLeft Load Table 1 Y", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Y]);
// 	CConfig.Set("TRAYPP" , "Measure BottomLeft Load Table 1 Z", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_1][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Z]);
// 	CConfig.Set("TRAYPP" , "Measure BottomLeft Load Table 2 X", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_BOTTOM][VAT_X]);
// 	CConfig.Set("TRAYPP" , "Measure BottomLeft Load Table 2 Y", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Y]);
// 	CConfig.Set("TRAYPP" , "Measure BottomLeft Load Table 2 Z", lpstVatTeachData->dTray3P_MeasureLoc[LOC_IDX_TRAY_PP_SHT_2][VAT3P_MEASURE_LEFT_BOTTOM][VAT_Z]);
// 
// 	// Find Test PP position
// 	CConfig.Set("TESTPP_1" , "Measure Load Table X", lpstVatTeachData->dTest3P_MeasureLoc[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_X]        );
// 	CConfig.Set("TESTPP_1" , "Measure Load Table Y", lpstVatTeachData->dTest3P_MeasureLoc[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]        );
// 	CConfig.Set("TESTPP_1" , "Measure Load Table Z", lpstVatTeachData->dTest3P_MeasureLoc[eTestPP_1][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]        );
// 
// 	CConfig.Set("TESTPP_2" , "Measure Load Table X", lpstVatTeachData->dTest3P_MeasureLoc[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_X]        );
// 	CConfig.Set("TESTPP_2" , "Measure Load Table Y", lpstVatTeachData->dTest3P_MeasureLoc[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Y]        );
// 	CConfig.Set("TESTPP_2" , "Measure Load Table Z", lpstVatTeachData->dTest3P_MeasureLoc[eTestPP_2][LOC_IDX_TEST_PP_LD_TBL][VAT_Z]        );
// 
// 	// tray pp gradient
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 1 X", lpstVatTeachData->dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_1][VAT_X]     );
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 1 Y", lpstVatTeachData->dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_1][VAT_Y]     );
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 2 X", lpstVatTeachData->dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_2][VAT_X]     );
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 2 Y", lpstVatTeachData->dTray3P_Gradient[LOC_IDX_TRAY_PP_SHT_2][VAT_Y]     );
// 
// 	// tray pp scale factor
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 1 X", lpstVatTeachData->dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_1][VAT_X]     );
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 1 Y", lpstVatTeachData->dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_1][VAT_Y]     );
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 2 X", lpstVatTeachData->dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_2][VAT_X]     );
// 	CConfig.Set("TRAYPP" , "Gradient Load Table 2 Y", lpstVatTeachData->dTray3P_ScalefactorRate[LOC_IDX_TRAY_PP_SHT_2][VAT_Y]     );
// 
// 	return 0;
// }

int CCokFileManager_Imp::LoadTrayAssign(ST_TRAY_ASSIGN &lpstData)
{
	char szCokFullName[512] = {0,};
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\TrayAssign.dat",m_szCurrentDir);
	CConfigData CConfig(szCokFullName);

	lpstData.nLoad1Top      = CConfig.GetInt("TrayAssign", "Load1Top"     , "0");
	lpstData.nLoad1Bottom   = CConfig.GetInt("TrayAssign", "Load1Bottom"  , "0");
	lpstData.nLoad2Top      = CConfig.GetInt("TrayAssign", "Load2Top"     , "0");
	lpstData.nLoad2Bottom   = CConfig.GetInt("TrayAssign", "Load2Bottom"  , "0");
	lpstData.nUnLoad1Top    = CConfig.GetInt("TrayAssign", "UnLoad1Top"   , "0");
	lpstData.nUnLoad1Bottom = CConfig.GetInt("TrayAssign", "UnLoad1Bottom", "0");
	lpstData.nUnLoad2Top    = CConfig.GetInt("TrayAssign", "UnLoad2Top"   , "0");
	lpstData.nUnLoad2Bottom = CConfig.GetInt("TrayAssign", "UnLoad2Bottom", "0");
	lpstData.nUnLoad3Top    = CConfig.GetInt("TrayAssign", "UnLoad3Top"   , "0");
	lpstData.nUnLoad3Bottom = CConfig.GetInt("TrayAssign", "UnLoad3Bottom", "0");
	lpstData.nUnLoad4Top    = CConfig.GetInt("TrayAssign", "UnLoad4Top"   , "0");
	lpstData.nUnLoad4Bottom = CConfig.GetInt("TrayAssign", "UnLoad4Bottom", "0");
	lpstData.nUnLoad5Top    = CConfig.GetInt("TrayAssign", "UnLoad5Top"   , "0");
	lpstData.nUnLoad5Bottom = CConfig.GetInt("TrayAssign", "UnLoad5Bottom", "0");

	return 0;
}

int CCokFileManager_Imp::SaveTrayAssign(ST_TRAY_ASSIGN *lpstData)
{
	char szCokFullName[512] = {0,};
	sprintf_s( szCokFullName, sizeof( szCokFullName ), "%s\\sys_data\\TrayAssign.dat",m_szCurrentDir);
	CConfigData CConfig(szCokFullName);

	CConfig.Set("TrayAssign", "Load1Top"     , lpstData->nLoad1Top     );
	CConfig.Set("TrayAssign", "Load1Bottom"  , lpstData->nLoad1Bottom  );
	CConfig.Set("TrayAssign", "Load2Top"     , lpstData->nLoad2Top     );
	CConfig.Set("TrayAssign", "Load2Bottom"  , lpstData->nLoad2Bottom  );
	CConfig.Set("TrayAssign", "UnLoad1Top"   , lpstData->nUnLoad1Top   );
	CConfig.Set("TrayAssign", "UnLoad1Bottom", lpstData->nUnLoad1Bottom);
	CConfig.Set("TrayAssign", "UnLoad2Top"   , lpstData->nUnLoad2Top   );
	CConfig.Set("TrayAssign", "UnLoad2Bottom", lpstData->nUnLoad2Bottom);
	CConfig.Set("TrayAssign", "UnLoad3Top"   , lpstData->nUnLoad3Top   );
	CConfig.Set("TrayAssign", "UnLoad3Bottom", lpstData->nUnLoad3Bottom);
	CConfig.Set("TrayAssign", "UnLoad4Top"   , lpstData->nUnLoad4Top   );
	CConfig.Set("TrayAssign", "UnLoad4Bottom", lpstData->nUnLoad4Bottom);
	CConfig.Set("TrayAssign", "UnLoad5Top"   , lpstData->nUnLoad5Top   );
	CConfig.Set("TrayAssign", "UnLoad5Bottom", lpstData->nUnLoad5Bottom);
	
	return 0;
}		



int CCokFileManager_Imp::SaveUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP* lpstData)
{
	char szCokFullName[512] = { 0, };
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\UserOffSetTestPp_MP.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);
	CConfigData CConfig(szCokFullName);

	char szTestPp[16] = { 0, };
	sprintf_s(szTestPp, sizeof(szTestPp), "TestPp%d", nTestPpIdx + 1);


	CConfig.Set(szTestPp, "Down_Station_1_X", lpstData->stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_X]);
	CConfig.Set(szTestPp, "Down_Station_1_Y", lpstData->stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_Y]);
	CConfig.Set(szTestPp, "Down_Station_1_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_Z_Pick]);
	CConfig.Set(szTestPp, "Down_Station_1_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_Z_Place]);

	CConfig.Set(szTestPp, "Down_Station_2_X", lpstData->stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_X]);
	CConfig.Set(szTestPp, "Down_Station_2_Y", lpstData->stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_Y]);
	CConfig.Set(szTestPp, "Down_Station_2_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_Z_Pick]);
	CConfig.Set(szTestPp, "Down_Station_2_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_Z_Place]);

	CConfig.Set(szTestPp, "Down_Station_3_X", lpstData->stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_X]);
	CConfig.Set(szTestPp, "Down_Station_3_Y", lpstData->stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_Y]);
	CConfig.Set(szTestPp, "Down_Station_3_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_Z_Pick]);
	CConfig.Set(szTestPp, "Down_Station_3_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_Z_Place]);

	CConfig.Set(szTestPp, "Down_Station_4_X", lpstData->stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_X]);
	CConfig.Set(szTestPp, "Down_Station_4_Y", lpstData->stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_Y]);
	CConfig.Set(szTestPp, "Down_Station_4_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_Z_Pick]);
	CConfig.Set(szTestPp, "Down_Station_4_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_Z_Place]);

// 	CConfig.Set(szTestPp, "Up_Station_1_X", lpstData->stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_1_Y", lpstData->stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_1_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_1_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_2_X", lpstData->stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_2_Y", lpstData->stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_2_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_2_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_3_X", lpstData->stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_3_Y", lpstData->stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_3_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_3_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_4_X", lpstData->stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_4_Y", lpstData->stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_4_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_4_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_Z_Place]);

	//Rear Site Front Picker Offset

// 	CConfig.Set(szTestPp, "Down_Station_1_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_1_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_1_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_1_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Down_Station_2_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_2_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_2_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_2_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Down_Station_3_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_3_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_3_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_3_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Down_Station_4_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_4_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_4_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_4_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);

// 	CConfig.Set(szTestPp, "Up_Station_1_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_1_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_1_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_1_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_2_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_2_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_2_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_2_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_3_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_3_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_3_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_3_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_4_RearSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_4_RearSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_4_RearSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_4_RearSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);

	//Front Site Front Picker Offset

// 	CConfig.Set(szTestPp, "Down_Station_1_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_1_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_1_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_1_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Down_Station_2_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_2_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_2_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_2_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Down_Station_3_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_3_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_3_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_3_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Down_Station_4_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_DN_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Down_Station_4_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_DN_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Down_Station_4_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_DN_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Down_Station_4_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_DN_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);

// 	CConfig.Set(szTestPp, "Up_Station_1_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_1_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_1_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_1_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_2_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_2_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_2_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_2_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_3_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_3_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_3_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_3_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);
// 
// 	CConfig.Set(szTestPp, "Up_Station_4_FrontSite_FrontPicker_X", lpstData->stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_X]);
// 	CConfig.Set(szTestPp, "Up_Station_4_FrontSite_FrontPicker_Y", lpstData->stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y]);
// 	CConfig.Set(szTestPp, "Up_Station_4_FrontSite_FrontPicker_Z_PICK", lpstData->stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick]);
// 	CConfig.Set(szTestPp, "Up_Station_4_FrontSite_FrontPicker_Z_PLACE", lpstData->stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place]);

	return 0;
}

int CCokFileManager_Imp::LoadUserOffSetDataTestPp_MP(char* szCokName, int nTestPpIdx, ST_USER_OFFSET_TESTPP_MP& lpstData)
{
	char szCokFullName[512] = { 0, };
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\UserOffSetTestPp_MP.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);
	CConfigData CConfig(szCokFullName);

	char szTestPp[16] = { 0, };
	sprintf_s(szTestPp, sizeof(szTestPp), "TestPp%d", nTestPpIdx + 1);

	lpstData.stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Down_Station_1_X", "0");
	lpstData.stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Down_Station_1_Y", "0");
	lpstData.stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Down_Station_1_Z_PICK", "0");
	lpstData.stTestSite[eATS_DN_STATION_L1].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Down_Station_1_Z_PLACE", "0");

	lpstData.stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Down_Station_2_X", "0");
	lpstData.stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Down_Station_2_Y", "0");
	lpstData.stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Down_Station_2_Z_PICK", "0");
	lpstData.stTestSite[eATS_DN_STATION_L2].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Down_Station_2_Z_PLACE", "0");

	lpstData.stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Down_Station_3_X", "0");
	lpstData.stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Down_Station_3_Y", "0");
	lpstData.stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Down_Station_3_Z_PICK", "0");
	lpstData.stTestSite[eATS_DN_STATION_L3].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Down_Station_3_Z_PLACE", "0");

	lpstData.stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Down_Station_4_X", "0");
	lpstData.stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Down_Station_4_Y", "0");
	lpstData.stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Down_Station_4_Z_PICK", "0");
	lpstData.stTestSite[eATS_DN_STATION_L4].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Down_Station_4_Z_PLACE", "0");


// 	lpstData.stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_1_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_1_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_1_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_1_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_2_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_2_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_2_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_2_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_3_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_3_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_3_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_3_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_4_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_4_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_4_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nOffSet[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_4_Z_PLACE", "0");

	//Rear site Front Picker Offset




// 	lpstData.stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_1_RearSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_1_RearSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_1_RearSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_1_RearSite_FrontPicker_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_2_RearSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_2_RearSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_2_RearSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_2_RearSite_FrontPicker_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_3_RearSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_3_RearSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_3_RearSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_3_RearSite_FrontPicker_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_4_RearSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_4_RearSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_4_RearSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_4_RearSite_FrontPicker_Z_PLACE", "0");


// 	lpstData.stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_1_FrontSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_1_FrontSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_1_FrontSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_1_FrontSite_FrontPicker_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_2_FrontSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_2_FrontSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_2_FrontSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L2].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_2_FrontSite_FrontPicker_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_3_FrontSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_3_FrontSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_3_FrontSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L3].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_3_FrontSite_FrontPicker_Z_PLACE", "0");
// 
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_X] = CConfig.GetInt(szTestPp, "Up_Station_4_FrontSite_FrontPicker_X", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y] = CConfig.GetInt(szTestPp, "Up_Station_4_FrontSite_FrontPicker_Y", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick] = CConfig.GetInt(szTestPp, "Up_Station_4_FrontSite_FrontPicker_Z_PICK", "0");
// 	lpstData.stTestSite[eATS_UP_STATION_L4].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place] = CConfig.GetInt(szTestPp, "Up_Station_4_FrontSite_FrontPicker_Z_PLACE", "0");


	//Load Table 2Y Row Offset
	return 0;
}



int CCokFileManager_Imp::SaveSiteOnOffInfo(char* szCokName,int nStation, int nSiteOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE])
{
	char szCokFullName[512] = { 0, };
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\BasicAction.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);//, szCokName );
	CConfigData CConfig(szCokFullName);

	char szSubTitle[256] = { 0, };
	ZeroMemory(szSubTitle, sizeof(szSubTitle));
	sprintf_s(szSubTitle, sizeof(szSubTitle), "Station%d_SocketOnOff", nStation + 1);
	CString SocketOnOff = _T("");
	CString tmp = _T("");
	for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
	{
		for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
		{
			tmp.Format(_T("%d,"), nSiteOnOff[nStation][nY][nX]);
			SocketOnOff += tmp;
		}
	}
	SocketOnOff.TrimRight(",");
	CConfig.Set("Machine Config", szSubTitle, (LPSTR)(LPCTSTR)SocketOnOff);

	return 0;
}

int CCokFileManager_Imp::LoadSiteOnOffInfo(char* szCokName, int* nSiteOnOff)
{
	char szCokFullName[512] = { 0, };
	sprintf_s(szCokFullName, sizeof(szCokFullName), "%s\\sys_data\\handlerRecipe\\%s\\%s\\BasicAction.dat", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);//, szCokName );

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe", m_szCurrentDir);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s", m_szCurrentDir, SZ_TRAY_FOLDER);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s", m_szCurrentDir, SZ_TRAY_FOLDER, szCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	CConfigData CConfig(szCokFullName);

	char szSubTitle[256] = { 0, };
	char szSocketOnOff[2056] = { 0, };
	//CString SocketOnOff = _T( "" );
	//CString tkn = _T( "" );
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		ZeroMemory(szSubTitle, sizeof(szSubTitle));
		sprintf_s(szSubTitle, sizeof(szSubTitle), "Station%d_SocketOnOff", nStation + 1);
		CConfig.GetString("Machine Config", szSubTitle, "0,0,0,0,0,0,0,0", szSocketOnOff);

		CParamList PrmList;
		PrmList.m_fnSetDelimiter(",", 1);
		PrmList.m_fnSetDelimitedText(szSocketOnOff, strlen(szSocketOnOff));
		int nPosCount = PrmList.m_fnGetCount();
		int index = 0;
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				int nBuffer_Index = (nX) + (nY *STATION_MAX_X_SITE) + (nStation * STATION_MAX_PARA);
				if (index < nPosCount)
					
					nSiteOnOff[nBuffer_Index] = atoi(PrmList.m_fnGetItemString(index++));
				else
					nSiteOnOff[nBuffer_Index] = 0;
			}
		}
	}

	return 0;
}