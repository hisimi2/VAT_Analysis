#pragma once

#include "DlgAioSimul.h"
#include <vector>

class CAIO_SIMUL_DRV
{
public:
	CAIO_SIMUL_DRV(void);
	~CAIO_SIMUL_DRV(void);

	int  AIO_INIT(const char* szIniFileName);

	int SetSimDataAi(int nIndex, double dValue);
	double GetSimDataAi(int nIndex);
	int SetSimDataAo(int nIndex, double dValue);
	double GetSimDataAo(int nIndex);

	int GetPointMax(int nInOrOut);
	void ShowDialogSimul();
protected:
	CDlgAioSimul* m_pDlgSimul;
	std::vector<double> m_vAiSimData;
	std::vector<double> m_vAoSimData;
	CMyList m_AiDbInfoList;
	CMyList m_AoDbInfoList;
};

