// DlgStunfo.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "DlgStunfo.h"
#include "../DBBase/DatabaseEx.h"
#include "../DBBase/DBTransactionRef.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../YDExamObjRef/YDClassRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStunfo dialog


CDlgStunfo::CDlgStunfo(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStunfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStunfo)
	m_strEmail = _T("");
	m_strName = _T("");
	m_strNum = _T("");
	m_strTel = _T("");
	m_strBlueteeth = _T("");
	m_iAge = 0;
	m_op = op;
	m_pStudent = NULL;
	m_pClassWrapper = new CYdObjWrapper();
	m_hPhotoBitmap = NULL;
	m_pBMPBuffer = NULL;
	m_dwFileLen = 0;
	//}}AFX_DATA_INIT
}

CDlgStunfo::~CDlgStunfo()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstDeptCache);
	CPtrAutoClean<CYdObjWrapper> clr3(m_pClassWrapper);
	ClearClassCache();
	DestroyPhoto();
}

void CDlgStunfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStunfo)
	DDX_Control(pDX, IDC_DM_STU_DEPARTMENT, m_department);
	DDX_Control(pDX, IDC_DM_STU_CLASS, m_class);
	DDX_Control(pDX, IDC_DM_STU_GENDER, m_gender);
	DDX_Text(pDX, IDC_DM_STU_EMIAL, m_strEmail);
	DDX_Text(pDX, IDC_DM_STU_NAME, m_strName);
	DDX_Text(pDX, IDC_DM_STU_NUM, m_strNum);
	DDX_Text(pDX, IDC_DM_STU_TEL, m_strTel);
	DDX_Text(pDX, IDC_DM_STU_BLUETEETH, m_strBlueteeth);
	DDX_Text(pDX, IDC_DM_STU_AGE, m_iAge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStunfo, CDialog)
	//{{AFX_MSG_MAP(CDlgStunfo)
	ON_BN_CLICKED(IDC_BTN_DM_STU_SELPIC, OnBtnSelPic)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_DM_STU_DEPARTMENT, OnSelchangeDepartment)
	ON_BN_CLICKED(IDOK, &CDlgStunfo::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStunfo message handlers

BOOL CDlgStunfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rectPic;
	if (m_op == OP_VIEW)
	{
		ReadOnly(TRUE);
	}
	else
	{
		ReadOnly(FALSE);
	}
	HRESULT hr = E_FAIL;
	//初始化部门下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT, pDB, &m_lstDeptCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if (m_lstDeptCache.size() > 0)
	{
		int nIndex = 0;
		m_department.InsertString(nIndex++, L"");
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstDeptCache.begin();
			itr != m_lstDeptCache.end(); ++itr, ++nIndex)
		{
			CString strDeptName;
			(*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDeptName);
			m_department.InsertString(nIndex, strDeptName);
			m_department.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}

	m_gender.AddString(L"男");
	m_gender.AddString(L"女");

	if (m_pStudent != NULL)
	{
		//学号
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_ID, m_strNum);
		if (FAILED(hr))
		{
			return hr;
		}
		//名称
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_NAME, m_strName);
		if (FAILED(hr))
		{
			return hr;
		}
		//年龄
		CComVariant varAge;
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_AGE, &varAge);
		if (FAILED(hr))
		{
			return hr;
		}
		m_iAge = CDataHandler::VariantToLong(varAge);
		//班级与院系
		std::list<CYDObjectRef*> lstClass;
		std::list<CYDLinkRef*>	lstLinks;
		hr = m_pStudent->GetClass(&lstClass, &lstLinks);
		if (FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstLinks.size() == 1);
		m_pClassWrapper->m_pObjRef = (*(lstClass.begin()));
		m_pClassWrapper->m_pLinkRef = (*(lstLinks.begin()));
		m_pClassWrapper->m_pObjRef->GetPropVal(FIELD_YDCLASS_NAME, m_strClass);
		
		CYDObjectRef* pDept = NULL;
		CYDClassRef* pClass = dynamic_cast<CYDClassRef*>(m_pClassWrapper->m_pObjRef);
		pClass->GetDepartment(&pDept, NULL);
		CPtrAutoClean<CYDObjectRef> clr(pDept);
		CString strDept;
		pDept->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDept);
		int departindex = m_department.FindStringExact(0, strDept);
		m_department.SetCurSel(departindex);
		OnSelchangeDepartment();
		int classIndex = m_class.FindStringExact(0, m_strClass);
		m_class.SetCurSel(classIndex);

		//性别
		m_pStudent->GetGender(&m_strGender);
		int genderIndex = m_gender.FindStringExact(0, m_strGender);
		m_gender.SetCurSel(genderIndex);

		//电话 
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_TEL, m_strTel);
		if (FAILED(hr))
		{
			return hr;
		}
		//email
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_MAIL, m_strEmail);
		if (FAILED(hr))
		{
			return hr;
		}
		//蓝牙
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_EVID, m_strBlueteeth);
		if (FAILED(hr))
		{
			return hr;
		}
		//头像
