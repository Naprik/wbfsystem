// FormImage.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormImage.h"
#include "MainFrm.h"
#include "ExamApp.h"


extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFormImage, CDialog)


CFormImage::CFormImage(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormImage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormImage, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormImage)
	ON_BN_CLICKED(IDC_OPEN_CAMERA, OnOpenCamera)
	ON_BN_CLICKED(IDC_CLOSE_CAMERA, OnCloseCamera)
	ON_BN_CLICKED(IDC_CATCH, OnCatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormImage message handlers

BOOL CFormImage::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_OPEN_CAMERA),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_CLOSE_CAMERA),mdNone,mdRepos);	
	AddSzControl(*GetDlgItem(IDC_CATCH),mdNone,mdRepos);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormImage::OnOpenCamera() 
{
	CRect rc; //��ƵԤ�����ڵĴ�С
	GetWindowRect(rc);  
	m_VideoCapX.Create("������","��������",WS_VISIBLE,rc,this,0,NULL);
	m_VideoCapX.SetConnected(TRUE); //��ʼ����
	m_VideoCapX.SetPreview(TRUE);  //��Ԥ������	
}

void CFormImage::OnCloseCamera() 
{
	m_VideoCapX.SetConnected(FALSE); //��������		
}

void CFormImage::OnCatch() 
{
	m_VideoCapX.SaveFrameJPG("E:\\11.jpg",100);		
}
