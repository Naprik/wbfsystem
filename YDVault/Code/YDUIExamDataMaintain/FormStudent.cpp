// FormStudent.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormStudent.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../Base/AutoClean.h"
#include "../YDUIExam\DlgStunfo.h"
#include "../DBBase/DBTransactionRef.h"
#include "../Base/DataHandler.h"
#include "../YDExamObjRef/YDClassRef.h"
#include "../UIBase\ListXlsoutput.h"
#include "DlgInputStudentPreview.h"
#include "../UIBase\ListXlsInput.h"

// CFormStudent

IMPLEMENT_DYNCREATE(CFormStudent, CYdFormView)

CFormStudent::CFormStudent()
	: CYdFormView(CFormStudent::IDD)
	, m_utotalNumber(0)
	, m_uPageSize(0)
	, m_uPage(0)
{

}

CFormStudent::~CFormStudent()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstDeptCache);
	CListAutoClean<CYDObjectRef> clr2(m_lstStudentCache);
	CListAutoClean<CQueryProp> clr3(m_lstQueryProp);
	CListAutoClean<CPropQueryContidition> clr4(m_lstPropQuery);
	ClearClassCache();
}

void CFormStudent::ClearClassCache()
{
	CListAutoClean<CYDObjectRef> clr2(m_lstClsCache);

}

void CFormStudent::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_DM_STUCONN_DEPT, m_cmbDept);
	DDX_Control(pDX, IDC_CMB_STUCONN_CLS, m_cmbCls);
	DDX_Control(pDX, IDC_LIST_DM_STU, m_StuList);

	DDX_Text(pDX, IDC_EDIT_QL_QUERY_TOTAL, m_utotalNumber);
	DDX_Text(pDX, IDC_EDIT_QL_QUERY_PAGENUM, m_uPageSize);
}

BEGIN_MESSAGE_MAP(CFormStudent, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_DM_STU_SEARCH, &CFormStudent::OnBnClickedBtnDmStuSearch)
	ON_BN_CLICKED(IDC_BTN_DM_STU_ADD, &CFormStudent::OnBnClickedBtnDmStuAdd)
	ON_BN_CLICKED(IDC_BTN_DM_STU_EDIT, &CFormStudent::OnBnClickedBtnDmStuEdit)
	ON_BN_CLICKED(IDC_BTN_DM_STU_DEL, &CFormStudent::OnBnClickedBtnDmStuDel)
	ON_CBN_SELCHANGE(IDC_CMB_DM_STUCONN_DEPT, &CFormStudent::OnCbnSelchangeCmbDmStuconnDept)

	ON_BN_CLICKED(IDC_BTN_DM_QSTU_BEGIN, &CFormStudent::OnBnClickedBtnDmQstuBegin)
	ON_BN_CLICKED(IDC_BTN_DM_QSTU_PREV, &CFormStudent::OnBnClickedBtnDmQstuPrev)
	ON_BN_CLICKED(IDC_BTN_DM_QSTU_NEXT, &CFormStudent::OnBnClickedBtnDmQstuNext)
	ON_BN_CLICKED(IDC_BTN_DM_QSTU_END, &CFormStudent::OnBnClickedBtnDmQstuEnd)
	ON_BN_CLICKED(IDC_BTN_DM_STU_ADDQC, &CFormStudent::OnBnClickedBtnDmStuAddqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_DELQC, &CFormStudent::OnBnClickedBtnDmStuDelqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_CLEARQC, &CFormStudent::OnBnClickedBtnDmStuClearqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_OUTPUT, &CFormStudent::OnBnClickedBtnDmStuOutput)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CFormStudent::OnBnClickedButtonInput)
END_MESSAGE_MAP()


// CFormStudent diagnostics

#ifdef _DEBUG
void CFormStudent::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormStudent::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormStudent message handlers

