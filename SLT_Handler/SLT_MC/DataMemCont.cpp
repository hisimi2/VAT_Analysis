#include "StdAfx.h"
#include "DataMemCont.h"
#include "ConfigData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDataMemCont::CDataMemCont(void)
{
}


CDataMemCont::~CDataMemCont(void)
{
}


int CDataMemCont::Create(char* szIniFileName)
{
	int nRet = 0;
	m_dmEQP.Create(SZ_DM_NAME_0);
	m_dmContac_Dn.Create(SZ_DM_NAME_1);
	m_dmContac_Up.Create(SZ_DM_NAME_2);
	m_dmShuttle.Create(SZ_DM_NAME_3);
	m_dmTestPP.Create(SZ_DM_NAME_4);
	m_dmTrayPP.Create(SZ_DM_NAME_5);
	m_dmConveyor.Create(SZ_DM_NAME_6);
	m_dmStacker.Create(SZ_DM_NAME_7);
	m_dmTransfer.Create(SZ_DM_NAME_9);
	m_dmFeeder.Create(SZ_DM_NAME_10);
	m_dmHandlerOpt.Create(SZ_DM_NAME_11);
	m_dmCOK.Create(SZ_DM_NAME_12);
	m_dmDevice.CreateMapData(SZ_DM_DEVICE_DATA);
	m_dm2DID.Create(SZ_DM_NAME_13);
	m_dmVAT.Create(SZ_DM_NAME_14);

	m_MotorParam.Load();

	char szCokName[128]={0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	m_Teach.Initialize(szIniFileName, szCokName);
	m_Teach.Load();

	char szFileName[MAX_PATH]={0,};
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szFileName, sizeof(szFileName), "%s\\sys_data\\", szTemp);

	char szDefault[512]="";
	char szValue[512]="";

	CConfigData ConfigData(szIniFileName);

	ZeroMemory(szDefault, sizeof(szDefault));
	ZeroMemory(szValue, sizeof(szValue));
	sprintf_s(szDefault, sizeof(szDefault), "%sEvtMsg_Eng.msg", szFileName);
	ConfigData.GetString("COLLECTION EVENT MESSAGE FILES", "English", szDefault, szValue);

	LoadIniFile(szValue, "COLLECTION EVENT MESSAGE", m_EvtMsgPairs_Eng);

	return nRet;
}

void CDataMemCont::Destroy()
{
	g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, TRUE);

	m_dmEQP.Destroy();
	m_dmContac_Dn.Destroy();
	m_dmContac_Up.Destroy();
	m_dmShuttle.Destroy();
	m_dmTestPP.Destroy();
	m_dmTrayPP.Destroy();
	m_dmConveyor.Destroy();
	m_dmStacker.Destroy();
	m_dmTransfer.Destroy();
	m_dmFeeder.Destroy();
	m_dmHandlerOpt.Destroy();
	m_dmCOK.Destroy();
	m_dmVAT.Destroy();
	m_dmDevice.DestroyMapData();
	m_Teach.Finalize();
	m_dm2DID.Destroy();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : INI파일에 있는 모든 데이터를 한꺼번에 읽어 오는 함수 임.
==============================================================================*/
void CDataMemCont::LoadIniFile( const std::string& iniFilePath, 
	const std::string& sectionName, 
	std::vector<NameValuePair>& nameValuePairs ) 
{
	const int bufferSize = 10000;
	char buffer[bufferSize] = "";

	int charsRead = 0;

	charsRead = GetPrivateProfileSection( sectionName.c_str(),
		buffer,
		bufferSize,
		iniFilePath.c_str() );
	// if there isn't enough space, returns bufferSize - 2

	// if we got some data...
	if( (0 < charsRead) && ((bufferSize - 2) > charsRead) ) {
		// walk the buffer extracting values

		// start at the beginning (const to remind us not to
		// change the contents of the buffer)
		const char* pSubstr = buffer;

		// while we have non-empty substrings...
		while('\0' != *pSubstr) {
			// length of key-value pair substring
			size_t substrLen = strlen(pSubstr);

			// split substring on '=' char
			const char* pos = strchr(pSubstr, '=');
			if(NULL != pos) {
				// todo: remove "magic number" for buffer size 
				char name [256] = "";
				char value[256] = "";

				// if you're not using VC++ you'll prob. need to replace
				// _countof(name) with sizeof(name)/sizeof(char) and
				// similarly for value. Plus replace strncpy_s with plain
				// old strncpy.
				strncpy_s(name , _countof(name), pSubstr, pos - pSubstr);
				strncpy_s(value, _countof(value), pos + 1, substrLen - (pos - pSubstr));

				nameValuePairs.push_back(NameValuePair(name, value));
			}

			// jump over the current substring plus its null
			pSubstr += (substrLen + 1);
		}
	}
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-10-08]
  NAME     : 
  PARAMS   : nEvtId : event id 입력 받는다.
             strMsg : event message를 반환한다.
  RETURN   : 
  COMMENTS : event message를 프로그램 기동 시에 파일로 부터 메모리로 로드 한다.
             메모리에 메세지가 없다면 파일에 default message를 기록해 둔다.
==============================================================================*/
void CDataMemCont::GetEvtMessage(int nEvtId, CString& strMsg)
{
	m_csEvtMsgPairs_Eng.Lock();

	BOOL bExist = FALSE;
	std::vector<NameValuePair>::size_type count = m_EvtMsgPairs_Eng.size();
	for(std::vector<NameValuePair>::size_type i=0; i<count; i++){
		if( atoi(m_EvtMsgPairs_Eng[i].first.c_str()) == nEvtId ){
			strMsg = m_EvtMsgPairs_Eng[i].second.c_str();
			bExist = TRUE;
			break;		
		}
	}

	if( !bExist ){
		char szFileName[MAX_PATH]={0,};
		char szTemp[512]={0,};
		::GetCurrentDirectory(sizeof(szTemp), szTemp);
		sprintf_s(szFileName, sizeof(szFileName), "%s\\sys_data\\EvtMsg_Eng.msg", szTemp);

		char szKey[512]="";
		char szValue[512]="";
		CConfigData ConfigData(szFileName);

		sprintf_s(szKey, sizeof(szKey), "%d", nEvtId);
		sprintf_s(szValue, sizeof(szValue), "please write event message %d", nEvtId);
		ConfigData.GetString("COLLECTION EVENT MESSAGE", szKey, szValue, szValue);
		strMsg = szValue;
	}

	m_csEvtMsgPairs_Eng.Unlock();
}
