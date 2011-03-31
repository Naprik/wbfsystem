// FormStudent.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormQueryMark.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../Base/AutoClean.h"

#include "../DBBase/DBTransactionRef.h"
#include "../Base/DataHandler.h"
#include "../YDExamObjRef/YDStuMark.h"

// CFormQueryMark

IMPLEMENT_DYNCREATE(CFormQueryMark, CYdFormView)

CFormQueryMark::CFormQueryMark()
	: CYdFormView(CFormQueryMark::IDD)
	, m_utotalNumber(0)
	, m_uPageSize(0)
	, m_uPage(0)
{

}

CFormQueryMark::~CFormQueryMark()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstPaperCache);
	CListAutoClean<CYDObjectRef> clr2(m_lstUnitCache);
	CListAutoClean<CPropQueryContidition> clr3(m_lstPropQuery);
}

void CFormQueryMark::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_DM_QMARK_MARKS, m_UnitList);
	DDX_Text(pDX, IDC_EDIT_DM_QMARK_TOTAL, m_utotalNumber);
	DDX_Text(pDX, IDC_EDIT_DM_QMARK_PAGENUM, m_uPageSize);
	DDX_Control(pDX, IDC_DM_QMARK_TAB, m_Tab);
	DDX_Control(pDX, IDC_CMB_DM_QMARK_PAPER, m_cmbPaper);
}

BEGIN_MESSAGE_MAP(CFormQueryMark, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_SEARCH, &CFormQueryMark::OnBnClickedBtnDmQMarkSearch)

	ON_BN_CLICKED(IDC_BTN_DM_QMARK_BEGIN, &CFormQueryMark::OnBnClickedBtnDmQMarkBegin)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_PREV, &CFormQueryMark::OnBnClickedBtnDmQMarkPrev)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_NEXT, &CFormQueryMark::OnBnClickedBtnDmQMarkNext)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_END, &CFormQueryMark::OnBnClickedBtnDmQMarkEnd)
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_DM_QMARK_TAB, &CFormQueryMark::OnTcnSelchangeDmQmarkTab)
END_MESSAGE_MAP()


// CFormQueryMark diagnostics

#ifdef _DEBUG
void CFormQueryMark::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormQueryMark::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormQueryMark message handlers

