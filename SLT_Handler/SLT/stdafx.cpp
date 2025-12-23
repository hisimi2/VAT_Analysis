
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// SLT.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


CLibCommCtrl			g_LibCommCtrl;
CDataMemCont			g_DMCont;
CDatabaseIF				g_DB;
CDatabaseIF				g_DB_LOG;
CDatabaseIF				g_DB_Teach;
CDatabaseIF				g_DB_LotHistory;
CCokFileManager         g_COK_FileMgr;
CDeviceDataMem			g_DvcDataMem;
CMCComm				    g_McComm;
CDotMatrix_Drv			g_DotMatrix;


// int 	eAXIS_TEST_PP_1_X_PITCH = 0;
// int 	eAXIS_TEST_PP_1_X2_PITCH = 1;
// int 	eAXIS_TEST_PP_1_Y_PITCH = 2;
// int 	eAXIS_TEST_PP_1_Y2_PITCH = 3;
// int 	eAXIS_TEST_PP_1_X = 4;
// int 	eAXIS_TEST_PP_1_X2 = 5;
// int 	eAXIS_TEST_PP_1_Y = 6;
// 
// int 	eAXIS_PRESS_UNIT_DN1 = 7;
// int 	eAXIS_PRESS_UNIT_DN2 = 8;
// int 	eAXIS_PRESS_UNIT_DN3 = 9;
// int 	eAXIS_PRESS_UNIT_DN4 = 10;
// 
// int 	eAXIS_TRAY_PP_1_X = 11;
// int 	eAXIS_TRAY_PP_1_X_PITCH = 12;
// int 	eAXIS_TRAY_PP_1_Y_PITCH = 13;
// 
// int 	eAXIS_TRAY_PP_2_X = 14;
// int 	eAXIS_TRAY_PP_2_Y = 15;
// int 	eAXIS_TRAY_PP_2_X_PITCH = 16;
// int 	eAXIS_TRAY_PP_2_Y_PITCH = 17;
// 
// int 	eAXIS_TRANSFER_X = 18;
// 
// int 	eAXIS_TRAY_FEEDER_Y = 19;
// int 	eAXIS_TRAY_FEEDER_END_STOPPER = 20;
// 
// int 	eAXIS_LOAD_TABLE_1_ROTATE = 21;
// int 	eAXIS_LOAD_TABLE_2_ROTATE = 22;
// int 	eAXIS_LOAD_TABLE_3_ROTATE = 23;
// int 	eAXIS_LOAD_TABLE_4_ROTATE = 24;
// 
// int 	eAXIS_LOAD_TABLE_1_Y = 25;
// int 	eAXIS_LOAD_TABLE_2_Y = 26;
// int 	eAXIS_LOAD_TABLE_3_Y = 27;
// int 	eAXIS_LOAD_TABLE_4_Y = 28;
// 
// int 	eAXIS_TEST_PP_2_Y = 29;
// int 	eAXIS_TEST_PP_2_X = 30;
// int 	eAXIS_TEST_PP_2_X2 = 31;
// int 	eAXIS_TEST_PP_2_X_PITCH = 32;
// int 	eAXIS_TEST_PP_2_X2_PITCH = 33;
// int 	eAXIS_TEST_PP_2_Y_PITCH = 34;
// int 	eAXIS_TEST_PP_2_Y2_PITCH = 35;
// 
// int 	eAXIS_PRESS_UNIT_DN5 = 36;
// int 	eAXIS_PRESS_UNIT_DN6 = 37;
// int 	eAXIS_PRESS_UNIT_DN7 = 38;
// int 	eAXIS_PRESS_UNIT_DN8 = 39;
// 
// int 	eAXIS_TRAY_PP_1_Z = 40;
// int 	eAXIS_TEST_PP_1_Z = 41;
// int 	eAXIS_TEST_PP_1_Z2 = 42;
// int 	eAXIS_TRAY_PP_2_Z = 43;
// int 	eAXIS_TEST_PP_2_Z = 44;
// int 	eAXIS_TEST_PP_2_Z2 = 45;
// 
// int 	eMAX_AXIS_COUNT = 46;



