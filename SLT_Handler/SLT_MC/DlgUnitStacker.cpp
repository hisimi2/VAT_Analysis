// DlgUnitStacker.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgUnitStacker.h"
#include "afxdialogex.h"


// CDlgUnitStacker 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgUnitStacker, CDialogEx)

CDlgUnitStacker::CDlgUnitStacker(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgUnitStacker::IDD, pParent)
{

}

CDlgUnitStacker::~CDlgUnitStacker()
{
}

void CDlgUnitStacker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUnitStacker, CDialogEx)
END_MESSAGE_MAP()


// CDlgUnitStacker 메시지 처리기입니다.
