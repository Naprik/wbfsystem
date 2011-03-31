#if !defined(AFX_MYLIST_H__7FD925BF_AA98_48F6_8CF1_82A6F09C2610__INCLUDED_)
#define AFX_MYLIST_H__7FD925BF_AA98_48F6_8CF1_82A6F09C2610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyList.h : header file
//
#include  <Afxtempl.h>
#include "../UIBase/SortListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomdrawList window

class AFX_EXT_CLASS CCustomdrawList : public CListCtrl
{
// Construction
public:
	CCustomdrawList();
	CMap<DWORD , DWORD& , COLORREF , COLORREF&> MapItemColor;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomdrawList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomdrawList();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	void SetItemColor(DWORD  iItem, COLORREF color);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomdrawList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLIST_H__7FD925BF_AA98_48F6_8CF1_82A6F09C2610__INCLUDED_)
