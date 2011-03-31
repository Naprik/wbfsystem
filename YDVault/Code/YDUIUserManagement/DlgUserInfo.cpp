// DlgUserInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DlgUserInfo.h"
#include "../ObjRef\YDUserRef.h"
#include "../Base\DataHandler.h"
#include "AuthorityHelper.h"
#include "../DBBase\DBTransactionRef.h"


// CDlgUserInfo dialog

IMPLEMENT_DYNAMIC(CDlgUserInfo, CDialog)

CDlgUserInfo::CDlgUserInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserInfo::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strUserPwd(_T(""))
	, m_bVault(FALSE)
	, m_bPaper(FALSE)
	, m_bOperate(FALSE)
	, m_bDataMaintain(FALSE)
	, m_bBlueTooth(FALSE)
	, m_bRedOut(FALSE)
{
	m_TypeOperation = OP_NEW;
	m_pUser = NULL;
}

CDlgUserInfo::~CDlgUserInfo()
{
}

void CDlgUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_strUserPwd);
	DDX_Check(pDX, IDC_CHECK_VAULT, m_bVault);
	DDX_Check(pDX, IDC_CHECK_PAPER, m_bPaper);
	DDX_Check(pDX, IDC_CHECK_OPERATE, m_bOperate);
	DDX_Check(pDX, IDC_CHECK_DATAMAINTAIN, m_bDataMaintain);
	DDX_Check(pDX, IDC_CHECK_BLUE_TOOTH, m_bBlueTooth);
	DDX_Check(pDX, IDC_CHECK_RED_OUT, m_bRedOut);
}


BEGIN_MESSAGE_MAP(CDlgUserInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgUserInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_OPERATE, &CDlgUserInfo::OnBnClickedCheckOperate)
END_MESSAGE_MAP()


