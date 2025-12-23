// DlgMon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgMon.h"
#include "afxdialogex.h"

// CDlgMon 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMon, CDialogEx)

CDlgMon::CDlgMon(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMon::IDD, pParent)
{
	memset(m_dTestHandOffSetZ, 0x00, sizeof(m_dTestHandOffSetZ));
	m_nX = 0;
	m_nY = 0;
	m_pThreadP1 = NULL;
}

CDlgMon::~CDlgMon()
{
	
}

void CDlgMon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMon, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDlgMon::OnInitDialog()
{
	CDialogEx::OnInitDialog();	

	GetClientRect(&m_bgRect);   // 화면의 크기를 얻는다.
	m_background.LoadBitmap(IDB_MT_BACK);

	m_BtnImg[eMonDlgRect_Transfer].LoadBitmap(IDB_TRANSFER);
	GetBitmapSize(&m_ImgRect[eMonDlgRect_Transfer], &m_BtnImg[eMonDlgRect_Transfer]);
	//m_ImgRect[eMonDlgRect_Transfer].left = 75;
	//m_ImgRect[eMonDlgRect_Transfer].top = 660;

	m_BtnImg[eMonDlgRect_TrayPP].LoadBitmap(IDB_MT_TRAY);
	GetBitmapSize(&m_ImgRect[eMonDlgRect_TrayPP], &m_BtnImg[eMonDlgRect_TrayPP]);
/*	m_ImgRect[eMonDlgRect_TrayPP].left = 70;
	m_ImgRect[eMonDlgRect_TrayPP].top = 530;*/	

	m_BtnImg[eMonDlgRect_LoadTbl1].LoadBitmap(IDB_MT_LDTBL);
	GetBitmapSize(&m_ImgRect[eMonDlgRect_LoadTbl1], &m_BtnImg[eMonDlgRect_LoadTbl1]);
	//m_ImgRect[eMonDlgRect_LoadTbl1].left = 310;
	//m_ImgRect[eMonDlgRect_LoadTbl1].top = 530;

	m_BtnImg[eMonDlgRect_LoadTbl2].LoadBitmap(IDB_MT_LDTBL);
	GetBitmapSize(&m_ImgRect[eMonDlgRect_LoadTbl2], &m_BtnImg[eMonDlgRect_LoadTbl2]);
	//m_ImgRect[eMonDlgRect_LoadTbl2].left = 550;
	//m_ImgRect[eMonDlgRect_LoadTbl2].top = 530;

	for (int i = eMonDlgRect_Press1; i <= eMonDlgRect_Press8; i++)
	{
		m_BtnImg[i].LoadBitmap(IDB_MT_PRESS);
		GetBitmapSize(&m_ImgRect[i], &m_BtnImg[i]);
		m_ImgRect[i].left = 20 + (i- eMonDlgRect_Press1)*127;
		m_ImgRect[i].top = 180;
	}

	for (int i = eMonDlgRect_Press9; i <= eMonDlgRect_Press16; i++)
	{
		m_BtnImg[i].LoadBitmap(IDB_MT_PRESS);
		GetBitmapSize(&m_ImgRect[i], &m_BtnImg[i]);
		m_ImgRect[i].left = 20 + (i - eMonDlgRect_Press9) * 127;
		m_ImgRect[i].top = 40;
	}

	m_BtnImg[eMonDlgRect_TestPP1].LoadBitmap(IDB_MT_TEST);
	GetBitmapSize(&m_ImgRect[eMonDlgRect_TestPP1], &m_BtnImg[eMonDlgRect_TestPP1]);
	//m_ImgRect[eMonDlgRect_TestPP1].left = 310;
	//m_ImgRect[eMonDlgRect_TestPP1].top = 300;

	m_BtnImg[eMonDlgRect_TestPP2].LoadBitmap(IDB_MT_TEST);
	GetBitmapSize(&m_ImgRect[eMonDlgRect_TestPP2], &m_BtnImg[eMonDlgRect_TestPP2]);
	//m_ImgRect[eMonDlgRect_TestPP2].left = 550;
	//m_ImgRect[eMonDlgRect_TestPP2].top = 300;
	
	m_pThreadP1 = AfxBeginThread(DisplayUI, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMon::GetBitmapSize(CRect *prect, CBitmap *pbitmap)
{

	// Bitmap의 속성을 얻는다.
	BITMAP	bmp;
	int	res = pbitmap->GetObject(sizeof(BITMAP), (LPVOID)&bmp);

	prect->right = (int)bmp.bmWidth;  // 그림의 폭 
	prect->bottom = (int)bmp.bmHeight; // 그림의 넓이
}

void CDlgMon::OnDestroy()
{
	CDialogEx::OnDestroy();

	//KillTimer(1);

	int nCount = 0;
	if (m_pThreadP1 != NULL) {
		while (::WaitForSingleObject(m_pThreadP1->m_hThread, 1) == WAIT_TIMEOUT) {
			nCount++;
			if (nCount > 500) {
				::TerminateThread(m_pThreadP1->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadP1 = NULL;

	m_background.DeleteObject();

	for (int i = 0; i < eMonDlgRectMax; i++)
		m_BtnImg[i].DeleteObject();	
}
void CDlgMon::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgMon::OnPaint()
{
	CDialog::OnPaint();
}

void CDlgMon::GetMotorEncoder()
{
	if (IsWindowVisible() != TRUE)
		return;

	m_cPosOffSet[eMonDlgRect_Transfer].x = (int)(g_Motor[eAXIS_TRANSFER_X]->GetMtMonCurPos() / 1000.0);

	for (int i = 0; i < eMaxPressUnitCount / 2; i++)
	{
		m_cPosOffSet[eMonDlgRect_Press1 + i].y = (int)(g_Motor[eAXIS_PRESS_UNIT_DN1 + i]->GetMtMonCurPos() / 1000.0);
		/*m_cPosOffSet[eMonDlgRect_Press9 + i].y = (int)(g_Motor[eAXIS_PRESS_UNIT_UP1 + i]->GetMtMonCurPos() / 1000.0);*/
	}

	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		m_cPosOffSet[eMonDlgRect_LoadTbl1 + i].y = (int)(g_Motor[eAXIS_LOAD_TABLE_1_Y + i]->GetMtMonCurPos() / 1000.0);
	}

	m_cPosOffSet[eMonDlgRect_TestPP1].x = (int)(g_Motor[eAXIS_TEST_PP_1_X]->GetMtMonCurPos() / 1000.0);
	m_cPosOffSet[eMonDlgRect_TestPP1].y = (int)(g_Motor[eAXIS_TEST_PP_1_Y]->GetMtMonCurPos() / 1000.0);
	m_cPosOffSet[eMonDlgRect_TestPP2].x = (int)(g_Motor[eAXIS_TEST_PP_2_X]->GetMtMonCurPos() / 1000.0);
	m_cPosOffSet[eMonDlgRect_TestPP2].y = (int)(g_Motor[eAXIS_TEST_PP_2_Y]->GetMtMonCurPos() / 1000.0);

	m_dTestHandOffSetZ[0] = (int)(g_Motor[eAXIS_TEST_PP_1_Z]->GetMtMonCurPos() / 1000.0);
	m_dTestHandOffSetZ[1] = (int)(g_Motor[eAXIS_TEST_PP_2_Z]->GetMtMonCurPos() / 1000.0);

	m_cPosOffSet[eMonDlgRect_TrayPP].x = (int)(g_Motor[eAXIS_TRAY_PP_1_X]->GetMtMonCurPos() / 1000.0);
	m_cPosOffSet[eMonDlgRect_TrayPP].y = (int)(g_Motor[eAXIS_TRAY_FEEDER_Y]->GetMtMonCurPos() / 1000.0);
}

void CDlgMon::DrawDoalog()
{
	if (IsWindowVisible() != TRUE)
		return;

	CClientDC dc(this);
	CDC *pDC = &dc;

	CDC memDC;

	CBitmap *oldBitmap;
	CDC dmemDC;

	memDC.CreateCompatibleDC(pDC);
	oldBitmap = memDC.SelectObject(&m_background);
	m_background.DeleteObject();
	m_background.LoadBitmap(IDB_MT_BACK);

	dmemDC.CreateCompatibleDC(&memDC);

	int nXpos = 0, nYpos = 0;

	// ========= PRESS
	for (int i = eMonDlgRect_Press1; i <= eMonDlgRect_Press16; i++)
	{
		dmemDC.SelectObject(&m_BtnImg[i]);
		memDC.BitBlt(m_ImgRect[i].left, m_ImgRect[i].top + (int)(m_cPosOffSet[i].y / 2),
			m_ImgRect[i].right, m_ImgRect[i].bottom,
			&dmemDC, 0, 0, SRCCOPY);
	}
	// ========= LOAD TABLE 2
	nXpos = 310;
	nYpos = (int)(530 - (m_cPosOffSet[eMonDlgRect_LoadTbl1].y *0.315));
	dmemDC.SelectObject(&m_BtnImg[eMonDlgRect_LoadTbl1]);
	memDC.BitBlt(m_ImgRect[eMonDlgRect_LoadTbl1].left + nXpos, m_ImgRect[eMonDlgRect_LoadTbl1].top + nYpos,
		m_ImgRect[eMonDlgRect_LoadTbl1].right, m_ImgRect[eMonDlgRect_LoadTbl1].bottom,
		&dmemDC, 0, 0, SRCCOPY);
	// ========= LOAD TABLE 2
	nXpos = 620;
	nYpos = (int)(530 - (m_cPosOffSet[eMonDlgRect_LoadTbl2].y *0.315));
	dmemDC.SelectObject(&m_BtnImg[eMonDlgRect_LoadTbl2]);
	memDC.BitBlt(m_ImgRect[eMonDlgRect_LoadTbl2].left + nXpos, m_ImgRect[eMonDlgRect_LoadTbl2].top + nYpos,
		m_ImgRect[eMonDlgRect_LoadTbl2].right, m_ImgRect[eMonDlgRect_LoadTbl2].bottom,
		&dmemDC, 0, 0, SRCCOPY);

	nXpos = 75 + (int)((m_cPosOffSet[eMonDlgRect_Transfer].x - 100)*0.55);
	nYpos = 660;

	// re 181 372 562 753 943 1134 1325
	// at 120 225 325 430 535 640 745
	dmemDC.SelectObject(&m_BtnImg[eMonDlgRect_Transfer]);
	memDC.BitBlt(m_ImgRect[eMonDlgRect_Transfer].left + nXpos, m_ImgRect[eMonDlgRect_Transfer].top + +nYpos,
		m_ImgRect[eMonDlgRect_Transfer].right, m_ImgRect[eMonDlgRect_Transfer].bottom,
		&dmemDC, 0, 0, SRCCOPY);


	// 44 ~ 1096, 55 ~ 690
	nXpos = (int)( 80 + (m_cPosOffSet[eMonDlgRect_TrayPP].y > -80 ? (205-abs(m_cPosOffSet[eMonDlgRect_TrayPP].y)*2.05) + (int)(m_cPosOffSet[eMonDlgRect_TrayPP].x*0.34) : (int)(m_cPosOffSet[eMonDlgRect_TrayPP].x*0.58) + 35) );
	nYpos = (int)(530 - (m_cPosOffSet[eMonDlgRect_TrayPP].y*0.4));

	dmemDC.SelectObject(&m_BtnImg[eMonDlgRect_TrayPP]);
	memDC.BitBlt(m_ImgRect[eMonDlgRect_TrayPP].left + nXpos, m_ImgRect[eMonDlgRect_TrayPP].top + nYpos,
		m_ImgRect[eMonDlgRect_TrayPP].right, m_ImgRect[eMonDlgRect_TrayPP].bottom,
		&dmemDC, 0, 0, SRCCOPY);

	// ===== TEST HAND 1
	m_cPosOffSet[eMonDlgRect_TestPP1].x = 650;
	m_cPosOffSet[eMonDlgRect_TestPP1].y = 27;
	m_dTestHandOffSetZ[0] = 639;

	nXpos = 20 + (int)((m_cPosOffSet[eMonDlgRect_TestPP1].x) * 0.41);
	nYpos = (int)( 95 + m_dTestHandOffSetZ[0] * 0.235 + (m_cPosOffSet[eMonDlgRect_TestPP1].y < 100 ? 70 : 0) );// -160 + (m_dTestHandOffSetZ[0] * 0.249) + (m_cPosOffSet[eMonDlgRect_TestPP1].y > 100 ? -30 : 30);
	// clean  -285, +40
	// retest -150 +40
	dmemDC.SelectObject(&m_BtnImg[eMonDlgRect_TestPP1]);
	memDC.BitBlt(m_ImgRect[eMonDlgRect_TestPP1].left + nXpos, m_ImgRect[eMonDlgRect_TestPP1].top + nYpos,
		m_ImgRect[eMonDlgRect_TestPP1].right, m_ImgRect[eMonDlgRect_TestPP1].bottom,
		&dmemDC, 0, 0, SRCCOPY);

	// ===== TEST HAND 2
	nXpos = 530 + (int)((m_cPosOffSet[eMonDlgRect_TestPP2].x) * 0.41);//(m_cPosOffSet[eMonDlgRect_TestPP2].y > 50 ? (int)((m_cPosOffSet[eMonDlgRect_TestPP2].x - 24) * 0.41)  : (int)((m_cPosOffSet[eMonDlgRect_TestPP2].x-278) * 0.651));
	nYpos = (int)(95 + m_dTestHandOffSetZ[1]*0.235 + (m_cPosOffSet[eMonDlgRect_TestPP2].y < 100 ? 70 : 0 ));// -160 + (m_dTestHandOffSetZ[1] * 0.249) + (m_cPosOffSet[eMonDlgRect_TestPP2].y > 100 ? -30 : 30);

	dmemDC.SelectObject(&m_BtnImg[eMonDlgRect_TestPP2]);
	memDC.BitBlt(m_ImgRect[eMonDlgRect_TestPP2].left + nXpos, m_ImgRect[eMonDlgRect_TestPP2].top + nYpos,
		m_ImgRect[eMonDlgRect_TestPP2].right, m_ImgRect[eMonDlgRect_TestPP2].bottom,
		&dmemDC, 0, 0, SRCCOPY);

	pDC->BitBlt(0, 0, m_bgRect.Width(), m_bgRect.Height(), &memDC, 0, 0, SRCCOPY);

	dmemDC.DeleteDC();

	//memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();
}

UINT CDlgMon::DisplayUI(LPVOID lpVoid) {
	CDlgMon* pSelf = (CDlgMon*)lpVoid;

	while (1) {
		//CClientDC dc( (CDlgMon&)lpVoid );
		pSelf->GetMotorEncoder();
		pSelf->DrawDoalog();

		Sleep(100);
	}

	return 0;
}
