#include "stdafx.h"
#include "FormFM.h"
#include "../YDcom/ExamApp.h"
#include "DlgFM.h"


IMPLEMENT_DYNCREATE(CFormFM, CYdFormView)


CFormFM::CFormFM()
	: CYdFormView(CFormFM::IDD)
	, m_strFrequency(_T(""))
{
	//{{AFX_DATA_INIT(CFormFM)
	//}}AFX_DATA_INIT
}


void CFormFM::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormFM)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_OPEN, m_open);
	DDX_Control(pDX, IDC_OP, m_op);
	DDX_Control(pDX, IDC_DOWN, m_down);
	DDX_Control(pDX, IDC_CLOSE, m_close);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_FREQUENCY, m_strFrequency);
}


BEGIN_MESSAGE_MAP(CFormFM, CYdFormView)
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
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		hr = (*itr)->OpenFM();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		Sleep(500);
		CString strFM ;
		hr = (*itr)->GetFM(strFM);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		m_strFrequency = strFM;
	}
	UpdateData(FALSE);
}

void CFormFM::OnSearch() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		hr = (*itr)->SearchFM();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
}

void CFormFM::OnClose() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		hr = (*itr)->CloseFM();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
	m_strFrequency = _T("");
	UpdateData(FALSE);
}

void CFormFM::OnUp() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		hr = (*itr)->UpFM();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		Sleep(500);
		CString strFM;
		hr = (*itr)->GetFM(strFM);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		m_strFrequency = strFM;
	}
	UpdateData(FALSE);
}

void CFormFM::OnDown() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		hr = (*itr)->DownFM();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		Sleep(500);
		CString strFM;
		hr = (*itr)->GetFM(strFM);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		m_strFrequency = strFM;
	}
	UpdateData(FALSE);
}

void CFormFM::OnOp() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	CDlgFM dlg;
	if(dlg.DoModal() == IDOK ) 
	{
		CString strFM = dlg.m_channel;
		if(decodeFM(strFM))
		{
			m_strFrequency = formatFM(strFM);
			UpdateData(FALSE);
			for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
				itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
			{
				hr = (*itr)->SetFM(strFM);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
			}
		}
		else
		{
			AfxMessageBox(_T("您输入的频段有误，请正确输入！"));
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
	CString file = (const TCHAR*)_ModulePath;
	return file;
}

void CFormFM::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

}

void CFormFM::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	
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
	m_Static1.SetCaption(_T("频段："));
	m_Static1.SetBackColor(RGB(0,0,0));


// 	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_OPEN),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_CLOSE),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_DOWN),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_OP),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRelative,mdRelative);
//	m_Static2.SubclassDlgItem(IDC_STATIC1,this);
//	m_Static2.SetCaption("FM：");
//	m_Static2.SetBackColor(RGB(0,0,0));
	return ;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CFormFM::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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