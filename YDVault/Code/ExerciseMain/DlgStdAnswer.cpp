// DlgStdAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgStdAnswer.h"
#include "../ObjRef\YDQuestionType.h"
#include "../Base\AutoClean.h"
#include "wmpcontrols.h"


IMPLEMENT_DYNAMIC(CStdAnswerBCGPGridCtrl,CBCGPKeyGridCtrl)
CStdAnswerBCGPGridCtrl::CStdAnswerBCGPGridCtrl(void)
{
}

CStdAnswerBCGPGridCtrl::~CStdAnswerBCGPGridCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CStdAnswerBCGPGridCtrl, CBCGPKeyGridCtrl)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
void CStdAnswerBCGPGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CBCGPGridCtrl::OnLButtonDblClk(nFlags,point);
	CBCGPGridRow* pGridRow =  GetCurSel();
	if(pGridRow == NULL)
	{
		return;
	}
	CYDQuestionRef* pRef = (CYDQuestionRef*)pGridRow->GetData();
	if (pRef != NULL)
	{
		CStdAnswerDetialDlg dlg;
		dlg.m_pQuestionRef = pRef;
		dlg.DoModal();
	}
}

// CDlgStdAnswer dialog

const int cColQuestionType = 0;//题型
const int cColQuestionNo   = 1;//题号
const int cColQuestionAnswer = 2;//答案
const int cColQuestionMultiMedia = 3;//多媒体


IMPLEMENT_DYNAMIC(CDlgStdAnswer, CDialog)

CDlgStdAnswer::CDlgStdAnswer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStdAnswer::IDD, pParent)
{
	m_pListQuestion = NULL;
}

CDlgStdAnswer::~CDlgStdAnswer()
{
}

void CDlgStdAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStdAnswer, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CDlgStdAnswer::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDOK, &CDlgStdAnswer::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgStdAnswer message handlers

