// DlgSelectItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgSelectItem.h"
#include "afxdialogex.h"


#define FIRST_BTN_RES_ID 100

// CDlgSelectItem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSelectItem, CDialogEx)

CDlgSelectItem::CDlgSelectItem( CWnd* pParent, ItemArray items )
	: CDialogEx(CDlgSelectItem::IDD, pParent)
{
	vecBtns.clear();
	vecItems.clear();
	for( auto i=items.begin(); i!=items.end(); i++){
		vecItems.push_back( CString(i->GetString()) );
	}
	selected_index = -1;
}

CDlgSelectItem::~CDlgSelectItem()
{
	vecItems.clear();
	for( auto btn=vecBtns.begin(); btn!=vecBtns.end(); btn++){
		(*btn)->DestroyWindow();
		delete *btn;
	}
}

void CDlgSelectItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSelectItem, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CDlgSelectItem::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSelectItem 메시지 처리기입니다.


LRESULT CDlgSelectItem::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_COMMAND:
		{
			UINT btn_res_id = wParam;
			if( btn_res_id == IDCANCEL) break;

			selected_index = btn_res_id - FIRST_BTN_RES_ID;

			ASSERT( selected_index >= 0 );
			ASSERT( selected_index < (signed)vecItems.size() );

			EndDialog(IDOK);
			return 0;
		}break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

BOOL CDlgSelectItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const int max_h = 80;
	const int btn_gab = 5;
	int div_h = 0;

	CRect ar_po;
	CRect ar_rect;
	CWnd* pArea = GetDlgItem( IDC_CMD_AREA ); ASSERT(pArea);
	pArea->GetClientRect( &ar_rect );

	//button area & margine
	ar_rect.left	+= 25;
	ar_rect.right	-= 5;
	ar_rect.top		+= 20;
	ar_rect.bottom	-= 10;

	//button height divide by item count
	int total_range = (ar_rect.bottom-ar_rect.top)-(btn_gab*vecItems.size());

	div_h = total_range/vecItems.size();
	if(div_h > max_h) div_h = max_h;

	//first button position
	CRect btn_rect = ar_rect;
	btn_rect.bottom = ar_rect.top + div_h;
	UINT btn_id = FIRST_BTN_RES_ID;
	vecBtns.clear();
	for( auto item=vecItems.begin(); item!=vecItems.end(); item++){
		//dynamic btn creation
		CButton* btn = new CButton();
		btn->Create( *item, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, btn_rect, this, btn_id++ );
		//btn->ShowWindow(SW_SHOW);
		vecBtns.push_back( btn );
		
		//next button position
		btn_rect.top = btn_rect.bottom + btn_gab;
		btn_rect.bottom = btn_rect.top + div_h;
	}

	return TRUE;
}

void CDlgSelectItem::OnClose()
{
	EndDialog( IDCANCEL );
}

CString CDlgSelectItem::GetValue()
{
	return vecItems[selected_index].GetString();
}

int CDlgSelectItem::GetSelIndex()
{
	return selected_index;
}



void CDlgSelectItem::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
