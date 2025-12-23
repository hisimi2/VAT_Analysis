#pragma once


class CCylinder
{
public:
	CCylinder(void);
	~CCylinder(void);

	void SetCyl_IO_Initialize(int nSen1, int nSen2, int nSol1, int nSol2)
	{
		if(nSen1 != -1){
			m_vInID.push_back(nSen1);
		}

		if(nSen2 != -1){
			m_vInID.push_back(nSen2);
		}

		if(nSol1 != -1){
			m_vOutID.push_back(nSol1);
		}

		if(nSol2 != -1){
			m_vOutID.push_back(nSol2);
		}
	};

	virtual void Output(int nOut)
	{

	};
	virtual int  ChkSen(int nOut)	
	{
		return 0;
	};

	virtual int  IsActComp(int nOut)
	{
		return 0;
	};

	int m_nDefaultDelayTime; // [msec]
	std::vector<int> m_vInID;
	std::vector<int> m_vOutID;
};

//=============================================================================

class CBaseCylAct
{
public:
	CBaseCylAct()
	{
		m_nStep = 0;
		m_bStop = FALSE;
	};
	~CBaseCylAct()
	{

	};

	void SetCylinderObj(std::vector<CCylinder*> vCylinders);

	void OnStop()
	{
		m_bStop = TRUE;
	};

	virtual int RunAct(); // return : 0=ing, 1=normal comp, À½¼ö:alarm comp

	int                         m_nStep;
	BOOL                        m_bStop;
	std::vector<CCylinder*> m_vCylinders;
};

class CCylAct_OneActNoSen : public CBaseCylAct
{
public:
	CCylAct_OneActNoSen(){};
	~CCylAct_OneActNoSen(){};

	int RunAct()
	{
		for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
			CCylinder* pCyl = m_vCylinders[i];
			pCyl->Output( 1 );
		}
		return 1;
	};

};

class CCylAct_OneActChkSen : public CBaseCylAct
{
public:
	CCylAct_OneActChkSen(){};
	~CCylAct_OneActChkSen(){};

	int RunAct()
	{
		for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
			CCylinder* pCyl = m_vCylinders[i];
			pCyl->Output( 1 );
		}

		Sleep( 50 );

		for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
			CCylinder* pCyl = m_vCylinders[i];
			int nSen = pCyl->ChkSen( 1 );
			if( nSen != 1 ){
				return -1;
			}
		}
		return 1;
	};
};


class CCylAct_ContActNoSen : public CBaseCylAct
{
public:
	CCylAct_ContActNoSen(){
		m_dUserDelayTime =0;
	};
	~CCylAct_ContActNoSen(){};

	double      m_dUserDelayTime; // msec

	BOOL RunAct()
	{
		if( m_bStop ){
			return 1;
		}

		switch( m_nStep )
		{
		case 0:
			{
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					pCyl->Output( 1 );
 				}
				Sleep( (int)m_dUserDelayTime );
				m_nStep++;
			}break;
		case 1:
			{
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					pCyl->Output( 0 );
				}
				Sleep( (int)m_dUserDelayTime );
				m_nStep = 0;
			}break;
		}
		return 0;
	}
};

class CCylAct_ContActChkSen : public CBaseCylAct
{
public:
	CCylAct_ContActChkSen(){};
	~CCylAct_ContActChkSen(){};

	BOOL RunAct()
	{
		if( m_bStop ){
			return 1;
		}

		switch( m_nStep )
		{
		case 0:
			{
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					pCyl->Output( 1 );
				}
				
				Sleep( 50 );

				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					int nSen = pCyl->ChkSen( 1 );
					if( nSen != 1 ){
						return -1;
					}
				}
				m_nStep++;
			}break;
		case 1:
			{
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					pCyl->Output( 0 );
				}
				Sleep( 50 );
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					int nSen = pCyl->ChkSen( 0 );
					if( nSen != 0 ){
						return -1;
					}
				}
				m_nStep = 0;
			}break;
		}

		return 0;
	}
};

class CCylAct_OneActChkTimeChkSen : public CBaseCylAct
{
public:
	CCylAct_OneActChkTimeChkSen()
	{
		m_dUserDelayTime = 0.0;
		m_dActionTime    = 0.0;
	};
	~CCylAct_OneActChkTimeChkSen(){};

	double      m_dUserDelayTime; // msec
	CTimerCheck m_tcCylActTime;
	double      m_dActionTime;

	BOOL RunAct()
	{
		if( m_bStop ){
			return 1;
		}

		switch( m_nStep )
		{
		case 0:
			{
				m_tcCylActTime.SetCheckTime(m_dUserDelayTime);
				m_tcCylActTime.StartTimer();
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					pCyl->Output( 1 );
				}
				Sleep( 30 );
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					int nSen = pCyl->ChkSen( 1 );
					if( nSen != 1 ){
						return -1;
					}
				}
				m_nStep++;
			}break;
		case 1:
			{
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					pCyl->Output( 0 );
				}
				Sleep( 30 );
				m_nStep++;
			}break;
		case 2:
			{
				int nChkOK = 0;
				for( std::vector<CCylinder*>::size_type i=0; i<m_vCylinders.size(); i++){
					CCylinder* pCyl = m_vCylinders[i];
					int nSen = pCyl->ChkSen( 0 );
					if( nSen == 0 ){
						nChkOK++;
					}
				}
				if( nChkOK == m_vCylinders.size() ){
					m_dActionTime = m_tcCylActTime.CompareTime();
					return 1;
				}else{
					return -1;
				}
			}break;
		}
		return 0;
	}
};



/*==================================================================================

	int nRet = 0;

	CBaseCylAct* pCylAct = new CCylAct_OneActChkSen;
	std::vector<CCylinder*> vCylinders;
	vCylinders.push_back( &g_cylLDStacker1 );
	pCylAct->SetCylinderObj( vCylinders );

	while(1)
	{
		nRet = cylact.RunAct();
		if( nRet != 0 ){
			break;
		}
		Sleep(1);
	}

	if( nRet < 0 ){
		MessageBox("alarm");
	}

	delete pCylAct;
	pCylAct = NULL;
==================================================================================*/
