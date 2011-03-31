#include "stdafx.h"
#include "hardctrl.h"
#include "FormImport.h"
#include "MainFrm.h"
#include "BackUp.h"
#include "DBStuInfo.h"
#include "DBPaper.h"
#include "DBResult.h"


extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFormImport, CDialog)

CFormImport::CFormImport(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormImport)
	DDX_Control(pDX, IDC_EXPORT, m_export);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_ANSWERLIST, m_answerlist);
	DDX_Control(pDX, IDC_IMPORT, m_import);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormImport, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormImport)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormImport message handlers



BOOL CFormImport::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	

	AddSzControl(*GetDlgItem(IDC_IMPORT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SAVE),mdNone,mdRepos);	
	AddSzControl(*GetDlgItem(IDC_EXPORT),mdNone,mdRepos);	
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdResize,mdResize);

	AddSzControl(m_paperlist,mdResize,mdResize);
	AddSzControl(m_answerlist,mdResize,mdResize);	

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
	m_paperlist.SetHeadings("考卷名称,100;考试科目,100; 参考院系,100; 参考班级,100;开考时间,150;收卷时间,150;开始题号,100;结束题号,100;最大题号,100"); 

	dwStyle=GetWindowLong(m_answerlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_answerlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_answerlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_answerlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_answerlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_answerlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	m_answerlist.ClearHeadings();
	m_answerlist.SetHeadings("学号,100;学生姓名,100;院系,100;班级,100;考场,100;答案,300");
	return TRUE; 
}
char convertHexData(char ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
    else return(-1);
}

int string2Hex(CString str, char *SendOut)
{
	int hexdata,lowhexdata; 
	int hexdatalen=0;
	int len=str.GetLength();
	for(int i=0;i<len;)
	{
		char lstr,hstr=str[i];
		if(hstr==' '||hstr=='\r'||hstr=='\n')
		{
			i++;
			continue;
		}
		i++;
		if (i>=len)
			break;
		lstr=str[i];
		hexdata=convertHexData(hstr);
		lowhexdata=convertHexData(lstr);
		if((hexdata==16)||(lowhexdata==16))
			break;
		else
			hexdata=hexdata*16+lowhexdata;
		i++;
		SendOut[hexdatalen]=(char)hexdata;
		hexdatalen++;
	}
	return hexdatalen;
}
void decode(CString input,BYTE *out,int min,int max)
{
	typedef void (_cdecl *DecodeAnswer)(BYTE *in,int t1,int t2,BYTE *out);
	HINSTANCE hinstDLL=NULL; 
	hinstDLL=LoadLibrary("0707.dll");
	if (hinstDLL)
	{
		DecodeAnswer PDecodeAnswer;
		PDecodeAnswer = (DecodeAnswer)GetProcAddress (hinstDLL,"DecodeAnswer");
		BYTE in[1000];
		memset(in,0,sizeof(in));
		string2Hex(input,(char *)in);
		memset(out,0,sizeof(out));
		PDecodeAnswer(in,min,max,out);
		FreeLibrary(hinstDLL);
	}
}


void CFormImport::OnImport() 
{
	static char BASED_CODE szFilter[] = "All Files (*.bak)|*.bak||";
  	CFileDialog dlg(TRUE,NULL,"*.bak",NULL,szFilter);
	CString strPath = "";
	if ( dlg.DoModal() == IDOK)
	{

		m_paperlist.DeleteAllItems();
		m_answerlist.DeleteAllItems();
		strPath = dlg.GetPathName();
		strPath.Replace("\\", "\\\\");
		CString	  path = strPath;

		CBackUp bk;
		CString strPaper = bk.Load("试卷名称",strPath);
		CString strSub = bk.Load("科目",strPath);
		CString strDepar = bk.Load("参考院系",strPath);
		CString strClass = bk.Load("参考班级",strPath);
		CString strAddr = bk.Load("考场",strPath);
		CString strStart = bk.Load("开考时间",strPath);
		CString strEnd = bk.Load("收卷时间",strPath);
		CString strStartNo = bk.Load("开始题号",strPath);
		CString strEndNo = bk.Load("结束题号",strPath);
		CString strMaxNo = bk.Load("最大题号",strPath);
		CString strAnswer = bk.Load("学生答案",strPath);
		int iStartNO = atoi(strStartNo);
		int iEndNO = atoi(strEndNo);
		m_paperlist.AddItem(0, strPaper, strSub, strDepar, strClass, strStart, strEnd, strStartNo, strEndNo, strMaxNo);
		
// 		CoInitialize(NULL);	
// 		CDBStuInfo  m_StuInfodb;
// 		if ( !m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			int pp = strAnswer.GetLength();
// 			int tt = (strAnswer.GetLength()+1)/309;
// 			for(int i = 0; i < tt; i++)
// 			{
// 				CString mac = strAnswer.Mid(309*i,8);
// 				CString strPeranswer = strAnswer.Mid(309*i+9,300);
// 				BYTE out[1000];
// 				decode(strPeranswer,out,0,199);
// 				CString answerstr = "";
// 				for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
// 				{
// 					CString tmp;
// 					if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
// 						tmp = "0";
// 					else
// 						tmp.Format("%d",out[j]);
// 					answerstr = answerstr+tmp;
// 				}	
// 				CString strsutanswer = ""; 
// 				for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
// 				{
// 					CString test = answerstr.Mid(strlength,1);
// 					if(answerstr.Mid(strlength,1) == "1")
// 						strsutanswer += "A";
// 					else if(answerstr.Mid(strlength,1) == "2")
// 						strsutanswer += "B";
// 					else if(answerstr.Mid(strlength,1) == "3")
// 						strsutanswer += "C";
// 					else if(answerstr.Mid(strlength,1) == "4")
// 						strsutanswer += "D";
// 					else
// 						strsutanswer += "0";
// 				}
// 
// 				BOOL flag = m_StuInfodb.LoadByDev(mac);
// 				if(flag)
// 				{
// 					m_answerlist.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Studepartment,
// 						m_StuInfodb.m_Stuclass,strAddr,strsutanswer);
// 				}
// 			}
// 			m_StuInfodb.Close();
// 		}
// 		CoUninitialize();
	}

}

