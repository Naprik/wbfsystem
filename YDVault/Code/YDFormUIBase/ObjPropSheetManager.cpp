// ObjPropViewManager.cpp: implementation of the CObjPropSheetManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "resource.h"

#include "ObjPropSheetManager.h"


#include "../include/ShowErr.h"


#include "ObjPropertyView.h"
#include "../Base/DataHandler.h"
#include "../Include\YDExamComm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_VIEW_NUMBER 10 //最多能同时打开10个窗口

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjPropSheetManager::CObjPropSheetManager()
{
	m_nMaxViewNum = 0;
}

CObjPropSheetManager::~CObjPropSheetManager()
{

}

HRESULT CObjPropSheetManager::Show(CYdObjWrapper* pWrapper, OPERATION nOp,
											  CObjPropertySheet* &_pSheet)
{
	HRESULT hr = E_FAIL;
	_pSheet = NULL;
	UINT objid = 0;
	if(pWrapper)
	{
		hr = pWrapper->m_pObjRef->GetID(&objid);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr;
	itr = m_mapObjSheet.find(pWrapper);
	if(itr != m_mapObjSheet.end() && objid != 0)  //该对象已经被打开
	{
		CObjPropertySheet* pSheet = (*itr).second;
		CMDIChildWnd* pChildFrm = (CMDIChildWnd*)pSheet->GetParentView()->GetParent();
		_ASSERT(pChildFrm!=NULL );
		pChildFrm->SetActiveView((CView*)pSheet->GetPage(0));
		OPERATION nOldOp;
		pSheet->GetOPERATION(nOldOp);
		if(nOldOp != nOp)  //
		{
			if(nOldOp == OP_VIEW)
			{
				//_ASSERT(nOp == OP_EDIT || nOp == CHECKOUT_EDIT);
				pSheet->ReadonlyPages(FALSE);
				if( nOp == OP_EDIT )
				{
					pChildFrm->SendMessage(WM_YD_UPDATE_BELOWBAR,WPARAM(nOp));
					pChildFrm->SendMessage(WM_YD_SHOW_BELOWBAR,WPARAM(TRUE));
					pChildFrm->RecalcLayout();
				}
			}
			pSheet->ResetOPERATION(nOp);
		}
		pChildFrm->MDIActivate();
		_pSheet =  (*itr).second;
		return S_OK;
	}
	else
	{
		if(m_nMaxViewNum >= MAX_VIEW_NUMBER)
		{
			CObjPropertySheet* pOldestSheet = NULL;
			for(itr=m_mapObjSheet.begin();itr!=m_mapObjSheet.end();++itr)
			{
				CObjPropertySheet* pSheet = (*itr).second;
				OPERATION nType;
				pSheet->GetOPERATION(nType);
				if(nType == OP_VIEW)
				{
					pOldestSheet = pSheet;
					break;
				}
			}
			if(pOldestSheet == NULL && m_mapObjSheet.size() > 0)
			{
				pOldestSheet = (*m_mapObjSheet.begin()).second;
			}
			_ASSERT(pOldestSheet != NULL);
			if(pOldestSheet != NULL)
			{
				CWnd* pChildWnd = pOldestSheet->GetParentView()->GetParent();
				_ASSERT(pChildWnd != NULL );
				pChildWnd->SendMessage(WM_CLOSE);	
			}
		}
		if(pWrapper)
		{
			CYDObjectRef* pObjRef = pWrapper->m_pObjRef;
			if(pObjRef != NULL)
			{
				HRESULT hr =pObjRef->Restore();
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONSTOP);
					return NULL;
				}
			}
		}
		
		hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_DOCUMENT,(WPARAM)pWrapper,(LPARAM)nOp);
		if(FAILED(hr))
		{
			return hr;
		}
		_pSheet = (CObjPropertySheet*)AfxGetMainWnd()->SendMessage(WM_YD_GETLAST_SHEET,0,0);
		ASSERT(_pSheet);
		CWnd* pChildWnd = _pSheet->GetParentView()->GetParent();
		_ASSERT(pChildWnd != NULL );
		/*hr = pSheet->Show(pWrapper,nOp)*/;
		//pMainView->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
	
		m_nMaxViewNum++;
		m_mapObjSheet.insert(std::map<CYdObjWrapper*,CObjPropertySheet*>::value_type(pWrapper,_pSheet));
		
	/*	else
		{
			pChildWnd->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);
		}
		((CFrameWnd*)pChildWnd)->SetActiveView(pSheet->GetParentView());*/
		return S_OK;
	}

	return NULL;
}

