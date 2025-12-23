#pragma  once

//==============================================================================
// Abstract Class for Digital Input Driver
//==============================================================================
class  CDI
{
public:
	CDI();
	~CDI();

	virtual void Read_DI(){};
	virtual int  Read_DI_Bit(int bit_num);

public:
	int				m_nDI_PointCount;					// POINT Count
	BYTE*			m_pDI_PointBuffer;					// POINT Value, 1 byte에 point 하나에 대한 값을 저장한다.
	BYTE*			m_pDI_PortBuffer;					// PORT Value

};



//==============================================================================
// Abstract Class for Digital Output Driver
//==============================================================================
class  CDO
{
public:
	CDO();
	~CDO();

	virtual void Write_DO(){};
	virtual int  Write_DO_Bit(int _idx, int onoff);
	void Reset();

public:
	int				m_nDO_PointCount;                     /* available DIO channel no. */
	BYTE*			m_pDO_PointBuffer;
	BYTE*			m_pDO_PortBuffer;
	BYTE*			m_pDO_PortBuffer_Old;
};