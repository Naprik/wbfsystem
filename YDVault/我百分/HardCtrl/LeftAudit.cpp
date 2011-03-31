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
				AfxMessageBox("û��ѡ���ʦ�����޷�����˲˵����뷵�ء��������˵�ѡ���ʦ����");
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
// 				msg.Format("�����������趨Ϊ�������ģʽ��ȷ����Ҫ�޸�Ϊ����������");
// 				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) == IDOK)
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
// 				msg.Format("�����������趨ΪUSB����ģʽ��ȷ����Ҫ�޸�Ϊ����������");
// 				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) == IDOK)
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
// 				msg.Format("ȷ��ʹ�������������б������ԣ�");
// 				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
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
// 				AfxMessageBox("��������ģʽ�ѿ������޷��л����������ģʽ��");
// 				return true;
// 			}
// 			if(m_examdeteil.nRoll_pattern == 1)
// 			{
// 				CString msg;
// 				msg.Format("�����������趨Ϊ��������ģʽ��ȷ����Ҫ�޸�Ϊ���������");
// 				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) == IDOK)
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
// 				msg.Format("�����������趨ΪUSB����ģʽ��ȷ����Ҫ�޸�Ϊ���������");
// 				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) == IDOK)
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
// 				msg.Format("ȷ��ʹ�ú���������б������ԣ�");
// 				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
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
				AfxMessageBox("�����ѿ������޷��л���USB����ģʽ��");
				return TRUE;
			}
			if(m_examdeteil.nSetp == 0 || m_examdeteil.nRoll_pattern == 1 || m_examdeteil.nRoll_pattern == 2)
			{
				CString msg;
				msg.Format("ȷ��ʹ��USB�������б������ԣ�");
				if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
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
// 				AfxMessageBox("ѧ��û�п������޷������վ�ģʽ��");
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
				AfxMessageBox("��������û�н������޷��Ա������Ա��ݣ�");
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
	
	iFolder = m_wndOutlookBar.AddFolder(_T("FM������"), 0);
	DWORD id = 0;
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("������ʦ��"), 1, 1);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("FM������"), 2, 2);
	
	iFolder = m_wndOutlookBar.AddFolder(_T("����"), 1);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("��������"), 3, 3);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("�������"), 4, 4);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("USB����"), 5, 5);

	iFolder = m_wndOutlookBar.AddFolder(_T("�վ�"), 2);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("�����վ�"), 6, 6);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("�����վ�"), 7, 7);
	m_wndOutlookBar.InsertItem(iFolder, 2, _T("USB�վ�"), 8, 8);
//	m_wndOutlookBar.InsertItem(iFolder, 3, _T("���Ա���"), 12, 12);

	iFolder = m_wndOutlookBar.AddFolder(_T("�Ծ�����"), 3);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("�����Ծ��ļ�"), 9, 9);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("�����Ծ��ļ�"), 10, 10);

//	iFolder = m_wndOutlookBar.AddFolder(_T("�������"), 3);
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("�ٵ�"), 9, 9);
//	m_wndOutlookBar.InsertItem(iFolder, 1, _T("��ǰ����"), 10, 10);
//	m_wndOutlookBar.InsertItem(iFolder, 2, _T("�豸����"), 11, 11);	


/*	iFolder = m_wndOutlookBar.AddFolder(_T("�ѻ�����"), 4);
	m_wndOutlookBar.InsertItem(iFolder, 0, _T("�ѻ�����"), 12, 12);
	m_wndOutlookBar.InsertItem(iFolder, 1, _T("�ѻ��վ�"), 13, 13);*/

//	iFolder = m_wndOutlookBar.AddFolder(_T("��Ϣ����"), 5);
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("��Ϣ����"), 14, 14);
//	m_wndOutlookBar.InsertItem(iFolder, 1, _T("��Ϣ����"), 15, 15);

//	iFolder = m_wndOutlookBar.AddFolder(_T("ͼ�����"), 6);
//	m_wndOutlookBar.InsertItem(iFolder, 0, _T("ͼ��ɼ�"), 16, 16);
//	m_wndOutlookBar.InsertItem(iFolder, 1, _T("�Ծ�����"), 15, 15);

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
