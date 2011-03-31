// YdConfigFtpPage.cpp : implementation file
//

#include "stdafx.h"
#include "YdConfig.h"
#include "YdConfigFtpPage.h"
#include "YdConfigSheet.h"
#include "../FtpBase/FtpRef.h"


// CYdConfigFtpPage dialog

IMPLEMENT_DYNAMIC(CYdConfigFtpPage, CPropertyPage)

CYdConfigFtpPage::CYdConfigFtpPage()
	: CPropertyPage(CYdConfigFtpPage::IDD)
	, m_iServerType(0)
	, m_strFtpAddr(_T(""))
	, m_strUser(_T(""))
	, m_strPwd(_T(""))
	, m_sPort(0)
{

}

CYdConfigFtpPage::~CYdConfigFtpPage()
{
}

void CYdConfigFtpPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SERVER_TYPE, m_iServerType);
	DDX_Text(pDX, IDC_EDIT_FTP_ADDR, m_strFtpAddr);
	DDX_Text(pDX, IDC_EDIT_FTP_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_FTP_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_FTP_PORT, m_sPort);
}


BEGIN_MESSAGE_MAP(CYdConfigFtpPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CONNET, &CYdConfigFtpPage::OnBnClickedButtonTestConnet)
END_MESSAGE_MAP()


// CYdConfigFtpPage message handlers

BOOL CYdConfigFtpPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CYdConfigSheet* pSheet = static_cast<CYdConfigSheet*>(GetParent());
	pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	return CPropertyPage::OnSetActive();
}

void CYdConfigFtpPage::OnBnClickedButtonTestConnet()
{
	// TODO: Add your control notification handler code here
	if(!ValidateData())
	{
		return ;
	}
	UpdateFtpConfig();
	CFtpRef FtpRef;
	HRESULT hr = E_FAIL;
	hr = FtpRef.TestConnect(m_pFtpConfig);
	if(FAILED(hr))
	{
		CString strMsg;
		strMsg.Format(_T("连接出错，错误值为：0x%x"),hr);
		AfxMessageBox(strMsg);
		return;
	}
	AfxMessageBox(_T("连接成功"));
}

BOOL	CYdConfigFtpPage::ValidateData()
{
	UpdateData();
	if(m_iServerType == -1)
	{
		AfxMessageBox(_T("请选择服务器类型"));
		return FALSE;
	}
	if(m_strFtpAddr.IsEmpty())
	{
		AfxMessageBox(_T("请输入FTP的地址"));
		return FALSE;
	}
	return TRUE;
}

HRESULT CYdConfigFtpPage::UpdateFtpConfig(BOOL _bUpdate/* = TRUE*/)
{
	if(_bUpdate)
	{
		m_pFtpConfig->m_serverMode = (CONFIG_SEVER_MODE)m_iServerType;
		m_pFtpConfig->m_FtpServerParam.m_strFtpAddr = m_strFtpAddr;
		m_pFtpConfig->m_FtpServerParam.m_strUser = m_strUser;
		m_pFtpConfig->m_FtpServerParam.m_strPwd = m_strPwd;
		m_pFtpConfig->m_FtpServerParam.m_uPort = m_sPort;
	}
	else
	{
		m_iServerType = m_pFtpConfig->m_serverMode;
		m_strFtpAddr = m_pFtpConfig->m_FtpServerParam.m_strFtpAddr;
		m_strUser = m_pFtpConfig->m_FtpServerParam.m_strUser;
		m_strPwd = m_pFtpConfig->m_FtpServerParam.m_strPwd;
		m_sPort = m_pFtpConfig->m_FtpServerParam.m_uPort;
	}
	return S_OK;
}
BOOL CYdConfigFtpPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CYdConfigSheet* pSheet = static_cast<CYdConfigSheet*>(GetParent());
	ASSERT(pSheet);
	m_pFtpConfig = &pSheet->m_FtpConfig;
	HRESULT hr = E_FAIL;
	hr = m_pFtpConfig->Read();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("FTP配置有错误！"));
		return FALSE;
	}
	UpdateFtpConfig(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CYdConfigFtpPage::OnWizardFinish()
{
	// TODO: Add your specialized code here and/or call the base class
	if(!ValidateData())
	{
		return FALSE;
	}
	UpdateFtpConfig();
	HRESULT hr = E_FAIL;
	CYdConfigSheet* pSheet = static_cast<CYdConfigSheet*>(GetParent());
	ASSERT(pSheet);
	hr = pSheet->Save();
	if(FAILED(hr))
	{
		CString strMsg;
		strMsg.Format(_T("保存配置文件出错，错误值为：0x%x"),hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	return CPropertyPage::OnWizardFinish();
}
