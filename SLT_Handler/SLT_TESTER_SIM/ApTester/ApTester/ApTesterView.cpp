
// ApTesterView.cpp : CApTesterView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ApTester.h"
#endif

#include "ApTesterDoc.h"
#include "ApTesterView.h"

#pragma comment(lib, "../Debug/iocp_socket_library.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SWAP16(s) (((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)))  
#define SWAP32(l) (((((l) & 0xff000000) >> 24) | (((l) & 0x00ff0000) >> 8)  | (((l) & 0x0000ff00) << 8)  | (((l) & 0x000000ff) << 24)))


// CApTesterView

IMPLEMENT_DYNCREATE(CApTesterView, CView)

BEGIN_MESSAGE_MAP(CApTesterView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CApTesterView 생성/소멸

CApTesterView::CApTesterView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	bPF = FALSE;
	
}

CApTesterView::~CApTesterView()
{	
	int nCount = 0;
	if (m_pThreadP1 != NULL ) {
		while (::WaitForSingleObject(m_pThreadP1->m_hThread, 1) == WAIT_TIMEOUT) {
			nCount++;
			if (nCount > 500) {
				::TerminateThread(m_pThreadP1->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadP1 = NULL;

	delete m_Socket; /*테스트*/
	delete pDlg; 

	
}

BOOL CApTesterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CApTesterView 그리기

void CApTesterView::OnDraw(CDC* /*pDC*/)
{
	CApTesterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CApTesterView 인쇄

BOOL CApTesterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CApTesterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CApTesterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CApTesterView 진단

#ifdef _DEBUG
void CApTesterView::AssertValid() const
{
	CView::AssertValid();
}

void CApTesterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CApTesterDoc* CApTesterView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CApTesterDoc)));
	return (CApTesterDoc*)m_pDocument;
}
#endif //_DEBUG


void CApTesterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_bDelSockFlag = FALSE;

	char szDriverFileName[1024] = { 0, };
	char szTemp[256] = { 0, };
	// get directory path
	//::GetCurrentDirectory(sizeof(szTemp), szTemp);
	
	GetModuleFileName(NULL, szTemp, MAX_PATH);
	CString strPath(szTemp), strResult;
	strResult = strPath.Left(strPath.ReverseFind('\\'));
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\Setting.ini", strResult);


	CString str, str1, str2;
	str = strPath.Left(strPath.ReverseFind('\\'));
	str1 = str.Right(str.GetLength() - str.ReverseFind('\\') - 1);
	str2 = str1.Mid(5, 2);
	//AfxMessageBox(strPath);

// 	CString str;
// 	str.Format("%s", szDriverFileName);
	//AfxMessageBox(str);

	CConfigData cCfgData(szDriverFileName);
	int nStationNo = /*atoi(str2);*/cCfgData.GetInt("STATION", "NO", "2");
	int nPort = cCfgData.GetInt("HANDLER", "PORT", "8008");

	char szIp[64] = { 0, };
	cCfgData.GetString("HANDLER", "IP", "192.168.30.171", szIp);

	str.Format("%d / %d / %s", nStationNo, nPort, szIp);
	//AfxMessageBox(str);
	m_Socket = new CConectSocket();

	m_Socket->setIp(szIp);
	m_Socket->Initialize(nStationNo);

	m_Socket->Create();
	if (m_Socket->Connect(szIp, nPort) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		PostQuitMessage(0);
		return;
	}
	
	m_Socket->setConnect(TRUE);
	int nStat = m_Socket->GetState();

	// REG
	BYTE sendDataReg[MAX_MSG_ID + REG_STR + 4] = { 0, };
	m_Socket->MakeSendData(sendDataReg, TESTER_REG, TESTER_REG);
	m_Socket->Send(sendDataReg, (MAX_MSG_ID + REG_STR + 4));

	m_pThreadP1 = AfxBeginThread(TesterIF, this);

	m_pThreadP2 = AfxBeginThread(TesterIF2, this);

	// IDLE STATE 20230831 hyb 바로 송신 하지 않는다.
	//BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
	//m_Socket->MakeSendData(sendDataState, TESTER_STAUS, STATUS_IDLE);
	//m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));

	// HEART BEAT heart beat 사용 안함.
	//BYTE sendDataHeartBeat[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX] = { 0, };
	//m_Socket->MakeSendData(sendDataHeartBeat, HEART_BEAT);
	//m_Socket->Send(sendDataHeartBeat, (MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX));
	
	createDlg();

}


