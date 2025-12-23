#include "StdAfx.h"
#include "AIO_SIMUL_DRV.h"


CAIO_SIMUL_DRV::CAIO_SIMUL_DRV(void)
{
	m_pDlgSimul = NULL;
}


CAIO_SIMUL_DRV::~CAIO_SIMUL_DRV(void)
{
	if( m_pDlgSimul ){
		m_pDlgSimul->DestroyWindow();
		delete m_pDlgSimul;
		m_pDlgSimul = NULL;
	}
}


int CAIO_SIMUL_DRV::AIO_INIT(const char* szIniFileName)
{
	CDatabaseIF* pDb = g_SingleDB.GetObj();
	pDb->SELECT_AIO_TBL(_tAIO_LS::eAI, m_AiDbInfoList);
	pDb->SELECT_AIO_TBL(_tAIO_LS::eAO, m_AoDbInfoList);

	if( m_AiDbInfoList.m_fnGetCount() > 0 ){
		m_vAiSimData.resize( m_AiDbInfoList.m_fnGetCount() );
	}
	if( m_AoDbInfoList.m_fnGetCount() > 0 ){
		m_vAoSimData.resize( m_AoDbInfoList.m_fnGetCount() );
	}

	m_pDlgSimul = new CDlgAioSimul;
	m_pDlgSimul->Create(IDD_AIO_SIMUL);
	int x,y,cx,cy;
	CRect r;
	m_pDlgSimul->GetWindowRect(&r);
	x = r.left;
	y = r.top;
	cx = r.Width();
	cy = r.Height();
	m_pDlgSimul->SetWindowPos(&CWnd::wndTopMost, x, y, cx, cy, 0);
	m_pDlgSimul->ShowWindow(SW_HIDE);

	return 0;
}

int CAIO_SIMUL_DRV::SetSimDataAi(int nIndex, double dValue)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAiSimData.size() );
	m_vAiSimData[nIndex] = dValue;
	return 0;
}

double CAIO_SIMUL_DRV::GetSimDataAi(int nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAiSimData.size() );

	return m_vAiSimData[nIndex];
}

int CAIO_SIMUL_DRV::SetSimDataAo(int nIndex, double dValue)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAoSimData.size() );
	m_vAoSimData[nIndex] = dValue;
	return 0;
}

double CAIO_SIMUL_DRV::GetSimDataAo(int nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < (int)m_vAoSimData.size() );

	return m_vAoSimData[nIndex];
}

int CAIO_SIMUL_DRV::GetPointMax(int nInOrOut)
{
	int nPointCount = 0;
	if( nInOrOut == _tAIO_LS::eAI ){
		nPointCount = m_AiDbInfoList.m_fnGetCount();
	}else{
		nPointCount = m_AoDbInfoList.m_fnGetCount();
	}
	return nPointCount;
}

void CAIO_SIMUL_DRV::ShowDialogSimul()
{
	if( m_pDlgSimul ){
		m_pDlgSimul->ShowWindow(SW_SHOW);
	}
}