// ExamMsgDlgView.cpp : 实现文件
//

#include "stdafx.h"
//#include "MainFrm.h"
//#include "YDVaultMain.h"
#include "ExamMsgDlgView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "resource.h"
#include "../Include/YDExamComm.h"
#include "../\Base\AutoClean.h"



// ExamMsgDlgView

CExamMsgDlgView::CExamMsgDlgView()
{

}

CExamMsgDlgView::~CExamMsgDlgView()
{

}


BEGIN_MESSAGE_MAP(CExamMsgDlgView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MI_MSG_DEL, &CExamMsgDlgView::OnMIDelMsg)
	ON_COMMAND(ID_MI_MSG_CLEAR, &CExamMsgDlgView::OnMIClearMsg)
	ON_COMMAND(ID_MI_MSG_COPY, &CExamMsgDlgView::OnMICopyMsg)
END_MESSAGE_MAP()



// CExamMsgDlgView 消息处理程序



int CExamMsgDlgView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE |  
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN|
								WS_VSCROLL |LBS_MULTIPLESEL;

	if (!m_lstBoxmsg.Create(dwViewStyle, rectDummy, this, IDC_EXAM_LIST_MSGBOX))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}
	OnChangeVisualStyle();


	return 0;
}

void CExamMsgDlgView::OnPaint()
{
//	return ;
	CPaintDC dc(this); // device context for painting
// 	if(m_pwndExamMsg && m_pwndExamMsg->GetSafeHwnd())
// 	{
// 		CRect rectTree;
// 		m_pwndExamMsg->GetWindowRect(rectTree);
// 		ScreenToClient(rectTree);
// 
// 		rectTree.InflateRect(1, 1);
// 		dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
// 
// 	}
}

void CExamMsgDlgView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	if(m_lstBoxmsg.GetSafeHwnd())
	{
		m_lstBoxmsg.SetFocus();
	}
}

void CExamMsgDlgView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CExamMsgDlgView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if(m_lstBoxmsg.GetSafeHwnd())
	{

		CRect rectClient;
		GetClientRect(rectClient);

		m_lstBoxmsg.SetWindowPos(NULL, rectClient.left + 1, rectClient.top/* + cyTlb*/ + 1, rectClient.Width() - 2, rectClient.Height() /*- cyTlb*/ - 2, SWP_NOACTIVATE | SWP_NOZORDER);	

	}
}



void CExamMsgDlgView::OnChangeVisualStyle()
{
// 	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;
// 
// 	CBitmap bmp;
// 	if (!bmp.LoadBitmap(uiBmpId))
// 	{
// 		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
// 		ASSERT(FALSE);
// 		return;
// 	}
// 
// 	BITMAP bmpObj;
// 	bmp.GetBitmap(&bmpObj);
// 
// 	UINT nFlags = ILC_MASK;
// 
// 	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
// 
// 	m_ExamViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
// 	m_ExamViewImages.Add(&bmp, RGB(255, 0, 0));
// 
// 	m_wndExamView.SetImageList(&m_ExamViewImages, TVSIL_NORMAL);
}


BOOL CExamMsgDlgView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CExamMsgDlgView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CListBoxEx* pWndList = (CListBoxEx*) &m_lstBoxmsg;
	ASSERT_VALID(pWndList);
	if (pWnd != pWndList)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
	pWndList->SetFocus();
	CMenu* pSubMenu = NULL;
	CMenu menu;
	menu.LoadMenu(IDR_MENU_LIST_MSG);
	pSubMenu = menu.GetSubMenu(0);
	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

	if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSubMenu->m_hMenu, FALSE, TRUE))
		return;

	((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
}

void CExamMsgDlgView::OnMIDelMsg()
{
	int nCount = m_lstBoxmsg.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_lstBoxmsg.GetSelItems(nCount, aryListBoxSel.GetData()); 
	for (int i = aryListBoxSel.GetUpperBound();i >= 0;i--)
	{
		int index  = aryListBoxSel.GetAt(i);
		m_lstBoxmsg.DeleteString(index);
	}

}

void CExamMsgDlgView::OnMIClearMsg()
{
	m_lstBoxmsg.ResetContent();
}

void CExamMsgDlgView::OnMICopyMsg()
{
	int nCount = m_lstBoxmsg.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_lstBoxmsg.GetSelItems(nCount, aryListBoxSel.GetData()); 
	CString strCopy ;
	for (int i= 0; i <= aryListBoxSel.GetUpperBound();i++)
	{
		int index  = aryListBoxSel.GetAt(i);
		CString strIndex;
		m_lstBoxmsg.GetText(index,strIndex);
		if(i != 0)
		{
			strCopy += _T("\r\n");
		}
		strCopy += strIndex;
	}
	if(strCopy.IsEmpty())
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	hr = CopyToClipboard(strCopy);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

HRESULT CExamMsgDlgView::CopyToClipboard(CString& strVal)//将strVal得值放到剪切板中
{
	HRESULT hr = E_FAIL;
	if(OpenClipboard())   
	{     
		EmptyClipboard(); 
		HGLOBAL   clipbuffer;   
		TCHAR   *buffer;   
		clipbuffer=GlobalAlloc(GHND|GMEM_DDESHARE,(strVal.GetLength()+1)*sizeof(TCHAR));   
		buffer=(TCHAR   *)GlobalLock(clipbuffer);   
#ifdef _UNICODE
		wmemset(buffer,0,strVal.GetLength()+1);
		wmemcpy(buffer,(LPCTSTR)strVal,strVal.GetLength());
#else
		memset(buffer,0,strVal.GetLength()+1);
		memcpy(buffer,(LPCTSTR)strVal,strVal.GetLength());
#endif
		//_tcscpy(buffer,LPCTSTR(strCode));   
		GlobalUnlock(clipbuffer);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT,clipbuffer);   
#else
		SetClipboardData(CF_TEXT,clipbuffer);   
#endif

		CloseClipboard();   
	}  
	return S_OK;
}
