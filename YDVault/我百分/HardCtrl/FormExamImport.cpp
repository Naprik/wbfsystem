// FormExamImport.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamImport.h"
#include "MainFrm.h"
#include "BackUp.h"
#include "DBStuInfo.h"
#include "DBClass.h"
#include "DBPaper.h"
#include "DBExamaddr.h"
#include "DBResult.h"

IMPLEMENT_DYNCREATE(CFormExamImport, CDialog)
//extern CExamDetail  m_examdeteil;

CFormExamImport::CFormExamImport(CWnd* pParent /*=NULL*/)
: cdxCSizingDialog(CFormExamImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamImport)
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_IMPORT, m_import);
	DDX_Control(pDX, IDC_STULIST1, m_stulist);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamImport, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamImport)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamImport message handlers

BOOL CFormExamImport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_IMPORT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SAVE),mdRelative,mdRelative);
	AddSzControl(m_paperlist,mdRelative,mdRelative);
	AddSzControl(m_stulist,mdRelative,mdRelative);
	
    DWORD dwStyle=GetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_paperlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_paperlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_paperlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_paperlist.SetExtendedStyle(dwStyle);
	
    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_paperlist.ClearHeadings();
	m_paperlist.SetHeadings("������Ϣ,100;����,200");
	
	dwStyle=GetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_stulist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_stulist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_stulist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_stulist.SetExtendedStyle(dwStyle);
	
    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_stulist.ClearHeadings();
	
	m_stulist.SetHeadings("ѧ��,150;����,100;�༶,100;׼��֤��,100;����ʱ��,150;����ʱ��,150;ѧ����,100");
	
	return TRUE;  
}

