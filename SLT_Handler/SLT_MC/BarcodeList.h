#pragma once
class CBarcodeList
{
public:
	CBarcodeList();
	~CBarcodeList();

	std::vector<CString> m_ve2DIDLiST;

	int DataInit();
	int ReadCSVData(CString strFilePath, CString& strAlramData);
	
};

