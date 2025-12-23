#ifndef __DEVICE_DATA_MEM__
#define __DEVICE_DATA_MEM__

/*=============================================================================================
* <<라이브러리 설명>>
* LIB_DATA_MEM 에서는 Device Data를 저장 할 수 있는 기능만 제공하고 어떤 메모리에 어떤 데이터를 
* 저장 할 것인지는 어플리케이션 단에서 결정 한다.
*
* << 용어 설명 >>
* Stage : Device가 놓일 수 있는 위치 (Setplate, Loading Table, Hand, Test Site 등을 모두 포함.) 
=============================================================================================*/
#include "DEF_COMMON.h"

#define SZ_DATA_MEM_PATH		_T(".\\sys_data\\DATA_MEM")

#define DEVICE_DATA_MAX_X				(35)
#define DEVICE_DATA_MAX_Y				(41)
#define DEVICE_DATA_MAX_STAGE_LOC       (41)

enum eDeviceExist
{
	eDeviceData_Unk   = -99,
	eDeviceData_Lost  = -1,
	eDeviceData_None  = 0,
	eDeviceData_Exist = 1,
};

enum eTrayType{
	eTrayType_C_Tray = 0,
	eTrayType_T_Tray,
	eMaxTrayTypeCount,
};

typedef struct _ST_DVC_DATA{
	int  nSiteNo;      // site number 0,1,2 ..
	CPoint cSubSiteNo;   // Station Para 대응 0-(0,0) , 0-(1,0) ....
	int  nPF;          // eBinFail=0, eBinGood=1
	int  nHwBin;	   // 0:UnloadSetplate#1 ~3:UnloadSetplate#4
	char szErrStr[32]; // Error Description
	char szErrCode[32];// ErrorCode .. "","E1","E97","E99"
	char szTesterHwBin[32]; // Tester Hw Bin 1,3,4,7
	int  nSwBin;
	BOOL bQAEmpty;

	_ST_DVC_DATA()
	{
		clear();
	};

	_ST_DVC_DATA( const _ST_DVC_DATA& src )
	{
		*this = src;
	};

	_ST_DVC_DATA& _ST_DVC_DATA::operator=(const _ST_DVC_DATA& src)
	{
		if( this == &src )
		{
			return *this;
		}

		nSiteNo = src.nSiteNo;
		cSubSiteNo = src.cSubSiteNo;
		nPF = src.nPF;
		nHwBin = src.nHwBin;
		nSwBin = src.nSwBin;
		bQAEmpty = src.bQAEmpty;
		strcpy(szErrStr, src.szErrStr);
		strcpy(szErrCode, src.szErrCode);
		strcpy(szTesterHwBin, src.szTesterHwBin);
		return *this;
	}

	void clear()
	{
		nSiteNo = -1;
		nPF     = -1;
		nHwBin  = -1;
		nSwBin  = -1;
		bQAEmpty = FALSE;
		cSubSiteNo = CPoint();
		ZeroMemory(szErrStr, sizeof(szErrStr));
		ZeroMemory(szErrCode, sizeof(szErrCode));
		ZeroMemory(szTesterHwBin, sizeof(szTesterHwBin));
	}
}ST_DVC_DATA;

typedef struct _ST_REC_DATA{
	ST_DVC_DATA stDvcInfo[3];

	_ST_REC_DATA()
	{
		clear();
		for(int i=0; i<3; i++){
			stDvcInfo[i].clear();
		}		
	}

	void clear()
	{
		memset(this, 0x00, sizeof(_ST_REC_DATA));
	}
}ST_REC_DATA;

typedef struct _ST_REC_LOAD_CNT{
	int nFirst;
	int nSecond;
	int nThird;

	_ST_REC_LOAD_CNT()
	{
		nFirst  = -1;
		nSecond = -1;
		nThird  = -1;
	}
}ST_REC_LOAD_CNT;

typedef struct _ST_AUDITTEST_INFO
{
	BYTE   nMaxTestCnt;											// Test Max 횟수
	BYTE   nTestCnt[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];	// Test 횟수
	//char   szTestSiteNum[4];									// 0~15 [0~7:Down, 8~15:Up ]

	BYTE    nSiteCntX;	// station의 site max x
	BYTE    nSiteCntY;	// station의 site max y
	
	_ST_AUDITTEST_INFO()
	{
		memset(this, 0, sizeof(_ST_AUDITTEST_INFO));
	};

	void testDone(int nX, int nY) { nTestCnt[nX][nY]++; }
	void testDoneAll() 
	{ 
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if (nTestCnt[nX][nY] < nMaxTestCnt)
					nTestCnt[nX][nY]++;
			}
		}
	}

	BOOL chkTestEnd() // 1station 내 전체 site 기준
	{
		int nDoneCnt = 0;
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if( chkTestEnd(nX, nY) == TRUE )
					nDoneCnt++;
			}
		}

		if (nDoneCnt == nSiteCntX*nSiteCntY)
			return TRUE;
		else
			return FALSE;
	}
	BOOL chkTestEnd(int nX, int nY) // 1station 내 1 site 기준
	{
		if (nTestCnt[nX][nY] >= nMaxTestCnt || nMaxTestCnt == 0 )
			return TRUE;
		else
			return FALSE;
	}
} ST_AUDITTEST_INFO;

