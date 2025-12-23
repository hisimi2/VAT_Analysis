#pragma once

//Data간의 복사를 위한 클래스
//Data 관리를 위함.

/*




*/



class CVAT_DataCopy
{
public:
	CVAT_DataCopy();
	~CVAT_DataCopy();

	/*
	함수 명 정의
	_dc_m(x)_GuiToReal
	
	dc: Data Copy를 의미.
	m(x): 1~5 Mode를 의미.
	  ex) m4는 Z Auto Teaching.    

	*/

	enum eDataCopyType
	{
		eDCType_GuiToReal = 0,
		eDCType_RealToGui,
	};

public:
	//1. Z Auto Focus

public:
	//2. Cam Center Calibration

public:
	//3. Picker Align

public:
	//4. Z Auto Teaching.
	


	//GUI <-> REAL
	void _dc_m4_GUiToReal(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand);
	void _dc_m4_RealToGUI(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand);
	



public:
	//5. Vision Auto Teaching.
};

