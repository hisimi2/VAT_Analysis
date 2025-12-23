#include "StdAfx.h"

CVatHwDesnDim g_VatHW;
CVatPcd g_VatPcd;
CVatBacklash g_VatBacklash;
//CVatRepeatability g_VatRepeatability;
//CVatAccuracy g_VatAccuracy;


#define read_ini_str GetPrivateProfileString
#define write_ini WritePrivateProfileString

int _parse_xyz( CPointF& coord, char *str );
CString _get_hw_file_name();



CVatHwDesnDim::CVatHwDesnDim()
{
	for (int i = 0; i < MAX_TEST_PP_CNT; i++)
	{
		for (int h = 0; h < eMaxTestPPTypeCount; h++)
		{
			testpp[i].testpp_index = i;
			testpp[i].testpp_handType = h;
		}
	}		
}

void CVatHwDesnDim::Load()
{
	trayloadpp.Load();
	trayunloadpp.Load();
	for (int i = 0; i < MAX_TEST_PP_CNT; i++)
	{
		testpp[i].Load();
	}
}

void CVatHwDesnDim::Save()
{

}




CPointF _read_ini_xyz(LPCSTR fnm, LPCSTR sect, LPCSTR fmt, ...)
{
	CString item;
	char* pszItemName = item.GetBuffer( 256 );
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(pszItemName, fmt, argList);
		va_end(argList);
	}
	item.ReleaseBuffer();

	CPointF xyz;
	char v[256];
	
	::read_ini_str( sect, item, "0,0,0", v, 255, fnm );
	_parse_xyz( xyz, v );
	
	return xyz;
}
CString _read_ini_str(LPCSTR fnm, LPCSTR sect, LPCSTR fmt, ...)
{
	CString item;
	char* pszItemName = item.GetBuffer( 256 );
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(pszItemName, fmt, argList);
		va_end(argList);
	}
	item.ReleaseBuffer();

	CPointF xyz;
	char v[256];
	::read_ini_str( sect, item, "1"/*"0"*/, v, 255, fnm );
	return CString(v);
}
void _write_ini(LPCSTR fnm, LPCSTR sect, CString& v, LPCSTR fmt, ...)
{
	CString item;
	char* pszItemName = item.GetBuffer( 256 );
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(pszItemName, fmt, argList);
		va_end(argList);
	}
	item.ReleaseBuffer();

	::write_ini( sect, item, v, fnm );
}

void CVatHwDesnDim::TrayLoadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();

	m_WideCamPos.CamPos		= _read_ini_xyz( fnm, "VAT Tray Load PP", "WebCam_Wide_XYZ" );
	m_WideCamPos.CamAdjOfs	= _read_ini_xyz( fnm, "VAT Tray Load PP", "WebCam_Wide_FineAdj" );
	m_NarwCamPos.CamPos		= _read_ini_xyz( fnm, "VAT Tray Load PP", "WebCam_Narrow_XYZ" );
	m_NarwCamPos.CamAdjOfs	= _read_ini_xyz( fnm, "VAT Tray Load PP", "WebCam_Narrow_FineAdj" );
																 
	m_WideCamPos.Pitch		= _read_ini_xyz( fnm, "VAT Tray Load PP", "PitchXY_Wide_mm" );
	m_WideCamPos.PitchEnc	= _read_ini_xyz( fnm, "VAT Tray Load PP", "PitchXY_Wide_Encoder" );
	m_NarwCamPos.Pitch		= _read_ini_xyz( fnm, "VAT Tray Load PP", "PitchXY_Narrow_mm" );
	m_NarwCamPos.PitchEnc	= _read_ini_xyz( fnm, "VAT Tray Load PP", "PitchXY_Narrow_Encoder" );

	CString item;
	for(int iy=0; iy<MAX_TRAY_PP_CNT_Y; iy++){
		for(int ix=0; ix<MAX_TRAY_PP_CNT_X; ix++){
			int px = (ix+1), py = (iy+1);
			arrWidPkrOfs[iy][ix] = _read_ini_xyz( fnm, "VAT Tray Load PP", "Picker_Wide_%d%d", px, py );
			arrNrwPkrOfs[iy][ix] = _read_ini_xyz( fnm, "VAT Tray Load PP", "Picker_Narrow_%d%d", px, py );
		}
	}

	//setplate_z	= _ttof( _read_ini_str( fnm, "VAT Tray PP", "Setplate_Z" ));
	//ld_tbl_z	= _ttof( _read_ini_str( fnm, "VAT Tray PP", "Load_table_Z" ));
	buff_z		= _ttof( _read_ini_str( fnm, "VAT Tray Load PP", "Buffer_Z" ));

	for(int i=0; i<MAX_SETPLATE; i++) {
		int setplt_no = (i+1);
		setplate_org[ i ][ eOrgIdxOfsWCam ] = _read_ini_xyz( fnm, "VAT Tray Load PP", "SETPLATE_%d_WCamOffset", setplt_no );
		setplate_org[ i ][ eOrgIdxOrigin  ] = _read_ini_xyz( fnm, "VAT Tray Load PP", "SETPLATE_%d_Origin", setplt_no );
		setplate_z[i]	= _ttof( _read_ini_str( fnm, "VAT Tray Load PP", "SETPLATE_%d_Z",setplt_no ));
	}

	CPointF ldtble_div = _read_ini_xyz( fnm, "VAT Tray Load PP", "LD_TBL_DIV" );
	ld_tbl_div.x = (int)ld_tbl_div.x;
	ld_tbl_div.y = (int)ld_tbl_div.y;

	for(int i=0; i<MAX_LD_TBL_CNT; i++){
		int ld_no = (i+1);
		ld_tbl_org[ i ][ eOrgIdxOfsWCam	] = _read_ini_xyz( fnm, "VAT Tray Load PP", "LD_TBL_%d_WCamOffset", ld_no );
		ld_tbl_org[ i ][ eOrgIdxOrigin 	] = _read_ini_xyz( fnm, "VAT Tray Load PP", "LD_TBL_%d_Origin", ld_no );
		ld_tbl_z[i]	= _ttof( _read_ini_str( fnm, "VAT Tray Load PP", "LD_TBL_%d_Z",ld_no ));
	}

	buff_org[ eOrgIdxOfsWCam ] = _read_ini_xyz( fnm, "VAT Tray Load PP", "BUFF_TBL_WCamOffset");
	buff_org[ eOrgIdxOrigin  ] = _read_ini_xyz( fnm, "VAT Tray Load PP", "BUFF_TBL_Origin");
	

}