void CFormExamImport::OnImport() 
{
	static char BASED_CODE szFilter[] = "All Files (*.ini)|*.ini||";
	CFileDialog dlg(TRUE,NULL,"*.ini",NULL,szFilter);
	CString strPath = "";
	if (dlg.DoModal() == IDOK)
	{
		m_paperlist.DeleteAllItems();
		m_stulist.DeleteAllItems();
		strPath = dlg.GetPathName();
		strPath.Replace("\\", "\\\\");
		CString	  path = strPath;	
	}
	else
		return;
	
	m_detail.Initdata();

	CBackUp bk;
	CString strRoll = bk.Load("ROLLMODE",strPath);
 	m_detail.nRoll_pattern = atoi(strRoll);

 	CString strRolling  = bk.Load("ROLLINGMODE", strPath);
	m_detail.nRolling_pattern = atoi(strRolling);

 	CString strExamquality = bk.Load("EXAMQUALITY", strPath); 	
 	m_detail.nExam_quality = atoi(strExamquality);

	int iCount = 0;
	while (true)
	{
		CString strTitle = "";
		strTitle.Format("CLASS%d", iCount);
		CString strClass = bk.Load(strTitle, "CLASS", strPath);
		if (strClass != "")
		{
			m_detail.arryClass.Add(strClass);
		}
		else
			break;
		iCount++;
	}

 	CString strExamAddr = bk.Load("EXAMADDR", strPath);
 	m_detail.sAddress = strExamAddr;
 
 	CString strPaperName = bk.Load("PAPERNAME", strPath);
 	m_detail.sPapername = strPaperName;
 
 	CString strSubject = bk.Load("SUBJECTNAME", strPath);
 	m_detail.sSubject = strSubject;
 
 
 	CString strStartTime = bk.Load("STARTTIME", strPath);
 	m_detail.sStart = strStartTime;
 
 	CString strEndTime = bk.Load("ENDTIME", strPath);
 	m_detail.sEnd = strEndTime;
 
 	CString strStartNO = bk.Load("STARTNO", strPath);
 	m_detail.nStartNO = atoi(strStartNO);
 
 	CString strEndNO = bk.Load("ENDNO", strPath);
 	m_detail.nEndNO = atoi(strEndNO);
 
 	CString strMaxNO = bk.Load("MAXNO", strPath);
 	m_detail.nMaxNO = atoi(strMaxNO);
 
 	CString strListen = bk.Load("LISTEN", strPath);
 	if (strListen == "1")
 	{
 		m_detail.b_listen = TRUE;
 		CString strPart1 = bk.Load("PART1", strPath);
 		if (strPart1 == "1")
 		{
 			m_detail.b_part1 = TRUE;
 			CString strStarttime1 = bk.Load("STARTPART1", strPath);
 			m_detail.m_start1.ParseDateTime(strStarttime1);
			CString strEndTime1 = bk.Load("ENDTPART1", strPath);
 			m_detail.m_end1.ParseDateTime(strEndTime1);
 			
 		}
 		else
 			m_detail.b_part1 = FALSE;
 	
 		CString strPart2 = bk.Load("PART2", strPath);
 		if (strPart2 == "1")
 		{
 			m_detail.b_part2 = TRUE;
 			CString strStarttime2 = bk.Load("STARTPART2", strPath);
 			m_detail.m_start2.ParseDateTime(strStarttime2);
 			CString strEndTime2 = bk.Load("ENDTPART2", strPath);
 			m_detail.m_end2.ParseDateTime(strEndTime2);
 
 		}
 		else
 			m_detail.b_part2 = FALSE;
 
 		CString strPart3 = bk.Load("PART3", strPath);
 		if (strPart3 == "1")
 		{
 			m_detail.b_part3 = TRUE;
 			CString strStarttime3 = bk.Load("STARTPART3", strPath);
 			m_detail.m_start3.ParseDateTime(strStarttime3);
 			CString strEndTime3 = bk.Load("ENDTPART3", strPath);
 			m_detail.m_end3.ParseDateTime(strEndTime3);
 		}
 		else
 			m_detail.b_part3 = FALSE;
 
 	}
 	else
 		m_detail.b_listen = FALSE;
 
 	CString strChanel = bk.Load("CHANEL", strPath);
 	m_detail.m_chanal = strChanel;
 
 	
	iCount = 0;
 
  	while(true)
  	{
		CString strTitle = "";
 		strTitle.Format("STUINFO%d", iCount);
 		CString strStuID = bk.Load(strTitle, "STUID", strPath);
 		if (strStuID != "")
 		{
			CString strStuAnswer = bk.Load(strTitle, "STUANSWER", strPath);
 			CStuInfo  stu;
 			stu.sStuID = strStuID;
			stu.sStuAnswer = strStuAnswer;
 			m_detail.m_arryStuIfo.Add(stu);
 		}
 		else
 			break;
 		iCount++;
  	}

	if (m_detail.nRoll_pattern == 1)
	{
		m_paperlist.AddItem(0, "������ʽ","��������");
	}
	else if (m_detail.nRoll_pattern == 2)
	{
		m_paperlist.AddItem(0, "������ʽ","�������");
	}
	else
		m_paperlist.AddItem(0, "������ʽ","USB����");
	
	if (m_detail.nRolling_pattern == 1)
	{
		m_paperlist.AddItem(0, "�վ�ʽ","�����վ�");
	}
	else if (m_detail.nRolling_pattern == 2)
	{
		m_paperlist.AddItem(0, "�վ�ʽ","�����վ�");
	}
	else
		m_paperlist.AddItem(0, "�վ�ʽ","USB�վ�");
	
	if (m_detail.nExam_quality == 1)
	{
		m_paperlist.AddItem(0, "��������","���濼��");
	}
	else
		m_paperlist.AddItem(0, "��������","һ�����");
	
	CString temp = "";
	for (int i = 0; i < m_detail.arryClass.GetSize(); i++)
	{
		temp +=  m_detail.arryClass[i] + " ";
	}
	m_paperlist.AddItem(0, "�μӿ��԰༶",temp);
	
	m_paperlist.AddItem(0, "����",m_detail.sAddress);
	m_paperlist.AddItem(0, "�Ծ�����",m_detail.sPapername);
	//	m_list.AddItem(0, "�Ծ��Ŀ",m_examdeteil.sSubject);
	m_paperlist.AddItem(0, "����ʱ��",m_detail.sStart);
	m_paperlist.AddItem(0, "�վ�ʱ��",m_detail.sEnd);
	
	temp = "";
	temp.Format("%d", m_detail.nStartNO);
	m_paperlist.AddItem(0, "��ʼ���",temp);
	
	temp = "";
	temp.Format("%d", m_detail.nEndNO);
	m_paperlist.AddItem(0, "�������",temp);
	
	temp = "";
	temp.Format("%d", m_detail.nMaxNO);
	m_paperlist.AddItem(0, "������", temp);
	
	if (m_detail.b_listen)
	{
		if (m_detail.b_part1)
		{
			m_paperlist.AddItem(0, "����1 ��ʼʱ��",m_detail.m_start1.Format());
			m_paperlist.AddItem(0, "����1 ����ʱ��",m_detail.m_end1.Format());
		}
		if (m_detail.b_part2)
		{
			m_paperlist.AddItem(0, "����2 ��ʼʱ��",m_detail.m_start2.Format());
			m_paperlist.AddItem(0, "����2 ����ʱ��",m_detail.m_end2.Format());
		}
		if (m_detail.b_part3)
		{
			m_paperlist.AddItem(0, "����3 ��ʼʱ��",m_detail.m_start3.Format());
			m_paperlist.AddItem(0, "����3 ����ʱ��",m_detail.m_end3.Format());
		}
		
		m_paperlist.AddItem(0, "����Ƶ��",m_detail.m_chanal);
	}

	iCount = 1;
	CoInitialize(NULL);
	CDBStuInfo m_stuinfodb;
	if (!m_stuinfodb.Connect())
	{
		AfxMessageBox("m_stuinfodb: �޷����ӵ����ݿ������");
	}
	else
	{
		CDBClass m_classdb;
		if (!m_classdb.Connect())
		{
			AfxMessageBox("m_classdb: �޷����ӵ����ݿ������");
		}
		else
		{
			for (i = 0; i < m_detail.m_arryStuIfo.GetSize(); i++)
			{
				//m_stulist.AddItem(0,m_detail.m_arryStuIfo[i].sStuID,"","","","","",m_detail.m_arryStuIfo[i].sStuAnswer);
				CString strStuid = m_detail.m_arryStuIfo[i].sStuID;
				CString strStuName = "";
				CString strClassid = "";
				CString strClassName = "";
				BOOL stuinfoflag = m_stuinfodb.LoadByStuid(strStuid);
				if (stuinfoflag)
				{
					strStuName = m_stuinfodb.m_stuname;
					strClassid = m_stuinfodb.m_stuclassid;
				}
				
				BOOL classflag = m_classdb.LoadByClassid(strClassid);
				if (classflag)
				{
					strClassName = m_classdb.m_classname;
				}
				m_stulist.AddItem(0,m_detail.m_arryStuIfo[i].sStuID,strStuName,strClassName,"",m_detail.sStart,m_detail.sEnd,m_detail.m_arryStuIfo[i].sStuAnswer);
			}
			m_classdb.Close();
		}
		m_stuinfodb.Close();
	}

	CoUninitialize();
}

