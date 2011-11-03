// QuestionListFormView.cpp : implementation file
//

#include "stdafx.h"
#include "QuestionListFormView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../objref/YDObjectRef.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YDQuestionType.h"
#include "../Include/ShowErr.h"
#include "../ObjRef/YDLinkRef.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionRef.h"
#include "../YDFormUIBase/ListCtrlOperate.h"
#include "../DBBase/DBTransactionRef.h"
#include "../Base/DataHandler.h"
#include "../YDFormUIBase/YDQuestionDlg.h"
#include "../YDUIQuestionVault/YDChoiceQuestionDlg.h"
#include "../YDUIQuestionVault/YDReadQuestionDlg.h"
#include "../YDUIQuestionVault/YDTranslationQuestionDlg.h"
#include "../ObjRef/PropQueryContidition.h"
#include "QuestionViewDlg.h"
#include "../UIBase\ListXlsoutput.h"
#include "DlgYDVaultQuestionFactorInfoConfig.h"
#include "VocabularyInputOutputQuestionHelper.h"
#include "DlgSelInputType.h"
// CQuestionListFormView



IMPLEMENT_DYNCREATE(CQuestionListFormView, CYdFormView)

CQuestionListFormView::CQuestionListFormView()
	: CYdFormView(CQuestionListFormView::IDD)
{
	m_plstOperate = NULL;
	m_uPage = 0;
	m_uTotalCount = 0;
	m_uTotalPages = 0;
}

CQuestionListFormView::~CQuestionListFormView()
{
	CPtrAutoClean<CListCtrlOperate> clr(m_plstOperate);
	CListAutoClean<CQueryProp> clr1(m_lstQueryProp);
	CListAutoClean<CPropQueryContidition> clr2(m_lstPropQuery);
}

void CQuestionListFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QL_QUESTION, m_lstQuestion);
	DDX_Text(pDX, IDC_EDIT_QL_QUERY_TOTAL, m_uTotalCount);
	DDX_Text(pDX, IDC_EDIT_QL_QUERY_PAGENUM, m_uTotalPages);
}

BEGIN_MESSAGE_MAP(CQuestionListFormView, CYdFormView)
	ON_BN_CLICKED(IDC_BUTTON_QL_ADD, &CQuestionListFormView::OnBnClickedButtonQlAdd)
	ON_BN_CLICKED(IDC_BUTTON_QL_MODIFY, &CQuestionListFormView::OnBnClickedButtonQlModify)
	ON_BN_CLICKED(IDC_BUTTON_QL_DEL, &CQuestionListFormView::OnBnClickedButtonQlDel)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_ADD, &CQuestionListFormView::OnBnClickedButtonQlQueryAdd)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_DEL, &CQuestionListFormView::OnBnClickedButtonQlQueryDel)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_CLEAR, &CQuestionListFormView::OnBnClickedButtonQlQueryClear)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_QUERY, &CQuestionListFormView::OnBnClickedButtonQlQueryQuery)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_BEGIN, &CQuestionListFormView::OnBnClickedButtonQlQueryBegin)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_PREV, &CQuestionListFormView::OnBnClickedButtonQlQueryPrev)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_NEXT, &CQuestionListFormView::OnBnClickedButtonQlQueryNext)
	ON_BN_CLICKED(IDC_BUTTON_QL_QUERY_END, &CQuestionListFormView::OnBnClickedButtonQlQueryEnd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QL_QUESTION, &CQuestionListFormView::OnNMDblclkListQlQuestion)
	ON_BN_CLICKED(IDC_BUTTON_QL_OUTPUT, &CQuestionListFormView::OnBnClickedButtonQlOutput)
	ON_BN_CLICKED(IDC_BUTTON_QL_CONFIG_FACTORINFO, &CQuestionListFormView::OnBnClickedButtonQlConfigFactorinfo)
	ON_BN_CLICKED(IDC_BUTTON_QL_INPUT, &CQuestionListFormView::OnBnClickedButtonQlInput)
	ON_BN_CLICKED(IDC_BUTTON_QL_OUTPUT_RTF, &CQuestionListFormView::OnBnClickedButtonQlOutputRtf)
END_MESSAGE_MAP()


// CQuestionListFormView diagnostics

#ifdef _DEBUG
void CQuestionListFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionListFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionListFormView message handlers

void CQuestionListFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the ase class
	m_lstQuestion.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect lstQLocate;
	GetDlgItem(IDC_LIST_QL_QUESTION)->GetWindowRect(lstQLocate);
	m_lstQuestion.InsertColumn(0,_T("序号"),LVCFMT_LEFT, lstQLocate.Width()/7 -1 );
	m_lstQuestion.InsertColumn(1,_T("提干"),LVCFMT_LEFT, lstQLocate.Width()*6/7 - 1);

	m_plstOperate = new CListCtrlOperate(&m_lstQuestion);
	//初始化查询条件
	CRect gridLocate;
	GetDlgItem(IDC_STATIC_QL_GRID)->GetWindowRect(gridLocate);
	ScreenToClient(&gridLocate);
	if (!m_gridConn.Create(WS_CHILD|WS_VISIBLE|WS_BORDER,gridLocate,
			this,IDC_QL_GCL_QUERYCONN))
	{
		TRACE0("Failed to m_listctrl.Create!\n");
		return ;
	}
	m_gridConn.EnableHeader(TRUE,0);
	m_gridConn.EnableDragHeaderItems(FALSE);
	m_gridConn.SetWholeRowSel(TRUE);


	//初始化表头
	CString strName = L"属性";
	m_gridConn.InsertColumn(0,strName,gridLocate.Width()/4-1);
	CString strOperator = L"操作";
	m_gridConn.InsertColumn(1,strOperator,gridLocate.Width()/4-1);
	CString strValue = L"属性值";
	m_gridConn.InsertColumn(2,strValue,gridLocate.Width()/4-1);
// 	CString strLogic = L"逻辑符号";
// 	m_gridConn.InsertColumn(3,strLogic,gridLocate.Width()/4-1);
	m_gridConn.AdjustLayout ();
	HRESULT hr = E_FAIL;

	//默认不显示试题
// 	//显示所有的试题
//  	CYDQuestionVault* pQV = NULL;
//  	hr = GetQuestionVault(pQV);
//  	if(FAILED(hr))
//  	{
//  		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
//  		return ;
//  	}
//  	ASSERT(pQV);
//  	CYDQuestionType* pQType = NULL;
//  	hr = GetQuestionType(pQType);
//  	if(FAILED(hr))
//  	{
//  		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
//  		return ;
//  	}
//  	ASSERT(pQType);
//  	UINT idQType = 0;
//  	hr = pQType->GetID(&idQType);
//  	if(FAILED(hr))
//  	{
//  		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
//  		return ;
//  	}
//  	std::list<CYDQuestionRef*> lstQuestion;
//  	std::list<CYDLinkRef*> lstLink;
//  	hr = pQV->GetQuestionByTypeID(idQType,&lstQuestion,&lstLink);
//  	if(FAILED(hr))
//  	{
//  		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
//  		return ;
//  	}
//  	ASSERT(lstQuestion.size() == lstLink.size());
// 	hr = InsertlstToListCtrl(lstQuestion,lstLink);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
 
	hr = CreateQuestionQueryProp();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}


HRESULT CQuestionListFormView::GetQuestionVault(CYDQuestionVault* &_pQV)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pSheet);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDLinkRef* pLinkRef = pObjWrapper->m_pLinkRef;
	ASSERT(pLinkRef);
	CYDObjectRef* pObjA = NULL;
	CYDObjectRef* pObjB = NULL;
	hr = pLinkRef->GetObjRef(pObjA,pObjB);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pObjA->IsKindOf(RUNTIME_CLASS(CYDQuestionVault)));
	_pQV = (CYDQuestionVault*)pObjA;
	return S_OK;
}

HRESULT CQuestionListFormView::GetQuestionVaultID(OBJID* _pQVID)
{
	HRESULT hr = E_FAIL;
	CYDQuestionVault* pQV = NULL;
	hr = GetQuestionVault(pQV);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pQV->GetID(_pQVID);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CQuestionListFormView::GetQuestionType(CYDQuestionType* &_pQType)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pSheet);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	ASSERT(pObjRef->IsKindOf(RUNTIME_CLASS(CYDQuestionType)));
	_pQType = (CYDQuestionType*)pObjRef;
	return S_OK;
}