HRESULT CObjPropSheetManager::Show( CString _sTitle, OPERATION nOp,
											  CObjPropertySheet* &_pSheet)
{
	HRESULT hr = E_FAIL;
	UINT objid = 0;
	_pSheet = NULL;

	std::map<CString,CObjPropertySheet*>::iterator itr;
	itr = m_mapStrSheet.find(_sTitle);
	if(itr == m_mapStrSheet.end())
	{
		if(_sTitle.CompareNoCase(EXAM_EXTACT_ANSWER) == 0 ||
			_sTitle.CompareNoCase(EXAM_BLUE) == 0)
		{
			//这个要特殊处理
			CString strSpec;
			if(_sTitle.CompareNoCase(EXAM_EXTACT_ANSWER) == 0)
			{
				strSpec = EXAM_BLUE;
			}
			else if(_sTitle.CompareNoCase(EXAM_BLUE) == 0)
			{
				strSpec = EXAM_EXTACT_ANSWER;
			}
			if(itr == m_mapStrSheet.end())
			{
				itr = m_mapStrSheet.find(strSpec);
			}
		}
	}
	
	if(itr != m_mapStrSheet.end() )  //该对象已经被打开
	{
		CObjPropertySheet* pSheet = (*itr).second;
		CMDIChildWnd* pChildFrm = (CMDIChildWnd*)pSheet->GetParentView()->GetParent();
		_ASSERT(pChildFrm!=NULL );
		pChildFrm->SetActiveView((CView*)pSheet->GetPage(0));
		OPERATION nOldOp;
		pSheet->GetOPERATION(nOldOp);
		if(nOldOp != nOp)  //
		{
			if(nOldOp == OP_VIEW)
			{
				//_ASSERT(nOp == OP_EDIT || nOp == CHECKOUT_EDIT);
				pSheet->ReadonlyPages(FALSE);
				if( nOp == OP_EDIT )
				{
					pChildFrm->SendMessage(WM_YD_UPDATE_BELOWBAR,WPARAM(nOp));
					pChildFrm->SendMessage(WM_YD_SHOW_BELOWBAR,WPARAM(TRUE));
					pChildFrm->RecalcLayout();
				}
			}
			pSheet->ResetOPERATION(nOp);
		}
		pChildFrm->MDIActivate();

		_pSheet = (*itr).second;
		return S_OK;
	}
	else
	{
		if(m_nMaxViewNum >= MAX_VIEW_NUMBER)
		{
			CObjPropertySheet* pOldestSheet = NULL;
			for(std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr 
				=m_mapObjSheet.begin();itr!=m_mapObjSheet.end();++itr)
			{
				CObjPropertySheet* pSheet = (*itr).second;
				OPERATION nType;
				pSheet->GetOPERATION(nType);
				if(nType == OP_VIEW)
				{
					pOldestSheet = pSheet;
					break;
				}
			}
			if(pOldestSheet == NULL && m_mapObjSheet.size() > 0)
			{
				pOldestSheet = (*m_mapObjSheet.begin()).second;
			}
			if(pOldestSheet == NULL)
			{
				for(std::map<CString,CObjPropertySheet*>::iterator itr 	=m_mapStrSheet.begin();
					itr!=m_mapStrSheet.end();++itr)
				{
					CObjPropertySheet* pSheet = (*itr).second;
					OPERATION nType;
					pSheet->GetOPERATION(nType);
					if(nType == OP_VIEW)
					{
						pOldestSheet = pSheet;
						break;
					}
				}
			}

			_ASSERT(pOldestSheet != NULL);
			if(pOldestSheet != NULL)
			{
				CWnd* pChildWnd = pOldestSheet->GetParentView()->GetParent();
				_ASSERT(pChildWnd != NULL );
				pChildWnd->SendMessage(WM_CLOSE);	
			}
		}
		
		hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_DOCUMENT_EX,(WPARAM)(&_sTitle),(LPARAM)nOp);
		if(FAILED(hr))
		{
			return hr;
		}
		_pSheet = (CObjPropertySheet*)AfxGetMainWnd()->SendMessage(WM_YD_GETLAST_SHEET,0,0);
		ASSERT(_pSheet);
		CWnd* pChildWnd = _pSheet->GetParentView()->GetParent();
		_ASSERT(pChildWnd != NULL );
	
		m_nMaxViewNum++;
		m_mapStrSheet.insert(std::map<CString,CObjPropertySheet*>::value_type(_sTitle,_pSheet));

		return S_OK;
	}

	return E_FAIL;
}


