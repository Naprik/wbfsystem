// FormCreatFile.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormCreatFile.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "BackUp.h"
#include "Base.h"
#include "DBStuInfo.h"
#include "DBClass.h"


extern CExamDetail  m_examdeteil;
IMPLEMENT_DYNCREATE(CFormCreatFile, CDialog)


CFormCreatFile::CFormCreatFile(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormCreatFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormCreatFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormCreatFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormCreatFile)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_CREATE, m_create);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormCreatFile, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormCreatFile)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormCreatFile message handlers

BOOL CFormCreatFile::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_CREATE),mdRelative,mdRelative);
	AddSzControl(m_list,mdRelative,mdRelative);
	
    DWORD dwStyle=GetWindowLong(m_list.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_list.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_list.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_list.SetExtendedStyle(dwStyle);
	
    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_list.ClearHeadings();
	m_list.SetHeadings("������Ϣ,100;����,200");	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormCreatFile::SetInfo()
{
	m_list.DeleteAllItems();
	if (m_examdeteil.nRoll_pattern == 1)
	{
		m_list.AddItem(0, "������ʽ","��������");
	}
	else if (m_examdeteil.nRoll_pattern == 2)
	{
		m_list.AddItem(0, "������ʽ","�������");
	}
	else
		m_list.AddItem(0, "������ʽ","USB����");
	
	if (m_examdeteil.nRolling_pattern == 1)
	{
		m_list.AddItem(0, "�վ�ʽ","�����վ�");
	}
	else if (m_examdeteil.nRolling_pattern == 2)
	{
		m_list.AddItem(0, "�վ�ʽ","�����վ�");
	}
	else
		m_list.AddItem(0, "�վ�ʽ","USB�վ�");
	
	if (m_examdeteil.nExam_quality == 1)
	{
		m_list.AddItem(0, "��������","���濼��");
	}
	else
		m_list.AddItem(0, "��������","һ�����");
	
	CString temp = "";
	for (int i = 0; i < m_examdeteil.arryClass.GetSize(); i++)
	{
		temp +=  m_examdeteil.arryClass[i];
	}
	m_list.AddItem(0, "�μӿ��԰༶",temp);
	
	m_list.AddItem(0, "����",m_examdeteil.sAddress);
	m_list.AddItem(0, "�Ծ�����",m_examdeteil.sPapername);
	//	m_list.AddItem(0, "�Ծ��Ŀ",m_examdeteil.sSubject);
	m_list.AddItem(0, "����ʱ��",m_examdeteil.sStart);
	m_list.AddItem(0, "�վ�ʱ��",m_examdeteil.sEnd);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nStartNO);
	m_list.AddItem(0, "��ʼ���",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nEndNO);
	m_list.AddItem(0, "�������",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nMaxNO);
	m_list.AddItem(0, "������", temp);
	
	if (m_examdeteil.b_listen)
	{
		if (m_examdeteil.b_part1)
		{
			m_list.AddItem(0, "����1 ��ʼʱ��",m_examdeteil.m_start1.Format());
			m_list.AddItem(0, "����1 ����ʱ��",m_examdeteil.m_end1.Format());
		}
		if (m_examdeteil.b_part2)
		{
			m_list.AddItem(0, "����2 ��ʼʱ��",m_examdeteil.m_start2.Format());
			m_list.AddItem(0, "����2 ����ʱ��",m_examdeteil.m_end2.Format());
		}
		if (m_examdeteil.b_part3)
		{
			m_list.AddItem(0, "����3 ��ʼʱ��",m_examdeteil.m_start3.Format());
			m_list.AddItem(0, "����3 ����ʱ��",m_examdeteil.m_end3.Format());
		}
		
		m_list.AddItem(0, "����Ƶ��",m_examdeteil.m_chanal);
	}
}