// 		TCHAR szTempPath[256];
// 		::GetTempPath(256, szTempPath);
// 		CString strTempPath(szTempPath);
// 		strTempPath += m_strNum;
// 		strTempPath += L".bmp";
// 		hr = m_pStudent->GetBlobProp(FIELD_YDSTU_PHOTO, CComBSTR(strTempPath));	
//  		strTempPath.Replace(L"\\", L"\\\\");
// 		m_strPath = strTempPath;
// 		m_strTelTmp = m_strPath;
// 		CStatic * pStatic = (CStatic *)GetDlgItem(IDC_DM_STU_PIC);  
// 		HBITMAP   hBitmap=(HBITMAP)::LoadImage(NULL,m_strPath,IMAGE_BITMAP,180,200,LR_LOADFROMFILE);  
	
		CComVariant valPhoto;
		hr = m_pStudent->GetPropVal(FIELD_YDSTU_PHOTO,&valPhoto);
		if(valPhoto.vt == (VT_ARRAY | VT_UI1))
		{
			//说明有图像的数据
			m_dwFileLen = valPhoto.parray->rgsabound[0].cElements;
			if(m_pBMPBuffer = new char[m_dwFileLen+1])
			{
				char *pBuf = NULL;
				SafeArrayAccessData(valPhoto.parray,(void **)&pBuf);
				memcpy(m_pBMPBuffer,pBuf,m_dwFileLen);				///复制数据到缓冲区m_pBMPBuffer
				SafeArrayUnaccessData (valPhoto.parray);
				m_hPhotoBitmap = BufferToHBITMAP();	
			}
		
		}

		

		if (OP_EDIT == m_op)
		{
			m_strNumTmp = m_strNum;
			m_strNameTmp = m_strName;
			m_strClassTmp = m_strClass;
			m_strEmailTmp = m_strEmail;
			m_strTelTmp = m_strTel;
			m_strBlueteethTmp = m_strBlueteeth;
			m_iAgeTmp = m_iAge;
		}
		UpdateData(FALSE);
	}
	
	return TRUE;
}

