#pragma once

#include "Resource.h"
#include "../YDFormUIBase/YdFormView.h"
// CPersionInfoView form view

class CPersionInfoView : public CYdFormView
{
	DECLARE_DYNCREATE(CPersionInfoView)

protected:
	CPersionInfoView();           // protected constructor used by dynamic creation
	virtual ~CPersionInfoView();

public:
	enum { IDD = IDD_DLG_PERSIONINFO_VIEW };
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
	virtual void ReadonlyPage(BOOL bReadonly);
private:
	CString m_strGender;
	CString m_strName;
	CString m_strAge;
	CString m_strID;
	CString m_strLevel;
	CString m_strDepartment;
	CString m_strPhone;
	CString m_strQQ;
	CString m_strEmail;
};


