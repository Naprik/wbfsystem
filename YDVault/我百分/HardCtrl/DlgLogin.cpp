#include "stdafx.h"
#include "hardctrl.h"
#include "DlgLogin.h"
#include "DBLogin.h"

extern CHardCtrlApp theApp;
//IMPLEMENT_DYNCREATE(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/) : CDialog(CDlgLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLogin)
	m_pass = _T("admin");
	m_user = _T("admin");
	//}}AFX_DATA_INIT
}
void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLogin)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_PASS, m_pass);
	DDX_Text(pDX, IDC_USER, m_user);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDOK, m_logButtons);
//	DDX_Control(pDX, IDCANCEL, m_outButtons);
}
BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	//{{AFX_MSG_MAP(CDlgLogin)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CDlgLogin::OnOK() 
{
	UpdateData(TRUE);
// 	CoInitialize(NULL);
// 	CDBLogin db;									//ͬ�����ݿ��豸��
// 	if ( !db.Connect())
// 	{
// 		CoUninitialize();
// 		AfxMessageBox("�޷����ӵ����ݿ������,��͹���Ա��ϵ��");
// 		CDialog::OnOK();
// 	}	
// 	int flag = db.LoadByAll();
// 	while(flag)
// 	{
// 		if(db.m_user == m_user && db.m_pass == m_pass)
// 		{
// 			db.Close();
// 			CoUninitialize();
// 			CDialog::OnOK();
// 			return;
// 		}
// 		flag = db.Move();
// 	}
// 	db.Close();
// 	CoUninitialize();
	AfxMessageBox("��¼ʧ�ܣ������û��������룡");
	

//	CDialog::OnOK();

} 
BOOL CDlgLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ModifyStyle( WS_CAPTION, WS_MINIMIZEBOX, SWP_DRAWFRAME );
	m_bmpBackground.LoadBitmap(IDB_BACK);

	CRect btnRect; 
	m_ok.GetWindowRect(btnRect); //��ȡ��ť���ھ�������
	ScreenToClient(btnRect); //ת���ɿͻ�����
	m_ok.SetBackBmp(IDB_BACK,btnRect); //���ð�ť�ı���ͼƬ���������ڵı���ͼƬһ��
//	m_ok.SetRgnMask(IDB_BN_BACK,TRUE);        //������Ӧ����,TRUE��ʾʹ������
//	m_ok.SetMaskBitmapId(IDB_BN_BACK,TRUE);   //��������ͼƬ
	m_ok.SetBitmapId(IDB_LOGIN1,IDB_LOGIN1,IDB_LOGIN_DOWN1,IDB_LOGIN1); //���ð�ť������״̬ͼ
	

	m_cancel.GetWindowRect(btnRect); //��ȡ��ť���ھ�������
	ScreenToClient(btnRect); //ת���ɿͻ�����
	m_cancel.SetBackBmp(IDB_BACK,btnRect); //���ð�ť�ı���ͼƬ���������ڵı���ͼƬһ��
//	m_cancel.SetRgnMask(IDB_BN_BACK,TRUE);        //������Ӧ����,TRUE��ʾʹ������
//	m_cancel.SetMaskBitmapId(IDB_BN_BACK,TRUE);   //��������ͼƬ
	m_cancel.SetBitmapId(IDB_EXIT1,IDB_EXIT1,IDB_EXIT_DOWN1,IDB_EXIT1); //���ð�ť������״̬ͼ
	
	
	ShowWindow(SW_SHOWMAXIMIZED);

/*	m_Static1.SubclassDlgItem(IDC_TEXT5,this);
	m_Static1.SetCaption("�û�����");
	m_Static1.SetBackColor(RGB(0,0,0));

	m_Static2.SubclassDlgItem(IDC_TEXT,this);
	m_Static2.SetCaption("�� �룺");
	m_Static2.SetBackColor(RGB(0,0,0));*/
	return FALSE;  
}

CString CDlgLogin::GetExePath()
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

void CDlgLogin::OnPaint() 
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

void CDlgLogin::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDlgLogin::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
