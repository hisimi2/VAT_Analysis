#pragma once

#include "DEF_LIB_TESTER_IF.h"
#include "ParamList.h" // LIB_UTIL

class AFX_EXT_CLASS CTESTER_IF_Cont : public CWnd
{
public:
	CTESTER_IF_Cont(void);
	~CTESTER_IF_Cont(void);

	//========================================================
	// Initialize and Close
	int     Initialize( CWnd* pClientWnd, ST_COMM_DATA stCommData, ST_TESTER_MESSAGE_MAP* pMessageMap );
	void    Finalize();
	//========================================================

	//=======================================================
	// App->Send to Tester command
	int				SendCMD( CTesterIFCmd* pPacket );
	int				SendCMD_Fast( CTesterIFCmd* pPacket );
	int				SRQLineCheck();

	CTesterIFCmd*	MakeCmd( CString strCmd, CParamList& cParamList );

	bool			IsConnected();

private:
	ST_COMM_DATA m_stCommData;
	CWnd*		 m_pClientWnd;
	CWnd*        m_pTesterIF_Imp;
	char		 m_szInitFileName[MAX_PATH];

};

/*
////////////////////////////////////// dll ////////////////////////////////////////////
// TIFWnd.h
class CTIFWnd : public CWnd
{
	virtual afx_msg LRESULT OnRcvGpib_BINON(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpib_ECHO(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvRS232_CF(WPARAM wParam, LPARAM lParam)=0;
};


////////////////////////////////////// app ////////////////////////////////////////////

CTESTER_IF_Cont g_TesterIF;
{
	g_TesterIF.Create(pTIFClientWnd, stCommData, szInitFile, strlen(szInitFile));

	CParamList ParamList;
	ParamList.m_fnAdd(0x41);
	CTesterIFCmd* pPacket = g_TesterIF.MakeCmd(TIF_CMD_GPIB_SRQ , cParamList);
	
	CParamList ParamList;
	ParamList.m_fnAdd(nSiteCount);
	CTesterIFCmd* pPacket = g_TesterIF.MakeCmd(TIF_CMD_RS232_CF , cParamList)
	g_TesterIF.SendCMD(pPacket);

/////////////////////////////////////////////////
	CParamList ParamList;
	ParamList.m_fnAdd(0x41);
	CTesterIFCmd* pPacket = g_TesterIF.MakeCmd(TIF_CMD_GPIB_SRQ , cParamList);
	g_TesterIF.SendCMD(pPacket);

	CParamList ParamList;
	ParamList.m_fnAdd(nSite1);
	ParamList.m_fnAdd(nSite3);
	ParamList.m_fnAdd(nSite4);
	CTesterIFCmd* pPacket = g_TesterIF.MakeCmd(TIF_CMD_DIO_SOT , cParamList)
	g_TesterIF.SendCMD(pPacket);

	g_TesterIF.Destroy();
}


// TIFClientWnd.h
class CTIFClientWnd : public CTIFWnd
{
public:
	LRESULT OnRcvGpib_BINON(WPARAM wParam, LPARAM lParam)
	{
		CParamList ParamList;
		ParamList.m_fnAdd(strRcvBinon);
		CTesterIFCmd* pPacket = g_TesterIF.MakeCmd(TIF_CMD_GPIB_ECHO , cParamList);
		g_TesterIF.SendCMD(pPacket);
	};

	LRESULT OnRcvGpib_ECHO(WPARAM wParam, LPARAM lParam)
	{
	};

	LRESULT OnRcvRS232_CF(WPARAM wParam, LPARAM lParam)
	{
	};
};



// TIFClientWnd_GPIB.cpp
LRESULT CTIFClientWnd::OnRcvGpib_ECHO(WPARAM wParam, LPARAM lParam)
{
};

// TIFClientWnd_RS232.cpp
LRESULT CTIFClientWnd::OnRcvRS232_CF(WPARAM wParam, LPARAM lParam)
{
};

// TIFClientWnd_DIO.cpp
*/
