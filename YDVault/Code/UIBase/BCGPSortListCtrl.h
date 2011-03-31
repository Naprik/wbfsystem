#pragma once

#include "BCGPListCtrl.h"
#include <List>

#define  ITEM_TEXT_COLOR  RGB(0,0,190)
#define  ITEM_TEXT_COLOR_DEFAULT  RGB(0,0,0)
#define  ITEM_TEXT_BK_COLOR_DEFAULT  RGB(255,255,255)
#define  ITEM_TEXT_BK_COLOR RGB(110,180,200)

class BASE_DLL_API CMyListItem : public CObject
{
public:
	CMyListItem(int _iRow, int _iCol, COLORREF clrText, COLORREF clrBckg) :
	  m_iRow (_iRow),
		  m_iCol (_iCol),
		  m_clrText (clrText),
		  m_clrBckg (clrBckg)
	  {
		  m_pData = NULL;
	  }

	  int	m_iRow;
	  int	m_iCol;
	  COLORREF	m_clrText;
	  COLORREF	m_clrBckg;
	  DWORD_PTR m_pData;
};



class BASE_DLL_API CBCGPSortListCtrl : public CBCGPListCtrl
{
	DECLARE_DYNAMIC(CBCGPSortListCtrl)

public:
	CBCGPSortListCtrl(void);
	~CBCGPSortListCtrl(void);
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	//}}AFX_VIRTUAL
	virtual int OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn);

	virtual COLORREF OnGetCellTextColor (int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor (int nRow, int nColum);

	std::list<CMyListItem*> m_lstItem;

	void SetItemColor( int nRow,
						int nColumn,
						COLORREF _colText = ITEM_TEXT_COLOR,
						COLORREF _colBk = ITEM_TEXT_BK_COLOR_DEFAULT,
						BOOL _bInvalidata = TRUE);
	void SetRowColor(int nRow,COLORREF _colText = ITEM_TEXT_COLOR,
		COLORREF _colBk = ITEM_TEXT_BK_COLOR_DEFAULT,
		BOOL _bInvalidata = TRUE);
	virtual void Sort (int iColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE);
	//设置两行之间颜色各不相同
	void SetSpaceRowColor(BOOL _bSpaceRowColor);

	DECLARE_MESSAGE_MAP()
};