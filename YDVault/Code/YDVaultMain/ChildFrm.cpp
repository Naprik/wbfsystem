
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "YDVaultMain.h"
#include "ChildFrm.h"
#include "../YDFormUIBase/ObjPropertySheet.h"
#include "../YDFormUIBase/ObjPropertyView.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../Include/ShowErr.h"

#include "../Base/DataHandler.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../DBBase/DBTransactionRef.h"
#include "../YDFormUIBase/ObjPropSheetManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_COMMAND(ID_BLEW_PROPERTY_SAVE, OnPropertySave)
	ON_COMMAND(ID_BLEW_PROPERTY_UPDATE, OnPropertyUpdate)
	ON_COMMAND(ID_BLEW_PROPERTY_CANCEL, OnPropertyCancel)
	ON_UPDATE_COMMAND_UI(ID_BLEW_PROPERTY_SAVE, OnUpdatePropertySave)
	ON_UPDATE_COMMAND_UI(ID_BLEW_PROPERTY_UPDATE, OnUpdatePropertyUpdate)
	ON_UPDATE_COMMAND_UI(ID_BLEW_PROPERTY_CANCEL, OnUpdatePropertyCancel)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_YD_SHOW_BELOWBAR,OnShowBelowBar)
	ON_MESSAGE(WM_YD_UPDATE_BELOWBAR,OnUpdateBelowBar)
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if(m_belowbar.Create(this,IDD_YDMAIN_BLEW_VIEW,WS_CHILD|CBRS_BOTTOM,322222222/*ID_YDMAIN_BLEW_VIEW*/)==-1)
	{
		return -1;	// TODO: Add your specialized creation code here
	}
	//m_belowbar.ShowWindow(SW_MAXIMIZE);
	return 0;
}

void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);
	if(m_belowbar.m_hWnd != NULL)
	{
		CRect rect;
		rect.top = cy - 45;
		rect.bottom = cy-15;
		rect.left = 0;
		rect.right = cx;
		m_belowbar.MoveWindow(&rect);
	}

	// TODO: Add your message handler code here
}

void CChildFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CChildFrame::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CMDIChildWndEx::OnNcPaint() for painting messages
	if(!IsZoomed())
		CMDIChildWnd::OnNcPaint();
	else{
		CRect rect,windowrect;
		GetWindowRect(&rect);

		CWindowDC dc(this);
		CPen pen(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
		CPen* oldpen = dc.SelectObject(&pen);
		dc.MoveTo(4,4);
		dc.LineTo(4,rect.Height()-6); 
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DDKSHADOW));//RGB(113,111,100)
		dc.SelectObject(&pen);
		dc.MoveTo(5,5);
		dc.LineTo(5,rect.Height()-6); 
		pen.DeleteObject();
		dc.SelectObject(oldpen);
		ReleaseDC(&dc);
	}
}

void CChildFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CMDIChildWndEx::OnPaint() for painting messages
	SendMessage(WM_NCPAINT);
}

