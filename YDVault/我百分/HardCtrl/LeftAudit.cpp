#include "stdafx.h"
#include "hardctrl.h"
#include "LeftAudit.h"
#include "MainFrm.h"
#include "ExamApp.h"
#include "ExamDetail.h"
#include "DlgRollingChoose.h"

IMPLEMENT_DYNCREATE(CLeftAudit, CDialog)
extern CExamApp m_app;
extern CExamDetail  m_examdeteil;
CLeftAudit::CLeftAudit(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CLeftAudit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeftAudit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
void CLeftAudit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftAudit)
	DDX_Control(pDX, IDC_GROUP, m_group);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CLeftAudit, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CLeftAudit)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_OUTBAR_NOTIFY, OnOutbarNotify)
END_MESSAGE_MAP()

LRESULT CLeftAudit::OnOutbarNotify(WPARAM wParam, LPARAM lParam)
{
    int nBarAction = (int)wParam;
	XT_OUTBAR_INFO* pOBInfo = (XT_OUTBAR_INFO*)lParam;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	if (nBarAction == OBN_XT_ITEMCLICK)
	{	
		DWORD data = m_wndOutlookBar.GetItemData(pOBInfo->nIndex);
		if(data == 1)
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_LINK);
		}
		if(data == 2)
		{
			if(m_app.Get_teach_count() == 0)
			{
				AfxMessageBox("没有选择教师机，无法进入此菜单！请返回“联机”菜单选择教师机！");
				return true;
			}
			fram->m_dlgMain.ShowDlg(IDD_FORM_FM);
		}
		if(data == 3)
		{
			fram->m_dlgMain.m_examdetail.SetKind(data);
			fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_DETAIL);

// 			if(m_examdeteil.nRoll_pattern == 2)
// 			{
// 				CString msg;
// 				msg.Format("本场考试已设定为红外点名模式，确定需要修改为蓝牙点名？");
// 				if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
// 				{
// 					m_examdeteil.nSetp = 1;
// 					m_examdeteil.nRoll_pattern = 1;
// 					fram->m_dlgMain.m_exammode.SetKind(data);
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 					return TRUE;
// 				}
// 				else
// 					return TRUE;
// 			}
// 			if(m_examdeteil.nRoll_pattern == 3)
// 			{
// 				CString msg;
// 				msg.Format("本场考试已设定为USB点名模式，确定需要修改为蓝牙点名？");
// 				if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
// 				{
// 					m_examdeteil.nSetp = 1;
// 					m_examdeteil.nRoll_pattern = 1;
// 					fram->m_dlgMain.m_exammode.SetKind(data);
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 					return TRUE;
// 				}
// 				else
// 					return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 0 || m_examdeteil.nRoll_pattern == 3)
// 			{
// 				CString msg;
// 				msg.Format("确定使用蓝牙点名进行本场考试？");
// 				if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
// 				{
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_MAIN);
// 					return TRUE;
// 				}
// 				m_examdeteil.nRoll_pattern = 1;
// 				m_examdeteil.nSetp = 1;
// 				fram->m_dlgMain.m_exammode.SetKind(data);
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 1)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 2)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 3)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_TIME);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 4)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 5)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_BLUE);
// 				return TRUE;
// 			}
		}
		if(data == 4)
		{

			CDlgRollingChoose dlg;
			if (dlg.DoModal() == IDOK)
			{
				if(dlg.m_rollkind == 101)
				{
					
				}
				else if (dlg.m_rollkind == 201)
				{
					fram->m_dlgMain.m_examdetail.SetKind(dlg.m_rollkind);
					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_DETAIL);
				}
				else
				{
					
				}
			}
			else
				fram->m_dlgMain.ShowDlg(IDD_FORM_MAIN);
