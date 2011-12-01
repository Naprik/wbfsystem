// ObjPropertySheet.cpp: implementation of the CObjPropertySheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "ObjPropertySheet.h"
#include "ObjPropertyView.h"
// #include "QuestionVaultClassificationFormView.h"
// #include "QuestionTypeFormView.h"
// #include "PaperFolderFormView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDLinkRef.h"
// #include "QuestionVaultBaseFormView.h"
// #include "QuestionVaultKnowledgeFormView.h"
// #include "QuestionVaultQTypeFormView.h"
// #include "QuestionListFormView.h"
// #include "AtricleQuestionFormView.h"
// #include "QuestionKnowledgeListFormView.h"
// #include "../Base/DataHandler.h"
// #include "GeneratePaperFormView.h"
//#include "DlgFmView.h"
// #include "../YDUIExam/FormDepartment.h"
#include "YdFormView.h"
#include "ObjPropShow.h"
#include "../Include\ExtDllState.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CObjPropertySheet, CObject)

CObjPropertySheet::CObjPropertySheet()
{
	m_pPropertyView = NULL;
	m_nOPERATION = OP_VIEW;
	m_pObjWrapper = NULL;

	m_bDelete = FALSE;
}

CObjPropertySheet::CObjPropertySheet(const CObjPropertyView* pView)
: m_pPropertyView((CObjPropertyView*)pView)
{
	m_nOPERATION = OP_VIEW;
	m_pObjWrapper = NULL;
	m_bDelete = FALSE;
}

CObjPropertySheet::~CObjPropertySheet()
{

}