void CVatHwDesnDim::TrayLoadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CamPitchPos* p;
	p = &m_WideCamPos;
	_write_ini( fnm, "VAT Tray Load PP", p->CamPos	.ToStr(CPointF::XYZ), "WebCam_Wide_XYZ" );
	_write_ini( fnm, "VAT Tray Load PP", p->CamAdjOfs.ToStr(CPointF::XYZ), "WebCam_Wide_FineAdj" );
	_write_ini( fnm, "VAT Tray Load PP", p->Pitch	.ToStr(CPointF::XY)	, "PitchXY_Wide_mm" );
	_write_ini( fnm, "VAT Tray Load PP", p->PitchEnc	.ToStr(CPointF::XY)	, "PitchXY_Wide_Encoder" );
	p = &m_NarwCamPos;			    
	_write_ini( fnm, "VAT Tray Load PP", p->CamPos	.ToStr(CPointF::XYZ), "WebCam_Narrow_XYZ" );
	_write_ini( fnm, "VAT Tray Load PP", p->CamAdjOfs.ToStr(CPointF::XYZ), "WebCam_Narrow_FineAdj" );
	_write_ini( fnm, "VAT Tray Load PP", p->Pitch	.ToStr(CPointF::XY)	, "PitchXY_Narrow_mm" );
	_write_ini( fnm, "VAT Tray Load PP", p->PitchEnc	.ToStr(CPointF::XY)	, "PitchXY_Narrow_Encoder" );

	CString item;
	for(int iy=0; iy<MAX_TRAY_PP_CNT_Y; iy++){
		for(int ix=0; ix<MAX_TRAY_PP_CNT_X; ix++){
			int px = (ix+1), py = (iy+1);
			_write_ini( fnm, "VAT Tray Load PP", arrWidPkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Wide_%d%d", px, py );
			_write_ini( fnm, "VAT Tray Load PP", arrNrwPkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Narrow_%d%d", px, py );
		}
	}

	CString v;
	//v.Format("%f",setplate_z);	_write_ini( fnm, "VAT Tray PP", v, "Setplate_Z" );
	//v.Format("%f",ld_tbl_z	);	_write_ini( fnm, "VAT Tray PP", v, "Load_table_Z" );
	v.Format("%f",buff_z	);	_write_ini( fnm, "VAT Tray Load PP", v, "Buffer_Z" );

	for(int i=0; i<MAX_SETPLATE; i++) {
		int setplt_no = (i+1);
		_write_ini( fnm, "VAT Tray Load PP", setplate_org[ i ][ eOrgIdxOfsWCam  ].ToStr(CPointF::XY), "SETPLATE_%d_WCamOffset", setplt_no );
		_write_ini( fnm, "VAT Tray Load PP", setplate_org[ i ][ eOrgIdxOrigin ].ToStr(CPointF::XY), "SETPLATE_%d_Origin", setplt_no );
		v.Format("%f",setplate_z[i]);	_write_ini( fnm, "VAT Tray LoadPP", v, "SETPLATE_%d_Z",setplt_no );
	}

	_write_ini( fnm, "VAT Tray Load PP", CPointF(ld_tbl_div.x, ld_tbl_div.y).ToStr(CPointF::XY), "LD_TBL_DIV" );

	for(int i=0; i<MAX_LD_TBL_CNT; i++){
		int ld_no = (i+1);
		_write_ini( fnm, "VAT Tray Load PP", ld_tbl_org[ i ][ eOrgIdxOfsWCam  ].ToStr(CPointF::XY), "LD_TBL_%d_WCamOffset"	  , ld_no );
		_write_ini( fnm, "VAT Tray Load PP", ld_tbl_org[ i ][ eOrgIdxOrigin ].ToStr(CPointF::XY), "LD_TBL_%d_Origin", ld_no );
		v.Format("%f",ld_tbl_z[i]	);	_write_ini( fnm, "VAT Tray PP", v, "LD_TBL_%d_Z",ld_no );
	}

	_write_ini( fnm, "VAT Tray Load PP", buff_org[ eOrgIdxOfsWCam  ].ToStr(CPointF::XY), "BUFF_TBL_WCamOffset"	);
	_write_ini( fnm, "VAT Tray Load PP", buff_org[ eOrgIdxOrigin ].ToStr(CPointF::XY), "BUFF_TBL_Origin" );
}


void CVatHwDesnDim::TrayUnloadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();

	m_WideCamPos.CamPos = _read_ini_xyz(fnm, "VAT Tray Unload PP", "WebCam_Wide_XYZ");
	m_WideCamPos.CamAdjOfs = _read_ini_xyz(fnm, "VAT Tray Unload PP", "WebCam_Wide_FineAdj");
	m_NarwCamPos.CamPos = _read_ini_xyz(fnm, "VAT Tray Unload PP", "WebCam_Narrow_XYZ");
	m_NarwCamPos.CamAdjOfs = _read_ini_xyz(fnm, "VAT Tray Unload PP", "WebCam_Narrow_FineAdj");

	m_WideCamPos.Pitch = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PitchXY_Wide_mm");
	m_WideCamPos.PitchEnc = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PitchXY_Wide_Encoder");
	m_NarwCamPos.Pitch = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PitchXY_Narrow_mm");
	m_NarwCamPos.PitchEnc = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PitchXY_Narrow_Encoder");

	CString item;
	for (int iy = 0; iy < MAX_TRAY_PP_CNT_Y; iy++) {
		for (int ix = 0; ix < MAX_TRAY_PP_CNT_X; ix++) {
			int px = (ix + 1), py = (iy + 1);
			arrWidPkrOfs[iy][ix] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Picker_Wide_%d%d", px, py);
			arrNrwPkrOfs[iy][ix] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Picker_Narrow_%d%d", px, py);
		}
	}

	//setplate_z	= _ttof( _read_ini_str( fnm, "VAT Tray PP", "Setplate_Z" ));
	//ld_tbl_z	= _ttof( _read_ini_str( fnm, "VAT Tray PP", "Load_table_Z" ));
	buff_z = _ttof(_read_ini_str(fnm, "VAT Tray Unload PP", "Buffer_Z"));

	for (int i = 0; i < MAX_SETPLATE; i++) {
		int setplt_no = (i + 1);
		setplate_org[i][eOrgIdxOfsWCam] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "SETPLATE_%d_WCamOffset", setplt_no);
		setplate_org[i][eOrgIdxOrigin] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "SETPLATE_%d_Origin", setplt_no);
		setplate_z[i] = _ttof(_read_ini_str(fnm, "VAT Tray Unload PP", "SETPLATE_%d_Z", setplt_no));
	}

	CPointF ldtble_div = _read_ini_xyz(fnm, "VAT Tray Unload PP", "LD_TBL_DIV");
	ld_tbl_div.x = (int)ld_tbl_div.x;
	ld_tbl_div.y = (int)ld_tbl_div.y;

	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int ld_no = (i + 1);
		ld_tbl_org[i][eOrgIdxOfsWCam] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "LD_TBL_%d_WCamOffset", ld_no);
		ld_tbl_org[i][eOrgIdxOrigin] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "LD_TBL_%d_Origin", ld_no);
		ld_tbl_z[i] = _ttof(_read_ini_str(fnm, "VAT Tray Unload PP", "LD_TBL_%d_Z", ld_no));
	}

	buff_org[eOrgIdxOfsWCam] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "BUFF_TBL_WCamOffset");
	buff_org[eOrgIdxOrigin] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "BUFF_TBL_Origin");


}

