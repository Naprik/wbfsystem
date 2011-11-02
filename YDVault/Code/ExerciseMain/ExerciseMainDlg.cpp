// ExerciseMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "ExerciseMainDlg.h"
#include "../UIBase/PdemWait.h"
#include "../ObjRef/YDQuestionFactory.h"
#include "../ObjRef/YDQuestionType.h"
#include "ExerciseSelectQuestionCfgMgr.h"
#include "../Base/AutoClean.h"
#include "DlgStdAnswer.h"
#include "DlgStudentAnswer.h"
#include "../DBBase/DBTransactionRef.h"
#include "../Base/DataHandler.h"
#include "DlgExsiceMark.h"
#include "DlgReName.h"
#include "StudentLevelUpdateUtil.h"
#include <WinUser.h>

#include "../YDUIUserManagement/StaticYdUser.h"
#include "../ObjRef/YDUserRef.h"
// CExerciseMainDlg dialog

IMPLEMENT_DYNAMIC(CExerciseMainDlg, CDialog)

CExerciseMainDlg::CExerciseMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExerciseMainDlg::IDD, pParent)
{
	m_pDefaultDlg = NULL;
	m_pActiveDlg = NULL;
	m_pListenDlg = NULL;
	m_pArticleDlg = NULL;
	m_pWriteDlg = NULL;
	m_pChoiceDlg = NULL;
	m_pArticleListenDlg = NULL;
	m_pArticleWithImageDlg = NULL;
	m_isupdateuser = FALSE;
	m_bShowStdAnswer = FALSE;
}

CExerciseMainDlg::~CExerciseMainDlg()
{
	CListAutoClean<CYDQuestionRef> clr(m_lstQuestions);
	delete m_pArticleDlg;
	m_pArticleDlg = NULL;
	delete m_pArticleWithImageDlg;
	m_pArticleWithImageDlg = NULL;
	delete  m_pDefaultDlg;
	m_pDefaultDlg = NULL;

	delete  m_pListenDlg;
	m_pListenDlg = NULL;
	delete  m_pWriteDlg;
	m_pWriteDlg = NULL;
	delete  m_pChoiceDlg;
	m_pChoiceDlg = NULL;
	delete  m_pArticleListenDlg;
	m_pArticleListenDlg = NULL;
}

BOOL CExerciseMainDlg::ReLocation(CQuestionDlg* pDlg)
{
	CRect rect, rectSelf;
	GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDlg->GetWindowRect(&rectSelf);
	pDlg->MoveWindow(rect.left, rect.top, rectSelf.Width(), rectSelf.Height());
	pDlg->ShowWindow(SW_HIDE);
	return TRUE;
}

BOOL CExerciseMainDlg::CreateArticleDlg()
{
	m_pArticleDlg = new CArticleDlg();
	
	m_pArticleDlg->m_pRecord = NULL;
	m_pArticleDlg->Create(CArticleDlg::IDD, this);
	ReLocation(m_pArticleDlg);

	m_pArticleWithImageDlg = new CArticleWithImageDlg();
	m_pArticleWithImageDlg->m_pRecord = NULL;
	m_pArticleWithImageDlg->Create(CArticleWithImageDlg::IDD, this);
	ReLocation(m_pArticleWithImageDlg);

	return TRUE;
}

BOOL CExerciseMainDlg::CreateListenDlg()
{
	m_pListenDlg = new CListenDlg();

	m_pListenDlg->m_pRecord = NULL;
	m_pListenDlg->Create(CListenDlg::IDD, this);
	ReLocation(m_pListenDlg);
	return TRUE;
}
BOOL CExerciseMainDlg::CreateWriteDlg()
{
	m_pWriteDlg = new CWriteDlg();

	m_pWriteDlg->m_pRecord = NULL;
	m_pWriteDlg->Create(CWriteDlg::IDD, this);
	ReLocation(m_pWriteDlg);
	return TRUE;
}

