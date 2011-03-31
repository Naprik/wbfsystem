#pragma once
#include "Resource.h"
#include "afxcmn.h"

// CDlgSelClass dialog
#include <list>
class CYDObjectRef;

class CDlgSelClass : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelClass)

public:
	CDlgSelClass(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelClass();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_SEL_CLASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstClass;
private:
	std::list<CYDObjectRef*> m_lstClear;
	HRESULT InitClass();
public:
	afx_msg void OnBnClickedOk();
public:
	std::list<CYDObjectRef*> m_lstSelClass;
	virtual INT_PTR DoModal();
};
