#include "stdafx.h"
#include "hardctrl.h"
#include "FormFM.h"
#include "ExamApp.h"
#include "MainFrm.h"
#include "DlgFM.h"

extern CHardCtrlApp theApp;
extern CExamApp m_app;

IMPLEMENT_DYNCREATE(CFormFM, CDialog)


CFormFM::CFormFM(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormFM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormFM)
	//}}AFX_DATA_INIT
}


void CFormFM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormFM)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_OPEN, m_open);
	DDX_Control(pDX, IDC_OP, m_op);
	DDX_Control(pDX, IDC_DOWN, m_down);
	DDX_Control(pDX, IDC_CLOSE, m_close);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormFM, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormFM)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_OP, OnOp)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormFM message handlers

void CFormFM::OnOpen() 
{
	m_app.OpenFM();	

	Sleep(500);
	CString strFM = m_app.GetFm();
	if(strFM != "")
		GetDlgItem(IDC_STATIC2)->SetWindowText(strFM);

}

void CFormFM::OnSearch() 
{
	m_app.SearchFM();	
}

void CFormFM::OnClose() 
{
	m_app.CloseFM();
}

void CFormFM::OnUp() 
{
	m_app.UpFM();
	Sleep(500);
	CString strFM = m_app.GetFm();
	if(strFM != "")
		GetDlgItem(IDC_STATIC2)->SetWindowText(formatFM(strFM));
}

void CFormFM::OnDown() 
{
	m_app.DownFM();	
	Sleep(500);
	CString strFM = m_app.GetFm();
	if(strFM != "")
		GetDlgItem(IDC_STATIC2)->SetWindowText(formatFM(strFM));
}

void CFormFM::OnOp() 
{
	CDlgFM dlg;
	if(dlg.DoModal() == IDOK ) 
	{
		CString strFM = dlg.m_channel;
		if(decodeFM(strFM))
		{
			GetDlgItem(IDC_STATIC2)->SetWindowText(formatFM(strFM));
			m_app.SetFM(strFM);
		}
		else
		{
			AfxMessageBox("您输入的频段有误，请正确输入！");
		}
	}
}

BOOL CFormFM::decodeFM(CString &strFM)
{
	strFM.Remove('.');

	BOOL bIsDigit = TRUE;
	int nCount = strFM.GetLength(); // 获得字符个数
	if(nCount > 4)
		return FALSE;
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( 0 == isdigit( strFM.GetAt(i) ) ) // 不是数字就置标志位
		{
				 bIsDigit = FALSE;
				 break;   // 退出
		}
	 }
	
	return bIsDigit;
}
CString CFormFM::formatFM(CString strFM)
{
	int iLen = strFM.GetLength();
	strFM.Insert(iLen-1,'.');
	return strFM;
}
CString CFormFM::GetExePath()
{
	CString file;
	char process[MAX_PATH];
	GetModuleFileName(NULL, process, MAX_PATH);
    for ( int i = strlen(process);i>0 ;i --)
	if ( process[i] =='\\')
	{
		process[i+1]='\0';
		break;
	}
	file.Format(process);
	return file;
}

void CFormFM::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

}

BOOL CFormFM::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	CRect btnRect;  
	m_open.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_open.SetBitmapId(IDB_OPENFM,IDB_OPENFM,IDB_OPENFM_DOWN,IDB_OPENFM); 

	m_close.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_close.SetBitmapId(IDB_CLOSEFM,IDB_CLOSEFM,IDB_CLOSEFM_DOWN,IDB_CLOSEFM); 

	m_search.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_search.SetBitmapId(IDB_SEARCHFM,IDB_SEARCHFM,IDB_SEARCHFM_DOWN,IDB_SEARCHFM); 

	m_up.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_up.SetBitmapId(IDB_UPFM,IDB_UPFM,IDB_UPFM_DOWN,IDB_UPFM);

	m_down.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_down.SetBitmapId(IDB_DOWNFM,IDB_DOWNFM,IDB_DOWNFM_DOWN,IDB_DOWNFM);

	m_op.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_op.SetBitmapId(IDB_INPUTFM,IDB_INPUTFM,IDB_INPUTFM_DOWN,IDB_INPUTFM);


	m_Static1.SubclassDlgItem(IDC_STATIC3,this);
	m_Static1.SetCaption("频段：");
	m_Static1.SetBackColor(RGB(0,0,0));


	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_OPEN),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_CLOSE),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_DOWN),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_OP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRelative,mdRelative);
//	m_Static2.SubclassDlgItem(IDC_STATIC1,this);
//	m_Static2.SetCaption("FM：");
//	m_Static2.SetBackColor(RGB(0,0,0));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
