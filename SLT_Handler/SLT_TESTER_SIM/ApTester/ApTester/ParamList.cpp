// ParamList.cpp: implementation of the CParamList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParamList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CParamList::CParamList()
{
	m_pcDelimiter = NULL;
	m_nDelimiterLen = 2;
	m_pcDelimiter = new char [m_nDelimiterLen];
	memset(m_pcDelimiter, 0x00, m_nDelimiterLen);
	m_pcDelimitedText = NULL;
	m_nDelimitedTextLen = 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-22]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 복사생성자
==============================================================================*/
CParamList::CParamList(const CParamList& src)
{
	m_pcDelimiter = NULL;
	m_nDelimiterLen = strlen(src.m_pcDelimiter);
	m_pcDelimiter = new char [m_nDelimiterLen+1];
	memset(m_pcDelimiter, 0x00, m_nDelimiterLen+1);
	strcpy_s(m_pcDelimiter, m_nDelimiterLen+1, src.m_pcDelimiter);

	m_pcDelimitedText = NULL;
	m_nDelimitedTextLen = 0;

	POSITION pos = src.m_List.GetHeadPosition();
	for(int i=0; i<src.m_List.GetCount(); i++)
	{		
		char* pcValue = src.m_List.GetNext(pos);
		int nLen = strlen(pcValue)+1;
		char* pcNewValue = new char[nLen];
		memset(pcNewValue, 0x00, nLen);
		strcpy_s(pcNewValue, nLen, pcValue);
		m_List.AddTail(pcNewValue);
	}
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CParamList::~CParamList()
{
	m_fnClear();	
	if( m_pcDelimiter != NULL ){
		delete [] m_pcDelimiter;
		m_pcDelimiter = NULL;
		m_nDelimiterLen = 0;
	}
	if( m_pcDelimitedText != NULL ){
		delete [] m_pcDelimitedText;
		m_pcDelimitedText = NULL;
		m_nDelimitedTextLen = 0;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-22]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CParamList& CParamList::operator = (const CParamList& src)
{
	if( m_pcDelimiter ){
		delete [] m_pcDelimiter;
		m_pcDelimiter = NULL;
		m_nDelimiterLen = 0;
	}
	m_nDelimiterLen = strlen(src.m_pcDelimiter);
	m_pcDelimiter = new char [m_nDelimiterLen+1];
	ZeroMemory(m_pcDelimiter, m_nDelimiterLen+1);
	strcpy_s(m_pcDelimiter, m_nDelimiterLen+1, src.m_pcDelimiter);

	m_fnClear();
	POSITION pos = src.m_List.GetHeadPosition();
	for(int i=0; i<src.m_List.GetCount(); i++)
	{
		char* pcValue = src.m_List.GetNext(pos);
		int nLen = strlen(pcValue)+1;
		char* pcNewValue = new char[nLen];
		memset(pcNewValue, 0x00, nLen);
		strcpy_s(pcNewValue, nLen, pcValue);
		m_List.AddTail(pcNewValue);
	}
	return *this;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnSetDelimiter(char* szDelimiter, int nLen)
{
	if( m_pcDelimiter != NULL ){
		delete [] m_pcDelimiter;
		m_pcDelimiter = NULL; 
		m_nDelimiterLen = 0;
	}
	m_nDelimiterLen = nLen+1;
	m_pcDelimiter = new char [m_nDelimiterLen];
	memset(m_pcDelimiter, 0x00, m_nDelimiterLen);

	strncpy(m_pcDelimiter, szDelimiter, nLen);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnGetDelimiter(char* szDelimiter, int nRangeLength)
{
	sprintf_s(szDelimiter, nRangeLength , "%s", m_pcDelimiter);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : // 1,2,3,4,5,6,7,8,9
==============================================================================*/
void CParamList::m_fnSetDelimitedText(char* szText, int nDelimitedTextLen)
{
	char szValue[128]={0};
	char* szBuff = NULL;
	char *pEnd=NULL;

	// 	ASSERT(szText!=NULL);
	// 	ASSERT(strlen(szText)>0);
	if( szText == NULL ) return;

	long lLen = strlen(szText);

	if( lLen <= 0 ) return;

// 	ASSERT(szText!=NULL);
// 	ASSERT(strlen(szText)>0);
//	ASSERT(strlen(szText)<sizeof(szBuff));
	szBuff = new char [nDelimitedTextLen+1];
	memset(szBuff, 0x00, nDelimitedTextLen+1 );

	szText[nDelimitedTextLen] = '\0';

	if( m_pcDelimitedText != NULL ){
		delete [] m_pcDelimitedText;
		m_pcDelimitedText = NULL;
		m_nDelimitedTextLen = 0;
	}
	m_nDelimitedTextLen = nDelimitedTextLen+1;
	m_pcDelimitedText = new char [m_nDelimitedTextLen];
	memset(m_pcDelimitedText, 0x00, m_nDelimitedTextLen);

	strncpy(m_pcDelimitedText, szText, nDelimitedTextLen);

	strncpy(szBuff, szText, nDelimitedTextLen);

	pEnd = strstr(szBuff, m_pcDelimiter);

	while(pEnd != NULL)
	{
		int nValueLen = (pEnd - szBuff);
		char* pcValue = new char [nValueLen+1];
		memset(pcValue, 0x00, nValueLen+1);

		//strncpy(pcValue, szBuff, nValueLen);

		memcpy(pcValue, szBuff, nValueLen);
		m_List.AddTail(pcValue);

		strcpy(szBuff, pEnd+strlen(m_pcDelimiter) );

		pEnd = strstr(szBuff, m_pcDelimiter);
	}
	
	// first or last value
	int nValueLen = strlen(szBuff);
	char* pcValue = new char [nValueLen+1];
	memset(pcValue, 0x00, nValueLen+1);
	
	if(nValueLen != 0){		// By ksj [150611] is not end delimiter problem fix
		strncpy(pcValue, szBuff, nValueLen);
		m_List.AddTail(pcValue);
	}	

	//m_List.AddTail(pcValue);

	if( szBuff != NULL ){
		delete [] szBuff;
		szBuff = NULL;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnGetDelimitedText(char* szText)
{
	char* pBuff = new char [102400];

	memset(pBuff, 0, 102400);

	int i=0;
	for(i=0; i<m_List.GetCount()-1; i++){
		strcat(pBuff, m_fnGetItemString(i));
		strcat(pBuff, m_pcDelimiter);
	}
	if( m_List.GetCount() > 0){
		strcat(pBuff, m_fnGetItemString(i));
	}
	strcpy(szText, pBuff);

	delete [] pBuff;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-05]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnGetDelimitedText(CString& strBuff)
{
	for( int i=0; i<m_List.GetCount(); i++){
		CString strItem = m_fnGetItemString(i);
		strBuff += strItem;
		if( (m_pcDelimiter != NULL) && (i < m_List.GetCount()-1) ){
			strBuff += m_pcDelimiter;
		}
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CParamList::m_fnGetCount()
{
	return m_List.GetCount();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CString CParamList::m_fnGetItemString(int nIndex)
{
	CString strValue=_T("");

	POSITION pos = m_List.FindIndex(nIndex);
	strValue = m_List.GetAt(pos);

	return strValue;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CParamList::m_fnGetItemInt(int nIndex)
{
	int nValue=0;

	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};

	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) ); //by wslee [150615] sprintf -> sprintf_s 변경

	nValue = atoi(szValue);

	return nValue;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double CParamList::m_fnGetItemDouble(int nIndex)
{
	double dValue=0.0;
	
	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};

	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) ); //by wslee [150615] sprintf -> sprintf_s 변경

	dValue = atof(szValue);

	return dValue;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnClear()
{
	while( !m_List.IsEmpty() )
	{
		char* p = m_List.GetHead();
		delete [] p;
		p = NULL;
		m_List.RemoveHead();
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 0 = OK, -1 = Out of Pos
  COMMENTS : 
==============================================================================*/
int	CParamList::m_fnGetItemString(int nIndex, char* szValue)
{
	if( nIndex >= m_fnGetCount() ) return -1;
	char szTemp[MAX_BUFF] = {0};
	
	POSITION pos = m_List.FindIndex(nIndex);
	sprintf_s(szTemp,sizeof(szTemp), "%s", m_List.GetAt(pos)); //by wslee [150615] sprintf -> sprintf_s 변경

	strcpy(szValue, szTemp);
	
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int	CParamList::m_fnGetItemInt(int nIndex, int* pnValue)
{
	if( nIndex >= m_fnGetCount() ) return -1;
	
	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};
	
	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) ); //by wslee [150615] sprintf -> sprintf_s 변경
	
	*pnValue = atoi(szValue);
	
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int	CParamList::m_fnGetItemDouble(int nIndex, double* pdValue)
{
	if( nIndex >= m_fnGetCount() ) return -1;
	
	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};
	
	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) ); //by wslee [150615] sprintf -> sprintf_s 변경
	
	*pdValue = atof(szValue);
	
	return 0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-08-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnAddItemFormat(const char* szfmt, ...)
{
	char szBuff[MAX_BUFF]={0};
	va_list va;
	va_start(va, szfmt);
	vsprintf(szBuff, szfmt, va);
	va_end(va);
	m_fnAddItem(szBuff);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnAddItem(char* szValue)
{
	int nLen = strlen(szValue)+1;
	char* pcNewValue = new char[nLen];
	memset(pcNewValue, 0x00, nLen);
	strcpy_s(pcNewValue, nLen, szValue);
	m_List.AddTail(pcNewValue);	
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-22]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnAddItem(int nValue)
{
	char szValue[128]={0,};
	sprintf_s(szValue, sizeof(szValue), "%d", nValue);
	int nLen = strlen(szValue)+1;
	char* pcNewValue = new char[nLen];
	memset(pcNewValue, 0x00, nLen);
	strcpy_s(pcNewValue, nLen, szValue);
	m_List.AddTail(pcNewValue);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-22]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CParamList::m_fnAddItem(double dValue)
{
	char szValue[128]={0,};
	sprintf_s(szValue, sizeof(szValue), "%f", dValue);
	int nLen = strlen(szValue)+1;
	char* pcNewValue = new char[nLen];
	memset(pcNewValue, 0x00, nLen);
	strcpy_s(pcNewValue, nLen, szValue);
	m_List.AddTail(pcNewValue);
}