void CFormStudent::OnBnClickedBtnDmStuSearch()
{
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;

	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
	}
	hr = CreatePropQueryContidion();
	if (FAILED(hr))
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

void CFormStudent::OnBnClickedBtnDmStuAdd()
{
	CDlgStunfo dlg(OP_NEW);

	CYdStudentRef* pStu = NULL;
	dlg.m_pStudent = pStu;
	if (dlg.DoModal() == IDOK)
	{
		AddItem(dlg.m_pStudent);
	}
}

void CFormStudent::OnBnClickedBtnDmStuEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_StuList.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一个学生！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_StuList.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_StuList.GetNextSelectedItem(pos);
	CYDObjectRef* pObjStudent = (CYDObjectRef*)m_StuList.GetItemData(nItem);
	ASSERT(pObjStudent != NULL);

	CDlgStunfo	dlg(OP_EDIT, this);
	dlg.m_pStudent = dynamic_cast<CYdStudentRef*>(pObjStudent);
	dlg.DoModal();

	m_StuList.SetItemText(nItem,1,dlg.m_strNum);
	m_StuList.SetItemText(nItem,2,dlg.m_strName);
	CString strGender;
	dlg.m_pStudent->GetGender(&strGender);
	m_StuList.SetItemText(nItem,3,strGender);
	m_StuList.SetItemText(nItem,4,dlg.m_strBlueteeth);
}

void CFormStudent::OnBnClickedBtnDmStuDel()
{
	int nCount = m_StuList.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一个学生！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_StuList.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_StuList.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_StuList.GetItemData(nItem);
	ASSERT(pObjRef );
	if (AfxMessageBox(_T("确定要删除选择的学生吗？"), MB_OKCANCEL) != IDOK)
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
	m_StuList.DeleteItem(nItem);
}

void CFormStudent::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	
	HRESULT hr = E_FAIL;
	//初始化部门下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT, pDB, &m_lstDeptCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstDeptCache.size() > 0)
	{
		int nIndex = 0;
		m_cmbDept.InsertString(nIndex++, L"");
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstDeptCache.begin();
			itr != m_lstDeptCache.end(); ++itr, ++nIndex)
		{
			CString strDeptName;
			(*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDeptName);
			m_cmbDept.InsertString(nIndex, strDeptName);
			m_cmbDept.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}
	
	//初始化学生查询条件
	CRect gridLocate;
	GetDlgItem(IDC_STATIC_DM_STUCON_GRID)->GetWindowRect(gridLocate);
	ScreenToClient(&gridLocate);
	if (!m_gridConn.Create(WS_CHILD|WS_VISIBLE|WS_BORDER,gridLocate,
		this,-1))
	{
		TRACE0("Failed to m_listctrl.Create!\n");
		return ;
	}
	m_gridConn.EnableHeader(TRUE,0);
	m_gridConn.EnableDragHeaderItems(FALSE);
	m_gridConn.SetWholeRowSel(TRUE);

	//初始化表头
	CString strName = L"属性";
	m_gridConn.InsertColumn(0,strName,gridLocate.Width()/3-1);
	CString strOperator = L"操作";
	m_gridConn.InsertColumn(1,strOperator,gridLocate.Width()/6-1);
	CString strValue = L"属性值";
	m_gridConn.InsertColumn(2,strValue,gridLocate.Width()/2-2);
	// 	CString strLogic = L"逻辑符号";
	// 	m_gridConn.InsertColumn(3,strLogic,gridLocate.Width()/4-1);
	m_gridConn.AdjustLayout ();

	//初始化结果列表
	m_StuList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_StuList.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_StuList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/10);
	m_StuList.InsertColumn(1, L"学号", LVCFMT_LEFT,iWidth*3/10);
	m_StuList.InsertColumn(2, L"姓名", LVCFMT_LEFT,iWidth*2/10);
	m_StuList.InsertColumn(3, L"性别", LVCFMT_LEFT,iWidth*1/10);
	m_StuList.InsertColumn(4, L"学生机设备号", LVCFMT_LEFT,iWidth*3/10 -2);

	CreateQuestionQueryProp();
}