HRESULT CQuestionListFormView::GetQuestionType(QTYPE* _pQtype)
{
	HRESULT hr = E_FAIL;
	CYDQuestionType	*pQtype = NULL;
	hr =  GetQuestionType(pQtype);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pQtype);
	CComVariant valType;
	hr = pQtype->GetPropVal(FIELD_QUESTIONTYPE_TYPE,&valType);
	if(FAILED(hr))
	{
		return hr;
	}
	*_pQtype = (QTYPE)CDataHandler::VariantToLong(valType);
	return S_OK;
}

HRESULT CQuestionListFormView::GetQuestionTypeID(OBJID* _pUID)
{
	HRESULT hr = E_FAIL;
	CYDQuestionType* pType = NULL;
	hr = GetQuestionType(pType);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pType);
	hr = pType->GetID(_pUID);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CQuestionListFormView::CreateQuestionDlg(CYDQuestionDlg* &_pDlg)
{
	HRESULT hr = E_FAIL;
	QTYPE qType = (QTYPE)-1;
	hr = GetQuestionType(&qType);
	if(FAILED(hr))
	{
		return hr;
	}
	
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
		//短句听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_LISTENINGLONG)
	{
		//长句听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_LISTENINGVOCABULARY)
	{
		//听力填空
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

void CQuestionListFormView::OnBnClickedButtonQlAdd()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CYDQuestionDlg* pDlg = NULL;
	hr = CreateQuestionDlg(pDlg);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pDlg);
	CYDQuestionType* pQType = NULL;
	hr = GetQuestionType(pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pDlg->SetYdQType(pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CYDQuestionVault* pQVault = NULL;
	hr = GetQuestionVault(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pQVault);
	hr = pDlg->SetYdQValut(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pDlg->SetListOperate(m_plstOperate,0);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CPtrAutoClean<CYDQuestionDlg> clr(pDlg);
	pDlg->DoModal();
}

void CQuestionListFormView::OnBnClickedButtonQlModify()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstQuestion.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("请选择要修改的题目！"));
		return;
	}
	HRESULT hr = E_FAIL;
	int nItem = m_lstQuestion.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstQuestion.GetItemData(nItem);
	ASSERT(pObjWrapper);
	CYDQuestionDlg* pDlg = NULL;
	hr = CreateQuestionDlg(pDlg);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pDlg);
	CPtrAutoClean<CYDQuestionDlg> clr(pDlg);
	hr = pDlg->SetObjWrapper(pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pDlg->SetOpType(OP_EDIT);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pDlg->SetListOperate(m_plstOperate,nItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CYDQuestionType* pQType = NULL;
	hr = GetQuestionType(pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pDlg->SetYdQType(pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CYDQuestionVault* pQVault = NULL;
	hr = GetQuestionVault(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pQVault);
	hr = pDlg->SetYdQValut(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}

	hr = pDlg->DoModal();
}

void CQuestionListFormView::OnBnClickedButtonQlDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstQuestion.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("请选择要删除的题目！"));
		return;
	}
	if(AfxMessageBox(_T("确定要删除当前选择的题目吗？"),MB_YESNO) != IDYES)
	{
		return;
	}
	int nItem = m_lstQuestion.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstQuestion.GetItemData(nItem);
	ASSERT(pObjWrapper);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB,TRUE);
	HRESULT hr = E_FAIL;
	hr = pObjWrapper->Remove();
	if(FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = m_plstOperate->DeleteItem(nItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
}
// CBCGPGridRow* CQuestionListFormView::AddSingGCLRow(const CYDQuestionRef& question, BOOL bNeedAdjustLayOut)
// {
// 	return NULL;
// }

void CQuestionListFormView::OnBnClickedButtonQlQueryAdd()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CBCGPGridRow* pRow = m_gridConn.CreateRow(m_gridConn.GetColumnCount());
	pRow->GetItem(0)->SetValue(_T(""));
	pRow->GetItem(1)->SetValue(_T(""));
	pRow->GetItem(2)->SetValue(_T(""));
	

	CBCGPGridItem* pItem = new CBCGPGridItem("");
	for(std::list<CQueryProp*>::const_iterator itr = m_lstQueryProp.begin();
		itr != m_lstQueryProp.end();++itr)
	{
		pItem->AddOption((*itr)->m_strDisplayName);
	}
	pRow->ReplaceItem (0, pItem);
	//pRow->GetItem(0)->Enable(FALSE);

	pItem = new CBCGPGridItem(_T(""));
	pItem->AddOption(_T("等于"));
	pItem->AddOption(_T("不等于"));
	pItem->AddOption(_T("包含"));

	pRow->ReplaceItem (1, pItem);
	//pRow->GetItem(1)->Enable(FALSE);

 	m_gridConn.AddRow(pRow);
}

void CQuestionListFormView::OnBnClickedButtonQlQueryDel()
{
	// TODO: Add your control notification handler code here
	CUIntArray nRows;
	CList<CBCGPGridItem*,CBCGPGridItem*> selItems;
	m_gridConn.GetSelectedItems(selItems);
	POSITION pos = selItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CBCGPGridItem* pItem = selItems.GetNext(pos);
		CBCGPGridItemID &itemID = pItem->GetGridItemID();
		BOOL bFound = FALSE;
		for(int i=0;i<nRows.GetSize();i++)
		{
			if(itemID.m_nRow == nRows[i])
			{
				bFound = TRUE;
				break;
			}
		}
		if(!bFound) nRows.Add(itemID.m_nRow);
	}
	for(int i=nRows.GetSize()-1;i>=0;i--)
	{
		m_gridConn.RemoveRow(nRows[i],TRUE);
	}
}