void CFormImport::OnSave() 
{
	if(m_paperlist.GetItemCount() <= 0)
	{
		AfxMessageBox("没有试卷信息，无法备份");
	}
	CString strSub = m_paperlist.GetItemText(0,1);
	CString strClass = m_paperlist.GetItemText(0,3);
	CString strStart = m_paperlist.GetItemText(0,4);
	CString strEnd = m_paperlist.GetItemText(0,5);
// 	CoInitialize(NULL);	
// 	CDBPaper  m_Paperdb;
// 	int iPaperID = 0;
// 	if ( !m_Paperdb.Connect())
// 	{
// 		AfxMessageBox("DBPaper:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Paperdb.LoadByPaper(strSub,strClass,strStart);
// 		if(flag)
// 		{
// 			AfxMessageBox("试卷已经存在！");
// 			iPaperID = m_Paperdb.m_ydid;
// 		}
// 		else
// 		{
// 			m_Paperdb.m_ydid = 0;
// 			m_Paperdb.m_papername = m_paperlist.GetItemText(0,0);
// 			m_Paperdb.m_subject = m_paperlist.GetItemText(0,1);
// 			m_Paperdb.m_depart = m_paperlist.GetItemText(0,2);
// 			m_Paperdb.m_class = m_paperlist.GetItemText(0,3);
// 			COleDateTime start;
// 			start.ParseDateTime(strStart);
// 			COleDateTime end;
// 			end.ParseDateTime(strEnd);
// 			m_Paperdb.m_starttime = start;
// 			m_Paperdb.m_endtime = end;
// 			m_Paperdb.m_startNO = atoi(m_paperlist.GetItemText(0,7));
// 			m_Paperdb.m_endNO = atoi(m_paperlist.GetItemText(0,8));
// 			m_Paperdb.m_maxNO = atoi(m_paperlist.GetItemText(0,9));
// 			if(m_Paperdb.Add())
// 				AfxMessageBox("添加试卷成功！");
// 			else
// 				AfxMessageBox("添加试卷失败！");
// 			 BOOL flag1 =  m_Paperdb.LoadByPaper(strSub,strClass,strStart);
// 			 if(flag1)
// 			 {
// 				iPaperID = m_Paperdb.m_ydid;
// 			 }
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();
	
/*	CString temp = "";
	temp.Format("%d",iPaperID);
	AfxMessageBox(temp);*/
// 	CoInitialize(NULL);	
// 	CDBResult  m_Resultdb;
// 	if ( !m_Resultdb.Connect())
// 	{
// 		AfxMessageBox("DBResult:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		for(int i = 0; i < m_answerlist.GetItemCount(); i++)
// 		{
// 			CString strStuid = m_answerlist.GetItemText(i,0);
// 			BOOL flag = m_Resultdb.LoadByRes(strStuid, strStart);
// 			if(flag)
// 			{
// 				int id = m_Resultdb.m_ydid;
// 				m_Resultdb.m_paperid = iPaperID;
// 				if(m_Resultdb.Update(id))
// 				{
// 					AfxMessageBox("更新成功！");
// 				}
// 				else
// 				{
// 					AfxMessageBox("更新成功！");
// 				}
// 			}
// 			else
// 			{
// 				m_Resultdb.m_ydid = 0;
// 				m_Resultdb.m_stuid = strStuid;
// 				m_Resultdb.m_paperid = iPaperID;
// 				COleDateTime start;
// 				start.ParseDateTime(strStart);
// 				COleDateTime end;
// 				end.ParseDateTime(strEnd);
// 				m_Resultdb.m_starttime = start;
// 				m_Resultdb.m_endtime = end;
// 				m_Resultdb.m_stuanswer = m_answerlist.GetItemText(i,5);
// 				m_Resultdb.m_addr = m_answerlist.GetItemText(i,4);
// 				if(m_Resultdb.Add())
// 					AfxMessageBox("添加成功！");
// 				else
// 					AfxMessageBox("添加失败！");
// 			}
// 		}
// 		m_Resultdb.Close();
// 	}
// 	CoUninitialize();


}
CString CFormImport::GetExePath()
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
void CFormImport::OnExport() 
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"back.bak";	

	CBackUp bk;
	CString title = bk.Load("试卷名称", capture.m_bmpFile);
	CFileDialog dlg(FALSE,"*.bak",title,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"bak Files (*.bak)|*.bak|All Files (*.*)|*.*||");
	CString path = "";
	if(dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();	
		BOOL flag = CopyFile(capture.m_bmpFile,path,FALSE);
		if(flag)
			AfxMessageBox("导出成功！");
		else
			AfxMessageBox("导出失败！");
	}

}

