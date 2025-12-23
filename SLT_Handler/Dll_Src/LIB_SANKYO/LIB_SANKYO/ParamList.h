// ParamList.h: interface for the CParamList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMLIST_H__F4FE6FF7_C56D_4933_98D4_6EC59A789640__INCLUDED_)
#define AFX_PARAMLIST_H__F4FE6FF7_C56D_4933_98D4_6EC59A789640__INCLUDED_

#include <AFXTEMPL.H>
#include <iostream>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BUFF_PARAMLIST    (1024)

class CParamList  
{
public:
	CParamList();
	virtual ~CParamList();

	void	m_fnSetDelimiter(char* szDelimiter, int nLen);
	void    m_fnGetDelimiter(char* szDelimiter, int nSize);
	void	m_fnSetDelimitedText(char* szText, int nLen, int nLastDelimiter=0, int nIncludeDelimiter=0);
	void    m_fnGetDelimitedText(char* szText, int nSize);

	void	m_fnAddItemString(char* szValue);

	int		m_fnGetCount();

	CString m_fnGetItemString(int nIndex);
	int		m_fnGetItemInt(int nIndex);
	double  m_fnGetItemDouble(int nIndex);

	int		m_fnGetItemString(int nIndex, char* szValue, int nSize);
	int		m_fnGetItemInt(int nIndex, int* pnValue);
	int		m_fnGetItemDouble(int nIndex, double* pdValue);
	std::pair<std::string, std::string>		m_fnGetPasedItemString(char* szValue);

	void	m_fnClear();

	BOOL    m_fnDoesItSplit();
	void    m_fnCopySplit(char* szBuff, int* pnSplitCount);

private:
	CList<char*, char*> m_List;
	char*    m_pcDelimiter;
	int      m_nDelimiterLen;
	char*    m_pcDelimitedText;
	int      m_nDelimitedTextLen;
	char*    m_pcSplit;
	int      m_nSplitCount;
};

#endif // !defined(AFX_PARAMLIST_H__F4FE6FF7_C56D_4933_98D4_6EC59A789640__INCLUDED_)
