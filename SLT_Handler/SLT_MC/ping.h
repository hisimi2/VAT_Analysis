#if !defined(AFX_PING_H__A1F06E99_6518_11D3_A2EB_0020182B2BA7__INCLUDED_)
#define AFX_PING_H__A1F06E99_6518_11D3_A2EB_0020182B2BA7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif



/////////////////////////////////////////////////////////////////////////////

BOOL InitPing(void);
void ExitPing(void);

enum	{
		PING_READY =0,
		PING_DOING,
		PING_DONE
};

enum {
		PING_NOERROR = 0,
		PING_TIMEOUT,
		PING_ERROR,
		PING_TTLOVER
};



class CPing : public CObject  
{
public:
	void ClearPing(void);
	BOOL DoPing(CString szIP,unsigned int wTimeOut=1000,unsigned int wRetry=3,unsigned int wSize = 64,unsigned int wTTL=32);
	CWinThread* m_pWTHD;
	unsigned int m_wTimeOut;
	unsigned int m_wTTL;
	unsigned int m_wRetry;
	unsigned int m_wSize;
	CString   m_szIP;
	CString   m_szRouter;
	unsigned int m_wTime;
	int		m_wStatus;
	int		m_wRetCode;
	CPing();
	virtual ~CPing();

};


#endif
