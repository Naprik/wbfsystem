#include "stdafx.h"
#include "MainFrm.h"
#include "hardctrl.h"
#include "LeftManage.h"

IMPLEMENT_DYNCREATE(CLeftManage, CDialog)

CLeftManage::CLeftManage(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CLeftManage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWeb)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CLeftManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftManage)
	DDX_Control(pDX, IDC_GROUP, m_group);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftManage, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CLeftManage)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_OUTBAR_NOTIFY, OnOutbarNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftManage message handlers


void CLeftManage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	if ( ::IsWindow(m_group.m_hWnd) )
	{	
		CRect lpRect,rect1;
		GetWindowRect(&lpRect);	
		rect1.left = 0;
		rect1.top  = 0;
		rect1.right= lpRect.right - lpRect.left;
		rect1.bottom=lpRect.bottom - lpRect.top ;
		m_group.MoveWindow(rect1);
		if ( ::IsWindow(m_wndOutlookBar))
		{
			m_group.GetWindowRect(&lpRect);
			rect1.left = 0;
			rect1.top  = 0;
			rect1.right= lpRect.right - lpRect.left;
			rect1.bottom=lpRect.bottom - lpRect.top;
     		m_wndOutlookBar.MoveWindow(rect1);
		}
	}
}
LRESULT CLeftManage::OnOutbarNotify(WPARAM wParam, LPARAM lParam)
{  
	int nBarAction = (int)wParam;
	XT_OUTBAR_INFO* pOBInfo = (XT_OUTBAR_INFO*)lParam;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	if (nBarAction == OBN_XT_ITEMCLICK)
	{	
		CString msg;
		DWORD data = m_wndOutlookBar.GetItemData(pOBInfo->nIndex);
        if ( data == 1 )
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_STU_RESULT_QUERY);
		}
	    if ( data == 2 )
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_CLASS_RESULT_QUERY);
		}
	    if ( data == 3 )
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_PAPER_ANALY);
		}
	    if ( data == 4 )
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_STUINFO);
		}

	}
	return TRUE;
}


BOOL CLeftManage::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	if (!m_wndOutlookBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0),this, NULL))
	{
		AfxMessageBox("Failed to create outlook bar.");
		return FALSE;
	}
	m_ImageSmall.Create (16, 16, ILC_COLOR16|ILC_MASK, 2, 1);
	m_ImageLarge.Create (32, 32, ILC_COLOR16|ILC_MASK, 2, 1);

	for (int i = 0; i < 20; ++i)
	{
		HICON hIcon;
		switch(i)
		{
		case 1:
			hIcon = AfxGetApp()->LoadIcon(IDI_LINK);
			break;
		case 2:
			hIcon = AfxGetApp()->LoadIcon(IDI_RADIO);
			break;
		case 3:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_ALL);
			break;
		case 4:
			hIcon = AfxGetApp()->LoadIcon(IDI_SINGLE);
			break;
		case 5:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROOLING);
			break;
		case 6:
			hIcon = AfxGetApp()->LoadIcon(IDI_SPESH);
			break;
		case 7:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_SINGLE);
			break;
		case 8:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_SINGLE);
			break;
		case 9:
			hIcon = AfxGetApp()->LoadIcon(IDI_IMPORT);
			break;
		case 10:
			hIcon = AfxGetApp()->LoadIcon(IDI_DEPART);
			break;
		case 11:
			hIcon = AfxGetApp()->LoadIcon(IDI_PP);
			break;
		case 12:
			hIcon = AfxGetApp()->LoadIcon(IDI_ANSWER);
			break;
		case 13:
			hIcon = AfxGetApp()->LoadIcon(IDI_TEACH);
			break;			
		case 14:
			hIcon = AfxGetApp()->LoadIcon(IDI_PAPER);
			break;		
		case 15:
			hIcon = AfxGetApp()->LoadIcon(IDI_SUB);
			break;		
		case 16:
			hIcon = AfxGetApp()->LoadIcon(IDI_WEB);
			break;
			
		default:
			hIcon = AfxGetApp()->LoadIcon(IDI_RADIO);
			break;
		}
		ASSERT(hIcon);
		m_ImageSmall.Add(hIcon);
		m_ImageLarge.Add(hIcon);
	}

    int iFolder; // index of the added folder
	m_wndOutlookBar.SetImageList(&m_ImageLarge, OBS_XT_LARGEICON);
	m_wndOutlookBar.SetImageList(&m_ImageSmall, OBS_XT_SMALLICON);	
	m_wndOutlookBar.SetBackColor(RGB(0x3a,0x6e,0xa5));
	m_wndOutlookBar.SetTextColor(RGB(0xff,0xff,0xff));

	iFolder = m_wndOutlookBar.AddFolder(_T("成绩查询"), 0);
	DWORD id = 0;
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("学生成绩"), 1, 1);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("班级-试卷成绩分布"), 2, 2);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("试卷分析"), 3, 3);
	m_wndOutlookBar.InsertItem(iFolder, 3, _T("学生查询"), 4, 4);

	m_wndOutlookBar.SetOwner(this);
	m_wndOutlookBar.SetSelFolder(0);
	m_wndOutlookBar.ShowWindow( SW_SHOW );
	m_group.ShowWindow(SW_HIDE);
	return TRUE;  
}

void CLeftManage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call cdxCSizingDialog::OnPaint() for painting messages
}
