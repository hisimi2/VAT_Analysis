// CBSpeedListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CBSpeedListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCBSpeedListCtrl

CCBSpeedListCtrl::CCBSpeedListCtrl()
{
	m_arLabels.SetSize(100);
	m_LabelCount = 0;
	memset(hColumn, 0, sizeof(hColumn));
}

CCBSpeedListCtrl::~CCBSpeedListCtrl()
{
	m_arLabels.RemoveAll();
}


BEGIN_MESSAGE_MAP(CCBSpeedListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCBSpeedListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBSpeedListCtrl message handlers

int CCBSpeedListCtrl::AddItem(LPCTSTR lpszItem)
{ 
	ASSERT(::IsWindow(m_hWnd)); 
	return InsertItem(LVIF_TEXT, 0, lpszItem, 0, 0, 0, 0); 
}

int CCBSpeedListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask,	int nImage, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hWnd));

	CLabelItem Label;
	Label.sItem.SetAtGrow(0, lpszItem);

	m_arLabels.SetAtGrow(m_LabelCount++, Label);
	return m_LabelCount-1;
}

void CCBSpeedListCtrl::Complete()
{
	SetItemCountEx(m_LabelCount);
}

BOOL CCBSpeedListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	m_arLabels[nItem].sItem.SetAtGrow(nSubItem, lpszText);

	return TRUE;
}

void CCBSpeedListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;
	CLabelItem rLabel = m_arLabels.ElementAt(pItem->iItem);

	if (pItem->mask & LVIF_TEXT)
		lstrcpy(pItem->pszText, rLabel.sItem.GetSize() > pItem->iSubItem ? rLabel.sItem[pItem->iSubItem] : "");

	*pResult = 0;
}

BOOL CCBSpeedListCtrl::DeleteAllItems()
{
	m_arLabels.RemoveAll();
	m_LabelCount = 0;
	SetItemCountEx(m_LabelCount);

	m_arLabels.SetSize(100);
	Invalidate();
	return TRUE;
}

BOOL CCBSpeedListCtrl::DeleteItem(int nItem)
{
	return FALSE;
}

int compare_column = 0;
int __cdecl CompareByLabelNameAsc(const void *elem1, const void *elem2)
{
        CLabelItem *p1 = (CLabelItem*)elem1;
        CLabelItem *p2 = (CLabelItem*)elem2;
        return strcmp(p1->sItem[compare_column],p2->sItem[compare_column]);
}

int __cdecl CompareByLabelNameDesc(const void *elem1, const void *elem2)
{
        CLabelItem *p1 = (CLabelItem*)elem1;
        CLabelItem *p2 = (CLabelItem*)elem2;
        return strcmp(p2->sItem[compare_column], p1->sItem[compare_column]);
}

int __cdecl CompareByLabelNameAsc_Int(const void *elem1, const void *elem2)
{
	CLabelItem *p1 = (CLabelItem*)elem1;
	CLabelItem *p2 = (CLabelItem*)elem2;

	if (atoi(p1->sItem[compare_column]) > atoi(p2->sItem[compare_column])) {
		return 1;
	}
	else {
		return -1;
	}
	
}

int __cdecl CompareByLabelNameDesc_Int(const void *elem1, const void *elem2)
{
	CLabelItem *p1 = (CLabelItem*)elem1;
	CLabelItem *p2 = (CLabelItem*)elem2;

	if (atoi(p2->sItem[compare_column]) > atoi(p1->sItem[compare_column])) {
		return 1;
	}
	else {
		return -1;
	}
	
}

void CCBSpeedListCtrl::SortByCol(const int ColIndex, BOOL bDesc /*= FALSE*/)
{
	compare_column = ColIndex;
	hColumn[ColIndex] = (bDesc == TRUE ? TRUE : !hColumn[ColIndex]);

	::AfxGetApp()->DoWaitCursor(TRUE);
	if(hColumn[ColIndex])
		qsort( static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameAsc );
	else
		qsort( static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameDesc );
	::AfxGetApp()->DoWaitCursor(FALSE);
}

void CCBSpeedListCtrl::SortByCol_Int(const int ColIndex, BOOL bDesc /*= FALSE*/)
{
	compare_column = ColIndex;
	hColumn[ColIndex] = (bDesc == TRUE ? TRUE : !hColumn[ColIndex]);

	::AfxGetApp()->DoWaitCursor(TRUE);
	if (hColumn[ColIndex])
		qsort(static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameDesc_Int);
	else
		qsort(static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameAsc_Int);
	::AfxGetApp()->DoWaitCursor(FALSE);
}


void CCBSpeedListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	LVCOLUMN Col;
	CString ColName;

	Col.mask=LVCF_TEXT;

	if (pNMHDR->idFrom == IDC_LIST_LOG_FREQUENCY &&(pNMListView->iSubItem == 3 || pNMListView->iSubItem == 5)) {
		SortByCol_Int(pNMListView->iSubItem);
	}
	else {
		SortByCol(pNMListView->iSubItem);
	}

	Invalidate();
	*pResult = 0;
}

void CCBSpeedListCtrl::OnNMDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	int nSubItem = pNMItemActivate->iSubItem;

	if (nItem >= 0 && nSubItem >= 0)
	{
		// WPARAM, LPARAM에 포장해서 보낼 수도 있고, 여기서는 lParam에 포장
		LPARAM lParamPacked = (nItem << 16) | (nSubItem & 0xFFFF);

		GetParent()->SendMessage(WM_USER + 100, 0, lParamPacked);
	}

	*pResult = 0;
}

