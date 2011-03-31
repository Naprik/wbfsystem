// FormExamAddress.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormExamAddress.h"

#include "DlgExamAddr.h"
#include "../YDExamObjRef/YDExamAddress.h"
#include "../YDExamObjRef/YdStudentRef.h"
#include "../ObjRef/YDObjectRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "DlgExamAddrSetSutdent.h"
#include "../UIBase\ListXlsoutput.h"
// CFormExamAddress

IMPLEMENT_DYNCREATE(CFormExamAddress, CYdFormView)

CFormExamAddress::CFormExamAddress()
	: CYdFormView(CFormExamAddress::IDD)
	, m_utotalNumber(0)
	, m_uPageSize(0)
	, m_uPage(0)
{

}

CFormExamAddress::~CFormExamAddress()
{
	CListAutoClean<CYDObjectRef> clr(m_lstExamAddrCache);
	CListAutoClean<CPropQueryContidition> clr1(m_lstPropQuery);
}

void CFormExamAddress::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_DM_EXAMADDR, m_ExamAddrListCtrl);


	DDX_Text(pDX, IDC_EDIT_QEXAMADDR_QUERY_TOTAL, m_utotalNumber);
	DDX_Text(pDX, IDC_EDIT_QEXAMADDR_QUERY_PAGENUM, m_uPageSize);
}

BEGIN_MESSAGE_MAP(CFormExamAddress, CFormView)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_ADD, &CFormExamAddress::OnBnClickedBtnDmExamaddrAdd)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_EDIT, &CFormExamAddress::OnBnClickedBtnDmExamaddrEdit)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_DEL, &CFormExamAddress::OnBnClickedBtnDmExamaddrDel)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_STU_EDIT, &CFormExamAddress::OnBnClickedBtnDmExamaddrStuEdit)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_SEARCH, &CFormExamAddress::OnBnClickedBtnDmExamaddrSearch)
	ON_BN_CLICKED(IDC_BTN_DM_QEXAMADDR_BEGIN, &CFormExamAddress::OnBnClickedBtnDmQexamaddrBegin)
	ON_BN_CLICKED(IDC_BTN_DM_QEXAMADDR_PREV, &CFormExamAddress::OnBnClickedBtnDmQexamaddrPrev)
	ON_BN_CLICKED(IDC_BTN_DM_QEXAMADDR_NEXT, &CFormExamAddress::OnBnClickedBtnDmQexamaddrNext)
	ON_BN_CLICKED(IDC_BTN_DM_QEXAMADDR_END, &CFormExamAddress::OnBnClickedBtnDmQexamaddrEnd)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_OUTPUT, &CFormExamAddress::OnBnClickedBtnDmExamaddrOutput)
END_MESSAGE_MAP()


// CFormExamAddress diagnostics

#ifdef _DEBUG
void CFormExamAddress::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormExamAddress::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormExamAddress message handlers

void CFormExamAddress::OnBnClickedBtnDmExamaddrAdd()
{
	CDlgExamAddr dlg(OP_NEW);

	if(dlg.DoModal() == IDOK)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDObjectRef* pRef = new CYDExamAddress(pDB);
		CComVariant varId(dlg.m_strCode);
		pRef->SetPropVal(FIELD_YDEXAMADDR_ID, &varId);
		CComVariant varName(dlg.m_strAddress);
		pRef->SetPropVal(FIELD_YDEXAMADDR_ADDR, &varName);
		CDBTransactionRef trans(pDB, TRUE);
		HRESULT hr = pRef->Save();
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
		AddExamAddrItem(pRef);
	}
}