void CQuestionListFormView::OnBnClickedButtonQlQueryClear()
{
	// TODO: Add your control notification handler code here
	while(m_gridConn.GetRowCount() > 0)
	{
		m_gridConn.RemoveRow(0);
	}
	
}

void CQuestionListFormView::OnBnClickedButtonQlQueryQuery()
{
	// TODO: Add your control notification handler code here
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;
	hr = CreatePropQueryContidion();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	m_uPage = 0;
	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

HRESULT CQuestionListFormView::CreatePropQueryContidion()
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
	}
	
	{
		//试题类型的ID
		CYDQuestionType*  pQType = NULL;
		hr = GetQuestionType(pQType);
		if(FAILED(hr))
		{	
			return hr;
		}
		CPropQueryContidition* pQContidion = new CPropQueryContidition();
		OBJID idQType = 0;
		hr = pQType->GetID(&idQType);
		if(FAILED(hr))
		{	
			return hr;
		}
		CString strVal;
		strVal.Format(_T("%d"),idQType);
		pQContidion->m_strFiledName = _T("TYPEID");
		pQContidion->m_uOpType = Q_EQUAL;
		pQContidion->m_strConVal = strVal;
		pQContidion->m_uFieldType = VT_INT;
		m_lstPropQuery.push_back(pQContidion);
	}

	for(int i = 0; i< m_gridConn.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_gridConn.GetRow(i);
		CBCGPGridItem* pFieldItem = pRow->GetItem(0);
		ASSERT(pFieldItem);
		CString strDisplayName = pFieldItem->GetLabel();
		CString strFieldName;
		UINT uFieldType = VT_I4;
		for(std::list<CQueryProp*>::const_iterator itr = m_lstQueryProp.begin();
			itr != m_lstQueryProp.end();++itr)
		{
			if(strDisplayName.CompareNoCase((*itr)->m_strDisplayName) == 0)
			{
				strFieldName = (*itr)->m_strInternalName;
				uFieldType = (*itr)->m_uType;
				break;
			}
		}
		if(strFieldName.IsEmpty())
		{
			//没有找到字段
			continue;
		}
		CPropQueryContidition* pPropQuery = new CPropQueryContidition();
		m_lstPropQuery.push_back(pPropQuery);
		pPropQuery->m_strFiledName = strFieldName;
		pPropQuery->m_uFieldType = uFieldType;
		UINT uOptype = 0;
		CBCGPGridItem* pOpItem = pRow->GetItem(1);
		ASSERT(pOpItem);
		CString strOp = pOpItem->GetLabel();
		if(strOp.CompareNoCase(_T("等于")) == 0)
		{
			pPropQuery->m_uOpType = Q_EQUAL;
		}
		else if(strOp.CompareNoCase(_T("不等于")) == 0)
		{
			pPropQuery->m_uOpType = Q_NOTEQUAL;
		}
		else if(strOp.CompareNoCase(_T("包含")) == 0)
		{
			pPropQuery->m_uOpType = Q_CONTAIN;
		}
		else
		{
			ASSERT(FALSE);
		}
		CBCGPGridItem* pValItem = pRow->GetItem(2);
		ASSERT(pValItem);
		pPropQuery->m_strConVal = pValItem->GetLabel();

	}
	return S_OK;
}

