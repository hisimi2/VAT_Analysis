// RegulatorComm.h: interface for the CRegulatorComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGULATORCOMM_H__34304B73_C820_4B98_BF14_6B3E6A5972F2__INCLUDED_)
#define AFX_REGULATORCOMM_H__34304B73_C820_4B98_BF14_6B3E6A5972F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadBase.h"
#include "WinRs232c.h"
#include "RegulatorIF.h"

#include "DEF_ERROR_CODE.h"

class  CRegulatorComm : public CWnd, public CThreadBase
{
public:
	CRegulatorComm();
	virtual ~CRegulatorComm();

public:
	int		GetDecFromMpa(double dMpa);
	BOOL	IsConnected()				{ return m_bConnected; };
	void	GetRecvData(int *nState=NULL, double *dMPa=NULL);
	void	SetRecvData(int nState, int nDecMPa=0);
	int		GetCommand()				{ return m_nCommand; };
	void    SetCommand(int nCommand, double dMPa = 0.0);
	void	Close();
	double  GetSettingMpa();
	BOOL    RegulatorPressCheck(double dRegulatorPermit);

	void	ThreadProc1();
	void	SetRegulator(double dSetPressure);

	BOOL	InitRs232c(CWnd* pParentWnd, ST_REGUL_DATA& stSetData, char* szLogfileName/*, FN_PARENT_CALLBACK cbAlarm*/);

	static void		cbReceive(CWnd* pWnd, LPARAM lParam, WPARAM wParam);
	void			OnReceive(char* lParam, WPARAM wParam);

public:
private:
	CWinRs232c  m_cWrs232C;
	int			m_nCommand;
	int			m_nState;
	double		m_dMPa;
	BOOL		m_bConnected;
	double		m_dSettingPressure;

	char		m_szRxBuff[MAX_BUFF];
	DWORD		m_dwRxCnt;

	CWnd*		m_pParentWnd;
	FN_PARENT_CALLBACK m_cbAlarm;

	bool		m_bCommandLock;
	bool GetCommandLock() const { return m_bCommandLock; }
	void SetCommandLock(bool val) { m_bCommandLock = val; }
};



#endif // !defined(AFX_REGULATORCOMM_H__34304B73_C820_4B98_BF14_6B3E6A5972F2__INCLUDED_)