void CFormQueryMark::OnBnClickedBtnDmQMarkSearch()
{
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;
	CString strSelPaper;
	m_cmbPaper.GetWindowText(strSelPaper);
	OBJID paperID = ID_EMPTY;
	if (strSelPaper.GetLength() <= 0)
	{
		AfxMessageBox(L"请选择试卷！");
		return;
	}

	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
		m_lstSelClass.clear();
	}

	AddPaperPropQueryContidion();
	if (m_Tab.GetCurSel() == 1)
	{
		m_PageClass.GetSearchClass(&m_lstSelClass);
		if (m_lstSelClass.size() <= 0)
		{
			AfxMessageBox(L"请选择要查询的班级！");
			return;
		}
	}
	else if (m_Tab.GetCurSel() == 0)
	{
		m_PageStudent.CreatePropQueryContidion(m_lstPropQuery, &m_bFromStu);
		if (m_lstPropQuery.size() <= 1)
		{
			AfxMessageBox(L"请输入查询条件！");
			return;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	

	m_uPage = 0;
	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CFormQueryMark::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	TCITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = L"个人成绩查询";

	m_Tab.InsertItem (0,&item);
	item.pszText = L"班级成绩查询";
	m_Tab.InsertItem (1,&item);

	m_PageClass.Create(IDD_YDUIEXAM_DATA_MAINTAIN_DLG_QMARKBYCLASS_SHEET, &m_Tab); 
	m_PageStudent.Create(IDD_YDUIEXAM_DATA_MAINTAIN_DLG_QMARKBYSTU_SHEET,&m_Tab); 
	
	//获得IDC_TABTEST客户区大小 
	CRect rs; 
	m_Tab.GetClientRect(&rs);
	//调整子对话框在父窗口中的位置 
	rs.top+=21;  
	rs.bottom-=2;  
	rs.left+=1;  
	rs.right-=2;  
	//设置子对话框尺寸并移动到指定位置 
	m_PageClass.MoveWindow(&rs); 
	m_PageStudent.MoveWindow(&rs); 
	//分别设置隐藏和显示 
	m_PageStudent.ShowWindow(SW_SHOW); 
	m_PageClass.ShowWindow(SW_HIDE); 
	//设置默认的选项卡 
	m_Tab.SetCurSel(0); 

	HRESULT hr = E_FAIL;
	//初始化试卷下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_PAPER, pDB, &m_lstPaperCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstPaperCache.size() > 0)
	{
		int nIndex = 0;
		m_cmbPaper.InsertString(nIndex++, L"");
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstPaperCache.begin();
			itr != m_lstPaperCache.end(); ++itr, ++nIndex)
		{
			CString strPaperName;
			(*itr)->GetPropVal(FIELD_YDPAPER_NAME, strPaperName);
			m_cmbPaper.InsertString(nIndex, strPaperName);
			m_cmbPaper.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}
	
	m_UnitList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_UnitList.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_UnitList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/10);
	m_UnitList.InsertColumn(1, L"学号", LVCFMT_LEFT,iWidth*3/10);
	m_UnitList.InsertColumn(2, L"姓名", LVCFMT_LEFT,iWidth*2/10);
	m_UnitList.InsertColumn(3, L"准考证号", LVCFMT_LEFT,iWidth*3/10);
	m_UnitList.InsertColumn(4, L"分数", LVCFMT_LEFT,iWidth*1/10 -2);
}

void CFormQueryMark::OnBnClickedBtnDmQMarkBegin()
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

void CFormQueryMark::OnBnClickedBtnDmQMarkPrev()
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

void CFormQueryMark::OnBnClickedBtnDmQMarkNext()
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

void CFormQueryMark::OnBnClickedBtnDmQMarkEnd()
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

HRESULT CFormQueryMark::AddItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}
	CYDStuMark* pUnit = dynamic_cast<CYDStuMark*>(_pref);
	CYdStudentRef* pStudent = NULL;
	pUnit->GetStudent(pStudent);
	CPtrAutoClean<CYdStudentRef> clr(pStudent);
	CString strID;
	hr = pStudent->GetPropVal(FIELD_YDSTU_ID, strID);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strName;
	hr = pStudent->GetPropVal(FIELD_YDSTU_NAME, strName);
	if(FAILED(hr))
	{
		return hr;
	}
	
	CString strExamId;
	hr = pUnit->GetPropVal(FIELD_YDSTUMARK_EXAMID, strExamId);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strMark;
	hr = pUnit->GetPropVal(FIELD_YDSTUMARK_MARK, strMark);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_UnitList.GetItemCount()+1);
	int index = m_UnitList.InsertItem(m_UnitList.GetItemCount(), strIndex);
	m_UnitList.SetItemText(index,1, strID);
	m_UnitList.SetItemText(index,2, strName);
	m_UnitList.SetItemText(index,3, strExamId);
	m_UnitList.SetItemText(index,4, strMark);
	m_UnitList.SetItemData(index, (DWORD_PTR)_pref);
	m_lstUnitCache.push_back(_pref);
	return S_OK;
}
BOOL CFormQueryMark::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

