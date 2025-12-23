#include "StdAfx.h"
#include "AIO_Cont_Imp.h"
#include "ConfigData.h"
#include "DlgDioAll.h"

int g_nAIO_Type = _AIO_TYPE_SIMUL_;

CAIO_Cont_Imp::CAIO_Cont_Imp(void)
{
	m_pAI = NULL;
	m_pAO = NULL;
	m_bBoardInit = FALSE;
	memset(m_szInitFileName, 0x00, sizeof(m_szInitFileName));
	m_nAI_PointMax = 0;
	m_nAO_PointMax = 0;
	m_pDlgIoAll = NULL;
}


CAIO_Cont_Imp::~CAIO_Cont_Imp(void)
{
	Finalize();

	if( m_pAI ){
		delete m_pAI;
		m_pAI = NULL;
	}
	if( m_pAO ){
		delete m_pAO;
		m_pAO = NULL;
	}
	if( g_pAioSimDrv ){
		delete g_pAioSimDrv;
		g_pAioSimDrv = NULL;
	}
	if( m_pDlgIoAll ){
		m_pDlgIoAll->DestroyWindow();
		delete m_pDlgIoAll;
		m_pDlgIoAll = NULL;
	}
}


int CAIO_Cont_Imp::Initialize(const char* szDriverInitFileName, int nFileNameLen)
{
	g_SingleDB.CreateDB(szDriverInitFileName, nFileNameLen);

	if( m_pAI != NULL ){
		delete m_pAI;
		m_pAI = NULL;
	}
	if( m_pAO != NULL ){
		delete m_pAO;
		m_pAO = NULL;
	}

	strncpy_s(m_szInitFileName, szDriverInitFileName, nFileNameLen);

	// TODO STRIP : ini 파일에서 읽어서 처리
	CConfigData ConfigData(m_szInitFileName);
	char szTemp[128]={0,};

	g_nAIO_Type = ConfigData.GetInt("AIO CONFIG", "Driver Type", "0");

	g_pDbIf->SELECT_AIO_TBL(_tAIO_LS::eAI, m_AiDbInfoList);
	g_pDbIf->SELECT_AIO_TBL(_tAIO_LS::eAO, m_AoDbInfoList);

	if( _AIO_TYPE_SYCON_ == g_nAIO_Type ){
		if( g_SyconDrv.IsConnected() != TRUE ){			
			g_SyconDrv.InitialDriver(m_szInitFileName, strlen(m_szInitFileName));
		}
		m_pAI = new CAI_SYCON;
		m_pAO = new CAO_SYCON;
		m_nAI_PointMax = g_SyconDrv.GetMaxAioPointCount(0);
		m_nAO_PointMax = g_SyconDrv.GetMaxAioPointCount(1);
	}else{
		g_pAioSimDrv = new CAIO_SIMUL_DRV;
		g_pAioSimDrv->AIO_INIT(m_szInitFileName);
		m_nAI_PointMax = g_pAioSimDrv->GetPointMax(_tAIO_LS::eAI);
		m_nAO_PointMax = g_pAioSimDrv->GetPointMax(_tAIO_LS::eAO);
		m_pAI = new CAI_SIMUL;
		m_pAO = new CAO_SIMUL;
	}

	m_vAI.resize(m_nAI_PointMax);
	m_vAO.resize(m_nAO_PointMax);

	UINT i;
	for(i=0; i<m_vAI.size(); i++){
		m_vAI[i] = 0.0;
	}
	for(i=0; i<m_vAO.size(); i++){
		m_vAO[i] = 0.0;
	}

	g_IOContImp.Initialize(szDriverInitFileName, strlen(szDriverInitFileName));

	m_bBoardInit = TRUE;

	return 0;
}

void CAIO_Cont_Imp::Finalize()
{
	g_IOContImp.Finalize();
}


void CAIO_Cont_Imp::out(int nIndex, double dValue)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAI.size() );

	m_vAO[nIndex] = dValue;
}

double CAIO_Cont_Imp::out(int nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAO.size() );

	return m_vAO[nIndex];
}

