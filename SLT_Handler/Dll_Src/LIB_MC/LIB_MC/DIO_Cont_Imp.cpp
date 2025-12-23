#include "StdAfx.h"
#include "DIO_Cont_Imp.h"


#include "DIO_Techwing.h"
#include "DIO_CONTEC.h"
#include "DIO_SYCON.h"
#include "DIO_SIMUL.h"
#include "ConfigData.h"
#include "DEF_DB_TBL.h"
#include "DIO_SoftServo.h"
using namespace std;



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDIO_Cont_Imp::CDIO_Cont_Imp(void)
{
	ZeroMemory(m_szInitFileName, sizeof(m_szInitFileName));

	m_nDO_PointMax	= 0;
	m_nDI_PointMax	= 0;
	m_nDI_BaseAddr	= 0;
	m_nDO_BaseAddr	= 0;
	m_bBoardInit	= FALSE;
	m_pDI			= NULL;
	m_pDO			= NULL;
	m_pDlgIoAll = NULL;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDIO_Cont_Imp::~CDIO_Cont_Imp(void)
{
	RESET_DO_DRV_CALL();

	Finalize();

	if(NULL != m_pDI){
		delete m_pDI;
		m_pDI = NULL;
	}

	if(NULL != m_pDO){
		delete m_pDO;
		m_pDO = NULL;
	}
	if( NULL != g_pContecDrv ){
		delete g_pContecDrv;
		g_pContecDrv = NULL;
	}
	if( NULL != g_pDioSimDrv ){
		delete g_pDioSimDrv;
		g_pDioSimDrv = NULL;
	}

	if( m_pDlgIoAll ){
		m_pDlgIoAll->DestroyWindow();
		delete m_pDlgIoAll;
		m_pDlgIoAll = NULL;
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CDIO_Cont_Imp::Initialize(const char* szDriverIniFileName, int nFileNameLen)
{
	g_SingleDB.CreateDB(szDriverIniFileName, nFileNameLen);

	if( m_pDI != NULL ){
		delete m_pDI;
		m_pDI = NULL;
	}
	if( m_pDO != NULL ){
		delete m_pDO;
		m_pDO = NULL;
	}

	// TODO STRIP : INI 파일이름 전달 받기

	strncpy_s(m_szInitFileName, szDriverIniFileName, nFileNameLen);

	// TODO STRIP : ini 파일에서 읽어서 처리
	CConfigData ConfigData(m_szInitFileName);
	char szTemp[512]={0,};

	g_nDIO_Type = ConfigData.GetInt("DIO CONFIG", "Driver Type", "0");

	g_pDbIf->SELECT_DIO_TBL(_tDIO_LS::eDI, m_ListDbInfoDi);
	g_pDbIf->SELECT_DIO_TBL(_tDIO_LS::eDO, m_ListDbInfoDo);

	// TODO : READ INI from TW_LS_DRIVER.INI		
	if( _DIO_TYPE_TECHWING_ == g_nDIO_Type )
	{
// 		OSVERSIONINFO osvi;
// 		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
// 		GetVersionEx(&osvi);
// 		//OS Type를 확인하여 giveio라이브러리를 등록 한다.
// 		if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
// 		{
// 			HANDLE h;
// 			h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
// 				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 			if(h == INVALID_HANDLE_VALUE)
// 			{
// 				AfxMessageBox("Couldn't access giveio device",MB_TOPMOST);
// 
// 			}
// 			CloseHandle(h);
// 		}
// 
// 		int nDI_BoardCount = 0;
// 		int nDO_BoardCount = 0;
// 		int nEsc_addr = 0;
// 		memset(szTemp, 0x00, sizeof(szTemp));
// 		ConfigData.GetString("DIO CONFIG TECHWING", "Base Address DI", "0x7000", szTemp);
// 		sscanf_s(szTemp, "%x", &m_nDI_BaseAddr);
// 
// 		memset(szTemp, 0x00, sizeof(szTemp));
// 		ConfigData.GetString("DIO CONFIG TECHWING", "Base Address DO", "0x7000", szTemp);
// 		sscanf_s(szTemp, "%x", &m_nDO_BaseAddr);
// 
// 		memset(szTemp, 0x00, sizeof(szTemp));
// 		ConfigData.GetString("DIO CONFIG TECHWING", "ESC Address", "0xF", szTemp);
// 		sscanf_s(szTemp, "%x", &nEsc_addr);
// 
// 		nDI_BoardCount = ConfigData.GetInt("DIO CONFIG TECHWING", "DI Board Count", "3");
// 		nDO_BoardCount = ConfigData.GetInt("DIO CONFIG TECHWING", "DO Board Count", "3");
// 
// 		m_pDI  = new CDI_Techwing(m_nDI_BaseAddr, nDI_BoardCount);
// 		m_pDO  = new CDO_Techwing(m_nDO_BaseAddr, nDO_BoardCount);
// 		m_nDI_PointMax = (TW_DIO_MAX_POINT_ONE_BD * nDI_BoardCount) + nEsc_addr;
// 		m_nDO_PointMax = (TW_DIO_MAX_POINT_ONE_BD * nDO_BoardCount) + nEsc_addr;
		int nErr = LoadIODLL();
		if( nErr != 0 )
		{
			AfxMessageBox( _T( "Couldn't access giveio device" ), MB_TOPMOST );
			return FALSE;
		}

		int nDI_BoardCount = 0;
		int nDO_BoardCount = 0;
		int nEsc_addr = 0;
		memset( szTemp, 0x00, sizeof( szTemp ) );
		ConfigData.GetString( _T( "DIO CONFIG TECHWING" ), _T( "Base Address DI" ), _T( "0x7000" ), szTemp );
		m_nDI_BaseAddr = std::stoul( szTemp, nullptr, 16 );

		memset( szTemp, 0x00, sizeof( szTemp ) );
		ConfigData.GetString( _T( "DIO CONFIG TECHWING" ), _T( "Base Address DO" ), _T( "0x7000" ), szTemp);
		m_nDO_BaseAddr = std::stoul( szTemp, nullptr, 16 );

		memset( szTemp, 0x00, sizeof( szTemp ) );
		ConfigData.GetString( _T( "DIO CONFIG TECHWING" ), _T( "ESC Address" ), _T( "0xF" ), szTemp );
		nEsc_addr = std::stoul( szTemp, nullptr, 16 );

		nDI_BoardCount = ConfigData.GetInt( _T( "DIO CONFIG TECHWING" ), _T( "DI Board Count" ), _T( "3" ) );
		nDO_BoardCount = ConfigData.GetInt( _T( "DIO CONFIG TECHWING" ), _T( "DO Board Count" ), _T( "3" ) );

		m_pDI = new CDI_Techwing( m_nDI_BaseAddr, nDI_BoardCount );
		m_pDO = new CDO_Techwing( m_nDO_BaseAddr, nDO_BoardCount );
		m_nDI_PointMax = ( TW_DIO_MAX_POINT_ONE_BD * nDI_BoardCount );
		m_nDO_PointMax = ( TW_DIO_MAX_POINT_ONE_BD * nDO_BoardCount );
	}
	else if( _DIO_TYPE_CONTEC_ == g_nDIO_Type ) 
	{
		int nDiBoardCount = ConfigData.GetInt("DIO CONFIG CONTEC", "DI Board Count", 0);
		int nDoBoardCount = ConfigData.GetInt("DIO CONFIG CONTEC", "DO Board Count", 0);
		m_nDI_PointMax = (CONTEC_DIO_MAX_POINT_ONE_MODULE * nDiBoardCount);
		m_nDO_PointMax = (CONTEC_DIO_MAX_POINT_ONE_MODULE * nDoBoardCount);

		g_pContecDrv = new CDIO_CONTEC_DRV;
		g_pContecDrv->Initialize(m_szInitFileName, strlen(m_szInitFileName));
		m_pDI  = new CDI_CONTEC(nDiBoardCount);
		m_pDO  = new CDO_CONTEC(nDoBoardCount);
	}
	else if( _DIO_TYPE_SYCON_ == g_nDIO_Type )
	{
		if( g_SyconDrv.IsConnected() != TRUE ){
			int nRetDrvInit = g_SyconDrv.InitialDriver(m_szInitFileName, strlen(m_szInitFileName));
			if( nRetDrvInit !=  1 ){
				AfxMessageBox("Sycon Board Init fail!!!");
			}
		}

		m_nDI_PointMax = g_SyconDrv.GetMaxDioPointCount(0);
		m_nDO_PointMax = g_SyconDrv.GetMaxDioPointCount(1);

		int nDiPortCount = m_nDI_PointMax / SYCON_DIO_MAX_POINT_ONE_PORT;
		int nDoPortCount = m_nDO_PointMax / SYCON_DIO_MAX_POINT_ONE_PORT;

		m_pDI = new CDI_SYCON(nDiPortCount);
		m_pDO = new CDO_SYCON(nDoPortCount);
	}
	else if (_DIO_TYPE_SOFTSERVO_ == g_nDIO_Type)
	{
		int nBaseAddress = 0;
		int nDIBoardCnt = 0;
		int nDOBoardCnt = 0;
		int nDebugOption = 0;


		if (g_pWmx3Lib_Io == nullptr)
 			g_pWmx3Lib_Io = new Io(&g_Wmx3Lib);
		//
		// 설정 정보를 읽어들인다.
		// 기존 DI/DO address로 설정이 나뉘어져 있던것과는 달리,
		// 여기에서는 하나의 통합된 address를 사용한다.
		//

		memset(szTemp, 0x00, sizeof(szTemp));
		ConfigData.GetString(_T("DIO CONFIG SOFTSERVO"), _T("Base Address"), _T("0x7000"), szTemp);
		nBaseAddress = std::stoul(szTemp, nullptr, 16);

		nDIBoardCnt = ConfigData.GetInt(_T("DIO CONFIG SOFTSERVO"), _T("DI Board Count"), _T("3"));
		nDebugOption = ConfigData.GetInt(_T("DIO CONFIG SOFTSERVO"), _T("Debug"), _T("0"));
		nDOBoardCnt = ConfigData.GetInt(_T("DIO CONFIG SOFTSERVO"), _T("DO Board Count"), _T("3"));

		//
		// Address는 16-bit로, 0x0000 - 0xffff 까지의 범위만 유효
		//

		if ((ULONG)nBaseAddress >= 0x10000)
		{
			AfxMessageBox(_T("Invalid base address"), MB_TOPMOST);
			return FALSE;
		}

		//
		// Board count가 0 이하가 될 수는 없다
		//

		if (nDIBoardCnt <= 0 || nDOBoardCnt <= 0)
		{
			AfxMessageBox(_T("Invalid board count"), MB_TOPMOST);
			return FALSE;
		}

		ASSERT(TW_DIO_MAX_POINT_ONE_BD > 0);

		m_pDI = new CDI_SoftServo(nBaseAddress, nDIBoardCnt);
		m_pDO = new CDO_SoftServo(nBaseAddress, nDOBoardCnt);
// 		m_nDI_PointMax = (TW_DIO_MAX_POINT_ONE_BD  * nDIBoardCnt);
// 		m_nDO_PointMax = (TW_DIO_MAX_POINT_ONE_BD  * nDOBoardCnt);
		m_nDI_PointMax = (TW_DIO_MAX_POINT_ONE_BD / 4) * nDIBoardCnt;
		m_nDO_PointMax = (TW_DIO_MAX_POINT_ONE_BD / 4 )* nDOBoardCnt;
	}
	else
	{
		m_nDI_PointMax = m_ListDbInfoDi.m_fnGetCount();
		m_nDO_PointMax = m_ListDbInfoDo.m_fnGetCount();
		g_pDioSimDrv = new CDIO_SIMUL_DRV;
		g_pDioSimDrv->DIO_INIT(m_szInitFileName, m_nDI_PointMax, m_nDO_PointMax);
		m_pDI = new CDI_SIMUL(m_nDI_PointMax);
		m_pDO = new CDO_SIMUL(m_nDO_PointMax);
	}

	_in.resize(m_nDI_PointMax);
	_out.resize(m_nDO_PointMax);

	UINT i;
	for(i=0; i<_in.size(); i++){
		_in[i] = 0;
	}
	for(i=0; i<_out.size(); i++){
		_out[i] = 0;
	}

	RESET_DO_DRV_CALL();

	g_IOContImp.Initialize(szDriverIniFileName, strlen(szDriverIniFileName));

	m_bBoardInit = TRUE;

	return TRUE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDIO_Cont_Imp::Finalize()
{
	g_IOContImp.Finalize();
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDIO_Cont_Imp::RESET_DO_DRV_CALL()
{
	UINT i;
	for(i=0; i<_in.size(); i++){
		_in[i] = 0;
	}
	for(i=0; i<_out.size(); i++){
		_out[i] = 0;
	}
	
	if(m_nDO_BaseAddr){
		m_pDO->Reset();
	}

	Sleep(1);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDIO_Cont_Imp::Read_DI_All()
{
	if( m_pDI == NULL ){
		return;
	}
	int i=0;
	m_pDI->Read_DI();
	for(i=0; i<m_pDI->m_nDI_PointCount; i++) {
		_in[i] = m_pDI->m_pDI_PointBuffer[i];
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDIO_Cont_Imp::Write_DO_All()
{
	if( m_pDO == NULL ){
		return;
	}
	int i=0;	
	for(i=0; i<m_pDO->m_nDO_PointCount; i++){
		m_pDO->m_pDO_PointBuffer[i] = _out[i];
	}
	m_pDO->Write_DO();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CDIO_Cont_Imp::GET_DI_DRV_CALL(int In_addr)
{
	if(m_pDI->Read_DI_Bit(In_addr))
	{
		return TRUE;
	}
	
	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-01-21]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDIO_Cont_Imp::SET_DO_DRV_CALL(int Out_addr, BOOL data)
{
	m_pDO->Write_DO_Bit(Out_addr, data);	
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
char CDIO_Cont_Imp::GET_DI(int nIndex)
{
	if( nIndex < 0 || nIndex >= m_nDI_PointMax ) return 0;
	return _in[nIndex];
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDIO_Cont_Imp::SET_DO(int nIndex, char cValue)
{
	if( nIndex < 0 || nIndex >= m_nDO_PointMax ) return;
	if( g_pDioSimDrv ){
		g_pDioSimDrv->SendSimulMsg_DoChanged(nIndex, cValue);
	}
	_out[nIndex] = cValue;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
char CDIO_Cont_Imp::GET_DO(int nIndex)
{
	if( nIndex < 0 || nIndex >= m_nDO_PointMax ) return 0;
	return _out[nIndex];
}

void CDIO_Cont_Imp::ShowDialog_IO_All()
{
	if( m_pDlgIoAll ){
		m_pDlgIoAll->DestroyWindow();
		delete m_pDlgIoAll;
		m_pDlgIoAll = NULL;
	}
	m_pDlgIoAll = new CDlgDioAll(0, NULL);
	m_pDlgIoAll->Create(IDD_DIO_ALL, NULL);

	CRect r;
	m_pDlgIoAll->GetWindowRect(&r);
	int cx = r.Width();
	int cy = r.Height();

	m_pDlgIoAll->SetWindowPos(&CWnd::wndNoTopMost, 5, 115, cx, cy,  SWP_SHOWWINDOW);
	m_pDlgIoAll->ShowWindow(SW_SHOW);
}

void CDIO_Cont_Imp::ShowDialogSimul()
{
	if( g_pDioSimDrv ){
		g_pDioSimDrv->ShowDialogSimul();
	}
}

int CDIO_Cont_Imp::GetIoInfo(int nInOrOut, int nIndex, _tDIO_LS& IoInfo)
{
	_tDIO_LS* pIoInfoSrc = NULL;
	if( nInOrOut == _tDIO_LS::eDI ){
		pIoInfoSrc = (_tDIO_LS*)m_ListDbInfoDi.m_fnGetData(nIndex);
	}else{
		pIoInfoSrc = (_tDIO_LS*)m_ListDbInfoDo.m_fnGetData(nIndex);
	}

	if( pIoInfoSrc != NULL ){
		IoInfo = *pIoInfoSrc;
	}

	return 0;
}

void CDIO_Cont_Imp::SetSimulDi(int nIndex, BOOL bValue)
{
	if( g_pDioSimDrv ){
		g_pDioSimDrv->SetSimDataDi(nIndex, bValue);
	}
}

void CDIO_Cont_Imp::SetSimulWnd(CWnd* pSimWnd)
{
	if( g_pDioSimDrv ){
		g_pDioSimDrv->SetSimulWnd( pSimWnd->GetSafeHwnd() );
	}
}
