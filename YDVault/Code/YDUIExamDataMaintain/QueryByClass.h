#pragma once

#include "Resource.h"
#include "../ObjRef/YDObjectRef.h"
#include <set>
#include "../UIBase/BCGPSortListCtrl.h"
// CQueryByClass dialog

class CQueryByClass : public CDialog
{
	DECLARE_DYNAMIC(CQueryByClass)

public:
	CQueryByClass(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQueryByClass();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_QMARKBYCLASS_SHEET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedBtnDmQmarkAdd();
	afx_msg void OnBnClickedBtnDmQmarkDel();
	afx_msg void OnCbnSelchangeCmbDmQmarkDept();
	virtual BOOL OnInitDialog();

	HRESULT GetSearchClass(std::list<OBJID>* pListClass);

private:
	BOOL IsClassExist(CYDObjectRef* pStu);
	HRESULT RemoveSelClass(CYDObjectRef* pStu);

	HRESULT AddSelClassItem(CYDObjectRef* pObjRef);
	HRESULT AddClassItem(CYDObjectRef* pObjRef);
private:
	CBCGPComboBox				m_cmbDept;

	CBCGPSortListCtrl			m_AllClassList;
	CBCGPSortListCtrl			m_SelClassList;

	std::list<CYDObjectRef*>	m_lstDeptCache;
	std::list<CYDObjectRef*>	m_lstClsCache;
	std::list<CYDObjectRef*>	m_lstSelClsCache;

	std::set<OBJID>				m_setExistCls;
};
