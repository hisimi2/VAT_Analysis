// BarcodeValidateSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "BarcodeValidateSetupDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "direct.h"
#include "Register2dDeviceName.h"
#include "BarcodeMarkSetupDlg.h"
// CBarcodeValidateSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBarcodeValidateSetupDlg, CDialogEx)

CBarcodeValidateSetupDlg::CBarcodeValidateSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_2D_BARCODE_VALIDATE, pParent)
{
	m_n2DLength = 0;
	m_n2DMarkStartIdx = 0;
	//m_strCurDeviceName = _T("");
}

CBarcodeValidateSetupDlg::~CBarcodeValidateSetupDlg()
{
}

void CBarcodeValidateSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//GUI Button
	DDX_Control(pDX, IDC_2D_VALIDATE_BTN_NEW, m_BtnNew);
	DDX_Control(pDX, IDC_2D_VALIDATE_BTN_DEL, m_BtnDel);
	DDX_Control(pDX, IDC_2D_VALIDATE_BTN_MODIFY, m_BtnModify);

	DDX_Control(pDX, IDC_2D_VALIDATE_BTN_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);

	//Check Button
	DDX_Control(pDX, IDC_CHECK_2D_BARCODE_USE, m_CkBtn2dBarcode);
	DDX_Control(pDX, IDC_CHECK_DUP_2D_BARCODE, m_CkBtnDuplication);
	DDX_Control(pDX, IDC_CHECK_2D_LENGTH, m_CkBtn2dLength);
	DDX_Control(pDX, IDC_CHECK_2D_BARCODE_MARK, m_CkBtn2dMark);

	//Edit Data
	DDX_Text(pDX, IDC_EDIT_DEVICE_LENGTH, m_n2DLength);
	DDX_Control(pDX, IDC_EDIT_DEVICE_LENGTH, m_Edit2DLength);

	DDX_Control(pDX, IDC_EDIT_DEVICE_NAME, m_EditDeviceName);
	DDX_Text(pDX, IDC_EDIT_DEVICE_MARK_START_IDX, m_n2DMarkStartIdx);
	DDX_Control(pDX, IDC_EDIT_DEVICE_MARK_START_IDX, m_Edit2DMarkStartIdx);

	DDX_Control(pDX, IDC_LIST_DEVICE_NAME, m_ListDeviceName);
	DDX_Control(pDX, IDC_LIST_BARCODE_MARK, m_ListMark);
}


BEGIN_MESSAGE_MAP(CBarcodeValidateSetupDlg, CDialogEx)
	//Check Box Clicked.
	ON_BN_CLICKED(IDC_CHECK_2D_BARCODE_USE, &CBarcodeValidateSetupDlg::OnBnClickedCheck2dBarcodeUse)
	ON_BN_CLICKED(IDC_CHECK_DUP_2D_BARCODE, &CBarcodeValidateSetupDlg::OnBnClickedCheckDup2dBarcode)
	ON_BN_CLICKED(IDC_CHECK_2D_LENGTH, &CBarcodeValidateSetupDlg::OnBnClickedCheck2dLength)
	ON_BN_CLICKED(IDC_CHECK_2D_BARCODE_MARK, &CBarcodeValidateSetupDlg::OnBnClickedCheck2dBarcodeMark)

	//2d Length 입력.
	ON_EN_SETFOCUS(IDC_EDIT_DEVICE_LENGTH, &CBarcodeValidateSetupDlg::OnEnSetfocusEditDeviceLength)

	//2d Mark
	ON_EN_SETFOCUS(IDC_EDIT_DEVICE_MARK_START_IDX, &CBarcodeValidateSetupDlg::OnEnSetfocusEditDeviceMarkStartIdx)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_NAME, &CBarcodeValidateSetupDlg::OnLbnSelchangeListDeviceName)

	ON_BN_CLICKED(IDC_2D_VALIDATE_BTN_NEW, &CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnNew)
	ON_BN_CLICKED(IDC_2D_VALIDATE_BTN_DEL, &CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnDel)
	ON_BN_CLICKED(IDC_2D_VALIDATE_BTN_MODIFY, &CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnModify)
	ON_BN_CLICKED(IDC_2D_VALIDATE_BTN_SAVE, &CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnSave)



	ON_EN_SETFOCUS(IDC_EDIT_DEVICE_NAME, &CBarcodeValidateSetupDlg::OnEnSetfocusEditDeviceName)