void CVatHwDesnDim::TrayUnloadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CamPitchPos* p;
	p = &m_WideCamPos;
	_write_ini(fnm, "VAT Tray Unload PP", p->CamPos.ToStr(CPointF::XYZ), "WebCam_Wide_XYZ");
	_write_ini(fnm, "VAT Tray Unload PP", p->CamAdjOfs.ToStr(CPointF::XYZ), "WebCam_Wide_FineAdj");
	_write_ini(fnm, "VAT Tray Unload PP", p->Pitch.ToStr(CPointF::XY), "PitchXY_Wide_mm");
	_write_ini(fnm, "VAT Tray Unload PP", p->PitchEnc.ToStr(CPointF::XY), "PitchXY_Wide_Encoder");
	p = &m_NarwCamPos;				 
	_write_ini(fnm, "VAT Tray Unload PP", p->CamPos.ToStr(CPointF::XYZ), "WebCam_Narrow_XYZ");
	_write_ini(fnm, "VAT Tray Unload PP", p->CamAdjOfs.ToStr(CPointF::XYZ), "WebCam_Narrow_FineAdj");
	_write_ini(fnm, "VAT Tray Unload PP", p->Pitch.ToStr(CPointF::XY), "PitchXY_Narrow_mm");
	_write_ini(fnm, "VAT Tray Unload PP", p->PitchEnc.ToStr(CPointF::XY), "PitchXY_Narrow_Encoder");

	CString item;
	for (int iy = 0; iy < MAX_TRAY_PP_CNT_Y; iy++) {
		for (int ix = 0; ix < MAX_TRAY_PP_CNT_X; ix++) {
			int px = (ix + 1), py = (iy + 1);
			_write_ini(fnm, "VAT Tray Unload PP", arrWidPkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Wide_%d%d", px, py);
			_write_ini(fnm, "VAT Tray Unload PP", arrNrwPkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Narrow_%d%d", px, py);
		}
	}

	CString v;
	//v.Format("%f",setplate_z);	_write_ini( fnm, "VAT Tray PP", v, "Setplate_Z" );
	//v.Format("%f",ld_tbl_z	);	_write_ini( fnm, "VAT Tray PP", v, "Load_table_Z" );
	v.Format("%f", buff_z);	_write_ini(fnm, "VAT Tray Unload PP", v, "Buffer_Z");

	for (int i = 0; i < MAX_SETPLATE; i++) {
		int setplt_no = (i + 1);
		_write_ini(fnm, "VAT Tray Unload PP", setplate_org[i][eOrgIdxOfsWCam].ToStr(CPointF::XY), "SETPLATE_%d_WCamOffset", setplt_no);
		_write_ini(fnm, "VAT Tray Unload PP", setplate_org[i][eOrgIdxOrigin].ToStr(CPointF::XY), "SETPLATE_%d_Origin", setplt_no);
		v.Format("%f", setplate_z[i]);	_write_ini(fnm, "VAT Tray PP", v, "SETPLATE_%d_Z", setplt_no);
	}

	_write_ini(fnm, "VAT Tray Unload PP", CPointF(ld_tbl_div.x, ld_tbl_div.y).ToStr(CPointF::XY), "LD_TBL_DIV");

	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int ld_no = (i + 1);
		_write_ini(fnm, "VAT Tray Unload PP", ld_tbl_org[i][eOrgIdxOfsWCam].ToStr(CPointF::XY), "LD_TBL_%d_WCamOffset", ld_no);
		_write_ini(fnm, "VAT Tray Unload PP", ld_tbl_org[i][eOrgIdxOrigin].ToStr(CPointF::XY), "LD_TBL_%d_Origin", ld_no);
		v.Format("%f", ld_tbl_z[i]);	_write_ini(fnm, "VAT Tray PP", v, "LD_TBL_%d_Z", ld_no);
	}

	_write_ini(fnm, "VAT Tray Unload PP", buff_org[eOrgIdxOfsWCam].ToStr(CPointF::XY), "BUFF_TBL_WCamOffset");
	_write_ini(fnm, "VAT Tray Unload PP", buff_org[eOrgIdxOrigin].ToStr(CPointF::XY), "BUFF_TBL_Origin");
}







/************************************************************************/
/*                                                                      */
/************************************************************************/
static LPSTR pkr_naming[MAX_TEST_PP_CNT_XY] = {"RL","RR","FL","FR" };
static LPSTR site_naming[MAX_TEST_PP_CNT_XY] = {"DL", "DR", "UL", "UR" };

CVatPkg::CVatPkg()
{
	for(int i=0; i<MAX_TEST_PP_CNT; i++)
		testpp[i].testpp_index = i;
}

