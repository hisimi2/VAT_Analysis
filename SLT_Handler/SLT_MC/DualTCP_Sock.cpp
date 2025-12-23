#include "StdAfx.h"
#include "DualTCP_Sock.h"
#include "LBaseSock.h"
#include "Token.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDualTCP_Sock::CDualTCP_Sock(void)
{
	m_pHeadPort1 = 0 , m_pTailPort1 = 0;
	m_bConnection_P1				= FALSE;
	m_bConnectionExit_P1	= FALSE;
	m_bConnectionExitSts_P1	= FALSE;
	m_nConnectionCnt_P1 = 0;
	m_nMakePort_P1 = FALSE;
	
	m_pHeadPort2 = 0 , m_pTailPort2 = 0;
	m_bConnection_P2				= FALSE;
	m_bConnectionExit_P2		= FALSE;
	m_bConnectionExitSts_P2	= FALSE;
	m_nConnectionCnt_P2 = 0;
	m_nMakePort_P2 = FALSE;

	NET_Reject1 = FALSE;
	NET_Reject2 = FALSE;

	ZeroMemory(NET_HOSTADDR0, sizeof(NET_HOSTADDR0));
	ZeroMemory(m_uszBuffPort1, sizeof(m_uszBuffPort1));
	ZeroMemory(m_uszBuffPort2, sizeof(m_uszBuffPort2));

	m_strBuffPort1.Empty();
	m_strDataPort1.Empty();

	m_pThreadP1 = NULL;
	m_pThreadP2 = NULL;

	InitializeCriticalSection( &Net_Cs1 );
	InitializeCriticalSection( &Net_Cs2 );

	m_nPort1No = 0;
	NET_SOCK1 = 0;
	NET_SOCK_LISTEN1 = 0;
	NET_FROMLEN1 = 0;
	NET_Reject_Char1 = 0;

	m_nPort2No = 0;
	NET_SOCK2 = 0;
	NET_SOCK_LISTEN2 = 0;
	NET_FROMLEN2 = 0;
	NET_Reject_Char2 = 0;
}


CDualTCP_Sock::~CDualTCP_Sock(void)
{
	DeleteCriticalSection( &Net_Cs2 );
	DeleteCriticalSection( &Net_Cs2 );
}



BOOL CDualTCP_Sock::Initialize_DoublePort( char *HostAddr , int PortNo1 , int PortNo2 , BOOL Client ) {
	return Net_Connect_Double_Port2( HostAddr , PortNo1 , PortNo2 , Client , Client );
}

BOOL CDualTCP_Sock::Net_Winsock_Init() {
	WSADATA WSAData;
	if ( WSAStartup( 0x202 , &WSAData ) == SOCKET_ERROR ){
		return FALSE;
	}
	return TRUE;
}

BOOL CDualTCP_Sock::Initialize_Port( char *HostAddr , int PortNo1, BOOL Client ) {
	return Net_Connect_Port( HostAddr, PortNo1, Client);
}
BOOL CDualTCP_Sock::Net_Connect_Port( char *HostAddr , int PortNo, BOOL Client) {
	if ( !Net_Winsock_Init() ) {
		WSACleanup();
		return FALSE;
	}
	strncpy( NET_HOSTADDR0 , HostAddr , 64 );
	m_nPort1No = PortNo;
	if ( m_nPort1No < 1000 || m_nPort1No > 65535 ) {
		printf( "Net Port 1 Range(1000-65535) Error [%d]\n" , m_nPort1No );
		return FALSE;
	}

	if ( Client )	{
		m_pThreadP1 = AfxBeginThread(ClientConnectPort1, this);
	} else	{
		m_pThreadP1 = AfxBeginThread(ServerConnectPort1, this);
	}

	return TRUE;
}

BOOL CDualTCP_Sock::Net_Connect_Double_Port2( char *HostAddr , int PortNo1 , int PortNo2 , BOOL Client1 , BOOL Client2 ) {
	if ( !Net_Winsock_Init() ) {
		WSACleanup();
		return FALSE;
	}
	strncpy( NET_HOSTADDR0 , HostAddr , 64 );
	m_nPort1No = PortNo1;
	m_nPort2No = PortNo2;
	if ( m_nPort1No < 1000 || m_nPort1No > 65535 ) {
		printf( "Net Port 1 Range(1000-65535) Error [%d]\n" , m_nPort1No );
		return FALSE;
	}
	if ( m_nPort2No < 1000 || m_nPort2No > 65535 ) {
		printf( "Net Port 2 Range(1000-65535) Error [%d]\n" , m_nPort2No );
		return FALSE;
	}

	if ( Client1 )	{
		m_pThreadP1 = AfxBeginThread(ClientConnectPort1, this);
	} else	{
		m_pThreadP1 = AfxBeginThread(ServerConnectPort1, this);
	}

	if ( Client2 )	{
		m_pThreadP2 = AfxBeginThread(ClientConnectPort2, this);
	} else {
		m_pThreadP2 = AfxBeginThread(ServerConnectPort2, this);
	}
	return TRUE;
}