END_MESSAGE_MAP()


// CBarcodeValidateSetupDlg 메시지 처리기입니다.

void CBarcodeValidateSetupDlg::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}


BOOL CBarcodeValidateSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitBtnGUI();

	InitChkBtnStatus();

	InitEditDataGUI();
	//현재 Mark 어떤건지 확인 후, setcursel
	//임시
/*	m_ListDeviceName.SetCurSel(0);*/

	

	//Update Device Name List
	UpdateDeviceNameList();

	//현재 Mark 정보 가져옴
	LoadCurMarkInfo();


	//Update Mark List
	m_ListMark.InsertColumn(1, "MARK", 0, 220); //보이지 않음.

	UpdateMarkList();

	return TRUE;
}


void CBarcodeValidateSetupDlg::InitBtnGUI()
{
	//NEW
	m_BtnNew.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnNew.SetIcon(IDI_BTN_NEW);
	m_BtnNew.SetAlign(CButtonST::ST_ALIGN_VERT);

	//DEL
	m_BtnDel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnDel.SetIcon(IDI_BTN_CLEAR);
	m_BtnDel.SetAlign(CButtonST::ST_ALIGN_VERT);

	//MODIFY
	m_BtnModify.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnModify.SetIcon(IDI_BTN_MODIFY);
	m_BtnModify.SetAlign(CButtonST::ST_ALIGN_VERT);

	//SAVE
	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	//CANCEL
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
}


//===========================================================
//COMMENT : Check Box Button 상태 초기화.
//===========================================================
void CBarcodeValidateSetupDlg::InitChkBtnStatus()
{
	BOOL n2dBarocdeUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
	m_CkBtn2dBarcode.SetCheck(n2dBarocdeUse);

	BOOL nDuplicationOptUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_DUPLICATION_OPT_USE);
	m_CkBtnDuplication.SetCheck(nDuplicationOptUse);

	BOOL n2dLengthOptUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_LENGTH_OPT_USE);
	m_CkBtn2dLength.SetCheck(n2dLengthOptUse);

	BOOL n2dMarkOptUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_MARK_OPT_USE);
	m_CkBtn2dMark.SetCheck(n2dMarkOptUse);

	EnableGUI_MainOpt();
}


void CBarcodeValidateSetupDlg::UpdateDeviceNameList()
{
	m_ListDeviceName.ResetContent();

	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	_chdir("SYS_DATA");
	_chdir("HandlerRecipe");
	_chdir(SZ_TRAY_FOLDER);
	_chdir(szCokName);
	_chdir(SZ_2D_MARK_FOLDER);
	CFileFind finder;
	BOOL endf = finder.FindFile();


	//현재 Index 찾는 것.
	int nTmpIndex = 0, nFindIndex = 0;
	char szCurDeviceName[MAX_PATH] = { 0, };
 	g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_DEVICE_NAME, szCurDeviceName, sizeof(szCurDeviceName));
	

	while (endf)
	{
		endf = finder.FindNextFile();
		CString strExt = finder.GetFileName();

		if (!finder.IsDots() && !finder.IsDirectory())
		{
			strExt.Replace(".dat", "");
			m_ListDeviceName.AddString(strExt);

			//현재 Device Name Sel 선택
 			if (strExt.Compare(szCurDeviceName) == 0) {
 				nFindIndex = nTmpIndex;
 			}
 			nTmpIndex++;
		}
		NEXT;
	}

	_chdir("..");
	_chdir("..");
	_chdir("..");
	_chdir("..");
	_chdir("..");


	//초기 아무 것도 선택 하지 않을 경우.
	//Save Device Name Delete 했을 경우. 
	CString strEmpty = _T("");
	if (strEmpty.Compare(szCurDeviceName) == 0) {
		m_ListDeviceName.SetCurSel(-1);
	}
	else {
		m_ListDeviceName.SetCurSel(nFindIndex);
	}	
}


