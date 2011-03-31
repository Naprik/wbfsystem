// DlgTeacherSet.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgTeacherSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherSet dialog


CDlgTeacherSet::CDlgTeacherSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeacherSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTeacherSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTeacherSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTeacherSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTeacherSet, CDialog)
	//{{AFX_MSG_MAP(CDlgTeacherSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherSet message handlers
