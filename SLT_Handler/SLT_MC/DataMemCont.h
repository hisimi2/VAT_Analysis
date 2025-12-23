#pragma once


#include <vector>
#include <string>

typedef std::pair<std::string, std::string> NameValuePair;

//==============================================================================
//
//==============================================================================
class CDataMem;
class CDeviceDataMem;
class CRamMotorParam;
class CRamTeaching;

//==============================================================================
//
//==============================================================================
class CDataMemCont
{
public:
	CDataMemCont(void);
	~CDataMemCont(void);

	CDataMem	   m_dmEQP;
	CDataMem	   m_dmContac_Dn;
	CDataMem	   m_dmContac_Up;
	CDataMem	   m_dmShuttle;
	CDataMem	   m_dmTestPP;
	CDataMem	   m_dmTrayPP;
	CDataMem	   m_dmConveyor;
	CDataMem	   m_dmStacker;
	CDataMem	   m_dmTransfer;
	CDataMem	   m_dmFeeder;
	CDataMem	   m_dmHandlerOpt;
	CDataMem	   m_dmCOK;
	CDataMem	   m_dm2DID;
	CDataMem	   m_dmVAT;
	CDeviceDataMem m_dmDevice;
	CRamMotorParam m_MotorParam;
	CRamTeaching   m_Teach;

	void LoadIniFile( const std::string& iniFilePath, const std::string& sectionName, 
		              std::vector<NameValuePair>& nameValuePairs );


	int Create(char* szIniFileName);
	void Destroy();

	void GetEvtMessage(int nEvtId, CString& strMsg);

private:
	std::vector<NameValuePair>  m_EvtMsgPairs_Eng;
	CCriticalSection m_csEvtMsgPairs_Eng;
};

/*============================================================================= Guide Code
	int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eSETPLATE_1, x, y);
	if( nExist == eDeviceData_None ){

	}else if( nExist == eDeviceData_Exist ){
	
	}

	g_DMCont.m_dmDevice;
=============================================================================*/