void CBarcodeValidateSetupDlg::UpdateMarkList()
{
	int nMarkCnt = m_stBarcodeMark.v2dMarkList.size();

	for (int i = 0; i < nMarkCnt; i++)
	{
		CString str2dMark = m_stBarcodeMark.v2dMarkList[i];

		m_ListMark.InsertItem(i, str2dMark);

		//열 하나밖에 없어서 사용 필요 없음.
//		m_ListMark.SetItem(i, 1, LVIF_TEXT, str2dMark, 0, 0, 0, NULL);
	}
}


BOOL CBarcodeValidateSetupDlg::IsLotOnGoing()
{
	BOOL bRet = FALSE;
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (bLotState == eHandlerLotMode_OnGoing && nRunMode == eRUN_MODE_ONLINE) {
		AfxMessageBox("Handler is Running!! Please save after lot end");
		bRet = TRUE;
	}
	return bRet;
}

//====================================================================
//COMMENT : 선택 된 Sel 기반으로 Data를 가져온다.
//			-. Sel 선택 할 때, 호출.
//			-. OninitDlg 호출
//			-. Modify 완료 후, 호출(Data 갱신)
//			-. Save 클릭 시, 호출.
//====================================================================
void CBarcodeValidateSetupDlg::LoadCurMarkInfo()
{
	//m_ListDeviceName에 List가 없을 경우, GetCurSel -1이 나올 수 있음.
	int nCurSel = m_ListDeviceName.GetCurSel();
	if (nCurSel == -1) {
		return;
	}

	//COk Name
	char szCokName[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	//Device Name
	char szCurDeviceName[MAX_PATH] = { 0, };

	CString strCurDeviceName = _T("");
	m_ListDeviceName.GetText(nCurSel, strCurDeviceName);
	strCurDeviceName.Replace(" ", "");
	strcpy_s(szCurDeviceName, sizeof(szCurDeviceName), strCurDeviceName);

	//현재 Device Mark 가져 온다.
	ST_2D_MARK_INFO lpstData;
	sprintf_s(lpstData.szDeviceName, sizeof(lpstData.szDeviceName), szCurDeviceName);
	g_COK_FileMgr.Load2dMarkInfo(szCokName, lpstData);

	//m_strCurDeviceName = strCurDeviceName;
	m_stBarcodeMark = lpstData;
}


//===========================================================
//COMMENT : 현재 Selected된 Device Name을 표기함.
//===========================================================
void CBarcodeValidateSetupDlg::DisplayCurDeviceName()
{
	//GetDlgItem(IDC_EDIT_DEVICE_NAME)->SetWindowText(_T(""));

	char szDeviceName[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_DEVICE_NAME, szDeviceName, sizeof(szDeviceName));
	
	CString strDeviceName = (LPCTSTR)szDeviceName;
	strDeviceName.Replace(" ", "");

	if (strDeviceName.Compare(_T("")) == 0)
	{
		strDeviceName = _T("NONE");
	}

	
// 	CString strDisplayEdit = _T("");
// 	strDisplayEdit.Format("Device Name : %s", (LPCTSTR)szDeviceName);

	GetDlgItem(IDC_EDIT_DEVICE_NAME)->SetWindowText(strDeviceName);
	m_EditDeviceName.Invalidate();
	m_EditDeviceName.UpdateWindow();
}

//===========================================================
//COMMENT : 작업자 Data Setting GUI 관련 초기화.
//===========================================================
void CBarcodeValidateSetupDlg::InitEditDataGUI()
{
	//Length 입력 Edit
	m_Edit2DLength.SetBackColor(COLOR_WWHITE);
	m_Edit2DLength.SetTextSize(25);

	int n2DLength = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_2DID_LENGTH);
	CString str2DLength = _T("");
	str2DLength.Format("%d", n2DLength);
	m_Edit2DLength.SetWindowText(str2DLength);

	//2D Mark Device Name
	//m_EditDeviceName.SetReadOnly(TRUE);
	//m_EditDeviceName.SetFont("Fixedsys", 20);

	//m_EditDeviceName.SetTextColor(COLOR_BBLACK);
	//m_EditDeviceName.SetBackColor(COLOR_LLIGHTYELLOW);

	//m_EditDeviceName.SetTextSize(20);
	DisplayCurDeviceName();


	//2D Mark Start Index
	m_Edit2DMarkStartIdx.SetBackColor(COLOR_WWHITE);
	m_Edit2DMarkStartIdx.SetTextSize(25);

	int n2DMarkStartIdx = g_DMCont.m_dmHandlerOpt.GN(NDM11_2D_MARK_START_INDEX);
	CString str2DMarkStartIdx = _T("");
	str2DMarkStartIdx.Format("%d", n2DMarkStartIdx);
	m_Edit2DMarkStartIdx.SetWindowText(str2DMarkStartIdx);
}


