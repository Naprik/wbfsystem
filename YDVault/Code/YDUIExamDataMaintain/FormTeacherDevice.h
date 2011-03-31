#pragma once

#include "stdafx.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "resource.h"
#include "afxwin.h"
#include "BCGPComboBox.h"
#include "BCGPGridCtrl.h"
#include "afxcmn.h"


// CFormTeacherDevice form view

class BASE_DLL_API CFormTeacherDevice : public CYdFormView
{
	DECLARE_DYNCREATE(CFormTeacherDevice)

protected:
	CFormTeacherDevice();           // protected constructor used by dynamic creation
	virtual ~CFormTeacherDevice();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_FORM_TEA_DEVICE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedDmTdeviceNew();
	afx_msg void OnBnClickedDmTdeviceEdit();
	afx_msg void OnBnClickedDmTdeviceDel();

private:
	HRESULT AddTeacherDeviceItem(CYDObjectRef* _pref);

private:
	CBCGPSortListCtrl			m_TDeviceListCtrl;
	std::list<CYDObjectRef*>	m_lstCache;
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnBnClickedDmTdeviceOutput();
};


