// DlgKnowledge.cpp : implementation file
//

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "DlgKnowledge.h"
#include "../Include/ShowErr.h"
#include "../Base/DataHandler.h"
#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YdKnowledge.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjRef/YDObjWrapper.h"


// CDlgKnowledge dialog

IMPLEMENT_DYNAMIC(CDlgKnowledge, CDialog)

CDlgKnowledge::CDlgKnowledge(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKnowledge::IDD, pParent)
	, m_strName(_T(""))
	, m_strCode(_T(""))
	, m_strCreator(_T(""))
	, m_strDescription(_T(""))
	, m_strCreateDate(_T(""))
{
	m_pObjWrapper = NULL;
	m_pParentObjRef  = NULL;
	m_uType = OP_VIEW;
	COleDateTime dt = COleDateTime::GetCurrentTime();
	_variant_t valCreateDate;
	valCreateDate.vt = VT_DATE;
	valCreateDate.date = dt;
	m_strCreateDate = CDataHandler::VariantToString(valCreateDate);
	m_strCreator = DEFAULT_CREATOR;
}

CDlgKnowledge::~CDlgKnowledge()
{
}

void CDlgKnowledge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CODE, m_strCode);
	DDX_Text(pDX, IDC_EDIT_CREATOR, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_CREATE_DATE, m_strCreateDate);
}


BEGIN_MESSAGE_MAP(CDlgKnowledge, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgKnowledge::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgKnowledge message handlers

void CDlgKnowledge::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYdKnowledge kng(pDB);
	CString strNameOrign = m_strName;
	CString strCodeOrign = m_strCode;

	BOOL bExist = FALSE;
	OBJID vaultID;
	m_pParentObjRef->GetID(&vaultID);
	UpdateData();
	if(m_strName.IsEmpty())
	{
		AfxMessageBox(_T("知识点名称不能为空！"));
		GetDlgItem(IDC_EDIT_NAME)->SetFocus();
		return;
	}
	if (m_uType == OP_NEW || (m_uType == OP_EDIT && 0 != m_strName.CompareNoCase(strNameOrign)))
	{
		kng.IsFieldExistOfVault(L"Name", m_strName, vaultID, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"当前名称已经存在，请重新输入！");
			m_strName = L"";
			UpdateData(FALSE);
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			return;
		}
		
	}
	if (m_uType == OP_NEW || (m_uType == OP_EDIT && 0 != m_strName.CompareNoCase(strCodeOrign)))
	{
		kng.IsFieldExistOfVault(L"Code", m_strCode, vaultID, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"当前编号已经存在，请重新输入！");
			m_strCode = L"";
			UpdateData(FALSE);
			GetDlgItem(IDC_EDIT_CODE)->SetFocus();
			return;
		}
	}
	HRESULT hr = E_FAIL;
	
	
	CDBTransactionRef TransRef(pDB,TRUE);
	if(m_uType == OP_NEW)
	{
		//新建一个知识点
		ASSERT(m_pObjWrapper == NULL);
		m_pObjWrapper = new CYdObjWrapper();
		m_pObjWrapper->m_pObjRef = new CYdKnowledge(pDB);

		hr = FillProp(m_pObjWrapper->m_pObjRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		
		m_pObjWrapper->m_pLinkRef = new CYDLinkRef(pDB,DB_QVAULTKPOINT);
		hr = m_pObjWrapper->m_pLinkRef->PutObjRef(m_pParentObjRef,m_pObjWrapper->m_pObjRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = m_pObjWrapper->Save();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	else if(m_uType == OP_EDIT)
	{
		//更新知识点
		ASSERT(m_pObjWrapper != NULL);
		hr= FillProp(m_pObjWrapper->m_pObjRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = m_pObjWrapper->Update();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	OnOK();
}

HRESULT CDlgKnowledge::FillProp(CYDObjectRef* _pObjRef)
{
	HRESULT hr = E_FAIL;
	CComVariant valName ;
	CDataHandler::StringToVariant(m_strName,VT_BSTR,&valName);
	hr = _pObjRef->SetPropVal(L"Name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valCode;
	CDataHandler::StringToVariant(m_strCode,VT_BSTR,&valCode);
	hr = _pObjRef->SetPropVal(L"CODE",&valCode);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valCreator;
	CDataHandler::StringToVariant(m_strCreator,VT_BSTR,&valCreator);
	hr = _pObjRef->SetPropVal(L"CREATOR",&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCreateDate;
	hr = CDataHandler::StringToVariant(m_strCreateDate, VT_DATE, &valCreateDate);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = _pObjRef->SetPropVal(L"CREATEDATE",&valCreateDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valDescription;
	CDataHandler::StringToVariant(m_strDescription,VT_BSTR,&valDescription);
	hr = _pObjRef->SetPropVal(L"DESCRIPTION",&valDescription);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
BOOL CDlgKnowledge::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_uType == OP_EDIT || m_uType == OP_VIEW)
	{
		//当前是更新，要将属性填进去
		ASSERT(m_pObjWrapper != NULL);
		HRESULT hr = E_FAIL;
		CComVariant valName;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"Name",&valName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		m_strName = CDataHandler::VariantToString(valName);
		CComVariant valCode;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"CODE",&valCode);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		m_strCode = CDataHandler::VariantToString(valCode);
		CComVariant valCreator;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"CREATOR",&valCreator);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		m_strCreator = CDataHandler::VariantToString(valCreator);
		if (m_strCreator.GetLength() <= 0)
		{
			m_strCreator = DEFAULT_CREATOR;
		}
		CComVariant valCreateDate;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		if (valCreateDate.vt == VT_EMPTY)
		{
			COleDateTime date = COleDateTime::GetCurrentTime();
			valCreateDate.vt = VT_DATE;
			valCreateDate.date = date;
		}
		m_strCreateDate = CDataHandler::VariantToString(valCreateDate);
		CComVariant valDescription;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"DESCRIPTION",&valDescription);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		m_strDescription = CDataHandler::VariantToString(valDescription); 		m_strDescription = CDataHandler::VariantToString(valDescription);

	}
	UpdateData(FALSE);
	ReadOnlyDlg(m_uType == OP_VIEW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgKnowledge::ReadOnlyDlg(BOOL _bReadOnly)
{
	HRESULT hr = E_FAIL;
	((CEdit*)(GetDlgItem(IDC_EDIT_NAME)))->SetReadOnly(_bReadOnly);
	((CEdit*)(GetDlgItem(IDC_EDIT_CODE)))->SetReadOnly(_bReadOnly);
	((CEdit*)(GetDlgItem(IDC_EDIT_DESCRIPTION)))->SetReadOnly(_bReadOnly);
	return S_OK;
}
INT_PTR CDlgKnowledge::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}