void CVatHwDesnDim::TestPP::Load()
{
// 	CPointF xyz;
// 	CString fnm = _get_hw_file_name();
// 	CString sect;
// 
// 	sect.Format("VAT Test PP %d", (testpp_index+1));
// 
// 	up_safety_z	= _ttof( _read_ini_str( fnm, sect, "Safety_UP_Z" ));
// 	dn_safety_z = _ttof( _read_ini_str( fnm, sect, "Safety_DN_Z" ));
// 
// 	CamPos		= _read_ini_xyz( fnm, sect, "WebCam_XYZ" );
// 	CamAdjOfs	= _read_ini_xyz( fnm, sect, "WebCam_FineAdj" );
// 
// 	for(int i=0; i<MAX_TEST_PP_CNT_XY; i++){
// 		arrPkrOfs[i] = _read_ini_xyz( fnm, sect, "Picker_%s", pkr_naming[i] );
// 	}
// 	ld_tbl_z	= _ttof( _read_ini_str( fnm, sect, "Load_table_Z" ));
// 	for(int i=0; i<eTestSiteMax; i++){
// 		ASSERT( eTestSiteMax==MAX_TESTSITE_ONE_AREA_PRESS_UNIT );
// 		test_site_z[i] = _ttof( _read_ini_str( fnm, sect, "Test_Site_%s_Z", site_naming[i] ));
// 	}
// 	sock_clean_z= _ttof( _read_ini_str( fnm, sect, "Socket_Clean_Z" ));
// 
// 	ld_tbl_org[ eOrgIdxOfsWCam ] = _read_ini_xyz( fnm, sect, "LD_TBL_WCamOffset" );
// 	ld_tbl_org[ eOrgIdxOrigin  ] = _read_ini_xyz( fnm, sect, "LD_TBL_Origin" );
// 	for(int i=0; i<eTestSiteMax; i++) {
// 		ASSERT( eTestSiteMax==MAX_TESTSITE_ONE_AREA_PRESS_UNIT );
// 		test_site_org[ i ][ eOrgIdxOfsWCam ] = _read_ini_xyz( fnm, sect, "TEST_SITE_%s_WCamOffset", site_naming[i] );
// 		test_site_org[ i ][ eOrgIdxOrigin  ] = _read_ini_xyz( fnm, sect, "TEST_SITE_%s_Origin", site_naming[i] );
// 	}
// 	sock_clean_org[ eOrgIdxOfsWCam ] = _read_ini_xyz( fnm, sect, "SOCKET_CLEAN_WCamOffset" );
// 	sock_clean_org[ eOrgIdxOrigin  ] = _read_ini_xyz( fnm, sect, "SOCKET_CLEAN_Origin" );
}

void CVatHwDesnDim::TestPP::Save()
{
// 	CPointF xyz;
// 	CString fnm = _get_hw_file_name();
// 	CString sect;
// 
// 	sect.Format("VAT Test PP %d", (testpp_index+1));
// 	CString v;
// 
// 	v.Format("%f", up_safety_z); _write_ini( fnm, sect, v, "Safety_UP_Z" );
// 	v.Format("%f", dn_safety_z); _write_ini( fnm, sect, v, "Safety_DN_Z" );
// 
// 	_write_ini( fnm, sect, CamPos	.ToStr(CPointF::XYZ), "WebCam_XYZ" );
// 	_write_ini( fnm, sect, CamAdjOfs.ToStr(CPointF::XYZ), "WebCam_FineAdj" );
// 
// 	for(int i=0; i<MAX_TEST_PP_CNT_XY; i++){
// 		_write_ini( fnm, sect, arrPkrOfs[i].ToStr(CPointF::XY), "Picker_%s", pkr_naming[i] );
// 	}
// 
// 	v.Format("%f", ld_tbl_z); _write_ini( fnm, sect, v, "Load_table_Z" );
// 	for(int i=0; i<eTestSiteMax; i++){
// 		ASSERT( eTestSiteMax==MAX_TESTSITE_ONE_AREA_PRESS_UNIT );
// 		v.Format("%f", test_site_z[i]); _write_ini( fnm, sect, v, "Test_Site_%s_Z", site_naming[i] );
// 	}
// 	v.Format("%f", sock_clean_z); _write_ini( fnm, sect, v, "Socket_Clean_Z" );
// 
// 	_write_ini( fnm, sect, ld_tbl_org[ eOrgIdxOfsWCam ].ToStr(CPointF::XY), "LD_TBL_WCamOffset" );
// 	_write_ini( fnm, sect, ld_tbl_org[ eOrgIdxOrigin  ].ToStr(CPointF::XY), "LD_TBL_Origin" );
// 	for(int i=0; i<eTestSiteMax; i++) {
// 		ASSERT( eTestSiteMax==MAX_TESTSITE_ONE_AREA_PRESS_UNIT );
// 		_write_ini( fnm, sect, test_site_org[ i ][ eOrgIdxOfsWCam ].ToStr(CPointF::XY), "TEST_SITE_%s_WCamOffset", site_naming[i] );
// 		_write_ini( fnm, sect, test_site_org[ i ][ eOrgIdxOrigin  ].ToStr(CPointF::XY), "TEST_SITE_%s_Origin", site_naming[i] );
// 	}
// 	_write_ini( fnm, sect, sock_clean_org[ eOrgIdxOfsWCam ].ToStr(CPointF::XY), "SOCKET_CLEAN_WCamOffset" );
// 	_write_ini( fnm, sect, sock_clean_org[ eOrgIdxOrigin  ].ToStr(CPointF::XY), "SOCKET_CLEAN_Origin" );
}



//////////////////////////////////////////////////////////////////////////
CString _get_hw_file_name()
{
	CString run_dir;
	::GetCurrentDirectory(MAX_PATH, run_dir.GetBuffer(MAX_PATH)); run_dir.ReleaseBuffer();

	CString ini_file;
	ini_file = run_dir + "\\sys_data\\HW_Design_Dimensions.ini";

	return ini_file;
}
CString _get_pkg_tray_file_name()
{
	CString strCokName;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, strCokName.GetBuffer(255), 255);
	strCokName.ReleaseBuffer();

	CString run_dir;
	::GetCurrentDirectory(MAX_PATH, run_dir.GetBuffer(MAX_PATH)); run_dir.ReleaseBuffer();

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\%s\\%s\\%s", (LPCTSTR)run_dir, (LPCTSTR)SZ_HANDLER_RCP_FOLDER, (LPCTSTR)SZ_TRAY_FOLDER, (LPCTSTR)strCokName);
	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir,NULL);
	}
	(strDir += "\\") += SZ_VAT_PKG_FILE_NAME;
	return strDir;
}
int _parse_xyz( CPointF& coord, char *str )
{
	const char* delim = ",";
	char* context = NULL;
	char* tok = NULL;
	int count = 0;

	coord = CPointF(0,0,0);

	tok = strtok_s( str, delim, &context);
	if( tok == NULL ) return count;
	else
	{
		coord.x = _ttof( tok );
		count++;
	}

	tok = strtok_s( context, delim, &context);
	if( tok == NULL ) return count;
	else
	{
		coord.y = _ttof( tok );
		count++;
	}

	tok = strtok_s( context, delim, &context);
	if( tok == NULL ) return count;
	else
	{
		coord.z = _ttof( tok );
		count++;
	}

	return count;
}


