// FormPaper.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormPaperAnsys.h"
#include "../ObjRef/YDPaper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../YDExamObjRef/YDClassRef.h"
#include "../YDExamObjRef/YDStuMark.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjHelper/PaperAnsysHelper.h"
#include "ChoiceAnsys.h"
#include "../UIBase\ListXlsoutput.h"
#include "ShowPSList.h"
#include "ShowPSChart.h"
// CFormPaper

const int cMarkListSerialNo = 0;//题号
const int cMarkListRightNumber = 1;//正确人数
const int cMarkListErrorNumber = 2;// L"错误人数", LVCFMT_LEFT,iWidth*2/8);
const int cMarkListRightRate = 3;//, L"正确率", LVCFMT_LEFT,iWidth*2/8 -2 );
const int cMarkListRightAnswer = 4;//, L"正确答案", LVCFMT_LEFT,iWidth*2/8 -2 );
const int cMarkListAnswerA = 5;//答A的人数
const int cMarkListAnswerB = 6;//答B的人数
const int cMarkListAnswerC = 7;//答C的人数
const int cMarkListAnswerD = 8;//答D的人数
const int cMarkListNotFillAnswer = 9;//未填写

IMPLEMENT_DYNCREATE(CFormPaperAnsys, CYdFormView)

CFormPaperAnsys::CFormPaperAnsys()
	: CYdFormView(CFormPaperAnsys::IDD)

{

}

CFormPaperAnsys::~CFormPaperAnsys()
{
	CListAutoClean<CYDObjectRef> clr(m_lstMarkCache);
	CListAutoClean<CYDObjectRef> clr2(m_lstPaperCache);
	CListAutoClean<CYDObjectRef> clr3(m_lstDeptCache);
	CListAutoClean<CYDObjectRef> clr4(m_lstClsCache);
	CListAutoClean<CYDObjectRef> clr5(m_lstSelClsCache);
	CListAutoClean<CPaperAnsysHelperResult> clr6(m_lstPaperAnsysResCache);
	CListAutoClean<CPropQueryContidition> clr7(m_lstPropQuery);
}

void CFormPaperAnsys::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OP_PAPERANSYS_MARK, m_MarkListCtrl);

	DDX_Control(pDX, IDC_CMB_OP_PAPERANSYS_PAPER, m_cmbPaper);
	DDX_Control(pDX, IDC_CMB_OP_PAPERANSYS_DEPT, m_cmbDept);
	DDX_Control(pDX, IDC_LIST_OP_PAPERANSYS_SEL_CLASS, m_SelClassList);
	DDX_Control(pDX, IDC_LIST_OP_PAPERANSYS_ALL_CLASS, m_AllClassList);
	DDX_Control(pDX, IDC_MSCHART_PAPERANSYS, m_chart);
}

BEGIN_MESSAGE_MAP(CFormPaperAnsys, CYdFormView)

	ON_BN_CLICKED(IDC_BTN_OP_PAPERANSYS_SEARCH, &CFormPaperAnsys::OnBnClickedBtnOPQpaperMarkSearch)
	
	ON_CBN_SELCHANGE(IDC_CMB_OP_PAPERANSYS_DEPT, &CFormPaperAnsys::OnCbnSelchangeCmbOpPaperansysDept)
	ON_BN_CLICKED(IDC_BTN_OP_PAPERANSYS_ADD, &CFormPaperAnsys::OnBnClickedBtnOpPaperansysAdd)
	ON_BN_CLICKED(IDC_BTN_OP_PAPERANSYS_DEL, &CFormPaperAnsys::OnBnClickedBtnOpPaperansysDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OP_PAPERANSYS_MARK, &CFormPaperAnsys::OnNMDblclkListOpPaperansysMark)
	ON_BN_CLICKED(IDC_BTN_OP_PAPERANSYS_OUTPUT, &CFormPaperAnsys::OnBnClickedBtnOpPaperansysOutput)
	ON_BN_CLICKED(IDC_BTN_SHOWCHART, &CFormPaperAnsys::OnBnClickedBtnShowchart)
	ON_BN_CLICKED(IDC_BTN_SHOWLIST, &CFormPaperAnsys::OnBnClickedBtnShowlist)