void CFormCreatFile::OnCreate() 
{
	CBackUp bk;
	bk.Remove(TRUE);
	
	CString temp = "";
	temp.Format("%d", m_examdeteil.nRoll_pattern);
	bk.Createfile("ROLLMODE",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nRolling_pattern);
	bk.Createfile("ROLLINGMODE",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nExam_quality);
	bk.Createfile("EXAMQUALITY",temp);
	
	// 	temp = "";
	// 	for (int i = 0; i < m_examdeteil.arryClass.GetSize(); i++)
	// 	{
	// 		temp +=  m_examdeteil.arryClass[i];
	// 	}
	// 	bk.Createfile("CLASS",temp);
	for (int i = 0; i < m_examdeteil.arryClass.GetSize(); i++)
	{
		CString sTitle = "";
		sTitle.Format("CLASS%d", i);
		bk.Createfile(sTitle, "CLASS", m_examdeteil.arryClass[i]);
	}
	
	bk.Createfile("EXAMADDR",m_examdeteil.sAddress);
	bk.Createfile("PAPERNAME",m_examdeteil.sPapername);
	bk.Createfile("SUBJECTNAME",m_examdeteil.sSubject);
	bk.Createfile("STARTTIME",m_examdeteil.sStart);
	bk.Createfile("ENDTIME",m_examdeteil.sEnd);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nStartNO);
	bk.Createfile("STARTNO",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nEndNO);
	bk.Createfile("ENDNO",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nMaxNO);
	bk.Createfile("MAXNO", temp);
	
	if(m_examdeteil.b_listen)
	{
		bk.Createfile("LISTEN","1");
		
		if (m_examdeteil.b_part1)
		{
			bk.Createfile("PART1","1");
			bk.Createfile("STARTPART1", m_examdeteil.m_start1.Format());
			bk.Createfile("ENDTPART1", m_examdeteil.m_end1.Format());
		}	
		else
			bk.Createfile("PART1","0");
		
		if (m_examdeteil.b_part2)
		{
			bk.Createfile("PART2","1");
			bk.Createfile("STARTPART2", m_examdeteil.m_start2.Format());
			bk.Createfile("ENDTPART2", m_examdeteil.m_end2.Format());
		}
		else
			bk.Createfile("PART2","0");
		
		if (m_examdeteil.b_part3)
		{
			bk.Createfile("PART3","1");
			bk.Createfile("STARTPART3", m_examdeteil.m_start3.Format());
			bk.Createfile("ENDTPART3", m_examdeteil.m_end3.Format());
		}
		else
			bk.Createfile("PART3","0");
	}
	else
		bk.Createfile("LISTEN","0");
	
	bk.Createfile("CHANEL", m_examdeteil.m_chanal);
	
	int iCount = 1;
	CoInitialize(NULL);
	CDBClass  m_classdb;
	if (!m_classdb.Connect())
	{
		AfxMessageBox("m_classdb:�޷����ӵ����ݿ������");
	}
	else
	{
		CDBStuInfo m_stuinfo;
		if (!m_stuinfo.Connect())
		{
			AfxMessageBox("m_stuinfo:�޷����ӵ����ݿ������");
		}
		else
		{
			for (int j = 0; j < m_examdeteil.arryClass.GetSize(); j++)
			{
				CString strClass = m_examdeteil.arryClass[j];
				strClass.TrimLeft();
				strClass.TrimRight();
				BOOL flagclass = m_classdb.LoadByClassname(strClass);
				if (flagclass)
				{
					CString strClassid = m_classdb.m_classid;
					strClassid.TrimLeft();
					strClassid.TrimRight();
					BOOL flagstuinfo = m_stuinfo.LoadByClassid(strClassid);
					while (flagstuinfo)
					{
						CString strStuID = m_stuinfo.m_stuid;
						CString strStuName = m_stuinfo.m_stuname;
						//CString strClass = strClass;
						CString strDev = m_stuinfo.m_devid;
						CString sTitle = "";
						sTitle.Format("STUINFO%d", iCount);
						bk.Createfile(sTitle, "STUID", strStuID);
						bk.Createfile(sTitle, "STUNAME", strStuName);
						bk.Createfile(sTitle, "STUCLASS", strClass);
						bk.Createfile(sTitle, "STUDEV", strDev);
						flagstuinfo = m_stuinfo.Move();
						iCount++;
					}
				}
			}

			m_stuinfo.Close();
		}
		
		m_classdb.Close();
	}
	CoUninitialize();

	
	CBase base;
	CJahCoverCapture capture;
	capture.m_bmpFile =base.GetExePath()+"temp.ini";	
	
	//	CBackUp bk;
	CString title = bk.Load("PAPERNAME", capture.m_bmpFile);
	CFileDialog dlg(FALSE,"*.ini",title,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"ini Files (*.ini)|*.ini|All Files (*.*)|*.*||");
	CString path = "";
	if(dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();	
		BOOL flag = CopyFile(capture.m_bmpFile,path,FALSE);
		if(flag)
			AfxMessageBox("�����ɹ���");
		else
			AfxMessageBox("����ʧ�ܣ�");
	}
	
}
