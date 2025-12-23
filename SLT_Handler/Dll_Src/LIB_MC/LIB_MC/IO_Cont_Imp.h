#pragma once

#include "ThreadBase.h"

class CIO_Cont_Imp : public CThreadBase
{
public:
	CIO_Cont_Imp(void);
	~CIO_Cont_Imp(void);

	int		Initialize(const char* szDriverIniFileName, int nFileNameLen);
	void	Finalize();

	static UINT	ThreadProc_Running_IO(LPVOID lpVoid);
	BOOL m_bStopThread;
	CEvent m_evtTerminated;

private:
	int     m_nRefObjCount;
};

