#pragma once

struct stMasterMacInfo
{
	int nStation;
	int nOnOff;
};

struct stMasterMacFileCheck
{
	BOOL bFileModifyTimeOver;
};

struct stMasterMacFileNotExist
{
	BOOL bFileNotExist;
};


class CMasterMac
{
public:
	CMasterMac();
	~CMasterMac();

	stMasterMacInfo m_stMasterMacInfo;
	stMasterMacFileCheck m_stMasterMacFileCheck;
	stMasterMacFileNotExist m_stMasterMacFileNotExist;

	std::vector<stMasterMacInfo> m_veMasterMacInfo;
	std::vector<stMasterMacFileCheck> m_veMasterMacFileCheck;
	std::vector<stMasterMacFileNotExist> m_veMasterMacFileNotExist;

	int CBFileDataRead(CString sFilePath);
	int CBFileModifyTimeCheck(CString sFilePath);
	int CBFileNotExist(CString sFilePath);

	CString CBFileModifyIndex();
	CString CBDataOnOffIndex();
	CString CBFileNotExistIndex();


	int CBFileAccessWrite(CString sFilePath, int nIndex);
};

