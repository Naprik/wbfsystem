
/****************************************************************
                           XP ��ť ��
ʹ�÷�����Ϊ�Ի���İ�ť��һ�����������ѱ������͸ĳ�CJahCoverXpButton
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
	CPen m_BoundryPen;              //��ť����߿�
	//##ModelId=4140A3680255
	CPen m_InsideBoundryPenLeft;    //���ָ�����ڰ�ť֮��ʱ��ť���ڱ߿�
	//##ModelId=4140A368024B
	CPen m_InsideBoundryPenRight;
	//##ModelId=4140A3680244
	CPen m_InsideBoundryPenTop;
	//##ModelId=4140A368023F
	CPen m_InsideBoundryPenBottom;
	//##ModelId=4140A3680237
	CPen m_InsideBoundryPenLeftSel; //��ť��ý���ʱ��ť���ڱ߿�
	//##ModelId=4140A368022D
	CPen m_InsideBoundryPenRightSel;
	//##ModelId=4140A3680228
	CPen m_InsideBoundryPenTopSel;
	//##ModelId=4140A3680223
	CPen m_InsideBoundryPenBottomSel;
	//##ModelId=4140A3680219
	CBrush m_FillActive;            //��ť�ĵ�ɫ��������Ч����Ч����״̬
	//##ModelId=4140A3680212
	CBrush m_FillInactive;
	
	//-----------��ť��״̬-------------------------------//

	//##ModelId=4140A368020F
	BOOL m_bOver;	         //���λ�ڰ�ť֮��ʱ��ֵΪtrue����֮Ϊflase
	//##ModelId=4140A368020E
	BOOL m_bTracking;	     //����갴��û���ͷ�ʱ��ֵΪtrue
	//##ModelId=4140A368020D
	BOOL m_bSelected;	     //��ť�������Ǹ�ֵΪtrue
	//##ModelId=4140A3680205
	BOOL m_bFocus;	         //��ťΪ��ǰ��������ʱ��ֵΪtrue
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

