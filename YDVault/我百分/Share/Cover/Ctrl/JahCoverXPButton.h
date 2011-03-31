
/****************************************************************
                           XP 按钮 类
使用方法：为对话框的按钮绑定一个变量，并把变量类型改成CJahCoverXpButton
****************************************************************/
//##ModelId=4140A3680203
#pragma once
class CJahCoverXpButton : public CButton
{
// Construction
public:
	//##ModelId=4140A36802A5
	CJahCoverXpButton();
	//##ModelId=4140A3680260
    CRect rect;
	// Attributes
protected:
	//##ModelId=4140A368025D
	CPen m_BoundryPen;              //按钮的外边框
	//##ModelId=4140A3680255
	CPen m_InsideBoundryPenLeft;    //鼠标指针置于按钮之上时按钮的内边框
	//##ModelId=4140A368024B
	CPen m_InsideBoundryPenRight;
	//##ModelId=4140A3680244
	CPen m_InsideBoundryPenTop;
	//##ModelId=4140A368023F
	CPen m_InsideBoundryPenBottom;
	//##ModelId=4140A3680237
	CPen m_InsideBoundryPenLeftSel; //按钮获得焦点时按钮的内边框
	//##ModelId=4140A368022D
	CPen m_InsideBoundryPenRightSel;
	//##ModelId=4140A3680228
	CPen m_InsideBoundryPenTopSel;
	//##ModelId=4140A3680223
	CPen m_InsideBoundryPenBottomSel;
	//##ModelId=4140A3680219
	CBrush m_FillActive;            //按钮的底色，包括有效和无效两种状态
	//##ModelId=4140A3680212
	CBrush m_FillInactive;
	
	//-----------按钮的状态-------------------------------//

	//##ModelId=4140A368020F
	BOOL m_bOver;	         //鼠标位于按钮之上时该值为true，反之为flase
	//##ModelId=4140A368020E
	BOOL m_bTracking;	     //在鼠标按下没有释放时该值为true
	//##ModelId=4140A368020D
	BOOL m_bSelected;	     //按钮被按下是该值为true
	//##ModelId=4140A3680205
	BOOL m_bFocus;	         //按钮为当前焦点所在时该值为true
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJahCoverXpButton)
	protected:
	//##ModelId=4140A36802A3
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
public:
	//##ModelId=4140A368029A
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//##ModelId=4140A3680290
	virtual void DoGradientFill(CDC *pDC, CRect* rect);
	//##ModelId=4140A3680287
	virtual void DrawInsideBorder(CDC *pDC, CRect* rect);
	//##ModelId=4140A3680285
	virtual ~CJahCoverXpButton();
protected:
	//{{AFX_MSG(CJahCoverXpButton)
	//##ModelId=4140A368027B
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=4140A3680271
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//##ModelId=4140A3680267
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

