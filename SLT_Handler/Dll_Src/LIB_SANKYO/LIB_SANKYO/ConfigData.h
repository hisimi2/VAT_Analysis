// ConfigData.h: interface for the CConfigData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGDATA_H__EDCF4252_1D6D_4449_8A26_F2AC0B03CC24__INCLUDED_)
#define AFX_CONFIGDATA_H__EDCF4252_1D6D_4449_8A26_F2AC0B03CC24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class  CConfigData  
{
public:
	CConfigData();
	CConfigData(char* szFileName);
	CConfigData(char* szFileName,char* szSection, char* szKey, char* szDefault);
	virtual ~CConfigData();

	void   Initialize(char* szFileName, char* szSection, char* szKey, char* szDefault);
	void   Initialize(char* szSection, char* szKey, char* szDefault);
	void   SetFileName(char* szFileName);

	int    GetInt	();
	int    GetInt	(char* szSection, char* szKey, char* szDefault);
	double GetDouble();
	double GetDouble(char* szSection, char* szKey, char* szDefault);
	void   GetString(char* pData, int nSize);
	void   GetString(char* szSection, char* szKey, char* szDefault, char* pData, int nSize);

	void   Set(int nData);
	void   Set(char* szSection, char* szKey, int nData);
	void   Set(double dData);
	void   Set(char* szSection, char* szKey, double dData);
	void   Set(char* szData);
	void   Set(char* szSection, char* szKey, char* szData);

protected:
	char	m_szFileName	[MAX_PATH];
	char	m_szSection		[128];
	char	m_szKey			[128];
	char	m_szDefault		[128];
};

#endif // !defined(AFX_CONFIGDATA_H__EDCF4252_1D6D_4449_8A26_F2AC0B03CC24__INCLUDED_)