END_MESSAGE_MAP()


// CFormPaper diagnostics

#ifdef _DEBUG
void CFormPaperAnsys::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormPaperAnsys::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormPaper message handlers
void CFormPaperAnsys::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	//初始化试卷下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	HRESULT hr = CStaticObjHelper::GetObjRef(DB_PAPER, pDB, &m_lstPaperCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstPaperCache.size() > 0)
	{
		int nIndex = 0;
		
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstPaperCache.begin();
			itr != m_lstPaperCache.end(); ++itr, ++nIndex)
		{
			CString strPaperName;
			(*itr)->GetPropVal(FIELD_YDPAPER_NAME, strPaperName);
			m_cmbPaper.InsertString(nIndex, strPaperName);
			m_cmbPaper.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}

	hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT, pDB, &m_lstDeptCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstDeptCache.size() > 0)
	{
		int nIndex = 0;
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstDeptCache.begin();
			itr != m_lstDeptCache.end(); ++itr, ++nIndex)
		{
			CString strDeptName;
			(*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDeptName);
			m_cmbDept.InsertString(nIndex, strDeptName);
			m_cmbDept.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}

	CRect rect1;
	m_AllClassList.GetWindowRect(rect1);
	int iWidth = rect1.Width();
	m_AllClassList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_AllClassList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_AllClassList.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*2/6);
	m_AllClassList.InsertColumn(2, L"名称", LVCFMT_LEFT,iWidth*3/6 - 2);

	m_SelClassList.GetWindowRect(rect1);
	iWidth = rect1.Width();
	m_SelClassList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_SelClassList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_SelClassList.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*2/6);
	m_SelClassList.InsertColumn(2, L"名称", LVCFMT_LEFT,iWidth*3/6 - 2);

	//初始化试卷列表
	m_MarkListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_MarkListCtrl.GetWindowRect(rect);
	iWidth = rect.Width();
	m_MarkListCtrl.InsertColumn(cMarkListSerialNo, L"题号", LVCFMT_LEFT, 40);
	m_MarkListCtrl.InsertColumn(cMarkListRightNumber, L"正确人数", LVCFMT_LEFT,60);
	m_MarkListCtrl.InsertColumn(cMarkListErrorNumber, L"错误人数", LVCFMT_LEFT,60);
	m_MarkListCtrl.InsertColumn(cMarkListRightRate, L"正确率", LVCFMT_LEFT,50 );
	m_MarkListCtrl.InsertColumn(cMarkListRightAnswer, L"正确答案", LVCFMT_LEFT,60 );
	m_MarkListCtrl.InsertColumn(cMarkListAnswerA, L"A", LVCFMT_LEFT,60 );
	m_MarkListCtrl.InsertColumn(cMarkListAnswerB, L"B", LVCFMT_LEFT,60 );
	m_MarkListCtrl.InsertColumn(cMarkListAnswerC, L"C", LVCFMT_LEFT,60);
	m_MarkListCtrl.InsertColumn(cMarkListAnswerD, L"D", LVCFMT_LEFT,60 );
	m_MarkListCtrl.InsertColumn(cMarkListNotFillAnswer, YD_NOT_FILL_ANSWER, LVCFMT_LEFT,60 );
	
	


	//初始化CHART图形
	m_chart.put_ColumnCount(1);
	m_chart.put_TitleText(_T("错题分布图"));
	m_chart.put_ShowLegend(FALSE);
//	VARIANT var;
	m_chart.put_RowCount(0);

	
}

