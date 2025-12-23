#pragma once

#include "zip.h"
#include "unzip.h"

class CNetDriver
{
public:
	CNetDriver(void);
	~CNetDriver(void);

	int CopyFileFromMac(int nFileType/*enum eFileTransType{*/, int nPressIdx, CString strLotStartTm, CString strLotEndTm);
	int MakeZipMacFile(int nPressIdx);
	int DelFileFromMac(int nFileType/*enum eFileTransType{*/, CString szSrcPath);
	int Initialize(CWnd* pMainFrameWnd);
private:
	CWnd* m_pMainFrameWnd;
};

