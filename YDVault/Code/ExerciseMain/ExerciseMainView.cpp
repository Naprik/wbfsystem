// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// ExerciseMainView.cpp : CExerciseMainView ���ʵ��
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

// CExerciseMainView ����/����

CExerciseMainView::CExerciseMainView()
{
	// TODO: �ڴ˴���ӹ������

}

CExerciseMainView::~CExerciseMainView()
{
}

BOOL CExerciseMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CExerciseMainView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
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


// CExerciseMainView ���

#ifdef _DEBUG
void CExerciseMainView::AssertValid() const
{
	CListView::AssertValid();
}

void CExerciseMainView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CExerciseMainDoc* CExerciseMainView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExerciseMainDoc)));
	return (CExerciseMainDoc*)m_pDocument;
}
#endif //_DEBUG


// CExerciseMainView ��Ϣ�������
void CExerciseMainView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: ��Ӵ�������Ӧ�û��Դ�����ͼ��ʽ�ĸ���	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

 

