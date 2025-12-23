// BarcodeMarkSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "BarcodeMarkSetupDlg.h"
#include "afxdialogex.h"


#define _MARK_SPRD_COL	(1)

// CBarcodeMarkSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBarcodeMarkSetupDlg, CDialogEx)

CBarcodeMarkSetupDlg::CBarcodeMarkSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_2D_MARK_SETUP_BIN, pParent)
{
	m_nMaxRow = 0;
}

CBarcodeMarkSetupDlg::~CBarcodeMarkSetupDlg()
{
}

void CBarcodeMarkSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MARK_INSERT, m_BtnInsert);
	DDX_Control(pDX, IDC_BTN_MARK_DEL, m_BtnDelete);
	DDX_Control(pDX, IDC_BTN_MARK_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);

	DDX_Control(pDX, IDC_SPREAD_BARCODE_MARK, m_SprdMark);
}


BEGIN_MESSAGE_MAP(CBarcodeMarkSetupDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MARK_INSERT, &CBarcodeMarkSetupDlg::OnBnClickedBtnMarkInsert)
	ON_BN_CLICKED(IDC_BTN_MARK_DEL, &CBarcodeMarkSetupDlg::OnBnClickedBtnMarkDel)
	ON_BN_CLICKED(IDC_BTN_MARK_SAVE, &CBarcodeMarkSetupDlg::OnBnClickedBtnMarkSave)
END_MESSAGE_MAP()


// CBarcodeMarkSetupDlg 메시지 처리기입니다.

BOOL CBarcodeMarkSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strDeviceName = _T("");
	strDeviceName.Format("%s", m_stMarkData.szDeviceName);

 	CString strTitle = strDeviceName + _T(" Mark Setup");
 	SetWindowText(strTitle);

	OnBtnGUI();

	UpdateSpread();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CBarcodeMarkSetupDlg::OnBtnGUI()
{
	m_BtnInsert.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnInsert.SetIcon(IDI_BTN_NEW);
	m_BtnInsert.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDelete.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnDelete.SetIcon(IDI_BTN_CLEAR);
	m_BtnDelete.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CBarcodeMarkSetupDlg::UpdateSpread()
{
	//Max Row 저장.
	m_nMaxRow = m_stMarkData.v2dMarkList.size();

	m_SprdMark.SetFontBold(TRUE);
	m_SprdMark.SetFontSize(10);
	m_SprdMark.SetTypeHAlign(2); //Center
	m_SprdMark.SetTypeVAlign(2); //Center


	m_SprdMark.SetMaxCols(_MARK_SPRD_COL);
	m_SprdMark.SetMaxRows(m_nMaxRow);

	m_SprdMark.SetTextMatrix(0, _MARK_SPRD_COL, "MARK");
	m_SprdMark.SetRowHeight(0, 14);

	for (int i = 0; i < m_nMaxRow; i++)
	{
		m_SprdMark.SetRowHeight(i + 1, 12);
		m_SprdMark.SetTextMatrix(i + 1, _MARK_SPRD_COL, m_stMarkData.v2dMarkList[i]);
	}

	m_SprdMark.SetTypeCheckTextAlign(1);
}


void CBarcodeMarkSetupDlg::MakeLog(LPCTSTR fmt, ...)
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

void CBarcodeMarkSetupDlg::OnBnClickedBtnMarkInsert()
{
	m_nMaxRow++;

	m_SprdMark.InsertRows(m_nMaxRow, 1); //삽입할 Row 갯수 1ea
	m_SprdMark.SetRowHeight(m_nMaxRow, 12);

	m_SprdMark.SetMaxRows(m_nMaxRow);
	m_SprdMark.SetReDraw(TRUE);
}


void CBarcodeMarkSetupDlg::OnBnClickedBtnMarkDel()
{
	if (m_nMaxRow <= 1)
		return;

	m_nMaxRow--;

	int nSelectSel = m_SprdMark.GetSelBlockRow();
	m_SprdMark.DeleteRows(nSelectSel, 1);

	m_SprdMark.SetMaxRows(m_nMaxRow);
	m_SprdMark.SetReDraw(TRUE);
}


void CBarcodeMarkSetupDlg::OnBnClickedBtnMarkSave()
{
	CString strTmpData = _T("");
	ST_2D_MARK_INFO stTempMarkData;

	int nMaxRow = m_nMaxRow;

	for (int i = 0; i < nMaxRow; i++)
	{
		strTmpData = m_SprdMark.GetTextMatrix(i + 1, _MARK_SPRD_COL);
		strTmpData.Replace(" ", "");

		//Save시, Empty 공간 삭제 및 MaxRow 변경.
		if (strTmpData == _T("")) {
			m_nMaxRow--;
			m_SprdMark.DeleteRows(i+1, 1);
			continue;
		}

		//2d Mark List 저장.
		stTempMarkData.v2dMarkList.push_back(strTmpData);
	}

	//2d Device Name 저장.(외부에서 받아 온 값)
	sprintf_s(stTempMarkData.szDeviceName, sizeof(stTempMarkData.szDeviceName), m_stMarkData.szDeviceName);
	
	//변경 된 Mark Log 기록.
	std::vector<std::pair<CString, std::pair<CString, CString>>> vData;
	m_stMarkData.IsSameBarcodeMarkSetUp(stTempMarkData, vData);

	if (!(vData.empty()))
	{
		BarcodeMarkSetupChangeLog(vData);
	}

	//2d Mark 구조체 File 저장.
	m_stMarkData.Clear();
	m_stMarkData = stTempMarkData;

	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.Save2dMarkInfo(szCokName, &m_stMarkData);




	//Spread Sheet Redraw
	m_SprdMark.SetMaxRows(m_nMaxRow);
	m_SprdMark.SetReDraw(TRUE);
}


void CBarcodeMarkSetupDlg::BarcodeMarkSetupChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < vData.size(); i++)
	{
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[i];

		MakeLog("[MENU = 2d Mark Setup] [Device Name = %s][Mark BEFORE = %s, Mark AFTER = %s]", p.first, p.second.first, p.second.second);

	}
}