/////////////////在屏幕上显示图像///////////////////
void CDlgStunfo::DrawUserPhoto(int x, int y, CDC *pDC)
{
	if(!m_hPhotoBitmap) return;
	BITMAP   Bitmap; 
	int   n=GetObject(m_hPhotoBitmap,sizeof(BITMAP),(LPSTR)&Bitmap); 
	// bitmap.GetBitmap(pBitmap); 
	int   wWidth=Bitmap.bmWidth   ; 
	int   wHeight=Bitmap.bmHeight; 

	HBITMAP OldBitmap;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	OldBitmap=(HBITMAP)MemDC.SelectObject(m_hPhotoBitmap);
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(x,y,150,180, &MemDC,0,0,wWidth,wHeight,SRCCOPY); 
//	pDC->BitBlt(x,y,150,180,&MemDC,0,0,SRCCOPY);
	MemDC.SelectObject(OldBitmap);
}
///////////将内存中的BMP文件内容转换到HBITMAP///////
HBITMAP CDlgStunfo::BufferToHBITMAP()
{
	HBITMAP				hBmp;
	LPSTR				hDIB,lpBuffer = m_pBMPBuffer;
	LPVOID				lpDIBBits;
	BITMAPFILEHEADER	bmfHeader;
	DWORD				bmfHeaderLen;

	bmfHeaderLen = sizeof(bmfHeader);
	strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen);
	//	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B')) return NULL;
	if (bmfHeader.bfType != (*(WORD*)"BM")) return NULL;//我copy《Windows程序设计》上的做法。
	hDIB = lpBuffer + bmfHeaderLen;
	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	/*	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 1 << bmiHeader.biBitCount; 
	if( bmInfo.bmiHeader.biBitCount > 8 )
	lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
	((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
	lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	*///原来的代码。
	lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits;//这行功能和上面被注释掉的代码相同，容易理解。
	CClientDC dc(this);
	hBmp = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,&bmInfo,DIB_RGB_COLORS);
	return hBmp;
}
void CDlgStunfo::OnOK() 
{
	// TODO: Add extra validation here
	if (OP_VIEW == m_op)
	{
		CDialog::OnOK();
	}

	UpdateData(TRUE);
	int index = m_class.GetCurSel();
	if(index != -1)
		m_class.GetLBText(index,m_strClass);
	else
		m_strClass = "";
	index = m_gender.GetCurSel();
	if(index != -1)
		m_gender.GetLBText(index,m_strGender);

	if(m_strNum == "")
	{
		AfxMessageBox(L"没有输入学生学号，请输入学生学号！");
		GetDlgItem(IDC_DM_STU_NUM)->SetFocus();
		return;
	}
	if(m_strName == "")
	{
		AfxMessageBox(L"没有输入学生姓名，请输入学生姓名！");
		GetDlgItem(IDC_DM_STU_NAME)->SetFocus();
		return;
	}
	if(m_strGender == "")
	{
		AfxMessageBox(L"没有选择学生性别，请选择学生性别！");
		GetDlgItem(IDC_DM_STU_GENDER)->SetFocus();
		return;
	}
	if(m_strClass == "")
	{
		AfxMessageBox(L"没有选择学生班级，请选择学生班级！");
		GetDlgItem(IDC_DM_STU_CLASS)->SetFocus();
		return;
	}
	HRESULT hr = E_FAIL;
	//要判断学号不能与其他的学生相同
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		std::list<CYDObjectRef*> lstObj;
		CListAutoClean<CYDObjectRef> clr1(lstObj);
		std::list<CPropQueryContidition*> lstQueryCon;
		CListAutoClean<CPropQueryContidition> clr2(lstQueryCon);
		CPropQueryContidition* pQueryConditon = new CPropQueryContidition();
		pQueryConditon->m_uOpType = Q_EQUAL;
		pQueryConditon->m_strFiledName = FIELD_YDSTU_ID;
		pQueryConditon->m_strConVal = m_strNum;
		pQueryConditon->m_uFieldType = VT_BSTR;
		lstQueryCon.push_back(pQueryConditon);
		hr = CStaticObjHelper::GetObjRef(DB_YDSTUINFO,pDB,&lstObj,&lstQueryCon);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		if (OP_NEW == m_op)
		{
			if(lstObj.size() > 0)
			{
				AfxMessageBox(_T("学号不能重复，请重新输入学号！"));
				return;
			}
		}
		else if(OP_EDIT == m_op)
		{
			OBJID stuid = ID_EMPTY;
			BOOL bFind = FALSE;
			hr = m_pStudent->GetID(&stuid);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return;
			}
			for(std::list<CYDObjectRef*>::const_iterator itr = lstObj.begin();
				itr != lstObj.end();++itr)
			{
				OBJID stuitrID = ID_EMPTY;
				hr = (*itr)->GetID(&stuitrID);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return;
				}
				if(stuitrID != stuid)
				{
					bFind = TRUE;
					break;
				}
			}
			if(bFind)
			{
				if(lstObj.size() > 0)
				{
					AfxMessageBox(_T("学号不能重复，请重新输入学号！"));
					return;
				}
			}
		}
	}
	if (OP_NEW == m_op)
	{
		HRESULT hr = Save();
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	else if (OP_EDIT == m_op)
	{
		HRESULT hr = Update();
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	CDialog::OnOK();
}
extern CString GetExePath();


void CDlgStunfo::OnBtnSelPic() 
{
	CString strFilter = L"All Files (*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE, NULL, L"*.bmp", NULL, strFilter);

	if ( dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetPathName();
		m_strPath.Replace(L"\\", L"\\\\");
		ReadPhotoBuf(m_strPath);
		m_hPhotoBitmap=(HBITMAP)::LoadImage(NULL,m_strPath,IMAGE_BITMAP,180,200,LR_LOADFROMFILE);  
	}	
}


void CDlgStunfo::OnSelchangeDepartment() 
{
	CString strSelDept;
	m_department.GetWindowText(strSelDept);

	for (int i = m_class.GetCount() - 1; i >= 0; i--)
	{
		m_class.DeleteString(i);
	}
	ClearClassCache();

	if (strSelDept.GetLength() > 0)
	{
		CYDDepartmentRef* pDept = (CYDDepartmentRef*)m_department.GetItemData(m_department.GetCurSel());
		ASSERT(pDept);
		HRESULT hr = pDept->GetAllClasses(&m_lstClsCache, NULL);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if (m_lstClsCache.size() > 0)
		{
			int nIndex = 0;
			for (std::list<CYDObjectRef*>::const_iterator itr = m_lstClsCache.begin();
				itr != m_lstClsCache.end(); ++itr, ++nIndex)
			{
				CString strClsName;
				(*itr)->GetPropVal(FIELD_YDCLASS_NAME, strClsName);
				m_class.InsertString(nIndex, strClsName);
				m_class.SetItemData(nIndex, (DWORD_PTR)(*itr));
			}
		}
	}
	m_class.SetFocus();
}

void CDlgStunfo::ClearClassCache()
{
	CListAutoClean<CYDObjectRef> clr2(m_lstClsCache);
}

void CDlgStunfo::ReadOnly(BOOL bReadOnly)
{
	GetDlgItem(IDC_DM_STU_NUM)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_NAME)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_DEPARTMENT)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_CLASS)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_GENDER)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_AGE)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_TEL)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_EMIAL)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_DM_STU_BLUETEETH)->EnableWindow(!bReadOnly);
	GetDlgItem(IDC_BTN_DM_STU_SELPIC)->EnableWindow(!bReadOnly);
}