void CVatPkg::TrayLoadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_pkg_tray_file_name();
	CString HWfnm  = _get_hw_file_name();

	setplate_z	= _ttof( _read_ini_str( fnm, "VAT Tray Load PP", "Setplate_Z" ));
	ld_tbl_z	= _ttof( _read_ini_str( fnm, "VAT Tray Load PP", "Load_table_Z" ));
	buff_z		= _ttof( _read_ini_str( fnm, "VAT Tray Load PP", "Buffer_Z" ));

	for(int i=0; i<MAX_SETPLATE; i++) {
		int setplt_no = (i+1);
		setplate_xy[ i ][ e3p_LT_Vsn ] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "SETPLATE_%d_L_Top_Vision", setplt_no );
		setplate_xy[ i ][ e3p_RT_Vsn ] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "SETPLATE_%d_R_Top_Vision", setplt_no );
		setplate_xy[ i ][ e3p_LB_Vsn ] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "SETPLATE_%d_L_Btm_Vision", setplt_no );
	}

	CPointF ldtble_div = _read_ini_xyz( fnm, "VAT Tray Load PP", "LD_TBL_DIV" );
	ld_tbl_div.x = (int)ld_tbl_div.x;
	ld_tbl_div.y = (int)ld_tbl_div.y;

	for(int i=0; i<MAX_LD_TBL_CNT; i++){
		int ld_no = (i+1);
		ld_tbl_xy[ i ][ e2p_LT_Vsn ] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "LD_TBL_%d_L_Top_Vision", ld_no );
		ld_tbl_xy[ i ][ e2p_RB_Vsn ] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "LD_TBL_%d_R_Btm_Vision", ld_no );
	}

	buff_xy[ e3p_LT_Vsn	] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "BUFF_TBL_L_Top_Vision" );
	buff_xy[ e3p_RT_Vsn	] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "BUFF_TBL_R_Top_Vision" );
	buff_xy[ e3p_LB_Vsn	] = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "BUFF_TBL_L_Btm_Vision" );

	CString item;
	for(int iy=0; iy<MAX_TRAY_PP_CNT_Y; iy++){
		for(int ix=0; ix<MAX_TRAY_PP_CNT_X; ix++){
			int px = (ix+1), py = (iy+1);
			arrSetplatePkrOfs[iy][ix] = _read_ini_xyz( fnm, "VAT Tray Load PP", "Picker_Setplate_%d%d", px, py );
			arrTablePkrOfs[iy][ix] = _read_ini_xyz( fnm, "VAT Tray Load PP", "Picker_Table_%d%d", px, py );
		}
	}

	setplate_3Point_pitch = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "3Point_Tray_Pitch");
	setplate_3Point_div = _read_ini_xyz( HWfnm, "VAT Tray Load PP", "3Point_Tray_Div");
}

void CVatPkg::TrayLoadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_pkg_tray_file_name();
	CString HWfnm  = _get_hw_file_name();

	CString v;
	v.Format("%f",setplate_z);	_write_ini( fnm, "VAT Tray Load PP", v, "Setplate_Z" );
	v.Format("%f",ld_tbl_z	);	_write_ini( fnm, "VAT Tray Load PP", v, "Load_table_Z" );
	v.Format("%f",buff_z	);	_write_ini( fnm, "VAT Tray Load PP", v, "Buffer_Z" );

	for(int i=0; i<MAX_SETPLATE; i++) {
		int setplt_no = (i+1);
		_write_ini( HWfnm, "VAT Tray Load PP", setplate_xy[ i ][ e3p_LT_Vsn	].ToStr(CPointF::XY), "SETPLATE_%d_L_Top_Vision", setplt_no );
		_write_ini( HWfnm, "VAT Tray Load PP", setplate_xy[ i ][ e3p_RT_Vsn	].ToStr(CPointF::XY), "SETPLATE_%d_R_Top_Vision", setplt_no );
		_write_ini( HWfnm, "VAT Tray Load PP", setplate_xy[ i ][ e3p_LB_Vsn	].ToStr(CPointF::XY), "SETPLATE_%d_L_Btm_Vision", setplt_no );
	}

	_write_ini( fnm, "VAT Tray Load PP", CPointF(ld_tbl_div.x, ld_tbl_div.y).ToStr(CPointF::XY), "LD_TBL_DIV" );

	for(int i=0; i<MAX_LD_TBL_CNT; i++){
		int ld_no = (i+1);
		_write_ini( HWfnm, "VAT Tray Load PP", ld_tbl_xy[ i ][ e2p_LT_Vsn	].ToStr(CPointF::XY), "LD_TBL_%d_L_Top_Vision", ld_no );
		_write_ini( HWfnm, "VAT Tray Load PP", ld_tbl_xy[ i ][ e2p_RB_Vsn	].ToStr(CPointF::XY), "LD_TBL_%d_R_Btm_Vision", ld_no );
	}

	_write_ini( HWfnm, "VAT Tray Load PP", buff_xy[ e3p_LT_Vsn	].ToStr(CPointF::XY), "BUFF_TBL_L_Top_Vision" );
	_write_ini( HWfnm, "VAT Tray Load PP", buff_xy[ e3p_RT_Vsn	].ToStr(CPointF::XY), "BUFF_TBL_R_Top_Vision" );
	_write_ini( HWfnm, "VAT Tray Load PP", buff_xy[ e3p_LB_Vsn	].ToStr(CPointF::XY), "BUFF_TBL_L_Btm_Vision" );
	
	CString item;
	for(int iy=0; iy<MAX_TRAY_PP_CNT_Y; iy++){
		for(int ix=0; ix<MAX_TRAY_PP_CNT_X; ix++){
			int px = (ix+1), py = (iy+1);
			_write_ini( fnm, "VAT Tray Load PP", arrSetplatePkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Setplate_%d%d", px, py );
			_write_ini( fnm, "VAT Tray Load PP", arrTablePkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Table_%d%d", px, py );
		}
	}
	_write_ini( HWfnm, "VAT_Tray Load PP", setplate_3Point_pitch.ToStr(CPointF::XY), "3Point_Tray_Pitch");
	_write_ini( HWfnm, "VAT_Tray Load PP", setplate_3Point_div.ToStr(CPointF::XY), "3Point_Tray_Div");
}

void CVatPkg::TrayUnloadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_pkg_tray_file_name();
	CString HWfnm = _get_hw_file_name();

	setplate_z = _ttof(_read_ini_str(fnm, "VAT Tray Unload PP", "Setplate_Z"));
	ld_tbl_z = _ttof(_read_ini_str(fnm, "VAT Tray Unload PP", "Load_table_Z"));
	buff_z = _ttof(_read_ini_str(fnm, "VAT Tray Unload PP", "Buffer_Z"));

	for (int i = 0; i < MAX_SETPLATE; i++) {
		int setplt_no = (i + 1);
		setplate_xy[i][e3p_LT_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "SETPLATE_%d_L_Top_Vision", setplt_no);
		setplate_xy[i][e3p_RT_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "SETPLATE_%d_R_Top_Vision", setplt_no);
		setplate_xy[i][e3p_LB_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "SETPLATE_%d_L_Btm_Vision", setplt_no);
	}

	CPointF ldtble_div = _read_ini_xyz(fnm, "VAT Tray Unload PP", "LD_TBL_DIV");
	ld_tbl_div.x = (int)ld_tbl_div.x;
	ld_tbl_div.y = (int)ld_tbl_div.y;

	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int ld_no = (i + 1);
		ld_tbl_xy[i][e2p_LT_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "LD_TBL_%d_L_Top_Vision", ld_no);
		ld_tbl_xy[i][e2p_RB_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "LD_TBL_%d_R_Btm_Vision", ld_no);
	}

	buff_xy[e3p_LT_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "BUFF_TBL_L_Top_Vision");
	buff_xy[e3p_RT_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "BUFF_TBL_R_Top_Vision");
	buff_xy[e3p_LB_Vsn] = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "BUFF_TBL_L_Btm_Vision");

	CString item;
	for (int iy = 0; iy < MAX_TRAY_PP_CNT_Y; iy++) {
		for (int ix = 0; ix < MAX_TRAY_PP_CNT_X; ix++) {
			int px = (ix + 1), py = (iy + 1);
			arrSetplatePkrOfs[iy][ix] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Picker_Setplate_%d%d", px, py);
			arrTablePkrOfs[iy][ix] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Picker_Table_%d%d", px, py);
		}
	}

	setplate_3Point_pitch = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "3Point_Tray_Pitch");
	setplate_3Point_div = _read_ini_xyz(HWfnm, "VAT Tray Unload PP", "3Point_Tray_Div");
}