void SetDotMatrixUpdate()
{
	char szData[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));

	ST_CATE_INFO lpstData;
	g_COK_FileMgr.LoadCategoryInfo(szData, lpstData);
	int nCount = lpstData.nIndexCount;

	std::vector<ST_CATE_SUB_INFO> vOutput[4];
	for (int i = 0; i < lpstData.nIndexCount; i++)
	{
		int nHWbin = atoi(lpstData.aCateInfo[i].szHWBin);
		switch (nHWbin)
		{
		case 1: { vOutput[0].push_back(lpstData.aCateInfo[i]); } break;
		case 2: { vOutput[1].push_back(lpstData.aCateInfo[i]); } break;
		case 3: { vOutput[2].push_back(lpstData.aCateInfo[i]); } break;
		case 4: { vOutput[3].push_back(lpstData.aCateInfo[i]); } break;
		}
	}

	stDotMatrix stData;
	stData.nAddress = 1;
	stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
	stData.vData.clear();
	stData.vData.push_back(DOT_STRING_E1);
	g_DotMatrix.SendData(stData);

	stData.nAddress = 2;
	stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
	stData.vData.clear();
	stData.vData.push_back(DOT_STRING_L);
	g_DotMatrix.SendData(stData);

	stData.nAddress = 3;
	stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
	stData.vData.clear();
	stData.vData.push_back(DOT_STRING_E2);
	g_DotMatrix.SendData(stData);

	// Auto Bin 1
	stData.nAddress = 4;	
	stData.vData.clear();
	if ((int)vOutput[0].size() <= 0)
	{
		stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
		stData.vData.push_back(DOT_STRING_START_X);
	}
	else {

		int nColor = atoi(vOutput[0][0].szPassFail);
		if(nColor == eFailBin)
			stData.DotColor = DOT_SETTING::eDOT_COLOR_RED;
		else
			stData.DotColor = DOT_SETTING::eDOT_COLOR_GREEN;

		for (int i = 0; i < (int)vOutput[0].size(); i++)
		{
			if (atoi(vOutput[0][i].szSWBin) >= 0 && atoi(vOutput[0][i].szSWBin) < 19)
				stData.vData.push_back(atoi(vOutput[0][i].szSWBin));
			else
				stData.vData.push_back(DOT_STRING_START_A + atoi(vOutput[0][i].szSWBin));
			//stData.vData.push_back(DOT_NUMBER_START_1 + atoi(vOutput[0][i].szSWBin));
		}
	}	
	g_DotMatrix.SendData(stData);
	
	// Auto Bin 2
	stData.nAddress = 5;
	stData.vData.clear();
	if ((int)vOutput[1].size() <= 0)
	{
		stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
		stData.vData.push_back(DOT_STRING_START_X);
	}
	else {

		int nColor = atoi(vOutput[1][0].szPassFail);
		if (nColor == eFailBin)
			stData.DotColor = DOT_SETTING::eDOT_COLOR_RED;
		else
			stData.DotColor = DOT_SETTING::eDOT_COLOR_GREEN;

		for (int i = 0; i < (int)vOutput[1].size(); i++)
		{
			if (atoi(vOutput[1][i].szSWBin) >= 0 && atoi(vOutput[1][i].szSWBin) <= 19)
				stData.vData.push_back(atoi(vOutput[1][i].szSWBin));
			else
				stData.vData.push_back(DOT_STRING_START_A + atoi(vOutput[1][i].szSWBin));
			//stData.vData.push_back(DOT_NUMBER_START_1 + atoi(vOutput[1][i].szSWBin));
		}
	}
	g_DotMatrix.SendData(stData);

	// Auto Bin 3
	stData.nAddress = 6;
	stData.vData.clear();
	if ((int)vOutput[2].size() <= 0)
	{
		stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
		stData.vData.push_back(DOT_STRING_START_X);
	}
	else {

		int nColor = atoi(vOutput[2][0].szPassFail);
		if (nColor == eFailBin)
			stData.DotColor = DOT_SETTING::eDOT_COLOR_RED;
		else
			stData.DotColor = DOT_SETTING::eDOT_COLOR_GREEN;

		for (int i = 0; i < (int)vOutput[2].size(); i++)
		{
			if( atoi(vOutput[2][i].szSWBin) >= 0 && atoi(vOutput[2][i].szSWBin) <= 19)
				stData.vData.push_back( atoi(vOutput[2][i].szSWBin));
			else
				stData.vData.push_back(DOT_STRING_START_A + atoi(vOutput[2][i].szSWBin));
		}
	}
	g_DotMatrix.SendData(stData);

	// Auto Bin 4
	stData.nAddress = 7;
	stData.vData.clear();
	if ((int)vOutput[3].size() <= 0)
	{
		stData.DotColor = DOT_SETTING::eDOT_COLOR_ORANGE;
		stData.vData.push_back(DOT_STRING_START_X);
	}
	else {

		int nColor = atoi(vOutput[3][0].szPassFail);
		if (nColor == eFailBin)
			stData.DotColor = DOT_SETTING::eDOT_COLOR_RED;
		else
			stData.DotColor = DOT_SETTING::eDOT_COLOR_GREEN;

		for (int i = 0; i < (int)vOutput[3].size(); i++)
		{
			if (atoi(vOutput[3][i].szSWBin) >= 0 && atoi(vOutput[3][i].szSWBin) <= 19)
				stData.vData.push_back( atoi(vOutput[3][i].szSWBin));
			else
				stData.vData.push_back(DOT_STRING_START_A + atoi(vOutput[3][i].szSWBin));
			//stData.vData.push_back(DOT_NUMBER_START_1 + atoi(vOutput[3][i].szSWBin));
		}
	}	
	g_DotMatrix.SendData(stData);
}


