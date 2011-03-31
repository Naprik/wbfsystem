#include "StdAfx.h"
#include "StaticYdComApp.h"
#include "../Base\AutoClean.h"
#include "../Base\CriticalSectionControl.h"

CStaticYdComApp CStaticYdComApp::m_staticYdComApp;

CGExamStruct::CGExamStruct()
{
	m_pExamDetail = new CExamDetail();;
	//m_pExamApp = new CExamApp();
	m_bExtractOnlyFromTeacher = FALSE;
	m_bThreadJump = FALSE;
	m_pMainWnd = NULL;
	m_TeacherType = GFROM_TEACHER;
}

CGExamStruct::~CGExamStruct()
{
	CPtrAutoClean<CExamDetail> clr1(m_pExamDetail);
	//CPtrAutoClean<CExamApp> clr2(m_pExamApp);
	CCriticalSectionControl cc(&m_cs);
	CListAutoClean<CYDTeacherAppCom> clr3(m_lstTacherAppCom);
}




CStaticYdComApp::CStaticYdComApp(void)
{
}

CStaticYdComApp::~CStaticYdComApp(void)
{
	ClearStudev();
}

HRESULT CStaticYdComApp::GetGExamStruct(CGExamStruct* &_pGExamStruct)
{
	HRESULT hr = E_FAIL;
	_pGExamStruct = &m_GExamStruct;
	return S_OK;
}


HRESULT CStaticYdComApp::ClearStudev()
{
	HRESULT hr = E_FAIL;
	//CListAutoClean<CStudev> clr1(m_GExamStruct.m_lstStudev);
	return S_OK;
}

