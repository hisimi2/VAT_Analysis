#include "StdAfx.h"
#include "IFComm_GPIB.h"

#include "Decl-32.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//CIFComm_GPIB g_IFComm_GPIB;

CIFComm_GPIB::CIFComm_GPIB(void)
{
	m_nKindGPIB  = -1;
	m_nGpibID    = 0;
}

CIFComm_GPIB::~CIFComm_GPIB(void)
{

}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : InitBoard_GPIB
  PARAMS   : None
  RETURN   : -1 : ERROR
              1 : OK
  COMMENTS : GPIB Board Initialize
==============================================================================*/
int CIFComm_GPIB::Init_Board()
{
	HINSTANCE hGpib32Lib = LoadLibrary("GPIBI32.DLL");	// Load DLL
	
	if ( hGpib32Lib == NULL ){ return ERROR_DLL_INIT_FAILED; }				// DLL Load Fail
	else
	{
		TRACE("Found GPIB board NI.");
		m_Logger.LOG("Found GPIB board NI Lib");
		m_nKindGPIB = eNIGPIB;
	}

	SendIFC(0);								// perform interface clear SendList 

	m_nGpibID = ibfind("gpib0");			// Load GPIB ID

	if(m_nGpibID == -1){					// Problem Communication ERROR!
		m_nKindGPIB  = -1;
		FreeLibrary(hGpib32Lib);
		AfxMessageBox("GPIB Interface Board Initialization Error!!",MB_TOPMOST);		
		m_Logger.LOG("GPIB Interface Board Initialization Error!!");
		return ERROR_FAILED_DRIVER_ENTRY;
	}
	ibonl(m_nGpibID,0);						// Change Board Status [ 1:Online, 2:Offline ]
	ibrsc(m_nGpibID,0);						// Setting Client
	if(m_stCommData.nGPIB_Adress !=31 ){	//Set of address
		ibpad(m_nGpibID, m_stCommData.nGPIB_Adress);
	}

	m_bConnected = TRUE;

	if(hGpib32Lib != NULL)
	{
		FreeLibrary(hGpib32Lib);
		hGpib32Lib = NULL;
	}

	m_Logger.LOG("GPIB Board initialize Success!!");

	return ERROR_SUCCESS;
}

void CIFComm_GPIB::Final_Board()
{
	m_bConnected = false;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : m_fnSendCMD
  PARAMS   : Packet (Class) : Send Data 및 Receive Command 정보 저장 Class
  RETURN   : 1: OK
  COMMENTS : Command Send Handler to Tester
==============================================================================*/
int CIFComm_GPIB::do_SendCMD(CTesterIFCmd& Packet)
{
	if(m_nGpibID == -1){					// Problem Communication ERROR!
		m_Logger.LOG("do_SendCMD is m_nGPibID = -1");
		return ERROR_FAILED_DRIVER_ENTRY;
	}

	CString data	= _T("");
	char szSendData[2048]	= {0,};
	char szEnd[5] = "\r\n";
	DWORD m_dwTickCount = ::GetTickCount();

	m_Logger.LOG("Send to Tester // Cmd = [%s]  ||  Data = [%s]", Packet.strCmd,Packet.strSndData);

	if(Packet.strCmd == TESTER_CMD_HTOT_SRQ41)
	{		// SQR41 일 경우
		int nSRQ = 0x41;
		ibrsv(m_nGpibID, nSRQ);
	}
	else if(Packet.strCmd == TESTER_CMD_HTOT_SRQ44)
	{	// SQR44 일 경우
		int nSRQ = 0x44;
		ibrsv(m_nGpibID, nSRQ);
	}
	else
	{										// Other Command
		strcpy_s(szSendData,sizeof(szSendData),(LPSTR)(LPCTSTR)Packet.strSndData);
		strcat_s(szSendData,sizeof(szSendData), szEnd);
		while(1)
		{
			ibsta = ibwait(m_nGpibID, 0);
			if(!(ibsta & ATN) && (ibsta & END) && (ibsta & TACS)){ // ibsta is ready
				ibwrt(m_nGpibID, szSendData, strlen(szSendData));
				break;
			}

			DWORD dwSpan = ::GetTickCount() - m_dwTickCount;
			if(dwSpan > 200){	// Limit 200um
				return -1;
			}
			Sleep(1);
		}
	}
	return ERROR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SRQLineCheck
  PARAMS   : None
  RETURN   : 1 : OK
            -1 : problem
  COMMENTS : monitor bus lines
==============================================================================*/
int CIFComm_GPIB::SRQLineCheck()
{
	SHORT pLines = 0;

	iblines(m_nGpibID, &pLines);
	if(!(pLines & BusSRQ)){
		return ERROR_INVALID_DATA;
	}
	
	return ERROR_SUCCESS;
}

void CIFComm_GPIB::GetBoardRcvCMD()
{
	CString strReciveData = _T("");
	char szBuff[2048] = {0,};

	ibsta = ibwait(m_nGpibID, 0);
	if(!(ibsta & ATN) && (ibsta & LACS) && !(ibsta & ERR))	// possible to read cmd and not error
	{
		ibsta = ibrd(m_nGpibID, szBuff, 2048L);
		if (ibsta & ERR){
			ibonl (m_nGpibID, 0);
		}
		strReciveData = szBuff;

		if(!strReciveData.IsEmpty())
		{
			CTesterIFCmd* pPacket = new CTesterIFCmd;
			if( pPacket != NULL )
				pPacket->TransReciveData( strReciveData );

			AddRcvCMD(pPacket);

			if( pPacket != NULL ){
				delete pPacket; 
				pPacket = NULL;
			}
		}	
	}	
}