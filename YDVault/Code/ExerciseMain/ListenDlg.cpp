// ListenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "ListenDlg.h"
#include "ExerciseMain.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../Base/AutoClean.h"
#include "wmpcontrols.h"


// CListenDlg dialog

IMPLEMENT_DYNAMIC(CListenDlg, CQuestionDlg)

CListenDlg::CListenDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CListenDlg::IDD, pParent)
{

}

CListenDlg::~CListenDlg()
{
}

void CListenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CListenDlg, CQuestionDlg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CListenDlg message handlers
BOOL CListenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rectBkMediaPlay;
	GetDlgItem(IDC_STATIC_BK_MEDIAPLAY)->GetWindowRect(&rectBkMediaPlay);
	ScreenToClient(&rectBkMediaPlay);
	GetDlgItem(IDC_STATIC_BK_MEDIAPLAY)->ShowWindow(SW_HIDE);
	m_WMPlay.Create(_T(""),WS_CHILD|WS_VISIBLE,rectBkMediaPlay,this,IDC_MEDIA_CREATE);

	m_Area.Create(CDlgQuestionArea::IDD, this);
	CRect rect, rectSelf;
	CWnd* pTmpWnd = GetDlgItem(IDC_STATIC_QUESTION);
	pTmpWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_Area.GetWindowRect(&rectSelf);
	m_Area.MoveWindow(rect.left, rect.top, rectSelf.Width(), rectSelf.Height());
	m_Area.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HRESULT CListenDlg::PersistData()
{
	m_pRecord->m_listUserAnswers.clear();
	m_Area.GetStuAnswers(&(m_pRecord->m_listUserAnswers));	
	return S_OK;
}
void CListenDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CQuestionDlg::OnShowWindow(bShow, nStatus);
	HRESULT hr = E_FAIL;
	if (bShow)
	{
		m_Area.m_pRecord = m_pRecord;
		//m_Area.ShowWindow(SW_SHOW);
		m_Area.ShowQuestion();
		hr = PlayMedia();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		UpdateData(FALSE);
	}
	else
	{
		hr = PauseMedia();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
}

HRESULT CListenDlg::PlayMedia()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pRecord);
	CYDQuestionRef* pQRef = dynamic_cast<CYDQuestionRef*>(m_pRecord->m_pQuestion);
	ASSERT(pQRef);
	std::list<CYDMediaRef*> lstMedia;
	hr = pQRef->GetAllMedia(&lstMedia);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CYDMediaRef> clr(lstMedia);
	if(lstMedia.size() <= 0)
	{
		AfxMessageBox(_T("当前选中的题目没有多媒体资料，无法播放!"));
		return S_FALSE;
	}
	CWMPControls WMPControls = m_WMPlay.GetControls();
	WMPControls.stop();
	CFtpRef* pFtpRef = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
	}
	ASSERT(pFtpRef);
	for(std::list<CYDMediaRef*>::const_iterator itr = lstMedia.begin();
		itr != lstMedia.end();++itr)
	{
		CString strVaultName;
		hr = (*itr)->CreateVaultName(strVaultName);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strLabel;
		hr = (*itr)->GetLabel(&strLabel);
		if(FAILED(hr))
		{
			return hr;
		}
		TCHAR   szPath[MAX_PATH*MAX_PATH] = _T("\0"); 
		GetTempPath(MAX_PATH*MAX_PATH,   szPath); 

		CString strLocal = szPath;
		strLocal += strLabel;
		hr = pFtpRef->DownLoad(strLocal, strVaultName);
		if (FAILED(hr))
		{
			CString strFormat = L"听力文件<%s>下载失败!";
			CString strInfo;
			strInfo.Format(strFormat, strLocal);
			AfxMessageBox(strInfo);
			return S_FALSE;
		}
		m_WMPlay.SetUrl(strLocal);
	}
	WMPControls.play();
	return S_OK;
}

HRESULT CListenDlg::PauseMedia()
{
	HRESULT hr = E_FAIL;
	CWMPControls WMPControls = m_WMPlay.GetControls();
	WMPControls.pause();
	return S_OK;
}
HRESULT CListenDlg::StopMedia()
{
	HRESULT hr = E_FAIL;
	CWMPControls WMPControls = m_WMPlay.GetControls();
	WMPControls.stop();
	return S_OK;
}