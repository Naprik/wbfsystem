// DlgPhoto.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgPhoto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPhoto dialog


CDlgPhoto::CDlgPhoto(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPhoto::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPhoto)
	//}}AFX_DATA_INIT
}


void CDlgPhoto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPhoto)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_PHOTO, m_photo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPhoto, CDialog)
	//{{AFX_MSG_MAP(CDlgPhoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPhoto message handlers

BOOL CDlgPhoto::OnInitDialog()
{
	CDialog::OnInitDialog();
	
//	GetDlgItem(IDC_STATIC)->SetWindowText(m_title);

	CButton   *aa=(CButton*)GetDlgItem(IDC_PHOTO);  
	CString path = m_path;
	HBITMAP   hbmp=(HBITMAP)::LoadImage(NULL,path,IMAGE_BITMAP,180,200,LR_LOADFROMFILE);   
	 aa->SetBitmap(hbmp); 

	GetDlgItem(IDC_IDNUM)->SetWindowText(m_id);
	GetDlgItem(IDC_NAME)->SetWindowText(m_name);
	GetDlgItem(IDC_GENDER)->SetWindowText(m_gender);
	GetDlgItem(IDC_DEPARTMENTSTATIC)->SetWindowText(m_depart);
	GetDlgItem(IDC_CLASSSTATIC)->SetWindowText(m_class);
	GetDlgItem(IDC_TEL)->SetWindowText(m_tel);
	GetDlgItem(IDC_EMAIL)->SetWindowText(m_email);
	 return TRUE;
}

void CDlgPhoto::OnOK() 
{	

	CDialog::OnOK();
}