void CBarcodeValidateSetupDlg::ChkOptionAuthority(CButton *btnOption)
{
	if (!(btnOption->GetCheck())) {
		// AfxGetMainWnd() 메인 윈도우의 정보 얻어온다.
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		// GetActiveView() 현재 활성화된 창의 정보를 얻어온다.
		CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();

		GetDlgItem(IDCANCEL)->SetFocus();

		int nUserLevel = pSltView->m_nLogInLv;
		int nRetLevel = 0;

		BOOL bRet = g_LibCommCtrl.ShowLogInBox(nUserLevel, &nRetLevel);
		if (RETURN_OK == bRet && nRetLevel >= eUSERLEVEL_ENGINEER) {
			btnOption->SetCheck(eOPTION_NOT_USE);
		}
		else {
			btnOption->SetCheck(eOPTION_USE);
			AfxMessageBox("User level is not apply selection.");
		}
	}
}


void CBarcodeValidateSetupDlg::OnBnClickedCheck2dBarcodeUse()
{
	//Running 중에는 2D Barcode On/Off 불가.	
	if (IsLotOnGoing()) {	
		int nCurStatus = m_CkBtn2dBarcode.GetCheck();
		m_CkBtn2dBarcode.SetCheck(!nCurStatus);
		return;
	}

	ChkOptionAuthority(&m_CkBtn2dBarcode);

	EnableGUI_MainOpt();
}


void CBarcodeValidateSetupDlg::OnBnClickedCheckDup2dBarcode()
{
	ChkOptionAuthority(&m_CkBtnDuplication);

	EnableGUI_SubOpt(_eValidateType_Duplication, m_CkBtnDuplication.GetCheck());
}


void CBarcodeValidateSetupDlg::OnBnClickedCheck2dLength()
{
	ChkOptionAuthority(&m_CkBtn2dLength);

	EnableGUI_SubOpt(_eValidateType_Length, m_CkBtn2dLength.GetCheck());
}


void CBarcodeValidateSetupDlg::OnBnClickedCheck2dBarcodeMark()
{
	ChkOptionAuthority(&m_CkBtn2dMark);

	EnableGUI_SubOpt(_eValidateType_Mark, m_CkBtn2dMark.GetCheck());
}


