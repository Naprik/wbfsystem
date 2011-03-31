#if !defined(AFX_DLGSTUNFO_H__91C809BA_6CF8_439C_A664_A3A521B2F65A__INCLUDED_)
#define AFX_DLGSTUNFO_H__91C809BA_6CF8_439C_A664_A3A521B2F65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Resource.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../YDExamObjRef/YdStudentRef.h"
// DlgStunfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStunfo dialog

class AFX_EXT_CLASS CDlgStunfo : public CDialog
{
// Construction
public:
	CDlgStunfo(OPERATION op, CWnd* pParent = NULL);   // standard constructor
	~CDlgStunfo();

// Dialog Data
	//{{AFX_DATA(CDlgStunfo)
	enum { IDD = IDD_YDUIEXAM_DLG_STUINFO };
	
	CComboBox	m_department;
	CComboBox	m_class;
	CComboBox	m_gender;
	CString		m_strEmail;
	CString		m_strName;
	CString		m_strNum;
	CString		m_strTel;
	CString		m_strPath;
	CString		m_strGender;
	CString     m_strClass;
	CString		m_strBlueteeth;
	int			m_iAge;

	CYdStudentRef*				m_pStudent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStunfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgStunfo)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSelPic();
	afx_msg void OnSelchangeDepartment();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ClearClassCache();
	void ReadOnly(BOOL bReadOnly);

	HRESULT Save();
	HRESULT Update();
	HRESULT GetPhotoData(VARIANT* _pVal);
	HRESULT ReadPhotoBuf(CString _strfile);
	void	DestroyPhoto();
	HBITMAP			BufferToHBITMAP();
	void			DrawUserPhoto(int x,int y,CDC* pDC);
private:
	std::list<CYDObjectRef*>	m_lstDeptCache;
	std::list<CYDObjectRef*>	m_lstClsCache;

	OPERATION					m_op;
	CYdObjWrapper*				m_pClassWrapper;

	CString		m_strEmailTmp;
	CString		m_strNameTmp;
	CString		m_strNumTmp;
	CString		m_strTelTmp;
	CString		m_strPathTmp;
	CString		m_strGenderTmp;
	CString     m_strClassTmp;
	CString		m_strBlueteethTmp;
	int			m_iAgeTmp;
	HBITMAP			m_hPhotoBitmap;
	char			*m_pBMPBuffer;
	DWORD			m_dwFileLen;
public:
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTUNFO_H__91C809BA_6CF8_439C_A664_A3A521B2F65A__INCLUDED_)
