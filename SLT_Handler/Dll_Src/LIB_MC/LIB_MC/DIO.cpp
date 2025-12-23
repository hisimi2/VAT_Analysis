#include "stdafx.h"
#include "DIO.h"

///////////////////////////////////////////////////////////////////////////////
//
//                            ::CDI::
//
///////////////////////////////////////////////////////////////////////////////

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDI::CDI()
{
	m_pDI_PointBuffer   = NULL;
	m_pDI_PortBuffer	= NULL;
	m_nDI_PointCount = 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDI::~CDI()
{ 
	if( m_pDI_PointBuffer != NULL ){
		delete[] m_pDI_PointBuffer;
		m_pDI_PointBuffer = NULL;
	}
	if( m_pDI_PortBuffer != NULL ){
		delete[] m_pDI_PortBuffer;
		m_pDI_PortBuffer = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//                            ::CDO::
//
///////////////////////////////////////////////////////////////////////////////

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 하드웨어 함수를 호출 하여 IO를 읽는다.
==============================================================================*/
int  CDI::Read_DI_Bit(int bit_num)
{
	Read_DI();
	return m_pDI_PointBuffer[bit_num];
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDO::CDO()
{
	m_pDO_PointBuffer		= NULL;
	m_pDO_PortBuffer		= NULL;
	m_pDO_PortBuffer_Old	= NULL;
	m_nDO_PointCount = 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CDO::~CDO()
{ 
	Reset(); 
	if( m_pDO_PointBuffer != NULL ){
		delete[] m_pDO_PointBuffer;
		m_pDO_PointBuffer = NULL;
	}
	if( m_pDO_PortBuffer != NULL ){
		delete[] m_pDO_PortBuffer;
		m_pDO_PortBuffer = NULL;
	}
	if( m_pDO_PortBuffer_Old != NULL ){
		delete[] m_pDO_PortBuffer_Old;
		m_pDO_PortBuffer_Old = NULL;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 하드웨어 함수까지 호출 한다.
==============================================================================*/
int  CDO::Write_DO_Bit(int bit_num, int onoff)
{
	m_pDO_PointBuffer[bit_num] = onoff;
	Write_DO();
	return 1;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDO::Reset()
{

}
