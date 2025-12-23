#pragma once

#include <iostream>     // std::cout
#include <algorithm>    // std::find
#include <vector>       // std::vector

// 1. JamRate
// SCK에서 사용하는 Format에 맞추어 운용
// 1. AAB Rule에 근거하여 Format의 자료를 수집한다.
// FT -> First, R1, R2로 분류하여 File Save함.
//    -> First는 Testing이 1번 이루어 진 것, AAB에서 첫번째 A에 해당 한다.
//    -> R1은 2번째 Test가 들어간 것들에 대한 것, AAB에서 두번째 A에 해당하며, AB가 될 수도 있음.
//    -> R2는 3번째 Test가 들어간 것들에 대해서, AAB에서 세번째 B에 해당.
//
// RT -> R3, R4, R5로 분류
//    -> R3는 AAB의 첫번째 A에 관련된 Data
//    -> R4는 AAB의 두번째 A에 관련된 Data
//    -> R5는 AAB의 세번째 B에 관련된 Data

typedef struct _ST_PRODUCTION_DATA{
	char szLotTitle[64];
	int  nInCount;			// Loading count
	int  nOutCount;			// unloading pass count
	double dYield;
	int  nHwBinCount[4];
	int  nInputJam;
	int  nOutputJam;
	int  nPartTest[16];
	int  nTotalGd[16];

	_ST_PRODUCTION_DATA()
	{
		memset(this, 0x00, sizeof(_ST_PRODUCTION_DATA));
	};
}ST_PRODUCTION_DATA;


class CProcJamRate
{
public:
	CProcJamRate(void);
	~CProcJamRate(void);

	// PushData
	void PushDeviceData(ST_REC_DATA stData);
	void PushInOutJam(int nInOutType/*enum eInOutJamType{*/, int nCount=0);

	// set category list item
	void SetBinCateItemList();

	// set data clear
	void ClearRecDvcData();
	void ResetAllData();

	// recovery data
	
	void SaveBackUpData(int nType/*enum eJamRateDataType{*/, const std::vector<ST_REC_DATA> &vec);
	void LoadBackUpData(int nType/*enum eJamRateDataType{*/);

	void ProductCsvTransfer();
	int SaveLotJamRate_Amkor(); // 301N
	int SaveLotJamRate_Amkor_TWSL421();
	int SaveLotHistory();
	int SaveLotJamRate_Summary();
	int SaveAmkorLotSummaryFileCreate();
private:
	std::vector<ST_REC_DATA>        m_vOnlyOneDvc;
	std::vector<ST_REC_DATA>        m_vFirstData;
	std::vector<ST_REC_DATA>        m_vSecondData;
	std::vector<ST_PRODUCTION_DATA> m_vProductionData;
	int m_nInputJamCnt;
	int m_nOutputJamCnt;

	BOOL m_fnCreateDirectory( LPCSTR pszDirectory );

	void WriteRecord(std::ostream& os, const std::vector<ST_REC_DATA> &vec);
	void ReadRecord(std::istream& is, std::vector<ST_REC_DATA> &vec);
};

