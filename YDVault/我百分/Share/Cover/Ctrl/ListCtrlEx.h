#ifndef _CLISTCTRLEX_H_
#define _CLISTCTRLEX_H_

class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNCREATE(CListCtrlEx)
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();
    //{{AFX_VIRTUAL(CFontCombo)     
	public:
	virtual void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL
public:
	COLORREF m_clrText;
	COLORREF m_clrBkgnd;
	COLORREF m_clrHText;
	COLORREF m_clrHBkgnd;
	COLORREF m_clrPercent;
    COLORREF m_clrHPercent;
	BOOL GetCellRect(int iRow, int iCol, int nArea, CRect &rect);
protected:
	//{{AFX_MSG(CListCtrlEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
