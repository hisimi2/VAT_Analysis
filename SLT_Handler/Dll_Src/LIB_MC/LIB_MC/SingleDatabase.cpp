#include "StdAfx.h"
#include "SingleDatabase.h"


CSingleDatabase::CSingleDatabase(void)
{
}


CSingleDatabase::~CSingleDatabase(void)
{
	if( g_pDbIf ){
		delete g_pDbIf;
		g_pDbIf = NULL;
	}
}


CDatabaseIF* CSingleDatabase::CreateDB(const char* szDriverInitFileName, int nFileNameLen)
{
	int nDbInit = 0;
	if( g_pDbIf == NULL ){
		g_pDbIf = new CDatabaseIF;
		nDbInit = g_pDbIf->Create(szDriverInitFileName, nFileNameLen, eDB_TYPE_SYSTEM);
	}
	return g_pDbIf;
}

CDatabaseIF* CSingleDatabase::GetObj()
{
	ASSERT( g_pDbIf != NULL );
	return g_pDbIf;
}
