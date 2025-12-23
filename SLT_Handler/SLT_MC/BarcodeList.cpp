#include "stdafx.h"
#include "BarcodeList.h"

#define MAX_DM_ITEM_COUNT 1024
CBarcodeList::CBarcodeList()
{
}


CBarcodeList::~CBarcodeList()
{
}

int CBarcodeList::DataInit()
{
	g_DMCont.m_dm2DID.SN(NDM13_2DID_LIST_COUNT, 0);

	for(int i =0; i<MAX_DM_ITEM_COUNT; i++)
		g_DMCont.m_dm2DID.SS(SDM13_2DID_STRING_LIST1 + i, "", MAX_PATH);

	m_ve2DIDLiST.clear();
	return NO_ERROR;
}

int CBarcodeList::ReadCSVData(CString strFilePath,CString& strAlramData)
{
	DataInit();
	CFileFind fd;
	FILE *fp = NULL;
	strAlramData = _T("");
	fopen_s(&fp, strFilePath, "r");

	if (!fd.FindFile(strFilePath)) //없는 File
	{
		strAlramData = _T("File Not Exist!!");
		return eFileNotExist;
	}

	fd.Close();

	if (fp)
	{
		char szContent[2048] = { 0, };

		memset(szContent, NULL, 2048);

		while (fgets(szContent, 2048, fp))
		{
			CString strContent;

			strContent.Format("%s", szContent);

			memset(szContent, NULL, 2048);

			if (strContent.Find("#") >= 0)
				continue;

			strContent.Remove('\r');
			strContent.Remove('\n');

			CString rString;
			int nSubString = 0;

			while (AfxExtractSubString(rString, strContent, nSubString++, ','))
			{
				if (strcmp(rString, "") == 0)
					break;

				switch (nSubString)
				{
					case 1: m_ve2DIDLiST.push_back(rString); 
						break;
				}
			}
		}
		fclose(fp);
	}
	else // 파일이 열려 있거나 수정 중이면 예외처리 필요함 [8/25/2020 donghyun.shin]
	{
		strAlramData = _T("File Not Access!!");
		return eFileNotAccess;
	}

	int n2DIDListCount = m_ve2DIDLiST.size();

	if (n2DIDListCount <= 0) {
		strAlramData = _T("File Not Data!!");
		return eFileNotData;
	}
		

	for (int i = 0; i < (int)m_ve2DIDLiST.size(); i++)
	{
		CString str = m_ve2DIDLiST[i];
		char sz2DID[MAX_PATH] = {0,};
		strcpy_s(sz2DID, MAX_PATH, (LPSTR)(LPCSTR)str);
		g_DMCont.m_dm2DID.SS(SDM13_2DID_STRING_LIST1 + i, sz2DID, str.GetLength());
	}
	g_DMCont.m_dm2DID.SN(NDM13_2DID_LIST_COUNT, (int)m_ve2DIDLiST.size());

	return NO_ERROR;
}
