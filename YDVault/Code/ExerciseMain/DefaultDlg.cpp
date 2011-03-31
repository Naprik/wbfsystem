// DefaultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DefaultDlg.h"


// CDefaultDlg dialog

IMPLEMENT_DYNAMIC(CDefaultDlg, CQuestionDlg)

CDefaultDlg::CDefaultDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CDefaultDlg::IDD, pParent)
{

}

CDefaultDlg::~CDefaultDlg()
{
}

void CDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDefaultDlg, CQuestionDlg)
END_MESSAGE_MAP()


// CDefaultDlg message handlers