void CFormExamAddress::OnBnClickedBtnDmExamaddrEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_ExamAddrListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一考场！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_ExamAddrListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_ExamAddrListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObj = (CYDObjectRef*)m_ExamAddrListCtrl.GetItemData(nItem);
	ASSERT(pObj != NULL);
	CComVariant valID;
	hr = pObj->GetPropVal(FIELD_YDEXAMADDR_ID, &valID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = pObj->GetPropVal(FIELD_YDEXAMADDR_ADDR,&valName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strName = CDataHandler::VariantToString(valName);


	CDlgExamAddr	dlg(OP_EDIT, this);
	dlg.m_strCode = strID;
	dlg.m_strAddress = strName;
	if(dlg.DoModal() == IDOK)
	{
		CYDExamAddress* pExamAddrRef = (CYDExamAddress*)pObj;

		CComVariant valID(dlg.m_strCode);
		hr = pExamAddrRef->SetPropVal(FIELD_YDDEPARTMENT_ID,&valID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valName(dlg.m_strAddress);
		hr = pExamAddrRef->SetPropVal(FIELD_YDEXAMADDR_ADDR,&valName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CDBTransactionRef transRef(pDB,TRUE);
		hr = pExamAddrRef->Update();
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
		m_ExamAddrListCtrl.SetItemText(nItem,1,dlg.m_strCode);
		m_ExamAddrListCtrl.SetItemText(nItem,2,dlg.m_strAddress);
	}
}

void CFormExamAddress::OnBnClickedBtnDmExamaddrDel()
{
	int nCount = m_ExamAddrListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一考场！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_ExamAddrListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_ExamAddrListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_ExamAddrListCtrl.GetItemData(nItem);
	ASSERT(pObjRef );
	if (AfxMessageBox(_T("确定要删除选择的考场吗？"), MB_OKCANCEL) != IDOK)
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
	m_ExamAddrListCtrl.DeleteItem(nItem);
}

void CFormExamAddress::OnBnClickedBtnDmExamaddrStuEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_ExamAddrListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一考场！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_ExamAddrListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_ExamAddrListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObj = (CYDObjectRef*)m_ExamAddrListCtrl.GetItemData(nItem);
	ASSERT(pObj != NULL);
	CDlgExamAddrSetSutdent dlg;
	dlg.m_pExamAddr = dynamic_cast<CYDExamAddress*>(pObj);
	dlg.DoModal();
}

HRESULT CFormExamAddress::AddExamAddrItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDEXAMADDR_ID, &valID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = _pref->GetPropVal(FIELD_YDEXAMADDR_ADDR,&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);


	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_ExamAddrListCtrl.GetItemCount()+1);
	int index = m_ExamAddrListCtrl.InsertItem(m_ExamAddrListCtrl.GetItemCount(), strIndex);
	m_ExamAddrListCtrl.SetItemText(index,1, strID);
	m_ExamAddrListCtrl.SetItemText(index,2, strName);
	m_ExamAddrListCtrl.SetItemData(index, (DWORD_PTR)_pref);
	m_lstExamAddrCache.push_back(_pref);

	return S_OK;
}

void CFormExamAddress::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	m_ExamAddrListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_ExamAddrListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_ExamAddrListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_ExamAddrListCtrl.InsertColumn(1, L"考场编号", LVCFMT_LEFT,iWidth*2/6);
	m_ExamAddrListCtrl.InsertColumn(2, L"考场地址", LVCFMT_LEFT,iWidth*3/6 -2 );
	//初始化查询列表
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_DM_EXAMADDRCON_GRID)->GetWindowRect(&rectGrid);
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

	CBCGPGridRow* pRowNum = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
	pRowNum->GetItem(0)->SetValue(_T("考场编号"));
	pRowNum->GetItem(0)->Enable(FALSE);

	//pRow->GetItem(0)->Enable(FALSE);

	CBCGPGridItem* pItem = new CBCGPGridItem(_T(""));
	pItem->AddOption(_T("等于"));
	pItem->AddOption(_T("不等于"));
	pItem->AddOption(_T("包含"));

	pRowNum->ReplaceItem (1, pItem);
	pRowNum->ReplaceItem(2, new CBCGPGridItem(L""));
	m_QueryGrid.AddRow(pRowNum);

	CBCGPGridRow* pRowName = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
	pRowName->GetItem(0)->SetValue(_T("考场名称"));
	pRowName->GetItem(0)->Enable(FALSE);
	//pRow->GetItem(0)->Enable(FALSE);

	CBCGPGridItem* pItemName = new CBCGPGridItem(_T(""));
	pItemName->AddOption(_T("等于"));
	pItemName->AddOption(_T("不等于"));
	pItemName->AddOption(_T("包含"));

	pRowName->ReplaceItem (1, pItemName);
	pRowName->ReplaceItem(2, new CBCGPGridItem(L""));
	m_QueryGrid.AddRow(pRowName);

}

