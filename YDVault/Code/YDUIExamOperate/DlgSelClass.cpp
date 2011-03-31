// DlgSelClass.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSelClass.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../Base\DataHandler.h"


// CDlgSelClass dialog

IMPLEMENT_DYNAMIC(CDlgSelClass, CDialog)

CDlgSelClass::CDlgSelClass(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelClass::IDD, pParent)
{

}

CDlgSelClass::~CDlgSelClass()
{
	CListAutoClean<CYDObjectRef> clr(m_lstClear);
}

void CDlgSelClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLASS, m_lstClass);
}


BEGIN_MESSAGE_MAP(CDlgSelClass, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSelClass::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSelClass message handlers

BOOL CDlgSelClass::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWaitCursor wait;

	// TODO:  Add extra initialization here
	m_lstClass.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstClass.InsertColumn(0,_T("班级编号"),LVCFMT_LEFT,150);
	m_lstClass.InsertColumn(1,_T("班级名称"),LVCFMT_LEFT,250);
	HRESULT hr = E_FAIL;
	hr  = InitClass();
	if (FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgSelClass::InitClass()
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstClass;//班级
	hr = CStaticObjHelper::GetObjRef(DB_YDCLASS,pDB,&lstClass);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstClass.begin();
		itr != lstClass.end();++itr)
	{
		m_lstClear.push_back(*itr);
		CComVariant valClassID;
		hr = (*itr)->GetPropVal(FIELD_YDCLASS_CLASSID,&valClassID);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strClassID = CDataHandler::VariantToString(valClassID);
		int index = m_lstClass.InsertItem(0,strClassID);
		CComVariant valClassName;
		hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,&valClassName);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strClassName = CDataHandler::VariantToString(valClassName);
		m_lstClass.SetItemText(index,1,strClassName);
		m_lstClass.SetItemData(index,(DWORD_PTR)(*itr));
	}

	return S_OK;
}

void CDlgSelClass::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstClass.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("请至少选择一个班级!"));
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_lstClass.GetNextSelectedItem(pos);
			CYDObjectRef* pObjRef = (CYDObjectRef*)m_lstClass.GetItemData(nItem);
			ASSERT(pObjRef);
			m_lstSelClass.push_back(pObjRef);
			TRACE1("Item %d was selected!\n", nItem);
			// you could do your own processing on nItem here
		}
	}


	OnOK();
}

INT_PTR CDlgSelClass::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}
