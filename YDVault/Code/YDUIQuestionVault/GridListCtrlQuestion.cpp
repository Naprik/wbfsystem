#include "StdAfx.h"
#include "GridListCtrlQuestion.h"
#include "../ObjRef/YDQuestionRef.h"
#include "resource.h"
#include "../Base/DataHandler.h"
#include "YDChoiceQuestionDlg.h"
#include "YDReadQuestionDlg.h"
#include "YDCompositionQuestionDlg.h"
#include "YDTranslationQuestionDlg.h"
#include "../Include/ShowErr.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YDQuestionType.h"
#include "QuestionViewDlg.h"

IMPLEMENT_DYNAMIC(CGridListCtrlQuestion,CBCGPKeyGridCtrl)
CGridListCtrlQuestion::CGridListCtrlQuestion(void)
{
}

CGridListCtrlQuestion::~CGridListCtrlQuestion(void)
{
}

BEGIN_MESSAGE_MAP(CGridListCtrlQuestion, CBCGPKeyGridCtrl)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
void CGridListCtrlQuestion::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CBCGPKeyGridCtrl::OnLButtonDblClk(nFlags,point);
	CBCGPGridRow* pGridRow =  GetCurSel();
	if(pGridRow == NULL)
	{
		return;
	}
	CYDQuestionRef* pQuestion = (CYDQuestionRef*)pGridRow->GetData();
	if(pQuestion == NULL)
	{
		return;
	}
	CQuestionViewDlg dlg;
	dlg.m_pQuestionRef = pQuestion;
	dlg.DoModal();
// 	CComVariant varQType;
// 	pQuestion->GetPropVal(L"TYPEID", &varQType);
// 	QTYPE qtype = (QTYPE)CDataHandler::VariantToLong(varQType);
// 	CYDQuestionDlg* pDlg = NULL;
// 	HRESULT hr = CreateQuestionDlg(qtype, pDlg);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	ASSERT(pDlg);
// 	CPtrAutoClean<CYDQuestionDlg> clr(pDlg);
// 	CYdObjWrapper pObjWrapper;
// 	pObjWrapper.m_pObjRef = pQuestion;
// 	hr = pDlg->SetObjWrapper(&pObjWrapper);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	hr = pDlg->SetOpType(OP_VIEW);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// // 	hr = pDlg->SetListOperate(m_plstOperate,pNMListView->iItem);
// // 	if(FAILED(hr))
// // 	{
// // 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// // 		return;
// // 	}
//   	CYDQuestionType qType(pQuestion->m_pDb);
//   	CComVariant varTypeID;
// 	pQuestion->GetPropVal(L"TYPEID", &varTypeID);
// 	OBJID typeID = (OBJID)CDataHandler::VariantToLong(varTypeID);
// 	qType.SetID(typeID);
//   	hr = pDlg->SetYdQType(&qType);
//   	if(FAILED(hr))
//   	{
//   		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
//   		return;
//   	}
//  	CYDQuestionVault qVault(pQuestion->m_pDb);
// 	qVault.SetID(m_VaultID);
//  	
//  	hr = pDlg->SetYdQValut(&qVault);
//  	if(FAILED(hr))
//  	{
//  		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
//  		return;
//  	}
// 
// 	hr = pDlg->DoModal();
}

HRESULT CGridListCtrlQuestion::CreateQuestionDlg(QTYPE qType, CYDQuestionDlg* &_pDlg)
{
	HRESULT hr = E_FAIL;
	
	if(qType == QTYPE_WRITING)
	{
		//作文题

		_pDlg = new CYDTranslationQuestionDlg();
	}
	else if(qType == QTYPE_READINGCOMPREHENSION)
	{
		//阅读理解
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_SKIMMING)
	{
		//快速阅读
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_READINGVOCABULARY)
	{
		//阅读填空
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_LISTENINGCOMPREHENSION)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_LISTENINGSHORT)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_LISTENINGLONG)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_LISTENINGVOCABULARY)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_ERRORCORRECTION)
	{
		//改错
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_CLOZE)
	{
		//完形填空
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_TRANSLATION)
	{
		//翻译
		_pDlg = new CYDTranslationQuestionDlg();
	}
	else if(qType == QTYPE_VOCABULARY)
	{
		//选择题
		_pDlg = new CYDChoiceQuestionDlg();
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}
