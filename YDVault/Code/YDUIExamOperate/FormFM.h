#if !defined(AFX_FORMFM_H__FBBFE3A2_40E8_47FD_BC2F_5C142B5692D4__INCLUDED_)
#define AFX_FORMFM_H__FBBFE3A2_40E8_47FD_BC2F_5C142B5692D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../UIBase/XPButton.h"
#include "../UIBase/BtnST.h"
#include "../UIBase/ShadeButtonST.h"
#include "../UIBase/ColorStatic.h"
#include "../UIBase/ControlButton.h"
#include "Resource.h"
#include "../YDFormUIBase\YdFormView.h"

class BASE_DLL_API CFormFM : public CYdFormView
{
// Construction
public:
	CFormFM();   // standard constructor
	DECLARE_DYNCREATE(CFormFM)

// Dialog Data
	//{{AFX_DATA(CFormFM)
	enum { IDD = IDD_YDUIEXAM_FORM_FM };
	CControlButton	m_up;
	CControlButton	m_search;
	CControlButton	m_open;
	CControlButton	m_op;
	CControlButton	m_down;
	CControlButton	m_close;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormFM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:
	BOOL decodeFM(CString &strFM);
	CString formatFM(CString strFM);
	// Generated message map functions
	//{{AFX_MSG(CFormFM)
	afx_msg void OnOpen();
	afx_msg void OnSearch();
	afx_msg void OnClose();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnOp();
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	CColorStatic m_Static1;
	CColorStatic m_Static2;
public:
	CString m_strFrequency;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMFM_H__FBBFE3A2_40E8_47FD_BC2F_5C142B5692D4__INCLUDED_)