// CDlgUserInfo message handlers
HRESULT CDlgUserInfo::ReadOnlyControl(BOOL _bReadOnly)
{
	HRESULT hr = E_FAIL;
	((CEdit*)(GetDlgItem(IDC_EDIT_USERNAME)))->SetReadOnly(_bReadOnly);
	((CEdit*)(GetDlgItem(IDC_EDIT_USERPWD)))->SetReadOnly(_bReadOnly);
	GetDlgItem(IDC_CHECK_VAULT)->EnableWindow(!_bReadOnly);
	GetDlgItem(IDC_CHECK_PAPER)->EnableWindow(!_bReadOnly);
	GetDlgItem(IDC_CHECK_OPERATE)->EnableWindow(!_bReadOnly);
	GetDlgItem(IDC_CHECK_DATAMAINTAIN)->EnableWindow(!_bReadOnly);
	GetDlgItem(IDC_CHECK_BLUE_TOOTH)->EnableWindow(!_bReadOnly);
	GetDlgItem(IDC_CHECK_RED_OUT)->EnableWindow(!_bReadOnly);
	return S_OK;
}
BOOL CDlgUserInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	HRESULT hr = E_FAIL;
	if(m_TypeOperation == OP_NEW || m_TypeOperation == OP_EDIT)
	{
		hr = ReadOnlyControl(FALSE);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}
	else if(m_TypeOperation == OP_VIEW)
	{
		hr = ReadOnlyControl(TRUE);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	if(m_TypeOperation == OP_EDIT)
	{
		//要考虑是否为Admin用户，如果是，不能修改用户名
		ASSERT(m_pUser);
		CYDUserRef* pUserRef = (CYDUserRef*)m_pUser;
		ASSERT(pUserRef);
		VARIANT_BOOL bAdmin = VARIANT_FALSE;
		hr = pUserRef->IsSysUser(&bAdmin);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		if(bAdmin)
		{
			((CEdit*)(GetDlgItem(IDC_EDIT_USERNAME)))->SetReadOnly(TRUE);
		}
		//更新用户信息
		hr = pUserRef->GetPropVal(FIELD_YDUSER_NAME,m_strUserName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		hr = pUserRef->GetPropVal(FIELD_YDUSER_PASSWORD,m_strUserPwd);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		CComVariant valAuthority;
		hr = pUserRef->GetPropVal(FIELD_YDUSER_AUTHORITY,&valAuthority);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		long lAuthority = CDataHandler::VariantToLong(valAuthority);
		CAuthorityHelper helper;
		hr = helper.ConvertByVal(lAuthority,m_bVault,m_bPaper,
								m_bOperate,
								m_bBlueTooth,m_bRedOut,
								m_bDataMaintain);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}
	UpdateData(FALSE);
	OnBnClickedCheckOperate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgUserInfo::Validate(VARIANT_BOOL* _bValidate)
{
	*_bValidate = VARIANT_FALSE;
	HRESULT hr = E_FAIL;
	if(m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("名称不能为空!"));
		return S_FALSE;
	}
	VARIANT_BOOL bAdmin = VARIANT_FALSE;
	if(m_TypeOperation == OP_EDIT)
	{
		ASSERT(m_pUser);
		CYDUserRef* pUserRef = (CYDUserRef*)m_pUser;
		hr = pUserRef->IsSysUser(&bAdmin);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	if(!bAdmin)
	{
		//要判断用户名不能为Admin
		if(m_strUserName.CompareNoCase(_T("Admin")) == 0)
		{
			AfxMessageBox(_T("用户名称不能为Admin,这与系统管理员名称相同!"));
			return S_FALSE;
		}
	}
	*_bValidate = VARIANT_TRUE;
	return S_OK;
}

void CDlgUserInfo::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_TypeOperation == OP_VIEW)
	{
		return ;
	}
	UpdateData();
	HRESULT hr = E_FAIL;
	VARIANT_BOOL bValidate = VARIANT_FALSE;
	hr = Validate(&bValidate);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	if(!bValidate)
	{
		return;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	if(m_TypeOperation == OP_NEW)
	{
		ASSERT(m_pUser == NULL);
		m_pUser = new CYDUserRef(pDB);
	}
	CComVariant valName;
	CDataHandler::StringToVariant(m_strUserName,VT_BSTR,&valName);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_NAME,&valName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	CComVariant valPwd;
	CDataHandler::StringToVariant(m_strUserPwd,VT_BSTR,&valPwd);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_PASSWORD,&valPwd);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	CAuthorityHelper helper;
	long lAuthority = 0;
	hr = helper.CreateVal(m_bVault,m_bPaper,m_bOperate,
						 m_bBlueTooth,m_bRedOut,
							m_bDataMaintain,lAuthority);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	CComVariant valAuthority(lAuthority);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_AUTHORITY,&valAuthority);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	//保存到数据库
	CDBTransactionRef trans(pDB, TRUE);
	if(m_TypeOperation == OP_NEW)
	{
		hr = m_pUser->Save();
	}
	else if(m_TypeOperation == OP_EDIT)
	{
		hr = m_pUser->Update();
	}
	else
	{
		ASSERT(FALSE);
	}
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	OnOK();
}

void CDlgUserInfo::OnBnClickedCheckOperate()
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = ((CButton*)(GetDlgItem(IDC_CHECK_OPERATE)))->GetCheck();
	if(bCheck)
	{
		GetDlgItem(IDC_CHECK_BLUE_TOOTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_RED_OUT)->EnableWindow(TRUE);
		//m_bBlueTooth = TRUE;
		//m_bRedOut = TRUE;
	}
	else
	{
		GetDlgItem(IDC_CHECK_BLUE_TOOTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RED_OUT)->EnableWindow(FALSE);
		((CButton*)(GetDlgItem(IDC_CHECK_BLUE_TOOTH)))->SetCheck(FALSE);
		((CButton*)(GetDlgItem(IDC_CHECK_RED_OUT)))->SetCheck(FALSE);
	}
	//UpdateData(FALSE);
}
