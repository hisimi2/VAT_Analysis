// ConfigData.cpp: implementation of the CConfigData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigData.h"

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
CConfigData::CConfigData()
{
	memset(m_szFileName,	0x00, sizeof(m_szFileName)	);
	memset(m_szSection,		0x00, sizeof(m_szSection)	);
	memset(m_szKey,			0x00, sizeof(m_szKey)		);
	memset(m_szDefault,		0x00, sizeof(m_szDefault)	);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CConfigData::CConfigData(char* szFileName)
{
	memset(m_szFileName,	0x00, sizeof(m_szFileName)	);
	memset(m_szSection,		0x00, sizeof(m_szSection)	);
	memset(m_szKey,			0x00, sizeof(m_szKey)		);
	memset(m_szDefault,		0x00, sizeof(m_szDefault)	);
	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CConfigData::CConfigData(char* szFileName,char* szSection, char* szKey, char* szDefault)
{
	memset(m_szFileName,	0x00, sizeof(m_szFileName)	);
	memset(m_szSection,		0x00, sizeof(m_szSection)	);
	memset(m_szKey,			0x00, sizeof(m_szKey)		);
	memset(m_szDefault,		0x00, sizeof(m_szDefault)	);

	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
	sprintf_s(m_szSection, sizeof(m_szSection), "%s", szSection);	
	sprintf_s(m_szKey,sizeof(m_szKey), "%s", szKey); 				
	sprintf_s(m_szDefault, sizeof(m_szDefault), "%s", szDefault);   
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CConfigData::~CConfigData()
{

}

/*==============================================================================
  DESCRIPT : 전달 받은 파일이름, Section, Key값, 기본 설정값을 Class의 멤버 변수에 대입
  KEY WORD : Unknown
  ARGUMENT : 파일이름, []section명, 세부 항목, 기본 값
  RETURN   : None
==============================================================================*/
void CConfigData::Initialize(char* szFileName, char* szSection, char* szKey, char* szDefault)
{
	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName); 
	sprintf_s(m_szSection, sizeof(m_szSection), "%s", szSection); 	 
	sprintf_s(m_szKey, sizeof(m_szKey), "%s", szKey); 				 
	sprintf_s(m_szDefault, sizeof(m_szDefault), "%s", szDefault); 	 
}

/*==============================================================================
  DESCRIPT : Section, Key값, 기본 설정값을 Class의 멤버 변수에 대입
  KEY WORD : Unknown
  ARGUMENT : []section명, 세부 항목, 기본 값
  RETURN   : None
==============================================================================*/
void CConfigData::Initialize(char* szSection, char* szKey, char* szDefault)
{
	sprintf_s(m_szSection, sizeof(m_szSection), "%s", szSection);
	sprintf_s(m_szKey, sizeof(m_szKey), "%s", szKey); 			 
	sprintf_s(m_szDefault, sizeof(m_szDefault), "%s", szDefault);
}

/*==============================================================================
  DESCRIPT : 전달 받은 파일이름을 Class의 멤버 변수에 대입
  KEY WORD : Unknown
  ARGUMENT : 파일이름, []section명, 세부 항목, 기본 값
  RETURN   : None
==============================================================================*/
void CConfigData::SetFileName(char* szFileName)
{
	sprintf_s(m_szFileName, sizeof(m_szFileName), "%s", szFileName);
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름, section 명, Key값, 기본설정) 값을 이용하여 해당 문자를 int로 변환
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : 
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 int형으로 변환하여 반환
==============================================================================*/
int CConfigData::GetInt()
{
	int nRet=0;
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	nRet = atoi(szBuff);
	return nRet;
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름)에 전달 된 section 명, Key값, 기본설정 값을 이용하여 해당 문자를 int로 변환
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : Section 명, Key 값, 기본설정 값
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 int형으로 변환하여 반환
==============================================================================*/

int CConfigData::GetInt(char* szSection, char* szKey, char* szDefault)
{
	int nRet=0;
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff,sizeof(szBuff), "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	nRet = atoi(szBuff);
	return nRet;
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름, section 명, Key값, 기본설정) 값을 이용하여 해당 문자를 long로 변환
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : 
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 long형으로 변환하여 반환
==============================================================================*/
long CConfigData::GetLong()
{
	long nRet=0;
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	nRet = atol(szBuff);
	return nRet;
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름)에 전달 된 section 명, Key값, 기본설정 값을 이용하여 해당 문자를 long로 변환
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : Section 명, Key 값, 기본설정 값
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 long형으로 변환하여 반환
==============================================================================*/
long CConfigData::GetLong(char* szSection, char* szKey, char* szDefault)\
{
	long nRet=0;
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff,sizeof(szBuff), "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	nRet = atol(szBuff);
	return nRet;
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름, section 명, Key값, 기본설정) 값을 이용하여 해당 문자를 double로 변환
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : None
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 double형으로 변환하여 반환
==============================================================================*/
double CConfigData::GetDouble()
{
	double dRet=0.0;
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	dRet = atof(szBuff);
	return dRet;
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름)에 전달된 section 명, Key값, 기본설정 값을 이용하여 해당 문자를 double로 변환
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : Section명, Key 값, 기본설정값
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 double형으로 변환하여 반환
==============================================================================*/
double CConfigData::GetDouble(char* szSection, char* szKey, char* szDefault)
{
	double dRet=0.0;
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf_s(szBuff, sizeof(szBuff), "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	dRet = atof(szBuff);
	return dRet;
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름, section 명, Key값, 기본설정) 값을 이용하여 해당 문자를 전달된 배열에 문자를 저장
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : char Array의 주소
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 전달받은 char Array에 복사
==============================================================================*/
void CConfigData::GetString(char* pData)
{
	char szBuff[512]={0};
	GetPrivateProfileString(m_szSection, m_szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf(szBuff, "%s", m_szDefault);
		WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
	}
	sprintf(pData, "%s", szBuff);
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수인 파일이름과 전달받은 section 명, Key값, 기본설정 값을 이용하여 해당 문자를 전달된 배열에 문자를 저장
             해당 내용을 찾지 못 할 경우 Default값으로 설정 및 저장
  KEY WORD : Unknown
  ARGUMENT : []Section, Key값, 기본설정값, char Array의 주소
  RETURN   : 해당 파일의 []section으로 분류내용 중 key값에 해당하는 문자를 전달받은 char Array에 복사
==============================================================================*/
void CConfigData::GetString(char* szSection, char* szKey, char* szDefault, char* pData)
{
	char szBuff[512]={0};
	GetPrivateProfileString(szSection, szKey, "", szBuff, sizeof(szBuff)-1, m_szFileName);
	if( strlen(szBuff) == 0 ){
		sprintf(szBuff, "%s", szDefault);
		WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
	}
	sprintf(pData, "%s", szBuff);
}

/*==============================================================================
  DESCRIPT : 전달 받은 int 형 데이터를 문자로 변환하여 기입력된 멤버변수(파일이름, section명, Key값)에
             해당위치에 데이터를 넣는다
  KEY WORD : Unknown
  ARGUMENT : int형 데이터
  RETURN   : None
==============================================================================*/
void CConfigData::Set(int nData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%d", nData);
	WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
}

/*==============================================================================
  DESCRIPT : 전달받은 Section 명의 Key값에 해당하는 위치에 전달받은 int형 데이터를 넣는다
  KEY WORD : Unknown
  ARGUMENT : section명, Key값, int형 데이터
  RETURN   : None
==============================================================================*/
void CConfigData::Set(char* szSection, char* szKey, int nData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%d", nData);
	WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
}

/*==============================================================================
  DESCRIPT : 전달 받은 double 형 데이터를 문자로 변환하여 기입력된 멤버변수(파일이름, section명, Key값)에
             해당위치에 데이터를 넣는다
  KEY WORD : Unknown
  ARGUMENT : double형 데이터
  RETURN   : None
==============================================================================*/
void CConfigData::Set(double dData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%f", dData);
	WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
}

/*==============================================================================
  DESCRIPT : 전달받은 Section 명의 Key값에 해당하는 위치에 전달받은 double형 데이터를 넣는다
  KEY WORD : Unknown
  ARGUMENT : section명, Key값, double형 데이터
  RETURN   : None
==============================================================================*/
void CConfigData::Set(char* szSection, char* szKey, double dData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%f", dData);
	WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
}

/*==============================================================================
  DESCRIPT : 기입력된 멤버변수(파일이름, section 명, Key값) 값을 이용하여 전달된 문자열을 
             넣는다
  KEY WORD : Unknown
  ARGUMENT : char Array의 주소
  RETURN   : None
==============================================================================*/
void CConfigData::Set(char* szData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%s", szData);
	WritePrivateProfileString(m_szSection, m_szKey, szBuff, m_szFileName);
}

/*==============================================================================
  DESCRIPT : 전달받은 section의 Key값에 전달받은 문자열을 넣는다
  KEY WORD : Unknown
  ARGUMENT : Section명, Key값, char Array의 주소
  RETURN   : None
==============================================================================*/
void CConfigData::Set(char* szSection, char* szKey, char* szData)
{
	char szBuff[512]={0};
	sprintf_s(szBuff, sizeof(szBuff), "%s", szData);
	WritePrivateProfileString(szSection, szKey, szBuff, m_szFileName);
}