void CFormStudent::OnCbnSelchangeCmbDmStuconnDept()
{
	CString strSelDept;
	m_cmbDept.GetWindowText(strSelDept);

	for (int i = m_cmbCls.GetCount() - 1; i >= 0; i--)
	{
		m_cmbCls.DeleteString(i);
	}
	ClearClassCache();

	if (strSelDept.GetLength() > 0)
	{
		CYDDepartmentRef* pDept = (CYDDepartmentRef*)m_cmbDept.GetItemData(m_cmbDept.GetCurSel());
		ASSERT(pDept);
		HRESULT hr = pDept->GetAllClasses(&m_lstClsCache, NULL);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if (m_lstClsCache.size() > 0)
		{
			int nIndex = 0;
			m_cmbCls.InsertString(nIndex++, L"");
			for (std::list<CYDObjectRef*>::const_iterator itr = m_lstClsCache.begin();
				itr != m_lstClsCache.end(); ++itr, ++nIndex)
			{
				CString strClsName;
				(*itr)->GetPropVal(FIELD_YDCLASS_NAME, strClsName);
				m_cmbCls.InsertString(nIndex, strClsName);
				m_cmbCls.SetItemData(nIndex, (DWORD_PTR)(*itr));
			}
		}
	}
	m_cmbCls.SetFocus();
}


void CFormStudent::OnBnClickedBtnDmQstuBegin()
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

void CFormStudent::OnBnClickedBtnDmQstuPrev()
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

void CFormStudent::OnBnClickedBtnDmQstuNext()
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

void CFormStudent::OnBnClickedBtnDmQstuEnd()
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

HRESULT CFormStudent::AddItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}
	
	CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDSTU_ID, &valID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = _pref->GetPropVal(FIELD_YDSTU_NAME,&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);

	CString strGender;
	CYdStudentRef* pStu = dynamic_cast<CYdStudentRef*>(_pref);
	pStu->GetGender(&strGender);

	CString strDevice;
	pStu->GetPropVal(FIELD_YDSTU_EVID, strDevice);

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_StuList.GetItemCount()+1);
	int index = m_StuList.InsertItem(m_StuList.GetItemCount(), strIndex);
	m_StuList.SetItemText(index,1, strID);
	m_StuList.SetItemText(index,2, strName);
	m_StuList.SetItemText(index,3, strGender);
	m_StuList.SetItemText(index,4, strDevice);
	m_StuList.SetItemData(index, (DWORD_PTR)_pref);
	m_lstStudentCache.push_back(_pref);
	return S_OK;
}
BOOL CFormStudent::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

HRESULT CFormStudent::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CString strSelDept;
	m_cmbDept.GetWindowText(strSelDept);
	OBJID deptID = ID_EMPTY;
	if (strSelDept.GetLength() > 0)
	{
		CYDDepartmentRef* pDept = (CYDDepartmentRef*)m_cmbDept.GetItemData(m_cmbDept.GetCurSel());
		pDept->GetID(&deptID);
	}
	CString strSelClass;
	m_cmbCls.GetWindowText(strSelClass);
	OBJID clsID = ID_EMPTY;
	if (strSelClass.GetLength() > 0)
	{
		CYDClassRef* pCls = (CYDClassRef*)m_cmbCls.GetItemData(m_cmbCls.GetCurSel());
		pCls->GetID(&clsID);
	}

	CYdStudentRef stuRef(pDB);
	if(_bFirstQuery)
	{
		//第一次查询，要计算总数
		long lCount = 0;
		hr = stuRef.GetStudentTotalNumber(deptID, clsID, &m_lstPropQuery, &lCount);
		if(FAILED(hr))
		{
			return hr;
		}
		m_utotalNumber = (UINT)lCount;
		m_uPageSize = m_utotalNumber/QUESTION_PAGE_COUNT;
		UpdateData(FALSE);
	}
	
	
	std::list<CYDObjectRef*> lstStus;

	hr = stuRef.GetStudentByConditionFromDeptAndCls(m_uPage, deptID, clsID,
									&m_lstPropQuery,
									&lstStus);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = InsertlstToListCtrl(lstStus);
	if(FAILED(hr))
	{
		return hr;
	}

	//按钮的可否使用
	if(m_uPage == 0)
	{
		//当前是第一页
		GetDlgItem(IDC_BTN_DM_QSTU_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QSTU_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QSTU_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QSTU_PREV)->EnableWindow(TRUE);
	}

	if(m_uPageSize <= m_uPage)
	{
		//到达最后一页了
		GetDlgItem(IDC_BTN_DM_QSTU_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QSTU_END)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QSTU_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QSTU_END)->EnableWindow(TRUE);
	}

	return S_OK;
}

