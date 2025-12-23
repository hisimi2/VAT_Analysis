// ParamList.h: interface for the CParamList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMLIST_H__F4FE6FF7_C56D_4933_98D4_6EC59A789640__INCLUDED_)
#define AFX_PARAMLIST_H__F4FE6FF7_C56D_4933_98D4_6EC59A789640__INCLUDED_


//==============================================================================
//
//==============================================================================
#include <AFXTEMPL.H>


//==============================================================================
//
//==============================================================================
#ifndef MAX_BUFF
	#define MAX_BUFF			(2500)
#endif // MAX_BUFF


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//==============================================================================
//
//==============================================================================
class CParamList  
{
public:
	CParamList();
	CParamList(const CParamList& src);
	virtual ~CParamList();

	CParamList& operator = (const CParamList& src);

	void	m_fnSetDelimiter(char* szDelimiter, int nLen);
	void    m_fnGetDelimiter(char* szDelimiter, int nRangeLength);
	void	m_fnSetDelimitedText(char* szText, int nLen);
	void    m_fnGetDelimitedText(char* szText);
	void    m_fnGetDelimitedText(CString& strBuff);

	void	m_fnAddItemFormat(const char* szfmt, ...);
	void	m_fnAddItem(char* szValue);
	void	m_fnAddItem(int nValue);
	void	m_fnAddItem(double dValue);

	int		m_fnGetCount();

	CString m_fnGetItemString(int nIndex);
	int		m_fnGetItemInt(int nIndex);
	double  m_fnGetItemDouble(int nIndex);

	int		m_fnGetItemString(int nIndex, char* szValue);
	int		m_fnGetItemInt(int nIndex, int* pnValue);
	int		m_fnGetItemDouble(int nIndex, double* pdValue);

	void	m_fnClear();

private:
	CList<char*, char*> m_List;
	char*    m_pcDelimiter;
	int      m_nDelimiterLen;
	char*    m_pcDelimitedText;
	int      m_nDelimitedTextLen;

};

#endif // !defined(AFX_PARAMLIST_H__F4FE6FF7_C56D_4933_98D4_6EC59A789640__INCLUDED_)
