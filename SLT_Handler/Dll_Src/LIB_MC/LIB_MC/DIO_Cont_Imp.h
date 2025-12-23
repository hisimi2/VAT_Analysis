#pragma once


//==============================================================================
//
//==============================================================================
#include <vector>
#include <memory>
#include "DlgDioAll.h"



//==============================================================================
//
//==============================================================================
#ifndef __DIO_TYPE__
#define __DIO_TYPE__
	#define _DIO_TYPE_SIMUL_		(0)
	#define _DIO_TYPE_SYCON_		(1)
	#define _DIO_TYPE_TECHWING_		(2)
	#define _DIO_TYPE_CONTEC_		(3)
	#define _DIO_TYPE_SOFTSERVO_	(4)
#endif // __DIO_TYPE__

//==============================================================================
//
//==============================================================================
class CDI;
class CDO;
class CDlgDioAll;
class CMyList;
//==============================================================================
//
//==============================================================================


class CDIO_Cont_Imp
{
public:
	CDIO_Cont_Imp(void);
	~CDIO_Cont_Imp(void);

	int		Initialize(const char* szDriverIniFileName, int nFileNameLen);
	void	Finalize();
	BOOL	IsInitBoard()				{ return m_bBoardInit; };
	void	RESET_DO_DRV_CALL();
	
	char    GET_DI(int nIndex);
	void    SET_DO(int nIndex, char cValue);
	char    GET_DO(int nIndex);

	int     GetDIPointCount(){ return m_nDI_PointMax; };
	int     GetDOPointCount(){ return m_nDO_PointMax; };
	void    ShowDialog_IO_All();
	void    ShowDialogSimul();
	int     GetIoInfo(int nInOrOut, int nIndex, _tDIO_LS& IoInfo);
	void    SetSimulDi(int nIndex, BOOL bValue);
	void    SetSimulWnd(CWnd* pSimWnd);
private:
	void	SET_DO_DRV_CALL(int Out_addr, BOOL data);
	BOOL	GET_DI_DRV_CALL(int In_addr);

	void	SetInitMMC(BOOL bOK)	{ m_bBoardInit = bOK; };

public:
	void	Read_DI_All();
	void	Write_DO_All();

	std::vector<BYTE>	_in;
	std::vector<BYTE>	_out;

private:

	int				m_nDO_PointMax;
	int				m_nDI_PointMax;
	int				m_nDI_BaseAddr;
	int				m_nDO_BaseAddr;
	BOOL			m_bBoardInit;
	CDI*			m_pDI;
	CDO*			m_pDO;
	char			m_szInitFileName[512];

	CDlgDioAll*     m_pDlgIoAll;
	CMyList         m_ListDbInfoDi;
	CMyList         m_ListDbInfoDo;
};