void CFormImport::SetInfo()
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"back.bak";	
	CString	  strPath = capture.m_bmpFile;

	m_paperlist.DeleteAllItems();
	m_answerlist.DeleteAllItems();

	CBackUp bk;
	CString strPaper = bk.Load("试卷名称",strPath);
	CString strSub = bk.Load("科目",strPath);
	CString strDepar = bk.Load("参考院系",strPath);
	CString strClass = bk.Load("参考班级",strPath);
	CString strAddr = bk.Load("考场",strPath);
	CString strStart = bk.Load("开考时间",strPath);
	CString strEnd = bk.Load("收卷时间",strPath);
	CString strStartNo = bk.Load("开始题号",strPath);
	CString strEndNo = bk.Load("结束题号",strPath);
	CString strMaxNo = bk.Load("最大题号",strPath);
	CString strAnswer = bk.Load("学生答案",strPath);
	int iStartNO = atoi(strStartNo);
	int iEndNO = atoi(strEndNo);
	m_paperlist.AddItem(0, strPaper, strSub, strDepar, strClass, strStart, strEnd, strStartNo, strEndNo, strMaxNo);
	
// 	CoInitialize(NULL);	
// 	CDBStuInfo  m_StuInfodb;
// 	if ( !m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		int pp = strAnswer.GetLength();
// 		int tt = (strAnswer.GetLength()+1)/309;
// 		for(int i = 0; i < tt; i++)
// 		{
// 			CString mac = strAnswer.Mid(309*i,8);
// 			CString strPeranswer = strAnswer.Mid(309*i+9,300);
// 			BYTE out[1000];
// 			decode(strPeranswer,out,0,199);
// 			CString answerstr = "";
// 			for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
// 			{
// 				CString tmp;
// 				if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
// 					tmp = "0";
// 				else
// 					tmp.Format("%d",out[j]);
// 				answerstr = answerstr+tmp;
// 			}	
// 			CString strsutanswer = ""; 
// 			for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
// 			{
// 				CString test = answerstr.Mid(strlength,1);
// 				if(answerstr.Mid(strlength,1) == "1")
// 					strsutanswer += "A";
// 				else if(answerstr.Mid(strlength,1) == "2")
// 					strsutanswer += "B";
// 				else if(answerstr.Mid(strlength,1) == "3")
// 					strsutanswer += "C";
// 				else if(answerstr.Mid(strlength,1) == "4")
// 					strsutanswer += "D";
// 				else
// 					strsutanswer += "0";
// 			}
// 
// 			BOOL flag = m_StuInfodb.LoadByDev(mac);
// 			if(flag)
// 			{
// 				m_answerlist.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Studepartment,
// 					m_StuInfodb.m_Stuclass,strAddr,strsutanswer);
// 			}
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();

}

void CFormImport::SetKind(int kind)
{
	m_paperlist.DeleteAllItems();
	m_answerlist.DeleteAllItems();
	m_kind = kind;
	if(kind == 12)
	{
		m_export.ShowWindow(SW_SHOW);
		m_save.ShowWindow(SW_HIDE);
		m_import.ShowWindow(SW_HIDE);
	}
	if(kind == 14)
	{
		m_export.ShowWindow(SW_HIDE);
		m_save.ShowWindow(SW_SHOW);
		m_import.ShowWindow(SW_SHOW);
	}
}

