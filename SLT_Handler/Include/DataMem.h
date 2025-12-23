#pragma once

class CDataMemObserver;

class AFX_EXT_CLASS CDataMem
{
public:
	CDataMem(void);
	~CDataMem(void);

	int    Create(LPCTSTR strMapName);
	int    Open(LPCTSTR strMapName);
	void   Destroy();

	BOOL   GB(int nOffset);
	int    SB(int nOffset, BOOL bData);

	int    GN(int nOffset);
	int    SN(int nOffset, int nData);

	double GD(int nOffset);
	int    SD(int nOffset, double dData);

	int    GS(int nOffset, char* pcBuff, int nBuffSize);
	int    SS(int nOffset, char* pcData, int nLen);

	void   BroadCast();

	int    RegisterObserver(CDataMemObserver* pObserver);
	int    UnregisterObserver(CDataMemObserver* pObserver);

private:
	CWnd* m_pDataObj;
};

