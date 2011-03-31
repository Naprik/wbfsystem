#pragma once
#include "afxcmn.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "../ObjRef/YDObjectRef.h"
#include "../YDExamObjRef/YDExamAddress.h"
#include "afxwin.h"
#include "resource.h"
#include <list>
#include <set>
#include "../ObjRef/YdObjWrapper.h"

// CDlgExamAddrSetSutdent dialog

class CDlgExamAddrSetSutdent : public CDialog
{
	DECLARE_DYNAMIC(CDlgExamAddrSetSutdent)

public:
	CDlgExamAddrSetSutdent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExamAddrSetSutdent();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_EXAMADDR_STU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();

private:
	CBCGPSortListCtrl	m_UnitListCtrl;
	CBCGPSortListCtrl	m_DeviceListCtrl;
	CBCGPSortListCtrl	m_StudentListCtrl;

	CComboBox			m_cmbDept;
	CComboBox			m_cmbClass;
	
	std::list<CYDObjectRef*>	m_lstDeptCache;
	std::list<CYDObjectRef*>	m_lstClassCache;
	std::list<CYDObjectRef*>	m_lstStudentsCache;
	std::list<CYDObjectRef*>	m_lstDeviceCache;
	std::list<CYdObjWrapper*>	m_lstUnitCache;
	std::set<OBJID>				m_setExistStu;
public:

	CYDExamAddress*		m_pExamAddr;
public:
	afx_msg void OnCbnSelchangeDmExamaddrDepartment();
	afx_msg void OnBnClickedBtnExamaddrOk();
	afx_msg void OnCbnSelchangeDmExamaddrClass();

	afx_msg void OnBnClickedBtnDmExamaddrAdd();
	afx_msg void OnBnClickedBtnDmExamaddrDel();

private:
	HRESULT AddDeptStuItem(CYDObjectRef* _pref);
	HRESULT AddDeviceItem(CYDObjectRef* _pref);
	HRESULT AddExamAddrUnitItem(CYdObjWrapper* _pref);
	BOOL	IsStudentExist(CYDObjectRef* pStu);

};
