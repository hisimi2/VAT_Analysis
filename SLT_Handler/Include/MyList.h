// MyList.h: interface for the CMyList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLIST_H__7B5379EF_7BB8_4988_82B1_F6288156E5AD__INCLUDED_)
#define AFX_MYLIST_H__7B5379EF_7BB8_4988_82B1_F6288156E5AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//==============================================================================
//
//==============================================================================
class AFX_EXT_CLASS CMyList  :  public CObList
{
public:
	CMyList() 
	{
		m_nCurrentPos = 0;
		
		m_pSync = NULL;

		m_pSync = new CRITICAL_SECTION;
		try
		{
			::InitializeCriticalSection( m_pSync );
		}
		catch(...)
		{
			AfxMessageBox("CMyList Initialize Error !!!");
		}
	}
	~CMyList() 
	{
		m_fnClear();

		if( m_pSync != NULL ){
			::DeleteCriticalSection( m_pSync );
			delete m_pSync; 
			m_pSync = NULL;
		}
	}

	int	m_fnGetCount()
	{
		int nCnt = 0;
		::EnterCriticalSection( m_pSync );
		nCnt = GetCount();
		::LeaveCriticalSection( m_pSync );
		return 	nCnt;
	}

	CObject* m_fnGetData(int nIndex)
	{
		CObject* pData = NULL;

		::EnterCriticalSection( m_pSync );

		POSITION ps = FindIndex(nIndex);
		
		if(ps != NULL)	{
			pData = GetAt(ps);
		}

		::LeaveCriticalSection( m_pSync );

		return pData;
	}

	CObject* m_fnNextData()
	{
		::EnterCriticalSection( m_pSync );
		CObject* pData = NULL;

		m_nCurrentPos++;
		
		POSITION ps = FindIndex(m_nCurrentPos);
		
		if(ps != NULL)	{
			pData = GetAt(ps);
		}	
		::LeaveCriticalSection( m_pSync );
		return pData;
	}

	CObject* m_fnFirstData()
	{
		::EnterCriticalSection( m_pSync );
		CObject* pData = NULL;
		m_nCurrentPos = 0;

		POSITION ps = FindIndex(m_nCurrentPos);

		if(ps != NULL)	{
			pData = GetAt(ps);
		}
		::LeaveCriticalSection( m_pSync );
		return pData;
	}

	CObject* m_fnLastData()
	{
		::EnterCriticalSection( m_pSync );
		CObject* pData = NULL;
		if(m_fnGetCount())
		{
			m_nCurrentPos = m_fnGetCount() - 1;

			POSITION ps = FindIndex(m_nCurrentPos);

			if(ps != NULL)	{
				pData = GetAt(ps);
			}
		}
		::LeaveCriticalSection( m_pSync );
		return pData;
	}

	int	m_fnAddData(CObject* pdata)
	{
		::EnterCriticalSection( m_pSync );
		AddTail((CObject*)pdata);
		::LeaveCriticalSection( m_pSync );
		return 0;
	}

	void m_fnDeleteData()
	{
		::EnterCriticalSection(m_pSync);
		POSITION ps = FindIndex(m_nCurrentPos);

		if(ps != NULL)
		{
			CObject* pfinddata = GetAt(ps);

			if(pfinddata != NULL) 
			{
				delete pfinddata;
				RemoveAt(ps);
			}
		}
		::LeaveCriticalSection(m_pSync);
	}

	void m_fnDeleteData(CObject* pdata)
	{
		::EnterCriticalSection(m_pSync);

		BOOL		bFind = FALSE;
		CObject*	pfinddata = NULL;
		POSITION	ps = NULL;

		for(int i=0; i<m_fnGetCount(); i++)
		{
			ps = FindIndex(i);

			if(ps != NULL)
			{
				pfinddata = GetAt(ps);

				if(pfinddata == pdata) { bFind = TRUE; break; }
			}
		}

		if(bFind)
		{
			delete pfinddata;
			RemoveAt(ps);
		}

		::LeaveCriticalSection(m_pSync);
	}

	// By jhjang [141124]: Modify Daily Jam Rate Function
	// m_fnDeleteData 함수 재정의
	void m_fnDeleteData(int nIndex)
	{
		::EnterCriticalSection(m_pSync);

		CObject*	pfinddata = NULL;
		POSITION	ps = NULL;

		ps = FindIndex(nIndex);

		if(ps != NULL){
			pfinddata = GetAt(ps);

			delete pfinddata;
			RemoveAt(ps);
		}

		::LeaveCriticalSection(m_pSync);
	}

	void m_fnClear()
	{
		::EnterCriticalSection(m_pSync);
		int nCount = m_fnGetCount();
		
		for(int i=0; i<nCount; i++)
		{
			CObject* pdata = m_fnGetData(i);
			if(pdata != NULL)
			{
				delete pdata;
				pdata = NULL;
			}
		}

		if(nCount) RemoveAll();
		::LeaveCriticalSection(m_pSync);
	}


protected:
	int					m_nCurrentPos;
	CRITICAL_SECTION*	m_pSync;
};




#endif // !defined(AFX_MYLIST_H__7B5379EF_7BB8_4988_82B1_F6288156E5AD__INCLUDED_)
