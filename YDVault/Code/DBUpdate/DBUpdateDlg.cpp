
// DBUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBUpdate.h"
#include "DBUpdateDlg.h"
#include "afxdialogex.h"
#include "DBUpdateFromXml.h"
#include "../DBBase/DatabaseEx.h"
#include "../DBBase/DBTransactionRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDBUpdateDlg dialog




CDBUpdateDlg::CDBUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBUpdateDlg::IDD, pParent)
{
	m_pDatabase= NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDBUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTENT, m_lstContent);
}

BEGIN_MESSAGE_MAP(CDBUpdateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_DB, &CDBUpdateDlg::OnBnClickedButtonUpdateDb)
END_MESSAGE_MAP()


// CDBUpdateDlg message handlers

BOOL CDBUpdateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_lstContent.SetHorizontalExtent(MAX_PATH*2);//设置水平滚动条 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDBUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDBUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDBUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDBUpdateDlg::OnBnClickedButtonUpdateDb()
{
	// TODO: Add your control notification handler code here
	//所有的升级数据库都在../cfg/UpdateDB.xml中
	CWaitCursor Wait;
	m_lstContent.ResetContent();
	m_lstContent.AddString(_T("开始升级数据库！\n"));
	CDBUpdateFromXml DBUpdateFromXml;
	HRESULT hr = E_FAIL;
	hr = DBUpdateFromXml.Load();
	if(FAILED(hr))
	{
		return;
	}
	int iMaxHorizontalExt = 0;
	CString strMaxResult;
	for(std::list<std::pair<CString,CString> >::const_iterator itr = DBUpdateFromXml.m_lstUpdateSQL.begin();
		itr != DBUpdateFromXml.m_lstUpdateSQL.end();++itr)
	{
		m_lstContent.AddString(_T("开始执行"));
		m_lstContent.AddString((*itr).first);
		CString strResult;
		strResult.Format(_T("执行%s"),(*itr).second);
		CString strError;
		if(ExecuteSQL((*itr).second,strError))
		{
			strResult += _T("成功！");
		}
		else
		{
			strResult += _T("失败,错误描述：");
			strResult += strError;
		}
		if(strResult.GetLength() > iMaxHorizontalExt)
		{
			iMaxHorizontalExt = strResult.GetLength();
			strMaxResult = strResult;
		}
		m_lstContent.AddString(strResult);
	}
	m_lstContent.AddString(_T("升级结束！\n"));
	CPaintDC   dc(this); 
	CSize   sz; 
	sz=dc.GetTextExtent(strMaxResult); 
	m_lstContent.SetHorizontalExtent(sz.cx);
}


BOOL CDBUpdateDlg::ExecuteSQL(CString _strSQL,CString &_strError)
{
	try
	{
		HRESULT hr = E_FAIL;
		CDBTransactionRef TransRef(m_pDatabase,TRUE);
		hr = m_pDatabase->InitializeSQL(_bstr_t(_strSQL));
		if(FAILED(hr))
		{
			return FALSE;
		}
		hr = m_pDatabase->ExecuteSQL();
		if(FAILED(hr))
		{
			return FALSE;
		}	
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			return FALSE;
		}
	}
	catch (_com_error &e)
	{
		_strError = (const TCHAR*)e.Description();
		return FALSE;
		
	}
	catch (...) 
	{
		_strError = _T("未知的异常");
		return FALSE;
	}
	return TRUE;
}