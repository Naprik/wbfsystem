// DlgAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAnswer.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../YDExamObjRef\YDStuMark.h"


// CDlgAnswer dialog

IMPLEMENT_DYNAMIC(CDlgAnswer, CDialog)

CDlgAnswer::CDlgAnswer(CYDObjectRef* _pPaper, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnswer::IDD, pParent)
{
	m_pPaper = _pPaper;
}

CDlgAnswer::~CDlgAnswer()
{
	CListAutoClean<CYdObjWrapper> clr(m_lstObjWrapperCache);
	MapAnswerRecord::iterator itr = m_mapExistAnswer.begin();
	for (; itr != m_mapExistAnswer.end(); ++itr)
	{
		CPtrAutoClean<CAnswerRecord> clr2(itr->second);
	}
}

void CDlgAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAnswer, CDialog)
	ON_BN_CLICKED(IDC_BTN_DM_ANSWER_OK, &CDlgAnswer::OnBnClickedBtnDmAnswerOk)
END_MESSAGE_MAP()


// CDlgAnswer message handlers

BOOL CDlgAnswer::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetDlgItem(IDC_DM_ANSWER_EDIT_GRID)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (!m_AnswerGrid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rect, this,
		-1))
	{
		return FALSE;
	}

	m_AnswerGrid.EnableDragHeaderItems(FALSE);
	m_AnswerGrid.EnableColumnAutoSize (TRUE);
	m_AnswerGrid.EnableGroupByBox (FALSE);
	m_AnswerGrid.SetWholeRowSel(FALSE);
	m_AnswerGrid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_AnswerGrid.SetColorTheme (colors);
	int iWidth = rect.Width();
	m_AnswerGrid.InsertColumn(0, L"序号", iWidth/4 - 2);
	m_AnswerGrid.InsertColumn(1, L"题号", iWidth/4);
	m_AnswerGrid.InsertColumn(2, L"答案", iWidth/4);
	m_AnswerGrid.InsertColumn(3, L"分值", iWidth/4);

	for (int iColumn = 0; iColumn < m_AnswerGrid.GetColumnCount(); iColumn++)
	{
		m_AnswerGrid.SetHeaderAlign(iColumn,HDF_CENTER);
		m_AnswerGrid.SetColumnAlign(iColumn, HDF_LEFT);
	}
	//修改标题栏位试卷名+试卷编辑
	ASSERT(m_pPaper);
	CString strPaperName;
	HRESULT hr = E_FAIL;
	hr = m_pPaper->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	CString strCaption = strPaperName +_T("-试卷编辑");
	SetWindowText(strCaption);
	FillGrid();
	m_AnswerGrid.AdjustLayout();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgAnswer::FillGrid()
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAnswers;
	std::list<CYDLinkRef*> lstLinks;
	CYDPaper* pPaper = dynamic_cast<CYDPaper*>(m_pPaper);
	hr = pPaper->GetStdAnswer(&lstAnswers, &lstLinks);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<long> lstNO;
	long lBegin;
	pPaper->GetPropVal(FIELD_YDPAPER_BEGIN1, &lBegin);
	long lEnd;
	pPaper->GetPropVal(FIELD_YDPAPER_END1, &lEnd);
	long lSum = 0;
	if (lBegin != 0 && lEnd != 0)
	{
		lSum = lEnd - lBegin + 1;
		for (int i=lBegin; i<=lEnd; ++i)
		{
			lstNO.push_back(i);
		}
	}
	
	pPaper->GetPropVal(FIELD_YDPAPER_BEGIN2, &lBegin);
	pPaper->GetPropVal(FIELD_YDPAPER_END2, &lEnd);
	if (lBegin != 0 && lEnd != 0)
	{
		lSum += (lEnd - lBegin + 1);
		for (int i=lBegin; i<=lEnd; ++i)
		{
			lstNO.push_back(i);
		}
	}
	
	pPaper->GetPropVal(FIELD_YDPAPER_BEGIN3, &lBegin);
	pPaper->GetPropVal(FIELD_YDPAPER_END3, &lEnd);
	if (lBegin != 0 && lEnd != 0)
	{
		lSum += (lEnd - lBegin + 1);
		for (int i=lBegin; i<=lEnd; ++i)
		{
			lstNO.push_back(i);
		}
	}
	std::list<long>::iterator itrNo = lstNO.begin();
	std::list<CYDObjectRef*>::iterator itrAnswer = lstAnswers.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstLinks.begin();
	int i = 0;
	for (; itrAnswer != lstAnswers.end(); ++itrAnswer, ++itrLink, ++i)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pObjRef = (*itrAnswer);
		pObjWrapper->m_pLinkRef = (*itrLink);
		pObjWrapper->m_state = S_UNKNOW;
		m_lstObjWrapperCache.push_back(pObjWrapper);
		InsertItem(pObjWrapper);
		if (itrNo != lstNO.end())
		{
			++itrNo;
		}
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	for (; i<lSum; ++i, ++itrNo)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pObjRef = new CYDAnswer(pDB);
		CComVariant var((long)*itrNo);
		pObjWrapper->m_pObjRef->SetPropVal(FIELD_YDANSWER_NO, &var);
		pObjWrapper->m_pLinkRef = new CYDLinkRef(pDB, DB_PAPERANSWER);
		pObjWrapper->m_pLinkRef->PutObjRef(m_pPaper, pObjWrapper->m_pObjRef);
		pObjWrapper->m_state = S_UNKNOW;
		m_lstObjWrapperCache.push_back(pObjWrapper);
		InsertItem(pObjWrapper);
	}

	return S_OK;
}


