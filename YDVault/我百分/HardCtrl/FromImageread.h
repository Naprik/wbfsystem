#if !defined(AFX_FROMIMAGEREAD_H__396BFC47_EEC8_4ED2_982E_0E170FF90C8E__INCLUDED_)
#define AFX_FROMIMAGEREAD_H__396BFC47_EEC8_4ED2_982E_0E170FF90C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "BmpDat.h"
#include "Control.h"
// FromImageread.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFromImageread dialog

class CFromImageread : public cdxCSizingDialog
{
// Construction
public:
	CFromImageread(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFromImageread)
	CBitmap   m_bmp;
	CBmpDat *bmp;
	CControl con[10];
	int contyp;
	int saved;
	int cnum;
	BOOL lbd;
	BOOL rbd;
	int tx[20],ty[20];

	int StringEnd(CString &ts,char e[]);

// Dialog Data
	//{{AFX_DATA(CFromImageread)
	enum { IDD = IDD_FORM_IMAGE_READ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFromImageread)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFromImageread)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnOpne();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSave();
	afx_msg void OnRevo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FROMIMAGEREAD_H__396BFC47_EEC8_4ED2_982E_0E170FF90C8E__INCLUDED_)
