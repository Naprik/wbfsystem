// DlgResDetail.cpp : implementation file
//

#include "stdafx.h"
#include "DlgResDetail.h"
#include "../YDExamObjRef\YdStudentRef.h"
#include "../\YDExamObjRef\YDEAddrUnit.h"
#include "../\Base\AutoClean.h"
#include "../YDExamObjRef\YDStuMark.h"
#include "../Base\DataHandler.h"
#include "../UIBase\ListXlsoutput.h"
#include "FormQueryMark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CExamDetail  m_examdeteil;
extern CArray<CStudev, CStudev>  m_arryStud;
/////////////////////////////////////////////////////////////////////////////
// CDlgResDetail dialog


CDlgResDetail::CDlgResDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResDetail::IDD, pParent)
	, m_stuNo(_T(""))
	, m_stuName(_T(""))
	, m_strSubject(_T(""))
	, m_strPaper(_T(""))
	, m_strRightItemNo(_T(""))
	, m_strErrorItemNo(_T(""))
	, m_strTotalScore(_T(""))
	, m_strScore(_T(""))
{
	//{{AFX_DATA_INIT(CDlgResDetail)
	//}}AFX_DATA_INIT
	m_pUnit = NULL;
	m_pStudev = NULL;
	m_pQueryMarkInfo = NULL;
	m_ResultType = FROM_DB;
	m_pPaper = NULL;
	m_pSubject = NULL;
	m_strAnswer = _T("");
}


void CDlgResDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgResDetail)
	DDX_Control(pDX, IDC_RESLIST, m_reslist);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STUNO, m_stuNo);
	DDX_Text(pDX, IDC_EDIT_STUNAME, m_stuName);
	DDX_Text(pDX, IDC_EDIT_SUBJECT, m_strSubject);
	DDX_Text(pDX, IDC_EDIT_PAPER, m_strPaper);
	DDX_Text(pDX, IDC_EDIT_RIGHT_ITEMNO, m_strRightItemNo);
	DDX_Text(pDX, IDC_EDIT_ERROR_ITEMNO, m_strErrorItemNo);
	DDX_Text(pDX, IDC_EDIT_TOTAL_SCORE, m_strTotalScore);
	DDX_Text(pDX, IDC_EDIT_SCORE, m_strScore);
}