void CVatPkg::TrayUnloadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_pkg_tray_file_name();
	CString HWfnm = _get_hw_file_name();

	CString v;
	v.Format("%f", setplate_z);	_write_ini(fnm, "VAT Tray Unload PP", v, "Setplate_Z");
	v.Format("%f", ld_tbl_z);	_write_ini(fnm, "VAT Tray Unload PP", v, "Load_table_Z");
	v.Format("%f", buff_z);	_write_ini(fnm, "VAT Tray Unload PP", v, "Buffer_Z");

	for (int i = 0; i < MAX_SETPLATE; i++) {
		int setplt_no = (i + 1);
		_write_ini(HWfnm, "VAT Tray Unload PP", setplate_xy[i][e3p_LT_Vsn].ToStr(CPointF::XY), "SETPLATE_%d_L_Top_Vision", setplt_no);
		_write_ini(HWfnm, "VAT Tray Unload PP", setplate_xy[i][e3p_RT_Vsn].ToStr(CPointF::XY), "SETPLATE_%d_R_Top_Vision", setplt_no);
		_write_ini(HWfnm, "VAT Tray Unload PP", setplate_xy[i][e3p_LB_Vsn].ToStr(CPointF::XY), "SETPLATE_%d_L_Btm_Vision", setplt_no);
	}

	_write_ini(fnm, "VAT Tray PP", CPointF(ld_tbl_div.x, ld_tbl_div.y).ToStr(CPointF::XY), "LD_TBL_DIV");

	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int ld_no = (i + 1);
		_write_ini(HWfnm, "VAT Tray Unload PP", ld_tbl_xy[i][e2p_LT_Vsn].ToStr(CPointF::XY), "LD_TBL_%d_L_Top_Vision", ld_no);
		_write_ini(HWfnm, "VAT Tray Unload PP", ld_tbl_xy[i][e2p_RB_Vsn].ToStr(CPointF::XY), "LD_TBL_%d_R_Btm_Vision", ld_no);
	}

	_write_ini(HWfnm, "VAT Tray Unload PP", buff_xy[e3p_LT_Vsn].ToStr(CPointF::XY), "BUFF_TBL_L_Top_Vision");
	_write_ini(HWfnm, "VAT Tray Unload PP", buff_xy[e3p_RT_Vsn].ToStr(CPointF::XY), "BUFF_TBL_R_Top_Vision");
	_write_ini(HWfnm, "VAT Tray Unload PP", buff_xy[e3p_LB_Vsn].ToStr(CPointF::XY), "BUFF_TBL_L_Btm_Vision");

	CString item;
	for (int iy = 0; iy < MAX_TRAY_PP_CNT_Y; iy++) {
		for (int ix = 0; ix < MAX_TRAY_PP_CNT_X; ix++) {
			int px = (ix + 1), py = (iy + 1);
			_write_ini(fnm, "VAT Tray Unload PP", arrSetplatePkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Setplate_%d%d", px, py);
			_write_ini(fnm, "VAT Tray Unload PP", arrTablePkrOfs[iy][ix].ToStr(CPointF::XY), "Picker_Table_%d%d", px, py);
		}
	}
	_write_ini(HWfnm, "VAT_Tray Unload PP", setplate_3Point_pitch.ToStr(CPointF::XY), "3Point_Tray_Pitch");
	_write_ini(HWfnm, "VAT_Tray Unload PP", setplate_3Point_div.ToStr(CPointF::XY), "3Point_Tray_Div");
}

void CVatPkg::TestPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();//_get_pkg_tray_file_name();
	CString sect;

	sect.Format("VAT Test PP %d", (testpp_index+1));

	_write_ini( fnm, sect, ld_tbl_xy[ e2p_LT_Vsn ].ToStr(CPointF::XY), "LD_TBL_L_Top_Vision" );
	_write_ini( fnm, sect, ld_tbl_xy[ e2p_RB_Vsn ].ToStr(CPointF::XY), "LD_TBL_R_Btm_Vision" );

	_write_ini( fnm, sect, sock_clean_xy[ e2p_LT_Vsn ].ToStr(CPointF::XY), "SOCKET_CLEAN_L_Top_Vision" );
	_write_ini( fnm, sect, sock_clean_xy[ e2p_RB_Vsn  ].ToStr(CPointF::XY), "SOCKET_CLEAN_R_Btm_Vision" );
}
void CVatPkg::TestPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();//_get_pkg_tray_file_name();
	CString sect;

	sect.Format("VAT Test PP %d", (testpp_index+1));

	ld_tbl_xy[ e2p_LT_Vsn ] = _read_ini_xyz( fnm, sect, "LD_TBL_L_Top_Vision");
	ld_tbl_xy[ e2p_RB_Vsn ] = _read_ini_xyz( fnm, sect, "LD_TBL_R_Btm_Vision");


	sock_clean_xy[ e2p_LT_Vsn ] = _read_ini_xyz( fnm, sect, "SOCKET_CLEAN_L_Top_Vision" );
	sock_clean_xy[ e2p_RB_Vsn ] = _read_ini_xyz( fnm, sect, "SOCKET_CLEAN_R_Btm_Vision" );
}

