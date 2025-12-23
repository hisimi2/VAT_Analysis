#pragma once

#include <vector>
#include <string>
#include "DEF_DB_TBL.h"

class CVision_2dBarcode
{
public:
	CVision_2dBarcode(void);
	~CVision_2dBarcode(void);

	bool IsDupData(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<ST_2D_BARCODE_INFO>& v2dBarcodeBakForCancel, std::vector<CPoint>& v2dDuplicateCoordi);
	int IsMarkValidate(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<std::pair<CPoint, CString>> &v2DErrorData);
	int  IsShortLen(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<CPoint> &v2DErrorData);
	int IsSameLength(int nTableIdx, std::vector<_tVisionReqData>& vReqData, std::vector<CPoint>& v2DLengthFailPocket, std::vector<_ST_2DID_SAME_LENGTH>& v2DSameLength);
	bool IsFullError(std::vector<_tVisionReqData>& vReqData);
	int  LoadData();
	void ResetData();
	void DelData(std::vector<ST_2D_BARCODE_INFO> vReqData  );
	void DelData(char* sz2dData);

private:
	std::vector<ST_2D_BARCODE_INFO> m_v2dBarcode;
	
	void Push2dData(std::vector<ST_2D_BARCODE_INFO>& vBarcode);
	void SaveHistoryData();
	void ReadRecord(std::istream& is, std::vector<_tVisionReqData> &vec);

	void AddRecode_DB(std::vector<ST_2D_BARCODE_INFO> &vec);
	void GetRecodeAllRows_DB(std::vector<ST_2D_BARCODE_INFO> &vec);

	void MakeTimeToString(SYSTEMTIME t, CString& strTime);
	CCriticalSection m_cs;
};