void CBarcodeValidateSetupDlg::OnEnSetfocusEditDeviceLength()
{
	UpdateData(TRUE);

	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_n2DLength, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Device Length", "50", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DEVICE_LENGTH)->SetWindowText(szRetVal);
		//m_n2DLength = _tstoi(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CBarcodeValidateSetupDlg::OnEnSetfocusEditDeviceMarkStartIdx()
{
	UpdateData(TRUE);

	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_n2DMarkStartIdx, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "2D Barcode Mark Start Index", "50", "0");
	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DEVICE_MARK_START_IDX)->SetWindowText(szRetVal);
		//m_n2DLength = _tstoi(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CBarcodeValidateSetupDlg::OnLbnSelchangeListDeviceName()
{
	m_ListMark.DeleteAllItems();

	LoadCurMarkInfo();

	UpdateMarkList();
}

void CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnNew()
{
	MakeLog("[MENU = 2d Mark] [New Button Click]");

	CRegister2dDeviceName dlg;
	dlg.DoModal();

	UpdateDeviceNameList();
}


void CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnDel()
{
	int nCurSel = m_ListDeviceName.GetCurSel();
	if (nCurSel == -1) {
		AfxMessageBox("Please Select the Device Name First", MB_TOPMOST);
		return;
	}

	int nRetMsg = 0;
	nRetMsg = AfxMessageBox("Do you want to delete?", MB_TOPMOST | MB_YESNO);

	if (nRetMsg != IDYES)
		return;

	char szCokName[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));


	char sz2dMarkPath[2048] = { 0, };
	char szCurrentDir[512] = { 0, };
	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);
	sprintf_s(sz2dMarkPath, sizeof(sz2dMarkPath), "%s\\sys_data\\handlerRecipe\\%s\\%s\\%s\\%s.dat", szCurrentDir, SZ_TRAY_FOLDER, szCokName, SZ_2D_MARK_FOLDER, m_stBarcodeMark.szDeviceName);

	BOOL bDeleteDone = FALSE;
	CFileFind findfile;
	BOOL bFind = findfile.FindFile(sz2dMarkPath);
	while (bFind)
	{
		bFind = findfile.FindNextFile();
		if (findfile.IsDots()) {
			continue;
		}
		else {
			CString strFileName = findfile.GetFilePath();
			CFileStatus status;
			CFile::GetStatus(strFileName, status);
			::DeleteFile(strFileName);
			bDeleteDone = TRUE;
		}

		NEXT;
	}
	findfile.Close();

	if (bDeleteDone == TRUE) {
		MakeLog("[MENU = 2d Mark] [%s Device Name DELETE]", m_stBarcodeMark.szDeviceName);
	}

	CString strEmpty = _T("");

 	char szData[MAX_PATH] = { 0, };
 	g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_DEVICE_NAME, szData, sizeof(szData));

	//현재 저장된 Device Name 삭제 or 초기 Device 선택 안했을 경우.
	if (strcmp(m_stBarcodeMark.szDeviceName, szData) == 0 || strEmpty.Compare(szData) == 0) {
		//구조체 삭제
		m_stBarcodeMark.Clear();

		g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_DEVICE_NAME, m_stBarcodeMark.szDeviceName, sizeof(m_stBarcodeMark.szDeviceName));

		DisplayCurDeviceName();
	}


	//Device Name Reload
	UpdateDeviceNameList();

	//m_stBarcodeMark 구조체 신규 Update
	OnLbnSelchangeListDeviceName();


}


void CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnModify()
{
	MakeLog("[MENU = 2d Mark] [Modify Button Click]");

	//Mark Data Reload
	//LoadCurMarkInfo();
	int nCurSel = m_ListDeviceName.GetCurSel();
	if (nCurSel == -1) {
		AfxMessageBox("Please Select the Device Name First", MB_TOPMOST);
		return;
	}



	CBarcodeMarkSetupDlg DO;
	DO.m_stMarkData = m_stBarcodeMark;
	DO.DoModal();

	
	LoadCurMarkInfo();

	//Mark List Update
	m_ListMark.DeleteAllItems();
	UpdateMarkList();

}



