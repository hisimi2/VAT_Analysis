#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

//==============================================================================
//
//==============================================================================
class CBaseTaskManager;

class _tVisionReqData;
class _tVisionMsg;
//==============================================================================
//
//==============================================================================
class CTaskLoadTable : public CBaseTaskManager
{
public:
	enum eTeachPosX {
		eXPOS_OUT_OF_TEACH = -1,
		eXPOS_LD_SIDE = 0,
		eXPOS_TEST_SIDE,
		eXPOS_MAX,
		eXPOS_Between_Nega_TestSide,
		eXPOS_TestSide,
		eXPOS_Between_LoadSide_TestSide,
		eXPOS_LoadSide,
		eXPOS_Between_LoadSide_Posi,
	};

	enum eTeachPosY {
		eYPOS_NegaToLoadSide,
		eYPOS_LoadSide,
		eYPOS_LoadSideToSitePos1,
		eYPOS_SitePos1,
		eYPOS_SitePos1ToSitePos2,
		eYPOS_SitePos2,
		eYPOS_SitePos2ToSitePos3,
		eYPOS_SitePos3,
		eYPOS_SitePos3ToSitePos4,
		eYPOS_SitePos4,
		eYPOS_SitePos4ToPosiLimit,
		eYPOS_MAX,

		eYPOS_Rotate_AngleNegTo0,
		eYPOS_Rotate_Angle0,
		eYPOS_Rotate_Angle0To90,
		eYPOS_Rotate_Angle90,
		eYPOS_Rotate_Angle90ToPos,

		//나중에 삭제 필요
// 		eYPOS_OUT_OF_TEACH = -1,
// 		eYPOS_LD_SIDE = 0,
// 		eYPOS_TEST_SIDE,
// 		//		eYPOS_MAX,
// 
// 		eYPOS_Between_Nega_TestSide,
// 		eYPOS_TestSide,
// 		eYPOS_Between_LoadSide_TestSide,
// 		eYPOS_LoadSide,
// 		eYPOS_Between_LoadSide_Posi,
	};

	enum eDI {
		xLOAD_TABLE_DEVICE1_SEN,
		xLOAD_TABLE_DEVICE2_SEN,
		xLOAD_TABLE_DEVICE3_SEN,
		xLOAD_TABLE_DEVICE4_SEN,

		xLOAD_TABLE_ROTATE1_SEN,
		xLOAD_TABLE_ROTATE2_SEN,
		xLOAD_TABLE_ROTATE_STOP_SEN,

		x2D_BARCODE_LEFT_SEN,
		x2D_BARCODE_RIGHT_SEN,

		xLOAD_TABLE_VIB_FOR_SEN,
		xLOAD_TABLE_VIB_BACK_SEN,

		xMAX_COUNT,
	};

	enum eDO {
		y2D_BARCODE_LEFT,
		y2D_BARCODE_RIGHT,
		yVIBRATOR_FOR,
		yVIBRATOR_BACK,
		yMAX_COUNT,
	};

	enum eMotor {
		eAxisAll = -1,
		eAxisY = 0,
		eAxisRotate,
		eMaxAxisCount
	};

	enum eLoadTbl_CylPos {
		eLoadTbl_CylPos_Left = 0,
		eLoadTbl_CylPos_Right,
		eMaxLoadTbl_CylPos,
	};

	enum eTeachPosRotator {
		eROT_POS_UNKOWN = -1,
		eROT_POS_ANGLE_0 = 0,
		eROT_POS_ANGLE_90,
		eROT_POS_ANGLE_180,
		eROT_POS_ANGLE_270,
		eMAX_ROT_POS_COUNT,
	};

	enum eLoadTbl_Lotate_Dir {
		eDir_LoadSite = 0,
		eDir_TestSite
	};

	enum eLoadTbl_Usage {
		eLoadType = 0,
		eUnloadType,

		eMaxTblType
	};
	
	enum eTableRow {
		eRow_Buffer, eRow_LoadUnlaod
	};

	CTaskLoadTable(void);
	~CTaskLoadTable(void);

public:
	int				Initialize(int nTableIdx, char* szDriverFileName );
	void			Finalize();

	void			ThreadProc1();

