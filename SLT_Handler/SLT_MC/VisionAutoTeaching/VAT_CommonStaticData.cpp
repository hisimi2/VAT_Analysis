#include "stdafx.h"
#include "VAT_CommonStaticData.h"


CVAT_CommonStaticData::CVAT_CommonStaticData()
{
}


CVAT_CommonStaticData::~CVAT_CommonStaticData()
{
}

CString CVAT_CommonStaticData::GetStaticDataFilePath(int nMode)
{
	char szCokName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	CString strFilePath = _T("");

	switch (nMode)
	{
	case VAT_STATIC_DATA_PATH_FIXED_DESIGN_VALUE:
		strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\VAT_FixedDesignValue.dat", SZ_TRAY_FOLDER, szCokName);
		break;
	default:
		break;
	}

	return strFilePath;
}

void CVAT_CommonStaticData::IsExistVATDirectory()
{
	char szCokName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	CString strFilePath = _T("");
	strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT", SZ_TRAY_FOLDER, szCokName);

	if (GetFileAttributes(strFilePath) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strFilePath, NULL);
	}
}

void CVAT_CommonStaticData::LoadVATStaticData()
{
	//File 유무 Check
	IsExistVATDirectory();

	LoadFixedDesignValueData();

	//고정된 설계 치수와 파일값이 다르다면 Static값으로 Save 진행.
	for (int nType = 0; nType < eCommon_Type_Max; nType++) {
		int nRet = CheckCommonDataValidate(nType);
		if (nRet != ERR_VAT_NO_ERROR) {
			SaveHandCommonData(nType);
		}
	}
}


// void CVAT_CommonStaticData::SaveVATStaticData()
// {
// 	//SaveHandCommonData();
// }




void CVAT_CommonStaticData::LoadFixedDesignValueData()
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetStaticDataFilePath(VAT_STATIC_DATA_PATH_FIXED_DESIGN_VALUE));

	// [1]기준 Picker
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{		
		m_stVATHandCommonData.pairStdPicker[nHand].first = cfData.GetDouble(m_stVATFileSection.pComStaticSec_StdPicker[nHand], m_stVATFileSection.pKeyStdPicker[VAT_X], m_pszStdPicker[nHand][VAT_X]);
		m_stVATHandCommonData.pairStdPicker[nHand].second = cfData.GetDouble(m_stVATFileSection.pComStaticSec_StdPicker[nHand], m_stVATFileSection.pKeyStdPicker[VAT_Y], m_pszStdPicker[nHand][VAT_Y]);
	}

	
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		// [2]Plate Jig
		m_stVATHandCommonData.dPlateJigHeight[nHand] = cfData.GetDouble(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyPlateJigHeight, m_pszPlateJigHeight[nHand]);
		m_stVATHandCommonData.dTopPlateThickness[nHand] = cfData.GetDouble(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyTopPlateThickness, m_pszTopPlateThickness[nHand]);
	
		// [3]Base 높이	
		m_stVATHandCommonData.dBaseToPlateJigHeight[nHand] = cfData.GetDouble(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyBaseToPlateJigHeight, m_pszBaseToPlateJigHeight[nHand]);

		for (int nUnit = 0; nUnit < VAT_MAX_BASE_TO_UNIT; nUnit++) {
			m_stVATHandCommonData.dBaseToUnitHeight[nHand][nUnit] = cfData.GetDouble(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyBaseToUnit[nHand][nUnit], m_pszBaseToUnitHeight[nHand][nUnit]);
		}
	}	
}


void CVAT_CommonStaticData::SaveHandCommonData(int nType)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetStaticDataFilePath(VAT_STATIC_DATA_PATH_FIXED_DESIGN_VALUE));

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		switch (nType)
		{
			case eCommon_STDPicker:
			{
				cfData.Set(m_stVATFileSection.pComStaticSec_StdPicker[nHand], m_stVATFileSection.pKeyStdPicker[VAT_X], m_pszStdPicker[nHand][VAT_X]);
				cfData.Set(m_stVATFileSection.pComStaticSec_StdPicker[nHand], m_stVATFileSection.pKeyStdPicker[VAT_Y], m_pszStdPicker[nHand][VAT_Y]);
			}break;
			case eCommon_Plate_Jig_Height:
			{
				cfData.Set(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyPlateJigHeight, m_pszPlateJigHeight[nHand]);
			}break;
			case eCommon_Top_Plate_Thickness:
			{
				cfData.Set(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyTopPlateThickness, m_pszTopPlateThickness[nHand]);
			}break;
			case eCommon_Base_To_Plate_Jig_Height:
			{
				cfData.Set(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyBaseToPlateJigHeight, m_pszBaseToPlateJigHeight[nHand]);
			}break;
			case eCommon_Base_To_Unit_Height:
			{
				for (int nUnit = 0; nUnit < VAT_MAX_BASE_TO_UNIT; nUnit++) {
					cfData.Set(m_stVATFileSection.pComStaticSec_Hand[nHand], m_stVATFileSection.pKeyBaseToUnit[nHand][nUnit], m_pszBaseToUnitHeight[nHand][nUnit]);
				}
			}break;
			default:
				break;
		}
	}

}


//File Data 값과 실제 Static Design Value 비교.
int CVAT_CommonStaticData::CheckCommonDataValidate(int nType)
{
	int nRet = ERR_VAT_NO_ERROR;

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		double dFileData = 0.0, dStaticData = 0.0;
		
		switch (nType)
		{
		case eCommon_STDPicker:
		{
			for (int nAxis = VAT_X; nAxis <= VAT_Y; nAxis++) {
				dFileData = (nAxis == VAT_X) ? m_stVATHandCommonData.pairStdPicker[nHand].first : m_stVATHandCommonData.pairStdPicker[nHand].second;
				dStaticData = atof(m_pszStdPicker[nHand][nAxis]);

				if (dFileData != dStaticData) {
					return ERR_VAT_ERROR;
				}
			}
		}break;
		case eCommon_Plate_Jig_Height:
		{
			dFileData = m_stVATHandCommonData.dPlateJigHeight[nHand];
			dStaticData = atof(m_pszPlateJigHeight[nHand]);

			if (dFileData != dStaticData) {
				return ERR_VAT_ERROR;
			}
		}break;
		case eCommon_Top_Plate_Thickness:
		{	
			dFileData = m_stVATHandCommonData.dTopPlateThickness[nHand];
			dStaticData = atof(m_pszTopPlateThickness[nHand]);

			if (dFileData != dStaticData) {
				return ERR_VAT_ERROR;
			}
		}break;
		case eCommon_Base_To_Plate_Jig_Height:
		{
			dFileData = m_stVATHandCommonData.dBaseToPlateJigHeight[nHand];
			dStaticData = atof(m_pszBaseToPlateJigHeight[nHand]);

			if (dFileData != dStaticData) {
				return ERR_VAT_ERROR;
			}
		}break;
		case eCommon_Base_To_Unit_Height:
		{
			for (int nUnit = 0; nUnit < VAT_MAX_BASE_TO_UNIT; nUnit++) {
				dFileData = m_stVATHandCommonData.dBaseToUnitHeight[nHand][nUnit];
				dStaticData = atof(m_pszBaseToUnitHeight[nHand][nUnit]);

				if (dFileData != dStaticData) {
					return ERR_VAT_ERROR;
				}
			}
		}break;
		default:
			break;
		}		
	}

	return ERR_VAT_NO_ERROR;
}