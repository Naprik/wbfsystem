// FormPaper.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormPaper.h"
#include "DlgAnswer.h"
#include "DlgPaper.h"

#include "../ObjRef/YDPaper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../UIBase\ListXlsoutput.h"

// CFormPaper

IMPLEMENT_DYNCREATE(CFormPaper, CYdFormView)

CFormPaper::CFormPaper()
	: CYdFormView(CFormPaper::IDD)
	, m_utotalNumber(0)
	, m_uPageSize(0)
	, m_uPage(0)
{

}

CFormPaper::~CFormPaper()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstPaperCache);
	CListAutoClean<CPropQueryContidition> clr2(m_lstPropQuery);
}

void CFormPaper::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DM_PAPER, m_PaperListCtrl);
	DDX_Text(pDX, IDC_EDIT_QPAPER_QUERY_TOTAL, m_utotalNumber);
	DDX_Text(pDX, IDC_EDIT_QPAPER_QUERY_PAGENUM, m_uPageSize);
}

BEGIN_MESSAGE_MAP(CFormPaper, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_DM_PAPER_ADD, &CFormPaper::OnBnClickedBtnDmPaperAdd)
	ON_BN_CLICKED(IDC_BTN_DM_PAPER_EDIT, &CFormPaper::OnBnClickedBtnDmPaperEdit)
	ON_BN_CLICKED(IDC_BTN_DM_PAPER_DEL, &CFormPaper::OnBnClickedBtnDmPaperDel)
	ON_BN_CLICKED(IDC_BTN_DM_PAPER_ANSWER_EDIT, &CFormPaper::OnBnClickedBtnDmPaperAnswerEdit)
	ON_BN_CLICKED(IDC_BTN_DM_PAPER_SEARCH, &CFormPaper::OnBnClickedBtnDmPaperSearch)
	ON_BN_CLICKED(IDC_BTN_DM_QPAPER_BEGIN, &CFormPaper::OnBnClickedBtnDmQpaperBegin)
	ON_BN_CLICKED(IDC_BTN_DM_QPAPER_PREV, &CFormPaper::OnBnClickedBtnDmQpaperPrev)
	ON_BN_CLICKED(IDC_BTN_DM_QPAPER_NEXT, &CFormPaper::OnBnClickedBtnDmQpaperNext)
	ON_BN_CLICKED(IDC_BTN_DM_QPAPER_END, &CFormPaper::OnBnClickedBtnDmQpaperEnd)
	ON_BN_CLICKED(IDC_BTN_DM_PAPER_OUTPUT, &CFormPaper::OnBnClickedBtnDmPaperOutput)
END_MESSAGE_MAP()


// CFormPaper diagnostics

#ifdef _DEBUG
void CFormPaper::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormPaper::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormPaper message handlers