typedef struct _ST_LOOPTEST_INFO
{
	int   nMaxTestCnt;		// Test Max 횟수
	int   nTestCnt[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];	// Test 횟수
	//char   szTestSiteNum[4];									// 0~15 [0~7:Down, 8~15:Up ]

	int    nSiteCntX;	// station의 site max x
	int    nSiteCntY;	// station의 site max y
	
	int    nRePickPlaceCnt;
	int    nReContinueCnt;

	_ST_LOOPTEST_INFO()
	{
		memset(this, 0, sizeof(_ST_LOOPTEST_INFO));
	};

	void testDone(int nX, int nY) { nTestCnt[nX][nY]++; }
	void testDoneAll() 
	{ 
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if (nTestCnt[nX][nY] < nMaxTestCnt)
					nTestCnt[nX][nY]++;
			}
		}
	}

	BOOL chkTestEnd() // 1station 내 전체 site 기준
	{
		int nDoneCnt = 0;
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if( chkTestEnd(nX, nY) == TRUE )
					nDoneCnt++;
			}
		}

		if (nDoneCnt == nSiteCntX*nSiteCntY)
			return TRUE;
		else
			return FALSE;
	}
	BOOL chkTestEnd(int nX, int nY) // 1station 내 1 site 기준
	{
		if (nTestCnt[nX][nY] >= nMaxTestCnt || nMaxTestCnt == 0 )
			return TRUE;
		else
			return FALSE;
	}

	BOOL chkRePickPlace() // 1station 내 1 site 기준
	{
		int nDoneCnt = 0;
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if (nTestCnt[nX][nY] % nReContinueCnt == 0)
					nDoneCnt++;
			}
		}

		if (nDoneCnt == nSiteCntX*nSiteCntY)
			return TRUE;
		else
			return FALSE;
	}

} ST_LOOPTEST_INFO;

typedef struct _ST_GRRTEST_INFO
{
	BYTE   nMaxTestCnt;											// Test Max 횟수
	BYTE   nTestCnt[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];	// Test 횟수
	char   szTestSiteNum[4];									// 0~15 [0~7:Down, 8~15:Up ]

	BYTE    nSiteCntX;	// station의 site max x
	BYTE    nSiteCntY;	// station의 site max y
	
	_ST_GRRTEST_INFO()
	{
		memset(this, 0, sizeof(_ST_GRRTEST_INFO));
	};

	void testDone(int nX, int nY) { nTestCnt[nX][nY]++; }
	void testDoneAll() 
	{ 
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if (nTestCnt[nX][nY] < nMaxTestCnt)
					nTestCnt[nX][nY]++;
			}
		}
	}

	BOOL chkTestEnd() // 1station 내 전체 site 기준
	{
		int nDoneCnt = 0;
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if( chkTestEnd(nX, nY) == TRUE )
					nDoneCnt++;
			}
		}

		if (nDoneCnt == nSiteCntX*nSiteCntY)
			return TRUE;
		else
			return FALSE;
	}
	BOOL chkTestEnd(int nX, int nY) // 1station 내 1 site 기준
	{
		if (nTestCnt[nX][nY] >= nMaxTestCnt || nMaxTestCnt == 0 )
			return TRUE;
		else
			return FALSE;
	}

	int chkTestDone() // 1Station 내 Socket Test 진행 완료 된 Count
	{
		int nDoneCnt = 0;
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				if (chkTestEnd(nX, nY) == TRUE)
					nDoneCnt++;
			}
		}

		return nDoneCnt;
	}
} ST_GRRTEST_INFO;

typedef struct _ST_DEVIC_PICK_HISTORY
{
	int nStage;        // Device Pick 했던 Stage 
	CPoint cPocketNum; // Stage 의 Pocket
	CPoint cPicker;    // Device 를 Pick 한 Picker

	_ST_DEVIC_PICK_HISTORY()
	{
		memset(this, 0x00, sizeof(_ST_DEVIC_PICK_HISTORY));
	};

}ST_DEVIC_PICK_HISTORY;