// 			if(m_examdeteil.bIsRollDone)
// 			{
// 				AfxMessageBox("本场蓝牙模式已开考，无法切换到红外点名模式！");
// 				return true;
// 			}
// 			if(m_examdeteil.nRoll_pattern == 1)
// 			{
// 				CString msg;
// 				msg.Format("本场考试已设定为蓝牙点名模式，确定需要修改为红外点名？");
// 				if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
// 				{
// 					m_examdeteil.nRoll_pattern = 2;
// 					m_examdeteil.nSetp = 1;
// 					fram->m_dlgMain.m_exammode.SetKind(data);
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 					return TRUE;
// 				}
// 				else
// 					return TRUE;
// 			}
// 			if(m_examdeteil.nRoll_pattern == 3)
// 			{
// 				CString msg;
// 				msg.Format("本场考试已设定为USB点名模式，确定需要修改为红外点名？");
// 				if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
// 				{
// 					m_examdeteil.nRoll_pattern = 2;
// 					m_examdeteil.nSetp = 1;
// 					fram->m_dlgMain.m_exammode.SetKind(data);
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 					return TRUE;
// 				}
// 				else
// 					return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 0 || m_examdeteil.nRoll_pattern == 3)
// 			{
// 				CString msg;
// 				msg.Format("确定使用红外点名进行本场考试？");
// 				if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
// 				{
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_MAIN);
// 					return TRUE;
// 				}
// 				else
// 				{
// 					m_examdeteil.nRoll_pattern = 2;
// 					fram->m_dlgMain.m_exammode.SetKind(data);
// 					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 				}
// 			}
// 			if(m_examdeteil.nSetp == 1)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 2)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 3)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_TIME);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 4)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);
// 				return TRUE;
// 			}
// 			if(m_examdeteil.nSetp == 5)
// 			{
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_INFRAR);
// 				return TRUE;
// 			}
		}
		if(data == 5)
		{
			if(m_examdeteil.bIsRollDone)
			{
				AfxMessageBox("本场已开考，无法切换到USB点名模式！");
				return TRUE;
			}
			if(m_examdeteil.nSetp == 0 || m_examdeteil.nRoll_pattern == 1 || m_examdeteil.nRoll_pattern == 2)
			{
				CString msg;
				msg.Format("确定使用USB点名进行本场考试？");
				if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
				{
					fram->m_dlgMain.ShowDlg(IDD_FORM_MAIN);
					return TRUE;
				}
				else
				{
					m_examdeteil.nRoll_pattern = 3;
					fram->m_dlgMain.m_exammode.SetKind(data);
					fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
				}
			}
			if(m_examdeteil.nSetp == 2)
			{
				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);
				return TRUE;
			}
			if(m_examdeteil.nSetp == 3)
			{
				fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);
				return TRUE;
			}
			if(m_examdeteil.nSetp == 4)
			{
				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_INFRAR);
				return TRUE;
			}
		}
		if(data == 6)
		{

			fram->m_dlgMain.m_examblue.SetKind(data);
			fram->m_dlgMain.m_examblue.SetInfo();
			fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_BLUE);
// 			if(m_examdeteil.bIsRollDone)
// 			{
// 				fram->m_dlgMain.m_examblue.SetKind(data);
// 				fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_BLUE);
// 			}
// 			else
// 				AfxMessageBox("学生没有开考，无法进入收卷模式！");
		}
		if (data == 7)
		{
			fram->m_dlgMain.m_examdetail.SetKind(data);
			fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_DETAIL);	
		}
		if (data == 9)
		{
			fram->m_dlgMain.m_examinfoback.SetKind(data);
			fram->m_dlgMain.ShowDlg(IDD_FORM_EXMA_INFO_BACK);
		}
		if (data == 10)
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_IMPORT);
		}
		if(data == 12)
		{
			if(m_examdeteil.bIsRollingDone)
			{
				fram->m_dlgMain.m_import.SetKind(data);
				fram->m_dlgMain.m_import.SetInfo();
				fram->m_dlgMain.ShowDlg(IDD_FORM_IMPORT);
			}
			else
			{
				AfxMessageBox("本场考试没有结束，无法对本场考试备份！");
			}
			
		}
		if(data == 14)
		{
			fram->m_dlgMain.m_import.SetKind(data);
			fram->m_dlgMain.ShowDlg(IDD_FORM_IMPORT);
		}
		if(data == 15)
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_IMAGE_READ);
		}
		if(data == 16)
		{
			fram->m_dlgMain.ShowDlg(IDD_FORM_IMAGE);
		}
	}
	return TRUE;
}

void CLeftAudit::OnSize(UINT nType, int cx, int cy) 
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
			rect1.bottom=lpRect.bottom - lpRect.top-5;
     		m_wndOutlookBar.MoveWindow(rect1);
		}
	}	
}

BOOL CLeftAudit::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	
	if (!m_wndOutlookBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0),this, NULL))
	{
		AfxMessageBox("Failed to create outlook bar.");
		return FALSE;
	}
	m_ImageSmall.Create (16, 16, ILC_COLOR16|ILC_MASK, 2, 1);
	m_ImageLarge.Create (32, 32, ILC_COLOR16|ILC_MASK, 2, 1);
