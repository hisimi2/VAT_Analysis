#include "stdafx.h"
#include "Ping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPing::CPing()
{
	m_pWTHD =NULL;
	m_wTimeOut =1000;
	m_wTTL =32;
	m_wRetry = 3;
	m_szIP ="";
//	m_wTime =-1;
	m_wTime = 0;
	m_wStatus = PING_READY;
	m_wRetCode =PING_NOERROR;

	m_wSize = 0;
}

CPing::~CPing()
{
	if( m_pWTHD != NULL ) {
//		::TerminateThread( m_pWTHD->m_hThread, -1);
		::TerminateThread( m_pWTHD->m_hThread, 0xffffff );
		delete m_pWTHD;
		m_pWTHD = NULL;
	}
}

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_TTL 11
#define ICMP_UNREACHED 3
#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header)

#define MAX_PACKET 1514
#define DEF_PACKET_SIZE 32
#define MAX_TTL 255

// ICMP header
typedef struct _iphdr {
	u_char	ip_hl:4;		/* header length */
	u_char	ip_v:4;			/* version */
	u_char	ip_tos;			/* type of service */
	short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	short	ip_off;			/* fragment offset field */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
} IPHeader;

// ICMP header
typedef struct _icmphdr {
	BYTE i_type;
	BYTE i_code; /* type sub code */
	USHORT i_cksum;
	USHORT i_id;
	USHORT i_seq;
	/* This is not the std header, but we reserve space for time */
	ULONG timestamp;
} IcmpHeader;

USHORT checksum(USHORT *buffer, int size) {

  unsigned long cksum=0;

  while(size >1) {
	cksum+=*buffer++;
	size -=sizeof(USHORT);
  }
  
  if(size ) {
	cksum += *(UCHAR*)buffer;
  }

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >>16);
  return (USHORT)(~cksum);
}



