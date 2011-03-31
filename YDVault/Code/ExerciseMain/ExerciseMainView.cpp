// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// ExerciseMainView.cpp : CExerciseMainView 类的实现
//

#include "stdafx.h"
#include "ExerciseMain.h"

#include "ExerciseMainDoc.h"
#include "ExerciseMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExerciseMainView

IMPLEMENT_DYNCREATE(CExerciseMainView, CListView)

BEGIN_MESSAGE_MAP(CExerciseMainView, CListView)
	ON_WM_STYLECHANGED()

END_MESSAGE_MAP()

// CExerciseMainView 构造/析构

CExerciseMainView::CExerciseMainView()
{
	// TODO: 在此处添加构造代码

}

CExerciseMainView::~CExerciseMainView()
{
}

BOOL CExerciseMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CExerciseMainView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
}

void CExerciseMainView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExerciseMainView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CExerciseMainView 诊断

#ifdef _DEBUG
void CExerciseMainView::AssertValid() const
{
	CListView::AssertValid();
}

void CExerciseMainView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CExerciseMainDoc* CExerciseMainView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExerciseMainDoc)));
	return (CExerciseMainDoc*)m_pDocument;
}
#endif //_DEBUG


// CExerciseMainView 消息处理程序
void CExerciseMainView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: 添加代码以响应用户对窗口视图样式的更改	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

 

