#pragma once


//==============================================================================
//
//==============================================================================
#include "afx.h"
#include "DEF_DB_TBL.h"
#include "MyList.h"

/*******************************************************************************
	<<pseudo code>>
	_tDIO_LS tDIO;
	LDBC_Update_DIO_LS* pDbCmd = new LDBC_Update_DIO_LS(db_conn);
	pDbCmd->m_nDirection = 0;
	pDbCmd->m_tblDIO = tDIO;
	g_DbIf->SEND_DB_QUEUE( pDbCmd );
*******************************************************************************/

//==============================================================================
//
//==============================================================================
class CADBC : public CObject
{
public:
	CADBC(void){};
	~CADBC(void){};
	virtual void Execute()=0;
	virtual void SetInsertData(const _tGRR_DATA& data)=0;
};

//==============================================================================
// sample code
//==============================================================================
/*
class CADBC_Update_DIO_LS : public CADBC
{
public:
	CADBC_Update_DIO_LS(){};
	CADBC_Update_DIO_LS(const CADBC_Update_DIO_LS& src)
	{
		// TODO Strip : 
	}
	~CADBC_Update_DIO_LS(){};
	void Execute();

	int			m_nDirection;
	CMyList		m_DioList;
};
*/
class AFX_EXT_CLASS CADBC_Insert_GrrData : public CADBC
{
public:
	_tGRR_DATA m_stGrrData;

	void Execute();
	void SetInsertData(const _tGRR_DATA& data);
};
