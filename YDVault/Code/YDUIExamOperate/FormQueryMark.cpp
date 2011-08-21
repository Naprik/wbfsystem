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
#include "DlgResDetail.h"
#include "../UIBase\ListXlsoutput.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../base\DataHandler.h"
#include "ShowChartDlg.h"
#include "ShowListDlg.h"
#include "../Base\Xml.h"
#include "InputStudentAnswerRecord.h"
#include "DlgInputStudentAnswerRecord.h"
// CFormQueryMark

CQueryMarkInfo::CQueryMarkInfo()
{
	m_lPaperID = 0;
	m_strStuID = _T("");
	m_strAnswer = _T("");
	m_strStuName = _T("");
	m_strExamID = _T("");
	m_fMark = 0.0;
}

HRESULT CQueryMarkInfo::GetPaper(CYDPaper*& pPaper)
{
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	pPaper = new CYDPaper(pDB);
	pPaper->SetID(m_lPaperID);
	return S_OK;
}

HRESULT	CQueryMarkInfo::GetPaperName(CString &_strPaperName)
{
	HRESULT hr = E_FAIL;
	CYDPaper* pPaper = NULL;
	hr = GetPaper(pPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDPaper> clr(pPaper);
	hr = pPaper->GetPropVal(FIELD_YDPAPER_NAME,_strPaperName);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

IMPLEMENT_DYNCREATE(CFormQueryMark, CYdFormView)

CFormQueryMark::CFormQueryMark()
	: CYdFormView(CFormQueryMark::IDD)
	, m_utotalNumber(0)
	, m_uPageSize(0)
	, m_uPage(0)

	, m_strAverageMark(_T(""))
{

}

CFormQueryMark::~CFormQueryMark()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstPaperCache);
	CListAutoClean<CYDObjectRef> clr2(m_lstUnitCache);
	CListAutoClean<CPropQueryContidition> clr3(m_lstPropQuery);
	CListAutoClean<CYDObjectRef> clr4(m_lstSubjectCache);
	CListAutoClean<CQueryMarkInfo> clr5(m_lstQueryMarkInfo);
}

void CFormQueryMark::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_DM_QMARK_MARKS, m_UnitList);
	DDX_Text(pDX, IDC_EDIT_DM_QMARK_TOTAL, m_utotalNumber);
	DDX_Text(pDX, IDC_EDIT_DM_QMARK_PAGENUM, m_uPageSize);
	DDX_Control(pDX, IDC_DM_QMARK_TAB, m_Tab);
	DDX_Control(pDX, IDC_CMB_DM_QMARK_PAPER, m_cmbPaper);
	DDX_Control(pDX, IDC_MSCHART_MARKQUERY, m_chart);
	DDX_Control(pDX, IDC_CMB_DM_QMARK_SUBJECT, m_cmbSubject);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_MARK, m_strAverageMark);
	DDX_Control(pDX, IDC_DATE_OP_QUERY_BEGIN, m_begin);
	DDX_Control(pDX, IDC_DATE_OP_QUERY_END, m_end);
}

