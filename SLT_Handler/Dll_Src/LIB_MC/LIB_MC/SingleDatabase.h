#pragma once

class CSingleDatabase
{
public:
	CSingleDatabase(void);
	~CSingleDatabase(void);

	CDatabaseIF* CreateDB(const char* szDriverInitFileName, int nFileNameLen);
	CDatabaseIF* GetObj();
};