HRESULT CFormStudent::InsertlstToListCtrl(std::list<CYDObjectRef*> &lst)
{
	HRESULT hr = E_FAIL;

	m_StuList.DeleteAllItems();

	std::list<CYDObjectRef*>::const_iterator itr = lst.begin();
	for (; itr != lst.end(); ++itr)
	{
		hr = AddItem(*itr);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CFormStudent::CreateQuestionQueryProp()
{
	HRESULT hr= E_FAIL;
	{
		CListAutoClean<CQueryProp> clr(m_lstQueryProp);
	}
	CQueryProp* pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_NAME;
	pQueryProp->m_strDisplayName = _T("姓名");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_ID;
	pQueryProp->m_strDisplayName = _T("学号");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_GENDER;
	pQueryProp->m_strDisplayName = _T("性别");
	pQueryProp->m_uType = VT_I4;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_AGE;
	pQueryProp->m_strDisplayName = _T("年龄");
	pQueryProp->m_uType = VT_I4;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_TEL;
	pQueryProp->m_strDisplayName = _T("电话");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_MAIL;
	pQueryProp->m_strDisplayName = _T("邮箱");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_EVID;
	pQueryProp->m_strDisplayName = _T("设备号");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);
	
	return S_OK;
}

void CFormStudent::OnBnClickedBtnDmStuAddqc()
{
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

void CFormStudent::OnBnClickedBtnDmStuDelqc()
{
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

void CFormStudent::OnBnClickedBtnDmStuClearqc()
{
	while(m_gridConn.GetRowCount() > 0)
	{
		m_gridConn.RemoveRow(0);
	}
}

HRESULT CFormStudent::CreatePropQueryContidion()
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
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
		if (strFieldName == FIELD_YDSTU_GENDER)
		{
			CString str = pValItem->GetLabel();
			if (str == L"男")
			{
				pPropQuery->m_strConVal = L"0";
			}
			else if (str == L"女")
			{
				pPropQuery->m_strConVal = L"1";
			}
		}
		else
		{
			pPropQuery->m_strConVal = pValItem->GetLabel();
		}

	}
	return S_OK;
}

void CFormStudent::OnBnClickedBtnDmStuOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_StuList);
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

void CFormStudent::OnBnClickedButtonInput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_cmbCls.GetCurSel() == -1)
	{
		AfxMessageBox(_T("请选择要导入的班级！"));
		return;
	}
	CYDClassRef* pCls = (CYDClassRef*)m_cmbCls.GetItemData(m_cmbCls.GetCurSel());
	ASSERT(pCls);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Excle文件(*.xls)|*.xls|所有文件(*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	CDlgInputStudentPreview dlgPreview;
	dlgPreview.m_pClass = pCls;
	CListXlsInput ListXlsInput(dlg.GetPathName());
	hr = ListXlsInput.Read(&dlgPreview.m_lstPropVal);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(dlgPreview.DoModal() != IDOK)
	{
		return;
	}

}