HRESULT CDlgAnswer::InsertItem(CYdObjWrapper* pObjWrapper)
{
	HRESULT hr = E_FAIL;

	CBCGPGridRow* pRow = m_AnswerGrid.CreateRow(m_AnswerGrid.GetColumnCount());
	
	pRow->SetData(DWORD_PTR(pObjWrapper));

	UINT iSerNO = m_AnswerGrid.GetRowCount();
	++iSerNO;
	COLORREF bg;
	if (iSerNO%2 == 0)
	{
		bg = RGB(110,180,200);
	}
	else
	{
		bg = RGB(225, 225, 225);
	}
	OBJID objID;
	pObjWrapper->m_pObjRef->GetID(&objID);
	//序号
	CString strSerNo;
	strSerNo.Format(L"%d", iSerNO);
	_variant_t varSerNO;
	CDataHandler::StringToVariant(strSerNo, VT_I4, &varSerNO);
	pRow->GetItem(0)->SetValue(varSerNO);
	pRow->GetItem(0)->SetBackgroundColor(bg);
	pRow->GetItem(0)->Enable(FALSE);
	//题号
	
	long lNO = 0;
	pObjWrapper->m_pObjRef->GetPropVal(FIELD_YDANSWER_NO, &lNO);
	CString strNO ;
	strNO.Format(_T("%d"),lNO);
	pRow->ReplaceItem(1, new CBCGPGridItem(_bstr_t(strNO)));
	if (objID != ID_EMPTY)
	{
		
		//pRow->ReplaceItem(1, new CBCGPGridItem(varNO));
		_variant_t varAnswer;
		pObjWrapper->m_pObjRef->GetPropVal(FIELD_YDANSWER_ANSWER, &varAnswer);
		//pRow->ReplaceItem(2, new CBCGPGridItem(varAnswer));
		_variant_t varMark;
		pObjWrapper->m_pObjRef->GetPropVal(FIELD_YDANSWER_MARK, &varMark);
		//pRow->ReplaceItem(3, new CBCGPGridItem(varMark));
		
		
		CString strAnswer = CDataHandler::VariantToString(varAnswer);
		CString strMark = CDataHandler::VariantToString(varMark);
		double fMark = _tstof(strMark);
		

		pRow->ReplaceItem(2, new CBCGPGridItem(_bstr_t(strAnswer)));
		pRow->ReplaceItem(3, new CBCGPGridItem(fMark));
		m_mapExistAnswer.insert(MapAnswerRecord::value_type(objID, new CAnswerRecord(strNO, strAnswer, strMark)));
	}
	else
	{
		//pRow->ReplaceItem(1, new CBCGPGridItem(L""));
		pRow->ReplaceItem(2, new CBCGPGridItem(L""));
		pRow->ReplaceItem(3, new CBCGPGridItem(1.00));
	}
	pRow->GetItem(1)->SetBackgroundColor(bg);
	pRow->GetItem(2)->SetBackgroundColor(bg);
	pRow->GetItem(3)->SetBackgroundColor(bg);
	m_AnswerGrid.AddRow(pRow);
	return S_OK;
}

