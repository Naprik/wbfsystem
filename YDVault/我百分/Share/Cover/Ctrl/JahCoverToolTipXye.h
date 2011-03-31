//##ModelId=4140A3680040
#pragma once
class CJahCoverToolTip : public CWnd
{
public:
	//##ModelId=4140A36800A6
	CJahCoverToolTip();
	//##ModelId=4140A36800A5
	void EnableToolTip();
public:
	//{{AFX_VIRTUAL(CJahCoverToolTip)
	//}}AFX_VIRTUAL
public:
	//##ModelId=4140A36800A0
	void GetMargin( LPRECT lprc ) const;
	//##ModelId=4140A368009E
	void SetMargin( LPRECT lprc );
	//##ModelId=4140A368009C
	COLORREF GetTipTextColor()const;
	//##ModelId=4140A368009A
	COLORREF GetTipBkColor()const;
	//##ModelId=4140A3680094
	void SetTipBkColor(COLORREF clr);
	//##ModelId=4140A3680092
	void SetTipTextColor(COLORREF clr);
	//##ModelId=4140A3680090
	void DelTool(CWnd*pWnd);
	//##ModelId=4140A368008C
	void RelayEvent( LPMSG lpMsg );
	//##ModelId=4140A3680089
	int GetDelayTime( DWORD dwDuration ) const;
	//##ModelId=4140A3680086
	BOOL AddTool( CWnd* pWnd, LPCTSTR lpszText= LPSTR_TEXTCALLBACK);
	//##ModelId=4140A368007E
	void SetDelayTime( DWORD dwDuration, int iTime );
	//##ModelId=4140A3680081
	void SetDelayTime(UINT nDelay);
	//##ModelId=4140A368007C
	BOOL Create(CWnd*pParent);
	//##ModelId=4140A3680076
	virtual ~CJahCoverToolTip();
protected:
	//##ModelId=4140A3680075
	void OnCmdTip();
	//##ModelId=4140A3680074
	void Show();
	//##ModelId=4140A3680073
	void Hide();
	//##ModelId=4140A3680072
	void RegisterWindowClass();
	//{{AFX_MSG(CJahCoverToolTip)
	//##ModelId=4140A368006D
	afx_msg void OnTimer(UINT nIDEvent);
	//##ModelId=4140A368006B
	afx_msg void OnDestroy();
	//##ModelId=4140A3680069
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//##ModelId=4140A3680068
	int m_nTimeAutoPop;
	//##ModelId=4140A3680064
	int m_nTimeReShow;
	//##ModelId=4140A3680063
	int m_nTimeInitial;
	//##ModelId=4140A3680060
	CMapPtrToPtr m_map;
	//##ModelId=4140A3680056
	BOOL m_bTimerInitial;
	//##ModelId=4140A3680055
	BOOL m_bTimerPop;

	//##ModelId=4140A3680054
	HWND m_hCurWnd;
	//##ModelId=4140A368004D
	COLORREF m_crBg;
	//##ModelId=4140A368004C
	COLORREF m_crText;
	//##ModelId=4140A368004B
	char m_strText[1024];
	//##ModelId=4140A368004A
	CRect m_rcMargin;
	//##ModelId=4140A3680044
	CButton m_btn;
};