HRESULT CFormPaperAnsys::AddPaperMarkItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	/*CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDANSWER_MARK, &valID);
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
	m_lstPaperCache.push_back(_pref);*/

	return S_OK;
}
HRESULT CFormPaperAnsys::InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion)
{
	HRESULT hr = E_FAIL;
	
	m_MarkListCtrl.DeleteAllItems();
	
	std::list<CYDObjectRef*>::const_iterator itr = lstQuestion.begin();
	for (; itr != lstQuestion.end(); ++itr)
	{
		hr = AddPaperMarkItem(*itr);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

void CFormPaperAnsys::OnBnClickedBtnOPQpaperMarkSearch()
{
	{
		CListAutoClean<CPropQueryContidition> clr1(m_lstPropQuery);
	}
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

	if (m_lstSelClsCache.size() <= 0)
	{
		AfxMessageBox(L"请选择要分析的班级！");
		return;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDStuMark markRef(pDB);
	std::list<CYDObjectRef*> lstMarks;
	CListAutoClean<CYDObjectRef> clrMarks(lstMarks);
	std::list<OBJID> lstClsId;
	int uSize = m_SelClassList.GetItemCount();
	for (int i=0; i<uSize; ++i)
	{
		CYDClassRef* pClassRef = (CYDClassRef*)(m_SelClassList.GetItemData(i));
		ASSERT(pClassRef != NULL);
		UINT objID;
		hr = pClassRef->GetID(&objID);
		if (FAILED(hr))
		{
			return;
		}
		lstClsId.push_back(objID);
	}
	hr = AddPaperPropQueryContidion();
	if(FAILED(hr))
	{
		return;
	}
	hr = markRef.GetUnitsByCls(-1, &lstClsId, &m_lstPropQuery,
		&lstMarks);
	if(FAILED(hr))
	{
		return;
	}
	CYDPaper* pPaper = (CYDPaper*)m_cmbPaper.GetItemData(m_cmbPaper.GetCurSel());
	pPaper->GetID(&paperID);
	m_mapRes.clear();
	CPaperAnsysHelper helper;
	hr = helper.Ansys(paperID, lstMarks, m_mapRes);
	if (FAILED(hr))
	{
		return;
	}
	for(std::map<UINT, CPaperAnsysHelperResult*>::const_iterator itrMap = m_mapRes.begin();
		itrMap != m_mapRes.end();++itrMap)
	{
		m_lstPaperAnsysResCache.push_back(itrMap->second);
	}
	if (lstMarks.size() <= 0)
	{
		m_MarkListCtrl.DeleteAllItems();
		AfxMessageBox(L"没有数据！");
		return;
	}
	m_Total = lstMarks.size();
	InsertList(m_Total, m_mapRes);

	ShowPicture(m_Total, m_mapRes);
}


HRESULT CFormPaperAnsys::ShowPicture(UINT iTotal, 
									 std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	//向chart中添加数据
	m_chart.put_ShowLegend(TRUE);
	int iXcount = mapRes.size();				
	m_chart.put_RowCount(iXcount);       //设置X轴的步数
	for (int i = 1; i <= iXcount; i++)    //为X轴设置值
	{
		std::map<UINT, CPaperAnsysHelperResult*>::const_iterator itrMap = 
			mapRes.find(i);
		if (itrMap != mapRes.end())
		{
			m_chart.put_Row(i);
			CString str;
			str.Format(_T("%d"), i);
			m_chart.put_RowLabel(str);		//设置X每步的标题  这里就是是题号
			CString strErrorCount;

			strErrorCount.Format(_T("%d"), (itrMap->second)->m_iError);
			m_chart.put_Data(strErrorCount);			//设置X的值
		}
		else
		{
			m_chart.put_Row(i);
			CString str;
			str.Format(_T("%d"), i);
			m_chart.put_RowLabel(str);		//设置X每步的标题  这里就是是题号
			CString strErrorCount = L"0";

			m_chart.put_Data(strErrorCount);			//设置X的值
		}
	}	

	return S_OK;
}

HRESULT CFormPaperAnsys::AddPaperPropQueryContidion()
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


BOOL CFormPaperAnsys::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CFormPaperAnsys::OnCbnSelchangeCmbOpPaperansysDept()
{
	CString strSelDept;
	m_cmbDept.GetWindowText(strSelDept);
	m_AllClassList.DeleteAllItems();
	if (strSelDept.GetLength() > 0)
	{
		{
			CListAutoClean<CYDObjectRef> clr(m_lstClsCache);
		}
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
			for (std::list<CYDObjectRef*>::const_iterator itr = m_lstClsCache.begin();
				itr != m_lstClsCache.end(); ++itr)
			{
				AddClassItem(*itr);
			}
		}
	}
}

HRESULT CFormPaperAnsys::AddClassItem(CYDObjectRef* pObjRef)
{
	HRESULT hr = E_FAIL;
	CString strClassID;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_CLASSID, strClassID);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strclassname;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_NAME, strclassname);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_AllClassList.GetItemCount()+1);
	int index = m_AllClassList.InsertItem(m_AllClassList.GetItemCount(), strIndex);
	m_AllClassList.SetItemText(index,1, strClassID);
	m_AllClassList.SetItemText(index,2, strclassname);

	m_AllClassList.SetItemData(index,(DWORD_PTR)pObjRef);

	return S_OK;
}

