#if !defined(AFX_FORMIMAGE_H__7E3A6BCB_B967_4849_B88C_EC4077C4F84C__INCLUDED_)
#define AFX_FORMIMAGE_H__7E3A6BCB_B967_4849_B88C_EC4077C4F84C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "videocapx.h"
#include "../uibase/XPButton.h"
#include "Resource.h"
#include "../YDFormUIBase\YdFormView.h"
// FormImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormImage dialog

class BASE_DLL_API CFormImage : public CYdFormView
{
// Construction
public:
	CFormImage();   // standard constructor
	DECLARE_DYNCREATE(CFormImage)

	CVideoCapX  m_VideoCapX;
// Dialog Data
	//{{AFX_DATA(CFormImage)
	enum { IDD = IDD_YDUIEXAM_FORM_IMAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormImage)
	virtual void OnInitialUpdate();
	afx_msg void OnOpenCamera();
	afx_msg void OnCloseCamera();
	afx_msg void OnCatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMIMAGE_H__7E3A6BCB_B967_4849_B88C_EC4077C4F84C__INCLUDED_)
