#pragma once
#include "afxcmn.h"
#include "./ocxfiles/spreadsheet.h"
// CDlgCTrayVAT 대화 상자입니다.
typedef CDialogEx TabPage;
class CDlgCTrayVAT : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCTrayVAT)

	CVatHwDesnDim::TrayPP::CamPitchPos m_stEditWideCam;
	CVatHwDesnDim::TrayPP::CamPitchPos m_stEditNarrowCam;

	/*tab contents*/
	enum { 
		TPG_PICKER,
		TPG_CTRAY1,TPG_CTRAY2,TPG_CTRAY3,TPG_CTRAY4,TPG_CTRAY5,TPG_CTRAY6,
		TPG_LDTBL1,TPG_LDTBL2,TPG_LDTBL3,TPG_LDTBL4,
		TPG_BUFFER,
		MAX_TPG_COUNT
	};
	/*timer id*/
	enum timer_id : UINT
	{
		general_tmr=0,
		hand_enc_tmr_500ms,
		af_check_tmr_500ms,
		wcam_adj_tmr_500ms,
	};

	CTabCtrl m_tabTeachingObj;
	TabPage* m_tpgOld;
	TabPage* m_arrTpgAligns[ MAX_TPG_COUNT ] ;
	void init_tab_pages();

	/* cam pos */
	int           m_nLastSel_Row ;
	int           m_nLastSel_Col ;
	CSpreadSheet m_gridvatCamPos;
	void display_cam_position();
	void edit_cam_pos(int r, int c, const char* v);
	BOOL is_modify();
	void start_cmd_exec_check(timer_id cmd_tmr);
	void cmd_compt_check(timer_id cmd_tmr);

	/* hand pos */
	CSpreadSheet m_gridCurEnc;
	void display_hand_encoder();
	void MakeLog(LPCTSTR fmt, ...);

public:
	CDlgCTrayVAT(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCTrayVAT();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_VAT_TRAYPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTcnSelchangeTab3point(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedManuBtnEncoder();
	afx_msg void OnBnClickedManuBtnServo();
	DECLARE_EVENTSINK_MAP()
	void ClickSpreadVatCamPos(long Col, long Row);
	void DblClickSpreadVatCamPos(long Col, long Row);
	afx_msg void OnBnClickedBtnAutoFocus();
	afx_msg void OnBnClickedBtnRefPkrFineAdj();
	afx_msg void OnBnClickedBtnApply();
};
