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

// ExerciseMainDoc.cpp : CExerciseMainDoc ���ʵ��
//

#include "stdafx.h"
#include "ExerciseMain.h"

#include "ExerciseMainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExerciseMainDoc

IMPLEMENT_DYNCREATE(CExerciseMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CExerciseMainDoc, CDocument)
END_MESSAGE_MAP()


// CExerciseMainDoc ����/����

CExerciseMainDoc::CExerciseMainDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CExerciseMainDoc::~CExerciseMainDoc()
{
}

BOOL CExerciseMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CExerciseMainDoc ���л�

void CExerciseMainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CExerciseMainDoc ���

#ifdef _DEBUG
void CExerciseMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CExerciseMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CExerciseMainDoc ����
