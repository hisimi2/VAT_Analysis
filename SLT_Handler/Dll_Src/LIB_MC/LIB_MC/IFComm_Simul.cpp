#include "StdAfx.h"
#include "IFComm_Simul.h"

CIFComm_Simul::CIFComm_Simul(void)
{
}


CIFComm_Simul::~CIFComm_Simul(void)
{
}

int	CIFComm_Simul::Init_Board()
{
	m_bConnected = true;

	return ERROR_SUCCESS;
}

void CIFComm_Simul::Final_Board()
{
	m_bConnected = false;
}

void CIFComm_Simul::GetBoardRcvCMD()
{

}

int	CIFComm_Simul::do_SendCMD(CTesterIFCmd& Packet)
{
	// Tester에게 보내는 명령을 처리한다.

	CString data	= _T("");
	char szSendData[2048]	= {0,};
	char szEnd[5] = "\r\n";
	DWORD m_dwTickCount = ::GetTickCount();

	m_Logger.LOG("Send to Tester // Cmd = [%s]  ||  Data = [%s]", Packet.strCmd, Packet.strSndData );


	if(Packet.strCmd == TESTER_CMD_HTOT_SRQ41)
	{	// SQR41 일 경우
		// 어플리케이션에게 "Fullsite?" 명령 보내기.
		Packet.strCmd = TESTER_CMD_TTOH_FULLSITE_UPPER;
		this->SendToClient( &Packet );
	}
	else if(Packet.strCmd == TESTER_CMD_HTOT_SRQ44)
	{	// SQR44 일 경우
		
	}
	else if( Packet.strCmd == TESTER_CMD_HTOT_FULLSITE )
	{
		Packet.strCmd = TESTER_CMD_TTOH_BINON;
		Packet.strRcvData = this->MakeBINON( Packet.strSndData );
		this->SendToClient( &Packet );
	}
	else if( Packet.strCmd == TESTER_CMD_HTOT_ECHO )
	{
		m_strBINON.Delete( 0, 6 );
		Packet.strSndData.Delete( 0, 5 );

		if( Packet.strSndData == m_strBINON )
		{
			Packet.strCmd = TESTER_CMD_TTOH_ECHO_OK;
		}
		else
		{
			Packet.strCmd = TESTER_CMD_TTOH_ECHO_NG;
		}
		
		this->SendToClient( &Packet );
	}
	else
	{	// Other Command
		strcpy_s(szSendData,sizeof(szSendData),(LPSTR)(LPCTSTR)Packet.strSndData);
		strcat_s(szSendData,sizeof(szSendData), szEnd);

		while(1)
		{
			//ibsta = ibwait(m_nGpibID, 0);
			//if(!(ibsta & ATN) && (ibsta & END) && (ibsta & TACS)){ // ibsta is ready
				//ibwrt(m_nGpibID, szSendData, strlen(szSendData));
				//break;
			//}

			DWORD dwSpan = ::GetTickCount() - m_dwTickCount;

			if(dwSpan > 200) // Limit 200um
			{	
				return ERROR_TIMEOUT;
			}

			Sleep(1);
		}
	}

	return ERROR_SUCCESS;
}

int	CIFComm_Simul::SRQLineCheck()
{
	return 0;
}

CString CIFComm_Simul::MakeBINON( CString strFullsites )
{
	m_strBINON = _T( "" );
	
	strFullsites.Delete( 0, 10 ); // 앞에 "Fullsites " 부분 자르기.
	strFullsites.Insert( 0, _T( "0x" ) ); // 16진수 처럼 모양 만들기.

	std::stringstream ss;
	ss << std::hex << strFullsites;
	unsigned int n;
	ss >> n;
	std::bitset< 256 > b( ( int )n );

	m_strBINON = b.to_string().c_str();

	m_strBINON.Delete( 0, 256 - m_nTIF_StdPara );

	int nLenght = m_strBINON.GetLength();
	for( int i = nLenght - 8; i > 7; i = i - 8 )
	{
		m_strBINON.Insert( i, _T( "," ) );
	}

	m_strBINON.Insert( 0, _T( "BINON:" ) );
	m_strBINON += _T( ";" );

	return m_strBINON;	
}