void CFormExamImport::OnSave() 
{
	if (m_detail.IsEmpty())
	{
		AfxMessageBox("û�е��뿼���ļ�������𰸣�");
		return;
	}

	if (m_stulist.GetItemCount() == 0)
	{
		AfxMessageBox("û��ѧ���𰸣��޷����棡");
		return;
	}

	CString strPapername = m_detail.sPapername;
	COleDateTime tStarttime, tEndtime;
	tStarttime.ParseDateTime(m_detail.sStart);
	tEndtime.ParseDateTime(m_detail.sEnd);
	CoInitialize(NULL);
	CDBPaper m_paperdb;
	if (!m_paperdb.Connect())
	{
		AfxMessageBox("m_paperdb: �޷����ӵ����ݿ������");
	}
	else
	{
		BOOL paperflag = m_paperdb.LoadByPaperName(strPapername);
		if (!paperflag)
		{
			CString msg;
			msg.Format("�������Ծ��Ƿ�Ҫ��Ӹ��Ծ�?");
			if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
				return;

			CString strSubject = m_detail.sSubject;
			CString strClass = "";
			CString strSubjectid = "";
			CString strClassid = "";
			if (m_detail.arryClass.GetSize() >= 1)
			{
				strClass = m_detail.arryClass[0];
			}

			int startno = m_detail.nStartNO;
			int endno = m_detail.nEndNO;
			int maxno = m_detail.nMaxNO;
			
			CDBSubject m_subjectdb;
			if (!m_subjectdb.Connect())
			{
				AfxMessageBox("m_subjectdb: �޷����ӵ����ݿ������");
			}
			else
			{
				BOOL subjectflag = m_subjectdb.LoadBySubject(strSubject);
				if (subjectflag)
				{
					strSubjectid = m_subjectdb.m_subjectid;
				}
				m_subjectdb.Close();
			}

			CDBClass m_classdb;
			if (!m_classdb.Connect())
			{
				AfxMessageBox("m_subjectdb: �޷����ӵ����ݿ������");
			}
			else
			{
				BOOL classflag = m_classdb.LoadByClassname(strClass);
				if (classflag)
				{
					strClassid = m_classdb.m_classid;
				}
				m_classdb.Close();
			}

			m_paperdb.m_ydid = 0;
			m_paperdb.m_papername = strPapername;
			m_paperdb.m_subjectid = strSubjectid;
			m_paperdb.m_classid = strClassid;
			m_paperdb.m_starttime = tStarttime;
			m_paperdb.m_endtime = tEndtime;
			m_paperdb.m_startNO = startno;
			m_paperdb.m_endNO = endno;
			m_paperdb.m_maxNO = maxno;

			if (m_paperdb.Add())
			{
				AfxMessageBox("����Ծ�ɹ���");
			}
			else
				AfxMessageBox("����Ծ�ʧ�ܣ�");
		}

		m_paperdb.Close();
	}

	Sleep(500);
	if (!m_paperdb.Connect())
	{
		AfxMessageBox("m_paperdb: �޷����ӵ����ݿ������");
	}
	else
	{
		int nPaperid = -1;
		CString strExamaddrID = "";
		BOOL paperflag = m_paperdb.LoadByPaperName(strPapername);
		if (paperflag)
		{
			nPaperid = m_paperdb.m_ydid;
		}
		if (m_detail.sAddress != "")
		{
			CDBExamaddr m_examaddrdb;
			if (!m_examaddrdb.Connect())
			{
				AfxMessageBox("m_examaddrdb: �޷����ӵ����ݿ������");
			}
			else
			{
				BOOL examaddrflag = m_examaddrdb.LoadByAddr(m_detail.sAddress);
				if (examaddrflag)
				{
					strExamaddrID = m_examaddrdb.m_addrid;
				}
				m_examaddrdb.Close();
			}

		}
		CDBResult m_resultdb;
		if (!m_resultdb.Connect())
		{
			AfxMessageBox("m_resultdb: �޷����ӵ����ݿ������");
		}
		else
		{
			for (int i = 0; i < m_detail.m_arryStuIfo.GetSize(); i++)
			{
				m_resultdb.m_ydid = 0;
				m_resultdb.m_stuid = m_detail.m_arryStuIfo[i].sStuID;
				m_resultdb.m_paperid = nPaperid;
				m_resultdb.m_starttime = tStarttime;
				m_resultdb.m_endtime = tEndtime;
				m_resultdb.m_addrid = strExamaddrID;
				m_resultdb.m_stuanswer = m_detail.m_arryStuIfo[i].sStuAnswer;

				if (!m_resultdb.Add())
				{
					CString msg;
					msg.Format("���ѧ��%s�Ĵ�ʧ�ܣ�", m_detail.m_arryStuIfo[i].sStuID);
					AfxMessageBox(msg);
				}
			}
			AfxMessageBox("����ѧ���𰸳ɹ���");
			m_resultdb.Close();
		}
		
		m_paperdb.Close();
	}
	CoUninitialize();
}