BEGIN_MESSAGE_MAP(CDlgResDetail, CDialog)
	//{{AFX_MSG_MAP(CDlgResDetail)
	ON_NOTIFY(NM_DBLCLK, IDC_RESLIST, OnDblclkReslist)
	ON_BN_CLICKED(ID_RESDETAIL_OUTPUT, &CDlgResDetail::OnBnClickedResdetailOutput)
	ON_BN_CLICKED(IDOK, &CDlgResDetail::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgResDetail message handlers
BOOL CDlgResDetail::OnInitDialog()
{
	CDialog::OnInitDialog();

	HRESULT hr = E_FAIL;

	m_reslist.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rt;
	m_reslist.GetClientRect(&rt);

	m_reslist.InsertColumn(0,_T("题号"),LVCFMT_LEFT,rt.right/5,0);
	m_reslist.InsertColumn(1,_T("学生答案"),LVCFMT_LEFT,rt.right/5,0 -2);
	m_reslist.InsertColumn(2,_T("标准答案"),LVCFMT_LEFT,rt.right/5,0 -2);
	m_reslist.InsertColumn(3,_T("分值"),LVCFMT_LEFT,rt.right/5,0);
	m_reslist.InsertColumn(4,_T("对错"),LVCFMT_LEFT,rt.right/5,0);

	CYDPaper* pPaper = NULL;
	if (m_ResultType == FROM_TEACHER_EXAM)
	{
		//普通的教师机考试
		CYdStudentRef* pStuRef = NULL;
	
		CYDSubjectRef* pSubject = NULL;
		ASSERT(m_pUnit);
		hr = m_pUnit->GetStudent(pStuRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		CGExamStruct* pGExamStruct = NULL;
		hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		ASSERT(pGExamStruct);
		pPaper = dynamic_cast<CYDPaper*>(pGExamStruct->m_pExamDetail->m_pPaper);
		pSubject = dynamic_cast<CYDSubjectRef*>(pGExamStruct->m_pExamDetail->m_pSubject);
		CPtrAutoClean<CYdStudentRef> clr(pStuRef);
		ASSERT(pStuRef);
		hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,m_stuNo);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,m_stuName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		hr = pPaper->GetPropVal(L"NAME",m_strPaper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		hr = pSubject->GetPropVal(FIELD_YDSUBJECT_NAME,m_strSubject);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}
	else if(m_ResultType == FROM_DB)
	{
		m_stuNo = m_pQueryMarkInfo->m_strStuID;
		m_stuName = m_pQueryMarkInfo->m_strStuName;
		hr = m_pQueryMarkInfo->GetPaperName(m_strPaper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		hr = m_pQueryMarkInfo->GetPaper(pPaper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}
	else if(m_ResultType == FROM_INRARED_EXAM)
	{
		//红外考试
		CYdStudentRef* pStuRef = NULL;
		CYDSubjectRef* pSubject = NULL;
		ASSERT(m_pUnit);
		hr = m_pUnit->GetStudent(pStuRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		CPtrAutoClean<CYdStudentRef> clr(pStuRef);
		ASSERT(pStuRef);
		hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,m_stuNo);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,m_stuName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		pPaper = m_pPaper;
		pSubject = m_pSubject;
		hr = pPaper->GetPropVal(L"NAME",m_strPaper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		hr = pSubject->GetPropVal(FIELD_YDSUBJECT_NAME,m_strSubject);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	

	hr = InitAnswerInfo(pPaper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	UpdateData(FALSE);
	if (m_ResultType == FROM_DB)
	{
		CPtrAutoClean<CYDPaper> clr(pPaper);
		//CPtrAutoClean<CYDSubjectRef> clr2(pSubject);
	}


	return TRUE;
}




void CDlgResDetail::OnDblclkReslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

HRESULT CDlgResDetail::InitAnswerInfo(CYDPaper* pPaper)
{
	HRESULT hr = E_FAIL;
	ASSERT(pPaper);
	long lBegin1 = 0,lEnd1 = 0,lBegin2 = 0,lEnd2 = 0,lBegin3 = 0,lEnd3 = 0;
	hr = pPaper->GetPropVal(FIELD_YDPAPER_BEGIN1,&lBegin1);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pPaper->GetPropVal(FIELD_YDPAPER_END1,&lEnd1);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pPaper->GetPropVal(FIELD_YDPAPER_BEGIN2,&lBegin2);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pPaper->GetPropVal(FIELD_YDPAPER_END2,&lEnd2);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pPaper->GetPropVal(FIELD_YDPAPER_BEGIN3,&lBegin3);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pPaper->GetPropVal(FIELD_YDPAPER_END3,&lEnd3);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*> lstStdAns;//标准答案
	CListAutoClean<CYDObjectRef> clr(lstStdAns);
	hr = pPaper->GetStdAnswer(&lstStdAns);
	if(FAILED(hr))
	{
		return hr;
	}
	double dbTotalScore = 0.0;//总分
	//计算总分数
	for(std::list<CYDObjectRef*>::const_iterator itr = lstStdAns.begin();
		itr != lstStdAns.end();++itr)
	{
		CComVariant varMark;
		hr = (*itr)->GetPropVal(FIELD_YDANSWER_MARK, &varMark);
		if(FAILED(hr))
		{
			return hr;
		}
		dbTotalScore+= (double)_variant_t(varMark);;
	}
	CComVariant valTotalScore(dbTotalScore);
	m_strTotalScore = CDataHandler::VariantToString(valTotalScore);
	std::map<int, CString> mapAnswer;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDStuMark YdStuMak(pDB);
	CComVariant valAnswer;
	if (m_ResultType == FROM_TEACHER_EXAM  )
	{
		CString strAnswer;
		hr = m_pStudev->GetAnswer(strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		CDataHandler::StringToVariant(strAnswer,VT_BSTR,&valAnswer);
	}
	else if(m_ResultType == FROM_INRARED_EXAM)
	{
		CDataHandler::StringToVariant(m_strAnswer,VT_BSTR,&valAnswer);
	}
	else
	{
		CDataHandler::StringToVariant(m_pQueryMarkInfo->m_strAnswer,VT_BSTR,&valAnswer);
	}
	hr =  YdStuMak.SetPropVal(FIELD_YDSTUMARK_ANSWER,&valAnswer);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = YdStuMak.TransferAnswerByString(&mapAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	UINT uErrorNum = 0;
	UINT uRightNum = 0;
	double dbMark = 0;
	hr = InsertAnswerByBeginEnd(lBegin1,lEnd1,lstStdAns,mapAnswer,uErrorNum,uRightNum,dbMark);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = InsertAnswerByBeginEnd(lBegin2,lEnd2,lstStdAns,mapAnswer,uErrorNum,uRightNum,dbMark);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = InsertAnswerByBeginEnd(lBegin3,lEnd3,lstStdAns,mapAnswer,uErrorNum,uRightNum,dbMark);
	if(FAILED(hr))
	{
		return hr;
	}
	m_strRightItemNo.Format(_T("%d"),uRightNum);
	m_strErrorItemNo.Format(_T("%d"),uErrorNum);
	CComVariant valMark(dbMark);
	m_strScore = CDataHandler::VariantToString(valMark);
	return S_OK;
}

HRESULT CDlgResDetail::InsertAnswerByBeginEnd(long lBegin,long lEnd,
							   std::list<CYDObjectRef*>& _lstStdAnswer,
							   std::map<int, CString> &_mapAnswer,
							   UINT &uErrorNum,
							   UINT &uRightNum,
							   double &dbMark )
{
	HRESULT hr = E_FAIL;
	if(lBegin == 0 || lEnd == 0)
	{
		return S_OK;
	}
	for(long l = lBegin;l <= lEnd;l++)
	{
		CString strStdAnswerNo;
		strStdAnswerNo.Format(L"%d", l);
		int iCount = m_reslist.GetItemCount();
		int index = m_reslist.InsertItem(iCount,strStdAnswerNo);
		CString strStdAnswer;
		CComVariant valStdAnswerMark(0.0);
		for(std::list<CYDObjectRef*>::const_iterator itr = _lstStdAnswer.begin();
			itr != _lstStdAnswer.end();++itr)
		{
			long iAnswerNo;
			hr = (*itr)->GetPropVal(FIELD_YDANSWER_NO,&iAnswerNo);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iAnswerNo != l)
			{
				continue;
			}
			CComVariant valStdAnswer;
			hr = (*itr)->GetPropVal(FIELD_YDANSWER_ANSWER,&valStdAnswer);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = (*itr)->GetPropVal(FIELD_YDANSWER_MARK,&valStdAnswerMark);
			if(FAILED(hr))
			{
				return hr;
			}
			strStdAnswer = CDataHandler::VariantToString(valStdAnswer);
		}
		std::map<int, CString>::const_iterator itrUser = _mapAnswer.find(l);
		BOOL bRight = TRUE;
		if (itrUser != _mapAnswer.end())
		{
			m_reslist.SetItemText(index,1,itrUser->second);
			if (itrUser->second.CompareNoCase(strStdAnswer) == 0)
			{
				uRightNum++;
				dbMark += (double)_variant_t(valStdAnswerMark);
			}
			else
			{
				uErrorNum++;

				//m_reslist.SetBkColor(RGB(255,0,0));
				//m_reslist.SetItemColor(index,(DWORD)RGB(255,0,0)/*颜色*/);
				bRight = FALSE;
			}
		}
		else
		{
			uErrorNum++;
			
			//m_reslist.SetBkColor(RGB(255,0,0));
			//m_reslist.SetItemColor(index,(DWORD)RGB(255,0,0)/*颜色*/);
			bRight = FALSE;
		}
		m_reslist.SetItemText(index,2,strStdAnswer);

		CString strStdAnswerMark = CDataHandler::VariantToString(valStdAnswerMark);
		m_reslist.SetItemText(index,3,strStdAnswerMark);
		if (bRight)
		{
			m_reslist.SetRowColor(index);
			m_reslist.SetItemText(index,4,L"对");
		}
		else 
		{
			m_reslist.SetRowColor(index,RGB(255,0,0));
			m_reslist.SetItemText(index,4,L"错");
		}
	//	m_reslist.SetItemData(index, (DWORD_PTR)(*itr));
	}
	return S_OK;
}

void CDlgResDetail::OnBnClickedResdetailOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_reslist);
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("学号"),m_stuNo));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("姓名"),m_stuName));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("科目"),m_strSubject));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("试卷"),m_strPaper));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("正确题数"),m_strRightItemNo));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("错误题数"),m_strErrorItemNo));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("总分"),m_strTotalScore));
	ListXlsoutput.m_lstCapiton.push_back(std::make_pair(_T("成绩"),m_strScore));
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

void CDlgResDetail::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
