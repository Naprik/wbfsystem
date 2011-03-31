#if !defined(AFX_FORMDEPARTMENT_H__4AB1430B_9B84_4AD4_B781_C1917DF4033F__INCLUDED_)
#define AFX_FORMDEPARTMENT_H__4AB1430B_9B84_4AD4_B781_C1917DF4033F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "resource.h"

class CDatabaseEx;
class CPropQueryContidition;

class BASE_DLL_API CFormDepartment : public CYdFormView
{
// Construction

	DECLARE_DYNCREATE(CFormDepartment)

protected:
	CFormDepartment();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormDepartment();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_FORM_DEPARTMENT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDepartment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFormDepartment)
	afx_msg void OnAdddepart();
	afx_msg void OnAddclass();
	afx_msg void OnUpdatedepart();
	afx_msg void OnUpdateclass();
	afx_msg void OnDeletedepart();
	afx_msg void OnDeleteclass();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	CBCGPSortListCtrl	m_ClassListCtrl;
	CBCGPSortListCtrl	m_DepartListCtrl;
protected:
	virtual void OnInitialUpdate();
private:
	//在部门列表中增加一条部门记录
	HRESULT AddDepartItem(CYDObjectRef* _pref);
	//在班级列表中增加一个班级记录
	HRESULT AddClassItem(CYdObjWrapper* _pWrapper);
	void showdata();
private:
	std::list<CYDObjectRef*>			m_lstDepart;
	std::list<CYdObjWrapper*>			m_lstClass;
	BOOL								m_bLoad;
	HRESULT ClearDepart();
	HRESULT ClearClass();
public:
	afx_msg void OnLvnItemchangedClass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedDepartmentlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickDepartmentlist(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnHdnItemdblclickClassList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemdblclickDeptList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMDEPARTMENT_H__4AB1430B_9B84_4AD4_B781_C1917DF4033F__INCLUDED_)
