// YdConfigDBPage.cpp : implementation file
//

#include "stdafx.h"
#include "YdConfig.h"
#include "YdConfigDBPage.h"
#include "YdConfigSheet.h"
#include "../DBBase/DatabaseEx.h"


// CYdConfigDBPage dialog

IMPLEMENT_DYNAMIC(CYdConfigDBPage, CPropertyPage)

CYdConfigDBPage::CYdConfigDBPage()
	: CPropertyPage(CYdConfigDBPage::IDD)
	, m_iDBType(0)
	, m_iConnectionMode(0)
	, m_strDBServerName(_T(""))
	, m_strUserName(_T(""))
	, m_strPwd(_T(""))
	, m_sConnectTime(0)
	, m_strDBFile(_T(""))
{

}

CYdConfigDBPage::~CYdConfigDBPage()
{
}

void CYdConfigDBPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_DB_TYPE, m_iDBType);
	DDX_CBIndex(pDX, IDC_COMBO_CONNECTION_MODE, m_iConnectionMode);
	DDX_Text(pDX, IDC_EDIT_DB_SERVER_NAME, m_strDBServerName);
	DDX_Text(pDX, IDC_EDIT_DB_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_DB_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_DB_CONNET_TIME, m_sConnectTime);
	DDV_MinMaxShort(pDX, m_sConnectTime, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_DB_FILE, m_strDBFile);
}


BEGIN_MESSAGE_MAP(CYdConfigDBPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CONNECT, &CYdConfigDBPage::OnBnClickedButtonTestConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEL_DBFILE, &CYdConfigDBPage::OnBnClickedButtonSelDbfile)
END_MESSAGE_MAP()


// CYdConfigDBPage message handlers

BOOL CYdConfigDBPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CYdConfigSheet* pSheet = static_cast<CYdConfigSheet*>(GetParent());
	pSheet->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

BOOL CYdConfigDBPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CYdConfigSheet* pSheet = static_cast<CYdConfigSheet*>(GetParent());
	ASSERT(pSheet);
	m_pDBConfig = &pSheet->m_DBConfig;
	HRESULT hr = E_FAIL;
	hr = m_pDBConfig->Read();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("数据库配置有错误！"));
		return FALSE;
	}
	UpdateDBConfig(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CYdConfigDBPage::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class
	if(!ValidateData())
	{
		return -1;
	}
	UpdateDBConfig();
	return CPropertyPage::OnWizardNext();
}

BOOL   CYdConfigDBPage::ValidateData()
{
	UpdateData();
	if(m_iDBType == -1)
	{
		AfxMessageBox(_T("请选择数据库类型！"));
		return FALSE;
	}
	if(m_iDBType != 2)
	{
		//当数据库不是Access时验证下列
		if(m_iConnectionMode == -1)
		{
			AfxMessageBox(_T("请选择数据库连接模式!"));
			return FALSE;
		}
		if(m_strDBServerName.IsEmpty())
		{
			AfxMessageBox(_T("请选择数据库服务名！"));
			return FALSE;
		}
	}
	else
	{
		//数据库选择的是Access数据库，要验证数据库文件
		if(m_strDBFile.IsEmpty())
		{
			AfxMessageBox(_T("请选择数据库文件！"));
			return FALSE;
		}
	}

	return TRUE;
}

void CYdConfigDBPage::OnBnClickedButtonTestConnect()
{
	// TODO: Add your control notification handler code here
	if(!ValidateData())
	{
		return ;
	}
	UpdateDBConfig();
	HRESULT hr = E_FAIL;
	CDatabaseEx DB;
	hr = DB.Open(m_pDBConfig);
	if(FAILED(hr))
	{
		CString strMsg;
		strMsg.Format(_T("连接数据库失败，返回值为0x%x"),hr);
		AfxMessageBox(strMsg);
		return;
	}
	AfxMessageBox(_T("数据库连接成功"));
}

HRESULT CYdConfigDBPage::UpdateDBConfig(BOOL _bUpdate /*= TRUE*/)
{
	HRESULT hr = E_FAIL;
	if(_bUpdate)
	{
		m_pDBConfig->m_uDBType = (CONFIG_DB_TYPE)m_iDBType;
		m_pDBConfig->m_uDBConnetMode = (CONFIG_DB_CONNET_MODE)m_iConnectionMode;
		m_pDBConfig->m_DBConnetParam.m_strSeverName = m_strDBServerName;
		m_pDBConfig->m_DBConnetParam.m_strDBFileName = m_strDBFile;
		m_pDBConfig->m_DBConnetParam.m_strUser = m_strUserName;
		m_pDBConfig->m_DBConnetParam.m_strPwd = m_strPwd;
		m_pDBConfig->m_DBConnetParam.m_uConectTime = m_sConnectTime;
	}
	else
	{
		m_iDBType = m_pDBConfig->m_uDBType;
		m_iConnectionMode = m_pDBConfig->m_uDBConnetMode;
		m_strDBServerName = m_pDBConfig->m_DBConnetParam.m_strSeverName;
		m_strDBFile = m_pDBConfig->m_DBConnetParam.m_strDBFileName;
		m_strUserName = m_pDBConfig->m_DBConnetParam.m_strUser;
		m_strPwd = m_pDBConfig->m_DBConnetParam.m_strPwd;
		m_sConnectTime = m_pDBConfig->m_DBConnetParam.m_uConectTime;
	}
	return S_OK;
}
void CYdConfigDBPage::OnBnClickedButtonSelDbfile()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Access数据库(*.mdb)|*.mdb|所有文件(*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	m_strDBFile = dlg.GetPathName();
	UpdateData(FALSE);
}