void CFormPaper::OnBnClickedBtnDmPaperAdd()
{
	CDlgPaper dlg(OP_NEW);

	if(dlg.DoModal() == IDOK)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDObjectRef* pPaperRef = new CYDPaper(pDB);
		CComVariant varId(dlg.m_strCode);
		pPaperRef->SetPropVal(FIELD_YDPAPER_CODE, &varId);
		CComVariant varName(dlg.m_strName);
		pPaperRef->SetPropVal(FIELD_YDPAPER_NAME, &varName);

		CComVariant valBegin1(dlg.m_uBegin1);
		HRESULT hr = E_FAIL;
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN1,&valBegin1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valEnd1(dlg.m_uEnd1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END1,&valEnd1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valCount1(dlg.m_uCount1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE1,&valCount1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valMuti1(dlg.m_uMuti1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG1,&valMuti1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valBegin2(dlg.m_uBegin2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN2,&valBegin2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valEnd2(dlg.m_uEnd2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END2,&valEnd2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valCount2(dlg.m_uCount2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE2,&valCount2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valMuti2(dlg.m_uMuti2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG2,&valMuti2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valBegin3(dlg.m_uBegin3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN3,&valBegin3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valEnd3(dlg.m_uEnd3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END3,&valEnd3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valCount3(dlg.m_uCount3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE3,&valCount3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valMuti3(dlg.m_uMuti3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG3,&valMuti3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CDBTransactionRef trans(pDB, TRUE);
		hr = pPaperRef->Save();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = trans.Commit();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		AddPaperItem(pPaperRef);
	}

}

void CFormPaper::OnBnClickedBtnDmPaperEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_PaperListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一试卷！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_PaperListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_PaperListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObj = (CYDObjectRef*)m_PaperListCtrl.GetItemData(nItem);
	ASSERT(pObj != NULL);
	CComVariant valID;
	hr = pObj->GetPropVal(FIELD_YDPAPER_CODE, &valID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = pObj->GetPropVal(FIELD_YDPAPER_NAME,&valName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strName = CDataHandler::VariantToString(valName);

	
	CDlgPaper	dlg(OP_EDIT, this);
	dlg.m_strCode = strID;
	dlg.m_strName = strName;
	hr = pObj->GetPropVal(FIELD_YDPAPER_BEGIN1, &dlg.m_uBegin1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_END1, &dlg.m_uEnd1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_ANSWERCHOICE1, &dlg.m_uCount1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_MULTIFLAG1, &dlg.m_uMuti1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_BEGIN2, &dlg.m_uBegin2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_END2, &dlg.m_uEnd2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_ANSWERCHOICE2, &dlg.m_uCount2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_MULTIFLAG2, &dlg.m_uMuti2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_BEGIN3, &dlg.m_uBegin3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_END3, &dlg.m_uEnd3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_ANSWERCHOICE3, &dlg.m_uCount3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = pObj->GetPropVal(FIELD_YDPAPER_MULTIFLAG3, &dlg.m_uMuti3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}

	if(dlg.DoModal() == IDOK)
	{
		CYDPaper* pPaperRef = (CYDPaper*)pObj;

		CComVariant valID(dlg.m_strCode);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_CODE,&valID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valName(dlg.m_strName);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_NAME,&valName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valBegin1(dlg.m_uBegin1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN1,&valBegin1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valEnd1(dlg.m_uEnd1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END1,&valEnd1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valCount1(dlg.m_uCount1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE1,&valCount1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valMuti1(dlg.m_uMuti1);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG1,&valMuti1);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valBegin2(dlg.m_uBegin2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN2,&valBegin2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valEnd2(dlg.m_uEnd2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END2,&valEnd2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valCount2(dlg.m_uCount2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE2,&valCount2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valMuti2(dlg.m_uMuti2);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG2,&valMuti2);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valBegin3(dlg.m_uBegin3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN3,&valBegin3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valEnd3(dlg.m_uEnd3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END3,&valEnd3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valCount3(dlg.m_uCount3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE3,&valCount3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CComVariant valMuti3(dlg.m_uMuti3);
		hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG3,&valMuti3);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CDBTransactionRef transRef(pDB,TRUE);
		hr = pPaperRef->Update();
		if(FAILED(hr))
		{
			transRef.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}

		hr = transRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		m_PaperListCtrl.SetItemText(nItem,1,dlg.m_strCode);
		m_PaperListCtrl.SetItemText(nItem,2,dlg.m_strName);
	}
}

void CFormPaper::OnBnClickedBtnDmPaperDel()
{
	int nCount = m_PaperListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一试卷！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_PaperListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_PaperListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_PaperListCtrl.GetItemData(nItem);
	ASSERT(pObjRef );
	if (AfxMessageBox(_T("确定要删除选择的试卷吗？"), MB_OKCANCEL) != IDOK)
		return;	
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = pObjRef->Remove();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_PaperListCtrl.DeleteItem(nItem);
}

void CFormPaper::OnBnClickedBtnDmPaperAnswerEdit()
{
	int nCount = m_PaperListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一试卷！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_PaperListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_PaperListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_PaperListCtrl.GetItemData(nItem);
	ASSERT(pObjRef );
	CDlgAnswer dlg(pObjRef);
	dlg.DoModal();
}

void CFormPaper::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	//初始化查询列表
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_DM_PAPERCON_GRID)->GetWindowRect(&rectGrid);
	ScreenToClient(&rectGrid);
	if (!m_QueryGrid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectGrid, this,
		-1))
	{
		return;
	}

	m_QueryGrid.EnableDragHeaderItems(FALSE);
	m_QueryGrid.EnableColumnAutoSize (TRUE);
	m_QueryGrid.EnableGroupByBox (FALSE);
	m_QueryGrid.SetWholeRowSel(FALSE);
	m_QueryGrid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_QueryGrid.SetColorTheme (colors);
	int iWidthGrid = rectGrid.Width();
	m_QueryGrid.InsertColumn(0, L"属性", iWidthGrid/4 - 2);
	m_QueryGrid.InsertColumn(1, L"操作", iWidthGrid/4);
	m_QueryGrid.InsertColumn(2, L"属性值", iWidthGrid/4);

	for (int iColumn = 0; iColumn < m_QueryGrid.GetColumnCount(); iColumn++)
	{
		m_QueryGrid.SetHeaderAlign(iColumn,HDF_CENTER);
		m_QueryGrid.SetColumnAlign(iColumn, HDF_LEFT);
	}

	CBCGPGridRow* pRow = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
	pRow->GetItem(0)->SetValue(_T("名称"));
	pRow->GetItem(0)->Enable(FALSE);
	
	//pRow->GetItem(0)->Enable(FALSE);

	CBCGPGridItem* pItem = new CBCGPGridItem(_T(""));
	pItem->AddOption(_T("等于"));
	pItem->AddOption(_T("不等于"));
	pItem->AddOption(_T("包含"));

	pRow->ReplaceItem (1, pItem);
	pRow->ReplaceItem(2, new CBCGPGridItem(L""));
	m_QueryGrid.AddRow(pRow);
	m_QueryGrid.AdjustLayout();

	//初始化试卷列表
	m_PaperListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_PaperListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_PaperListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_PaperListCtrl.InsertColumn(1, L"试卷编号", LVCFMT_LEFT,iWidth*2/6);
	m_PaperListCtrl.InsertColumn(2, L"试卷名称", LVCFMT_LEFT,iWidth*3/6 -2 );

	
}

HRESULT CFormPaper::AddPaperItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDPAPER_CODE, &valID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = _pref->GetPropVal(FIELD_YDPAPER_NAME,&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);


	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_PaperListCtrl.GetItemCount()+1);
	int index = m_PaperListCtrl.InsertItem(m_PaperListCtrl.GetItemCount(), strIndex);
	m_PaperListCtrl.SetItemText(index,1, strID);
	m_PaperListCtrl.SetItemText(index,2, strName);
	m_PaperListCtrl.SetItemData(index, (DWORD_PTR)_pref);
	m_lstPaperCache.push_back(_pref);

	return S_OK;
}
HRESULT CFormPaper::InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion)
{
	HRESULT hr = E_FAIL;
	
	m_PaperListCtrl.DeleteAllItems();
	
	std::list<CYDObjectRef*>::const_iterator itr = lstQuestion.begin();
	for (; itr != lstQuestion.end(); ++itr)
	{
		hr = AddPaperItem(*itr);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

void CFormPaper::OnBnClickedBtnDmPaperSearch()
{
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;

	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
	}
	CPropQueryContidition* pPropQuery = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropQuery);
	pPropQuery->m_strFiledName = FIELD_YDPAPER_NAME;
	pPropQuery->m_uFieldType = VT_BSTR;
	UINT uOptype = 0;
	CBCGPGridRow* pRow = m_QueryGrid.GetRow(0);
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
		pPropQuery->m_uOpType = Q_UNKNOW;
	}
	CBCGPGridItem* pValItem = pRow->GetItem(2);
	ASSERT(pValItem);
	pPropQuery->m_strConVal = pValItem->GetLabel();

	m_uPage = 0;
	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

HRESULT CFormPaper::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;
	
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	CYDPaper paperRef(pDB);
	if(_bFirstQuery)
	{
		//第一次查询，要计算总数
		long lCount = 0;
		hr = paperRef.GetObjectTotalNumber(&m_lstPropQuery, &lCount);
		if(FAILED(hr))
		{
			return hr;
		}
		m_utotalNumber = (UINT)lCount;
		m_uPageSize = m_utotalNumber/QUESTION_PAGE_COUNT;
		UpdateData(FALSE);
	}
	std::list<CYDObjectRef*> lstPaper;

	hr = paperRef.GetObjectByCondition(m_uPage,
											&m_lstPropQuery,
											&lstPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = InsertlstToListCtrl(lstPaper);
	if(FAILED(hr))
	{
		return hr;
	}

	//按钮的可否使用
	if(m_uPage == 0)
	{
		//当前是第一页
		GetDlgItem(IDC_BTN_DM_QPAPER_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QPAPER_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QPAPER_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QPAPER_PREV)->EnableWindow(TRUE);
	}

	if(m_uPageSize <= m_uPage)
	{
		//到达最后一页了
		GetDlgItem(IDC_BTN_DM_QPAPER_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QPAPER_END)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QPAPER_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QPAPER_END)->EnableWindow(TRUE);
	}

	return S_OK;
}

void CFormPaper::OnBnClickedBtnDmQpaperBegin()
{
	HRESULT hr = E_FAIL;
	m_uPage = 0;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CFormPaper::OnBnClickedBtnDmQpaperPrev()
{
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

void CFormPaper::OnBnClickedBtnDmQpaperNext()
{
	HRESULT hr = E_FAIL;
	m_uPage++;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CFormPaper::OnBnClickedBtnDmQpaperEnd()
{
	HRESULT hr = E_FAIL;
	m_uPage = m_uPageSize;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

BOOL CFormPaper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormPaper::OnBnClickedBtnDmPaperOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_PaperListCtrl);
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