HRESULT CFormQueryMark::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDStuMark markRef(pDB);
	if (m_Tab.GetCurSel() == 1)
	{
		if(_bFirstQuery)
		{
			//第一次查询，要计算总数
			long lCount = 0;
			
			hr = markRef.GetCountByCls(&m_lstSelClass, &m_lstPropQuery, &lCount);
			if(FAILED(hr))
			{
				return hr;
			}

			m_utotalNumber = (UINT)lCount;
			m_uPageSize = m_utotalNumber/QUESTION_PAGE_COUNT;
			UpdateData(FALSE);
		}

		std::list<CYDObjectRef*> lstPapers;
		
		hr = markRef.GetUnitsByCls(m_uPage, &m_lstSelClass, &m_lstPropQuery,
									&lstPapers);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = InsertlstToListCtrl(lstPapers);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else if (m_Tab.GetCurSel() == 0)
	{
		if(_bFirstQuery)
		{
			//第一次查询，要计算总数
			long lCount = 0;
			if (m_bFromStu)
			{
				hr = markRef.GetCountByStu(&m_lstPropQuery, &lCount);
				if(FAILED(hr))
				{
					return hr;
				}
			}
			else
			{
				hr = markRef.GetCountBySelfProp(&m_lstPropQuery, &lCount);
				if(FAILED(hr))
				{
					return hr;
				}
			}
			
			m_utotalNumber = (UINT)lCount;
			m_uPageSize = m_utotalNumber/QUESTION_PAGE_COUNT;
			UpdateData(FALSE);
		}
		
		
		std::list<CYDObjectRef*> lstPapers;
		if (m_bFromStu)
		{
			hr = markRef.GetUnitsByStu(m_uPage, &m_lstPropQuery,
										&lstPapers);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else
		{
			hr = markRef.GetUnitsBySelfProp(m_uPage, &m_lstPropQuery,
				&lstPapers);
			if(FAILED(hr))
			{
				return hr;
			}
		}

		hr = InsertlstToListCtrl(lstPapers);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	
	//按钮的可否使用
	if(m_uPage == 0)
	{
		//当前是第一页
		GetDlgItem(IDC_BTN_DM_QMARK_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QMARK_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QMARK_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QMARK_PREV)->EnableWindow(TRUE);
	}

	if(m_uPageSize <= m_uPage)
	{
		//到达最后一页了
		GetDlgItem(IDC_BTN_DM_QMARK_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QMARK_END)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QMARK_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QMARK_END)->EnableWindow(TRUE);
	}

	return S_OK;
}

HRESULT CFormQueryMark::InsertlstToListCtrl(std::list<CYDObjectRef*> &lst)
{
	HRESULT hr = E_FAIL;

	m_UnitList.DeleteAllItems();

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


HRESULT CFormQueryMark::AddPaperPropQueryContidion()
{
	HRESULT hr = E_FAIL;
	
	CString strSelPaper;
	m_cmbPaper.GetWindowText(strSelPaper);
	OBJID paperID = ID_EMPTY;
	if (strSelPaper.GetLength() > 0)
	{
		OBJID paperID;
		CYDPaper* pPaper = (CYDPaper*)m_cmbPaper.GetItemData(m_cmbPaper.GetCurSel());
		pPaper->GetID(&paperID);
		CString strID;
		strID.Format(L"%d", paperID);

		CPropQueryContidition* pPropQuery = new CPropQueryContidition();
		m_lstPropQuery.push_back(pPropQuery);
		pPropQuery->m_strFiledName = FIELD_YDSTUMARK_PAPERID;
		pPropQuery->m_uFieldType = VT_I4;
		pPropQuery->m_uOpType = Q_EQUAL;
		pPropQuery->m_strConVal = strID;
	}
		
	return S_OK;
}

void CFormQueryMark::OnTcnSelchangeDmQmarkTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch(m_Tab.GetCurSel())
	{
	case 0: 
		m_PageStudent.ShowWindow(SW_SHOW); 
		m_PageClass.ShowWindow(SW_HIDE); 
		break; 
	case 1: 
		m_PageStudent.ShowWindow(SW_HIDE); 
		m_PageClass.ShowWindow(SW_SHOW); 
		break; 
	}
	*pResult = 0;
}
