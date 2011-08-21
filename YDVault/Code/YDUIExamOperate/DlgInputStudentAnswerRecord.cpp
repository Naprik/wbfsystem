// DlgInputStudentAnswerRecord.cpp : implementation file
//

#include "stdafx.h"
#include "DlgInputStudentAnswerRecord.h"
#include "afxdialogex.h"
#include "InputStudentAnswerRecord.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../Base\DataHandler.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\PropQueryContidition.h"
#include "../ObjRef\YDObjectRef.h"
#include "../DBBase\DBTransactionRef.h"
#include "../UIBase\PdemWait.h"

// CDlgInputStudentAnswerRecord dialog

const int cColSerial = 0;//序号
const int cColStuID = 1;//学生学号
const int cColExamTime = 2;//考试时间
const int cColSubjectID = 3;//科目编号
const int cColPaperID = 4;//试卷编号
const int cColStuAnswer = 5;//学生答案

IMPLEMENT_DYNAMIC(CDlgInputStudentAnswerRecord, CDialogEx)

CDlgInputStudentAnswerRecord::CDlgInputStudentAnswerRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInputStudentAnswerRecord::IDD, pParent)
{
	m_plstRecord = NULL;
}

CDlgInputStudentAnswerRecord::~CDlgInputStudentAnswerRecord()
{
	m_plstRecord = NULL;
}

void CDlgInputStudentAnswerRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputStudentAnswerRecord, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgInputStudentAnswerRecord::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputStudentAnswerRecord message handlers


BOOL CDlgInputStudentAnswerRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rectBK;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rectBK);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	if (!m_Grid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_Grid.EnableDragHeaderItems(FALSE);
	//m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	m_Grid.InsertColumn(cColSerial, L"序号", 60);
	m_Grid.InsertColumn(cColStuID, L"学生学号", 80);
	m_Grid.InsertColumn(cColExamTime, L"考试时间", 120);
	m_Grid.InsertColumn(cColSubjectID, L"科目编号", 80);
	m_Grid.InsertColumn(cColPaperID, L"试卷编号", 80);
	m_Grid.InsertColumn(cColStuAnswer, L"学生答案", 160);
	ASSERT(m_plstRecord);
	for(auto itr = m_plstRecord->begin();itr != m_plstRecord->end();++itr)
	{
		int iRow = m_Grid.GetRowCount();
		CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		CString strSerialNo;
		strSerialNo.Format(_T("%d"),iRow+1);
		
		//序号
		pRow->GetItem(cColSerial)->SetValue(CComVariant(strSerialNo));
		pRow->GetItem(cColSerial)->SetBackgroundColor(RGB(110,180,200));
		pRow->GetItem(cColSerial)->Enable(FALSE);

		//学生学号
		pRow->GetItem(cColStuID)->SetValue(CComVariant((*itr)->m_strStuID));
		CComVariant valTime;
		valTime.vt = VT_DATE;
		valTime.date = (*itr)->m_ExamTime;
		pRow->GetItem(cColExamTime)->SetValue(valTime);
		pRow->GetItem(cColExamTime)->Enable(FALSE);
		pRow->GetItem(cColExamTime)->SetBackgroundColor(RGB(110,180,200));
		pRow->GetItem(cColSubjectID)->SetValue(CComVariant((*itr)->m_strSubjectID));
		pRow->GetItem(cColPaperID)->SetValue(CComVariant((*itr)->m_strExamPaperID));
		pRow->GetItem(cColStuAnswer)->SetValue(CComVariant((*itr)->m_strStuAnswer));
		pRow->SetData(DWORD_PTR(*itr));
		m_Grid.AddRow(pRow);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgInputStudentAnswerRecord::OnBnClickedOk()
{
	// TODO: Add your control notification handler code 
	UpdateData();
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	for(int i = 0; i < m_Grid.GetRowCount();i++)
	{
		VARIANT_BOOL bExist = VARIANT_FALSE;
		hr = ValidateIsExist(pDB,i,&bExist);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}	
		if(!bExist)
		{
			return;
		}

	}
	//向数据库插入记录
	CPdemWait	wait(_T("请稍候，正在导入数据..."),FALSE,m_Grid.GetRowCount());
	wait.BeginWait();
	CDBTransactionRef TransRef(pDB,TRUE);
	for(int i = 0; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CInputStudentAnswerRecord* pRecord = (CInputStudentAnswerRecord*)pRow->GetData();
		ASSERT(pRecord);
		hr = pRecord->InsertToDB(pDB);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		wait.StepIt();
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	wait.Close();
	AfxMessageBox(_T("数据导入成功！"));
	CDialogEx::OnOK();
}

HRESULT CDlgInputStudentAnswerRecord::ValidateIsExist(CDatabaseEx* _pDB,int _iRow,VARIANT_BOOL* _bValid)
{
	HRESULT hr = E_FAIL;
	//要判断学生学号，科目编号和试卷编号是否存在
	*_bValid = VARIANT_FALSE;
	CBCGPGridRow* pRow = m_Grid.GetRow(_iRow);
	ASSERT(pRow);
	CInputStudentAnswerRecord* pRecord = (CInputStudentAnswerRecord*)pRow->GetData();
	ASSERT(pRecord);
	CComVariant valStuID = pRow->GetItem(cColStuID)->GetValue();
	CString strStuID = CDataHandler::VariantToString(valStuID);
	CDataHandler::TrimString(strStuID);
	pRecord->m_strStuID = strStuID;
	CComVariant valSubjectID = pRow->GetItem(cColSubjectID)->GetValue();
	CString strSubjectID = CDataHandler::VariantToString(valSubjectID);
	CDataHandler::TrimString(strSubjectID);
	pRecord->m_strSubjectID = strSubjectID;
	CComVariant valPaperID = pRow->GetItem(cColPaperID)->GetValue();
	CString strPaperID = CDataHandler::VariantToString(valPaperID);
	CDataHandler::TrimString(strPaperID);
	pRecord->m_strExamPaperID = strPaperID;
	hr = pRecord->CreateObj(_pDB);
	if(FAILED(hr))
	{
		return hr;
	}
	{
		//判断学生学号是否存在
		if(pRecord->m_pStudent == NULL)
		{
			CString strMsg;
			strMsg.Format(_T("第%d行学生学号为%s的学生不存在！"),_iRow+1,strStuID);
			AfxMessageBox(strMsg);
			return S_FALSE;
		}
	}

	{
		//判断科目编号是否存在
		if(pRecord->m_pSubject == NULL)
		{
			CString strMsg;
			strMsg.Format(_T("第%d行科目编号为%s的考试科目不存在！"),_iRow+1,strSubjectID);
			AfxMessageBox(strMsg);
			return S_FALSE;
		}
	}

	{
		//判断试卷编号是否存在
		if(pRecord->m_pPaper == NULL)
		{
			CString strMsg;
			strMsg.Format(_T("第%d行试卷编号为%s的试卷不存在！"),_iRow+1,strPaperID);
			AfxMessageBox(strMsg);
			return S_FALSE;
		}
	}
	{
		//判断关系是否存在
		if(pRecord->m_pLinkSubjectPaper == NULL)
		{
			CString strMsg;
			strMsg.Format(_T("第%d行科目编号为%s的科目下面不存在试卷编号为%s的试卷！"),_iRow+1,strSubjectID,strPaperID);
			AfxMessageBox(strMsg);
			return S_FALSE;
		}
	}
	*_bValid = VARIANT_TRUE;
	return S_OK;
}