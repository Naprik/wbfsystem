#if !defined(AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJahCoverColorListBox window

//##ModelId=4140A36800E0
class CJahCoverColorListBox : public CListBox
{
// Construction
public:
	//##ModelId=4140A36800F9
	CJahCoverColorListBox();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJahCoverColorListBox)
	public:
	//##ModelId=4140A36800F6
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//##ModelId=4140A36800F0
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=4140A36800ED
	int AddString(LPCTSTR lpszItem, COLORREF itemColor = RGB(0,0,0));
	//##ModelId=4140A36800EB
	virtual ~CJahCoverColorListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJahCoverColorListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_)