HRESULT CObjPropertySheet::Show(CString sTipName, OPERATION nOp)
{
	HRESULT hr = E_FAIL;
	CObjPropShow ObjPropShow;
	ObjPropShow.m_type = SHOW_STRING;
	ObjPropShow.m_strName = sTipName;
	ObjPropShow.m_pSheet = this;
	hr = AfxGetMainWnd()->SendMessage(WM_YD_ADD_PAGE,WPARAM(&ObjPropShow));
	if(FAILED(hr))
	{
		return hr;
	}
	m_nOPERATION = nOp;
	UINT iCount = GetPageCount();
	for(UINT i = 0;i < iCount;i++)
	{
		CYdFormView* pYdFromView = GetPage(i);
		ASSERT(pYdFromView);
		pYdFromView->OnInitialUpdate();
		pYdFromView->ReadonlyPage(nOp == OP_VIEW);
		hr = pYdFromView->UpdateProp(TRUE);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CObjPropertySheet::UpdateShow()
{
	HRESULT hr = E_FAIL;
	UINT iCount = GetPageCount();
	for(UINT i = 0;i < iCount;i++)
	{
		CYdFormView* pYdFromView = GetPage(i);
		ASSERT(pYdFromView);
		hr = pYdFromView->UpdateProp(TRUE);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}


HRESULT CObjPropertySheet::Show(CYdObjWrapper* _pObjWrapper,OPERATION nOp)
{
	HRESULT hr = E_FAIL;
	CObjPropShow ObjPropShow;
	ObjPropShow.m_type = SHOW_OBJECT;
	ObjPropShow.m_pObjWrapper = _pObjWrapper;
	ObjPropShow.m_pSheet = this;
	ObjPropShow.m_nOp = nOp;
	AfxGetMainWnd()->SendMessage(WM_YD_ADD_PAGE,WPARAM(&ObjPropShow));


	
	m_nOPERATION = nOp;
	m_pObjWrapper = _pObjWrapper;
	UINT iCount = GetPageCount();
	for(UINT i = 0;i < iCount;i++)
	{
		CYdFormView* pYdFromView = GetPage(i);
		ASSERT(pYdFromView);
		pYdFromView->OnInitialUpdate();
		pYdFromView->ReadonlyPage(nOp == OP_VIEW);
		hr = pYdFromView->UpdateProp(TRUE);
		if(FAILED(hr))
		{
			return hr;
		}
	}


	return S_OK;
}

HRESULT CObjPropertySheet::ShowPropertyView(CYdObjWrapper* _pObjWrapper,OPERATION nOp)
{
	HRESULT hr = CObjPropertySheet::Show(_pObjWrapper,nOp);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CObjPropertySheet::Save()
{
	_ASSERT(m_pObjWrapper!=NULL);

	HRESULT hr = E_FAIL;

	//获取窗口中各个component的属性值，存入属性集prop_set
	UINT nPropCount = this->GetPropertyCount();
	UINT nCount = this->GetPageCount();
	int index = 0;
	BOOL bFoundPartType = FALSE;
	CString strPartType(_T(""));
	for(UINT i=0;i<nCount;i++)
	{
		CYdFormView* pYdFormView = this->GetPage(i);
		if(!pYdFormView->UpdateData())  //值校验不通过
		{
			//hr = E_INVALID_INPUT_VALUE;
			return hr;
		}
		hr = pYdFormView->UpdateProp(FALSE);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	ASSERT(m_pObjWrapper);
	hr = m_pObjWrapper->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	for(UINT i=0;i<nCount;i++)
	{
		CYdFormView* pYdFormView = this->GetPage(i);
		hr = pYdFormView->Save();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

BOOL CObjPropertySheet::ValidateData(OPERATION nOp)
{
	for(UINT i=0;i<GetPageCount();i++)
	{
		CYdFormView* pYdFormView = this->GetPage(i);
		if(!pYdFormView->ValidateData(nOp))
		{
			return FALSE;
		}
	}
	return TRUE;
}

HRESULT CObjPropertySheet::Update()
{
	_ASSERT(m_pObjWrapper!=NULL);
	
	HRESULT hr = E_FAIL;
	//获取窗口中各个component的属性值，存入属性集prop_set
	UINT nPropCount = this->GetPropertyCount();
	UINT nCount = this->GetPageCount();
	int index = 0;
	BOOL bFoundPartType = FALSE;
	CString strPartType(_T(""));
	for(UINT i=0;i<nCount;i++)
	{
		CYdFormView* pYdFormView = this->GetPage(i);
		if(!pYdFormView->UpdateData())  //值校验不通过
		{
			//hr = E_INVALID_INPUT_VALUE;
			return hr;
		}
		hr = pYdFormView->UpdateProp(FALSE);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	ASSERT(m_pObjWrapper);
	hr = m_pObjWrapper->Update();
	if(FAILED(hr))
	{
		return hr;
	}
	for(UINT i=0;i<nCount;i++)
	{
		CYdFormView* pYdFormView = this->GetPage(i);
		hr = pYdFormView->Update();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CObjPropertySheet::Close()
{
	return S_OK;
}

HRESULT CObjPropertySheet::ResetOPERATION(OPERATION nOp)
{
	m_nOPERATION = nOp;
	return S_OK;
}

UINT CObjPropertySheet::GetPageCount()
{
	return m_listPages.size();
}

CYdFormView* CObjPropertySheet::GetPage(UINT nIndex)
{
	if(nIndex<0 || nIndex >=m_listPages.size()) return NULL;

	std::list<CYdFormView*>::iterator itr = m_listPages.begin();
	for(UINT i=0;i<nIndex;i++) ++itr;
	return *itr;
}

void CObjPropertySheet::ActivePage(int nIndex)
{

}

void CObjPropertySheet::ReadonlyPages(BOOL bReadonly)
{
	UINT nCount = GetPageCount();
	for(UINT i=0;i<nCount;i++)
	{
		CYdFormView* pPage = GetPage(i);
		_ASSERT(pPage != NULL && pPage->GetSafeHwnd()!=NULL);
		pPage->ReadonlyPage(bReadonly);
	}
}

UINT CObjPropertySheet::GetPropertyCount()
{
	UINT nCount = 0;
	for(UINT i = 0;i< GetPageCount(); i++)
	{
		nCount += GetPage(i)->GetPropertyCount();
	}
	return nCount;
}

CYdFormView* CObjPropertySheet::AddPage(CRuntimeClass* pViewClass, const CString& strViewLabel,
				int iIndex, CCreateContext* pContext)
{
	_ASSERT(m_pPropertyView != NULL);
	UINT nIndex = m_pPropertyView->AddView(pViewClass,strViewLabel,iIndex,pContext);
	_ASSERT(nIndex != -1);
	CYdFormView* pPage = (CYdFormView*)m_pPropertyView->GetTabControl().GetTabWnd(nIndex);
	_ASSERT(pPage != NULL);
	pPage->SetPropertySheet(this);
	m_listPages.push_back(pPage);

	return pPage;
}

void CObjPropertySheet::RemovePage(CYdFormView* pPage)
{
	_ASSERT(m_pPropertyView != NULL);
	UINT nIndex = m_pPropertyView->FindTab(pPage->m_hWnd);
	_ASSERT(nIndex != -1);
	m_pPropertyView->RemoveView(nIndex);
	m_listPages.remove(pPage);
	pPage->DestroyWindow();
}





void CObjPropertySheet::SetPreTabIndex(int index)
{

}

int CObjPropertySheet::GetPreTabIndex()
{
	return 0;
}
