#pragma once

#include <afxmt.h>
#include <deque>

class AFX_EXT_CLASS COEEFile : public CWnd
{
public:
	COEEFile(void);
	~COEEFile(void);

	void Initialize();
	void Finalize();
	void PutOEE(CString strData);
	void WriteOEE(std::deque<CString> &qOEEData);
	static UINT WriteProcedure(LPVOID lpVoid);

	CCriticalSection m_csOEE;
	
	std::deque<CString> m_qOEEData;

	HANDLE m_hThread;
	CString m_strFileName;
	CString m_strFileData;
};

