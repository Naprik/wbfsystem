// UserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "UserInfoDlg.h"
#include "afxdialogex.h"
#include "../ObjRef/YDUserRef.h"
#include "../Base/DataHandler.h"
#include "../YDUIUserManagement/StaticYdUser.h"
#include "../DBBase/DBTransactionRef.h"

// CUserInfoDlg dialog

IMPLEMENT_DYNAMIC(CUserInfoDlg, CDialogEx)

CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserInfoDlg::IDD, pParent) 
	, m_strUserName(_T(""))
	, m_strUserPwd(_T(""))
	, m_iAge(0)
	, m_strPhone(_T(""))
	, m_strMail(_T(""))
	, m_strID(_T(""))
	, m_strQQ(_T(""))
{

}

CUserInfoDlg::~CUserInfoDlg()
{
}

void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_strUserPwd);
	DDX_Control(pDX, IDC_DM_STU_GENDER, m_gender);
	DDX_Text(pDX, IDC_DM_STU_AGE, m_iAge);
	DDX_Text(pDX, IDC_DM_STU_TEL, m_strPhone);
	DDX_Text(pDX, IDC_DM_STU_EMIAL, m_strMail);
	DDX_Text(pDX, IDC_DM_STU_ID, m_strID);
	DDX_Text(pDX, IDC_DM_STU_QQ, m_strQQ);
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUserInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserInfoDlg message handlers


BOOL CUserInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_gender.AddString(L"��");
	m_gender.AddString(L"Ů");

	HRESULT hr = E_FAIL;
	
	//Ҫ�����Ƿ�ΪAdmin�û�������ǣ������޸��û���

	hr = CStaticYdUser::Instance()->GetCurUser(m_pUser);
	if (FAILED(hr))
	{
		return FALSE;
	}
	
	VARIANT_BOOL bAdmin = VARIANT_FALSE;
	hr = m_pUser->IsSysUser(&bAdmin);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if(bAdmin)
	{
		AfxMessageBox(_T("��ǰ�û�Ϊϵͳ����Ա�������Խ��д˲���!"));
		CDialogEx::OnOK();
		return FALSE;
	}
	//�����û���Ϣ
	hr = m_pUser->GetPropVal(FIELD_YDUSER_NAME,m_strUserName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//pwd
	hr = m_pUser->GetPropVal(FIELD_YDUSER_PASSWORD,m_strUserPwd);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//id
	hr = m_pUser->GetPropVal(FIELD_YDUSER_ID, m_strID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//�Ա�
		
	m_pUser->GetGender(&m_strGender);
	int genderIndex = m_gender.FindStringExact(0, m_strGender);
	m_gender.SetCurSel(genderIndex);
	//age
	CComVariant varAge;
	hr = m_pUser->GetPropVal(FIELD_YDUSER_AGE, &varAge);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	m_iAge = CDataHandler::VariantToLong(varAge);
	//phone
	hr = m_pUser->GetPropVal(FIELD_YDUSER_MPHONE, m_strPhone);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//email
	hr = m_pUser->GetPropVal(FIELD_YDUSER_EMAIL, m_strMail);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//qq
	hr = m_pUser->GetPropVal(FIELD_YDUSER_QQ, m_strQQ);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CUserInfoDlg::Validate(VARIANT_BOOL* _bValidate)
{
	*_bValidate = VARIANT_FALSE;
	HRESULT hr = E_FAIL;
	if(m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("���Ʋ���Ϊ��!"));
		return S_FALSE;
	}
	
	//Ҫ�ж��û�������ΪAdmin
	if(m_strUserName.CompareNoCase(_T("Admin")) == 0)
	{
		AfxMessageBox(_T("�û����Ʋ���ΪAdmin,����ϵͳ����Ա������ͬ!"));
		return S_FALSE;
	}
	
	*_bValidate = VARIANT_TRUE;
	return S_OK;
}

void CUserInfoDlg::OnBnClickedOk()
{
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
	CYDUserRef* pUserRef = (CYDUserRef*)m_pUser;
	int index = m_gender.GetCurSel();
	if(index != -1)
		m_gender.GetLBText(index,m_strGender);
	hr = pUserRef->SetGender(m_strGender);
	//age
	CComVariant varAge((long)m_iAge);
	hr = pUserRef->SetPropVal(FIELD_YDUSER_AGE, &varAge);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	//phone
	CComVariant valID;
	CDataHandler::StringToVariant(m_strPhone,VT_BSTR,&valID);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_ID, &valID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	//phone
	CComVariant valPhone;
	CDataHandler::StringToVariant(m_strPhone,VT_BSTR,&valPhone);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_MPHONE, &valPhone);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	//email
	CComVariant valMail;
	CDataHandler::StringToVariant(m_strMail,VT_BSTR,&valMail);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_EMAIL, &valMail);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	//qq
	CComVariant valQQ;
	CDataHandler::StringToVariant(m_strQQ,VT_BSTR,&valQQ);
	hr = m_pUser->SetPropVal(FIELD_YDUSER_QQ, &valQQ);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}


	//���浽���ݿ�
	CDBTransactionRef trans(pDB, TRUE);
	
	hr = m_pUser->Update();
	
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
	AfxGetMainWnd()->SendMessage(WM_YD_UPDATE_PERSIONINFO, (WPARAM)(&TREE_NODE_USER_INFO),0);
	CDialogEx::OnOK();
}
