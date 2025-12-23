#pragma once
typedef CDialogEx TabPage;
class CDlgTestPP_VAT : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTestPP_VAT)

	CPointF m_stEditCamPos;
	CPointF m_stEditCamAdjOfs;
	double m_dEditSafetyUpZ;
	double m_dEditSafetyDnZ;

	/*tab contents*/
	enum { 
		TPG_PICKER,
		TPG_TEST_SITE,
		TPG_LDTBL,
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
	int m_nHandIndex;
	CTabCtrl m_ctrlTab;
	TabPage* m_tpgOld;
	TabPage* m_arrTpgs[ MAX_TPG_COUNT ] ;
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
	CDlgTestPP_VAT(int hand_index, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTestPP_VAT();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_VAT_TESTPP };

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
	afx_msg void OnBnClickedBtnZSafetyUp();
	afx_msg void OnBnClickedBtnZSafetyDn();
	afx_msg void OnBnClickedBtnReload();
};