BEGIN_MESSAGE_MAP(CFormQueryMark, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_SEARCH, &CFormQueryMark::OnBnClickedBtnDmQMarkSearch)

	ON_BN_CLICKED(IDC_BTN_DM_QMARK_BEGIN, &CFormQueryMark::OnBnClickedBtnDmQMarkBegin)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_PREV, &CFormQueryMark::OnBnClickedBtnDmQMarkPrev)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_NEXT, &CFormQueryMark::OnBnClickedBtnDmQMarkNext)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_END, &CFormQueryMark::OnBnClickedBtnDmQMarkEnd)
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_DM_QMARK_TAB, &CFormQueryMark::OnTcnSelchangeDmQmarkTab)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DM_QMARK_MARKS, &CFormQueryMark::OnNMDblclkListDmQmarkMarks)
	ON_CBN_SELCHANGE(IDC_CMB_DM_QMARK_SUBJECT, &CFormQueryMark::OnCbnSelchangeCmbDmQmarkSubject)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DM_QMARK_MARKS, &CFormQueryMark::OnNMRClickListDmQmarkMarks)
	
	ON_COMMAND(ID_MI_STUMARK_ANA, &CFormQueryMark::OnMIStuMarkAna)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_OUTPUT, &CFormQueryMark::OnBnClickedBtnDmQmarkOutput)
	ON_BN_CLICKED(IDC_BTN_SHOWCHART, &CFormQueryMark::OnBnClickedBtnShowchart)
	ON_BN_CLICKED(IDC_BTN_SHOWLIST, &CFormQueryMark::OnBnClickedBtnShowlist)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_FROM_XML, &CFormQueryMark::OnBnClickedButtonInputFromXml)
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
	CString strSelSubject;
	m_cmbSubject.GetWindowText(strSelSubject);
	OBJID paperID = ID_EMPTY;
	if (strSelSubject.GetLength() <= 0)
	{
		AfxMessageBox(L"��ѡ���Ŀ��");
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
			AfxMessageBox(L"��ѡ��Ҫ��ѯ�İ༶��");
			return;
		}
	}
	else if (m_Tab.GetCurSel() == 0)
	{
		m_PageStudent.CreatePropQueryContidion(m_lstPropQuery, &m_bFromStu);
		if (m_lstPropQuery.size() <= 1)
		{
			AfxMessageBox(L"�������ѯ������");
			return;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	

	m_uPage = 0;
	UpdateData();
	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	UpdateData(FALSE);
}

void CFormQueryMark::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	TCITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = L"���˳ɼ���ѯ";

	m_Tab.InsertItem (0,&item);
	item.pszText = L"�༶�ɼ���ѯ";
	m_Tab.InsertItem (1,&item);

	m_PageClass.Create(IDD_YDUIEXAM_DATA_MAINTAIN_DLG_QMARKBYCLASS_SHEET, &m_Tab); 
	m_PageStudent.Create(IDD_YDUIEXAM_DATA_MAINTAIN_DLG_QMARKBYSTU_SHEET,&m_Tab); 
	
	//���IDC_TABTEST�ͻ�����С 
	CRect rs; 
	m_Tab.GetClientRect(&rs);
	//�����ӶԻ����ڸ������е�λ�� 
	rs.top+=21;  
	rs.bottom-=2;  
	rs.left+=1;  
	rs.right-=2;  
	//�����ӶԻ���ߴ粢�ƶ���ָ��λ�� 
	m_PageClass.MoveWindow(&rs); 
	m_PageStudent.MoveWindow(&rs); 
	//�ֱ��������غ���ʾ 
	m_PageStudent.ShowWindow(SW_SHOW); 
	m_PageClass.ShowWindow(SW_HIDE); 
	//����Ĭ�ϵ�ѡ� 
	m_Tab.SetCurSel(0); 

	HRESULT hr = E_FAIL;
	//��ʼ����Ŀ������
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT, pDB, &m_lstSubjectCache);
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
			CString strSubjectName;
			(*itr)->GetPropVal(FIELD_YDSUBJECT_NAME, strSubjectName);
			m_cmbSubject.InsertString(nIndex, strSubjectName);
			m_cmbSubject.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}
	
	m_UnitList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_UnitList.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_UnitList.InsertColumn(0, L"���", LVCFMT_LEFT, iWidth/10);
	m_UnitList.InsertColumn(1, L"ѧ��", LVCFMT_LEFT,iWidth*2/10 -20);
	m_UnitList.InsertColumn(2, L"����", LVCFMT_LEFT,iWidth/10);
	m_UnitList.InsertColumn(3, L"����", LVCFMT_LEFT,iWidth*1/10 -2);
	m_UnitList.InsertColumn(4, L"׼��֤��", LVCFMT_LEFT,iWidth*2/10);
	m_UnitList.InsertColumn(5, L"�Ծ���", LVCFMT_LEFT,iWidth*2/10);
	m_UnitList.InsertColumn(6, L"����ʱ��", LVCFMT_LEFT,iWidth*2/10 +20);
	m_UnitList.InsertColumn(7, L"ѧ����", LVCFMT_LEFT,iWidth*2/10 +20);

	//��ʼ��chart
	m_chart.put_ColumnCount(1);
