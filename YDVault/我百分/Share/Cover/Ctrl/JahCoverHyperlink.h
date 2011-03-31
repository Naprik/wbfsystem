#if !defined(HYPERLINK_H)
#define HYPERLINK_H

//##ModelId=4140A36701BB
class CJahCoverHyperLink : public CStatic
{
public:
	//##ModelId=4140A367020B
	CJahCoverHyperLink();
	//##ModelId=4140A3670206
	virtual ~CJahCoverHyperLink();

public:
	//##ModelId=4140A36701E5
	COLORREF	m_LinkColor;
	//##ModelId=4140A36701E4
	COLORREF	m_HoverColor;
	//##ModelId=4140A36701E3
	COLORREF	m_VisitedColor;
	//##ModelId=4140A36701DB
	BOOL		m_bIsHover;
	//##ModelId=4140A36701DA
	BOOL		m_bIsVisited;
	//##ModelId=4140A36701D9
	CString		m_sUrl;
	//##ModelId=4140A36701D5
    HCURSOR		m_hLinkCursor;                        
	//##ModelId=4140A36701D2
    CFont		m_Font; 
	//##ModelId=4140A36701CF
	BOOL		m_bUnderline;
	//##ModelId=4140A36701C7
    CToolTipCtrl m_ToolTip;                        

private:
	//##ModelId=4140A3670205
	void SetCursor();

public:
	//##ModelId=4140A3670203
	void SetURL(CString sUrl);

protected:
    //{{AFX_VIRTUAL(CJahCoverHyperLink)
    public:
	//##ModelId=4140A36701FE
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
	//##ModelId=4140A36701FC
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CJahCoverHyperLink)
	//##ModelId=4140A36701F8
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//##ModelId=4140A36701F1
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//##ModelId=4140A36701ED
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
	//##ModelId=4140A36701E6
    afx_msg void OnClicked();
    DECLARE_MESSAGE_MAP()
};

#endif
