#pragma once

#include "DEF_COK_STRUCT.h"

class CSystemData
{
public:
	CSystemData(void);
	~CSystemData(void);

	void Initialize(char* szCokName);
	void Finalize();

	int UpdateFileDataToSM();

private:
	ST_SLT_SYSTEM m_SltData;
	char m_szCokName[256];

	void UpdateBasicDataToSM(ST_BASIC_INFO lpstDataBasic);
	void UpdateCokDataToSM(ST_COK_INFO lpstDataCOK);
	void UpdateSltPgmDataToSM(ST_SLT_PGM lpstData);
	//void CSystemData::UpdateCategoryDataToSM(ST_CATE_INFO lpstDataCategory);
	void UpdateFTPFileToSM();
	void Update2DMarkToSM();
};