BOOL CExerciseMainDlg::CreateChoiceDlg()
{
	m_pChoiceDlg = new CChoiceDlg();;

	m_pChoiceDlg->m_pRecord = NULL;
	m_pChoiceDlg->Create(CChoiceDlg::IDD, this);
	ReLocation(m_pChoiceDlg);
	return TRUE;
}

BOOL CExerciseMainDlg::CreateArticleListenDlg()
{
	m_pArticleListenDlg = new CArticleListenDlg();

	m_pArticleListenDlg->m_pRecord = NULL;
	m_pArticleListenDlg->Create(CArticleListenDlg::IDD, this);
	ReLocation(m_pArticleListenDlg);
	return TRUE;
}

BOOL CExerciseMainDlg::CreateDefaultDlg()
{
	m_pDefaultDlg = new CDefaultDlg();
	m_pDefaultDlg->Create(CDefaultDlg::IDD, this);
	ReLocation(m_pDefaultDlg);
	return TRUE;
}

BOOL CExerciseMainDlg::SwitchActiveDlg(CQuestionDlg* pDlg)
{
	pDlg->m_bShowStdAnswer = m_bShowStdAnswer;
	if (NULL == m_pActiveDlg)
	{
		if (NULL != pDlg)
		{
			pDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_pActiveDlg->ShowWindow(SW_HIDE);
		pDlg->ShowWindow(SW_SHOW);
	}
	m_pActiveDlg = pDlg;
	return TRUE;
}

void CExerciseMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
// 	DDX_Control(pDX, IDC_BTN_EM_ABORT, m_btnAbort);
// 	DDX_Control(pDX, IDC_BTN_EM_BEGIN, m_btnBegin);
	DDX_Control(pDX, IDC_BTN_EM_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_EM_PREV, m_btnPrev);
//	DDX_Control(pDX, IDC_BTN_EM_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BTN_EM_STDANSWER, m_btnStdAnswer);
	DDX_Control(pDX, IDC_BTN_EM_STUANSWER, m_btnStuAnswer);
	DDX_Control(pDX, IDC_BTN_EM_CLOSE, m_btnFinish);
	DDX_Control(pDX, IDC_BTN_EM_RENAME, m_btnRename);
	DDX_Control(pDX, IDC_TREE_EM_QUESTION, m_tree);

}


BEGIN_MESSAGE_MAP(CExerciseMainDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_EM_STUANSWER, &CExerciseMainDlg::OnBnClickedBtnEmStuanswer)
	ON_BN_CLICKED(IDC_BTN_EM_STDANSWER, &CExerciseMainDlg::OnBnClickedBtnEmStdanswer)
	ON_NOTIFY(NM_CLICK, IDC_TREE_EM_QUESTION, &CExerciseMainDlg::OnNMClickTreeEmQuestion)
	ON_BN_CLICKED(IDC_BTN_EM_PREV, &CExerciseMainDlg::OnBnClickedBtnEmPrev)
	ON_BN_CLICKED(IDC_BTN_EM_NEXT, &CExerciseMainDlg::OnBnClickedBtnEmNext)
	ON_BN_CLICKED(IDC_BTN_EM_CLOSE, &CExerciseMainDlg::OnBnClickedBtnEmClose)
	ON_BN_CLICKED(IDC_BTN_EM_RENAME, &CExerciseMainDlg::OnBnClickedBtnEmRename)
//	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw )
END_MESSAGE_MAP()


