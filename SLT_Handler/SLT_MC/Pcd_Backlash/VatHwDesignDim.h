#pragma once

#ifndef MAX_TEST_PP_CNT_XY
#define MAX_TEST_PP_CNT_XY	(MAX_TEST_PP_PKR_CNT_X*MAX_TEST_PP_PKR_CNT_Y)
#endif

class CVatHwDesnDim
{
public:
	enum eIdxOriginPoints{
		eOrgIdxOfsWCam=0,
		eOrgIdxOrigin,
		eOrgIdxMaxPoints
	};
	
	CVatHwDesnDim();
public:
	struct TrayLoadPP
	{
		/*camera*/
		struct CamPitchPos {
			CPointF	CamPos;
			CPointF	CamAdjOfs;
			CPointF Pitch;
			CPointF PitchEnc;
		};
		CamPitchPos m_WideCamPos;
		CamPitchPos m_NarwCamPos;

		/*picker*/
		CPointF	arrNrwPkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	//narrow offset
		CPointF arrWidPkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	//wide offset

		/*device hw pos*/
		double setplate_z[MAX_SETPLATE];
		double ld_tbl_z[MAX_LD_TBL_CNT];
		double buff_z;
		CPoint  ld_tbl_div;
		CPointF setplate_org[MAX_SETPLATE][eOrgIdxMaxPoints];	// wide 기준의 HW Org
		CPointF ld_tbl_org[MAX_LD_TBL_CNT][eOrgIdxMaxPoints];	// wide 기준의 HW Org
		CPointF buff_org[eOrgIdxMaxPoints];
		inline TrayLoadPP(){ ZeroMemory( this,sizeof(TrayLoadPP) );};
		void Load();
		void Save();
	};

	struct TrayUnloadPP
	{
		/*camera*/
		struct CamPitchPos {
			CPointF	CamPos;
			CPointF	CamAdjOfs;
			CPointF Pitch;
			CPointF PitchEnc;
		};
		CamPitchPos m_WideCamPos;
		CamPitchPos m_NarwCamPos;

		/*picker*/
		CPointF	arrNrwPkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	//narrow offset
		CPointF arrWidPkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	//wide offset

																	/*device hw pos*/
		double setplate_z[MAX_SETPLATE];
		double ld_tbl_z[MAX_LD_TBL_CNT];
		double buff_z;
		CPoint  ld_tbl_div;
		CPointF setplate_org[MAX_SETPLATE][eOrgIdxMaxPoints];	// wide 기준의 HW Org
		CPointF ld_tbl_org[MAX_LD_TBL_CNT][eOrgIdxMaxPoints];	// wide 기준의 HW Org
		CPointF buff_org[eOrgIdxMaxPoints];
		inline TrayUnloadPP() { ZeroMemory(this, sizeof(TrayUnloadPP)); };
		void Load();
		void Save();
	};

	struct TestPP
	{
		enum eIdxTestSite {
			eTestSite_DL=0,
			eTestSite_DR,
			eTestSite_UL,
			eTestSite_UR,
			eTestSiteMax,
		};

		int testpp_index;
		int testpp_handType;
		/*z safety position*/
		double up_safety_z;
		double dn_safety_z;

		/*camera*/
		CPointF	CamPos;
		CPointF	CamAdjOfs;
		/*picker*/
		CPointF	arrPkrOfs[MAX_TEST_PP_CNT_XY];
		/*device hw pos*/
		double ld_tbl_z;
		double test_site_z[eTestSiteMax];
		double sock_clean_z;

		CPointF ld_tbl_org[eOrgIdxMaxPoints];
		CPointF test_site_org[eTestSiteMax][eOrgIdxMaxPoints];
		CPointF sock_clean_org[eOrgIdxMaxPoints];
		inline TestPP(){ ZeroMemory( this,sizeof(TestPP) ); };
		inline TestPP(int index){ ZeroMemory( this,sizeof(TestPP) ); testpp_index = index; };
		void Load();
		void Save();
	};

	TrayLoadPP trayloadpp;
	TrayUnloadPP trayunloadpp;
	TestPP testpp[MAX_TEST_PP_CNT];
	void Load();
	void Save();
};

/* pakage mode auto teaching data */
class CVatPkg
{
public:
	enum eIdx3Points {
		e3p_LT_Vsn,
		e3p_RT_Vsn,
		e3p_LB_Vsn,
		e3pMaxPoints
	};
	enum eIdx2Points {
		e2p_LT_Vsn,
		e2p_RB_Vsn,
		e2pMaxPoints
	};
	CVatPkg();

public:
	class TrayLoadPP
	{
	public:
		/*device pocket*/
		double setplate_z;
		double ld_tbl_z;
		double buff_z;
		CPoint  ld_tbl_div;
		CPointF setplate_xy[MAX_SETPLATE][e3pMaxPoints];
		CPointF ld_tbl_xy[MAX_LD_TBL_CNT][e2pMaxPoints];
		CPointF buff_xy[e3pMaxPoints];
		CPointF setplate_3Point_pitch;
		CPointF setplate_3Point_div;

		inline TrayLoadPP() { ZeroMemory(this, sizeof(TrayLoadPP)); };
		void Load();
		void Save();

