// DlgLoginEX.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLoginEX.h"
#include "../UIBASE\JahCoverCapture.h"
#include "../DBBase\DatabaseEx.h"
#include "../Include\ShowErr.h"
#include "../Base\DataHandler.h"
#include "../Base\AutoClean.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../YDUIUserManagement\StaticYdUser.h"
#include "../ObjRef\PropQueryContidition.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoginEX dialog

 
CDlgLoginEX::CDlgLoginEX(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgLoginEX::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoginEX)
	m_user = _T("admin");
	m_pass = _T("");
	//}}AFX_DATA_INIT
	m_pDatabase = NULL;
}


void CDlgLoginEX::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoginEX)
	DDX_Control(pDX, IDC_PASS, m_passctl);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_flash);
	DDX_Text(pDX, IDC_PASS, m_pass);
	DDX_Text(pDX, IDC_USER, m_user);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoginEX, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgLoginEX)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgLoginEX::OnBnClickedOk)
//	ON_STN_CLICKED(IDC_STATIC_COPYRIGHT, &CDlgLoginEX::OnStnClickedStaticCopyright)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoginEX message handlers

BOOL CDlgLoginEX::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	ModifyStyle( WS_CAPTION, WS_MINIMIZEBOX, SWP_DRAWFRAME );
	CString strBackBmpFile = _ModulePath;
	strBackBmpFile += _T("\\Res\\LoginBack.bmp");
	if(PathFileExists(strBackBmpFile))
	{
		HBITMAP hBitmapBack = (HBITMAP)LoadImage(NULL, strBackBmpFile, IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
		if(hBitmapBack)
		{
			m_bmpBackground.Attach(hBitmapBack);
		}
	
	

	}
	ShowWindow(SW_SHOWMAXIMIZED);
	
	// TODO: Add extra initialization here
	RECT rc;
	this->GetClientRect(&rc);
	rc.top = 200;
//	rc.left = 0;
	rc.bottom = rc.bottom - 200;
//	rc.right= 300;
	
	m_flash.MoveWindow( &rc, false );
	
// 	TCHAR strCurDrt[500];
// 	int nLen = ::GetCurrentDirectory(500,strCurDrt);
// 	if( strCurDrt[nLen]!='\\' )
// 	{
// 		strCurDrt[nLen++] = '\\';
// 		strCurDrt[nLen] = '\0';
// 	}
	CString strFlashName = _ModulePath;
	strFlashName += _T("\\Res\\LoginBack.swf");
	if(PathFileExists(strFlashName))
	{
		CJahCoverCapture capture;

		capture.m_bmpFile =strFlashName ;
		m_flash.LoadMovie(0, strFlashName);
		//	m_flash.SetScale("ExactFit");   
		m_flash.Play();
	}


	this->GetClientRect(&rc);
	rc.right = rc.right - 100;
	rc.left = rc.right - 100 - 50;
	rc.bottom = rc.bottom - 100;
	rc.top = rc.bottom - 25;
	m_passctl.MoveWindow(&rc,false);


	rc.right = rc.right - 130;
	rc.left = rc.right - 100;
	GetDlgItem(IDC_TEXT2)->MoveWindow(&rc,false);
	rc.top = rc.top - 50;
	rc.bottom = rc.bottom - 50;
	GetDlgItem(IDC_TEXT1)->MoveWindow(&rc,false);
	rc.right = +130;
	GetDlgItem(IDC_PASS)->GetWindowRect(&rc);
	rc.top = rc.top - 50;
	rc.bottom = rc.bottom - 50;
	GetDlgItem(IDC_USER)->MoveWindow(&rc,false);

	GetDlgItem(IDC_PASS)->GetWindowRect(&rc);
	rc.left = rc.right-100;
	rc.top += 50;
	rc.bottom = rc.top+30;
	GetDlgItem(IDCANCEL)->MoveWindow(&rc,false);



	rc.right = rc.right - 120;
	rc.left = rc.right - 100;
	GetDlgItem(IDOK)->MoveWindow(&rc,false);



	this->GetClientRect(&rc);
	rc.bottom -= 10;
	rc.top = rc.bottom - 50;
	rc.left += 10;
	rc.right = (rc.right - rc.left)/3;
	GetDlgItem(IDC_STATIC_COPYRIGHT)->MoveWindow(&rc,false);
	GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(_T("Copyright @ 2010-2020 南京我佰分教育科技有限公司"));
	
	Invalidate(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoginEX::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	rect.right = rect.right - 20;
	rect.left = rect.right - 300;
	rect.top  = 20;
	rect.bottom = rect.top + 150;

	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	BITMAP bitMap;
	m_bmpBackground.GetBitmap(&bitMap);
	CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);
	dc.StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);	
	// Do not call CBCGPDialog::OnPaint() for painting messages
}

void CDlgLoginEX::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pDatabase);
	UpdateData();
	if(m_user.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空！"));
		return ;
	}
	std::list<CYDObjectRef*> lstUser;
	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr2(lstQueryCon);
	CPropQueryContidition* pQueryConditon = new CPropQueryContidition();
	lstQueryCon.push_back(pQueryConditon);
	pQueryConditon->m_uOpType = Q_EQUAL;
	pQueryConditon->m_strFiledName = FIELD_YDUSER_NAME;
	pQueryConditon->m_strConVal = m_user;
	pQueryConditon->m_uFieldType = VT_BSTR;

	pQueryConditon = new CPropQueryContidition();
	lstQueryCon.push_back(pQueryConditon);
	pQueryConditon->m_uOpType = Q_EQUAL;
	pQueryConditon->m_strFiledName = FIELD_YDUSER_PASSWORD;
	pQueryConditon->m_strConVal = m_pass;
	pQueryConditon->m_uFieldType = VT_BSTR;

	HRESULT hr = E_FAIL;
	hr = CStaticObjHelper::GetObjRef(DB_YDUSER,m_pDatabase,&lstUser,&lstQueryCon);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	if(lstUser.size() <= 0)
	{
		AfxMessageBox(_T("用户名或密码不正确!"));
		return;
	}
	ASSERT(lstUser.size() == 1);
	//设置当前用户
	CYDUserRef* pCurUser = (CYDUserRef*)(*(lstUser.begin()));
	hr = CStaticYdUser::Instance()->SetCurUser(pCurUser);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	OnOK();
}


