
// SLT_MCDoc.cpp : CSLT_MCDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SLT_MC.h"
#endif

#include "SLT_MCDoc.h"

#include <propkey.h>
#include <Mmsystem.h>
#pragma comment (lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSLT_MCDoc

IMPLEMENT_DYNCREATE(CSLT_MCDoc, CDocument)

BEGIN_MESSAGE_MAP(CSLT_MCDoc, CDocument)
END_MESSAGE_MAP()


// CSLT_MCDoc 생성/소멸

CSLT_MCDoc::CSLT_MCDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CSLT_MCDoc::~CSLT_MCDoc()
{
}

BOOL CSLT_MCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	//타이머 해상도 설정
	TIMECAPS tc;

	//타이머의 최소/최대 해상도를 얻어옵니다
	if( timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR){
		return FALSE;
	}
	m_wTimerRes = min(max(tc.wPeriodMin, TARGET_RESULUTION), tc.wPeriodMax);
	
	//타이머의 최소 해상도를 설정
	if(timeBeginPeriod(m_wTimerRes) != TIMERR_NOERROR){
		return FALSE;
	}

	return TRUE;
}




// CSLT_MCDoc serialization

void CSLT_MCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CSLT_MCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CSLT_MCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSLT_MCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSLT_MCDoc 진단

#ifdef _DEBUG
void CSLT_MCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSLT_MCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSLT_MCDoc 명령

