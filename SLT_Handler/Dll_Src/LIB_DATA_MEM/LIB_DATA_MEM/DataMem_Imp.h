#pragma once

#include "Sharedmem.h"
#include "MyList.h"
#include "DataMemObserver.h"
#include "ThreadBase.h"

#ifndef _ST_DATA_MEM
	#define MAX_DM_ITEM_COUNT		(1024)
	#define MAX_DM_S_ITEM_LEN		(256)
	typedef struct _ST_DATA_MEM {
		BYTE   B[MAX_DM_ITEM_COUNT];		// bit data 1024개
		int    N[MAX_DM_ITEM_COUNT];		// numeric data 1024개
		double D[MAX_DM_ITEM_COUNT];		// double data 1024개
		CHAR   S[MAX_DM_ITEM_COUNT][MAX_DM_S_ITEM_LEN];	// string data 1024개
		_ST_DATA_MEM()
		{
			memset(this, 0x00, sizeof(_ST_DATA_MEM));
		};
	}ST_DATA_MEM;
#endif //_ST_DATA_MEM

#ifndef SZ_DATA_MEM_PATH
#define SZ_DATA_MEM_PATH  _T(".\\sys_data\\DATA_MEM")
#endif

//==============================================================================
//
//==============================================================================
class CDataMem_Imp : public CWnd, public CThreadBase
{
public:
	CDataMem_Imp(void);
	~CDataMem_Imp(void);

	int	   CreateMapData(LPCTSTR strMapName);
	int    OpenMapData(LPCTSTR strMapName);
	void   DestroyMapData();

	BOOL   GB(int nOffset);
	int    SB(int nOffset, BOOL bData);

	int    GN(int nOffset);
	int    SN(int nOffset, int nData);

	double GD(int nOffset);
	int    SD(int nOffset, double dData);

	int    GS(int nOffset, char* pcBuff, int nBuffsize);
	int    SS(int nOffset, char* pcData, int nSize);

	void   BroadCast();

	int    RegisterObserver(CDataMemObserver* pObserver);
	int    UnregisterObserver(CDataMemObserver* pObserver);

//	void   ThreadProc1();

private:
	CSharedMem					m_SharedMem;
	void*						m_pData;	// ST_DATA_MEM
	CMyList						m_listDataMemObserver;
	char*						m_pcMapName;

private:
	BOOL    m_fnCreateDirectory( LPCSTR pszDirectory );
};