	int				OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);

	void			OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	// Motor
	int				do_MovePos(int nLoc, int nAxisIdx, int nSpdRate=100, double dOffset=0.0, int nStep = 0);
	int				Interlock_MovePos(int nLoc, int a_nAxisIdx);

    // DIO
	int				ChkDI(int nIndex, BOOL bValue = TRUE);
	BOOL			GetDI(int nIndex);
	int				GetDI(int nIndex, BOOL bValue);

	// 2D
	int				do_cyl_2DBarcode_LeftRight(int nLeftRight);
	int				chk_cyl_2DBarcode_LeftRight(int nLeftRight);
	int				get_cyl_2DBarcode_LeftRight(int nLeftRight); // 현재 사용 안함.

	int				chk_rotate_direction(int nDir);

	// Teaching
	OneAxis*		GetAxis(int nAxisIdx);
	int				GetAxisCount();
	int				GetTeachPos(int nLocStage, double* pdPosY, double* pdRotator);
	double			GetTeachPos(int nLocStage, int nAxisIdx);	
	int				ChkMotorPos(int nLoc, int nAxisIdx=eAxisAll, int nStep =0);
	int				ChkMotorPos(int nLoc, double dOffset, int nAxisIdx=eAxisAll);
	int				WhereIsMotorPos(std::vector<int>& vLocList, int nAxisIdx=eAxisAll);
	int				ChkMoveConflictX(double a_dTPos);
	int				ChkMoveConflictY(double a_dTPos);

	// Reserve Load Tbl
	void			SetWorkReserve(int nRobotID);
	int				GetWorkReserve();

	// Device Cnt or Device Info
	int				GetLoadDeviceCnt();
	int				GetLoadAreaEmptyPocketCnt();
	int				GetUnloadDeviceCnt();
	int				GetUnload2dBarcodeErrDvcCnt();
	int				GetBufferDeviceCnt();
	BOOL			GetBufferPocketCheck(int nPocketX, int nPocketY);
	int				GetBufferDeviceAARetestCnt();
	BOOL			GetTableDeviceInCnt();
	BOOL			GetLoadDeviceRearFrontExist(BOOL bRear);

	int				GetDeviceDataExist(int nX, int nY, int* pnExist);
	int				SetDeviceDataExist(int nX, int nY, int nExist);
	BOOL			IsInitNeed();

	void			SetMoveSetPlateTm(CTime cTm);
	CTime			GetMoveSetplateTm();

	CString			GetLocName(int nLoc);
	CString			GetDirName(int nLoc);

	int				GetTableUsage();

	int				LoadTblFromTestPPToIdx(int nLoadTblIdx);

	//Trigger
	void			InitialTrigger();
	BOOL			IsTriggerInitialize() { return m_bTriggerInitial; };

	//Test PP wait pos
	void			SetTestppPrePos(int nJogIdx);
	int				GetTestppPrePos();

	//Load Table Buffer 사용 Flag
	void			SetFlagLoadTblBuff(BOOL TR_FL);
	BOOL			GetFlagLoadTblBuff();

	void			SetTableCurPos(int nCurPos);
	int				GetTableCurPos();

	//Grr
	int				GrrSetTableIdx(int nJobIdx);
	void			GrrConvertWorkPkrToPocket(int& nPocketX, int& nPocketY);
	BOOL			ChkGrrTestEnd(int nStationIdx);

	int				do_SideVibratorOnOff(int nOnOff);
	int				chk_cyl_vibrator_onoff(int nOnOff);


	//Tray Site에서 Table Step Move 사용.
	void			SetTableRowStep(int nStep);
	int				GetTableRowStep();

	void			SetTableColStep(int nStep);
	int				GetTableColStep();

	//ChkTrayExist_TransferArea_Sen
	BOOL			IsTableStackerImpactArea();

	// Table Vaccum Check
	int				DM_StartAddress();
	void			Set_Table_DM_Vacuum_Check(int nX, int nY, BOOL bOnOff);
	void			SetTableVacuumCheckRowStep(int nStep);
	BOOL			Get_Table_DM_Vacuum_Check(int nX, int nY);
	BOOL			Get_Table_DM_Vacuum_All_Check();
	BOOL			TableVacuumCheckMem();
	void			SetPocketLocNum(_eCw_CCw eCW);

	std::vector<std::pair<CPoint, CPoint>>	
					GetPocketLocNum();

	void			MakeLog(LPCTSTR fmt, ...);

public:
// 제어 변수
	BOOL								m_bInitialized;
	BOOL								m_bTriggerInitial;
	BOOL								m_bLoadTableSkip;
	BOOL								m_bLoadTableAlarmMove;

	int									m_nWorkReserve;

	// Physical Information
	int									m_nTableIdx;	 // 0=LoadTbl1, 1=LoadTbl2
	std::vector<OneAxis*>				m_vMotor;       // Motor Pointer list x, y (2번,3번 테이블은 Y축만 들어간다.)
	double								m_dMotorErrRange[eMaxAxisCount];

    std::vector<int>					m_vX;           // DI id list
	std::vector<int>					m_vY;           // DO id list

    CMap<int,int,CString,CString>		m_mapDI;
	CMap<int,int,CString,CString>		m_mapDO;
	CMap<int,int,CString,CString>		m_mapMotor;

	stMatrix<char>						m_LoadTbl_pocket_setting;

private:
	int									m_nTableRowStep;
	int									m_nTableColStep;

	int									m_nTestPPPrePos;
	int									m_nTblCurPos;

	int									m_nLoadUnloadTablePocket[2];

	BOOL								m_bLoadTblBuffUse;

	CTime								m_cTmMoveTime;

	std::vector<_tVisionReqData>		m_vRec2d;

	CCriticalSection					m_cs;

	std::vector<std::pair<CPoint, CPoint>> 
										m_vPocketLocChange;
	

//VAT
public:
	int do_VATMovePos(int nAxisIdx, double dTargetPos, double dOffset = 0.0);

	int ChkVATMotorPos(int nAxisIdx, double dPos);

	void MakeVatLog(LPCTSTR fmt, ...);
};