//	m_chart.put_TitleText(_T("����ֲ�ͼ"));
	m_chart.put_ShowLegend(FALSE);
	m_chart.put_RowCount(0);
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
	CYDStuMark* pStuMark = dynamic_cast<CYDStuMark*>(_pref);
	CYdStudentRef* pStudent = NULL;
	hr = pStuMark->GetStudent(pStudent);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYdStudentRef> clr1(pStudent);
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
	hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_EXAMID, strExamId);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strMark;
	hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_MARK, strMark);
	if(FAILED(hr))
	{
		return hr;
	}
	CYDPaper* pPaper = NULL;
	hr = pStuMark->GetPaper(pPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDPaper> clr2(pPaper);
	CString strPaperName;
	hr = pPaper->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_UnitList.GetItemCount()+1);
	int index = m_UnitList.InsertItem(m_UnitList.GetItemCount(), strIndex);
	m_UnitList.SetItemText(index,1, strID);
	m_UnitList.SetItemText(index,2, strName);
	m_UnitList.SetItemText(index,3, strMark);
	m_UnitList.SetItemText(index,4, strExamId);

	m_UnitList.SetItemText(index,5, strPaperName);
	
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
	m_chart.put_ColumnCount(1);
	m_chart.put_TitleText(_T("�ɼ��ֲ�ͼ"));
	m_chart.put_ShowLegend(FALSE);
	//	VARIANT var;
	m_chart.put_RowCount(0);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDStuMark markRef(pDB);
	if (m_Tab.GetCurSel() == 1)
	{
		if(_bFirstQuery)
		{
			//��һ�β�ѯ��Ҫ��������
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
		CListAutoClean<CYDObjectRef> clr(lstPapers);
		
		hr = markRef.GetUnitsByCls(m_uPage, &m_lstSelClass, &m_lstPropQuery,
									&lstPapers);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = TransferMarkToQueryInfo(lstPapers);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = InsertlstToListCtrl();
		if(FAILED(hr))
		{
			return hr;
		}
		hr = ComputeAverageMark();
		if(FAILED(hr))
		{
			return hr;
		}
		DrawStuMarkCountSub(m_chart);
	}
	else if (m_Tab.GetCurSel() == 0)
	{
		if(_bFirstQuery)
		{
			//��һ�β�ѯ��Ҫ��������
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
		CListAutoClean<CYDObjectRef> clr(lstPapers);
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
		hr = TransferMarkToQueryInfo(lstPapers);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = InsertlstToListCtrl();
		if(FAILED(hr))
		{
			return hr;
		}
		hr = ComputeAverageMark();
		if(FAILED(hr))
		{
			return hr;
		}
		DrawStuMarkCountSub(m_chart);
	}
	else
	{
		ASSERT(FALSE);
	}
	
	//��ť�Ŀɷ�ʹ��
	if(m_uPage == 0)
	{
		//��ǰ�ǵ�һҳ
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
		//�������һҳ��
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

HRESULT CFormQueryMark::DrawStuMarkCountSub(CMschart_paperansys& chart)
{
	HRESULT hr = E_FAIL;
	m_bShowFromSearch = TRUE;
	chart.put_ColumnLabel(_T("����/����"));
	int i90 = 0;
	int i80 = 0;
	int i70 = 0;
	int i60 = 0;
	int i50 = 0;
	if(m_lstQueryMarkInfo.size() <= 0)
	{
		return S_OK;
	}
	//�ж��Ƿ��ж���Ծ�,����ж�ݳɼ�����100�ּ��㣬�ֶ�
	//�����Ծ�ķ���ʵ�ʷֶ�
	BOOL bHasMulityPaper = FALSE;
	{
		BOOL bFirst = TRUE;
		long lPrevPaperID = 0;
		for(std::list<CQueryMarkInfo*>::const_iterator itr = m_lstQueryMarkInfo.begin();
			itr != m_lstQueryMarkInfo.end(); ++itr)
		{
			if(bFirst)
			{
				lPrevPaperID = (*itr)->m_lPaperID;
				bFirst = FALSE;
			}
			else
			{
				if(lPrevPaperID != (*itr)->m_lPaperID)
				{
					bHasMulityPaper = TRUE;
					break;
				}
				else
				{
					lPrevPaperID = (*itr)->m_lPaperID;
				}
			}
		}
	}
	int iSpace90 = 90;
	int iSpace80 = 80;
	int iSpace70 = 70;
	int iSpace60 = 60;
	int iSpace50 = 0;
	if(!bHasMulityPaper)
	{
		//ֻ��һ���Ծ�ʱ��ͳ��ʦ�����Ծ�ķ�ֵ���ܷ�Ϊ5��
		//�Ծ��ֵ*0.6һ��Ϊ������
		//ʣ�µķ��Ķ�
		long lPaperMarkVale = 100;//�Ծ��ֵ
		CQueryMarkInfo* pMarkInfo = (CQueryMarkInfo*)(*m_lstQueryMarkInfo.begin());
		CYDPaper* pPaper = NULL;
		hr = pMarkInfo->GetPaper(pPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDPaper> clr(pPaper);
		hr = pPaper->GetPaperMarkVal(lPaperMarkVale);
		if(FAILED(hr))
		{
			return hr;
		}
		iSpace90 = (int)(lPaperMarkVale * 0.9);
		iSpace80 = (int)(lPaperMarkVale * 0.8);
		iSpace70 = (int)(lPaperMarkVale * 0.7);
		iSpace60 = (int)(lPaperMarkVale * 0.6);
	}


	for(std::list<CQueryMarkInfo*>::const_iterator itr = m_lstQueryMarkInfo.begin();
		itr != m_lstQueryMarkInfo.end(); ++itr)
	{
		float fMark = (*itr)->m_fMark;
		int iMark = (int)fMark;
		if (iMark >= iSpace90)
		{
			i90++;
		}
		else if (iMark >= iSpace80)
		{
			i80++;
		}
		else if (iMark >= iSpace70)
		{
			i70++;
		}
		else if (iMark >= iSpace60)
		{
			i60++;
		}
		else
		{
			i50++;
		}

	}

	//��chart���������
	chart.put_ShowLegend(TRUE);
	int iXcount = 5;				
	chart.put_RowCount(iXcount);       //����X��Ĳ���
	for (int i = 1; i <= iXcount; i++)    //ΪX������ֵ
	{
		
		chart.put_Row(i);
		CString str;
		int iCount = 0;
		if (i == 1)
		{
			str.Format(_T("%d����"),iSpace90);
			iCount = i90;
		}
		else if (2 == i)
		{
			str.Format(_T("%d-%d"),iSpace80,iSpace90-1);
			//str = L"80-89";
			iCount = i80;
		}
		else if (3 == i)
		{
			str.Format(_T("%d-%d"),iSpace70,iSpace80-1);
			//str = L"70-79";
			iCount = i70;
		}
		else if (4 == i)
		{
			str.Format(_T("%d-%d"),iSpace60,iSpace70-1);
			//str = L"60-69";
			iCount = i60;
		}
		else
		{
			str.Format(_T("%d����"),iSpace60);
			//str = L"60����";
			iCount = i50;
		}
		CString strErrorCount;
		strErrorCount.Format(_T("%d"), iCount);
		str += _T("(");
		str += strErrorCount;
		str += _T(")");
		chart.put_RowLabel(str);		//����Xÿ���ı���  ������������
		chart.put_Data(strErrorCount);			//����X��ֵ
	}	
	return S_OK;
}

HRESULT CFormQueryMark::InsertlstToListCtrl()
{
	HRESULT hr = E_FAIL;

	m_UnitList.DeleteAllItems();
	for(std::list<CQueryMarkInfo*>::const_iterator itr = m_lstQueryMarkInfo.begin();
		itr != m_lstQueryMarkInfo.end();++itr)
	{
		CString strIndex(_T(""));
		strIndex.Format(_T("%d"), m_UnitList.GetItemCount()+1);
		int index = m_UnitList.InsertItem(m_UnitList.GetItemCount(), strIndex);
		m_UnitList.SetItemText(index,1, (*itr)->m_strStuID);
		m_UnitList.SetItemText(index,2, (*itr)->m_strStuName);
		CString strMark;
		strMark.Format(_T("%.0f"),(*itr)->m_fMark);
		m_UnitList.SetItemText(index,3, strMark);
		m_UnitList.SetItemText(index,4, (*itr)->m_strExamID);
		CString strPaperName;
		hr = (*itr)->GetPaperName(strPaperName);
		if(FAILED(hr))
		{
			return hr;
		}
		m_UnitList.SetItemText(index,5, strPaperName);
		
		m_UnitList.SetItemText(index,6, (*itr)->m_strExamTime);
		m_UnitList.SetItemText(index,7, (*itr)->m_strAnswer);
		m_UnitList.SetItemData(index, (DWORD_PTR)(*itr));
	}
	m_UnitList.SetSpaceRowColor(TRUE);
	return S_OK;
}

HRESULT CFormQueryMark::ComputeAverageMark()//����ƽ���ɼ�
{
	HRESULT hr = E_FAIL;
	m_strAverageMark = _T("");
	UINT uCount = m_lstQueryMarkInfo.size();
	if(uCount <=0)
	{
		return S_OK;
	}
	float fAllMark = 0.0;
	for(std::list<CQueryMarkInfo*>::const_iterator itr = m_lstQueryMarkInfo.begin();
		itr != m_lstQueryMarkInfo.end();++itr)
	{
		fAllMark += (*itr)->m_fMark;
	}
	float fAverageMark = fAllMark / uCount;
	m_strAverageMark.Format(_T("%.2f"),fAverageMark);
	return S_OK;
}


HRESULT CFormQueryMark::AddPaperPropQueryContidion()
{
	HRESULT hr = E_FAIL;
	CString strSelSubject;
	m_cmbSubject.GetWindowText(strSelSubject);
	if(strSelSubject.GetLength() <= 0)
	{
		return S_FALSE;
	}
	m_lstPaperConditon.clear();
	int iPaperIndex = m_cmbPaper.GetCurSel();
	CString strPaperID;
	if(iPaperIndex <= 0)
	{
		//��ѯ��ǰ��Ŀ�����е��Ծ�
		CYDSubjectRef* pSub = (CYDSubjectRef*)m_cmbSubject.GetItemData(m_cmbSubject.GetCurSel());
		ASSERT(pSub);
		std::list<CYDObjectRef*> lstPaper;
		CListAutoClean<CYDObjectRef> clr(lstPaper);
		hr = pSub->GetPapers(&lstPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		BOOL bFirst = TRUE;
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
			if(bFirst)
			{
				bFirst = FALSE;
			}
			else
			{
				strPaperID += _T(",");
			}
			strPaperID += strID;
		}
		if(strPaperID.IsEmpty())
		{
			//˵����ǰ��Ŀ��û���κ��Ծ�
			strPaperID = _T("-1");
		}
		else
		{
			//�鵱ǰ��Ŀ�µ������Ծ����Ծ���䵽m_lstPaperConditon��
			for(int i = 0; i < m_cmbPaper.GetCount();i++)
			{
				CYDObjectRef* pPaper = (CYDObjectRef*)m_cmbPaper.GetItemData(i);
				if(pPaper != NULL)
				{
					m_lstPaperConditon.push_back(pPaper);
				}
			}
		}
	}
	else 
	{
		//��ѯ����һ���Ծ�
		CYDPaper* pPaper = (CYDPaper*)m_cmbPaper.GetItemData(iPaperIndex);
		ASSERT(pPaper);
		OBJID paperID = ID_EMPTY;
		hr = pPaper->GetID(&paperID);
		if(FAILED(hr))
		{
			return hr;
		}
		strPaperID.Format(_T("%d"), paperID);
		m_lstPaperConditon.push_back(pPaper);
	}
	CPropQueryContidition* pPropQuery = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropQuery);
	pPropQuery->m_strFiledName = FIELD_YDSTUMARK_PAPERID;
	pPropQuery->m_uFieldType = VT_I4;
	pPropQuery->m_uOpType = Q_IN;
	pPropQuery->m_strConVal = strPaperID;

	COleDateTime dateBegin, dateEnd;
	m_begin.GetTime(dateBegin);
	dateBegin.SetDateTime(dateBegin.GetYear(), dateBegin.GetMonth(),dateBegin.GetDay(),
			0, 0, 0);
	m_end.GetTime(dateEnd);
	dateEnd.SetDateTime(dateEnd.GetYear(), dateEnd.GetMonth(),dateEnd.GetDay(),
		23, 59, 59);
	CString strBegin = CDataHandler::DateTimeToStr(dateBegin, 0);
	CString strEnd = CDataHandler::DateTimeToStr(dateEnd, 0);
	CPropQueryContidition* pPropBegin = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropBegin);
	pPropBegin->m_strFiledName = FIELD_YDSTUMARK_EXAMDATE;
	pPropBegin->m_uFieldType = VT_DATE;
	pPropBegin->m_uOpType = Q_GE;
	pPropBegin->m_strConVal = strBegin;

	CPropQueryContidition* pPropEnd = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropEnd);
	pPropEnd->m_strFiledName = FIELD_YDSTUMARK_EXAMDATE;
	pPropEnd->m_uFieldType = VT_DATE;
	pPropEnd->m_uOpType = Q_LE;
	pPropEnd->m_strConVal = strEnd;
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


void CFormQueryMark::OnNMDblclkListDmQmarkMarks(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	CQueryMarkInfo* pQueryMarkInfo = (CQueryMarkInfo*)m_UnitList.GetItemData(pNMListView->iItem);
	ASSERT(pQueryMarkInfo != NULL);
	HRESULT hr = E_FAIL;
	CDlgResDetail dlg;
	dlg.m_ResultType = FROM_DB;
	dlg.m_pQueryMarkInfo = pQueryMarkInfo;
	m_cmbSubject.GetWindowText(dlg.m_strSubject); 
	dlg.DoModal();

	*pResult = 0;
}

void CFormQueryMark::OnCbnSelchangeCmbDmQmarkSubject()
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
		m_cmbPaper.InsertString(nIndex++, L"����");
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

void CFormQueryMark::OnNMRClickListDmQmarkMarks(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	*pResult = 0;
	if(pNMItemActivate->iItem < 0)
	{
		return;
	}
	CPoint pt;
	GetCursorPos(&pt);
	CPoint point(pt);
	m_UnitList.ScreenToClient(&pt);
	UINT uFlags;

	int nItem = m_UnitList.HitTest(pt, &uFlags);
	if (nItem != -1)
	{
		m_UnitList.SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 
			LVIS_SELECTED, 0);

		CMenu* pSubMenu = NULL;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_LIST_MARK_ANA);
		pSubMenu = menu.GetSubMenu(0);
		CPoint pt;
		GetCursorPos(&pt);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
		//((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	}
}

void CFormQueryMark::OnMIStuMarkAna()
{
	POSITION pos = m_UnitList.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("�����б���ѡ��һ����¼���з�����"));
		return;
	}
	int nItem = m_UnitList.GetNextSelectedItem(pos);
	if(nItem < 0)
	{
		return ;
	}
	CQueryMarkInfo* pQueryMarkInfo = (CQueryMarkInfo*)m_UnitList.GetItemData(nItem);
	ASSERT(pQueryMarkInfo != NULL);
	HRESULT hr = E_FAIL;
	m_lstSelQueryMarkInfo.clear();
	hr = GetSameQueryMarkInfoByStuID(pQueryMarkInfo->m_strStuID,&m_lstSelQueryMarkInfo);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	ASSERT(m_lstSelQueryMarkInfo.size() > 0);
	hr = DrawStuMarkByPaper(m_lstSelQueryMarkInfo);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