void CFormExamAddress::OnBnClickedBtnDmExamaddrSearch()
{
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;

	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
	}
	CPropQueryContidition* pPropQuery1 = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropQuery1);
	pPropQuery1->m_strFiledName = FIELD_YDEXAMADDR_ID;
	pPropQuery1->m_uFieldType = VT_BSTR;
	UINT uOptypeID = 0;
	CBCGPGridRow* pRowID = m_QueryGrid.GetRow(0);
	CBCGPGridItem* pOpItemID = pRowID->GetItem(1);
	ASSERT(pOpItemID);
	CString strOpID = pOpItemID->GetLabel();
	if(strOpID.CompareNoCase(_T("等于")) == 0)
	{
		pPropQuery1->m_uOpType = Q_EQUAL;
	}
	else if(strOpID.CompareNoCase(_T("不等于")) == 0)
	{
		pPropQuery1->m_uOpType = Q_NOTEQUAL;
	}
	else if(strOpID.CompareNoCase(_T("包含")) == 0)
	{
		pPropQuery1->m_uOpType = Q_CONTAIN;
	}
	else
	{
		pPropQuery1->m_uOpType = Q_UNKNOW;
	}
	CBCGPGridItem* pValItem = pRowID->GetItem(2);
	ASSERT(pValItem);
	pPropQuery1->m_strConVal = pValItem->GetLabel();

	CPropQueryContidition* pPropQuery2 = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropQuery2);
	pPropQuery2->m_strFiledName = FIELD_YDEXAMADDR_ADDR;
	pPropQuery2->m_uFieldType = VT_BSTR;
	UINT uOptypeAddr = 0;
	CBCGPGridRow* pRowAddr = m_QueryGrid.GetRow(1);
	CBCGPGridItem* pOpItemAddr = pRowAddr->GetItem(1);
	ASSERT(pOpItemAddr);
	CString strOpAddr = pOpItemAddr->GetLabel();
	if(strOpAddr.CompareNoCase(_T("等于")) == 0)
	{
		pPropQuery2->m_uOpType = Q_EQUAL;
	}
	else if(strOpAddr.CompareNoCase(_T("不等于")) == 0)
	{
		pPropQuery2->m_uOpType = Q_NOTEQUAL;
	}
	else if(strOpAddr.CompareNoCase(_T("包含")) == 0)
	{
		pPropQuery2->m_uOpType = Q_CONTAIN;
	}
	else
	{
		pPropQuery2->m_uOpType = Q_UNKNOW;
	}
	CBCGPGridItem* pValItemAddr = pRowAddr->GetItem(2);
	ASSERT(pValItemAddr);
	pPropQuery2->m_strConVal = pValItemAddr->GetLabel();

	m_uPage = 0;
	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CFormExamAddress::OnBnClickedBtnDmQexamaddrBegin()
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

void CFormExamAddress::OnBnClickedBtnDmQexamaddrPrev()
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

void CFormExamAddress::OnBnClickedBtnDmQexamaddrNext()
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

void CFormExamAddress::OnBnClickedBtnDmQexamaddrEnd()
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

HRESULT CFormExamAddress::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	CYDExamAddress examAdressRef(pDB);
	if(_bFirstQuery)
	{
		//第一次查询，要计算总数
		long lCount = 0;
		hr = examAdressRef.GetObjectTotalNumber(&m_lstPropQuery, &lCount);
		if(FAILED(hr))
		{
			return hr;
		}
		m_utotalNumber = (UINT)lCount;
		m_uPageSize = m_utotalNumber/QUESTION_PAGE_COUNT;
		UpdateData(FALSE);
	}
	std::list<CYDObjectRef*> lstExamAddr;

	hr = examAdressRef.GetObjectByCondition(m_uPage,
		&m_lstPropQuery,
		&lstExamAddr);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = InsertlstToListCtrl(lstExamAddr);
	if(FAILED(hr))
	{
		return hr;
	}

	//按钮的可否使用
	if(m_uPage == 0)
	{
		//当前是第一页
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_PREV)->EnableWindow(TRUE);
	}

	if(m_uPageSize <= m_uPage)
	{
		//到达最后一页了
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_END)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QEXAMADDR_END)->EnableWindow(TRUE);
	}

	return S_OK;
}

HRESULT CFormExamAddress::InsertlstToListCtrl(std::list<CYDObjectRef*> &lst)
{
	HRESULT hr = E_FAIL;

	m_ExamAddrListCtrl.DeleteAllItems();

	std::list<CYDObjectRef*>::const_iterator itr = lst.begin();
	for (; itr != lst.end(); ++itr)
	{
		hr = AddExamAddrItem(*itr);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}
BOOL CFormExamAddress::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormExamAddress::OnBnClickedBtnDmExamaddrOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_ExamAddrListCtrl);
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