void CDlgAnswer::OnBnClickedBtnDmAnswerOk()
{
	HRESULT hr = E_FAIL;
	for (int i=0; i<m_AnswerGrid.GetRowCount(); ++i)
	{
		CBCGPGridRow* pRow = m_AnswerGrid.GetRow(i);
		CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)pRow->GetData();
		ASSERT(pObjWrapper != NULL);
		OBJID objId = ID_EMPTY;
		pObjWrapper->m_pObjRef->GetID(&objId);
		
		_variant_t varNO = pRow->GetItem(1)->GetValue();
		CString strNO = varNO;
		_variant_t varAnswer = pRow->GetItem(2)->GetValue();
		CString strAnswer = varAnswer;
		_variant_t varMark = pRow->GetItem(3)->GetValue();
	

		int cEmpty = 0;
		CString strMakr = varMark;
		if (strNO.IsEmpty())
		{
			++cEmpty;
		}
		if (strAnswer.IsEmpty())
		{
			++cEmpty;
		}
		if (strMakr.IsEmpty())
		{
			++cEmpty;
		}
		if (cEmpty != 0 && cEmpty != 3)
		{
			_variant_t varSerNo = pRow->GetItem(0)->GetValue();
			CString strSerNo = varSerNo;
			CString strInfo = L"第" +  strSerNo + L"填写不完整！";
			AfxMessageBox(strInfo);
			return;
		}
		if (!strMakr.IsEmpty())
		{
			long lMark = CDataHandler::VariantToLong(varMark);
			if(lMark < 0)
			{
				_variant_t varSerNo = pRow->GetItem(0)->GetValue();
				CString strSerNo = varSerNo;
				CString strInfo = L"第" +  strSerNo + L"分数不能为负！";
				AfxMessageBox(strInfo);
				return ;
			}
		}

		_variant_t varProp;
		CDataHandler::StringToVariant(strNO, VT_I4, &varProp);
		pObjWrapper->m_pObjRef->SetPropVal(FIELD_YDANSWER_NO, &varProp);
		CDataHandler::StringToVariant(strAnswer, VT_BSTR, &varProp);
		pObjWrapper->m_pObjRef->SetPropVal(FIELD_YDANSWER_ANSWER, &varProp);
		CDataHandler::StringToVariant(strMakr, VT_R4, &varProp);
		pObjWrapper->m_pObjRef->SetPropVal(FIELD_YDANSWER_MARK, &varProp);

		if (objId == ID_EMPTY)//新建的
		{
			if (cEmpty == 0)
			{
				pObjWrapper->m_state = S_NEED_SAVE_BOTH;//新建
			}
		}
		else
		{
			if (cEmpty == 3)
			{
				pObjWrapper->m_state = S_NEED_DELETE_BOTH;//删除
			}
			else
			{
				MapAnswerRecord::iterator itrRec = m_mapExistAnswer.find(objId);
				ASSERT(itrRec != m_mapExistAnswer.end());
				if (0 != strNO.CompareNoCase(itrRec->second->m_strNO) ||
					0 != strAnswer.CompareNoCase(itrRec->second->m_strAnswer) ||
					0 != strMakr.CompareNoCase(itrRec->second->m_strMark))
				{
					pObjWrapper->m_state = S_NEED_UPDATE_OBJECT; //更新
				}
			}
		}
	}
	CDBTransactionRef trans(m_pPaper->m_pDb, TRUE);
	for (std::list<CYdObjWrapper*>::const_iterator itr = m_lstObjWrapperCache.begin();
			itr != m_lstObjWrapperCache.end(); ++itr)
	{
		hr = (*itr)->Persist();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	//要将该试卷关联的学生成绩信息进行更新
	OBJID IDPaper = 0;
	hr = m_pPaper->GetID(&IDPaper);
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
	CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDSTUMARK_PAPERID;
	pPropQueryContidition->m_strConVal.Format(_T("%d"),IDPaper);
	lstQueryCon.push_back(pPropQueryContidition);
	std::list<CYDObjectRef*> lstStuMark;
	CListAutoClean<CYDObjectRef> clr1(lstStuMark);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDSTUMARK,pDB,&lstStuMark,&lstQueryCon);
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstStuMark.begin();
		itr != lstStuMark.end();++itr)
	{
		//重新计算成绩
		CYDStuMark* pStuMark = (CYDStuMark*)(*itr);
		ASSERT(pStuMark);
		double dbMark =0;
		hr = pStuMark->CalMark(&dbMark);
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CComVariant valMark(dbMark);
		hr = pStuMark->SetPropVal(FIELD_YDSTUMARK_MARK,&valMark);
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = (*itr)->Update();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}

	hr = trans.Commit();
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();

}

INT_PTR CDlgAnswer::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}