HRESULT CFormQueryMark::GetSameQueryMarkInfoByStuID(CString stuID,std::list<CQueryMarkInfo*> *_lstQueryMarkInfo)
{
	HRESULT hr = E_FAIL;
	for(int i = 0; i <m_UnitList.GetItemCount();i++)
	{
		CQueryMarkInfo* pQueryMarkInfo = (CQueryMarkInfo*)m_UnitList.GetItemData(i);
		ASSERT(pQueryMarkInfo != NULL);
		if(stuID.CompareNoCase(pQueryMarkInfo->m_strStuID) == 0)
		{
			_lstQueryMarkInfo->push_back(pQueryMarkInfo);
		}
	}
	return S_OK;
}

HRESULT CFormQueryMark::DrawStuMarkByPaper(std::list<CQueryMarkInfo*> &_lstQueryMarkInfo)
{
	HRESULT hr = E_FAIL;

	m_bShowFromSearch = FALSE;
	//��chart���������
	m_chart.put_ColumnLabel(_T("�Ծ���/����"));
	//m_chart.put_TitleText()
	m_chart.put_ShowLegend(TRUE);
	int iXcount = _lstQueryMarkInfo.size();				
	m_chart.put_RowCount(iXcount);
	int iRow = 1;
	for (std::list<CQueryMarkInfo*>::const_iterator itr = _lstQueryMarkInfo.begin();
		itr != _lstQueryMarkInfo.end(); ++itr,iRow++)
	{
		CString strPaperName;
		hr = (*itr)->GetPaperName(strPaperName);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strMark;
		strMark.Format(_T("%f"),(*itr)->m_fMark);
	
		m_chart.put_Row(iRow);
		m_chart.put_RowLabel(strPaperName);		//����Xÿ���ı���  ��������Ծ���
		m_chart.put_Data(strMark);			//����X��ֵ,���ǳɼ�

	}
	return S_OK;
}