HRESULT CQuestionListFormView::InsertlstToListCtrl(std::list<CYDQuestionRef*> &lstQuestion,std::list<CYDLinkRef*> &lstLink)
{
	HRESULT hr = E_FAIL;
	UINT iIndex = 0;
	CYDQuestionVault* pQVault = NULL;
	hr = GetQuestionVault(pQVault);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pQVault);
	std::list<CYDQuestionRef*>::const_iterator itr = lstQuestion.begin();
	std::list<CYDLinkRef*>::const_iterator itrLink = lstLink.begin();
	for(;itr != lstQuestion.end()&& itrLink != lstLink.end();
		++itr,++itrLink,++iIndex)
	{
		CYdObjWrapper* pWrapper = new CYdObjWrapper();
		pWrapper->m_pObjRef = *itr;
		pWrapper->m_pLinkRef = *itrLink;
		hr = (*itrLink)->PutObjRef(pQVault,(*itr));
		if(FAILED(hr))
		{
			return hr;
		}
		hr = m_plstOperate->AddItem(pWrapper);
		if(FAILED(hr))
		{
			return hr;
		}

	}
	return S_OK;
}

void CQuestionListFormView::OnBnClickedButtonQlQueryBegin()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	m_uPage = 0;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CQuestionListFormView::OnBnClickedButtonQlQueryPrev()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	if(m_uPage > 0)
	{
		m_uPage --;
	}
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CQuestionListFormView::OnBnClickedButtonQlQueryNext()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	m_uPage++;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CQuestionListFormView::OnBnClickedButtonQlQueryEnd()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	m_uPage = m_uTotalPages;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CQuestionListFormView::OnNMDblclkListQlQuestion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstQuestion.GetItemData(pNMListView->iItem);
	ASSERT(pObjWrapper != NULL);
	HRESULT hr = E_FAIL;
	CQuestionViewDlg dlg;
	dlg.m_pQuestionRef = (CYDQuestionRef*)(pObjWrapper->m_pObjRef);
	dlg.DoModal();
// 	CYDQuestionDlg* pDlg = NULL;
// 	hr = CreateQuestionDlg(pDlg);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	ASSERT(pDlg);
// 	CPtrAutoClean<CYDQuestionDlg> clr(pDlg);
// 	hr = pDlg->SetObjWrapper(pObjWrapper);
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
// 	hr = pDlg->SetListOperate(m_plstOperate,pNMListView->iItem);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	CYDQuestionType* pQType = NULL;
// 	hr = GetQuestionType(pQType);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	hr = pDlg->SetYdQType(pQType);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	CYDQuestionVault* pQVault = NULL;
// 	hr = GetQuestionVault(pQVault);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 	ASSERT(pQVault);
// 	hr = pDlg->SetYdQValut(pQVault);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
// 
// 	hr = pDlg->DoModal();
}

