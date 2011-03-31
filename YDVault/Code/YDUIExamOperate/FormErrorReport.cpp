// FormPaper.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormErrorReport.h"
#include "../YDExamObjRef/YDSubjectRef.h"
#include "../ObjRef/YDObjectRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../YDExamObjRef/YDClassRef.h"
#include "../YDExamObjRef/YDStuMark.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjHelper/PaperAnsysHelper.h"
#include "../\UIBase\ListXlsoutput.h"
#include "../ObjHelper\PaperErrorResult.h"
#include "DlgErrorDetail.h"


// CFormPaper

IMPLEMENT_DYNCREATE(CFormErrorReport, CYdFormView)

CFormErrorReport::CFormErrorReport()
	: CYdFormView(CFormErrorReport::IDD)
	
{

}

CFormErrorReport::~CFormErrorReport()
{
	CListAutoClean<CQueryProp> clr1(m_lstQueryProp);
	CListAutoClean<CYDObjectRef> clr2(m_lstSubjectCache);
	CListAutoClean<CYDObjectRef> clr3(m_lstPaperCache);
	CListAutoClean<CPaperErrorResult> clr4(m_lstErrorResult);
	CListAutoClean<CPropQueryContidition> clr5(m_lstStuPropQuery);
	CListAutoClean<CPropQueryContidition> clr6(m_lstPaperPropQuery);
	CListAutoClean<CYDObjectRef> clr7(m_lstMarks);
}

void CFormErrorReport::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMB_OP_ERRORREPORT_SUBJECT, m_cmbSubject);
	DDX_Control(pDX, IDC_LIST_OP_ERRORREPORT_LIST, m_ErrorReportList);
	DDX_Control(pDX, IDC_CMB_OP_ERRORREPORT_PAPER, m_cmbPaper);
	DDX_Control(pDX, IDC_DATE_OP_ERRORREPORT_BEGIN, m_begin);
	DDX_Control(pDX, IDC_DATE_OP_ERRORREPORT_END, m_end);
}

BEGIN_MESSAGE_MAP(CFormErrorReport, CYdFormView)

	
	ON_BN_CLICKED(IDC_BTN_DM_STU_DELQC, &CFormErrorReport::OnBnClickedBtnDmStuDelqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_ADDQC, &CFormErrorReport::OnBnClickedBtnDmStuAddqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_CLEARQC, &CFormErrorReport::OnBnClickedBtnDmStuClearqc)
	ON_BN_CLICKED(IDC_BTN_OP_ERRORREPORT_SEARCH, &CFormErrorReport::OnBnClickedBtnOpErrorreportSearch)
	ON_CBN_SELCHANGE(IDC_CMB_OP_ERRORREPORT_SUBJECT, &CFormErrorReport::OnCbnSelchangeCmbOpErrorreportSubject)
	ON_BN_CLICKED(IDC_BTN_OP_ERRORREPORT_OUTPUT, &CFormErrorReport::OnBnClickedBtnOpErrorreportOutput)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OP_ERRORREPORT_LIST, &CFormErrorReport::OnNMDblclkListOpErrorreportList)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_OP_ERRORREPORT_LIST, &CFormErrorReport::OnLvnItemchangedListOpErrorreportList)
END_MESSAGE_MAP()


// CFormPaper diagnostics

#ifdef _DEBUG
void CFormErrorReport::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormErrorReport::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormPaper message handlers
void CFormErrorReport::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

		//初始化查询列表
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_DM_QMARK_GRID)->GetWindowRect(&rectGrid);
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
	m_QueryGrid.InsertColumn(2, L"属性值", iWidthGrid*2/4);

	m_QueryGrid.AdjustLayout ();

	CreateStuQuestionQueryProp();

	//初始化部门下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	HRESULT hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT, pDB, &m_lstSubjectCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstSubjectCache.size() > 0)
	{
		int nIndex = 0;
		m_cmbSubject.InsertString(nIndex++, L"");
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstSubjectCache.begin();
			itr != m_lstSubjectCache.end(); ++itr, ++nIndex)
		{
			CString strDeptName;
			(*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDeptName);
			m_cmbSubject.InsertString(nIndex, strDeptName);
			m_cmbSubject.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}
	//初始化错题报表列表
	m_ErrorReportList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_ErrorReportList.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_ErrorReportList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/8);
	m_ErrorReportList.InsertColumn(1, L"试卷名称", LVCFMT_LEFT,iWidth*2/8);
	m_ErrorReportList.InsertColumn(2, L"错题", LVCFMT_LEFT,iWidth*5/8 - 2);
}

