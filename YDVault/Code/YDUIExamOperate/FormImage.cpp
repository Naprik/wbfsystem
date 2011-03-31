// FormImage.cpp : implementation file
//

#include "stdafx.h"
#include "FormImage.h"



IMPLEMENT_DYNCREATE(CFormImage, CYdFormView)


CFormImage::CFormImage()
	: CYdFormView(CFormImage::IDD)
{
	//{{AFX_DATA_INIT(CFormImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormImage::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormImage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormImage, CYdFormView)
	//{{AFX_MSG_MAP(CFormImage)
	ON_BN_CLICKED(IDC_OPEN_CAMERA, OnOpenCamera)
	ON_BN_CLICKED(IDC_CLOSE_CAMERA, OnCloseCamera)
	ON_BN_CLICKED(IDC_CATCH, OnCatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormImage message handlers

void CFormImage::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	
// 	AddSzControl(*GetDlgItem(IDC_OPEN_CAMERA),mdNone,mdRepos);
// 	AddSzControl(*GetDlgItem(IDC_CLOSE_CAMERA),mdNone,mdRepos);	
// 	AddSzControl(*GetDlgItem(IDC_CATCH),mdNone,mdRepos);	
	
	return ;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormImage::OnOpenCamera() 
{
	CRect rc; //视频预览窗口的大小
	GetWindowRect(rc);  
	m_VideoCapX.Create(_T("类名称"),_T("窗口名称"),WS_VISIBLE,rc,this,0,NULL);
	m_VideoCapX.SetConnected(TRUE); //开始连接
	m_VideoCapX.SetPreview(TRUE);  //打开预览窗口	
}

void CFormImage::OnCloseCamera() 
{
	m_VideoCapX.SetConnected(FALSE); //结束连接		
}

void CFormImage::OnCatch() 
{
	m_VideoCapX.SaveFrameJPG(_T("E:\\11.jpg"),100);		
}

BOOL CFormImage::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
											 CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}