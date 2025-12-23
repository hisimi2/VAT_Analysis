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

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
CParamList::CParamList()
{
	m_pcDelimiter = NULL;
	m_nDelimiterLen = 2;
	m_pcDelimiter = new char [m_nDelimiterLen];
	memset(m_pcDelimiter, 0x00, m_nDelimiterLen);
	m_pcDelimitedText = NULL;
	m_nDelimitedTextLen = 0;
	m_pcSplit = NULL;
	m_nSplitCount = 0;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
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
	if( m_pcSplit != NULL ){
		delete [] m_pcSplit;
		m_pcSplit = NULL;
		m_nSplitCount = 0;
	}
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
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

	strncpy_s(m_pcDelimiter, m_nDelimiterLen, szDelimiter, nLen);
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
void CParamList::m_fnGetDelimiter(char* szDelimiter, int nSize)
{
	sprintf_s(szDelimiter, nSize, "%s", m_pcDelimiter);
}


/*==================================================================================================
  DESCRIPT : "100,200,300,400,500,600,700,800,900" 일 경우 list에 아래와 같이 들어간다.
             list[0] = "100", 
			 list[1] = "200", 
			     ... 중략
			 list[7] = "800", 
			 list[8] = "900"
  ARGUMENT : szText             :
             nDelimitedTextLen  :
			 nLastDelimiter     : 0 = 문자열 마지막에 반드시 Delimiter가 있을 필요는 없음.
			                      1 = 문자열 마지막에 반드시 Delimiter가 있어야 함.
								      Delimiter가 없을 경우 문자열이 잘린것으로 간주하고 Split 보관함에 저장 함.
			 nIncludeDelimiter  : 0 = 문자열을 저장 할 때 Delimiter를 포함 시키지 않음.
			                      1 = 문자열을 저장 할 때 Delimiter를 포함 시킴.
  RETURN   : 
==================================================================================================*/
void CParamList::m_fnSetDelimitedText(char* szText, int nDelimitedTextLen, int nLastDelimiter/*=0*/, int nIncludeDelimiter/*=0*/)
{
	char szValue[128] = {0};
	char* szBuff      = NULL;
	char *pEnd        = NULL;
	int   nValueLen   = 0;
	char* pcValue     = NULL;
	int   nRemainLen  = 0;
	int   nSize       = 0;

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

	nRemainLen = nDelimitedTextLen;

	strncpy_s(m_pcDelimitedText, m_nDelimitedTextLen, szText, nDelimitedTextLen);

	strncpy_s(szBuff, nDelimitedTextLen+1, szText, nDelimitedTextLen);

	pEnd = strstr(szBuff, m_pcDelimiter);
	
	while(pEnd != NULL)
	{
		nValueLen = (pEnd - szBuff);
		nSize     = nValueLen+m_nDelimiterLen+1;
		pcValue = new char [nSize];
		memset(pcValue, 0x00, nSize);

		memcpy(pcValue, szBuff, nValueLen);
		if( nIncludeDelimiter == 1 ){
			strcat_s(pcValue, nSize, m_pcDelimiter );
		}
		m_List.AddTail(pcValue);
		nRemainLen -= nValueLen;

		strcpy_s(szBuff, nDelimitedTextLen, pEnd+strlen(m_pcDelimiter) );

		pEnd = strstr(szBuff, m_pcDelimiter);
	}

	if( nRemainLen > 0 )
	{
		if( nLastDelimiter == 1 ){
			// 잘림 처리.
			nValueLen = strlen(szBuff);
			m_pcSplit = new char [nValueLen+1];
			memset(m_pcSplit, 0x00, nValueLen+1);
			strncpy_s(m_pcSplit, nValueLen+1, szBuff, nValueLen);
			m_nSplitCount = nValueLen;
		}else{   
			// 잘림 처리 무시.
			// first or last value
			nValueLen = strlen(szBuff);
			nSize = nValueLen+m_nDelimiterLen+1;
			pcValue = new char [nSize];
			memset(pcValue, 0x00, nSize);
			strncpy_s(pcValue, nSize, szBuff, nValueLen);
			if( nIncludeDelimiter == 1 ){
				strcat_s( pcValue, nSize, m_pcDelimiter);
			}
			m_List.AddTail(pcValue);
		}
	}

	if( szBuff != NULL ){
		delete [] szBuff;
		szBuff = NULL;
	}
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
void CParamList::m_fnGetDelimitedText(char* szText, int nSize)
{
	char* pBuff = new char [102400];

	memset(pBuff, 0, 102400);

	int i=0;
	for(i=0; i<m_List.GetCount()-1; i++){
		strcat_s(pBuff, 102400, m_fnGetItemString(i));
		strcat_s(pBuff, 102400, m_pcDelimiter);
	}
	if( m_List.GetCount() > 0){
		strcat_s(pBuff, 102400, m_fnGetItemString(i));
	}
	strcpy_s(szText, nSize, pBuff);

	delete [] pBuff;
}


/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
int CParamList::m_fnGetCount()
{
	return m_List.GetCount();
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
CString CParamList::m_fnGetItemString(int nIndex)
{
	CString strValue="";

	POSITION pos = m_List.FindIndex(nIndex);
	strValue = m_List.GetAt(pos);

	return strValue;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
int CParamList::m_fnGetItemInt(int nIndex)
{
	int nValue=0;

	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};

	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) );

	nValue = atoi(szValue);

	return nValue;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
double CParamList::m_fnGetItemDouble(int nIndex)
{
	double dValue=0.0;
	
	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};

	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) );

	dValue = atof(szValue);

	return dValue;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
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

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 0 = OK, -1 = Out of Pos
==================================================================================================*/
int	CParamList::m_fnGetItemString(int nIndex, char* szValue, int nSize)
{
	if( nIndex >= m_fnGetCount() ) return -1;
	char szTemp[MAX_BUFF_PARAMLIST] = {0};
	
	POSITION pos = m_List.FindIndex(nIndex);
	sprintf_s(szTemp, sizeof(szTemp), "%s", m_List.GetAt(pos));

	strcpy_s(szValue, nSize, szTemp);
	
	return 0;
}