HRESULT CFormPaperAnsys::AddSelClassItem(CYDObjectRef* pObjRef)
{
	HRESULT hr = E_FAIL;
	CString strClassID;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_CLASSID, strClassID);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strclassname;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_NAME, strclassname);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_SelClassList.GetItemCount()+1);
	int index = m_SelClassList.InsertItem(m_SelClassList.GetItemCount(), strIndex);
	m_SelClassList.SetItemText(index,1, strClassID);
	m_SelClassList.SetItemText(index,2, strclassname);

	m_SelClassList.SetItemData(index,(DWORD_PTR)pObjRef);
	m_lstSelClsCache.push_back(pObjRef);
	return S_OK;
}
void CFormPaperAnsys::OnBnClickedBtnOpPaperansysAdd()
{
	POSITION pos = m_AllClassList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选择要查询的班级！");
	}
	else
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		while (pos)
		{
			int nItem = m_AllClassList.GetNextSelectedItem(pos);
			CYDClassRef* pCls = (CYDClassRef*)m_AllClassList.GetItemData(nItem);
			if (IsClassExist(pCls))
			{
				continue;
			}
			CYDClassRef* pClsCopy = new CYDClassRef(pDB);
			OBJID objID;
			pCls->GetID(&objID);
			pClsCopy->SetID(objID);
			m_setExistCls.insert(objID);
			AddSelClassItem(pClsCopy);
		}
	}
}
BOOL CFormPaperAnsys::IsClassExist(CYDObjectRef* pStu)
{
	OBJID objID;
	pStu->GetID(&objID);
	std::set<OBJID>::iterator itr = m_setExistCls.find(objID);
	if (itr != m_setExistCls.end())
	{
		return TRUE;
	}

	return FALSE;
}
HRESULT CFormPaperAnsys::RemoveSelClass(CYDObjectRef* pStu)
{
	OBJID objID;
	pStu->GetID(&objID);
	std::set<OBJID>::iterator itr = m_setExistCls.find(objID);
	if (itr != m_setExistCls.end())
	{
		m_setExistCls.erase(itr);
	}
	std::list<CYDObjectRef*>::iterator itrLst = m_lstSelClsCache.begin();
	for (; itrLst != m_lstSelClsCache.end(); ++itrLst)
	{
		UINT selObjID;
		(*itrLst)->GetID(&selObjID);
		if (objID == selObjID)
		{
			delete pStu;
			pStu = NULL;
			m_lstSelClsCache.erase(itrLst);
			return S_OK;
		}
	}

	return S_OK;
}

void CFormPaperAnsys::OnBnClickedBtnOpPaperansysDel()
{
	POSITION pos = m_SelClassList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选择删除的查询班级！");
	}
	else
	{
		while (pos)
		{
			int nItem = m_SelClassList.GetNextSelectedItem(pos);
			CYDClassRef* pClassRef = (CYDClassRef*)m_SelClassList.GetItemData(nItem);
			RemoveSelClass(pClassRef);
			m_SelClassList.DeleteItem(nItem);
		}
	}
}


