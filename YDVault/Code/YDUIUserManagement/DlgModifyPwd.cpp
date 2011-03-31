// DlgModifyPwd.cpp : implementation file
//

#include "stdafx.h"
#include "DlgModifyPwd.h"
#include "../\ObjRef\YDUserRef.h"
#include "StaticYdUser.h"
#include "../Base\DataHandler.h"
#include "../\DBBase\DBTransactionRef.h"

// CDlgModifyPwd dialog

IMPLEMENT_DYNAMIC(CDlgModifyPwd, CDialog)

CDlgModifyPwd::CDlgModifyPwd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyPwd::IDD, pParent)
	, m_strOldPwd(_T(""))
	, m_strNewPwd(_T(""))
	, m_strNewPwdConfirm(_T(""))
{

}

CDlgModifyPwd::~CDlgModifyPwd()
{
}

void CDlgModifyPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD_PWD, m_strOldPwd);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD, m_strNewPwd);
	DDX_Text(pDX, IDC_EDIT_NEW_PWD_CONFIRM, m_strNewPwdConfirm);
}


BEGIN_MESSAGE_MAP(CDlgModifyPwd, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModifyPwd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModifyPwd message handlers

void CDlgModifyPwd::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//���ж��������������Ƿ�һ��
	UpdateData();
	if(m_strNewPwd.CompareNoCase(m_strNewPwdConfirm) != 0)
	{
		AfxMessageBox(_T("��������ȷ�����벻һ�£����������룡"));
		return;
	}
	//�ж�ԭ�����Ƿ���ȷ
	HRESULT hr = E_FAIL;
	CYDUserRef* pCurUserRef = NULL;
	hr = CStaticYdUser::Instance()->GetCurUser(pCurUserRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pCurUserRef);
	//�ȴ����ݿ����¶�ȡ
	hr = pCurUserRef->Restore(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CString strOldPwd;
	hr = pCurUserRef->GetPropVal(FIELD_YDUSER_PASSWORD,strOldPwd);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(strOldPwd.CompareNoCase(m_strOldPwd) != 0)
	{
		AfxMessageBox(_T("ԭ�������벻��ȷ�������޸����룡"));
		return;
	}
	//�޸�����
	CComVariant valNewPwd ;
	CDataHandler::StringToVariant(m_strNewPwd,VT_BSTR,&valNewPwd);
	hr = pCurUserRef->SetPropVal(FIELD_YDUSER_PASSWORD,&valNewPwd);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB,TRUE);
	hr = pCurUserRef->Update();
	if(FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	AfxMessageBox(_T("�����޸ĳɹ���"));
	OnOK();
}