typedef struct _ST_DD_DEVICE
{
	int    sExist;
	char   szBinResult[16];
	char   szTestSiteNum[4];  // 0~15 [0~7:Down, 8~15:Up ]
	CPoint cSubSiteNum;       // 0-(0,0), 0-(1,0) ....
	int    nTestingCnt;		   // enum eAutoRetestBin
	int    nTestingInfo;	   // enum eTestSiteInfo
	int    nBinColor;		   // enum eBIN_COLOR
	char   szErrString[32];
    char   szErrorCode[32];
	char   szTesterHWBin[32];
	char   sz2DBarcode[128];
	int    nRealTestingCnt;
	int		n2DIDErrorCnt;
	int    nAlramErrorBin;
// Auto GRR ->	
 	char   szUniqueID[10];				// 고유ID(DB Data 연동용)
	int    nTotalStationCnt;			// Test 진행할 Site 개수	
 	int    nRow;						// Device의 원래위치 (y)
 	int	   nCol;						// Device의 원래위치 (x) 
 	ST_GRRTEST_INFO stGrrTestInfo[eMaxPressUnitCount];
// <-
	ST_LOOPTEST_INFO stLoopTestInfo[eMaxPressUnitCount];
	
	ST_AUDITTEST_INFO stAuditTestInfo[eMaxPressUnitCount];
	
	ST_DEVIC_PICK_HISTORY stHistory;

	ST_REC_DATA stRecInfo;

	_ST_DD_DEVICE()
	{
		clear();
		stRecInfo.clear();
	};

	void clear()
	{
		memset(this, 0x00, sizeof(_ST_DD_DEVICE));
	}

 	int findIndexTestSite(char* szSiteNum)
 	{
 		int nIndex = -1;
 		for (int i = 0; i < nTotalStationCnt; i++)
 		{
			if (strcmp(szSiteNum, stGrrTestInfo[i].szTestSiteNum) == 0)
			{
				nIndex = i;
				break;
			}
 		} 
 		return nIndex;
 	}
 
 	BOOL chkTestEndSite(char* szSiteNum)
 	{
 		int nIndex = -1;
		for (int i = 0; i < nTotalStationCnt; i++)
		{
			if (strcmp(szSiteNum, stGrrTestInfo[i].szTestSiteNum) == 0)
			{
				nIndex = i;
				break;
			}
		}
 
 		ASSERT(nIndex >= 0);
 		return stGrrTestInfo[nIndex].chkTestEnd();
 	}
 
 	int findNextTestSite()
 	{
 		int nRetNextSiteNo = -1;
 
 		for (int i = 0; i < nTotalStationCnt; i++)
 		{
 			if (stGrrTestInfo[i].chkTestEnd() != TRUE)
 			{
 				nRetNextSiteNo = atoi(stGrrTestInfo[i].szTestSiteNum);
 				break;
 			}
 		} 
 		return nRetNextSiteNo;
 	}
 
 	BOOL chkAutoGrrCompelte()
 	{
 		int nChkCnt = 0;
 		for (int i = 0; i < nTotalStationCnt; i++)
 		{
			if (stGrrTestInfo[i].chkTestEnd() == TRUE)
			{
				nChkCnt++;
			}
 		} 
 		if (nChkCnt >= nTotalStationCnt)
 			return TRUE;
 		else
 			return FALSE;
 	}
}ST_DD_DEVICE;

typedef struct _ST_DD_TRAY
{
	int nTrayType;
	BOOL bTrayExist;
	char szTrayId[32];
	int  nDeviceQty;
	int  nColMax;
	int  nRowMax;

	_ST_DD_TRAY()
	{
		clear();
	}
	void clear()
	{
		nTrayType = eTrayType_C_Tray;
		bTrayExist = 0;
		ZeroMemory(szTrayId, sizeof(char)*32);
		nDeviceQty = 0;
		nColMax = 1;
		nRowMax = 1;
	}
}ST_DD_TRAY;

typedef struct _ST_DD_STAGE
{
	ST_DD_DEVICE Device[DEVICE_DATA_MAX_Y][DEVICE_DATA_MAX_X];
	ST_DD_TRAY   TrayData;
	char         szStageName[32];

	_ST_DD_STAGE()
	{
		clear();
	};

	void clear()
	{
		for(int y=0; y<DEVICE_DATA_MAX_Y; y++){
			for(int x=0; x<DEVICE_DATA_MAX_X; x++){
				Device[y][x].clear();
			}
		}
		TrayData.clear();
		ZeroMemory(szStageName, sizeof(char)*32);
	};
}ST_DD_STAGE;

typedef struct _ST_DEVICE_DATA_MEM
{
	ST_DD_STAGE Loc[DEVICE_DATA_MAX_STAGE_LOC];
}ST_DEVICE_DATA_MEM;

#endif // __DEVICE_DATA_MEM__