BOOL CDlgStdAnswer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	ScreenToClient(&rect);	
	if(!m_listGrid.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE,rect,this,IDC_GRID_CREATE))
	{
		TRACE0("Failed to m_listctrlDR.Create!\n");
		return FALSE;
	}
	m_listGrid.EnableHeader(TRUE,0);
	m_listGrid.EnableDragHeaderItems(FALSE);
	m_listGrid.SetWholeRowSel(TRUE);
	m_listGrid.SetReadOnly();

	GetDlgItem(IDC_STATIC_BK_MEDIA)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BK_MEDIA)->ShowWindow(SW_HIDE);
	ScreenToClient(&rect);	
	m_WMPlay.Create(_T(""),WS_CHILD|WS_VISIBLE,rect,this,IDC_MEDIA_CREATE);
	//m_WMPlay.CreateControl(viewer_desc.viewer_progid,_T(""),WS_CHILD|WS_VISIBLE,rect,this,2);


	m_listGrid.InsertColumn(cColQuestionType,_T("题型"),100);
	m_listGrid.InsertColumn(cColQuestionNo,_T("题号"),120);
	m_listGrid.InsertColumn(cColQuestionAnswer,_T("答案"),200);
	m_listGrid.InsertColumn(cColQuestionMultiMedia,_T("多媒体"),80);
	
	ASSERT(m_pListQuestion);
	HRESULT hr = E_FAIL;
	int typeId = 0;
	int index = 1;
	CBCGPGridRow* pParentRow = NULL;
	for (CLISTQUESTION::const_iterator itr = m_pListQuestion->begin();
		 itr != m_pListQuestion->end(); ++itr,index++)
	{
		CYDQuestionRef* pQuestion = *itr;
		long id = 0;
		hr = pQuestion->GetPropVal(L"TYPEID", &id);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		if (typeId != id)
		{
			pParentRow = NULL;
			typeId = id;
		}
		CBCGPGridRow* pInsertRow = NULL;
		hr = InsertList(*itr,pParentRow,pInsertRow);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		if(pParentRow == NULL)
		{
			pParentRow = pInsertRow;
		}
	}
	for(int i= 0; i < m_listGrid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_listGrid.GetRow(i);
		ASSERT(pRow);
		if(i%2 == 0)
		{
			if(pRow->GetParent() == NULL)
			{
				pRow->GetItem(cColQuestionType)->SetBackgroundColor(RGB(110,180,200));
			}
			pRow->GetItem(cColQuestionNo)->SetBackgroundColor(RGB(110,180,200));
			pRow->GetItem(cColQuestionAnswer)->SetBackgroundColor(RGB(110,180,200));
		}
	}
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgStdAnswer::InsertList(CYDQuestionRef* _pQuestionRef,
								  CBCGPGridRow* _pParentRow,
								  CBCGPGridRow*& _pInsertRow)
{
	HRESULT hr = E_FAIL;
	_pInsertRow = m_listGrid.CreateRow(m_listGrid.GetColumnCount());
	_pInsertRow->AllowSubItems();
	if(_pParentRow == NULL)
	{
		//根节点
		CYDQuestionType qtype(theApp.m_pDatabase);
		long id = 0;
		hr = _pQuestionRef->GetPropVal(L"TYPEID", &id);
		if(FAILED(hr))
		{
			return hr;
		}
		qtype.SetID(id);
		CString strType;
		qtype.GetPropVal(L"name", strType);
		_pInsertRow->GetItem(cColQuestionType)->SetValue(_variant_t(strType));
		m_listGrid.AddRow(_pInsertRow);
	}
	else
	{
		_pParentRow->AddSubItem(_pInsertRow);
	}
	CString strIndex;
	strIndex.Format(L"第%d题", m_listGrid.GetRowCount());
	_pInsertRow->GetItem(cColQuestionNo)->SetValue(_variant_t(strIndex));
	CString strStdAnswer;
	hr = _pQuestionRef->GetStandardAnswer(strStdAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	_pInsertRow->GetItem(cColQuestionAnswer)->SetValue(_variant_t(strStdAnswer));
	std::list<CYDMediaRef*> lstMedia;
	hr = _pQuestionRef->GetAllMedia(&lstMedia);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CYDMediaRef> clr(lstMedia);
	CString strMedia;
	BOOL bFirst = TRUE;
	for(std::list<CYDMediaRef*>::const_iterator itr = lstMedia.begin();
		itr != lstMedia.end();++itr)
	{
		CString str;
		hr = (*itr)->GetLabel(&str);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bFirst)
		{
			bFirst = FALSE;
		}
		else
		{
			strMedia += _T(";");
		}
		strMedia += str;
	}
	_pInsertRow->GetItem(cColQuestionMultiMedia)->SetValue(_variant_t(strMedia));
	_pInsertRow->SetData(DWORD_PTR(_pQuestionRef));
	return S_OK;
}

void CDlgStdAnswer::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pRow = m_listGrid.GetCurSel();
	if(pRow == NULL)
	{
		AfxMessageBox(_T("请选择一条要播放的题号！"));
		return;
	}
	HRESULT hr = E_FAIL;
	
	ASSERT(pRow);
	CYDQuestionRef* pQRef = (CYDQuestionRef*)pRow->GetData();
	ASSERT(pQRef);
	std::list<CYDMediaRef*> lstMedia;
	hr = pQRef->GetAllMedia(&lstMedia);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CListAutoClean<CYDMediaRef> clr(lstMedia);
	if(lstMedia.size() <= 0)
	{
		AfxMessageBox(_T("当前选中的题目没有多媒体资料，无法播放!"));
		return;
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
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CString strLabel;
		hr = (*itr)->GetLabel(&strLabel);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
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
			return ;
		}
		m_WMPlay.SetUrl(strLocal);
	}
	WMPControls.play();
}

void CDlgStdAnswer::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