// CExerciseMainDlg message handlers
//void CExerciseMainDlg::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)            
//{
//	LPNMHDR pNmhdr = (LPNMHDR)pNMHDR;   
// 
//	if(m_tree.m_hWnd != pNmhdr->hwndFrom)
//		return;
// 
//	LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
//	switch (pCustomDraw->nmcd.dwDrawStage)
//	{
//	case CDDS_PREPAINT:
//		// Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW, 
//		// otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH) 
//		*pResult = CDRF_NOTIFYITEMDRAW;
//		return;
//     
//	case CDDS_ITEMPREPAINT:
//		if(this->IsWindowEnabled()==1)
//		{
//			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0
//			&&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
//			{ 
//				pCustomDraw->clrTextBk=RGB(255, 255, 255);
//				pCustomDraw->clrText = RGB(0, 0, 0);
//			}
//			*pResult = CDRF_NOTIFYPOSTPAINT;
//			return;
//		}
//		else
//		{
//			*pResult = CDRF_DODEFAULT ;
//			return;
//		}
//	case CDDS_ITEMPOSTPAINT:
//		if(this->IsWindowEnabled()==1)
//		{
//			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0
//			&&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
//			{
//				CRect   rcText;  
//				HTREEITEM hItem=(HTREEITEM) pCustomDraw->nmcd.dwItemSpec;
//				m_tree.GetItemRect(hItem,   &rcText,   true); 
//				CPen penBlue(PS_SOLID ,1,RGB(0, 0, 255));
//				CDC* pDC=CDC::FromHandle(pCustomDraw->nmcd.hdc); 
//				CBrush* pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
//				CBrush* pOldBrush = pDC->SelectObject(pBrush);
//				CPen* pOldPen = pDC->SelectObject(&penBlue);
//				pDC->Rectangle(&rcText);
//				pDC->SelectObject(pOldBrush);
//				pDC->SelectObject(pOldPen);
//			}
//			*pResult = CDRF_SKIPDEFAULT;
//			return;
//		}
//		else
//		{
//			*pResult = CDRF_DODEFAULT ;
//			return;
//		}
//	 }
//}