HRESULT CFormPaperAnsys::InsertList(UINT iTotal, std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	m_MarkListCtrl.DeleteAllItems();
	std::map<UINT, CPaperAnsysHelperResult*>::const_iterator itrMap = mapRes.begin();
	for (; itrMap != mapRes.end(); ++itrMap)
	{
		CString strIndex;
		strIndex.Format(L"%d", itrMap->first);
		int index = m_MarkListCtrl.InsertItem(m_MarkListCtrl.GetItemCount(), strIndex);
		float fRight = 0.0000;
		if (iTotal != 0)
		{
			fRight = (itrMap->second)->m_iRight/float(iTotal);
		}
		
		CString strRight;
		strRight.Format(L"%d", (itrMap->second)->m_iRight);
		CString strError;
		strError.Format(L"%d", (itrMap->second)->m_iError);
		CString strRoate;
		float fRes = fRight * 100;
		strRoate.Format(L"%0.2f%c", fRes, 0x25);
		m_MarkListCtrl.SetItemText(index,cMarkListRightNumber, strRight);
		m_MarkListCtrl.SetItemText(index,cMarkListErrorNumber, strError);
		m_MarkListCtrl.SetItemText(index,cMarkListRightRate, strRoate);
		m_MarkListCtrl.SetItemText(index,cMarkListRightAnswer, (itrMap->second)->m_strRightAnswer);
		//各个答案的个数
		UINT uTotalAnswerCount = 0;//答案的总数，要计算百分比
		for(std::map<CString, UINT>::const_iterator itrMapAnswer = (itrMap->second)->m_mapAnswer.begin();
			itrMapAnswer != (itrMap->second)->m_mapAnswer.end();++itrMapAnswer)
		{
			UINT uAnswerCount = itrMapAnswer->second;
			uTotalAnswerCount += uAnswerCount;
		}
		ASSERT(uTotalAnswerCount > 0);
		for(std::map<CString, UINT>::const_iterator itrMapAnswer = (itrMap->second)->m_mapAnswer.begin();
			itrMapAnswer != (itrMap->second)->m_mapAnswer.end();++itrMapAnswer)
		{
			CString strAnswer = itrMapAnswer->first;
			UINT uAnswerCount = itrMapAnswer->second;
			float fPercent = (float)((uAnswerCount /(uTotalAnswerCount * 1.0)) * 100);
			CString strAnswerCount;
			strAnswerCount.Format(_T("%d(%0.2f%%)"),uAnswerCount,fPercent);
			if(strAnswer.CompareNoCase(_T("A")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerA, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(_T("B")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerB, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(_T("C")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerC, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(_T("D")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerD, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(YD_NOT_FILL_ANSWER) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListNotFillAnswer, strAnswerCount);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		m_MarkListCtrl.SetItemData(index, (DWORD_PTR)(itrMap->second));

		

	}
	//修改颜色，每一行颜色都不相同，并且正确答案那一列要求为红色
	for(int iRow = 0; iRow < m_MarkListCtrl.GetItemCount();iRow++)
	{
		if(iRow % 2 == 0)
		{
			for(int iCol = 0; iCol < m_MarkListCtrl.GetHeaderCtrl().GetItemCount();iCol++)
			{
				if(iCol == cMarkListRightAnswer)
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,RGB(255,0,0),ITEM_TEXT_BK_COLOR,FALSE);
				}
				else
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,ITEM_TEXT_COLOR,ITEM_TEXT_BK_COLOR,FALSE);
				}
			}
		}
		else
		{
			for(int iCol = 0; iCol < m_MarkListCtrl.GetHeaderCtrl().GetItemCount();iCol++)
			{
				if(iCol == cMarkListRightAnswer)
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,RGB(255,0,0),ITEM_TEXT_BK_COLOR_DEFAULT,FALSE);
				}
				else
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,ITEM_TEXT_COLOR_DEFAULT,ITEM_TEXT_BK_COLOR_DEFAULT,FALSE);
				}
			}
		}
	}
	m_MarkListCtrl.Invalidate(TRUE);
	return S_OK;
}
void CFormPaperAnsys::OnNMDblclkListOpPaperansysMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	CPaperAnsysHelperResult* pRes = (CPaperAnsysHelperResult*)m_MarkListCtrl.GetItemData(pNMListView->iItem);
	ASSERT(pRes != NULL);
	HRESULT hr = E_FAIL;
	CChoiceAnsys dlg;
	dlg.m_pRes = pRes;
	dlg.DoModal();
	*pResult = 0;
}