void CVatPkg::Save()
{

}

void CVatPkg::Load()
{
	trayloadpp.Load();
	trayunloadpp.Load();
	for(int i=0; i<MAX_TEST_PP_CNT; i++)
		testpp[i].Load();
}

CVatPcd::CVatPcd()
{
	for (int i = 0; i < MAX_TEST_PP_CNT; i++)
	{
		for (int h = 0; h < eMaxTestPPTypeCount; h++)
		{
			testpp[i][h].testpp_index = i;
		}
	}
		
}

void CVatPcd::TrayLoadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString v;

	_write_ini( fnm, "VAT Tray Load PP", m_PcdStartXPos_XYZ.ToStr(CPointF::XYZ), "PCD_X_Start_Pos");
	_write_ini(fnm, "VAT Tray Load PP", m_PcdStartYPos_XYZ.ToStr(CPointF::XYZ), "PCD_Y_Start_Pos");
	_write_ini( fnm, "VAT Tray Load PP", m_PcdOffset_XY.ToStr(CPointF::XY), "PCD_Offset");
}

void CVatPcd::TrayLoadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();

	m_PcdStartXPos_XYZ = _read_ini_xyz( fnm, "VAT Tray Load PP", "PCD_X_Start_Pos");
	m_PcdStartYPos_XYZ = _read_ini_xyz(fnm, "VAT Tray Load PP", "PCD_Y_Start_Pos");
	m_PcdOffset_XY = _read_ini_xyz( fnm, "VAT Tray Load PP", "PCD_Offset");
}

void CVatPcd::TrayUnloadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString v;

	_write_ini(fnm, "VAT Tray Unload PP", m_PcdStartXPos_XYZ.ToStr(CPointF::XYZ), "PCD_X_Start_Pos");
	_write_ini(fnm, "VAT Tray Unload PP", m_PcdStartYPos_XYZ.ToStr(CPointF::XYZ), "PCD_Y_Start_Pos");
	_write_ini(fnm, "VAT Tray Unload PP", m_PcdOffset_XY.ToStr(CPointF::XY), "PCD_Offset");
}

void CVatPcd::TrayUnloadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();

	m_PcdStartXPos_XYZ = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PCD_X_Start_Pos");
	m_PcdStartYPos_XYZ = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PCD_Y_Start_Pos");
	m_PcdOffset_XY = _read_ini_xyz(fnm, "VAT Tray Unload PP", "PCD_Offset");
}

void CVatPcd::TestPP::Save(int nHandType)
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d-%d", (testpp_index+1), nHandType+1);

	_write_ini( fnm, sect, m_PcdStartXPos_XYZ.ToStr(CPointF::XYZ), "PCD_X_Start_Pos" );
	_write_ini( fnm, sect, m_PcdStartYPos_XYZ.ToStr(CPointF::XYZ), "PCD_Y_Start_Pos" );
	_write_ini( fnm, sect, m_PcdOffset_XY.ToStr(CPointF::XY), "PCD_Offset" );
}

void CVatPcd::TestPP::Load(int nHandType)
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d-%d", (testpp_index+1), nHandType+1);

	m_PcdStartXPos_XYZ = _read_ini_xyz( fnm, sect, "PCD_X_Start_Pos");
	m_PcdStartYPos_XYZ = _read_ini_xyz( fnm, sect, "PCD_Y_Start_Pos");
	m_PcdOffset_XY = _read_ini_xyz( fnm, sect, "PCD_Offset");

}

void CVatPcd::TestPP::SafetyZAxisSave()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d", (testpp_index+1));

	CString v;
	v.Format("%f",m_dUpperSafetyPos);	_write_ini( fnm, sect, v, "Safety_UP_Z" );
	v.Format("%f",m_dLowerSafetyPos	);	_write_ini( fnm, sect, v, "Safety_DN_Z" );
}

void CVatPcd::TestPP::SafetyZAxisLoad()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d", (testpp_index+1));

	m_dUpperSafetyPos = _ttof( _read_ini_str( fnm, sect, "Safety_UP_Z" ));
	m_dLowerSafetyPos = _ttof( _read_ini_str( fnm, sect, "Safety_DN_Z" ));
}

void CVatPcd::Save()
{
	trayloadpp.Save();
	trayunloadpp.Save();
	for(int i =0; i<MAX_TEST_PP_CNT; i++){
		for (int h = 0; h < eMaxTestPPTypeCount; h++) {
			testpp[i][h].Save(h);
			testpp[i][h].SafetyZAxisSave();
		}
	}
}

void CVatPcd::Load()
{
	trayloadpp.Load();
	trayunloadpp.Load();
	for(int i =0; i<MAX_TEST_PP_CNT; i++){
		for (int h = 0; h < eMaxTestPPTypeCount; h++) {
			testpp[i][h].Load(h);
			testpp[i][h].SafetyZAxisLoad();
		}
	}
}


// Backlash
CVatBacklash::CVatBacklash()
{
	for (int i = 0; i < MAX_TEST_PP_CNT; i++) {
		for (int h = 0; h < eMaxTestPPTypeCount; h++) {
			testpp[i][h].testpp_index = i;
			testpp[i][h].SafetyZAxisLoad();
		}
	}
}

void CVatBacklash::Save()
{
	trayloadpp.Save();
	trayunloadpp.Save();
	for(int i =0; i<MAX_TEST_PP_CNT; i++){
		for (int h = 0; h < eMaxTestPPTypeCount; h++) {
			testpp[i][h].Save(h);
		}
	}
}

void CVatBacklash::Load()
{
	trayloadpp.Load();
	trayunloadpp.Load();
	for(int i =0; i<MAX_TEST_PP_CNT; i++){
		for (int h = 0; h < eMaxTestPPTypeCount; h++) {
			testpp[i][h].Load(h);
			testpp[i][h].SafetyZAxisLoad();
		}
	}
}