void CBarcodeValidateSetupDlg::OnBnClicked2dValidateBtnSave()
{
	UpdateData(TRUE);

	//==================================================================================공유 메모리 저장
	//2d Barcode Option Check Status
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_BarCode_Use, (BOOL)m_CkBtn2dBarcode.GetCheck());

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_DUPLICATION_OPT_USE, (BOOL)m_CkBtnDuplication.GetCheck());

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_LENGTH_OPT_USE, (BOOL)m_CkBtn2dLength.GetCheck());
	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_2DID_LENGTH, m_n2DLength);


	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_MARK_OPT_USE, (BOOL)m_CkBtn2dMark.GetCheck());
	g_DMCont.m_dmHandlerOpt.SN(NDM11_2D_MARK_START_INDEX, m_n2DMarkStartIdx);


	//Device Name 저장. 
	LoadCurMarkInfo();

	//g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_DEVICE_NAME, m_stBarcodeMark.szDeviceName, sizeof(m_stBarcodeMark.szDeviceName));


	//Marlist 저장.
	//현재 Device Name에 맞는 List ','구분자로 저장.
	CString str2DMark = _T("");

	int nMarkCnt = m_stBarcodeMark.v2dMarkList.size();
	for (int i = 0; i < nMarkCnt; i++) {
		CString temp = m_stBarcodeMark.v2dMarkList[i];
		temp += ",";

		str2DMark += temp;
	}
	str2DMark.Replace(" ", "");

	char szCur2dMark[MAX_PATH] = { 0, };
	sprintf_s(szCur2dMark, sizeof(szCur2dMark), str2DMark);
	//g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_MARK_LIST, szCur2dMark, sizeof(szCur2dMark));

	//Running 중에는 Device Name 및 Mark 변경 불가.
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState != eHandlerLotMode_OnGoing) {
		g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_DEVICE_NAME, m_stBarcodeMark.szDeviceName, sizeof(m_stBarcodeMark.szDeviceName));
		g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_MARK_LIST, szCur2dMark, sizeof(szCur2dMark));
	}

	DisplayCurDeviceName();
	//==================================================================================File Data 저장
	char szCokName[512] = { 0, };
	ST_COK_INFO     stCokData, stBeforeCokData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	stBeforeCokData = stCokData;

	//2d Barcode Option Check Status
	stCokData.b2dBarcodeUse = m_CkBtn2dBarcode.GetCheck();

	stCokData.b2dDuplicateOptUse = m_CkBtnDuplication.GetCheck();

	stCokData.b2dLengthOptUse = m_CkBtn2dLength.GetCheck();
	stCokData.n2dDeviceLength = m_n2DLength;

	stCokData.b2dMarkOptUse = m_CkBtn2dMark.GetCheck();
	stCokData.n2dMarkStartIdx = m_n2DMarkStartIdx;

	//Running 중에는 Device Name 및 Mark 변경 불가.
	bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState != eHandlerLotMode_OnGoing) {
		strcpy_s(stCokData.sz2dDeviceName, m_stBarcodeMark.szDeviceName);
		strcpy_s(stCokData.sz2dMarkList, szCur2dMark);
	}

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);

	//Option Check
	std::vector<std::pair<CString, std::pair<double, double>>> vDataOpt;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	stBeforeCokData.IsSame2dValidateOptUse(stCokData, vDataOpt);

	if (!(vDataOpt.empty()))
	{
		BarcodeValidateOptChangeLog(vDataOpt);
	}

	//Device Name & Mark List Check
	std::vector<std::pair<CString, std::pair<CString, CString>>> vData2dMark;
	stBeforeCokData.IsSame2DMark(stCokData, vData2dMark);
	if (!(vData2dMark.empty()))
	{
		BarcodeMarkChangeLog(vData2dMark);
	}
}


