// DlgStudentAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgStudentAnswer.h"
#include "QuestionRecord.h"
#include "QuestionRecordStruct.h"
#include "../ObjRef\YDQuestionType.h"


const int cColQuestionType = 0;//题型
const int cColQuestionNo   = 1;//题号
const int cColQuestionAnswer = 2;//答案

// CDlgStudentAnswer dialog

IMPLEMENT_DYNAMIC(CDlgStudentAnswer, CDialog)

CDlgStudentAnswer::CDlgStudentAnswer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStudentAnswer::IDD, pParent)
{
	m_pQuestionRecord = NULL;
}

CDlgStudentAnswer::~CDlgStudentAnswer()
{
}

void CDlgStudentAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStudentAnswer, CDialog)
END_MESSAGE_MAP()


// CDlgStudentAnswer message handlers

BOOL CDlgStudentAnswer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	ScreenToClient(&rect);	
	if(!m_listGrid.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE,rect,this,-1))
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
	m_listGrid.InsertColumn(cColQuestionAnswer,_T("答案"),280);

	ASSERT(m_pQuestionRecord);
	HRESULT hr = E_FAIL;
	int typeId = 0;
	int index = 1;
	CBCGPGridRow* pParentRow = NULL;
	for(std::list<CQuestionRecordStruct*>::const_iterator itr = m_pQuestionRecord->m_lstQuestionStruct.begin();
		itr != m_pQuestionRecord->m_lstQuestionStruct.end();++itr)
	{
		CQuestionRecordStruct* pQRecordStruct = *itr;
		if (typeId != pQRecordStruct->m_QTypeID)
		{
			pParentRow = NULL;
			typeId = pQRecordStruct->m_QTypeID;
		}
		CBCGPGridRow* pInsertRow = NULL;
		hr = InsertList(*itr,pParentRow,pInsertRow);
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
	for(int i= 0; i < m_listGrid.GetRowCount();i++)
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
			pRow->GetItem(cColQuestionAnswer)->SetBackgroundColor(RGB(110,180,200));
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgStudentAnswer::InsertList(CQuestionRecordStruct* _pQRecordStuct,
				   CBCGPGridRow* _pParentRow,
				   CBCGPGridRow*& _pInsertRow)
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
	strIndex.Format(L"第%d题", m_listGrid.GetRowCount());
	_pInsertRow->GetItem(cColQuestionNo)->SetValue(_variant_t(strIndex));
	CString strUserAnswer;
	for(std::list<CString>::const_iterator itr = _pQRecordStuct->m_listUserAnswers.begin();
		itr != _pQRecordStuct->m_listUserAnswers.end();++itr)
	{
		strUserAnswer += *itr;
	}
	_pInsertRow->GetItem(cColQuestionAnswer)->SetValue(_variant_t(strUserAnswer));
	_pInsertRow->SetData(DWORD_PTR(_pQRecordStuct));
	return S_OK;
}