std::pair<std::string, std::string>	CParamList::m_fnGetPasedItemString(char* szValue)
{
	std::string sTempString(szValue);
	std::string sTempTag, sTempVal;
	int nSplitIdx;

	nSplitIdx = sTempString.find(':');

	//sTempTag = sTempString.substr( 0, nSplitIdx - 1 );
	//sTempVal = sTempString.substr(nSplitIdx + 1, sizeof(sTempString) - nSplitIdx);
	sTempTag = sTempString.substr(0, nSplitIdx);
	sTempVal = sTempString.substr(nSplitIdx + 1, sizeof(sTempString) - nSplitIdx);

	sTempVal = sTempVal.substr(sTempVal.find('\"') + 1);
	//sTempVal.pop_back();

	std::pair<std::string, std::string> p;

	p.first = sTempTag;
	p.second = sTempVal;

	return p;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
int	CParamList::m_fnGetItemInt(int nIndex, int* pnValue)
{
	if( nIndex >= m_fnGetCount() ) return -1;
	
	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};
	
	sprintf_s(szValue, sizeof(szValue), "%s", m_List.GetAt(pos) );
	
	*pnValue = atoi(szValue);
	
	return 0;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
int	CParamList::m_fnGetItemDouble(int nIndex, double* pdValue)
{
	if( nIndex >= m_fnGetCount() ) return -1;
	
	POSITION pos = m_List.FindIndex(nIndex);
	
	char szValue[32]={0};
	
	sprintf_s(szValue, sizeof(szValue),"%s", m_List.GetAt(pos) );
	
	*pdValue = atof(szValue);
	
	return 0;
}


/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
void CParamList::m_fnAddItemString(char* szValue)
{
	m_List.AddTail(szValue);	
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
BOOL    CParamList::m_fnDoesItSplit()
{
	return (m_nSplitCount > 0 ) ? TRUE : FALSE;
}

/*==================================================================================================
  DESCRIPT :
  ARGUMENT : 
  RETURN   : 
==================================================================================================*/
void    CParamList::m_fnCopySplit(char* szBuff, int* pnSplitCount)
{
	memcpy(szBuff, m_pcSplit, m_nSplitCount);
	*pnSplitCount = m_nSplitCount;
}