HRESULT CDlgStunfo::Save()
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	m_pStudent = new CYdStudentRef(pDB);
	
	CComVariant varNum(m_strNum);
	m_pStudent->SetPropVal(FIELD_YDSTU_ID, &varNum);

	CComVariant varName(m_strName);
	m_pStudent->SetPropVal(FIELD_YDSTU_NAME, &varName);

	CComVariant varAge((long)m_iAge);
	m_pStudent->SetPropVal(FIELD_YDSTU_AGE, &varAge);

	CComVariant varTel(m_strTel);
	m_pStudent->SetPropVal(FIELD_YDSTU_TEL, &varTel);

	CComVariant varEMail(m_strEmail);
	m_pStudent->SetPropVal(FIELD_YDSTU_MAIL, &varEMail);

	CComVariant varBlueteech(m_strBlueteeth);
	m_pStudent->SetPropVal(FIELD_YDSTU_EVID, &varBlueteech);
	m_pStudent->SetGender(m_strGender);
	//头像

	CComVariant varBMP;
	hr = GetPhotoData(&varBMP);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pStudent->SetPropVal(FIELD_YDSTU_PHOTO, &varBMP);
	//班级关系
	int index = m_class.GetCurSel();
	ASSERT(index != -1);
	CYDObjectRef* pObjClass = (CYDObjectRef*)m_class.GetItemData(index);
	ASSERT(pObjClass != NULL);
	CYDLinkRef classToStu(pDB, DB_YDLINK_CLASS_STUDENT);
	classToStu.PutObjRef(pObjClass, m_pStudent);

	CDBTransactionRef trans(pDB, TRUE);
	hr = m_pStudent->Save();
	if (FAILED(hr))
	{
		return hr;
	}
	hr = classToStu.Save();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = trans.Commit();
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CDlgStunfo::ReadPhotoBuf(CString _strfile)
{
	DestroyPhoto();
	if(m_strPath.IsEmpty())
	{
		return S_OK;
	}
	CFile file;
	if( !file.Open( m_strPath, CFile::modeRead) )
		return S_FALSE;
	m_dwFileLen = (DWORD)file.GetLength();
	m_pBMPBuffer = new char[m_dwFileLen + 1];
	if(!m_pBMPBuffer)
	{
		return S_FALSE;
	}
	if(file.Read(m_pBMPBuffer,m_dwFileLen) != m_dwFileLen)
	{
		return S_FALSE;
	}
	return S_OK;
}
HRESULT CDlgStunfo::GetPhotoData(VARIANT* _pVal)
{
	HRESULT hr = E_FAIL;
	
	//VARIANT			varBLOB;
	SAFEARRAY		*psa;
	SAFEARRAYBOUND	rgsabound[1];
	char			*pBuf = m_pBMPBuffer;
	if(pBuf)
	{    
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = m_dwFileLen;
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		for (long i = 0; i < (long)m_dwFileLen; i++)
			SafeArrayPutElement (psa, &i, pBuf++);
		_pVal->vt = VT_ARRAY | VT_UI1;
		_pVal->parray = psa;
	}
	return S_OK;
}