UINT PingThreadProc( LPVOID pParam )
{
    CPing* p = (CPing*)pParam;
    if (p == NULL ){ 
		return 1;
	}
	SOCKET sockRaw;
	u_long ttl;
	unsigned int  time, pingTime;
	struct sockaddr_in dest,from;
	int bread,datasize,bwrote;

	int fromlen = sizeof(from);
	int timeout = 1000;
	unsigned char icmp_data[MAX_PACKET];
	unsigned char recvbuf[MAX_PACKET];
	USHORT seq_no = 0;

	sockRaw = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
	if (sockRaw == INVALID_SOCKET) {
		p->m_wRetCode = PING_ERROR;
		p->m_wStatus = PING_DONE;
		return 1;
	}
	timeout = p->m_wTimeOut;
	if( setsockopt( sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout))  == SOCKET_ERROR) {
		closesocket( sockRaw );
		p->m_wRetCode = PING_ERROR;
		p->m_wStatus = PING_DONE;
		return 1;
	}
	if(setsockopt( sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) == SOCKET_ERROR) {
		closesocket( sockRaw );
		p->m_wRetCode = PING_ERROR;
		p->m_wStatus = PING_DONE;
		return 1;
	}
	memset(&dest,0,sizeof(dest));

  	dest.sin_addr.s_addr =  inet_addr(p->m_szIP);
	dest.sin_family = AF_INET;
	datasize = p->m_wSize;
	datasize += sizeof(IcmpHeader);  
	if( datasize >MAX_PACKET) {
		datasize = MAX_PACKET;
	}
	memset(icmp_data,0,MAX_PACKET);

	IcmpHeader *icmp_hdr;
	unsigned char *datapart;

	icmp_hdr = (IcmpHeader*)icmp_data;
	icmp_hdr->i_type = ICMP_ECHO;
	icmp_hdr->i_code = 0;
	datapart = icmp_data + sizeof(IcmpHeader);
	memset(datapart,'Y', datasize - sizeof(IcmpHeader));

	ttl = p->m_wTTL;
	if (setsockopt(sockRaw, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR){
		closesocket( sockRaw );
		p->m_wRetCode = PING_ERROR;
		p->m_wStatus = PING_DONE;
		return 1;
	}
	UINT wRetry = 0;
	BOOL bDone;
	bDone = FALSE;
	p->m_wRetCode = PING_TIMEOUT;
	while(!bDone  &&  wRetry < p->m_wRetry ){
		((IcmpHeader*)icmp_data)->i_cksum = 0;
		time = (double)GetTickCount64();
		((IcmpHeader*)icmp_data)->timestamp = time;
		icmp_hdr->i_id = (USHORT)GetCurrentThreadId();
		((IcmpHeader*)icmp_data)->i_seq = seq_no++;
		((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, 
												datasize);
		bwrote = sendto(sockRaw,(const char *)icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest));
		if (bwrote == SOCKET_ERROR){
			closesocket( sockRaw );
			p->m_wRetCode = PING_ERROR;
			p->m_wStatus = PING_DONE;
			return 1;
		}
		BOOL bGet;
		bGet = FALSE;
		do {
			CString strRouter;
			CString s;
			USHORT id = (USHORT)GetCurrentThreadId();
			bread = recvfrom(sockRaw,(char *)recvbuf,MAX_PACKET,0,(struct sockaddr*)&from,&fromlen);
			if (bread == SOCKET_ERROR){
				if (WSAGetLastError() == WSAETIMEDOUT) {
					wRetry++;
					break;
				} else {
					closesocket( sockRaw );
					p->m_wRetCode = PING_ERROR;
					p->m_wStatus = PING_DONE;
					return 1;
				}
			}
			pingTime = (double)GetTickCount64() - time;
			IPHeader *iphdr;
			IcmpHeader *icmphdr;
			unsigned short iphdrlen;
			iphdr = (IPHeader *)recvbuf;
			iphdrlen = (unsigned short)( iphdr->ip_hl * 4 ); // number of 32-bit words *4 = bytes
			icmphdr = (IcmpHeader*)(recvbuf + iphdrlen);
			switch (icmphdr->i_type) {
			case ICMP_TTL:
				strRouter = inet_ntoa(iphdr->ip_src);
				bGet = TRUE;
				bDone = TRUE;
				p->m_wRetCode = PING_TTLOVER;
				p->m_wTime = pingTime;
				p->m_szRouter = strRouter;
				break;
			case ICMP_ECHOREPLY:
				if( icmphdr->i_id == id  ) {
					bGet = TRUE;
					bDone = TRUE;
					p->m_wRetCode = PING_NOERROR;
					p->m_wTime = pingTime;
				}
				break;
			default:
				bGet = FALSE;
				break;
			}
		} while( !bGet &&  pingTime  < p->m_wTimeOut) ;
	}
	closesocket( sockRaw );
	p->m_wStatus = PING_DONE;
	return 0;

}

BOOL CPing::DoPing(CString szIP, unsigned int wTimeOut, unsigned int wRetry,unsigned int wSize, unsigned int wTTL)
{
	if( szIP.IsEmpty() ) return(FALSE);
	m_szIP = szIP;
	m_wTimeOut = wTimeOut;
	m_wRetry = wRetry;
	m_wTTL = wTTL;
	m_wSize = wSize;
	m_szRouter ="";
	m_pWTHD = AfxBeginThread(PingThreadProc, this,THREAD_PRIORITY_TIME_CRITICAL
		,0,CREATE_SUSPENDED,NULL);
	if( m_pWTHD == NULL ) return(FALSE);
	m_wStatus = PING_DOING;
	m_pWTHD->m_bAutoDelete =FALSE;
	m_pWTHD->ResumeThread();
	return(TRUE);
}

void CPing::ClearPing()
{
	if( m_pWTHD != NULL ) {
		::TerminateThread( m_pWTHD->m_hThread, 0xffffffff );
		delete m_pWTHD;
		m_pWTHD = NULL;
	}
	m_wTimeOut =1000;
	m_wTTL =32;
	m_wRetry = 3;
	m_szIP ="";
//	m_wTime =-1;
	m_wTime = 0;
	m_wSize = 64;
	m_wStatus = PING_READY;
	m_wRetCode =PING_NOERROR;
	m_szRouter ="";
}

BOOL InitPing(void)
{
	WSADATA	wsock;
	WORD wVersionRequested = MAKEWORD(2,2);
	int nResult = WSAStartup(wVersionRequested, &wsock);
	if ( nResult != 0 )
	{
		AfxMessageBox("WinSock Error!");
		return FALSE;
	}
	return(TRUE);
}

void ExitPing(void)
{
	WSACleanup();
}