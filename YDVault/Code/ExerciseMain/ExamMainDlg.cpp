// ExamMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExamMainDlg.h"


// CExamMainDlg dialog

IMPLEMENT_DYNAMIC(CExamMainDlg, CDialog)

CExamMainDlg::CExamMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExamMainDlg::IDD, pParent)
{

}

CExamMainDlg::~CExamMainDlg()
{
}

void CExamMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_ABORT, m_btnAbort);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_BEGIN, m_btnBegin);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_STDANSWER, m_btnStdAnswer);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_STUANSWER, m_btnStuAnswer);
	DDX_Control(pDX, IDC_BTN_EM_EXAM_CLOSE, m_btnFinish);
}


BEGIN_MESSAGE_MAP(CExamMainDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_EM_EXAM_STDANSWER, &CExamMainDlg::OnBnClickedBtnEmExamStdanswer)
END_MESSAGE_MAP()


// CExamMainDlg message handlers

BOOL CExamMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnBegin.SetImage(IDB_BITMAP_BEGIN);
	m_btnAbort.SetImage(IDB_BITMAP_Abort);
	m_btnNext.SetImage(IDB_BITMAP_NEXT);
	m_btnPrev.SetImage(IDB_BITMAP_PREV);
	m_btnStop.SetImage(IDB_BITMAP_STOP);
	m_btnStdAnswer.SetImage(IDB_BITMAP_STDANS);
	m_btnStuAnswer.SetImage(IDB_BITMAP_STUANS);
	m_btnFinish.SetImage(IDB_BITMAP_FINISH);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExamMainDlg::OnBnClickedBtnEmExamStdanswer()
{
	// TODO: Add your control notification handler code here
}
