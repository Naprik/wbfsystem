#pragma once

#include "stdafx.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "resource.h"
#include "afxwin.h"
#include "BCGPComboBox.h"
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "afxcmn.h"
#include "../ObjRef/PropQueryContidition.h"
// CFormPaper form view

class BASE_DLL_API CFormStudentDevice : public CYdFormView
{
	DECLARE_DYNCREATE(CFormStudentDevice)

protected:
	CFormStudentDevice();           // protected constructor used by dynamic creation
	virtual ~CFormStudentDevice();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_FORM_DEVICE };
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
	afx_msg void OnBnClickedBtnDmDeviceAdd();
	afx_msg void OnBnClickedBtnDmDeviceEdit();
	afx_msg void OnBnClickedBtnDmDeviceDel();
	afx_msg void OnBnClickedBtnDmDeviceSearch();
	virtual void OnInitialUpdate();

private:
	HRESULT AddItem(CYDObjectRef* _pref);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);

	HRESULT InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion);

private:
	CBCGPListCtrl				m_DeviceListCtrl;
	CBCGPKeyGridCtrl			m_QueryGrid;
	std::list<CYDObjectRef*>	m_lstDeviceCache;
	UINT m_utotalNumber;
	UINT m_uPageSize;
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*>	m_lstPropQuery;
public:
	afx_msg void OnBnClickedBtnDmQDeviceBegin();
	afx_msg void OnBnClickedBtnDmQDevicePrev();
	afx_msg void OnBnClickedBtnDmQDeviceNext();
	afx_msg void OnBnClickedBtnDmQDeviceEnd();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnBnClickedBtnDmDeviceOutput();
	afx_msg void OnBnClickedBtnDmDeviceInput();
};