HRESULT CObjPropSheetManager::RemoveUnderNoAsk(CObjPropertySheet* pView)
{
	for(std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr=m_mapObjSheet.begin();
		itr!=m_mapObjSheet.end();++itr)
	{
		if(pView == (*itr).second)
		{
			OPERATION type;
			pView->GetOPERATION(type);
			if(type == OP_NEW)  //新建的则删除新增的树节点
			{
				CYdObjWrapper* pWrapper = pView->GetCurObjWrapper();
				_ASSERT(pWrapper != NULL);
				CWnd* pWnd = pWrapper->m_pRelatedWnd;
				CWnd* pMainfrm = AfxGetMainWnd();
				if(pWnd!=NULL && pWnd->IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
				{
// 					TreeItemUpdateInfo info;
// 					info.mask = PDEM_TVIS_DELETE;
// 					info.pWrapper = pWrapper;
// 					pMainfrm->SendMessage(WM_UPDATE_TREEITEM,(LPARAM)pWrapper->GetRelatedWnd(),(LPARAM)&info);
				}
				else
				{
					_ASSERT(FALSE);
				}
			}
			m_mapObjSheet.erase(itr);
			m_nMaxViewNum--;
			HRESULT hr = pView->Close();
			if(FAILED(hr)) return hr;
			return S_OK;
		}
	}
	for(std::map<CString,CObjPropertySheet*>::iterator itr=m_mapStrSheet.begin();
		itr!=m_mapStrSheet.end();++itr)
	{
		if(pView == (*itr).second)
		{
			m_mapStrSheet.erase(itr);
			m_nMaxViewNum--;
			HRESULT hr = pView->Close();
			if(FAILED(hr)) return hr;
			return S_OK;
		}
	}
	_ASSERT(FALSE);
	return S_OK;
}

HRESULT CObjPropSheetManager::Remove(CObjPropertySheet* pSheet)
{
	for(std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr=m_mapObjSheet.begin();
		itr!=m_mapObjSheet.end();++itr)
	{
		if(pSheet == (*itr).second)
		{
			OPERATION nOp;
			pSheet->GetOPERATION(nOp);
			if((nOp == OP_NEW || nOp == OP_EDIT)&&(!pSheet->m_bDelete))
			{
				//Active(pSheet);
				//提示修改为"你确定要取消吗?"
				if(IDNO == AfxMessageBox(_T("你确定要取消操作吗？"),MB_YESNO|MB_ICONQUESTION))
				{
					//不取消,则保持原样
					return S_FALSE;
					HRESULT hr = E_FAIL;
					if(nOp == OP_NEW)
					{
						hr = pSheet->Save();
					}
					else
					{
						hr = pSheet->Update();
					}
					if(FAILED(hr))
						return hr;
					
					CYdObjWrapper* pWrapper = (*itr).first;
					_ASSERT(pWrapper != NULL);
					if(pWrapper != NULL)
					{
						CWnd* pWnd = pWrapper->m_pRelatedWnd;
						_ASSERT(pWnd != NULL && pWnd->GetSafeHwnd() != NULL);
						CWnd* pMainfrm = AfxGetMainWnd();
						if(pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
						{
						}
					}
				}
				else  //不保存需要删除已经增加到树或者列表中的new item
				{
					if(nOp == OP_NEW)
					{
						CYdObjWrapper* pWrapper = pSheet->GetCurObjWrapper();
						_ASSERT(pWrapper != NULL);
						CWnd* pWnd = pWrapper->m_pRelatedWnd;
						CWnd* pMainfrm = AfxGetMainWnd();
						if(pWnd!=NULL && pWnd->IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
						{
						}
						else
						{
							_ASSERT(FALSE);
						}
					}
				}
			}
			m_mapObjSheet.erase(itr);
			m_nMaxViewNum--;
			return S_OK;
		}
	}
	for(std::map<CString,CObjPropertySheet*>::iterator itr=m_mapStrSheet.begin();
		itr!=m_mapStrSheet.end();++itr)
	{
		if(pSheet == (*itr).second)
		{
			m_mapStrSheet.erase(itr);
			m_nMaxViewNum--;
			return S_OK;
		}
	}
	_ASSERT(FALSE);
	return S_OK;
}

CObjPropertySheet* CObjPropSheetManager::IsExist(CYdObjWrapper* pWrapper)
{
//	_ASSERT(pWrapper != NULL);
	std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr;
	for(itr=m_mapObjSheet.begin();itr!=m_mapObjSheet.end();++itr)
	{
		if(pWrapper == (*itr).second->GetCurObjWrapper())
		{
			return (*itr).second;
		}
	}
	return NULL;
}

CObjPropertySheet* CObjPropSheetManager::IsExist(CString _str)
{
	for(std::map<CString,CObjPropertySheet*>::const_iterator itr = m_mapStrSheet.begin();
		itr != m_mapStrSheet.end();++itr)
	{
		if(_str.CompareNoCase((*itr).first) == 0)
		{
			return (*itr).second;
		}
	}
	return NULL;
}

void CObjPropSheetManager::RemoveAll()
{
	HRESULT hr = E_FAIL;
	for(std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr=m_mapObjSheet.begin();
		itr!=m_mapObjSheet.end();)
	{
		hr = Remove((*itr).second);
		if(FAILED(hr))
		{
			break;
		}
		itr = m_mapObjSheet.begin();
	}
	for(std::map<CString,CObjPropertySheet*>::iterator itr = m_mapStrSheet.begin();
		itr != m_mapStrSheet.end();)
	{
		hr = Remove((*itr).second);
		if(FAILED(hr))
		{
			break;
		}
		itr = m_mapStrSheet.begin();
	}
}

void CObjPropSheetManager::Active(CObjPropertySheet* pSheet)
{
	_ASSERT(pSheet!=NULL);
	CMDIChildWnd* pChildFrm = (CMDIChildWnd*)pSheet->GetParentView()->GetParent();
	//_ASSERT(pChildFrm!=NULL && pChildFrm->IsKindOf(RUNTIME_CLASS(CChildFrame)));
	ASSERT(pChildFrm);
	pChildFrm->MDIActivate();
}

void CObjPropSheetManager::GetSheetObjWraper(int nIndex,CObjPropertySheet*& pSheet,
								  CYdObjWrapper*& pWrapper)
{
	pSheet = NULL;
	pWrapper = NULL;

	if(nIndex < 0 || nIndex >= (int)m_mapObjSheet.size()) return;
	std::map<CYdObjWrapper*,CObjPropertySheet*>::iterator itr=m_mapObjSheet.begin();
	for(int i=0;i<nIndex;i++) ++itr;
	pSheet = (*itr).second;
	pWrapper = (*itr).first;
}

void CObjPropSheetManager::GetSheetStr(int nIndex,CObjPropertySheet*& pSheet,CString& _str)
{
	pSheet = NULL;
	_str = _T("");

	if(nIndex < 0 || nIndex >= (int)m_mapStrSheet.size()) return;
	std::map<CString,CObjPropertySheet*>::iterator itr=m_mapStrSheet.begin();
	for(int i=0;i<nIndex;i++) ++itr;
	pSheet = (*itr).second;
	_str = (*itr).first;
}

CObjPropertySheet* CObjPropSheetManager::CreateEmptyPropertySheet(const CString& moniker,const CObjPropertyView* pMainView,BOOL bCustomizedTree)
{
	CObjPropertySheet* pSheet = NULL;
	return pSheet;
}