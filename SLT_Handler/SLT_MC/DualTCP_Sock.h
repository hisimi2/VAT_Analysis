#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <process.h>
#include <windows.h>
#include <winuser.h>
#include <winbase.h>
#include <time.h>
#include <winsock2.h>
#include <commdlg.h>
#include <io.h>

#define	MAX_GLOBAL_BUFFER		(50100)

enum eReadStrPortState{
	eReadStrPortState_NoData=0,
	eReadStrPortState_Continue,
	eReadStrPortState_ChkOK,
	eReadStrPortState_OverFlow,
};

typedef std::vector<CString>	TokenArray;


class CDualTCP_Sock
{
public:
	CDualTCP_Sock(void);
	~CDualTCP_Sock(void);

public:
	// Network initialize & finalize port
	BOOL Initialize_DoublePort( char * , int , int , BOOL );
	BOOL Initialize_Port( char * , int, BOOL );
	BOOL Finalize_DoublePort();

	// ============================================= NETWORK_1 Function	
	BOOL Cmd_WrtStr_Port1( char* pSzSndData, int nSndDataLen );
	int Cmd_RecvStr_Port1( char* pSzRecvData, char* pSzTermStr/*=<CR>*/, int nRecvMaxLen, int nTimeOut, int* pnReadRecvLen );
	int Cmd_2DRecvStr_Port1(TokenArray* pSzRecvData, int nTblIdx, int nTimeOut, int nRecvMaxLen);

 	// ============================================= NETWORK_2 Function	
	BOOL Cmd_WrtStr_Port2( char* pSzSndData, int nSndDataLen );	
 	int Cmd_RecvStr_Port2( char* pSzRecvData , char* pSzTermStr, int nRecvMaxLen, int nTimeOut, int* pnReadRecvLen);
	
protected:
	static UINT ClientConnectPort1(LPVOID lpVoid);
	static UINT ServerConnectPort1(LPVOID lpVoid);
 	static UINT ClientConnectPort2(LPVOID lpVoid);
 	static UINT ServerConnectPort2(LPVOID lpVoid);

	BOOL  ChkClientConnectPort2();
	BOOL  ChkServerConnectPort1();	
	BOOL  ChkServerConnectPort2();
	BOOL  ChkClientConnectPort1();
private:
	BOOL Net_Winsock_Init();
	BOOL Net_Connect_Double_Port2( char * , int , int , BOOL , BOOL );
	BOOL Net_Connect_Port(char * , int, BOOL );
	// ============================================= NETWORK_1 Function
	void CmdClrBuff_Port1();
	BOOL GetReadChar_Port1( char *Res );
	BOOL GetReadString_Port1(CString *Res);
	// ============================================= NETWORK_2 Function
	void CmdClrBuff_Port2();
	BOOL GetReadChar_Port2( char *Res );

private:
	CRITICAL_SECTION Net_Cs1, Net_Cs2;
	char	NET_HOSTADDR0[65];
	
	// [ == PORT 1 ==]
	//---------------------------------------------------------------------------
	unsigned char m_uszBuffPort1[ MAX_GLOBAL_BUFFER + 1 ];
	int  m_pHeadPort1, m_pTailPort1;
	CString m_strBuffPort1;
	CString m_strDataPort1;
	//---------------------------------------------------------------------------
	// Just Use TCP
	//---------------------------------------------------------------------------
	BOOL	m_bConnection_P1	   ;
	BOOL	m_bConnectionExit_P1   ;
	BOOL	m_bConnectionExitSts_P1;
	int		m_nPort1No             ;
	int		m_nConnectionCnt_P1    ;
	int		m_nMakePort_P1         ;
	//---------------------------------------------------------------------------
	SOCKET	NET_SOCK1;
	SOCKET	NET_SOCK_LISTEN1;
	int		NET_FROMLEN1;	

	BOOL                NET_Reject1;
	unsigned char       NET_Reject_Char1;
	//---------------------------------------------------------------------------

	// [ == PORT 2 == ]
	//---------------------------------------------------------------------------
	unsigned char m_uszBuffPort2[ MAX_GLOBAL_BUFFER + 1 ];
	int  m_pHeadPort2, m_pTailPort2;
	//---------------------------------------------------------------------------
	// Just Use TCP
	//---------------------------------------------------------------------------
	BOOL	m_bConnection_P2	   ;
	BOOL	m_bConnectionExit_P2   ;
	BOOL	m_bConnectionExitSts_P2;
	int		m_nPort2No             ;
	int		m_nConnectionCnt_P2    ;
	int		m_nMakePort_P2         ;
	//---------------------------------------------------------------------------
	SOCKET				NET_SOCK2;
	SOCKET				NET_SOCK_LISTEN2;
	int					NET_FROMLEN2;

	BOOL                NET_Reject2;
	unsigned char       NET_Reject_Char2;
	//---------------------------------------------------------------------------

	CWinThread* m_pThreadP1;
	CWinThread* m_pThreadP2;

	struct sockaddr_in	NET_FROM1;
	struct sockaddr_in	NET_SERVER1;
	struct sockaddr_in	NET_FROM2;
	struct sockaddr_in	NET_SERVER2;
};

// ==================================================================== [ Function No-Use ]
//void  Net_Set_Reject_Char_1( unsigned char ch );
//void  Net_Set_Reject_Char_2( unsigned char ch );
//BOOL  Net_Write_Char_1( char );
//BOOL  Net_Connect_Status_1();
//int   Net_Connect_Count_1();
//void  Net_Wait_Connect_1();
//int   Net_Read_Connect_Port_1();
//char* Net_Read_Connect_Host_1();
//BOOL  Net_Read_Count_Char_1( char * , int , int * , int );
//BOOL  Cmd_Send_Port1( char *SendBuffer , int dl , int timeout , int *count );
//BOOL  Cmd_ReadStr_Port1( char* pSzSndData, int nSndDataLen, int nTimeOut, int* pnReadCnt );
//int   Cmd_ReadStr_Port1( char* pSzSndData, int nSndDataLen, int *nReadDataLen );
//int   Net_Read_Char_End_2_H_1( char *rch , int max , int *rc , BOOL *HCODE );

//BOOL  Net_Connect_Status_2();
//void  Net_Wait_Connect_2();
//BOOL  Net_Write_Char_2( char );
//BOOL  Net_Read_Count_Char_2( char * , int , int * , int );
//int   Net_Connect_Count_2();
//int   Net_Read_Connect_Port_2();
//char* Net_Read_Connect_Host_2();
//int   Net_Read_Char_End_2_H_2( char *rch , int max , int *rc , BOOL *HCODE );
//BOOL  Net_Send_Receive_2( char *SendBuffer , int dl , int timeout , int *count );
//int   Net_Read_Char_End_2_2( char *rch , int max , int *rc );
//BOOL  Net_Read_String_End_2_2( char *Buffer , int MaxCount , int TimeOut , int *ReadCnt );