#if !defined(AFX_WINRS232C_H__26C55AD6_88E6_11D3_A4AB_0020AF26BBA4__INCLUDED_)
#define AFX_WINRS232C_H__26C55AD6_88E6_11D3_A4AB_0020AF26BBA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WinRs232c.h : header file
//

#include "Logger.h"

#ifndef MAX_BUFF
	#define MAX_BUFF			(1024)
#endif // MAX_BUFF

/////////////////////////////////////////////////////////////////////////////
// CWinRs232c ¼ö½Å Event

// Stop bits
#define	RS_STOP1	0
#define	RS_STOP1_5	1
#define	RS_STOP2	2

// Parity bits
#define	RS_PARITY_NONE	0	// 'N'
//#define NONE 0
#define	RS_PARITY_ODD	1	// 'O'
#define	RS_PARITY_EVEN	2	// 'E'
#define	RS_PARITY_MARK	3	// 'M'
#define	RS_PARITY_SPACE	4	// 'S'

#ifndef _DEF_FN_PARENT_CALLBACK_
#define _DEF_FN_PARENT_CALLBACK_
typedef void (*FN_PARENT_CALLBACK) (CWnd* pWnd, LPARAM lParam, WPARAM wParam);
#endif //_DEF_FN_PARENT_CALLBACK_

/////////////////////////////////////////////////////////////////////////////
// CWinRs232c class

class AFX_EXT_CLASS CWinRs232c
{
// Construction
public:
	CWinRs232c();
	virtual ~CWinRs232c();

	BOOL Create(CWnd* pParent, DWORD nRcvMaxLen=8192, DWORD nSndMaxLen=8192);
	BOOL m_fnInitial(int nPort, DWORD nRate, BYTE nParity , BYTE nSize, BYTE nStop, char *szLogFileName, int nLogPeriod, void (*FN_PARENT_CALLBACK)(CWnd* pWnd, LPARAM lParam, WPARAM	wParam));

	BOOL Open(CString strPort, DWORD nRate, BYTE nParity , BYTE nSize, BYTE nStop);
	void Close();

	DWORD Receive(LPVOID lpData);
	BOOL Send(LPVOID lpData, DWORD nLength);

	BOOL isOpen();

	int m_fnGetDTR();
	int m_fnSetDTR(int nMode);

// Attributes
public:
	HANDLE	m_Handle;

	DWORD	m_nRcvMaxLen;
	DWORD	m_nSndMaxLen;

	OVERLAPPED	m_ioRead;
	OVERLAPPED	m_ioWrite;

	CWnd*	m_pParent;
	CWinThread*	Receive232WinThread;

	BYTE*	m_Data;
	DWORD	m_RcvLen;

	FN_PARENT_CALLBACK m_fnParentCallBack;

protected:
	HANDLE m_hFModuleSemaphore;
	//CLogger m_Logger;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINRS232C_H__26C55AD6_88E6_11D3_A4AB_0020AF26BBA4__INCLUDED_)