void CChildFrame::OnPropertySave() 
{
	HRESULT hr = E_FAIL;
	CView* pView = GetActiveView();
	if(pView == NULL) return;
	CObjPropertySheet* pSheet = NULL;
	_ASSERT(pView->GetSafeHwnd()!=NULL);
	if(pView->IsKindOf(RUNTIME_CLASS(CYdFormView)))
	{
		pSheet = ((CYdFormView*)pView)->GetPropertySheet();
	}
	if(pSheet != NULL)
	{
		if(!pSheet->ValidateData(OP_NEW))
		{
			return ;
		}
		CYdFormView* pYdFormView = (CYdFormView*)pView;
		CDBTransactionRef TransRef(theApp.m_pDatabase,TRUE);
		CWaitCursor cursor;
		hr = pSheet->Save();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CYdObjWrapper* pObjWrapper = pSheet->GetCurObjWrapper();
		hr = pSheet->ResetOPERATION(OP_VIEW);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		pSheet->ReadonlyPages(TRUE);
		m_belowbar.SaveCompleted();
		hr = UpdateRelatedTree(pObjWrapper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
}

void CChildFrame::OnPropertyUpdate() 
{
	HRESULT hr = E_FAIL;
	CView* pView = GetActiveView();
	if(pView == NULL) return;
	CObjPropertySheet* pSheet = NULL;
	_ASSERT(pView->GetSafeHwnd()!=NULL);
	if(pView->IsKindOf(RUNTIME_CLASS(CYdFormView)))
	{
		pSheet = ((CYdFormView*)pView)->GetPropertySheet();
	}
	if(pSheet != NULL)
	{
		CWaitCursor cursor;
		if(!pSheet->ValidateData(OP_EDIT))
		{
			return ;
		}
		CYdFormView* pFormView = (CYdFormView*)pView;
		CDBTransactionRef TransRef(theApp.m_pDatabase,TRUE);
		hr = pSheet->Update();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CYdObjWrapper* pObjWrapper = pSheet->GetCurObjWrapper();
		hr = pSheet->ResetOPERATION(OP_VIEW);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		pSheet->ReadonlyPages(TRUE);
		m_belowbar.UpdateCompleted();
		hr = UpdateRelatedTree(pObjWrapper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
}

void CChildFrame::OnPropertyCancel() 
{
	CView* pView = GetActiveView();
	if(pView == NULL) return;
	CObjPropertySheet* pSheet = NULL;
	_ASSERT(pView->GetSafeHwnd()!=NULL);
	if(pView->IsKindOf(RUNTIME_CLASS(CYdFormView)))
	{
		pSheet = ((CYdFormView*)pView)->GetPropertySheet();
	}
	if(pSheet != NULL)
	{
		OPERATION nType;
		pSheet->GetOPERATION(nType);
		if(nType == OP_EDIT)  //
		{
			HRESULT hr = pSheet->ResetOPERATION(OP_VIEW);
			if(SUCCEEDED(hr))
			{
				int nCount = pSheet->GetPageCount();
				for(int i=0;i<nCount;i++)
				{
					CYdFormView* pYdFormView = pSheet->GetPage(i);
					if(pYdFormView != NULL && pYdFormView->GetSafeHwnd() != NULL)
					{
						// 						hr = pPage->LoadObjProperty();
						// 						if(FAILED(hr))
						// 						{
						// 							DISPLAY_PDEMERROR(hr,MB_OK|MB_ICONSTOP);
						// 							break;
						// 						}
						pYdFormView->UpdateData(FALSE);
					}
				}
				m_belowbar.UpdateCompleted();
				m_belowbar.ShowWindow(SW_HIDE);
				RecalcLayout();
				pSheet->ReadonlyPages(TRUE);
			}
			return;
		}
	}
	SendMessage(WM_CLOSE);
}

void CChildFrame::OnUpdatePropertySave(CCmdUI* pCmdUI) 
{
	CView* pView = GetActiveView();
	if(pView == NULL) return;
	CObjPropertySheet* pSheet = NULL;
	_ASSERT(pView->GetSafeHwnd()!=NULL);
	if(pView->IsKindOf(RUNTIME_CLASS(CYdFormView)))
	{
		pSheet = ((CYdFormView*)pView)->GetPropertySheet();
	}
	// 	else if(pView->IsKindOf(RUNTIME_CLASS(CObjPropertyPage)))
	// 	{
	// 		pSheet = ((CObjPropertyPage*)pView)->m_pSheet;
	// 	}
	if(pSheet != NULL)
	{
		OPERATION nType;
		pSheet->GetOPERATION(nType);
		if(nType == OP_NEW)
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
}

void CChildFrame::OnUpdatePropertyUpdate(CCmdUI* pCmdUI) 
{
	CView* pView = GetActiveView();
	if(pView == NULL) return;
	CObjPropertySheet* pSheet = NULL;
	_ASSERT(pView->GetSafeHwnd()!=NULL);
	if(pView->IsKindOf(RUNTIME_CLASS(CYdFormView)))
	{
		pSheet = ((CYdFormView*)pView)->GetPropertySheet();
	}
	// 	else if(pView->IsKindOf(RUNTIME_CLASS(CObjPropertyPage)))
	// 	{
	// 		pSheet = ((CObjPropertyPage*)pView)->m_pSheet;
	// 	}
	if(pSheet != NULL)
	{
		OPERATION nType;
		pSheet->GetOPERATION(nType);
		if(nType == OP_EDIT)
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
}

void CChildFrame::OnUpdatePropertyCancel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}



void CChildFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	HRESULT hr = E_FAIL;
	CView* pView = GetActiveView();
	if(pView == NULL) return;
	CObjPropertySheet* pSheet = NULL;
	_ASSERT(pView->GetSafeHwnd()!=NULL);
	if(pView->IsKindOf(RUNTIME_CLASS(CYdFormView)))
	{
		CYdFormView* pYDFormView = (CYdFormView*)pView;
		hr = pYDFormView->Close();
		if (hr == S_FALSE)
		{
			return;
		}
		pSheet = ((CYdFormView*)pView)->GetPropertySheet();
	}
	if(pSheet != NULL)
	{
		CObjPropSheetManager* pSheetManager = (CObjPropSheetManager*)AfxGetMainWnd()->SendMessage(WM_YD_GET_PROPSHEETMANAGER);
		ASSERT(pSheetManager);
		CYdObjWrapper* pObjWrapper = pSheet->GetCurObjWrapper();
//		ASSERT(pObjWrapper != NULL);
		hr = pSheetManager->Remove(pSheet);
		if(SUCCEEDED(hr))
		{
			if (hr == S_FALSE)
			{
				return;
			}
		}
		else
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONSTOP);
			return;
		}
		if (pObjWrapper)
		{
			UINT uID = 0;
			hr = pObjWrapper->m_pObjRef->GetID(&uID);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONSTOP);
				return;
			}
			if(uID == 0)
			{
				//说明是新建的，要将树上的节点删除
				if(pObjWrapper->m_pRelatedWnd != NULL)
				{
					CTreeCtrl* pTreeCtrl = (CTreeCtrl*)pObjWrapper->m_pRelatedWnd;
					HTREEITEM hItem = (HTREEITEM)pObjWrapper->m_hRelatedItem;
					ASSERT(pTreeCtrl != NULL && hItem != NULL);
					pTreeCtrl->DeleteItem(hItem);
				}
			}
		}
	}
	CMDIChildWndEx::OnClose();
}

HRESULT CChildFrame::UpdateRelatedTree(CYdObjWrapper* _pObjWrapper)
{
	CTreeCtrl* pTreeCtrl = (CTreeCtrl*)_pObjWrapper->m_pRelatedWnd;
	HTREEITEM hItem = _pObjWrapper->m_hRelatedItem;
	HRESULT hr = E_FAIL;
	CYDObjectRef* pObjRef = _pObjWrapper->m_pObjRef;
	CComVariant valName;
	hr = pObjRef->GetPropVal(L"Name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);
	pTreeCtrl->SetItemText(hItem,strName);
	return S_OK;
}

HRESULT CChildFrame::OnShowBelowBar(WPARAM wParam, LPARAM lParam)
{
	BOOL bShow = (BOOL)wParam;
	ShowBelowBar(bShow);
	return S_OK;
}

HRESULT CChildFrame::OnUpdateBelowBar(WPARAM wParam, LPARAM lParam)
{
	OPERATION op = OPERATION(wParam);
	UpdateBelowBar(op);
	return S_OK;
}