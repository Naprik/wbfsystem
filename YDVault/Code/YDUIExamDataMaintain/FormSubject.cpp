// FormSubject.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormSubject.h"

#include "../YDExamObjRef/YDSubjectRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../YDExamObjRef/YDClassRef.h"
#include "DlgSubject.h"
#include "DlgSubjectPaper.h"
#include "../UIBase\ListXlsoutput.h"

// CFormSubject

IMPLEMENT_DYNCREATE(CFormSubject, CYdFormView)

CFormSubject::CFormSubject()
	: CYdFormView(CFormSubject::IDD)
{

}

CFormSubject::~CFormSubject()
{
	CListAutoClean<CYDObjectRef> clr(m_lstSubjectCache);
}

void CFormSubject::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DM_SUBJECT, m_SubjectListCtrl);
}

BEGIN_MESSAGE_MAP(CFormSubject, CYdFormView)
	ON_BN_CLICKED(BTN_DM_SUBJ_NEW, &CFormSubject::OnBnClickedDmSubjNew)
	ON_BN_CLICKED(BTN_DM_SUBJ_EDIT, &CFormSubject::OnBnClickedDmSubjEdit)
	ON_BN_CLICKED(BTN_DM_SUBJ_DEL, &CFormSubject::OnBnClickedDmSubjDel)
	ON_BN_CLICKED(BTN_DM_SUBJ_EDITPAPER, &CFormSubject::OnBnClickedDmSubjEditpaper)
	ON_BN_CLICKED(BTN_DM_SUBJ_OUTPUT, &CFormSubject::OnBnClickedDmSubjOutput)
END_MESSAGE_MAP()


// CFormSubject diagnostics

#ifdef _DEBUG
void CFormSubject::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormSubject::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormSubject message handlers

void CFormSubject::OnBnClickedDmSubjNew()
{
	CDlgSubject	dlg(OP_NEW);
	
	if(dlg.DoModal() == IDOK)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDObjectRef* pSubjectRef = new CYDSubjectRef(pDB);
		CComVariant varId(dlg.m_subjectid);
		pSubjectRef->SetPropVal(FIELD_YDSUBJECT_ID, &varId);
		CComVariant varName(dlg.m_subject);
		pSubjectRef->SetPropVal(FIELD_YDSUBJECT_NAME, &varName);
		CDBTransactionRef trans(pDB, TRUE);
		HRESULT hr = pSubjectRef->Save();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = trans.Commit();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		AddSubjectItem(pSubjectRef);
	}
		
}

void CFormSubject::OnBnClickedDmSubjEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_SubjectListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一个考试科目！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_SubjectListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_SubjectListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjSubject = (CYDObjectRef*)m_SubjectListCtrl.GetItemData(nItem);
	ASSERT(pObjSubject != NULL);
	CComVariant valID;
	hr = pObjSubject->GetPropVal(FIELD_YDSUBJECT_ID, &valID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = pObjSubject->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strName = CDataHandler::VariantToString(valName);

	
	CDlgSubject	dlg(OP_EDIT, this);
	dlg.m_subject = strName;
	dlg.m_subjectid = strID;
	if(dlg.DoModal() == IDOK)
	{
		CYDSubjectRef* pSubjectRef = (CYDSubjectRef*)pObjSubject;

		CComVariant valID(dlg.m_subjectid);
		hr = pSubjectRef->SetPropVal(FIELD_YDDEPARTMENT_ID,&valID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valName(dlg.m_subject);
		hr = pSubjectRef->SetPropVal(FIELD_YDDEPARTMENT_NAME,&valName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CDBTransactionRef transRef(pDB,TRUE);
		hr = pSubjectRef->Update();
		if(FAILED(hr))
		{
			transRef.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}

		hr = transRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		m_SubjectListCtrl.SetItemText(nItem,1,dlg.m_subjectid);
		m_SubjectListCtrl.SetItemText(nItem,2,dlg.m_subject);
	}
}

void CFormSubject::OnBnClickedDmSubjDel()
{
	int nCount = m_SubjectListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一个考试科目！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_SubjectListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_SubjectListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_SubjectListCtrl.GetItemData(nItem);
	ASSERT(pObjRef );
	if (AfxMessageBox(_T("确定要删除选择的考试科目吗？"), MB_OKCANCEL) != IDOK)
		return;	
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = pObjRef->Remove();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_SubjectListCtrl.DeleteItem(nItem);
}

void CFormSubject::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	m_SubjectListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_SubjectListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_SubjectListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_SubjectListCtrl.InsertColumn(1, L"科目编号", LVCFMT_LEFT,iWidth*2/6);
	m_SubjectListCtrl.InsertColumn(2, L"科目名称", LVCFMT_LEFT,iWidth*3/6 -2 );

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstSubject;
	HRESULT hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT, pDB, &lstSubject);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	for(std::list<CYDObjectRef*>::const_iterator itr = lstSubject.begin();
		itr != lstSubject.end();++itr)
	{
		hr = AddSubjectItem(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
}

HRESULT CFormSubject::AddSubjectItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valSubjectID;
	hr = _pref->GetPropVal(FIELD_YDSUBJECT_ID, &valSubjectID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strSubjectID = CDataHandler::VariantToString(valSubjectID);

	CComVariant valSubjectName;
	hr = _pref->GetPropVal(FIELD_YDSUBJECT_NAME,&valSubjectName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strSubjectName = CDataHandler::VariantToString(valSubjectName);

	
	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_SubjectListCtrl.GetItemCount()+1);
	int index = m_SubjectListCtrl.InsertItem(m_SubjectListCtrl.GetItemCount(), strIndex);
	m_SubjectListCtrl.SetItemText(index,1, strSubjectID);
	m_SubjectListCtrl.SetItemText(index,2, strSubjectName);
	m_SubjectListCtrl.SetItemData(index, (DWORD_PTR)_pref);
	m_lstSubjectCache.push_back(_pref);

	return S_OK;
}
BOOL CFormSubject::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormSubject::OnBnClickedDmSubjEditpaper()
{
	// TODO: Add your control notification handler code here
	int nCount = m_SubjectListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一个考试科目！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_SubjectListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_SubjectListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjSubject = (CYDObjectRef*)m_SubjectListCtrl.GetItemData(nItem);
	ASSERT(pObjSubject != NULL);
	CDlgSubjectPaper dlg;
	dlg.m_pSubject = pObjSubject;
	dlg.DoModal();
}

void CFormSubject::OnBnClickedDmSubjOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_SubjectListCtrl);
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
	AfxMessageBox(_T("输出XLS成功！"));
}