HRESULT CFormErrorReport::CreateStuQuestionQueryProp()
{
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
	pQueryProp->m_strInternalName = FIELD_YDSTUMARK_EXAMID;
	pQueryProp->m_strDisplayName = _T("准考证号");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	return S_OK;
}


void CFormErrorReport::OnBnClickedBtnDmStuDelqc()
{
	CUIntArray nRows;
	CList<CBCGPGridItem*,CBCGPGridItem*> selItems;
	m_QueryGrid.GetSelectedItems(selItems);
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
		m_QueryGrid.RemoveRow(nRows[i],TRUE);
	}
	GetDlgItem(IDC_BTN_DM_STU_ADDQC)->EnableWindow(TRUE);
}

void CFormErrorReport::OnBnClickedBtnDmStuAddqc()
{
	HRESULT hr = E_FAIL;
	CBCGPGridRow* pRow = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
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

	m_QueryGrid.AddRow(pRow);
	GetDlgItem(IDC_BTN_DM_STU_ADDQC)->EnableWindow(FALSE);
}

void CFormErrorReport::OnBnClickedBtnDmStuClearqc()
{
	while(m_QueryGrid.GetRowCount() > 0)
	{
		m_QueryGrid.RemoveRow(0);
	}
	GetDlgItem(IDC_BTN_DM_STU_ADDQC)->EnableWindow(TRUE);
}