void CFormQueryMark::OnBnClickedBtnDmQmarkOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_UnitList);
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("ƽ������"),m_strAverageMark));
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstPaperConditon.begin();
		itr != m_lstPaperConditon.end();++itr)
	{
		CString strPaperName;
		hr = (*itr)->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("�Ծ�����"),strPaperName));
		CYDPaper* pPaper = (CYDPaper*)(*itr);
		ASSERT(pPaper);
		CString strStdAnswer;
		hr = pPaper->GetStdAnswer(strStdAnswer);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("��׼��"),strStdAnswer));
	}
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
	AfxMessageBox(_T("���XLS�ɹ���"));
}

HRESULT CFormQueryMark::ExeSpecial()
{
	HRESULT hr = E_FAIL;
	//��ȫ�ֱ����л�ȡ�Ծ�ʹ���Ϣ
	{
		CListAutoClean<CQueryMarkInfo> clr(m_lstQueryMarkInfo);
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	OBJID PaperID = ID_EMPTY;
	if(pGExamStruct->m_TeacherType == GFROM_TEACHER)
	{
		hr = pGExamStruct->m_pExamDetail->m_pPaper->GetID(&PaperID);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else if(pGExamStruct->m_TeacherType == GFROM_INFRARED)
	{
		PaperID = pGExamStruct->m_InfraredStruct.m_idPaper;
	}

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	if(pGExamStruct->m_TeacherType == GFROM_TEACHER)
	{
		for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
			itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
		{
			for(std::list<CStudev*>::const_iterator itrStu = (*itr)->m_lstStuDev.begin();
				itrStu != (*itr)->m_lstStuDev.end();++itrStu)
			{
				int iItrRolling = -1;
				hr = (*itrStu)->GetRolling(iItrRolling);
				if(FAILED(hr))
				{
					return hr;
				}
				if(iItrRolling != 1)
				{
					//��ǰû���վ�ɹ�����Ҫ��ʾ
					continue;
				}
				CQueryMarkInfo* pQueryMarkInfo = new CQueryMarkInfo;
				m_lstQueryMarkInfo.push_back(pQueryMarkInfo);
				CYDEAddrUnit* pUnit = NULL;
				CString strItrMac;
				hr = (*itrStu)->GetMac(strItrMac);
				if(FAILED(hr))
				{
					return hr;
				}
				hr = GetUnitByMac(strItrMac,pUnit);
				if(FAILED(hr))
				{
					return hr;
				}
				ASSERT(pUnit);
				pQueryMarkInfo->m_lPaperID = PaperID;
				CString strItrAnswer;
				hr = (*itrStu)->GetAnswer(strItrAnswer);
				if(FAILED(hr))
				{
					return hr;
				}
				pQueryMarkInfo->m_strAnswer = strItrAnswer;
				CYdStudentRef* pStundent = NULL;
				hr = pUnit->GetStudent(pStundent);
				if(FAILED(hr))
				{
					return hr;
				}
				ASSERT(pStundent);
				CPtrAutoClean<CYdStudentRef> clr(pStundent);
				hr = pStundent->GetPropVal(FIELD_YDSTU_ID,pQueryMarkInfo->m_strStuID);
				if(FAILED(hr))
				{
					return hr;
				}
				hr = pStundent->GetPropVal(FIELD_YDSTU_NAME,pQueryMarkInfo->m_strStuName);
				if(FAILED(hr))
				{
					return hr;
				}
				//׼��֤��
				hr = pUnit->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,pQueryMarkInfo->m_strExamID);
				if(FAILED(hr))
				{
					return hr;
				}
				//�������
				CYDStuMark YDStuMark(pDB);
				CComVariant valPaperID(PaperID);
				hr = YDStuMark.SetPropVal(FIELD_YDSTUMARK_PAPERID,&valPaperID);
				if(FAILED(hr))
				{
					return hr;
				}
				CComVariant valAnswer(strItrAnswer);
				hr = YDStuMark.SetPropVal(FIELD_YDSTUMARK_ANSWER,&valAnswer);
				if(FAILED(hr))
				{
					return hr;
				}
				double dbMark = 0;
				hr = YDStuMark.CalMarkByString(&dbMark);
				if(FAILED(hr))
				{
					return hr;
				}
				pQueryMarkInfo->m_fMark = (float)dbMark;
				pQueryMarkInfo->m_strExamTime = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeStart,0);
			}
		}
	}
	else if(pGExamStruct->m_TeacherType == GFROM_INFRARED)
	{
		for(std::map<CYDEAddrUnit*, CYDStuMark*>::const_iterator itr = pGExamStruct->m_InfraredStruct.m_mapUnitToMark.begin();
				itr != pGExamStruct->m_InfraredStruct.m_mapUnitToMark.end();++itr)
		{
			CQueryMarkInfo* pQueryMarkInfo = new CQueryMarkInfo;
			m_lstQueryMarkInfo.push_back(pQueryMarkInfo);
			CYDEAddrUnit* pUnit = (*itr).first;
			ASSERT(pUnit);
			CString strItrMac;
			hr = pUnit->GetDeviceMac(strItrMac);
			if(FAILED(hr))
			{
				return hr;
			}
			pQueryMarkInfo->m_lPaperID = PaperID;
			CYDStuMark* pStuMark = (*itr).second;
			ASSERT(pStuMark);
			CString strItrAnswer;
			hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_ANSWER,strItrAnswer);
			if(FAILED(hr))
			{
				return hr;
			}
			pQueryMarkInfo->m_strAnswer = strItrAnswer;
			CYdStudentRef* pStundent = NULL;
			hr = pUnit->GetStudent(pStundent);
			if(FAILED(hr))
			{
				return hr;
			}
			ASSERT(pStundent);
			CPtrAutoClean<CYdStudentRef> clr(pStundent);
			hr = pStundent->GetPropVal(FIELD_YDSTU_ID,pQueryMarkInfo->m_strStuID);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pStundent->GetPropVal(FIELD_YDSTU_NAME,pQueryMarkInfo->m_strStuName);
			if(FAILED(hr))
			{
				return hr;
			}
			//׼��֤��
			hr = pUnit->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,pQueryMarkInfo->m_strExamID);
			if(FAILED(hr))
			{
				return hr;
			}
			//�������
			double dbMark = 0;
			hr = pStuMark->CalMarkByString(&dbMark);
			if(FAILED(hr))
			{
				return hr;
			}
			pQueryMarkInfo->m_fMark = (float)dbMark;
			pQueryMarkInfo->m_strExamTime = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeStart,0);
		}
	}

	UpdateData();
	{
		//��ʾ���Կ�Ŀ���Ծ�
		OBJID gSubjectID = ID_EMPTY;
		if(pGExamStruct->m_TeacherType == GFROM_TEACHER)
		{
			hr = pGExamStruct->m_pExamDetail->m_pSubject->GetID(&gSubjectID);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else if(pGExamStruct->m_TeacherType == GFROM_INFRARED)
		{
			gSubjectID = pGExamStruct->m_InfraredStruct.m_idSubject;
		}
		for(int i = 0; i < m_cmbSubject.GetCount();i++)
		{
			CYDSubjectRef* pCmbSubRef = (CYDSubjectRef*)m_cmbSubject.GetItemData(i);
			if(pCmbSubRef == NULL)
			{
				continue;
			}
			OBJID idCmbSub = ID_EMPTY;
			hr = pCmbSubRef->GetID(&idCmbSub);
			if(FAILED(hr))
			{
				return hr;
			}
			if(idCmbSub == gSubjectID)
			{
				m_cmbSubject.SetCurSel(i);
				OnCbnSelchangeCmbDmQmarkSubject();
			}
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
			if(idCmbPaper == PaperID)
			{
				m_cmbPaper.SetCurSel(i);
			}
		}
	}
	hr = InsertlstToListCtrl();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = ComputeAverageMark();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = DrawStuMarkCountSub(m_chart);
	if(FAILED(hr))
	{
		return hr;
	}
	UpdateData(FALSE);
	return S_OK;
}

