// DlgInputStudentPreview.cpp : implementation file
//

#include "stdafx.h"
#include "DlgInputStudentPreview.h"
#include "../DBBase\DatabaseEx.h"
#include "../DBBase\DBTransactionRef.h"
#include "../YDExamObjRef\YdStudentRef.h"
#include "../Base\DataHandler.h"
#include "../Base\AutoClean.h"


// CDlgInputStudentPreview dialog

IMPLEMENT_DYNAMIC(CDlgInputStudentPreview, CDialog)

CDlgInputStudentPreview::CDlgInputStudentPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputStudentPreview::IDD, pParent)
{
	m_pClass = NULL;
}

CDlgInputStudentPreview::~CDlgInputStudentPreview()
{
}

void CDlgInputStudentPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputStudentPreview, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgInputStudentPreview::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgInputStudentPreview::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CDlgInputStudentPreview::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputStudentPreview message handlers

BOOL CDlgInputStudentPreview::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rectBK;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rectBK);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	if (!m_Grid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_Grid.EnableDragHeaderItems(FALSE);
	m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	//m_Grid.InsertColumn(0, L"���", 50);
	int iColIndex = 1;
	ASSERT(m_lstPropVal.size() > 0);
	BOOL bFirst = TRUE;
	for(std::list <std::list<std::pair<CString,CString> > >::const_iterator itrLst = m_lstPropVal.begin();
		itrLst != m_lstPropVal.end();++itrLst)
	{
		std::list<std::pair<CString,CString> > lst = (*itrLst);
		int iCol = 0;
		if(bFirst)
		{
			bFirst = FALSE;
			for(std::list<std::pair<CString,CString> >::const_iterator itr = lst.begin();
				itr != lst.end();++itr)
			{
				m_Grid.InsertColumn(iCol++, (*itr).first, 80);
			}
		}
		iCol = 0;
		CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		ASSERT(pRow);
		
		for(std::list<std::pair<CString,CString> >::const_iterator itr = lst.begin();
			itr != lst.end();++itr)
		{
			pRow->GetItem(iCol++)->SetValue(CComVariant((*itr).second));
		}
		m_Grid.AddRow(pRow);
	}
	m_Grid.AdjustLayout ();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR CDlgInputStudentPreview::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}

void CDlgInputStudentPreview::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	ASSERT(pRow);

	for(int iCol = 0; iCol < m_Grid.GetColumnCount();iCol++)
	{
		pRow->GetItem(iCol++)->SetValue(L"");
	}
	m_Grid.AddRow(pRow);
}

void CDlgInputStudentPreview::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pSelRow = m_Grid.GetCurSel();
	if(pSelRow == NULL)
	{
		AfxMessageBox(_T("�����б���ѡ��Ҫɾ������"));
		return;
	}
	if(AfxMessageBox(_T("��ȷ��Ҫɾ��ѡ�������"),MB_YESNO) != IDYES)
	{
		return;
	}
	for(int i = 0 ; i <m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		if(pRow == pSelRow)
		{
			m_Grid.RemoveRow(i);
			break;
		}
	}
}

void CDlgInputStudentPreview::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	/*
	AddPropDef(FIELD_YDSTU_ID,VT_BSTR);//ѧ��
	AddPropDef(FIELD_YDSTU_NAME,VT_BSTR);	//ѧ������
	AddPropDef(FIELD_YDSTU_GENDER,VT_I4,_T("1"));	//ѧ���Ա�
	AddPropDef(FIELD_YDSTU_AGE,VT_I4);	//ѧ������
	AddPropDef(FIELD_YDSTU_TEL,VT_BSTR);	//ѧ���绰
	AddPropDef(FIELD_YDSTU_MAIL,VT_BSTR);	//ѧ��Emai
	AddPropDef(FIELD_YDSTU_PHOTO,VT_BLOB_OBJECT);	//ѧ��ͼƬ	
	AddPropDef(FIELD_YDSTU_EVID,VT_BSTR);	//�豸ID
	*/
	
	for(int i = 0 ; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CYdStudentRef* pStudentRef = new CYdStudentRef(pDB);
		CPtrAutoClean<CYdStudentRef> clr(pStudentRef);
		for(int iCol = 0; iCol < m_Grid.GetColumnCount();iCol++)
		{
			CString strColName = m_Grid.GetColumnName(iCol);
			CComVariant val = pRow->GetItem(iCol)->GetValue();
			if(strColName.CompareNoCase(_T("ѧ��")) == 0)
			{
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_ID,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
			else if(strColName.CompareNoCase(_T("����")) == 0)
			{
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_NAME,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}

			}
			else if(strColName.CompareNoCase(_T("�Ա�")) == 0)
			{
				CString strVal = CDataHandler::VariantToString(val);
				CComVariant valSex(long(0));
				if(strVal.CompareNoCase(_T("Ů")) == 0)
				{
					valSex = 1;
				}
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_GENDER,&valSex);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
			else if(strColName.CompareNoCase(_T("ѧ�����豸��")) == 0)
			{
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_EVID,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
			else if(strColName.CompareNoCase(_T("�绰")) == 0)
			{
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_TEL,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
			else if(strColName.CompareNoCase(_T("Email")) == 0)
			{
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_MAIL,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
			else if(strColName.CompareNoCase(_T("����")) == 0)
			{
				long lAge = CDataHandler::VariantToLong(val);
				CComVariant valAge(lAge);
				hr = pStudentRef->SetPropVal(FIELD_YDSTU_AGE,&valAge);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
		}
		hr = pStudentRef->Save();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		//�����༶��ѧ���Ĺ�ϵ
		ASSERT(m_pClass != NULL);
		CYDLinkRef classToStu(pDB, DB_YDLINK_CLASS_STUDENT);
		hr = classToStu.PutObjRef(m_pClass, pStudentRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		hr = classToStu.Save();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	AfxMessageBox(_T("����ɹ���"));
	OnOK();
}
