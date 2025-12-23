#include "StdAfx.h"
#include "DIO_SoftServo.h"

//////////////////////////////////////////////////////////////////////////
//
//  DI SoftServo
//
//////////////////////////////////////////////////////////////////////////

CDI_SoftServo::CDI_SoftServo(int nBaseAddress, int nDiBoardCount)
{
//	CDB_SYSTEM db;	
//	db.SELECT_DIO_TBL(eIO_DIR::eInput, DIList);

	CMyList DIList;
	g_pDbIf->SELECT_DIO_TBL(_tDIO_LS::eDI, DIList);

//	g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, m_vLockSen[i], 1, rDiInfo);

	m_nBaseAddress = nBaseAddress;
	m_nDi_BoardCount = nDiBoardCount;
	m_nDI_PointCount = SOFT_SERVOE_DIO_MAX_POINT_ONE_BD * nDiBoardCount;
	m_nDI_PortCount = SOFT_SERVOE_DIO_MAX_POINT_ONE_BD  * nDiBoardCount;	// Board 한개에 16개의 Port 존재, Port 한개에 8 Point

	if (!m_nBaseAddress) {
		return;
	}

	m_pDI_PointBuffer = new BYTE[m_nDI_PointCount];
	memset(m_pDI_PointBuffer, 0x00, sizeof(BYTE)*m_nDI_PointCount);

	m_pDI_PortBuffer = new BYTE[m_nDI_PortCount];
	memset(m_pDI_PortBuffer, 0x00, sizeof(BYTE)*m_nDI_PortCount);

	nEtherCAT_IN = new int[m_nDI_PointCount];

	memset(nEtherCAT_IN, -1, sizeof(nEtherCAT_IN));

	for (int i = 0; i < DIList.m_fnGetCount(); i++)
	{
		_tDIO_LS*  tDI= (_tDIO_LS*)DIList.m_fnGetData(i);
		nEtherCAT_IN[i] = tDI->address;
	}
}


CDI_SoftServo::~CDI_SoftServo()
{
	if (m_pDI_PointBuffer != NULL) {
		delete[] m_pDI_PointBuffer;
		m_pDI_PointBuffer = NULL;
	}
	if (m_pDI_PortBuffer != NULL) {
		delete[] m_pDI_PortBuffer;
		m_pDI_PortBuffer = NULL;
	}

	if (nEtherCAT_IN != NULL) {
		delete[] nEtherCAT_IN;
		nEtherCAT_IN = NULL;
	}
}

