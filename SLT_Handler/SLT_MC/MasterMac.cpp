#include "stdafx.h"
#include "MasterMac.h"



CMasterMac::CMasterMac()
{
	memset(&m_stMasterMacInfo, 0x00, sizeof(m_stMasterMacInfo));
	memset(&m_stMasterMacFileCheck, 0x00, sizeof(m_stMasterMacFileCheck));
	memset(&m_stMasterMacFileNotExist, 0x00, sizeof(m_stMasterMacFileNotExist));
}


CMasterMac::~CMasterMac()
{

}

// Master Mac PC Csv File Reading 하는 부분 [8/25/2020 donghyun.shin]

int CMasterMac::CBFileDataRead(CString sFilePath)
{
	CFileFind fd;
	FILE *fp = NULL;
	fopen_s(&fp, sFilePath, "r");

	if (!fd.FindFile(sFilePath)) // 없는 File에 대해선 Data 0을 삽입 [8/25/2020 donghyun.shin]
	{
		m_stMasterMacInfo.nStation = 0;
		m_stMasterMacInfo.nOnOff = 0;
		m_veMasterMacInfo.push_back(m_stMasterMacInfo);
		return 2;
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

			if(strContent.Find("#") >= 0)
				continue;

			strContent.Remove('\r');
			strContent.Remove('\n');

			CString rString;
			int nSubString = 0;

			while (AfxExtractSubString(rString, strContent, nSubString++, ','))
			{
				switch (nSubString)
				{
					case 1: m_stMasterMacInfo.nStation = atoi(rString); break;
					case 2: m_stMasterMacInfo.nOnOff = atoi(rString); break;
				}
			}

			m_veMasterMacInfo.push_back(m_stMasterMacInfo);		

			if (m_stMasterMacInfo.nOnOff == 0)
			{
				fclose(fp);
				return 1;
			}
		}
		fclose(fp);
	}
	else // 파일이 열려 있거나 수정 중이면 예외처리 필요함 [8/25/2020 donghyun.shin]
	{
		m_stMasterMacInfo.nStation = 0;
		m_stMasterMacInfo.nOnOff = 0;
		m_veMasterMacInfo.push_back(m_stMasterMacInfo);
		return 2;
	}

	return NO_ERROR;
}

CString CMasterMac::CBDataOnOffIndex()
{
	CString strData, strDataSum;
	if (m_veMasterMacInfo.size() == 0)
		return "";

	for (int i = 0; i < (int)m_veMasterMacInfo.size(); i++)
	{
		if (m_veMasterMacInfo[i].nOnOff == 0)
		{
			strData.Format("%d,", i + 1);
			strDataSum += strData;
		}
	}

	if (strDataSum.GetLength() > 0)
		strDataSum.Delete(strDataSum.GetLength() - 1, 1);

	return strDataSum;
}

//////////////////////////////////////////////////////////////////////////

int CMasterMac::CBFileModifyTimeCheck(CString sFilePath)
{
	HANDLE h_file = CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,NULL);

	FILETIME modify_time;

	if (h_file != INVALID_HANDLE_VALUE)
	{
		GetFileTime(h_file, NULL, NULL, &modify_time);
		SYSTEMTIME modify_sys_time, access_local_time;

		FileTimeToSystemTime(&modify_time, &modify_sys_time);

		SystemTimeToTzSpecificLocalTime(NULL, &modify_sys_time, &access_local_time);

		CTime FileTime(access_local_time);

		CTime tmCurrentTime = CTime::GetCurrentTime();
		CTimeSpan tmSpan_interval = tmCurrentTime - FileTime;
	
		CloseHandle(h_file);

		if (tmSpan_interval > (3600 * 6)) // 파일 수정 날짜가 6시간 이상이면 Alram [8/25/2020 donghyun.shin]
		{
			m_stMasterMacFileCheck.bFileModifyTimeOver = 1;
			m_veMasterMacFileCheck.push_back(m_stMasterMacFileCheck);
			return 1;
		}
	}
	else // 파일이 없거나 열려 있거나 둘 중 하나 [8/26/2020 donghyun.shin]
	{
		m_stMasterMacFileCheck.bFileModifyTimeOver = 0;
		m_veMasterMacFileCheck.push_back(m_stMasterMacFileCheck);
		return 1;
	}

	m_stMasterMacFileCheck.bFileModifyTimeOver = 0;
	m_veMasterMacFileCheck.push_back(m_stMasterMacFileCheck);

	return NO_ERROR;
}

CString CMasterMac::CBFileModifyIndex()
{
	CString strData,strDataSum;
	if (m_veMasterMacFileCheck.size() == 0)
		return "";

	for (int i = 0; i < (int)m_veMasterMacFileCheck.size(); i++)
	{
		if (m_veMasterMacFileCheck[i].bFileModifyTimeOver)
		{
			strData.Format("%d,",i+1);
			strDataSum += strData;
		}
	}
	
	if(strDataSum.GetLength() > 0)
		strDataSum.Delete(strDataSum.GetLength() - 1, 1);

	return strDataSum;
}

//////////////////////////////////////////////////////////////////////////

int CMasterMac::CBFileNotExist(CString sFilePath)
{
	CFileFind fd;
	if (!fd.FindFile(sFilePath)) // 없는 File에 대해선 Data 0을 삽입 [8/25/2020 donghyun.shin]
	{
		m_stMasterMacFileNotExist.bFileNotExist = 1;
		m_veMasterMacFileNotExist.push_back(m_stMasterMacFileNotExist);
		return 1;
	}

	m_stMasterMacFileNotExist.bFileNotExist = 0;
	m_veMasterMacFileNotExist.push_back(m_stMasterMacFileNotExist);
	fd.Close();

	return NO_ERROR;
}

CString CMasterMac::CBFileNotExistIndex()
{
	CString strData, strDataSum;
	if (m_veMasterMacFileNotExist.size() == 0)
		return "";

	for (int i = 0; i < (int)m_veMasterMacFileNotExist.size(); i++)
	{
		if (m_veMasterMacFileNotExist[i].bFileNotExist == 1)
		{
			strData.Format("%d,", i + 1);
			strDataSum += strData;
		}
	}

	if (strDataSum.GetLength() > 0)
		strDataSum.Delete(strDataSum.GetLength() - 1, 1);

	return strDataSum;
}




int CMasterMac::CBFileAccessWrite(CString sFilePath,int nIndex)
{
	CString strName;
	strName.Format("%s\\%d.csv", sFilePath,nIndex);
	FILE* fp = fopen(strName, "w+");

	fprintf(fp, "%d,0\n",nIndex);
	fclose(fp);

	return NO_ERROR;
}