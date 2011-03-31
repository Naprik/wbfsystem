// DlgUserMgr.cpp : implementation file
//

#include "stdafx.h"
#include "DlgUserMgr.h"
#include "../UIBase\ListXlsoutput.h"
#include "../\Base\AutoClean.h"
#include "../DBBase\DatabaseEx.h"
#include "../\ObjHelper\StaticObjHelper.h"
#include "../ObjRef\YDObjectRef.h"
#include "../Base\DataHandler.h"
#include "AuthorityHelper.h"
#include "DlgUserInfo.h"
#include "../DBBase\DBTransactionRef.h"
#include "../ObjRef\YDUserRef.h"


// CDlgUserMgr dialog

IMPLEMENT_DYNAMIC(CDlgUserMgr, CDialog)

CDlgUserMgr::CDlgUserMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserMgr::IDD, pParent)
{

}

CDlgUserMgr::~CDlgUserMgr()
{
	CListAutoClean<CYDObjectRef> clr(m_lstUserRef);
}

void CDlgUserMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_lstUser);
}


BEGIN_MESSAGE_MAP(CDlgUserMgr, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_USERMGR_ADD, &CDlgUserMgr::OnBnClickedButtonUsermgrAdd)
	ON_BN_CLICKED(IDC_BUTTON_USERMGR_MODIFY, &CDlgUserMgr::OnBnClickedButtonUsermgrModify)
	ON_BN_CLICKED(IDC_BUTTON_USERMGR_DEL, &CDlgUserMgr::OnBnClickedButtonUsermgrDel)
	ON_BN_CLICKED(IDC_BUTTON_USERMGR_OUTPUT, &CDlgUserMgr::OnBnClickedButtonUsermgrOutput)
END_MESSAGE_MAP()


// CDlgUserMgr message handlers

void CDlgUserMgr::OnBnClickedButtonUsermgrAdd()
{
	// TODO: Add your control notification handler code here
	CDlgUserInfo dlg;
	dlg.m_TypeOperation = OP_NEW;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	ASSERT(dlg.m_pUser);
	m_lstUserRef.push_back(dlg.m_pUser);
	HRESULT hr = E_FAIL;
	hr = AddUser(dlg.m_pUser);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	//���б��в���һ����¼
}

void CDlgUserMgr::OnBnClickedButtonUsermgrModify()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstUser.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("�����б���ѡ��Ҫ�޸ĵ��û���"));
		return ;
	}
	int nItem = m_lstUser.GetNextSelectedItem(pos);
	ASSERT(nItem >= 0);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_lstUser.GetItemData(nItem);
	ASSERT(pObjRef);
	CDlgUserInfo dlg;
	dlg.m_TypeOperation = OP_EDIT;
	dlg.m_pUser = pObjRef;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	hr = UpdateUser(nItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}

HRESULT CDlgUserMgr::UpdateUser(int _nItem)
{
	HRESULT hr = E_FAIL;
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_lstUser.GetItemData(_nItem);
	ASSERT(pObjRef);
	CAuthorityHelper helper;
	CString strName;
	hr = (pObjRef)->GetPropVal(FIELD_YDUSER_NAME,strName);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valAuthority;
	hr = (pObjRef)->GetPropVal(FIELD_YDUSER_AUTHORITY,&valAuthority);
	if(FAILED(hr))
	{
		return hr;
	}
	long lAuthority = CDataHandler::VariantToLong(valAuthority);
	CString strAuthority;
	hr = helper.ConvertString(lAuthority,strAuthority);
	if(FAILED(hr))
	{
		return hr;
	}
	m_lstUser.SetItemText(_nItem,1,strName);
	m_lstUser.SetItemText(_nItem,2,strAuthority);
	return S_OK;
}
void CDlgUserMgr::OnBnClickedButtonUsermgrDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstUser.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("�����б���ѡ��Ҫɾ�����û���"));
		return ;
	}
	int nItem = m_lstUser.GetNextSelectedItem(pos);
	ASSERT(nItem >= 0);
	CYDUserRef* pObjRef = (CYDUserRef*)m_lstUser.GetItemData(nItem);
	ASSERT(pObjRef);
	HRESULT hr = E_FAIL;
	//Ҫ�ж��û��Ƿ�ΪAdmin
	VARIANT_BOOL bAdmin = VARIANT_FALSE;
	hr = pObjRef->IsSysUser(&bAdmin);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	if(bAdmin)
	{
		AfxMessageBox(_T("ϵͳ����Ա����ɾ����"));
		return;
	}
	if(AfxMessageBox(_T("ȷ��Ҫɾ����ǰѡ�е��û���"),MB_YESNO) != IDYES)
	{
		return;
	}
	//�����ݿ���ɾ��
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB, TRUE);
	hr = pObjRef->Remove();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
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
	//listɾ��ѡ��ļ�¼
	m_lstUser.DeleteItem(nItem);
}

void CDlgUserMgr::OnBnClickedButtonUsermgrOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_lstUser);
	hr = ListXlsoutput.OutputExcel();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(hr == S_FALSE)
	{
		return ;
	}
	AfxMessageBox(_T("���XLS�ɹ���"));
}

BOOL CDlgUserMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lstUser.SetExtendedStyle(LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);
	m_lstUser.ClearHeadings();
	m_lstUser.SetHeadings(_T("���,50;�û���,80;Ȩ��,280"));
	HRESULT hr = E_FAIL;
	hr =InitUser();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgUserMgr::InitUser()
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDUSER, pDB, &m_lstUserRef);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstUserRef.begin();
		itr != m_lstUserRef.end();++itr)
	{
		hr = AddUser(*itr);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
INT_PTR CDlgUserMgr::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}

HRESULT CDlgUserMgr::AddUser(CYDObjectRef* _pUser)
{
	HRESULT hr= E_FAIL;
	CAuthorityHelper helper;
	ASSERT(_pUser);
	int iCount = m_lstUser.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"),iCount+1);
	CString strName;
	hr = (_pUser)->GetPropVal(FIELD_YDUSER_NAME,strName);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valAuthority;
	hr = (_pUser)->GetPropVal(FIELD_YDUSER_AUTHORITY,&valAuthority);
	if(FAILED(hr))
	{
		return hr;
	}
	long lAuthority = CDataHandler::VariantToLong(valAuthority);
	CString strAuthority;
	hr = helper.ConvertString(lAuthority,strAuthority);
	if(FAILED(hr))
	{
		return hr;
	}
	int index = m_lstUser.AddItem(iCount,strIndex,strName,strAuthority);
	m_lstUser.SetItemData(index,(DWORD_PTR)(_pUser));
	return S_OK;
}