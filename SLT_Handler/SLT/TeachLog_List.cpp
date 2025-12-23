// TeachLog_List.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TeachLog_List.h"
#include "afxdialogex.h"


// CTeachLog_List 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTeachLog_List, CDialog)

CTeachLog_List::CTeachLog_List(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EVENT_TEACH_LOG, pParent)
{

}

CTeachLog_List::~CTeachLog_List()
{
}

void CTeachLog_List::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTeachLog_List, CDialog)
END_MESSAGE_MAP()


// CTeachLog_List 메시지 처리기입니다.