void CVatBacklash::TrayLoadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString v;

	_write_ini( fnm, "VAT Tray Load PP", m_BacklashTargetPos_XYZ.ToStr(CPointF::XYZ), "Backlash_TargetPos");
	_write_ini( fnm, "VAT Tray Load PP", m_BacklashStartPos_XY.ToStr(CPointF::XYZ), "Backlash_StartPos");
	_write_ini( fnm, "VAT Tray Load PP", m_BacklashEndPos_XY.ToStr(CPointF::XYZ), "Backlash_EndPos");

	_write_ini( fnm, "VAT Tray Load PP", m_BacklashOffset_XY[eSpeed_5].ToStr(CPointF::XY), "Backlash_Offset_5");
	for(int i = eSpeed_10; i<eSpeed_MAX; i++){
		_write_ini( fnm, "VAT Tray Load PP", m_BacklashOffset_XY[i].ToStr(CPointF::XY), "Backlash_Offset_%d",i*10);
	}
}

void CVatBacklash::TrayLoadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();

	m_BacklashTargetPos_XYZ = _read_ini_xyz( fnm, "VAT Tray Load PP", "Backlash_TargetPos");
	m_BacklashStartPos_XY = _read_ini_xyz( fnm, "VAT Tray Load PP", "Backlash_StartPos");
	m_BacklashEndPos_XY = _read_ini_xyz( fnm, "VAT Tray Load PP", "Backlash_EndPos");

	m_BacklashOffset_XY[eSpeed_5] = _read_ini_xyz( fnm, "VAT Tray Load PP", "Backlash_Offset_5");
	for(int i = eSpeed_10; i<eSpeed_MAX; i++){
		m_BacklashOffset_XY[i] = _read_ini_xyz( fnm, "VAT Tray Load PP", "Backlash_Offset_%d",i*10);
	}
}

void CVatBacklash::TrayUnloadPP::Save()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString v;

	_write_ini(fnm, "VAT Tray Unload PP", m_BacklashTargetPos_XYZ.ToStr(CPointF::XYZ), "Backlash_TargetPos");
	_write_ini(fnm, "VAT Tray Unload PP", m_BacklashStartPos_XY.ToStr(CPointF::XYZ), "Backlash_StartPos");
	_write_ini(fnm, "VAT Tray Unload PP", m_BacklashEndPos_XY.ToStr(CPointF::XYZ), "Backlash_EndPos");

	_write_ini(fnm, "VAT Tray Unload PP", m_BacklashOffset_XY[eSpeed_5].ToStr(CPointF::XY), "Backlash_Offset_5");
	for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
		_write_ini(fnm, "VAT Tray Unload PP", m_BacklashOffset_XY[i].ToStr(CPointF::XY), "Backlash_Offset_%d", i * 10);
	}
}

void CVatBacklash::TrayUnloadPP::Load()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();

	m_BacklashTargetPos_XYZ = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Backlash_TargetPos");
	m_BacklashStartPos_XY = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Backlash_StartPos");
	m_BacklashEndPos_XY = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Backlash_EndPos");

	m_BacklashOffset_XY[eSpeed_5] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Backlash_Offset_5");
	for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
		m_BacklashOffset_XY[i] = _read_ini_xyz(fnm, "VAT Tray Unload PP", "Backlash_Offset_%d", i * 10);
	}

}

void CVatBacklash::TestPP::Save(int nHandType)
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d-%d", (testpp_index+1), nHandType+1);

	_write_ini( fnm, sect, m_BacklashTargetPos_XYZ.ToStr(CPointF::XYZ), "Backlash_TargetPos" );
	_write_ini( fnm, sect, m_BacklashStartPos_XY.ToStr(CPointF::XYZ), "Backlash_StartPos" );
	_write_ini( fnm, sect, m_BacklashEndPos_XY.ToStr(CPointF::XYZ), "Backlash_EndPos" );

// 	_write_ini(fnm, sect, m_BacklashOffset_X[eSpeed_5].ToStr(CPointF::XY), "Backlash_Offset_5");
// 	for (int nSpeed = eSpeed_10; nSpeed < eSpeed_MAX; nSpeed++)
// 	{
// 		_write_ini(fnm, sect, m_BacklashOffset_X[nSpeed].ToStr(CPointF::XY), "Backlash_Offset_%d", nSpeed * 10);
// 	}
// 	

	_write_ini(fnm, sect, m_BacklashOffset_XY[eSpeed_5].ToStr(CPointF::XY), "Backlash_Offset_5");
	for (int nSpeed = eSpeed_10; nSpeed < eSpeed_MAX; nSpeed++)
	{
		_write_ini(fnm, sect, m_BacklashOffset_XY[nSpeed].ToStr(CPointF::XY), "Backlash_Offset_%d", nSpeed * 10);
	}
	
}

void CVatBacklash::TestPP::Load(int nHandType)
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d-%d", (testpp_index+1), nHandType+1);

	m_BacklashTargetPos_XYZ = _read_ini_xyz( fnm, sect, "Backlash_TargetPos");
	m_BacklashStartPos_XY = _read_ini_xyz( fnm, sect, "Backlash_StartPos");
	m_BacklashEndPos_XY = _read_ini_xyz( fnm, sect, "Backlash_EndPos");

// 	m_BacklashOffset_X[eSpeed_5] = _read_ini_xyz(fnm, sect, "Backlash_Offset_5");
// 	for (int nSpeed = eSpeed_10; nSpeed < eSpeed_MAX; nSpeed++)
// 	{
// 		m_BacklashOffset_X[nSpeed] = _read_ini_xyz(fnm, sect, "Backlash_Offset_%d", nSpeed * 10);
// 	}


	m_BacklashOffset_XY[eSpeed_5] = _read_ini_xyz(fnm, sect, "Backlash_Offset_5");
	for (int nSpeed = eSpeed_10; nSpeed < eSpeed_MAX; nSpeed++)
	{
		m_BacklashOffset_XY[nSpeed] = _read_ini_xyz(fnm, sect, "Backlash_Offset_%d",  nSpeed * 10);
	}
	
}

void CVatBacklash::TestPP::SafetyZAxisLoad()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d", (testpp_index + 1));

	m_dUpperSafetyPos = _ttof(_read_ini_str(fnm, sect, "Safety_UP_Z"));
	m_dLowerSafetyPos = _ttof(_read_ini_str(fnm, sect, "Safety_DN_Z"));
}

void CVatBacklash::TestPP::SafetyZAxisSave()
{
	CPointF xyz;
	CString fnm = _get_hw_file_name();
	CString sect;

	sect.Format("VAT Test PP %d", (testpp_index + 1));

	CString v;
	v.Format("%f", m_dUpperSafetyPos);	_write_ini(fnm, sect, v, "Safety_UP_Z");
	v.Format("%f", m_dLowerSafetyPos);	_write_ini(fnm, sect, v, "Safety_DN_Z");
}

//////////////////////////////////////////////////////////

