#pragma once
/*==================================================================================
// Buffer가 필요 없는 Data 관리.
// 불변의 Data 관리.
ex) PKG INFO, COK INFO
=====================================================================================*/
#include "../../Include/DEF_COMMON.h"
#include "../../Include/DEF_VAT_COMMON.h"
#include "VAT_FILE_SECTION.h"

#define VAT_STATIC_DATA_PATH_FIXED_DESIGN_VALUE		(0) //수정 불가능한 Design Value


//================================================================================================================
//COMMENT : 수정 불가능 한 Design Value DEFINE
//================================================================================================================
#define VAT_D_VAL_FEEDER_PLATE_JIG_TARGETS_DIS			(35500) //35.5mm



//================================================================================================================
//COMMENT : Hand별로 관리 되어야 하는 구조체
//================================================================================================================
typedef struct _ST_VAT_HAND_COMMON_INFO
{
	//기준 Picker
	std::pair<int, int> pairStdPicker[VAT_HAND_MAX]; 
	
	//Plate Jig 
	double dPlateJigHeight[VAT_HAND_MAX];
	double dTopPlateThickness[VAT_HAND_MAX];

	//Base 높이
	double dBaseToPlateJigHeight[VAT_HAND_MAX];
	double dBaseToUnitHeight[VAT_HAND_MAX][VAT_MAX_BASE_TO_UNIT];

	_ST_VAT_HAND_COMMON_INFO() {
		memset(this, 0x00, sizeof(_ST_VAT_HAND_COMMON_INFO));
	}
}ST_VAT_HAND_COMMON_INFO;

// [1]기준 Picker
static char *m_pszStdPicker[VAT_HAND_MAX][VAT_MAX_AXIS_XY] = {
	{ "2", "1" }, //VAT_HAND_LOAD_TRAYPP
	{ "2", "1" }, //VAT_HAND_UNLOAD_TRAYPP
	{ "1", "1" }, //VAT_HAND_LOAD_TESTPP1(6번 Picker)
	{ "2", "1" }, //VAT_HAND_UNLOAD_TESTPP1(7번 Picker)
	{ "2", "1" }, //VAT_HAND_LOAD_TESTPP2(7번 Picker)
	{ "1", "1" }, //VAT_HAND_UNLOAD_TESTPP2(6번 Picker)
};

// [2]Plate Jig
static char *m_pszPlateJigHeight[VAT_HAND_MAX] = {
	"24000.0",	//VAT_HAND_LOAD_TRAYPP		(24mm)
	"84000.0",	//VAT_HAND_UNLOAD_TRAYPP	(84mm)	
	"84000.0",	//VAT_HAND_LOAD_TESTPP1		(84mm)	
	"84000.0",	//VAT_HAND_UNLOAD_TESTPP1	(84mm)	
	"84000.0",	//VAT_HAND_LOAD_TESTPP2		(84mm)	
	"84000.0",	//VAT_HAND_UNLOAD_TESTPP2	(84mm)	
};

static char *m_pszTopPlateThickness[VAT_HAND_MAX] = {
	"6000.0",	//VAT_HAND_LOAD_TRAYPP		(6mm)
	"10000.0",	//VAT_HAND_UNLOAD_TRAYPP	(10mm)	
	"10000.0",	//VAT_HAND_LOAD_TESTPP1		(10mm)	
	"10000.0",	//VAT_HAND_UNLOAD_TESTPP1	(10mm)	
	"10000.0",	//VAT_HAND_LOAD_TESTPP2		(10mm)	
	"10000.0",	//VAT_HAND_UNLOAD_TESTPP2	(10mm)	
};

// [3]Base 높이
static char *m_pszBaseToPlateJigHeight[VAT_HAND_MAX] = {
	"75000.0",	//VAT_HAND_LOAD_TRAYPP		(75mm)
	"108000.0",	//VAT_HAND_UNLOAD_TRAYPP	(108mm)	
	"106000.0",	//VAT_HAND_LOAD_TESTPP1		(106mm)
	"106000.0",	//VAT_HAND_UNLOAD_TESTPP1	(106mm)
	"106000.0",	//VAT_HAND_LOAD_TESTPP2		(106mm)
	"106000.0",	//VAT_HAND_UNLOAD_TESTPP2	(106mm)
};

static char *m_pszBaseToUnitHeight[VAT_HAND_MAX][VAT_MAX_BASE_TO_UNIT] = {
	{ "51000.0", "66500.0", "0.0" },				//VAT_HAND_LOAD_TRAYPP
	{ "99000.0", "99500.0", "0.0" },				//VAT_HAND_UNLOAD_TRAYPP
	{ "99000.0", "66000.0", "98500.0" },				//VAT_HAND_LOAD_TESTPP1
	{ "99000.0", "66000.0", "98500.0" },				//VAT_HAND_UNLOAD_TESTPP1
	{ "99000.0", "66000.0", "98500.0" },				//VAT_HAND_LOAD_TESTPP2
	{ "99000.0", "66000.0", "98500.0" },				//VAT_HAND_UNLOAD_TESTPP2
};



class CVAT_CommonStaticData
{
public:
	CVAT_CommonStaticData();
	~CVAT_CommonStaticData();

public:
	ST_VAT_FILE_SECTION				m_stVATFileSection;

	ST_VAT_HAND_COMMON_INFO			m_stVATHandCommonData;

private:

public:
	CString GetStaticDataFilePath(int nMode);
	void IsExistVATDirectory();

	void LoadVATStaticData();
	//void SaveVATStaticData();


	//구조체 Data Save
	void LoadFixedDesignValueData();

private:
	//외부에서 Save 불가.
	void SaveHandCommonData(int nType);

	enum _eCommon_Type
	{
		eCommon_STDPicker =0,
		eCommon_Plate_Jig_Height,
		eCommon_Top_Plate_Thickness,
		eCommon_Base_To_Plate_Jig_Height,
		eCommon_Base_To_Unit_Height,

		eCommon_Type_Max,
	};

	int CheckCommonDataValidate(int nType);
};