HRESULT CQuestionListFormView::CreateQuestionQueryProp()
{
	HRESULT hr= E_FAIL;
	{
		CListAutoClean<CQueryProp> clr(m_lstQueryProp);
	}
	{
		//公共的都有的
		CQueryProp* pQueryProp = new CQueryProp;
		pQueryProp->m_strInternalName = _T("HARDLEVEL");
		pQueryProp->m_strDisplayName = _T("难度");
		pQueryProp->m_uType = VT_I4;
		m_lstQueryProp.push_back(pQueryProp);

		pQueryProp = new CQueryProp;
		pQueryProp->m_strInternalName = _T("CREATOR");
		pQueryProp->m_strDisplayName = _T("创建人");
		pQueryProp->m_uType = VT_BSTR;
		m_lstQueryProp.push_back(pQueryProp);

		pQueryProp = new CQueryProp;
		pQueryProp->m_strInternalName = _T("CREATEDATE");
		pQueryProp->m_strDisplayName = _T("创建时间");
		pQueryProp->m_uType = VT_DATE;
		m_lstQueryProp.push_back(pQueryProp);
	}

	QTYPE qType = QTYPE(-1);
	hr = GetQuestionType(&qType);
	if(FAILED(hr))
	{
		return hr;
	}
	switch(qType)
	{
		case QTYPE_WRITING:		//写作题
		case QTYPE_TRANSLATION:/* = 5,*///翻译
		case QTYPE_VOCABULARY:/* = 6,*///选择题
			{
				//题干，答案
				CQueryProp* pQueryProp ;
				pQueryProp = new CQueryProp;
				pQueryProp->m_strInternalName = _T("TITLE");
				pQueryProp->m_strDisplayName = _T("题干");
				pQueryProp->m_uType = VT_BSTR;
				m_lstQueryProp.push_back(pQueryProp);

				pQueryProp = new CQueryProp;
				pQueryProp->m_strInternalName = _T("ANSWER");
				pQueryProp->m_strDisplayName = _T("答案");
				pQueryProp->m_uType = VT_BSTR;
				m_lstQueryProp.push_back(pQueryProp);
				break;
			}
		case QTYPE_LISTENINGCOMPREHENSION: /*= 2,*/// 听力
		case QTYPE_READINGCOMPREHENSION:/* = 3,*///阅读理解
		case QTYPE_CLOZE:/* = 4,*///完形填空
		case QTYPE_SKIMMING:/* = 7,*///快速阅读
		case QTYPE_READINGVOCABULARY:/*= 8,*///阅读选词填空
		case QTYPE_LISTENINGVOCABULARY:/* = 9,*///听力填空
		case QTYPE_LISTENINGSHORT:/* = 10,*///短句听力
		case QTYPE_LISTENINGLONG:/* = 11*///长句听力
		case QTYPE_ERRORCORRECTION:
			{
				//文章
				CQueryProp* pQueryProp ;
				pQueryProp = new CQueryProp;
				pQueryProp->m_strInternalName = _T("ARTICLE");
				pQueryProp->m_strDisplayName = _T("文章");
				pQueryProp->m_uType = VT_BSTR;
				m_lstQueryProp.push_back(pQueryProp);
				break;
			}
	}
	return S_OK;
}

HRESULT CQuestionListFormView::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;
	OBJID uQTypeID = 0;
	hr = GetQuestionTypeID(&uQTypeID);
	if(FAILED(hr))
	{
		return hr;
	}
	CYDQuestionVault* pVault = NULL;
	hr = GetQuestionVault(pVault);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pVault);
	if(_bFirstQuery)
	{
		//第一次查询，要计算总数
		long lCount = 0;
		hr = pVault->GetQuestionByTypeIDConditionCount(uQTypeID,
													   &m_lstPropQuery,
													   &lCount);
		if(FAILED(hr))
		{
			return hr;
		}
		m_uTotalCount = (UINT)lCount;
		m_uTotalPages = m_uTotalCount/QUESTION_PAGE_COUNT;
		UpdateData(FALSE);
	}
	std::list<CYDQuestionRef*> lstQRef;
	std::list<CYDLinkRef*>     lstLinkRef;

	hr = pVault->GetQuestionByTypeIDCondition(uQTypeID,
		m_uPage,
		&m_lstPropQuery,
		&lstQRef,
		&lstLinkRef);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(lstQRef.size() == lstLinkRef.size());
	hr = m_plstOperate->RemoveAllItem();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = InsertlstToListCtrl(lstQRef,lstLinkRef);
	if(FAILED(hr))
	{
		return hr;
	}

	//按钮的可否使用
	if(m_uPage == 0)
	{
		//当前是第一页
		GetDlgItem(IDC_BUTTON_QL_QUERY_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QL_QUERY_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_QL_QUERY_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QL_QUERY_PREV)->EnableWindow(TRUE);
	}
	
	if(m_uTotalPages <= m_uPage)
	{
		//到达最后一页了
		GetDlgItem(IDC_BUTTON_QL_QUERY_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QL_QUERY_END)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_QL_QUERY_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QL_QUERY_END)->EnableWindow(TRUE);
	}

	return S_OK;
}

BOOL CQuestionListFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
												  DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
												  CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}
void CQuestionListFormView::OnBnClickedButtonQlOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_lstQuestion);
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