BOOL CExerciseMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnBegin.SetImage(IDB_BITMAP_BEGIN);
	m_btnAbort.SetImage(IDB_BITMAP_Abort);
	m_btnNext.SetImage(IDB_BITMAP_NEXT);
	m_btnPrev.SetImage(IDB_BITMAP_PREV);
	m_btnStop.SetImage(IDB_BITMAP_STOP);
	m_btnStdAnswer.SetImage(IDB_BITMAP_STDANS);
	m_btnStuAnswer.SetImage(IDB_BITMAP_STUANS);
	m_btnFinish.SetImage(IDB_BITMAP_FINISH);
	m_btnRename.SetImage(IDB_BITMAP_RENAME);

	CreateArticleDlg();
	CreateListenDlg();
	CreateDefaultDlg();
	CreateChoiceDlg();
	CreateWriteDlg();
	CreateArticleListenDlg();
	CBitmap bmp;
	if (!bmp.LoadBitmap(IDB_BITMAP_TREENODE))
	{
		return FALSE;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= ILC_COLOR24;

	m_TreeNodeImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_TreeNodeImages.Add(&bmp, RGB(0, 0, 0));

	m_tree.SetImageList(&m_TreeNodeImages, TVSIL_NORMAL);

	if (m_bLoadMode)
	{
		m_log.SetFileName(m_strLogName);
		m_log.Load();
		std::list<CQuestionRecordStruct*>::iterator itr = m_log.m_lstQuestionStruct.begin();
		int typeId = 0;
		HTREEITEM hParentItem;
		HTREEITEM hFocuseItem = NULL;
		BOOL bFind = FALSE;
		int index = 1;
		CYDQuestionFactory factory;
		for (; itr != m_log.m_lstQuestionStruct.end(); ++itr)
		{
			CQuestionRecordStruct* pRec = *itr;
			CYDQuestionType type(theApp.m_pDatabase);
			type.SetID(pRec->m_QTypeID);
			CYDQuestionRef* pQuestion = factory.CreateQuestion(&type, theApp.m_pDatabase);
			pQuestion->SetID(pRec->m_uObjID);
			pRec->m_pQuestion = pQuestion;
			m_lstQuestions.push_back(pQuestion);
			if (typeId != pRec->m_QTypeID)
			{
				typeId = pRec->m_QTypeID;
				//根节点
				CString strType;
				type.GetPropVal(L"name", strType);
				hParentItem = m_tree.InsertItem(strType, 0, 0 ,NULL, NULL);
				m_tree.SetItemData(hParentItem, (DWORD_PTR)NULL);

			}

			CString strIndex;
			strIndex.Format(L"第%d题", index);
			++index;
			HTREEITEM hItem = m_tree.InsertItem(strIndex, 1, 1, hParentItem);
			m_tree.SetItemData(hItem, (DWORD_PTR)pRec);
			if (!bFind)
			{
				if (pRec->m_listUserAnswers.size() == 0)
				{
					hFocuseItem = hItem;
					bFind =TRUE;
				}
			}
			m_tree.Expand(hParentItem, TVE_EXPAND);
		}
		if(bFind)
		{
			m_tree.SelectItem(hFocuseItem);
			ShowSelItem(hFocuseItem);
		}
		m_log.UpdateLogName(m_strLogName);
		AfxGetMainWnd()->SendMessage(WM_YD_UPDATE_HISTORYINFO, (WPARAM)(&TREE_NODE_HISTORY),0);
	}
	else
	{
		CPdemWait wait(L"正在抽取试题请稍候...", FALSE, 3);
		wait.BeginWait();

		CSelectorEngine engine;

		HRESULT hr = engine.GetQuestionsFromCfg(CExerciseSelectQuestionCfgMgr::Instance(), 
			&m_lstQuestions);
		if (FAILED(hr))
		{
			return hr;
		}
		wait.StepIt();
		if (m_lstQuestions.size() > 0)
		{
			//生成record
			m_log.CreateLog();
			hr = m_log.GetFileName(m_strLogName);
			if (FAILED(hr))
			{
				return hr;
			}
			CLISTQUESTION::const_iterator itr = m_lstQuestions.begin();
			int typeId = 0;
			HTREEITEM hParentItem;
			int index = 1;
			for (; itr != m_lstQuestions.end(); ++itr)
			{
				CYDQuestionRef* pQuestion = *itr;

				long id = 0;
				pQuestion->GetPropVal(L"TYPEID", &id);
				if (typeId != id)
				{
					typeId = id;
					//根节点
					CYDQuestionType qtype(theApp.m_pDatabase);
					qtype.SetID(id);
					CString strType;
					qtype.GetPropVal(L"name", strType);
					hParentItem = m_tree.InsertItem(strType, 0, 0 ,NULL, NULL);
					m_tree.SetItemData(hParentItem, (DWORD_PTR)NULL);

				}

				CString strIndex;
				strIndex.Format(L"第%d题", index);
				CQuestionRecordStruct* pRecord;
				m_log.AddRecord(index, pQuestion, &pRecord);
				++index;
				HTREEITEM hItem = m_tree.InsertItem(strIndex, 1, 1, hParentItem);
				m_tree.SetItemData(hItem, (DWORD_PTR)pRecord);
				m_tree.Expand(hParentItem, TVE_EXPAND);
			}
			m_log.Save();
			wait.StepIt();

			//更新取题数
			itr = m_lstQuestions.begin();
			CDBTransactionRef trans(theApp.m_pDatabase, TRUE);
			for (; itr != m_lstQuestions.end(); ++itr)
			{
				CYDQuestionRef* pQuestion = *itr;

				CComVariant var;
				pQuestion->GetPropVal(L"USEDCOUNT", &var);
				long lCount = CDataHandler::VariantToLong(var);
				CString str;
				lCount++;
			
				hr = pQuestion->UpdateUsedCount(lCount);
				if (FAILED(hr))
				{
					trans.Rollback();
					return hr;
				}
			}
			trans.Commit();
			wait.StepIt();
			AfxGetMainWnd()->SendMessage(WM_YD_UPDATE_HISTORYINFO, (WPARAM)(&TREE_NODE_HISTORY),0);
		}
		else
		{
			wait.StepIt();
			wait.StepIt();
		}
	}
	if (m_lstQuestions.size() <= 0)
	{
		AfxMessageBox(L"没有获取到题目供当前练习使用！");
		OnOK();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExerciseMainDlg::OnBnClickedBtnEmStuanswer()
{
	// TODO: Add your control notification handler code here
	if (m_pActiveDlg != NULL)
	{
		m_pActiveDlg->PersistData();
		m_log.Save();
	}
	CDlgStudentAnswer dlg;
	dlg.m_pQuestionRecord = &m_log;
	dlg.DoModal();
}

void CExerciseMainDlg::OnBnClickedBtnEmStdanswer()
{
	// TODO: Add your control notification handler code here
	/*CDlgStdAnswer dlg;
	dlg.m_pListQuestion = &m_lstQuestions;
	dlg.DoModal();*/
	m_bShowStdAnswer = !m_bShowStdAnswer;
	if (m_bShowStdAnswer)
	{
		m_btnStdAnswer.SetWindowText(L"关闭参考答案");
	}
	else
	{
		m_btnStdAnswer.SetWindowText(L"显示参考答案");
	}
	SwitchActiveDlg(m_pDefaultDlg);
	HTREEITEM   hItem   =   m_tree.GetRootItem();
	
	HTREEITEM hNextItem = NULL;
	HRESULT hr = GetNextTreeItem(hItem,hNextItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(hNextItem == NULL)
	{
		return;
	}
	CQuestionRecordStruct* pReocrd = (CQuestionRecordStruct*)m_tree.GetItemData(hNextItem);
	if(pReocrd == NULL)
	{
		//当前不是题目，可能是题目的类型，再往前找一个
		hItem = hNextItem;
		hNextItem = NULL;
		hr = GetNextTreeItem(hItem,hNextItem);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(hNextItem == NULL)
		{
			AfxMessageBox(_T("当前已经是最后一个节点了！"));
			return;
		}
	}
	ASSERT(hNextItem);
	m_tree.SelectItem(hNextItem);
	hr = ShowSelItem(hNextItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CExerciseMainDlg::OnNMClickTreeEmQuestion(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	DWORD   pos   =   GetMessagePos(); 
	CPoint   pt(LOWORD(pos),   HIWORD(pos));   
	ScreenToClient(&pt); 
	UINT   uFlags;   
	HTREEITEM   hItem   =   m_tree.HitTest(pt,   &uFlags); 
	HRESULT hr = E_FAIL;
	hr = ShowSelItem(hItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}	
	
}

void CExerciseMainDlg::OnBnClickedBtnEmPrev()
{
	// TODO: Add your control notification handler code here
	HTREEITEM   hItem   =   m_tree.GetSelectedItem();
	if(hItem == NULL)
	{
		AfxMessageBox(_T("请在树上选中一个节点！"));
		return;
	}
	HRESULT hr = E_FAIL;
	HTREEITEM hPrevItem = NULL;
	hr = GetPrevTreeItem(hItem,hPrevItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(hPrevItem == NULL)
	{
		AfxMessageBox(_T("当前已经是第一个节点了！"));
		return;
	}
	CQuestionRecordStruct* pReocrd = (CQuestionRecordStruct*)m_tree.GetItemData(hPrevItem);
	if(pReocrd == NULL)
	{
		//当前不是题目，可能是题目的类型，再往前找一个
		hItem = hPrevItem;
		hPrevItem = NULL;
		hr = GetPrevTreeItem(hItem,hPrevItem);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(hPrevItem == NULL)
		{
			AfxMessageBox(_T("当前已经是第一个节点了！"));
			return;
		}
	}
	ASSERT(hPrevItem);
	m_tree.SelectItem(hPrevItem);
	hr = ShowSelItem(hPrevItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CExerciseMainDlg::OnBnClickedBtnEmNext()
{
	// TODO: Add your control notification handler code here
	HTREEITEM   hItem   =   m_tree.GetSelectedItem();
	if(hItem == NULL)
	{
		AfxMessageBox(_T("请在树上选中一个节点！"));
		return;
	}
	HRESULT hr = E_FAIL;
	HTREEITEM hNextItem = NULL;
	hr = GetNextTreeItem(hItem,hNextItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(hNextItem == NULL)
	{
		AfxMessageBox(_T("当前已经是最后一个节点了！"));
		return;
	}
	CQuestionRecordStruct* pReocrd = (CQuestionRecordStruct*)m_tree.GetItemData(hNextItem);
	if(pReocrd == NULL)
	{
		//当前不是题目，可能是题目的类型，再往前找一个
		hItem = hNextItem;
		hNextItem = NULL;
		hr = GetNextTreeItem(hItem,hNextItem);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(hNextItem == NULL)
		{
			AfxMessageBox(_T("当前已经是最后一个节点了！"));
			return;
		}
	}
	ASSERT(hNextItem);
	m_tree.SelectItem(hNextItem);
	hr = ShowSelItem(hNextItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}



HRESULT CExerciseMainDlg::GetPrevTreeItem(HTREEITEM _hItem,HTREEITEM &_hPrevItem)
{
	HRESULT hr = E_FAIL;
	_hPrevItem = NULL;
	HTREEITEM hSiblingPrevItem = m_tree.GetPrevSiblingItem(_hItem);//_hItem的当前的兄弟节点
	if(hSiblingPrevItem == NULL)
	{
		//没有前一个节点,则前一个节点为当前的父节点的前一个兄弟节点的最后一个节点
		HTREEITEM hParentItem = m_tree.GetParentItem(_hItem);
		if(!hParentItem)
		{
			return S_OK;
		}
		_hPrevItem = hParentItem;
	}
	else
	{
		if(m_tree.ItemHasChildren(hSiblingPrevItem))
		{
			//应该是字节点下的最后一个
			HTREEITEM hCurItem = m_tree.GetChildItem(hSiblingPrevItem);
			while(TRUE)
			{
				HTREEITEM hNextItem = m_tree.GetNextSiblingItem(hCurItem);
				if(hNextItem == NULL)
				{
					//到最后一个了,要判断是否有字
					if(!m_tree.ItemHasChildren(hCurItem))
					{
						break;
					}
					hCurItem = m_tree.GetChildItem(hCurItem);
				}
				else
				{
					hCurItem = hNextItem;
				}
				
			}
			_hPrevItem = hCurItem;	
		}
		else
		{
			_hPrevItem = hSiblingPrevItem;
		}
	}
	return S_OK;
}

HRESULT CExerciseMainDlg::GetNextTreeItem(HTREEITEM _hItem,HTREEITEM &_hNextItem)
{
	HRESULT hr = E_FAIL;
	_hNextItem = NULL;
	if(m_tree.ItemHasChildren(_hItem))
	{
		_hNextItem = m_tree.GetChildItem(_hItem);
		return S_OK;
	}
	HTREEITEM hSiblingNextItem = m_tree.GetNextSiblingItem(_hItem);//_hItem的当前的兄弟节点
	if(hSiblingNextItem == NULL)
	{
		//没有后一个节点,则后一个节点为当前的父节点的后一个兄弟节点
		HTREEITEM hParentItem = m_tree.GetParentItem(_hItem);
		if(!hParentItem)
		{
			return S_OK;
		}
		HTREEITEM hParentNextItem = m_tree.GetNextSiblingItem(hParentItem);
		_hNextItem = hParentNextItem;
	}
	else
	{
		_hNextItem = hSiblingNextItem;
	}
	return S_OK;
}

HRESULT CExerciseMainDlg::ShowSelItem(HTREEITEM _hItem)
{
	HRESULT hr = E_FAIL;
	if (_hItem)
	{
		CQuestionRecordStruct* pReocrd = (CQuestionRecordStruct*)m_tree.GetItemData(_hItem);
		if (pReocrd != NULL)
		{
			if (m_pActiveDlg == NULL || pReocrd != m_pActiveDlg->m_pRecord)
			{
				if (m_pActiveDlg != NULL)
				{
					m_pActiveDlg->PersistData();
					m_log.Save();
				}
				TITLEMODE titlemode = TITLEMODE_TEXT;
				pReocrd->m_pQuestion->GetTitleMode(&titlemode);
				CQuestionDlg* pSwitchDlg = NULL;
				if (pReocrd->m_QTypeID == 1)//作文
				{
					pSwitchDlg = m_pWriteDlg;
				}
				else if (pReocrd->m_QTypeID == 2 || pReocrd->m_QTypeID == 4)//阅读理解 完形填空
				{
					if (TITLEMODE_TEXT == titlemode)
					{
						pSwitchDlg = m_pArticleDlg;
					}
					else if (TITLEMODE_IMAGE == titlemode)
					{
						pSwitchDlg = m_pArticleWithImageDlg;
					}
				}
				else if (pReocrd->m_QTypeID == 3)//短文听力
				{
					pSwitchDlg = m_pListenDlg;
				}
				else if (pReocrd->m_QTypeID == 5)//翻译题
				{
					pSwitchDlg = m_pWriteDlg;
				}
				else if (pReocrd->m_QTypeID == 6)//词汇与语法
				{
					pSwitchDlg = m_pChoiceDlg;
				}
				else if (pReocrd->m_QTypeID == 7)//快速阅读
				{
					if (TITLEMODE_TEXT == titlemode)
					{
						pSwitchDlg = m_pArticleDlg;
					}
					else if (TITLEMODE_IMAGE == titlemode)
					{
						pSwitchDlg = m_pArticleWithImageDlg;
					}
				}
				else if (pReocrd->m_QTypeID == 8)//阅读选词填空
				{
					if (TITLEMODE_TEXT == titlemode)
					{
						pSwitchDlg = m_pArticleDlg;
					}
					else if (TITLEMODE_IMAGE == titlemode)
					{
						pSwitchDlg = m_pArticleWithImageDlg;
					}
				}
				else if (pReocrd->m_QTypeID == 9)//听力填空
				{
					pSwitchDlg = m_pArticleListenDlg;
				}

				else if (pReocrd->m_QTypeID == 10)//短句听力
				{
					pSwitchDlg = m_pListenDlg;
				}
				else if (pReocrd->m_QTypeID == 11)//长句听力
				{
					pSwitchDlg = m_pListenDlg;
				}
				else if (pReocrd->m_QTypeID == 12)//改错
				{
					if (TITLEMODE_TEXT == titlemode)
					{
						pSwitchDlg = m_pArticleDlg;
					}
					else if (TITLEMODE_IMAGE == titlemode)
					{
						pSwitchDlg = m_pArticleWithImageDlg;
					}
				}
				else
				{
					ASSERT(FALSE);
				}
				pSwitchDlg->m_pRecord = pReocrd;
				SwitchActiveDlg(pSwitchDlg);
			}
		}
		else
		{
			SwitchActiveDlg(m_pDefaultDlg);
		}
		UpdateData(FALSE);
	}
	return S_OK;
}

void CExerciseMainDlg::OnBnClickedBtnEmClose()
{
	// TODO: Add your control notification handler code here
	if (m_lstQuestions.size() > 0)
	{
		if (m_pActiveDlg != NULL)
		{
			m_pActiveDlg->PersistData();
			m_log.Save();
		}
		CDlgExsiceMark dlg;
		dlg.m_pQuestionRecord = &m_log;
		dlg.DoModal();

		if (!m_isupdateuser)
		{
			CYDUserRef* puser = NULL;
			CStaticYdUser::Instance()->GetCurUser(puser);
			puser->SetPropVal(FIELD_YDUSER_LEVEL, &CComVariant(dlg.m_strLevel));
			AfxGetMainWnd()->SendMessage(WM_YD_UPDATE_PERSIONINFO, (WPARAM)(&TREE_NODE_USER_INFO),0);
			CDBTransactionRef trans(theApp.m_pDatabase, TRUE);
			HRESULT hr = puser->Update();
			if (FAILED(hr))
			{
				return;
			}
			trans.Commit();
			m_isupdateuser = TRUE;
		}
		}
}

void CExerciseMainDlg::OnBnClickedBtnEmRename()
{
	// TODO: Add your control notification handler code here
	CDlgReName dlg;
	dlg.m_strName = m_strLogName;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	m_strLogName = dlg.m_strName;
	m_log.SetFileName(m_strLogName,TRUE);

}