HRESULT CFormQueryMark::GetUnitByMac(CString _strMac,CYDEAddrUnit* &_pUnit )
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	for (std::list<CYDObjectRef*>::const_iterator itrUnit = pGExamStruct->m_pExamDetail->m_lstUnit.begin();
		itrUnit != pGExamStruct->m_pExamDetail->m_lstUnit.end();++itrUnit)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)(*itrUnit);
		ASSERT(pUnit);
		CString strMac;
		hr = pUnit->GetDeviceMac(strMac);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strMac.CompareNoCase(_strMac) == 0)
		{
			_pUnit = pUnit;
			return S_OK;
		}
	}
	return S_FALSE;
}
HRESULT CFormQueryMark::TransferMarkToQueryInfo(std::list<CYDObjectRef*> &_lstStuMark)
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CQueryMarkInfo> clr(m_lstQueryMarkInfo);
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = _lstStuMark.begin();
		itr != _lstStuMark.end();++itr)
	{
		CYDStuMark* pStuMark = (CYDStuMark*)(*itr);
		ASSERT(pStuMark);
		CQueryMarkInfo* pMarkInfo = new CQueryMarkInfo();
		m_lstQueryMarkInfo.push_back(pMarkInfo);
		CYdStudentRef* pStudent = NULL;
		hr = pStuMark->GetStudent(pStudent);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYdStudentRef> clr1(pStudent);
		hr = pStudent->GetPropVal(FIELD_YDSTU_ID, pMarkInfo->m_strStuID);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = pStudent->GetPropVal(FIELD_YDSTU_NAME, pMarkInfo->m_strStuName);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_EXAMID, pMarkInfo->m_strExamID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_ANSWER,pMarkInfo->m_strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valMark;
		hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_MARK, &valMark);
		if(FAILED(hr))
		{
			return hr;
		}
		pMarkInfo->m_fMark = (float)valMark;
		hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_PAPERID, &pMarkInfo->m_lPaperID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_EXAMDATE,pMarkInfo->m_strExamTime);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
