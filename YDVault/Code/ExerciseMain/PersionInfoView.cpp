// PersionInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "PersionInfoView.h"
#include "../YDUIUserManagement/StaticYdUser.h"
#include "../ObjRef/YDUserRef.h"

// CPersionInfoView

IMPLEMENT_DYNCREATE(CPersionInfoView, CYdFormView)

CPersionInfoView::CPersionInfoView()
	: CYdFormView(CPersionInfoView::IDD)
	, m_strGender(_T(""))
	, m_strName(_T(""))
{

}

CPersionInfoView::~CPersionInfoView()
{
}

void CPersionInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CMB_GENDER, m_strGender);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_AGE, m_strAge);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_strLevel);
	DDX_Text(pDX, IDC_EDIT_DEP, m_strDepartment);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
	DDX_Text(pDX, IDC_EDIT_QQ, m_strQQ);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
}

BEGIN_MESSAGE_MAP(CPersionInfoView, CFormView)
END_MESSAGE_MAP()


// CPersionInfoView diagnostics

#ifdef _DEBUG
void CPersionInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPersionInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPersionInfoView message handlers


void CPersionInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CYDUserRef* pUser = NULL;
	CStaticYdUser::Instance()->GetCurUser(pUser);
	pUser->GetPropVal(FIELD_YDUSER_NAME, m_strName);
	pUser->GetPropVal(FIELD_YDUSER_AGE, m_strAge);
	pUser->GetPropVal(FIELD_YDUSER_GENDER, m_strGender);
	pUser->GetPropVal(FIELD_YDUSER_ID, m_strID);
	pUser->GetPropVal(FIELD_YDUSER_LEVEL, m_strLevel);
	pUser->GetPropVal(FIELD_YDUSER_DEPARTMENT, m_strDepartment);
	pUser->GetPropVal(FIELD_YDUSER_MPHONE, m_strPhone);
	pUser->GetPropVal(FIELD_YDUSER_QQ, m_strQQ);
	pUser->GetPropVal(FIELD_YDUSER_EMAIL, m_strEmail);
	UpdateData(FALSE);
}

void CPersionInfoView::ReadonlyPage(BOOL bReadonly)
{
	GetDlgItem(IDC_CMB_GENDER)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_AGE)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_LEVEL)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_DEP)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_QQ)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_EDIT_EMAIL)->EnableWindow(!bReadonly);

}