UINT CApTesterView::TesterIF(LPVOID lpVoid) 
{
	CApTesterView* pSelf = (CApTesterView*)lpVoid;

	while (1) {

	 int nAction = pSelf->m_Socket->GetAction();
		 
		switch (nAction)
		{
// 		case HEART_BEAT:
// 			{
// 				pSelf->SetTimer(0, 3000, NULL);
// 			}break;
		
		case ACTION_START:
		{
			int nStat = pSelf->m_Socket->GetState();
			pSelf->m_nActionFlag = ACTION_START;


			//pSelf->m_Socket->m_SendData

			// 받은 2DID 저장 한다.

			BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
			pSelf->m_Socket->MakeSendData(sendDataState, TESTER_STAUS, STATUS_TESTING);
			pSelf->m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));

			pSelf->SetTimer(1, 5000, NULL);
		}break;
		case ACTION_STOP:   // H->T  ACTION_STOP을 SEND하면 T->H  T의상태를 IDLE 상태로 바꿔서 SEND한다.
		{
			BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
			pSelf->m_Socket->MakeSendData(sendDataState, TESTER_STAUS, STATUS_IDLE);
			pSelf->m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));
			
		}break;

		case ACTION_RESET:
		{
		//	int nStat = pSelf->m_Socket->GetState();
			BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
			pSelf->m_Socket->MakeSendData(sendDataState, TESTER_STAUS, STATUS_IDLE);
			pSelf->m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));

		}break;

		case ACTION_STATUS:
		{
			int nStat = pSelf->m_Socket->GetState();
			BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
			pSelf->m_Socket->MakeSendData(sendDataState, TESTER_STAUS, nStat);
			pSelf->m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));
		}break;

		case ACTION_INFO :
		{
			BYTE sendDataInfo[MAX_MSG_ID + TESTER_INFO_STR + 5] = { 0, };
			pSelf->m_Socket->MakeSendData(sendDataInfo, TESTER_INFORMATION);
			pSelf->m_Socket->Send(sendDataInfo, (MAX_MSG_ID + TESTER_INFO_STR + 5));

		}break;
		case ACTION_SET_TEST_MODE:   // H->T  ACTION_SET_TEST_MODE을 SEND하면 T->H 에서 할 작업.
		{
			int nStat = pSelf->m_Socket->GetState();
			pSelf->m_nActionFlag = ACTION_SET_TEST_MODE;
			
		
				/*    Step
				1. Unregister 던짐
				2. 연결 끊음
				3. 재 연결
				4. Register 던짐
				*/
				        
				// 1. UNREGISTER 
				BYTE sendDataUnReg[MAX_MSG_ID + REG_STR + 4] = { 0, };
				pSelf->m_Socket->MakeSendData(sendDataUnReg, TESTER_REG, UNREG_TESTER);
				pSelf->m_Socket->Send(sendDataUnReg, (MAX_MSG_ID + REG_STR + 4));

				//2. 연결 끊음  
				pSelf->m_Socket->CloseSocket();

				//3. 재연결 
				pSelf->m_Socket->Reconnect();

				//4. Register 
				BYTE sendDataReg[MAX_MSG_ID + REG_STR + 4] = { 0, };
				pSelf->m_Socket->MakeSendData(sendDataReg, TESTER_REG, TESTER_REG);
				pSelf->m_Socket->Send(sendDataReg, (MAX_MSG_ID + REG_STR + 4));

				//5. Idle 상태로 바꿈
				BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
				pSelf->m_Socket->MakeSendData(sendDataState, TESTER_STAUS, STATUS_IDLE);
				pSelf->m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));

				//6. HEART BEAT
				BYTE sendDataHeartBeat[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX] = { 0, };
				pSelf->m_Socket->MakeSendData(sendDataHeartBeat, HEART_BEAT);
				pSelf->m_Socket->Send(sendDataHeartBeat, (MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX));


		}break;
		case ACTION_TEST_SCRIPT:
		{
			BYTE sendDataInfo[MAX_MSG_ID + TESTER_INFO_STR + 5] = { 0, };
			pSelf->m_Socket->MakeSendData(sendDataInfo, TESTER_SCRIPT);
			pSelf->m_Socket->Send(sendDataInfo, (MAX_MSG_ID + TESTER_INFO_STR + 5));
		}break;

		///////////
		}
		Sleep(1);
	}
	return 0;
}

void CApTesterView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDEvent == 0)
	{
		KillTimer(0);
		BYTE sendDataHeartBeat[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX] = { 0, };
		m_Socket->MakeSendData(sendDataHeartBeat, HEART_BEAT);
		m_Socket->Send(sendDataHeartBeat, (MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX));

	}
	else if (nIDEvent == 1)
	{
		KillTimer(1);

		if (m_Socket->GetState() == STATUS_TESTING) //Modified By JJh *STOP 이 날라오면 결과값 날리면 안됨. *
		{
			bPF = !bPF;
			BYTE sendData[MAX_MSG_ID + 3 + RESULT_DATA_STR] = { 0, };
			m_Socket->MakeSendData(sendData, TEST_RESULT, bPF);
			m_Socket->Send(sendData, (MAX_MSG_ID + 3 + RESULT_DATA_STR));

			Sleep(100);

			BYTE sendDataState[MAX_MSG_ID + 4] = { 0, };
			m_Socket->MakeSendData(sendDataState, TESTER_STAUS, STATUS_IDLE);
			m_Socket->Send(sendDataState, (MAX_MSG_ID + 4));
		}
	}
	else if (nIDEvent == 2) // UI 정보용 
	{
		KillTimer(2);
		pDlg->showTesterInfo(m_Socket, m_nActionFlag);
		SetTimer(2, 500, NULL);
	}
		CView::OnTimer(nIDEvent);
}

void CApTesterView::createDlg() //다이얼로그 생성 및 Display
{
	pDlg = new CDlgDisplay();
	pDlg->Create(IDD_TesterInfo);
	pDlg->ShowWindow(SW_SHOW);
	SetTimer(2, 500, NULL);
}

UINT CApTesterView::TesterIF2(LPVOID lpVoid)
{
	CApTesterView* pSelf = (CApTesterView*)lpVoid;

	while (1) 
	{
// 		if (pSelf->m_Socket->GetState() == STATUS_IDLE)
// 		{
// 			BYTE sendDataHeartBeatTotal[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX * 1000] = {1, };
// 			BYTE sendDataHeartBeat[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX] = { 0, };
// 			for (int i = 1; i < 999; i++)
// 			{
// 				pSelf->m_Socket->MakeSendData(sendDataHeartBeat, HEART_BEAT);
// 				memcpy(&sendDataHeartBeatTotal[MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX * i], sendDataHeartBeat, MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX);
// 			}
// 				
// 
// 			pSelf->m_Socket->Send(sendDataHeartBeatTotal, (MAX_MSG_ID + SEND_HEART_BEAT_DATA_MAX)*10);
// 			
// 	
// 		}

				
		Sleep(1);
	}
	return 0;
}