void CFormQueryMark::OnBnClickedBtnShowchart()
{
	CShowChartDlg dlg(m_bShowFromSearch);
	if (m_bShowFromSearch)
	{
		dlg.m_pLstQueryMarkInfo = &m_lstQueryMarkInfo;
	}
	else
	{
		dlg.m_pLstQueryMarkInfo = &m_lstSelQueryMarkInfo;
	}
	dlg.DoModal();
}

void CFormQueryMark::OnBnClickedBtnShowlist()
{
	CShowListDlg dlg;
	dlg.m_pLstQueryMarkInfo = &m_lstQueryMarkInfo;
	dlg.DoModal();
}


void CFormQueryMark::OnBnClickedButtonInputFromXml()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Xml�ļ�(*.xml)|*.xml|�����ļ�(*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	CString strFileName = dlg.GetPathName();
	CPdemWait	wait(_T("���Ժ����ڶ�ȡ�����ļ�..."),FALSE,100,0);
	wait.BeginWait();
	std::list<CInputStudentAnswerRecord*> lstRecord;
	CListAutoClean<CInputStudentAnswerRecord> clr(lstRecord);
	try
	{
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)strFileName);
		CXmlNodeList nodesInput;
		reader.ReadNodeListByTagName(_T("Input"),nodesInput);
		ASSERT(nodesInput.GetLength() == 1);
		CXmlNode nodeInput;
		nodesInput.GetNode(0,nodeInput);
		CXmlNodeList nodes;
		nodeInput.GetChilds(nodes);
		for(long i = 0; i < nodes.GetLength(); i++)
		{
			CXmlNode node;
			nodes.GetNode(i, node);
			CComBSTR bstrNodeName = node.GetNodeName();
			if(CDataHandler::BSTRCompare(bstrNodeName,L"Student") == 0)
			{
				CInputStudentAnswerRecord* pRecord = new CInputStudentAnswerRecord();
				CComBSTR bstrStuID;
				node.ReadAttributeByNoCase(L"StuID",&bstrStuID);
				pRecord->m_strStuID = (const TCHAR*)bstrStuID;
				CComBSTR bstrExamTime;
				node.ReadAttributeByNoCase(L"ExamTime",&bstrExamTime);
				pRecord->m_ExamTime.ParseDateTime((const TCHAR*)bstrExamTime);
				CComBSTR bstrSubjectID;
				node.ReadAttributeByNoCase(L"ExamSubjectID",&bstrSubjectID);
				pRecord->m_strSubjectID = (const TCHAR*)bstrSubjectID;
				CComBSTR bstrPaperID;
				node.ReadAttributeByNoCase(L"ExamPaperID",&bstrPaperID);
				pRecord->m_strExamPaperID = (const TCHAR*)bstrPaperID;
				CComBSTR bstrAnswer = node.GetNodeValue();
				pRecord->m_strStuAnswer = (const TCHAR*)bstrAnswer;
				lstRecord.push_back(pRecord);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	catch (CXmlException* e) 
	{
		e->ReportError();
		e->Delete();
		CString strMsg;
		strMsg.Format(_T("�ļ�%s��ʽ����ȷ"),strFileName);
		AfxMessageBox(strMsg);
		return ;
	}
	wait.Close();
	if(lstRecord.size() <= 0)
	{
		AfxMessageBox(_T("û�ж�ȡ����¼��Ϣ��"));
		return;
	}
	CDlgInputStudentAnswerRecord dlg1;
	dlg1.m_plstRecord = &lstRecord;
	dlg1.DoModal();
}