double CAIO_Cont_Imp::in(int nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAI.size() );

	return m_vAI[nIndex];
}

int CAIO_Cont_Imp::GetAiCount()
{
	return m_nAI_PointMax;
}

int CAIO_Cont_Imp::GetAoCount()
{
	return m_nAO_PointMax;
}

void CAIO_Cont_Imp::ShowDialog_IO_All()
{
// 	CDlgDioAll dlg(1, NULL);
// 	dlg.DoModal();
	if( m_pDlgIoAll ){
		m_pDlgIoAll->DestroyWindow();
		delete m_pDlgIoAll;
		m_pDlgIoAll = NULL;
	}
	m_pDlgIoAll = new CDlgDioAll(1, NULL);
	m_pDlgIoAll->Create(IDD_DIO_ALL, NULL);
	
	CRect r;
	m_pDlgIoAll->GetWindowRect(&r);
	int cx = r.Width();
	int cy = r.Height();

	m_pDlgIoAll->SetWindowPos(&CWnd::wndNoTopMost, 5, 115, cx, cy,  SWP_SHOWWINDOW);
	m_pDlgIoAll->ShowWindow(SW_SHOW);
}

void CAIO_Cont_Imp::ShowDialogSimul()
{
	if( g_pAioSimDrv ){
		g_pAioSimDrv->ShowDialogSimul();
	}
}


void CAIO_Cont_Imp::Read_AI_All()
{
	m_pAI->ReadAll(m_vAI);
}

void CAIO_Cont_Imp::Write_AO_All()
{
	m_pAO->WriteAll(m_vAO);
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : nIoType : 0 = analog input, 1 = analog output
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont_Imp::GetIoDbInfo(int nIoType, int nIoIndex, _tAIO_LS* lpstIoInfo)
{
	_tAIO_LS* pDataSrc;
	if( nIoType == _tAIO_LS::eAI){
		pDataSrc = (_tAIO_LS*)m_AiDbInfoList.m_fnGetData(nIoIndex);
	}else{
		pDataSrc = (_tAIO_LS*)m_AoDbInfoList.m_fnGetData(nIoIndex);
	}
	*lpstIoInfo = *pDataSrc;
	return 0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont_Imp::GetPhysicalValue(int nIoType, int nIndex, double* pdValue, CString& strUnit)
{
	_tAIO_LS stIoInfo;
	GetIoDbInfo(nIoType, nIndex, &stIoInfo);
	
	double dCurVolt = 0.0;
	if( nIoType == _tAIO_LS::eAI ) dCurVolt = m_vAI[nIndex];
	else                           dCurVolt = m_vAO[nIndex];

	if( dCurVolt < stIoInfo.dMinVolt ){
		dCurVolt = stIoInfo.dMinVolt;
	}
	if( dCurVolt > stIoInfo.dMaxVolt ){
		dCurVolt = stIoInfo.dMaxVolt;
	}

	double dRate = (dCurVolt-stIoInfo.dMinVolt) / (stIoInfo.dMaxVolt - stIoInfo.dMinVolt);
	double dPhyValue = stIoInfo.dMinPhyVal + ((stIoInfo.dMaxPhyVal - stIoInfo.dMinPhyVal)*dRate);
	
	strUnit = stIoInfo.strUnit;
	*pdValue = dPhyValue;

	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-18]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CAIO_Cont_Imp::SetPhysicalValue(int nIndex, double dPhyValue)
{
	_tAIO_LS stIoInfo;
	GetIoDbInfo(_tAIO_LS::eAO, nIndex, &stIoInfo);
	double dRate = (dPhyValue-stIoInfo.dMinPhyVal) / (stIoInfo.dMaxPhyVal - stIoInfo.dMinPhyVal);
	double dOutVolt = stIoInfo.dMinVolt + ((stIoInfo.dMaxVolt - stIoInfo.dMinVolt)*dRate);

	m_vAO[nIndex] = dOutVolt;
	
	return 0;
}