void CFormErrorReport::OnBnClickedBtnOpErrorreportSearch()
{
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;
	CString strSelSubject;
	m_cmbSubject.GetWindowText(strSelSubject);
	OBJID paperID = ID_EMPTY;
	if (strSelSubject.GetLength() <= 0)
	{
		AfxMessageBox(L"请选择科目！");
		return;
	}

	{
		CListAutoClean<CPropQueryContidition> clr(m_lstStuPropQuery);
		CListAutoClean<CPropQueryContidition> clr1(m_lstPaperPropQuery);
	}

	CreatePaperPropQueryContidion();
	m_bFromStu = FALSE;
	CreateStuPropQueryContidion( &m_bFromStu);
 	if (m_lstStuPropQuery.size() <= 0 || 
		m_lstPaperPropQuery.size() <= 0)
 	{
		//两种查询条件都要有
 		AfxMessageBox(L"请输入查询条件！");
 		return;
 	}
	
	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

HRESULT CFormErrorReport::CreatePaperPropQueryContidion()
{
	HRESULT hr = E_FAIL;

	CString strSelSubject;
	m_cmbSubject.GetWindowText(strSelSubject);
	if(strSelSubject.GetLength() <= 0)
	{
		return S_FALSE;
	}
	int iPaperIndex = m_cmbPaper.GetCurSel();
	if(iPaperIndex <= 0)
	{
		//查询当前科目下所有的试卷
		CYDSubjectRef* pSub = (CYDSubjectRef*)m_cmbSubject.GetItemData(m_cmbSubject.GetCurSel());
		ASSERT(pSub);
		std::list<CYDObjectRef*> lstPaper;
		CListAutoClean<CYDObjectRef> clr(lstPaper);
		hr = pSub->GetPapers(&lstPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		for(std::list<CYDObjectRef*>::const_iterator itr = lstPaper.begin();
			itr != lstPaper.end();++itr)
		{
			CYDPaper* pPaper = (CYDPaper*)(*itr);
			ASSERT(pPaper);
			OBJID paperID = ID_EMPTY;
			hr = pPaper->GetID(&paperID);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strID;
			strID.Format(_T("%d"), paperID);
			CPropQueryContidition* pPropQuery = new CPropQueryContidition();
			m_lstPaperPropQuery.push_back(pPropQuery);
			pPropQuery->m_strFiledName = FIELD_YDSTUMARK_PAPERID;
			pPropQuery->m_uFieldType = VT_I4;
			pPropQuery->m_uOpType = Q_EQUAL;
			pPropQuery->m_strConVal = strID;
		}
	}
	else 
	{
		//查询单独一个试卷
		CYDPaper* pPaper = (CYDPaper*)m_cmbPaper.GetItemData(iPaperIndex);
		ASSERT(pPaper);
		OBJID paperID = ID_EMPTY;
		hr = pPaper->GetID(&paperID);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strID;
		strID.Format(_T("%d"), paperID);
		CPropQueryContidition* pPropQuery = new CPropQueryContidition();
		m_lstPaperPropQuery.push_back(pPropQuery);
		pPropQuery->m_strFiledName = FIELD_YDSTUMARK_PAPERID;
		pPropQuery->m_uFieldType = VT_I4;
		pPropQuery->m_uOpType = Q_EQUAL;
		pPropQuery->m_strConVal = strID;
	}
	

	return S_OK;
}

HRESULT CFormErrorReport::CreateStuPropQueryContidion(BOOL* pbFromStu)
{
	*pbFromStu = TRUE;
	for(int i = 0; i< m_QueryGrid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_QueryGrid.GetRow(i);
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
				if (strFieldName.CompareNoCase(FIELD_YDSTUMARK_EXAMID) == 0)
				{
					*pbFromStu = FALSE;
				}
				break;
			}
		}
		if(strFieldName.IsEmpty())
		{
			//没有找到字段
			continue;
		}
		CPropQueryContidition* pPropQuery = new CPropQueryContidition();

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
			delete pPropQuery;
			pPropQuery = NULL;
			return S_OK;
		}
		m_lstStuPropQuery.push_back(pPropQuery);
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

HRESULT CFormErrorReport::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDStuMark markRef(pDB);
	
	{
		CListAutoClean<CYDObjectRef> clr1(m_lstMarks);
	}
	
	//要将每个PaperID的查询条件一一与Stu的查询条件合并起来查找
	//因为有可能查多个Paper
	for(std::list<CPropQueryContidition*>::const_iterator itr = m_lstPaperPropQuery.begin();
		itr != m_lstPaperPropQuery.end();++itr)
	{
		std::list<CPropQueryContidition*> lstPropQuery;
		lstPropQuery.push_back(*itr);
		for(std::list<CPropQueryContidition*>::const_iterator itrStu = m_lstStuPropQuery.begin();
			itrStu != m_lstStuPropQuery.end();++itrStu)
		{
			lstPropQuery.push_back(*itrStu);
		}
		COleDateTime dateBegin, dateEnd;
		m_begin.GetTime(dateBegin);
		dateBegin.SetDateTime(dateBegin.GetYear(), dateBegin.GetMonth(), dateBegin.GetDay(),
			0, 0, 0);
		m_end.GetTime(dateEnd);
		dateEnd.SetDateTime(dateEnd.GetYear(), dateEnd.GetMonth(), dateEnd.GetDay(),
			23, 59, 59);
		CString strBegin = CDataHandler::DateTimeToStr(dateBegin, 0);
		CString strEnd = CDataHandler::DateTimeToStr(dateEnd, 0);
		CPropQueryContidition* pPropBegin = new CPropQueryContidition();
		lstPropQuery.push_back(pPropBegin);
		pPropBegin->m_strFiledName = FIELD_YDSTUMARK_EXAMDATE;
		pPropBegin->m_uFieldType = VT_DATE;
		pPropBegin->m_uOpType = Q_GE;
		pPropBegin->m_strConVal = strBegin;

		CPropQueryContidition* pPropEnd = new CPropQueryContidition();
		lstPropQuery.push_back(pPropEnd);
		pPropEnd->m_strFiledName = FIELD_YDSTUMARK_EXAMDATE;
		pPropEnd->m_uFieldType = VT_DATE;
		pPropEnd->m_uOpType = Q_LE;
		pPropEnd->m_strConVal = strEnd;
		if (m_bFromStu)
		{
			hr = markRef.GetUnitsByStu(-1, &lstPropQuery,
				&m_lstMarks);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else
		{
			hr = markRef.GetUnitsBySelfProp(-1, &lstPropQuery,
				&m_lstMarks);
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}
	{
		CListAutoClean<CPaperErrorResult> clr4(m_lstErrorResult);
	}
	CPaperAnsysHelper helper;
	hr = helper.ErrorReport(m_lstMarks, m_lstErrorResult);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = InsertResToListCtrl();
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CFormErrorReport::InsertResToListCtrl()
{
	HRESULT hr = E_FAIL;

	m_ErrorReportList.DeleteAllItems();
	for(std::list<CPaperErrorResult*>::const_iterator itr = m_lstErrorResult.begin();
		itr != m_lstErrorResult.end();++itr)
	{
		if ((*itr)->m_lstErrorItemNo.size() <= 0)
		{
			continue;
		}
		CString strIndex(_T(""));
		strIndex.Format(_T("%d"), m_ErrorReportList.GetItemCount()+1);
		int index = m_ErrorReportList.InsertItem(m_ErrorReportList.GetItemCount(), strIndex);
		CString strPaperName;
		hr = (*itr)->m_pPaper->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
		if(FAILED(hr))
		{
			return hr;
		}
		m_ErrorReportList.SetItemText(index,1, strPaperName);
		CString strError;
		
		std::list<UINT>::const_iterator itrNO = (*itr)->m_lstErrorItemNo.begin();
		
		CString strNO;
		strNO.Format(L"%d", *itrNO);
		strError += strNO;
		++itrNO;
		for (; itrNO != (*itr)->m_lstErrorItemNo.end(); ++itrNO)
		{
			CString tmp;
			tmp.Format(L"%d", *itrNO);
			strError += L";";
			strError += tmp;
		}
		m_ErrorReportList.SetItemText(index,2, strError);
		m_ErrorReportList.SetItemData(index,(DWORD_PTR)(*itr));
	}
	m_ErrorReportList.SetSpaceRowColor(TRUE);
	return S_OK;
}

void CFormErrorReport::OnCbnSelchangeCmbOpErrorreportSubject()
{
	// TODO: Add your control notification handler code here
	{
		m_cmbPaper.ResetContent();
		CListAutoClean<CYDObjectRef> clr(m_lstPaperCache);
	}
	int nIndexSubject = m_cmbSubject.GetCurSel();
	if(nIndexSubject <= 0)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	CYDSubjectRef* pSubRef = (CYDSubjectRef*)m_cmbSubject.GetItemData(nIndexSubject);
	ASSERT(pSubRef);
	hr = pSubRef->GetPapers(&m_lstPaperCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstPaperCache.size() > 0)
	{
		int nIndex = 0;
		m_cmbPaper.InsertString(nIndex++, L"所有");
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstPaperCache.begin();
			itr != m_lstPaperCache.end(); ++itr, ++nIndex)
		{
			CString strPaperName;
			(*itr)->GetPropVal(FIELD_YDPAPER_NAME, strPaperName);
			m_cmbPaper.InsertString(nIndex, strPaperName);
			m_cmbPaper.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}
}

void CFormErrorReport::OnBnClickedBtnOpErrorreportOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_ErrorReportList);
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

void CFormErrorReport::OnNMDblclkListOpErrorreportList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	CPaperErrorResult* pErrorResult = (CPaperErrorResult*)m_ErrorReportList.GetItemData(pNMListView->iItem);
	
	ASSERT(pErrorResult != NULL);
	if (pErrorResult == NULL)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	CDlgErrorDetail dlg;
	dlg.m_pResult = pErrorResult;
	dlg.DoModal();
}

//void CFormErrorReport::OnLvnItemchangedListOpErrorreportList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}