		/*picker*/
		CPointF	arrSetplatePkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	//Setplate Pitch offset
		CPointF arrTablePkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	    //Load Table Pitch offset

	};
	class TrayUnloadPP
	{
	public:
		/*device pocket*/
		double setplate_z;
		double ld_tbl_z;
		double buff_z;
		CPoint  ld_tbl_div;
		CPointF setplate_xy[MAX_SETPLATE][e3pMaxPoints];
		CPointF ld_tbl_xy[MAX_LD_TBL_CNT][e2pMaxPoints];
		CPointF buff_xy[e3pMaxPoints];
		CPointF setplate_3Point_pitch;
		CPointF setplate_3Point_div;

		inline TrayUnloadPP() { ZeroMemory(this, sizeof(TrayUnloadPP)); };
		void Load();
		void Save();

		/*picker*/
		CPointF	arrSetplatePkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	//Setplate Pitch offset
		CPointF arrTablePkrOfs[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];	    //Load Table Pitch offset

	};
public:
	class TestPP
	{
	public:
		int testpp_index;

		/*device pocket*/
		CPointF ld_tbl_xy[e2pMaxPoints];
		CPointF sock_clean_xy[e2pMaxPoints];
		inline TestPP() { ZeroMemory(this, sizeof(TestPP)); };
		inline TestPP(int index) { ZeroMemory(this, sizeof(TestPP)); testpp_index = index; };
		void Save();
		void Load();
	};

	TrayLoadPP trayloadpp;
	TrayUnloadPP trayunloadpp;
	TestPP testpp[MAX_TEST_PP_CNT];
	void Load();
	void Save();
};

class CVatPcd
{
public:
	CVatPcd();
public:
	class TrayLoadPP
	{
	public:
		CPointF m_PcdOffset_XY;
		CPointF m_PcdStartXPos_XYZ;
		CPointF m_PcdStartYPos_XYZ;
		void Save();
		void Load();

		inline TrayLoadPP(){ ZeroMemory( this,sizeof(TrayLoadPP) );};
	};
class TrayUnloadPP
	{
	public:
		CPointF m_PcdOffset_XY;
		CPointF m_PcdStartXPos_XYZ;
		CPointF m_PcdStartYPos_XYZ;
		void Save();
		void Load();

		inline TrayUnloadPP() { ZeroMemory(this, sizeof(TrayUnloadPP)); };
	};
public:
	class TestPP
	{
	public:
		CPointF m_PcdOffset_XY;
		CPointF m_PcdStartXPos_XYZ;
		CPointF m_PcdStartYPos_XYZ;
		double m_dUpperSafetyPos;
		double m_dLowerSafetyPos;

		void Save(int nHandType);
		void Load(int nHandType);
		void SafetyZAxisSave();
		void SafetyZAxisLoad();

		int testpp_index;
		inline TestPP(){ ZeroMemory( this,sizeof(TestPP) ); };
		inline TestPP(int index){ ZeroMemory( this,sizeof(TestPP) ); testpp_index = index; };
	};

	TrayLoadPP trayloadpp;
	TrayUnloadPP trayunloadpp;
	TestPP testpp[MAX_TEST_PP_CNT][eMaxTestPPTypeCount];
	void Save();
	void Load();
};

class CVatBacklash
{
public:
	CVatBacklash();
public:
	class TrayLoadPP
	{
	public:
		CPointF m_BacklashOffset_XY[eSpeed_MAX];
		CPointF m_BacklashTargetPos_XYZ;
		CPointF m_BacklashStartPos_XY;
		CPointF m_BacklashEndPos_XY;

		void Save();
		void Load();
		
		inline TrayLoadPP(){ ZeroMemory( this,sizeof(TrayLoadPP) );};
	};
	class TrayUnloadPP
	{
	public:
		CPointF m_BacklashOffset_XY[eSpeed_MAX];
		CPointF m_BacklashTargetPos_XYZ;
		CPointF m_BacklashStartPos_XY;
		CPointF m_BacklashEndPos_XY;

		void Save();
		void Load();

		inline TrayUnloadPP() { ZeroMemory(this, sizeof(TrayUnloadPP)); };
	};
public:
	class TestPP
	{
	public:
		//CPointF m_BacklashOffset_X[eSpeed_MAX];
		CPointF m_BacklashOffset_XY[eSpeed_MAX];
		CPointF m_BacklashTargetPos_XYZ;
		CPointF m_BacklashStartPos_XY;
		CPointF m_BacklashEndPos_XY;
		BOOL m_bIsApplyBacklash;
		void Save(int nHandType);
		void Load(int nHandType);
		void SafetyZAxisSave();
		void SafetyZAxisLoad();

		double m_dUpperSafetyPos;
		double m_dLowerSafetyPos;

		int testpp_index;
		inline TestPP(){ ZeroMemory( this,sizeof(TestPP) ); };
		inline TestPP(int index){ ZeroMemory( this,sizeof(TestPP) ); testpp_index = index; };
	};

	TrayLoadPP trayloadpp;
	TrayUnloadPP trayunloadpp;
	TestPP testpp[MAX_TEST_PP_CNT][eMaxTestPPTypeCount];
	void Save();
	void Load();
};

extern CVatHwDesnDim g_VatHW;
extern CVatPkg g_VatPkg;
extern CVatPcd g_VatPcd;
extern CVatBacklash g_VatBacklash;