void CDI_SoftServo::Read_DI()
{
	int indx = 0;
	int bitcnt;
	int byteno = 0;
	int bitno = 0;
	unsigned char inData[wmx3Api::constants::maxIoInSize];
	BYTE value[300];

	g_pWmx3Lib_Io->GetInBytes(0, 300, value);
	for (int byte_idx = 0; byte_idx < 300; byte_idx++)
	{
		for (int i = 0; i < 8; i++)
		{
			m_pDI_PointBuffer[indx] = (value[byte_idx] & (1 << i)) ? 1 : 0;
		}
	}


	for (indx = 0; indx < m_nDI_PointCount; indx++)
	{
		//in[indx] = Wmx3Lib_Io.GetInBit(indx / 8, indx % 8, &inData[indx]);
		//in[indx] = pEtherCat->Get_Input_IO(indx);			

		byteno = nEtherCAT_IN[indx];
		bitno = byteno % 10;
		byteno = byteno / 10;

		m_pDI_PointBuffer[indx] = (value[byteno] & (1 << bitno)) ? 1 : 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//  DO SoftServo
//
//////////////////////////////////////////////////////////////////////////

CDO_SoftServo::CDO_SoftServo(int nBaseAddress, int nDoBoardCount)
{
	CMyList DOList;
	g_pDbIf->SELECT_DIO_TBL(_tDIO_LS::eDO, DOList);

	int i = 0;

	m_nBaseAddress = nBaseAddress;
	m_nDo_BoardCount = nDoBoardCount;
	m_nDO_PointCount = SOFT_SERVOE_DIO_MAX_POINT_ONE_BD * nDoBoardCount;
	m_nDO_PortCount = SOFT_SERVOE_DIO_MAX_POINT_ONE_BD  * nDoBoardCount;

	if (!m_nBaseAddress) {
		return;
	}

	m_pDO_PointBuffer = new BYTE[m_nDO_PointCount];
	m_pDO_PortBuffer = new BYTE[m_nDO_PortCount];
	m_pDO_PortBuffer_Old = new BYTE[m_nDO_PortCount];

	for (i = 0; i < m_nDO_PortCount; i++) {
		m_pDO_PortBuffer[i] = 0;
		m_pDO_PortBuffer_Old[i] = 0xFF;
	}

	for (i = 0; i < m_nDO_PointCount; i++) {
		m_pDO_PointBuffer[i] = 0;
	}

	nEtherCAT_OUT = new int[m_nDO_PointCount];

	for (int i = 0; i < DOList.m_fnGetCount(); i++)
	{
		_tDIO_LS*  tDO = (_tDIO_LS*)DOList.m_fnGetData(i);
		nEtherCAT_OUT[i] = tDO->address;
	}
}

CDO_SoftServo::~CDO_SoftServo(void)
{
	if (m_pDO_PointBuffer != NULL) {
		delete[] m_pDO_PointBuffer;
		m_pDO_PointBuffer = NULL;
	}

	if (m_pDO_PortBuffer != NULL) {
		delete[] m_pDO_PortBuffer;
		m_pDO_PortBuffer = NULL;
	}

	if (m_pDO_PortBuffer_Old != NULL) {
		delete[] m_pDO_PortBuffer_Old;
		m_pDO_PortBuffer_Old = NULL;
	}

	if (nEtherCAT_OUT != NULL) {
		delete[] nEtherCAT_OUT;
		nEtherCAT_OUT = NULL;
	}
}

void CDO_SoftServo::Write_DO()
{
	int indx;
	int bitcnt;
	int byteno;
	int bitno;
	
	for (indx = 0; indx < m_nDO_PointCount; indx++)
	{
		byteno = nEtherCAT_OUT[indx];
		bitno = byteno % 10;
		byteno = byteno / 10;
	
		if (m_pDO_PointBuffer[indx] == 0)
		{
			g_pWmx3Lib_Io->SetOutBit(byteno, bitno, 0);
		}
		else
		{
			g_pWmx3Lib_Io->SetOutBit(byteno, bitno, 1);
		}
	}
	
}

int CDO_SoftServo::Write_DO_Bit(int _idx, int _on_off)
{
	int indx;
	int bitcnt;
	int byteno;
	int bitno;

	byteno = nEtherCAT_OUT[_idx];
	bitno = byteno % 10;
	byteno = byteno / 10;

	g_pWmx3Lib_Io->SetOutBit(byteno, bitno, _on_off);

	return 1;
}

void CDO_SoftServo::Reset()
{
	if (!m_nBaseAddress) {
		return;
	}

	int i = 0;

	memset(m_pDO_PointBuffer, 0x00, sizeof(BYTE)*m_nDO_PointCount);

	// Board 한개의 16개 Port 존재
	for (i = 0; i < m_nDO_PointCount; i++) {
		m_pDO_PointBuffer[i] = 0;
	}

	Read_DO_Data();
	
	//Write_DO();
}

void CDO_SoftServo::Read_DO_Data()
{
	int indx;
	int byteno;
	int bitno;
	BYTE nOnOff;
	for (indx = 0; indx < m_nDO_PointCount; indx++)
	{
		byteno = nEtherCAT_OUT[indx];
		bitno = byteno % 10;
		byteno = byteno / 10;
		g_pWmx3Lib_Io->GetOutBit(byteno, bitno, &nOnOff);
		if (nOnOff == 0x01)
		{
			m_pDO_PointBuffer[indx] = 1;
		}
		else
		{
			m_pDO_PointBuffer[indx] = 0;
		}
	}
}