void CBarcodeValidateSetupDlg::BarcodeValidateOptChangeLog(std::vector<std::pair<CString, std::pair<double, double>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++)
	{
		std::pair<CString, std::pair<double, double>> p;

		p = vData[i];

		MakeLog("[MENU = 2d Barcode Validate Check Option, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);

	}
}


void CBarcodeValidateSetupDlg::BarcodeMarkChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++)
	{
		std::pair<CString, std::pair<CString, CString>> p;
		p = vData[i];

		if (p.first == "2D Device Name")
		{
			MakeLog("[MENU = 2d Barcode Device Name Changed [BEFORE=%s, AFTER=%s]", p.second.first, p.second.second);
		}
		else { //"2D Mark List"
			MakeLog("[MENU = 2d Barcode Mark List Changed [BEFORE=%s, AFTER=%s]", p.second.first, p.second.second);
		}
	}
}

void CBarcodeValidateSetupDlg::OnEnSetfocusEditDeviceName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->SetFocus();
}


//=============================================================================
//COMMENT : 2D Barcode Use/No Use에 따라 GUI 활성화 함수.
//=============================================================================
void CBarcodeValidateSetupDlg::EnableGUI_MainOpt()
{
	BOOL bMainChk = m_CkBtn2dBarcode.GetCheck();

	//Validate Option Enable/Disable 처리.
	CButton *btnSubOpt[_eValidateType_Max] = { &m_CkBtnDuplication, &m_CkBtn2dLength, &m_CkBtn2dMark };
	for (int i = 0; i < _countof(btnSubOpt); i++) {
		//2d Barcode Use/No Use에 따라 Sub Option Check 버튼 Enable/Disable 처리.
		btnSubOpt[i]->EnableWindow(bMainChk);


		//Sub Option GUI들 Enable/Disable 처리.
		//bMainChk가 False면 모든 Sub Gui 비활성화.
		//bMainChk가 True면 Sub Option Check에 따라 비활성화/활성화 진행.
		BOOL bSubChk = (bMainChk == TRUE) ? btnSubOpt[i]->GetCheck() : bMainChk;

		EnableGUI_SubOpt(i, bSubChk);
	}
}

//=============================================================================
//COMMENT : Validate Option Use/No Use에 따라 SUB GUI 활성화 함수.
//          -. TRUE : Enable 활성화.
//			-. FALSE : Enable 비활성화.
//=============================================================================
void CBarcodeValidateSetupDlg::EnableGUI_SubOpt(int eType, BOOL bTR_FL)
{
	switch (eType)
	{
	case CBarcodeValidateSetupDlg::_eValidateType_Duplication:
	{
		//현재 Enable 진행 할 GUI 없음.
	}break;
	case CBarcodeValidateSetupDlg::_eValidateType_Length:
	{
		int nId[3] = { IDC_STATIC_SEPARATE_1 , IDC_STATIC_DEVICE_LENGTH , IDC_EDIT_DEVICE_LENGTH };

		for (int i = 0; i < _countof(nId); i++) {
			GetDlgItem(nId[i])->EnableWindow(bTR_FL);
		}
	}break;
	case CBarcodeValidateSetupDlg::_eValidateType_Mark:
	{
		int nId[11] = 
		{ IDC_STATIC_SEPARATE_2 , IDC_STATIC_2D_MARK_START_INDEX , IDC_EDIT_DEVICE_MARK_START_IDX,
		  IDC_STATIC_SEPARATE_3 , IDC_STATIC_2D_MARK_CUR_DEVICE_NAME , IDC_EDIT_DEVICE_NAME ,
		  IDC_LIST_DEVICE_NAME , IDC_LIST_BARCODE_MARK ,
		  IDC_2D_VALIDATE_BTN_NEW , IDC_2D_VALIDATE_BTN_DEL , IDC_2D_VALIDATE_BTN_MODIFY 
		};

		for (int i = 0; i < _countof(nId); i++) {
			GetDlgItem(nId[i])->EnableWindow(bTR_FL);
		}
	}break;
	default:
		break;
	}


}