HRESULT CDlgStunfo::Update()
{
	HRESULT hr = E_FAIL;
	
	CComVariant varNum(m_strNum);
	m_pStudent->SetPropVal(FIELD_YDSTU_ID, &varNum);

	CComVariant varName(m_strName);
	m_pStudent->SetPropVal(FIELD_YDSTU_NAME, &varName);

	CComVariant varAge((long)m_iAge);  
	m_pStudent->SetPropVal(FIELD_YDSTU_AGE, &varAge);

	CComVariant varTel(m_strTel);
	m_pStudent->SetPropVal(FIELD_YDSTU_TEL, &varTel);

	CComVariant varEMail(m_strEmail);
	m_pStudent->SetPropVal(FIELD_YDSTU_MAIL, &varEMail);

	CComVariant varBlueteech(m_strBlueteeth);
	m_pStudent->SetPropVal(FIELD_YDSTU_EVID, &varBlueteech);
	m_pStudent->SetGender(m_strGender);

	int index = m_class.GetCurSel();
	ASSERT(index != -1);
	CYDObjectRef* pObjClass = (CYDObjectRef*)m_class.GetItemData(index);
	ASSERT(pObjClass != NULL);
	OBJID idClass = ID_EMPTY;
	pObjClass->GetID(&idClass);
	OBJID idClassOr = ID_EMPTY;
	m_pClassWrapper->m_pObjRef->GetID(&idClassOr);
	BOOL bClassChange = FALSE;
	if (idClassOr != idClass)
	{
		bClassChange = TRUE;
	}

	CComVariant valPhoto;
	hr = GetPhotoData(&valPhoto);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pStudent->SetPropVal(FIELD_YDSTU_PHOTO, &valPhoto);
	if (FAILED(hr))
	{
		return hr;
	}
	//头像
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	CDBTransactionRef trans(pDB, TRUE);
	hr = m_pStudent->Update();
	if (FAILED(hr))
	{
		return hr;
	}
	if (bClassChange)
	{
		hr = m_pClassWrapper->m_pLinkRef->Remove();
		if (FAILED(hr))
		{
			return hr;
		}
		CYDLinkRef linkNew(pDB, DB_YDLINK_CLASS_STUDENT);
		linkNew.PutObjRef(pObjClass, m_pStudent);
		hr = linkNew.Save();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	hr = trans.Commit();
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
INT_PTR CDlgStunfo::DoModal()
{
	CExtDllState extdllstate;
	return CDialog::DoModal();
}

void CDlgStunfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CDlgStunfo::DestroyPhoto()
{
	if(m_hPhotoBitmap)
	{
		DeleteObject(m_hPhotoBitmap);
		m_hPhotoBitmap = NULL;
	}
	if(m_pBMPBuffer)
	{
		delete m_pBMPBuffer;
		m_pBMPBuffer = NULL;
	}
}

void CDlgStunfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	DrawUserPhoto(307,30,&dc);
	CDialog::OnPaint();
}