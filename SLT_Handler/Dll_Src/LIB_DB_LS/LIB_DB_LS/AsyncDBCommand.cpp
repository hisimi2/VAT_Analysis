#include "StdAfx.h"
#include "AsyncDBCommand.h"

#include "DatabaseIF_Imp.h"

/*
void CADBC_Update_DIO_LS::Execute()
{
	g_DbIf.Update_DioTbl(m_nDirection, m_DioList);
}
*/


void CADBC_Insert_GrrData::Execute()
{
	g_DbIf->INSERT_GRRDATA_TBL(m_stGrrData);
}

void CADBC_Insert_GrrData::SetInsertData(const _tGRR_DATA& data)
{
	m_stGrrData = data;
}