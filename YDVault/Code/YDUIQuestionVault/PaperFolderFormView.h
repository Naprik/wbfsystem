#pragma once

#include "../YDFormUIBase/YdFormView.h"
#include "resource.h"
#include "../UIBase/BCGPSortListCtrl.h"
// CPaperFolderFormView form view

class BASE_DLL_API CPaperFolderFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CPaperFolderFormView)

protected:
	CPaperFolderFormView();           // protected constructor used by dynamic creation
	virtual ~CPaperFolderFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_PAPERFOLDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	virtual void ReadonlyPage(BOOL bReadonly);
	virtual HRESULT UpdateProp(BOOL bUpdate) ;

	afx_msg void OnBnClickedBtnOutput();
	afx_msg void OnBnClickedBtnDel();
	HRESULT AddItem(CYdObjWrapper* _pWrapper);

private:
	void showdata();
	HRESULT ClearPaper();

public:
	CString				m_strName;
	CString				m_strCreator;
	CString				m_strCreateDate;
	CString				m_strDescription;
	CBCGPSortListCtrl	m_PaperListCtrl;
	std::list<CYdObjWrapper*>			m_lstPaper;
//	afx_msg void OnLvnItemchangedListPapers(NMHDR *pNMHDR, LRESULT *pResult);
};