void CQuestionListFormView::OnBnClickedButtonQlConfigFactorinfo()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstQuestion.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("请选择要配置的题目！"));
		return;
	}
	HRESULT hr = E_FAIL;
	int nItem = m_lstQuestion.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstQuestion.GetItemData(nItem);
	ASSERT(pObjWrapper);
	CYDQuestionVault* pQVault = NULL;
	hr = GetQuestionVault(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pQVault);
	CYDQuestionType* pQType = NULL;
	hr = GetQuestionType(pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pQType);
	CDlgYDVaultQuestionFactorInfoConfig dlg;
	dlg.m_pVault = pQVault;
	dlg.m_pQType = pQType;
	dlg.m_pQuestion = pObjWrapper->m_pObjRef;
	dlg.DoModal();
}


void CQuestionListFormView::OnBnClickedButtonQlInput()
{
	// TODO: Add your control notification handler code here
	CYDQuestionType* pType = NULL;
	HRESULT hr = E_FAIL;
	hr = GetQuestionType(pType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CComVariant varType;
	hr = pType->GetPropVal(FIELD_QUESTIONTYPE_TYPE, &varType);
	QTYPE qType = QTYPE(CDataHandler::VariantToLong(varType));
	if(qType != QTYPE_VOCABULARY)
	{
		AfxMessageBox(_T("暂不支持该类型题目导入！"));
		return;
	}
	CDlgSelInputType dlgSelType;
	if(dlgSelType.DoModal() != IDOK)
	{
		return ;
	}
	if(dlgSelType.m_iSelType == 0)
	{
		//从文本文件导入
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Txt文件(*.txt)|*.txt|所有文件(*.*)|*.*||"));
		if(dlg.DoModal() != IDOK)
		{
			return ;
		}
		CYDQuestionVault* pVault = NULL;
		hr = GetQuestionVault(pVault);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		CInputOutputQuestionHelper* pHelper = NULL;
		if(qType == QTYPE_VOCABULARY)
		{
			pHelper = new CVocabularyInputOutputQuestionHelper(pVault,pType);
		}
		ASSERT(pHelper);
		CPtrAutoClean<CInputOutputQuestionHelper> clr(pHelper);
		hr = pHelper->ExeInputFileFromTxt(dlg.GetPathName());
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
	}
	else if(dlgSelType.m_iSelType == 1)
	{
		//从rtf文件导入
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Rtf文件(*.rtf)|*.rtf|所有文件(*.*)|*.*||"));
		if(dlg.DoModal() != IDOK)
		{
			return ;
		}
		CYDQuestionVault* pVault = NULL;
		hr = GetQuestionVault(pVault);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		CInputOutputQuestionHelper* pHelper = NULL;
		if(qType == QTYPE_VOCABULARY)
		{
			pHelper = new CVocabularyInputOutputQuestionHelper(pVault,pType);
		}
		ASSERT(pHelper);
		CPtrAutoClean<CInputOutputQuestionHelper> clr(pHelper);
		hr = pHelper->ExeInputFile(dlg.GetPathName());
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	
}


void CQuestionListFormView::OnBnClickedButtonQlOutputRtf()
{
	// TODO: Add your control notification handler code here
	CYDQuestionType* pType = NULL;
	HRESULT hr = E_FAIL;
	hr = GetQuestionType(pType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CComVariant varType;
	hr = pType->GetPropVal(FIELD_QUESTIONTYPE_TYPE, &varType);
	QTYPE qType = QTYPE(CDataHandler::VariantToLong(varType));
	if(qType != QTYPE_VOCABULARY)
	{
		AfxMessageBox(_T("暂不支持该类型题目导出！"));
		return;
	}
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Rtf文件(*.rtf)|*.rtf|所有文件(*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	CYDQuestionVault* pVault = NULL;
	hr = GetQuestionVault(pVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CInputOutputQuestionHelper* pHelper = NULL;
	if(qType == QTYPE_VOCABULARY)
	{
		pHelper = new CVocabularyInputOutputQuestionHelper(pVault,pType);
	}
	ASSERT(pHelper);
	std::list<CYDObjectRef*> lstQuestion;
	for(int i = 0; i < m_lstQuestion.GetItemCount();i++)
	{
		CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstQuestion.GetItemData(i);
		ASSERT(pObjWrapper);
		lstQuestion.push_back(pObjWrapper->m_pObjRef);
	}
	CPtrAutoClean<CInputOutputQuestionHelper> clr(pHelper);
	hr = pHelper->ExeOutputFile(dlg.GetPathName(),&lstQuestion);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	AfxMessageBox(_T("导出成功！"));
}
