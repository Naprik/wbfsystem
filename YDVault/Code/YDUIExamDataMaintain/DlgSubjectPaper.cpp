// DlgSubjectPaper.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSubjectPaper.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../\YDExamObjRef\YDSubjectRef.h"
#include "../DBBase\DBTransactionRef.h"

// CDlgSubjectPaper dialog

IMPLEMENT_DYNAMIC(CDlgSubjectPaper, CDialog)

CDlgSubjectPaper::CDlgSubjectPaper(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubjectPaper::IDD, pParent)
{
	m_pSubject = NULL;
}

CDlgSubjectPaper::~CDlgSubjectPaper()
{
	CListAutoClean<CYDObjectRef> clr(m_lstClear);
}

void CDlgSubjectPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALLPAPER, m_lstAllPaper);
	DDX_Control(pDX, IDC_LIST_SUBJECTPAPER, m_lstSubjectPaper);
}


BEGIN_MESSAGE_MAP(CDlgSubjectPaper, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSubjectPaper::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgSubjectPaper::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CDlgSubjectPaper message handlers

INT_PTR CDlgSubjectPaper::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState st;
	return CDialog::DoModal();
}

BOOL CDlgSubjectPaper::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lstAllPaper.InsertColumn(0,_T("试卷名称"), LVCFMT_LEFT,200);
	m_lstSubjectPaper.InsertColumn(0,_T("试卷名称"), LVCFMT_LEFT,200);
	ASSERT(m_pSubject);
	std::list<CYDObjectRef*> lstAllPaper;//所有的试卷
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_PAPER,pDB,&lstAllPaper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	
	for(std::list<CYDObjectRef*>::const_iterator itr = lstAllPaper.begin();
		itr != lstAllPaper.end();++itr)
	{
		m_lstClear.push_back(*itr);
		CString strPaperName;
		hr =(*itr)->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		int index = m_lstAllPaper.InsertItem(0,strPaperName);
		m_lstAllPaper.SetItemData(index,(DWORD_PTR)(*itr));
	}
	std::list<CYDObjectRef*> lstSubjectPaper;
	CYDSubjectRef* pSubjectRef = (CYDSubjectRef*)m_pSubject;
	ASSERT(pSubjectRef);
	hr = pSubjectRef->GetPapers(&lstSubjectPaper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstSubjectPaper.begin();
		itr != lstSubjectPaper.end();++itr)
	{
		m_lstClear.push_back(*itr);
		CString strPaperName;
		hr =(*itr)->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		int index = m_lstSubjectPaper.InsertItem(0,strPaperName);
		m_lstSubjectPaper.SetItemData(index,(DWORD_PTR)(*itr));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSubjectPaper::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstAllPaper.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("请在左边列表上至少选择一条记录！"));
		return;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB, TRUE);
	ASSERT(m_pSubject);
	CYDSubjectRef* pSubobject = (CYDSubjectRef*)m_pSubject;
	ASSERT(pSubobject);
	HRESULT hr = E_FAIL;
	while(pos)
	{
		int nItem = m_lstAllPaper.GetNextSelectedItem(pos);
		CYDObjectRef* pObj = (CYDObjectRef*)m_lstAllPaper.GetItemData(nItem);
		ASSERT(pObj);
		OBJID idObj = ID_EMPTY;
		hr = pObj->GetID(&idObj);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		BOOL bFind = FALSE;
		for(int i = 0; i < m_lstSubjectPaper.GetItemCount();i++)
		{
			CYDObjectRef* pSubObj = (CYDObjectRef*)m_lstSubjectPaper.GetItemData(i);
			ASSERT(pSubObj);
			OBJID idSubObj = ID_EMPTY;
			hr = pSubObj->GetID(&idSubObj);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			if(idSubObj == idObj)
			{
				bFind = TRUE;
				break;
			}
		}
		if(!bFind)
		{
			//在右边列表中插入一条记录
			CString strName;
			hr = pObj->GetPropVal(FIELD_YDPAPER_NAME,strName);
			int iCount = m_lstSubjectPaper.GetItemCount();
			int index = m_lstSubjectPaper.InsertItem(iCount,strName);
			m_lstSubjectPaper.SetItemData(index,(DWORD_PTR)pObj);
		}
		hr = pSubobject->AddPaper(pObj);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		m_lstAllPaper.DeleteItem(nItem);
		pos = m_lstAllPaper.GetFirstSelectedItemPosition();
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}

void CDlgSubjectPaper::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstSubjectPaper.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("请在右边列表上至少选择一条记录！"));
		return;
	}
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB, TRUE);
	ASSERT(m_pSubject);
	CYDSubjectRef* pSubobject = (CYDSubjectRef*)m_pSubject;
	ASSERT(pSubobject);
	while(pos)
	{
		int nItem = m_lstSubjectPaper.GetNextSelectedItem(pos);
		CYDObjectRef* pSubObj = (CYDObjectRef*)m_lstSubjectPaper.GetItemData(nItem);
		ASSERT(pSubObj);
		OBJID idSubObj = ID_EMPTY;
		hr = pSubObj->GetID(&idSubObj);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		BOOL bFind = FALSE;
		for(int i = 0; i < m_lstAllPaper.GetItemCount();i++)
		{
			CYDObjectRef* pAllObj = (CYDObjectRef*)m_lstAllPaper.GetItemData(i);
			ASSERT(pAllObj);
			OBJID idAllObj = ID_EMPTY;
			hr = pAllObj->GetID(&idAllObj);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			if(idSubObj == idAllObj)
			{
				bFind = TRUE;
				break;
			}
		}
		if(!bFind)
		{
			//在左边列表中插入一条记录
			CString strName;
			hr = pSubObj->GetPropVal(FIELD_YDPAPER_NAME,strName);
			int iCount = m_lstAllPaper.GetItemCount();
			int index = m_lstAllPaper.InsertItem(iCount,strName);
			m_lstAllPaper.SetItemData(index,(DWORD_PTR)pSubObj);
		}
		hr = pSubobject->RemovePaper(pSubObj);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		m_lstSubjectPaper.DeleteItem(nItem);
		pos = m_lstSubjectPaper.GetFirstSelectedItemPosition();
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}