BOOL CDualTCP_Sock::Finalize_DoublePort() {
	m_bConnectionExit_P1 = TRUE;
	m_bConnectionExit_P2 = TRUE;
	WSACleanup();

	int nCount=0;
	if( m_pThreadP1 != NULL ){
		while( ::WaitForSingleObject( m_pThreadP1->m_hThread, 1 ) == WAIT_TIMEOUT ){
			nCount++;
			if( nCount > 500 ){
				::TerminateThread( m_pThreadP1->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadP1 = NULL;

	nCount=0;
	if( m_pThreadP2 != NULL ){
		while( ::WaitForSingleObject( m_pThreadP2->m_hThread, 1 ) == WAIT_TIMEOUT ){
			nCount++;
			if( nCount > 500 ){
				::TerminateThread( m_pThreadP2->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadP2 = NULL;

	DeleteCriticalSection(&Net_Cs1);
	DeleteCriticalSection(&Net_Cs2);

	return TRUE;
}

//=========================================================================================
// DESC   : Receive data buffer clear
//=========================================================================================
void CDualTCP_Sock::CmdClrBuff_Port1() {
	EnterCriticalSection( &Net_Cs1 );
	ZeroMemory(m_uszBuffPort1, sizeof(m_uszBuffPort1));
	m_pHeadPort1 = 0;
	m_pTailPort1 = 0;
	LeaveCriticalSection( &Net_Cs1 );
}
//=========================================================================================
// DESC   : Receive data buffer clear
//=========================================================================================
void CDualTCP_Sock::CmdClrBuff_Port2() {
	EnterCriticalSection( &Net_Cs2 );
	m_pHeadPort2 = 0;
	m_pTailPort2 = 0;
	LeaveCriticalSection( &Net_Cs2 );
}

//=========================================================================================
// DESC   : Send data write PortNo 1
// PARAM  : ( naming name same )
// RETURN : TRUE  = of normal complete
//          FALSE = Send error or Network disconnection 
//=========================================================================================
BOOL CDualTCP_Sock::Cmd_WrtStr_Port1( char* pSzSndData, int nSndDataLen )
{
	CmdClrBuff_Port1();

	m_strBuffPort1.Empty();
 	m_strDataPort1.Empty();
// 	////for test/////
// 	int n = send(NET_SOCK1, pSzSndData, nSndDataLen, 0);
// 	////////////////
// 
// 	if ( !m_bConnection_P1 || (send( NET_SOCK1, pSzSndData, nSndDataLen, 0 ) == SOCKET_ERROR) ){
// 		return FALSE;
// 	}
	return TRUE;
}
//=========================================================================================
// DESC   : Send data write PortNo 2
// PARAM  : ( naming name same )
// RETURN : TRUE  = of normal complete
//          FALSE = Send error or Network disconnection 
//=========================================================================================
BOOL CDualTCP_Sock::Cmd_WrtStr_Port2( char* pSzSndData, int nSndDataLen ) 
{
	CmdClrBuff_Port2();

	if ( !m_bConnection_P2 || send( NET_SOCK2 , pSzSndData, nSndDataLen , 0 ) == SOCKET_ERROR ){
		return FALSE;
	}
	return TRUE;
}

//=========================================================================================
// DESC   : Read receive data
// PARAM  :
// RETURN : 
//=========================================================================================
BOOL CDualTCP_Sock::Cmd_RecvStr_Port1( char* pSzRecvData, char* pSzTermStr, int nRecvMaxLen , int nTimeOut , int* pnReadRecvLen )
{
	double dStartTime , dThisTime , dCurrentTime , dSpanTime = 0;
	int	i = 0 , j, nReclock = 0;
	char ch;

	*pnReadRecvLen = 0;
	int nTermLen = strlen( pSzTermStr );

	if ( nTermLen > 0 ) {
		dStartTime = (double)GetTickCount64();
		while( m_bConnection_P1 ) {
			if ( GetReadChar_Port1( &ch ) ) 
			{
				pSzRecvData[i++] = ch;
				if ( i >= nRecvMaxLen ) { 
					*pnReadRecvLen = i; 
					return -2; 
				}
				if ( i >= nTermLen ) {
					for ( j = nTermLen-1 ; j >=0 ; j-- ) {
						if ( pSzRecvData[ ( i - 1 ) - ((nTermLen-1) - j) ] != pSzTermStr[ j ] ) break;
					}
					if ( j == -1 ) {
						pSzRecvData[ i - nTermLen ] = 0x00;
						*pnReadRecvLen = i;
						return 0;
					}
				}
			}
			else {
				_sleep(1);
				dCurrentTime = (double)GetTickCount64();
				dThisTime = dCurrentTime - dStartTime;
				if      ( dThisTime <= 0 ) {
					nReclock = 1;
				}
				else if ( ( nReclock != 2 ) && ( dThisTime > 10000 ) ) {
					nReclock = 2;
				}
				else {
					dSpanTime += dThisTime;
					if ( dSpanTime >= (unsigned) nTimeOut ) {
						*pnReadRecvLen = i;
						return -4;
					}
				}
				dStartTime = dCurrentTime;
			}
		}
	}
	return -1;
}

BOOL CDualTCP_Sock::Cmd_2DRecvStr_Port1(TokenArray* pSzRecvData, int nTblIdx, int nTimeOut, int nRecvMaxLen)
{
	double dStartTime, dThisTime, dCurrentTime, dSpanTime = 0;
	int nReclock = 0;

	CString str;

	dStartTime = (double)GetTickCount64();
	while (m_bConnection_P1) {
		if (GetReadString_Port1(&str)) //abc\r,efg\r
		{
			TokenArray tokens;
			tokens.clear();

			Tokenize(_T(","), str, tokens);

			if (tokens.size() != 4)
			{
				*pSzRecvData = tokens;
				return -2; //Data 4개가 들어 오지 않음.
			}

			int nTriggerCnt = g_trigger_generator.GetTriggerCount(nTblIdx);

			/* Data 길이는 IsShortLen 으로 검사.
			//Data가 안찍힐 경우, Error가 들어오는지 확인이 필요 함.
			//Error 들어오면 Count가 맞음.
			if (tokens.size() != nTriggerCnt) {
				return -2;
			}

			//얘로 검사
			for (int i = 0; i < tokens.size(); i++) {
				if (tokens[i].GetLength() >= nRecvMaxLen) {
					return -2;
				}
			}
			*/

			*pSzRecvData = tokens;

			return 0;

		}
		else {
			_sleep(1);
			dCurrentTime = (double)GetTickCount64();
			dThisTime = dCurrentTime - dStartTime;
			if (dThisTime <= 0) {
				nReclock = 1;
			}
			else if ((nReclock != 2) && (dThisTime > 10000)) {
				nReclock = 2;
			}
			else {
				dSpanTime += dThisTime;
				if (dSpanTime >= (unsigned)nTimeOut) {
					return -4;
				}
			}
			dStartTime = dCurrentTime;
		}
	}
	
	return -1;
}


//=========================================================================================
// DESC   : Read receive data
// PARAM  :
// RETURN : 
//=========================================================================================
int CDualTCP_Sock::Cmd_RecvStr_Port2( char *pSzRecvData , char *pSzTermStr , int nRecvMaxLen , int nTimeOut , int *pnReadRecvLen )
{
	double dStartTime , dThisTime , dCurrentTime , dSpanTime = 0;
	int i = 0, j, Reclock = 0;
	char ch;

	*pnReadRecvLen = 0;
	int nTermCnt = strlen( pSzTermStr );
	if ( nTermCnt > 0 ) {
		dStartTime = (double)GetTickCount64();
		while( m_bConnection_P2 ) {
			if( GetReadChar_Port2( &ch ) ) 
			{
				pSzRecvData[i++] = ch;
				if ( i >= nRecvMaxLen ) { *pnReadRecvLen = i; return -2; }
				if ( i >= nTermCnt ) {
					for ( j = nTermCnt-1 ; j >=0 ; j-- ) {
						if ( pSzRecvData[ ( i - 1 ) - ((nTermCnt-1) - j) ] != pSzTermStr[ j ] ) break;
					}
					if ( j == -1 ) {
						pSzRecvData[ i - nTermCnt ] = 0x00;
						*pnReadRecvLen = i;
						return 0;
					}
				}
			}
			else {
				_sleep(1);
				dCurrentTime = (double)GetTickCount64();
				dThisTime = dCurrentTime - dStartTime;
				if      ( dThisTime <= 0 ) {
					Reclock = 1;
				}
				else if ( ( Reclock != 2 ) && ( dThisTime > 10000 ) ) {
					Reclock = 2;
				}
				else {
					dSpanTime += dThisTime;
					if ( dSpanTime >= (unsigned) nTimeOut ) {
						*pnReadRecvLen = i;
						return -4;
					}
				}
				dStartTime = dCurrentTime;
			}
		}
	}
	return -1;
}

BOOL CDualTCP_Sock::GetReadChar_Port1( char *Res )
{
	EnterCriticalSection( &Net_Cs1 );
	if ( m_pHeadPort1 == m_pTailPort1 ) {
		LeaveCriticalSection( &Net_Cs1 );
		return FALSE;
	}
	*Res = m_uszBuffPort1[ m_pHeadPort1 ];
	m_pHeadPort1++;
	if ( m_pHeadPort1 >= MAX_GLOBAL_BUFFER ) m_pHeadPort1 = 0;
	LeaveCriticalSection( &Net_Cs1 );
	return TRUE;
}

BOOL CDualTCP_Sock::GetReadString_Port1(CString *Res)
{
	EnterCriticalSection(&Net_Cs1);

	if (!m_strDataPort1.IsEmpty()) {
		*Res = m_strDataPort1;
		m_strDataPort1.Empty();
	}
	else {
		LeaveCriticalSection(&Net_Cs1);
		return FALSE;
	}
	LeaveCriticalSection(&Net_Cs1);
	return TRUE;
}


BOOL CDualTCP_Sock::GetReadChar_Port2( char *Res )
{
	EnterCriticalSection( &Net_Cs2 );
	if ( m_pHeadPort2 == m_pTailPort2 ) {
		LeaveCriticalSection( &Net_Cs2 );
		return FALSE;
	}
	*Res = m_uszBuffPort2[ m_pHeadPort2 ];
	m_pHeadPort2++;
	if ( m_pHeadPort2 >= MAX_GLOBAL_BUFFER ) m_pHeadPort2 = 0;
	LeaveCriticalSection( &Net_Cs2 );
	return TRUE;
}


//--------------------------------------------------------------------------------------
// Client Port1 :: Thread & Connection Checking Function
//--------------------------------------------------------------------------------------
BOOL CDualTCP_Sock::ChkClientConnectPort1() {
	struct hostent *hp;
	//unsigned int addr;

	EnterCriticalSection( &Net_Cs1 );

// 	if ( ( NET_HOSTADDR0[0] != 0 ) && ( NET_HOSTADDR0[0] >= '0' ) && ( NET_HOSTADDR0[0] <= '9' ) ) {
// 		addr = inet_addr( NET_HOSTADDR0 );
// 		hp = gethostbyaddr( (char *) &addr , 4 , AF_INET );
// 	}
// 	else  {
		hp = gethostbyname( NET_HOSTADDR0 );
//	}
	if ( hp == NULL ) {
		printf( "[CLIENT1] IP Address or Port Error (%s:%d)\n" , NET_HOSTADDR0 , m_nPort1No );
		LeaveCriticalSection( &Net_Cs1 );
		return FALSE;
	}
	memset( &NET_SERVER1 , 0 , sizeof( NET_SERVER1 ) );
	memcpy( &(NET_SERVER1.sin_addr) , hp->h_addr , hp->h_length );
	NET_SERVER1.sin_family = hp->h_addrtype;
	NET_SERVER1.sin_port = htons( (unsigned short) m_nPort1No );
	LeaveCriticalSection( &Net_Cs1 );
	return TRUE;
}
UINT CDualTCP_Sock::ClientConnectPort1(LPVOID lpVoid) {
	CDualTCP_Sock* pSelf = (CDualTCP_Sock*)lpVoid;

	char Buffer[1024] = {0,};
	int retval , i;

	pSelf->m_bConnection_P1			= FALSE;
	pSelf->m_bConnectionExit_P1		= FALSE;
	pSelf->m_bConnectionExitSts_P1	= FALSE;
	pSelf->m_nMakePort_P1			= FALSE;

	while ( !pSelf->m_bConnectionExit_P1 ) {
		if ( !pSelf->m_nMakePort_P1 ) {
			if ( pSelf->ChkClientConnectPort1() ) pSelf->m_nMakePort_P1 = TRUE; // 2003.02.17
			else _sleep(3000);
		}
		else {
			pSelf->NET_SOCK1 = socket( AF_INET , SOCK_STREAM , 0 );
// 			if ( pSelf->NET_SOCK1 <0 ) {
// 				printf( "[CLIENT1] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort1No );
// 				pSelf->m_bConnection_P1 = FALSE;
// 				closesocket( pSelf->NET_SOCK1 );
// 				_sleep(1000);
// 				continue;
// 			}

			if ( connect( pSelf->NET_SOCK1 , (struct sockaddr*) &pSelf->NET_SERVER1 , sizeof(pSelf->NET_SERVER1) ) == SOCKET_ERROR ) {
				printf( "[CLIENT1] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort1No );
				pSelf->m_bConnection_P1 = FALSE;

				if( pSelf->ChkClientConnectPort1() != 0){
					pSelf->m_nMakePort_P1 = FALSE;
				}
				closesocket( pSelf->NET_SOCK1 );
				_sleep(1000);
				continue;
			}

			pSelf->m_bConnection_P1 = TRUE;
			pSelf->m_nConnectionCnt_P1++;

			while( pSelf->m_bConnection_P1 ) {
				char received_data[1024] = { 0, };
				retval = recv( pSelf->NET_SOCK1 , (char*)Buffer , sizeof(Buffer) , 0 );

				if (pSelf->NET_SOCK1 == 2932)
				{
					int a = 1;
				}
				if ( retval == SOCKET_ERROR ) {
					printf( "[CLIENT1] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort1No );
					pSelf->m_bConnection_P1 = FALSE;
					closesocket( pSelf->NET_SOCK1 );
					_sleep(1000);
					break;
				}
				if ( retval == 0 ) {
					printf( "[CLIENT1] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort1No );
					pSelf->m_bConnection_P1 = FALSE;
					closesocket( pSelf->NET_SOCK1 );
					_sleep(1000);
					break;
				}
				EnterCriticalSection( &pSelf->Net_Cs1 );
				for ( i = 0 ; i < retval ; i++ ) {
					if ( !pSelf->NET_Reject1 || ( Buffer[i] != pSelf->NET_Reject_Char1 ) ) {
						pSelf->m_uszBuffPort1[ pSelf->m_pTailPort1++ ] = Buffer[i];
						
						if (Buffer[i] == CR) 
						{
							pSelf->m_strBuffPort1 = pSelf->m_uszBuffPort1; //memcpy
							pSelf->m_strBuffPort1 += ',';

							//Data 저장, Buffer Clear
							pSelf->m_strDataPort1 += pSelf->m_strBuffPort1;
							pSelf->m_strBuffPort1.Empty();

							ZeroMemory(pSelf->m_uszBuffPort1, sizeof(pSelf->m_uszBuffPort1));
							pSelf->m_pTailPort1 = 0;

						}


						if ( pSelf->m_pTailPort1 >= MAX_GLOBAL_BUFFER ) pSelf->m_pTailPort1 = 0;
					}
				}
				LeaveCriticalSection( &pSelf->Net_Cs1 );
			}
		}
	}
	pSelf->m_bConnectionExitSts_P1 = TRUE;
	return 0;
}

//--------------------------------------------------------------------------------------
// Server Port1 :: Thread & Connection Checking Function
//--------------------------------------------------------------------------------------
BOOL CDualTCP_Sock::ChkServerConnectPort1() {
	char * szInterface= NULL;
	struct sockaddr_in local;

	EnterCriticalSection( &Net_Cs1 );

	local.sin_family = AF_INET;
	//local.sin_addr.s_addr = ( !szInterface ) ? INADDR_ANY : inet_addr( szInterface ); 
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons( (unsigned short) m_nPort1No );

	NET_SOCK_LISTEN1 = socket( AF_INET , SOCK_STREAM , 0 ); // TCP socket

	if ( NET_SOCK_LISTEN1 == INVALID_SOCKET ) {
		printf( "[SERVER1] Socket Error Error (%d)\n" , m_nPort1No );
		closesocket(NET_SOCK_LISTEN1);
		LeaveCriticalSection( &Net_Cs1 );
		return FALSE;
	}

	// 2. 소켓설정
	if ( bind( NET_SOCK_LISTEN1 , ( struct sockaddr* ) &local , sizeof( local ) ) == SOCKET_ERROR ) {
		printf( "[SERVER1] Network Bind Error (%d)\n" , m_nPort1No );
		closesocket(NET_SOCK_LISTEN1);
		LeaveCriticalSection( &Net_Cs1 );
		return FALSE;
	}

	// 3. 수신대기열생성
	if ( listen( NET_SOCK_LISTEN1 , 5 ) == SOCKET_ERROR ) {
		printf( "[SERVER1] Network Listen Error (%d)\n" , m_nPort1No );
		closesocket(NET_SOCK_LISTEN1);
		LeaveCriticalSection( &Net_Cs1 );
		return FALSE;
	}
	LeaveCriticalSection( &Net_Cs1 );
	return TRUE;
}
UINT CDualTCP_Sock::ServerConnectPort1(LPVOID lpVoid) {
	CDualTCP_Sock* pSelf = (CDualTCP_Sock *)lpVoid;

	char Buffer[1024];
	int retval , i;

	pSelf->m_bConnection_P1			= FALSE;
	pSelf->m_bConnectionExit_P1		= FALSE;
	pSelf->m_bConnectionExitSts_P1	= FALSE;
	pSelf->m_nMakePort_P1			= FALSE;

	while ( !pSelf->m_bConnectionExit_P1 ) {
		if ( !pSelf->m_nMakePort_P1 ) {
			if ( pSelf->ChkServerConnectPort1() ) {
				pSelf->m_nMakePort_P1 = TRUE;
			}
			else _sleep(3000);
		}
		else {
			pSelf->NET_FROMLEN1 = sizeof( pSelf->NET_FROM1 );
			pSelf->NET_SOCK1 = accept( pSelf->NET_SOCK_LISTEN1 , ( struct sockaddr* ) &pSelf->NET_FROM1 , &pSelf->NET_FROMLEN1 );
			if ( pSelf->NET_SOCK1 == INVALID_SOCKET ) {
				printf( "[SERVER1] Accept Error (%d)\n" , pSelf->m_nPort1No );
				pSelf->m_bConnection_P1 = FALSE;
				closesocket( pSelf->NET_SOCK1 );
				_sleep(1000);
				continue;
			}

			pSelf->m_bConnection_P1 = TRUE;
			pSelf->m_nConnectionCnt_P1++;

			printf( "[SERVER1] Connected. Connect (%d) from Client[%d]\n" , pSelf->m_nPort1No , pSelf->m_nConnectionCnt_P1 );

			while ( pSelf->m_bConnection_P1 ) {
				retval = recv( pSelf->NET_SOCK1 , (char*)Buffer , sizeof( Buffer ) , 0 );

				if ( retval == SOCKET_ERROR ) {
					printf( "[SERVER1] Disconnect. Wait Connect (%d) from Client\n" , pSelf->m_nPort1No );
					closesocket( pSelf->NET_SOCK1 );
					pSelf->m_bConnection_P1 = FALSE;
					break;
				}
				if ( retval == 0 ) {
					printf( "[SERVER1] Disconnect. Wait Connect (%d) from Client\n" ,pSelf-> m_nPort1No );
					closesocket( pSelf->NET_SOCK1 );
					pSelf->m_bConnection_P1 = FALSE;
					break;
				}
				EnterCriticalSection( &pSelf->Net_Cs1 );
				for ( i = 0 ; i < retval ; i++ ) {
					if ( !pSelf->NET_Reject1 || ( Buffer[i] != pSelf->NET_Reject_Char1 ) ) {
						pSelf->m_uszBuffPort1[ pSelf->m_pTailPort1++ ] = Buffer[i];
						if ( pSelf->m_pTailPort1 >= MAX_GLOBAL_BUFFER ) pSelf->m_pTailPort1 = 0;
					}
				}
				LeaveCriticalSection( &pSelf->Net_Cs1 );
			}
		}
	}
	pSelf->m_bConnectionExitSts_P1 = TRUE;
	return 0;
}

//--------------------------------------------------------------------------------------
// Client Port2 :: Thread & Connection Checking Function
//--------------------------------------------------------------------------------------
BOOL CDualTCP_Sock::ChkClientConnectPort2() {
	struct hostent *hp;
	unsigned int addr;

	EnterCriticalSection( &Net_Cs2 );

	if (/* ( NET_HOSTADDR0[0] != 0 ) && */( NET_HOSTADDR0[0] >= '0' ) && ( NET_HOSTADDR0[0] <= '9' ) ) {
		addr = inet_addr( NET_HOSTADDR0 );
		hp = gethostbyaddr( (char *) &addr , 4 , AF_INET );
	}
	else  {
		hp = gethostbyname( NET_HOSTADDR0 );
	}
	if ( hp == NULL ) {
		printf( "[CLIENT2] IP Address or Port Error (%s:%d)\n" , NET_HOSTADDR0 , m_nPort2No );
		LeaveCriticalSection( &Net_Cs2 );
		return FALSE;
	}
	memset( &NET_SERVER2 , 0 , sizeof( NET_SERVER2 ) );
	memcpy( &(NET_SERVER2.sin_addr) , hp->h_addr , hp->h_length );
	NET_SERVER2.sin_family = hp->h_addrtype;
	NET_SERVER2.sin_port = htons( (unsigned short) m_nPort2No );
	LeaveCriticalSection( &Net_Cs2 );
	return TRUE;
}
UINT CDualTCP_Sock::ClientConnectPort2(LPVOID lpVoid) {
	CDualTCP_Sock* pSelf = (CDualTCP_Sock *)lpVoid;

	char Buffer[1024] = {0,};
	int retval , i;

	pSelf->m_bConnection_P2			= FALSE;
	pSelf->m_bConnectionExit_P2		= FALSE;
	pSelf->m_bConnectionExitSts_P2	= FALSE;
	pSelf->m_nMakePort_P2			= FALSE;

	while ( !pSelf->m_bConnectionExit_P2 ) {
		if ( !pSelf->m_nMakePort_P2 ) {
			if ( pSelf->ChkClientConnectPort2() ) pSelf->m_nMakePort_P2 = TRUE; // 2003.02.17
			else _sleep(3000);
		}
		else {
			pSelf->NET_SOCK2 = socket( AF_INET , SOCK_STREAM , 0 );
// 			if ( pSelf->NET_SOCK2 <0 ) {
// 				printf( "[CLIENT2] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort2No );
// 				pSelf->m_bConnection_P2 = FALSE;
// 				closesocket( pSelf->NET_SOCK2 );
// 				_sleep(1000);
// 				continue;
// 			}

			if ( connect( pSelf->NET_SOCK2 , (struct sockaddr*) &pSelf->NET_SERVER2 , sizeof(pSelf->NET_SERVER2) ) == SOCKET_ERROR ) {
				printf( "[CLIENT2] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort2No );
				pSelf->m_bConnection_P2 = FALSE;
				closesocket( pSelf->NET_SOCK2 );
				_sleep(1000);
				continue;
			}

			pSelf->m_bConnection_P2 = TRUE;
			pSelf->m_nConnectionCnt_P2++;

			while( pSelf->m_bConnection_P2 ) {
				retval = recv( pSelf->NET_SOCK2 , Buffer , sizeof(Buffer) , 0 );
				if ( retval == SOCKET_ERROR ) {
					printf( "[CLIENT2] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort2No );
					pSelf->m_bConnection_P2 = FALSE;
					closesocket( pSelf->NET_SOCK2 );
					_sleep(1000);
					break;
				}
				if ( retval == 0 ) {
					printf( "[CLIENT2] Connect Retry to Server (%s:%d)\n" , pSelf->NET_HOSTADDR0 , pSelf->m_nPort2No );
					pSelf->m_bConnection_P2 = FALSE;
					closesocket( pSelf->NET_SOCK2 );
					_sleep(1000);
					break;
				}
				EnterCriticalSection( &pSelf->Net_Cs2 );
				for ( i = 0 ; i < retval ; i++ ) {
					if ( !pSelf->NET_Reject2 || ( Buffer[i] != pSelf->NET_Reject_Char2 ) ) {
						pSelf->m_uszBuffPort2[ pSelf->m_pTailPort2++ ] = Buffer[i];
						if ( pSelf->m_pTailPort2 >= MAX_GLOBAL_BUFFER ) pSelf->m_pTailPort2 = 0;
					}
				}
				LeaveCriticalSection( &pSelf->Net_Cs2 );
			}
		}
	}
	pSelf->m_bConnectionExitSts_P2 = TRUE;
	return 0;
}

//--------------------------------------------------------------------------------------
// Server Port2 :: Thread & Connection Checking Function
//--------------------------------------------------------------------------------------
BOOL CDualTCP_Sock::ChkServerConnectPort2() {
	char *szInterface= NULL;
	struct sockaddr_in local;

	EnterCriticalSection( &Net_Cs2 );

	local.sin_family = AF_INET;
	//local.sin_addr.s_addr = ( !szInterface ) ? INADDR_ANY : inet_addr( szInterface ); 
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons( (unsigned short) m_nPort2No );
	NET_SOCK_LISTEN2 = socket( AF_INET , SOCK_STREAM , 0 ); // TCP socket

	if ( NET_SOCK_LISTEN2 == INVALID_SOCKET ) {
		printf( "[SERVER2] Socket Error Error (%d)\n" , m_nPort2No );
		LeaveCriticalSection( &Net_Cs2 );
		return FALSE;
	}

	if ( bind( NET_SOCK_LISTEN2 , ( struct sockaddr* ) &local , sizeof( local ) ) == SOCKET_ERROR ) {
		printf( "[SERVER2] Network Bind Error (%d)\n" , m_nPort2No );
		LeaveCriticalSection( &Net_Cs2 );
		return FALSE;
	}

	if ( listen( NET_SOCK_LISTEN2 , 5 ) == SOCKET_ERROR ) {
		printf( "[SERVER2] Network Listen Error (%d)\n" , m_nPort2No );
		LeaveCriticalSection( &Net_Cs2 );
		return FALSE;
	}
	LeaveCriticalSection( &Net_Cs2 );
	return TRUE;
}
UINT CDualTCP_Sock::ServerConnectPort2(LPVOID lpVoid) {
	CDualTCP_Sock* pSelf = (CDualTCP_Sock*)lpVoid;

	unsigned char Buffer[128];
	int retval , i;

	pSelf->m_bConnection_P2			= FALSE;
	pSelf->m_bConnectionExit_P2		= FALSE;
	pSelf->m_bConnectionExitSts_P2	= FALSE;
	pSelf->m_nMakePort_P2			= FALSE;

	while ( !pSelf->m_bConnectionExit_P2 ) {
		if ( !pSelf->m_nMakePort_P2 ) {
			if ( pSelf->ChkServerConnectPort2() ) pSelf->m_nMakePort_P2 = TRUE; // 2003.02.17
			else _sleep(3000);
		}
		else {
			pSelf->NET_FROMLEN2 = sizeof(pSelf->NET_FROM2 );
			pSelf->NET_SOCK2 = accept( pSelf->NET_SOCK_LISTEN2 , ( struct sockaddr* )&pSelf->NET_FROM2 , &pSelf->NET_FROMLEN2 );
			if ( pSelf->NET_SOCK2 == INVALID_SOCKET ) {
				printf( "[SERVER2] Accept Error (%d)\n" , pSelf->m_nPort2No );
				pSelf->m_bConnection_P2 = FALSE;
				closesocket( pSelf->NET_SOCK2 );
				_sleep(1000);
				continue;
			}

			pSelf->m_bConnection_P2 = TRUE;
			pSelf->m_nConnectionCnt_P2++;

			printf( "[SERVER2] Connected. Connect (%d) from Client[%d]\n" , pSelf->m_nPort2No , pSelf->m_nConnectionCnt_P2 );

			while ( pSelf->m_bConnection_P2 ) {
				retval = recv( pSelf->NET_SOCK2 , (char*)Buffer , sizeof( Buffer ) , 0 );

				if ( retval == SOCKET_ERROR ) {
					printf( "[SERVER2] Disconnect. Wait Connect (%d) from Client\n" , pSelf->m_nPort2No );
					closesocket( pSelf->NET_SOCK2 );
					pSelf->m_bConnection_P2 = FALSE;
					break;
				}
				if ( retval == 0 ) {
					printf( "[SERVER2] Disconnect. Wait Connect (%d) from Client\n" , pSelf->m_nPort2No );
					closesocket( pSelf->NET_SOCK2 );
					pSelf->m_bConnection_P2 = FALSE;
					break;
				}
				EnterCriticalSection( &pSelf->Net_Cs2 );
				for ( i = 0 ; i < retval ; i++ ) {
					if ( !pSelf->NET_Reject2 || ( Buffer[i] != pSelf->NET_Reject_Char2 ) ) {
						pSelf->m_uszBuffPort2[ pSelf->m_pTailPort2++ ] = Buffer[i];
						if ( pSelf->m_pTailPort2 >= MAX_GLOBAL_BUFFER ) pSelf->m_pTailPort2 = 0;
					}
				}
				LeaveCriticalSection( &pSelf->Net_Cs2 );
			}
		}
	}
	pSelf->m_bConnectionExitSts_P2 = TRUE;
	return 0;
}


//============================================================================== < PORT 1 >
// BOOL CDualTCP_Sock::Net_Write_Char_1( char ch ) {
// 	return( Cmd_WrtStr_1( &ch , 1 ) );
// }
// void CDualTCP_Sock::Net_Set_Reject_Char_1( unsigned char ch ) {
// 	NET_Reject1 = TRUE;
// 	NET_Reject_Char1 = ch;
// }
// void CDualTCP_Sock::Net_Set_Reject_Char_2( unsigned char ch ) {
// 	NET_Reject2 = TRUE;
// 	NET_Reject_Char2 = ch;
// }
// BOOL CDualTCP_Sock::Net_Connect_Status_1() {
// 	return NET_CONNECTION1;
// }
// int CDualTCP_Sock::Net_Connect_Count_1() {
// 	return NET_CONN_COUNT1;
// }
// void CDualTCP_Sock::Net_Wait_Connect_1() {
// 	while( !NET_CONNECTION1 ) _sleep(50);
// }
// int CDualTCP_Sock::Net_Read_Connect_Port_1() {
// 	return NET_PORT1;
// }
// char* CDualTCP_Sock::Net_Read_Connect_Host_1() {
// 	return NET_HOSTADDR0;
// }
// BOOL CDualTCP_Sock::Net_Read_Count_Char_1( char *Buffer , int ReqCount , int *ReadCount , int TimeOut ) {
// 	DWORD start_clock , this_clock , curr_clock , out_clock = 0;
// 	int Reclock = 0;
// 	int		i = 0;
// 	char    ch;
// 
// 	*ReadCount = 0;
// 	//	start_clock = clock(); // 2003.10.09
// 	start_clock = (double)GetTickCount64(); // 2003.10.09
// 	while( NET_CONNECTION1 ) {
// 		if ( Net_Read_Char_1( &ch ) ) {
// 			Buffer[i++] = ch;
// 			if ( i >= ReqCount ) {
// 				Buffer[i] = 0x00;
// 				*ReadCount = i;
// 				return( TRUE );
// 			}
// 		}
// 		else {
// 			_sleep(1);
// 			//			curr_clock = clock(); // 2003.10.09
// 			curr_clock = (double)GetTickCount64(); // 2003.10.09
// 			this_clock = curr_clock - start_clock;
// 			if      ( this_clock <= 0 ) {
// 				Reclock = 1;
// 			}
// 			else if ( ( Reclock != 2 ) && ( this_clock > 10000 ) ) {
// 				Reclock = 2;
// 			}
// 			else {
// 				out_clock += this_clock;
// 				if ( out_clock >= (unsigned) TimeOut ) {
// 					Buffer[i] = 0;
// 					*ReadCount = i;
// 					return FALSE;
// 				}
// 			}
// 			start_clock = curr_clock;
// 		}
// 	}
// 	return FALSE;
// }
// =========================================================================================
// DESC   : 외부에서 불러서 Send 하는 부분 
// PARAM  : *szSndBuff   = send data       [ ex. <stx>LON<etx> ]
//          nLength      = send data length
//          nTimeOut     = Receive data wait time out
//          nReadDataLen = Read Data Length
// RETURN : FALSE = Send Fail or Read Fail
//           TRUE  = of normal complete
// =========================================================================================
// BOOL CDualTCP_Sock::Cmd_Send_Port1( char* pSzSndData , int nSndDataLen , int nTimeOut , int *nReadDataLen )
// {
// 	CmdClrBuff_Port1();
// 	if ( !Cmd_WrtStr_Port1( pSzSndData , strlen( pSzSndData ) ) ) {
// 		return FALSE;
// 	}
// 	if ( !Cmd_ReadStr_Port1( pSzSndData , nSndDataLen , nTimeOut , nReadDataLen ) ) {
// 		return FALSE;
// 	}
// 	return TRUE;
// }
//=========================================================================================
// DESC   : Send data write
// PARAM  : ( naming name same )
// RETURN : TRUE  = of normal complete
//          FALSE = Send error or Network disconnection 
//=========================================================================================
// BOOL CDualTCP_Sock::Cmd_ReadStr_Port1( char* pSzSndData, int nSndDataLen , int nTimeOut , int *nReadDataLen ) {
// 	DWORD dwThisTime , dwCurrentTime , dwSpanTime = 0;
// 	int nReclock  = 0;
// 	*nReadDataLen = 0;
// 
// 	DWORD dwStartTime = (double)GetTickCount64();
// 	while( NET_CONNECTION1 )
// 	{
// 		switch( Cmd_ReadStr_Port1( pSzSndData , nSndDataLen , nReadDataLen ) )
// 		{
// 		case eReadStrPortState_NoData :
// 			{
// 				_sleep(1);
// 				dwCurrentTime = (double)GetTickCount64();
// 				dwThisTime    = dwCurrentTime - dwStartTime;
// 
// 				if( dwThisTime <= 0 ) {
// 					nReclock = 1;
// 				} else if ( ( nReclock != 2 ) && ( dwThisTime > 10000 ) ) {
// 					nReclock = 2;
// 				} else {
// 					dwSpanTime += dwThisTime;
// 					if ( dwSpanTime >= (unsigned)nTimeOut ) {
// 						return FALSE;
// 					}
// 				}
// 				dwStartTime = dwCurrentTime;
// 			}break;
// 		case eReadStrPortState_Continue : {               } break;
// 		case eReadStrPortState_ChkOK    : {	return TRUE;  } break;
// 		case eReadStrPortState_OverFlow : { return FALSE; } break;
// 		}
// 	}
// 	return FALSE;
// }
// int CDualTCP_Sock::Cmd_ReadStr_Port1( char* pSzSndData, int nSndDataLen, int *nReadDataLen ) {
// 	EnterCriticalSection( &Net_Cs1 );
// 
// 	if ( m_pHeadPort1 == m_pTailPort1 ) {
// 		LeaveCriticalSection( &Net_Cs1 );
// 		return 0;
// 	}
// 
// 	do {
// 		pSzSndData[(*nReadDataLen)] = GlobalBuffer1[ m_pHeadPort1 ];
// 		m_pHeadPort1++;
// 		if ( m_pHeadPort1 >= MAX_GLOBAL_BUFFER ) m_pHeadPort1 = 0;
// 		if ( pSzSndData[(*nReadDataLen)] == 2 ) {
// 			pSzSndData[(*nReadDataLen)] = 0 ;
// 			(*nReadDataLen)++;
// 			LeaveCriticalSection( &Net_Cs1 );
// 			return 2;
// 		}
// 		(*nReadDataLen)++;
// 		if ( (*nReadDataLen) >= nSndDataLen ) {
// 			LeaveCriticalSection( &Net_Cs1 );
// 			return 3;
// 		}
// 		if ( m_pHeadPort1 == m_pTailPort1 ) {
// 			LeaveCriticalSection( &Net_Cs1 );
// 			return 1;
// 		}
// 	} while( TRUE );
// 
// 	LeaveCriticalSection( &Net_Cs1 );
// 	return 1;
// }
// int CDualTCP_Sock::Net_Read_Char_End_2_H_1( char *rch , int max , int *rc , BOOL *HCODE ) {
// 	EnterCriticalSection( &Net_Cs1 );
// 	if ( m_pHeadPort1 == m_pTailPort1 ) {
// 		LeaveCriticalSection( &Net_Cs1 );
// 		return 0;
// 	}
// 	do {
// 		if ( *HCODE ) {
// 			*HCODE = FALSE;
// 			rch[(*rc)] = GlobalBuffer1[ m_pHeadPort1 ];
// 			m_pHeadPort1++;
// 			if ( m_pHeadPort1 >= MAX_GLOBAL_BUFFER ) m_pHeadPort1 = 0;
// 			(*rc)++;
// 			if ( (*rc) >= max ) {
// 				LeaveCriticalSection( &Net_Cs1 );
// 				return 3;
// 			}
// 			if ( m_pHeadPort1 == m_pTailPort1 ) {
// 				LeaveCriticalSection( &Net_Cs1 );
// 				return 1;
// 			}
// 		}
// 		else {
// 			if ( GlobalBuffer1[ m_pHeadPort1 ] == 3 ) {
// 				*HCODE = TRUE;
// 				m_pHeadPort1++;
// 				if ( m_pHeadPort1 >= MAX_GLOBAL_BUFFER ) m_pHeadPort1 = 0;
// 				if ( m_pHeadPort1 == m_pTailPort1 ) {
// 					LeaveCriticalSection( &Net_Cs1 );
// 					return 1;
// 				}
// 			}
// 			else {
// 				rch[(*rc)] = GlobalBuffer1[ m_pHeadPort1 ];
// 				m_pHeadPort1++;
// 				if ( m_pHeadPort1 >= MAX_GLOBAL_BUFFER ) m_pHeadPort1 = 0;
// 				if ( rch[(*rc)] == 2 ) {
// 					rch[(*rc)] = 0 ;
// 					(*rc)++;
// 					LeaveCriticalSection( &Net_Cs1 );
// 					return 2;
// 				}
// 				(*rc)++;
// 				if ( (*rc) >= max ) {
// 					LeaveCriticalSection( &Net_Cs1 );
// 					return 3;
// 				}
// 				if ( m_pHeadPort1 == m_pTailPort1 ) {
// 					LeaveCriticalSection( &Net_Cs1 );
// 					return 1;
// 				}
// 			}
// 		}
// 	}
// 	while( TRUE );
// 	LeaveCriticalSection( &Net_Cs1 );
// 	return 1;
// }

//============================================================== < PORT 2 >
// BOOL CDualTCP_Sock::Net_Connect_Status_2() {
// 	return NET_CONNECTION2;
// }
// void CDualTCP_Sock::Net_Wait_Connect_2() {
// 	while( !NET_CONNECTION2 ) _sleep(50);
// }
// BOOL CDualTCP_Sock::Net_Write_Char_2( char ch ) {
// 	return( Cmd_WrtStr_Port2( &ch , 1 ) );
// }
// BOOL CDualTCP_Sock::Net_Read_Count_Char_2( char *Buffer , int ReqCount , int *ReadCount , int TimeOut ) {
// 	DWORD start_clock , this_clock , curr_clock , out_clock = 0;
// 	int Reclock = 0;
// 	int		i = 0;
// 	char    ch;
// 
// 	*ReadCount = 0;
// 	//	start_clock = clock(); // 2003.10.09
// 	start_clock = (double)GetTickCount64(); // 2003.10.09
// 	while( NET_CONNECTION2 ) {
// 		if ( GetReadChar_Port2( &ch ) ) {
// 			Buffer[i++] = ch;
// 			if ( i >= ReqCount ) {
// 				Buffer[i] = 0x00;
// 				*ReadCount = i;
// 				return( TRUE );
// 			}
// 		}
// 		else {
// 			_sleep(1);
// 			//			curr_clock = clock(); // 2003.10.09
// 			curr_clock = (double)GetTickCount64(); // 2003.10.09
// 			this_clock = curr_clock - start_clock;
// 			if      ( this_clock <= 0 ) {
// 				Reclock = 1;
// 			}
// 			else if ( ( Reclock != 2 ) && ( this_clock > 10000 ) ) {
// 				Reclock = 2;
// 			}
// 			else {
// 				out_clock += this_clock;
// 				if ( out_clock >= (unsigned) TimeOut ) {
// 					Buffer[i] = 0;
// 					*ReadCount = i;
// 					return FALSE;
// 				}
// 			}
// 			start_clock = curr_clock;
// 		}
// 	}
// 	return FALSE;
// }
// int CDualTCP_Sock::Net_Connect_Count_2() {
// 	return NET_CONN_COUNT2;
// }
// int CDualTCP_Sock::Net_Read_Connect_Port_2() {
// 	return NET_PORT2;
// }
// char* CDualTCP_Sock::Net_Read_Connect_Host_2() {
// 	return NET_HOSTADDR0;
// }
// int CDualTCP_Sock::Net_Read_Char_End_2_H_2( char *rch , int max , int *rc , BOOL *HCODE ) {
// 	EnterCriticalSection( &Net_Cs2 );
// 	if ( GlobalPointerHead2 == GlobalPointerTail2 ) {
// 		LeaveCriticalSection( &Net_Cs2 );
// 		return 0;
// 	}
// 	do {
// 		if ( *HCODE ) {
// 			*HCODE = FALSE;
// 			rch[(*rc)] = GlobalBuffer2[ GlobalPointerHead2 ];
// 			GlobalPointerHead2++;
// 			if ( GlobalPointerHead2 >= MAX_GLOBAL_BUFFER ) GlobalPointerHead2 = 0;
// 			(*rc)++;
// 			if ( (*rc) >= max ) {
// 				LeaveCriticalSection( &Net_Cs2 );
// 				return 3;
// 			}
// 			if ( GlobalPointerHead2 == GlobalPointerTail2 ) {
// 				LeaveCriticalSection( &Net_Cs2 );
// 				return 1;
// 			}
// 		}
// 		else {
// 			if ( GlobalBuffer2[ GlobalPointerHead2 ] == 3 ) {
// 				*HCODE = TRUE;
// 				GlobalPointerHead2++;
// 				if ( GlobalPointerHead2 >= MAX_GLOBAL_BUFFER ) GlobalPointerHead2 = 0;
// 				if ( GlobalPointerHead2 == GlobalPointerTail2 ) {
// 					LeaveCriticalSection( &Net_Cs2 );
// 					return 1;
// 				}
// 			}
// 			else {
// 				rch[(*rc)] = GlobalBuffer2[ GlobalPointerHead2 ];
// 				GlobalPointerHead2++;
// 				if ( GlobalPointerHead2 >= MAX_GLOBAL_BUFFER ) GlobalPointerHead2 = 0;
// 				if ( rch[(*rc)] == 2 ) {
// 					rch[(*rc)] = 0 ;
// 					(*rc)++;
// 					LeaveCriticalSection( &Net_Cs2 );
// 					return 2;
// 				}
// 				(*rc)++;
// 				if ( (*rc) >= max ) {
// 					LeaveCriticalSection( &Net_Cs2 );
// 					return 3;
// 				}
// 				if ( GlobalPointerHead2 == GlobalPointerTail2 ) {
// 					LeaveCriticalSection( &Net_Cs2 );
// 					return 1;
// 				}
// 			}
// 		}
// 	}
// 	while( TRUE );
// 	LeaveCriticalSection( &Net_Cs2 );
// 	return 1;
// }
// BOOL CDualTCP_Sock::Net_Read_String_End_2_2( char *Buffer , int MaxCount , int TimeOut , int *ReadCnt ) {
// 	DWORD start_clock , this_clock , curr_clock , out_clock = 0;
// 	int Reclock = 0;
// 
// 	*ReadCnt = 0;
// 	//	start_clock = clock(); // 2003.10.09
// 	start_clock = (double)GetTickCount64(); // 2003.10.09
// 	while( NET_CONNECTION2 ) {
// 		switch( Net_Read_Char_End_2_2( Buffer , MaxCount , ReadCnt ) ) {
// 		case 0 : // No Data
// 			_sleep(1);
// 			//			curr_clock = clock(); // 2003.10.09
// 			curr_clock = (double)GetTickCount64(); // 2003.10.09
// 			this_clock = curr_clock - start_clock;
// 			if      ( this_clock <= 0 ) {
// 				Reclock = 1;
// 			}
// 			else if ( ( Reclock != 2 ) && ( this_clock > 10000 ) ) {
// 				Reclock = 2;
// 			}
// 			else {
// 				out_clock += this_clock;
// 				if ( out_clock >= (unsigned) TimeOut ) {
// 					return FALSE;
// 				}
// 			}
// 			start_clock = curr_clock;
// 			break;
// 		case 1 : // Continue
// 			break;
// 		case 2 : // Check 2 OK
// 			return TRUE;
// 			break;
// 		case 3 : // Overflow
// 			return FALSE;
// 			break;
// 		}
// 	}
// 	return FALSE;
// }
// int CDualTCP_Sock::Net_Read_Char_End_2_2( char *rch , int max , int *rc ) {
// 	EnterCriticalSection( &Net_Cs2 );
// 	if ( GlobalPointerHead2 == GlobalPointerTail2 ) {
// 		LeaveCriticalSection( &Net_Cs2 );
// 		return 0;
// 	}
// 	do {
// 		rch[(*rc)] = GlobalBuffer2[ GlobalPointerHead2 ];
// 		GlobalPointerHead2++;
// 		if ( GlobalPointerHead2 >= MAX_GLOBAL_BUFFER ) GlobalPointerHead2 = 0;
// 		if ( rch[(*rc)] == 2 ) {
// 			rch[(*rc)] = 0 ;
// 			(*rc)++;
// 			LeaveCriticalSection( &Net_Cs2 );
// 			return 2;
// 		}
// 		(*rc)++;
// 		if ( (*rc) >= max ) {
// 			LeaveCriticalSection( &Net_Cs2 );
// 			return 3;
// 		}
// 		if ( GlobalPointerHead2 == GlobalPointerTail2 ) {
// 			LeaveCriticalSection( &Net_Cs2 );
// 			return 1;
// 		}
// 	}
// 	while( TRUE );
// 	LeaveCriticalSection( &Net_Cs2 );
// 	return 1;
// }
// BOOL CDualTCP_Sock::Net_Send_Receive_2( char *SendBuffer , int dl , int timeout , int *count ) {
// 	CmdClrBuff_Port2();
// 	if ( !Cmd_WrtStr_Port2( SendBuffer , strlen( SendBuffer ) ) ) {
// 		return FALSE;
// 	}
// 	if ( !Net_Read_String_End_2_2( SendBuffer , dl , timeout , count ) ) {
// 		return FALSE;
// 	}
// 	return TRUE;
// }