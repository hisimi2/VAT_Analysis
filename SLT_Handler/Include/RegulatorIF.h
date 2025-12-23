#pragma once

#include "WinRs232c.h"

typedef struct _ST_REGUL_DATA{
	int   nPort;
	DWORD dwRate;
	BYTE  btParity;
	BYTE  btSize;
	BYTE  btStop;

	_ST_REGUL_DATA() {	clear(); };

	void clear()
	{
		nPort = 0;
		dwRate = 0;
		btParity = 0;
		btSize = 0;
		btStop = 0;
	}
}ST_REGUL_DATA;

// REGULATOR DEFINE
#define SET		        (0)
#define INC		        (1)
#define DEC		        (2)
#define REQ		        (3)
#define MON		        (4)
#define SET_NN			(0)
#define OUT_OF_RANGE	(1)
#define UNKNOWN_COMMAND	(2)
#define SET_MM			(3)

class AFX_EXT_CLASS CRegulatorIF : public CWnd
{
public:
	CRegulatorIF(void);
	~CRegulatorIF(void);

	BOOL	Initialize(CWnd* pParentWnd, ST_REGUL_DATA &stSetData, char* szLogFile/*, FN_PARENT_CALLBACK cbAlarm*/);
	void	Finalize();
	BOOL	IsConnected();

	int		GetCommand();
	void    SetCommand(int nCommand, double dMPa = 0.0);

	void	SetRegulator(double dSetPressure);
	BOOL    GetPress(double dRegulatorPermit); //RegulatorPressCheck

	int		GetDecFromMpa(double dMpa);
	double  GetSettingMpa();

	void	GetRecvData(int *nState=NULL, double *dMPa=NULL);
	void	SetRecvData(int nState, int nDecMPa=0);

    BOOL    SetReconnected();

private:
	CWnd* m_pDrv;
	ST_REGUL_DATA m_stSetData;
	char m_szLogfilename[512];
	CWnd* m_pParentWnd;
};

