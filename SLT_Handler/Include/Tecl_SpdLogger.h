#pragma once

#define TW_SPD_LOGGER_DLL_DECEL __declspec(dllexport)
#include <string>

class TW_SPD_LOGGER_DLL_DECEL CTecl_SpdLogger
{
private:
	// Member Data
	std::string m_strLogName;
	std::string m_strDirectory;
	std::string m_strLogExt;

	int	m_nMaxFileSize;		// [MB]
	int	m_nMaxFileCnt;

	// Member Function
	void UpdateLogDirectory();

public:
	CTecl_SpdLogger(bool bIsAsyncLoging = false);
	~CTecl_SpdLogger();

	bool m_bIsAsyncLoging;

	void SetFileName(const char* pszFileName);
	void LOG(const char* szLogData);
	void ResetLogger();

	//Set Member Data()
	void SetMaxFileSize(int nMaxFileSize);
	void SetMaxFileCnt(int nMaxFileCnt);

	//Get Member Data()
	int	GetMaxFileSize();
	int	GetMaxFileCnt();
};