void CFormPaperAnsys::OnBnClickedBtnOpPaperansysOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_MarkListCtrl);
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

HRESULT CFormPaperAnsys::ExeSpecial()
{
	HRESULT hr = E_FAIL;
	//显示当前考试收卷成功的试卷分析结果
	//从当前的全局变量的考试结果中来显示
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	OBJID paperID = 0;
	if(pGExamStruct->m_TeacherType == GFROM_TEACHER)
	{
		hr = pGExamStruct->m_pExamDetail->m_pPaper->GetID(&paperID);
		if(FAILED(hr))
		{
			return hr;
		}
		
	}
	else if(pGExamStruct->m_TeacherType == GFROM_INFRARED)
	{
		paperID = pGExamStruct->m_InfraredStruct.m_idPaper;
	}
	for(int i = 0; i < m_cmbPaper.GetCount();i++)
	{
		CYDObjectRef* pCmbPaper = (CYDObjectRef*)m_cmbPaper.GetItemData(i);
		if(pCmbPaper == NULL)
		{
			continue;
		}
		OBJID idCmbPaper = ID_EMPTY;
		hr = pCmbPaper->GetID(&idCmbPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		if(idCmbPaper == paperID)
		{
			m_cmbPaper.SetCurSel(i);
		}
	}
	
	std::list<CString>  lstStrStuAnswer;
	if(pGExamStruct->m_TeacherType == GFROM_TEACHER)
	{
		for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
			itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
		{
			for(std::list<CStudev*>::const_iterator itrStu = (*itr)->m_lstStuDev.begin();
				itrStu != (*itr)->m_lstStuDev.end();++itrStu)
			{
				CString strItrAnswer;
				hr = (*itrStu)->GetAnswer(strItrAnswer);
				if(FAILED(hr))
				{
					return hr;
				}
				lstStrStuAnswer.push_back(strItrAnswer);
			}
		}
	}
	else if(pGExamStruct->m_TeacherType == GFROM_INFRARED)
	{
		for(std::map<CYDEAddrUnit*, CYDStuMark*>::const_iterator itr = pGExamStruct->m_InfraredStruct.m_mapUnitToMark.begin();
			itr != pGExamStruct->m_InfraredStruct.m_mapUnitToMark.end();++itr)
		{
			CYDStuMark* pStuMark = (*itr).second;
			CString strAnswer;
			hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_ANSWER,strAnswer);
			if(FAILED(hr))
			{
				return hr;
			}
			lstStrStuAnswer.push_back(strAnswer);
		}
	}
	
	if (lstStrStuAnswer.size() <= 0)
	{
		AfxMessageBox(L"没有数据！");
		return S_FALSE;
	}
	m_mapRes.clear();
	CPaperAnsysHelper helper;
	hr = helper.Ansys(paperID, lstStrStuAnswer, m_mapRes);
	if (FAILED(hr))
	{
		return hr;
	}
	
	InsertList(lstStrStuAnswer.size(), m_mapRes);

	ShowPicture(lstStrStuAnswer.size(), m_mapRes);
	return S_OK;
}
void CFormPaperAnsys::OnBnClickedBtnShowchart()
{
	CShowPSChart dlg;
	dlg.m_Total = m_Total;
	dlg.m_pMapRes = &m_mapRes;
	dlg.DoModal();
}

void CFormPaperAnsys::OnBnClickedBtnShowlist()
{
	CShowPSList dlg;
	dlg.m_Total = m_Total;
	dlg.m_pMapRes = &m_mapRes;
	dlg.DoModal();
}