// 	for (int i = 0; i < 20; ++i)
// 	{
// 		HICON hIcon;
// 		switch(i)
// 		{
// 		case 1:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_LINK);
// 			break;
// 		case 2:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_RADIO);
// 			break;
// 		case 3:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_ALL);
// 			break;
// 		case 4:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_SINGLE);
// 			break;
// 		case 5:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_ROOLING);
// 			break;
// 		case 6:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_SPESH);
// 			break;
// 		case 7:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_SINGLE);
// 			break;
// 		case 8:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_SINGLE);
// 			break;
// 		case 9:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_IMPORT);
// 			break;
// 		case 10:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_DEPART);
// 			break;
// 		case 11:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_PP);
// 			break;
// 		case 12:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_ANSWER);
// 			break;
// 		case 13:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_TEACH);
// 			break;			
// 		case 14:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_PAPER);
// 			break;		
// 		case 15:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_SUB);
// 			break;		
// 		case 16:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_WEB);
// 			break;
// 			
// 		default:
// 			hIcon = AfxGetApp()->LoadIcon(IDI_RADIO);
// 			break;
// 		}
// 		ASSERT(hIcon);
// 		m_ImageSmall.Add(hIcon);
// 		m_ImageLarge.Add(hIcon);
// 	}

	for (int i = 0; i < 16; ++i)
	{
		HICON hIcon = AfxGetApp()->LoadIcon(IDI_CTRL_1);
		ASSERT(hIcon);
		m_ImageSmall.Add(hIcon);
		m_ImageLarge.Add(hIcon);
	}
    int iFolder; // index of the added folder

	m_wndOutlookBar.SetImageList(&m_ImageLarge, OBS_XT_LARGEICON);
	m_wndOutlookBar.SetImageList(&m_ImageSmall, OBS_XT_SMALLICON);
//	m_wndOutlookBar.SetBackColor(RGB(0x26,0x62,0xbb));

	m_wndOutlookBar.SetBackColor(RGB(0x3a,0x6e,0xa5));
	m_wndOutlookBar.SetTextColor(RGB(0xff,0xff,0xff));
	
	iFolder = m_wndOutlookBar.AddFolder(_T("FM收音机"), 0);
	DWORD id = 0;
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("搜索教师机"), 1, 1);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("FM收音机"), 2, 2);
	
	iFolder = m_wndOutlookBar.AddFolder(_T("点名"), 1);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("蓝牙点名"), 3, 3);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("红外点名"), 4, 4);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("USB点名"), 5, 5);

	iFolder = m_wndOutlookBar.AddFolder(_T("收卷"), 2);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("蓝牙收卷"), 6, 6);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("红外收卷"), 7, 7);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("USB收卷"), 8, 8);
//	m_wndOutlookBar.InsertItem(iFolder, 3, _T("考试备份"), 12, 12);

	iFolder = m_wndOutlookBar.AddFolder(_T("试卷生成"), 3);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("生成试卷文件"), 9, 9);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("导入试卷文件"), 10, 10);

//	iFolder = m_wndOutlookBar.AddFolder(_T("特殊情况"), 3);
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("迟到"), 9, 9);
//	m_wndOutlookBar.InsertItem(iFolder, 1, _T("提前交卷"), 10, 10);
//	m_wndOutlookBar.InsertItem(iFolder, 2, _T("设备更换"), 11, 11);	


/*	iFolder = m_wndOutlookBar.AddFolder(_T("脱机操作"), 4);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("脱机点名"), 12, 12);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("脱机收卷"), 13, 13);*/

//	iFolder = m_wndOutlookBar.AddFolder(_T("信息备份"), 5);
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("信息导入"), 14, 14);
//	m_wndOutlookBar.InsertItem(iFolder, 1, _T("信息导出"), 15, 15);

//	iFolder = m_wndOutlookBar.AddFolder(_T("图像操作"), 6);
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("图像采集"), 16, 16);
//	m_wndOutlookBar.InsertItem(iFolder, 1, _T("试卷批阅"), 15, 15);

	m_wndOutlookBar.SetOwner(this);
	m_wndOutlookBar.SetSelFolder(0);
	m_wndOutlookBar.ShowWindow( SW_SHOW );
	m_group.ShowWindow(SW_HIDE);
	return TRUE;  
}

void CLeftAudit::OnOK() 
{	
}

void CLeftAudit::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem; 
		dcMem.CreateCompatibleDC(&dc); 
		BITMAP bitMap;
		m_bmpBackground.GetBitmap(&bitMap);
		CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);
	}
}
