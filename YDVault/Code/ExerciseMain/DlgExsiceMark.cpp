// DlgExsiceMark.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgExsiceMark.h"
#include "QuestionRecord.h"
#include "QuestionRecordStruct.h"
#include "../ObjRef\YDQuestionType.h"


// CDlgExsiceMark dialog


const int cColQuestionType = 0;//题型
const int cColQuestionNo   = 1;//题号
const int cColQuestionStdAnswer = 2;//标准答案
const int cColQuestionStudentAnswer = 3;//学生答案

IMPLEMENT_DYNAMIC(CDlgExsiceMark, CDialog)

CDlgExsiceMark::CDlgExsiceMark(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExsiceMark::IDD, pParent)
	, m_strMark(_T(""))
{
	m_pQuestionRecord = NULL;
	m_accuracy = 0;
}

CDlgExsiceMark::~CDlgExsiceMark()
{
}

void CDlgExsiceMark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MARK, m_strMark);
}


BEGIN_MESSAGE_MAP(CDlgExsiceMark, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgExsiceMark::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgExsiceMark message handlers

BOOL CDlgExsiceMark::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	HRESULT hr = E_FAIL;
	CRect rect;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	ScreenToClient(&rect);	
	if(!m_listGrid.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE,rect,this,IDC_GRID_CREATE))
	{
		TRACE0("Failed to m_listctrlDR.Create!\n");
		return FALSE;
	}
	m_listGrid.EnableHeader(TRUE,0);
	m_listGrid.EnableDragHeaderItems(FALSE);
	m_listGrid.SetWholeRowSel(TRUE);
	m_listGrid.SetReadOnly();



	m_listGrid.InsertColumn(cColQuestionType,_T("题型"),100);
	m_listGrid.InsertColumn(cColQuestionNo,_T("题号"),120);
	m_listGrid.InsertColumn(cColQuestionStdAnswer,_T("标准答案"),160);
	m_listGrid.InsertColumn(cColQuestionStudentAnswer,_T("学生答案"),160);

	ASSERT(m_pQuestionRecord);
	double dbMark = 0.0;
	hr = m_pQuestionRecord->ComputeMark(&dbMark, &m_accuracy);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	m_strMark.Format(_T("%.2f"),dbMark);
	int typeId = 0;
	int index = 1;
	CBCGPGridRow* pParentRow = NULL;
	for(std::list<CQuestionRecordStruct*>::const_iterator itr = m_pQuestionRecord->m_lstQuestionStruct.begin();
		itr != m_pQuestionRecord->m_lstQuestionStruct.end();++itr,++index)
	{
		CQuestionRecordStruct* pQRecordStruct = *itr;
		if (typeId != pQRecordStruct->m_QTypeID)
		{
			pParentRow = NULL;
			typeId = pQRecordStruct->m_QTypeID;
		}
		CBCGPGridRow* pInsertRow = NULL;
		hr = InsertList(*itr,pParentRow,pInsertRow,index);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		if(pParentRow == NULL)
		{
			pParentRow = pInsertRow;
		}
	}
	/*for(int i= 0; i < m_listGrid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_listGrid.GetRow(i);
		ASSERT(pRow);
		if(i%2 == 0)
		{
			if(pRow->GetParent() == NULL)
			{
				pRow->GetItem(cColQuestionType)->SetBackgroundColor(RGB(110,180,200));
			}
			pRow->GetItem(cColQuestionNo)->SetBackgroundColor(RGB(110,180,200));
			pRow->GetItem(cColQuestionStdAnswer)->SetBackgroundColor(RGB(110,180,200));
			pRow->GetItem(cColQuestionStudentAnswer)->SetBackgroundColor(RGB(110,180,200));
		}
	}*/
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgExsiceMark::InsertList(CQuestionRecordStruct* _pQRecordStuct,
									  CBCGPGridRow* _pParentRow,
									  CBCGPGridRow*& _pInsertRow,
									  int _iItemNo /*题号*/)
{
	HRESULT hr = E_FAIL;
	_pInsertRow = m_listGrid.CreateRow(m_listGrid.GetColumnCount());
	_pInsertRow->AllowSubItems();
	if(_pParentRow == NULL)
	{
		//根节点
		CYDQuestionType qtype(theApp.m_pDatabase);		
		qtype.SetID(_pQRecordStuct->m_QTypeID);
		CString strType;
		qtype.GetPropVal(L"name", strType);
		_pInsertRow->GetItem(cColQuestionType)->SetValue(_variant_t(strType));
		m_listGrid.AddRow(_pInsertRow);
	}
	else
	{
		_pParentRow->AddSubItem(_pInsertRow);
	}

	CString strIndex;
	strIndex.Format(L"第%d题", _iItemNo);
	_pInsertRow->GetItem(cColQuestionNo)->SetValue(_variant_t(strIndex));
	_pInsertRow->SetData(DWORD_PTR(_pQRecordStuct));

	ASSERT(_pQRecordStuct->m_pQuestion);
	std::list<CString> lstStdAnswer;//标准答案
	hr = _pQRecordStuct->m_pQuestion->GetStandardAnswer(lstStdAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	
	if (_pQRecordStuct->m_QTypeID == 8)//阅读选词填空
	{
		CString useranswer = *(lstStdAnswer.begin());
		lstStdAnswer.clear();
		for (int i=0; i<useranswer.GetLength(); ++i)
		{
			CString answer(useranswer.GetAt(i));
			lstStdAnswer.push_back(answer);
		}
	}
	ASSERT(lstStdAnswer.size() >= _pQRecordStuct->m_listUserAnswers.size());
	int index = 1;
	BOOL bUserAnswerEnd = FALSE;
	std::list<CString>::const_iterator itrUserAnswer = _pQRecordStuct->m_listUserAnswers.begin();
	for(std::list<CString>::const_iterator itrStd = lstStdAnswer.begin();
		itrStd != lstStdAnswer.end();++itrStd,++index)
	{
		CString strIndex;
		strIndex.Format(_T("%d)"),index);
		CBCGPGridRow* pAnswerRow = m_listGrid.CreateRow(m_listGrid.GetColumnCount());
		pAnswerRow->AllowSubItems();
		pAnswerRow->GetItem(cColQuestionNo)->SetValue(_variant_t(strIndex));
		pAnswerRow->GetItem(cColQuestionStdAnswer)->SetValue(_variant_t(*itrStd));
		if(itrUserAnswer != _pQRecordStuct->m_listUserAnswers.end())
		{
			pAnswerRow->GetItem(cColQuestionStudentAnswer)->SetValue(_variant_t(*itrUserAnswer));
		}
		if ((*itrStd).CompareNoCase(*itrUserAnswer))
		{
			pAnswerRow->GetItem(cColQuestionNo)->SetBackgroundColor(RGB(254,137,149));
			pAnswerRow->GetItem(cColQuestionStdAnswer)->SetBackgroundColor(RGB(254,137,149));
			pAnswerRow->GetItem(cColQuestionStudentAnswer)->SetBackgroundColor(RGB(254,137,149));
		}
		_pInsertRow->AddSubItem(pAnswerRow);
		
		if(itrUserAnswer == _pQRecordStuct->m_listUserAnswers.end())
		{
			bUserAnswerEnd = TRUE;
		}
		else
		{
			++itrUserAnswer;
		}
	}
	return S_OK;
}


void CDlgExsiceMark::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

int		CDlgExsiceMark::GetAccuracy()
{
	return m_accuracy;
}