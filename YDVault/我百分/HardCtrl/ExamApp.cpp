#include "stdafx.h"
#include "hardctrl.h"
#include "ExamApp.h"
#include "BackUp.h"
#include "ExamDetail.h"

CArray<CStudev, CStudev>  m_arryStud;
extern CExamDetail  m_examdeteil;

CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;

CExamApp::CExamApp()
{

}

CExamApp::~CExamApp()
{

}

BOOL CExamApp::Test()
{
	m_com1.Closecom();
	m_com2.Closecom();
	CYDAppCom  com;
	arryTeacherdev.RemoveAll();
	int iCount = com.Testcom(arryTeacherdev);

	if(iCount == 0 )
		return  FALSE;
	else
		return TRUE;
}
BOOL CExamApp::TestStu()
{
	CYDAppCom  com;
	arryTeacherdev.RemoveAll();

/*	CTeacherDev td;
	memset(td.m_com,0,sizeof(td.m_com));
	strcpy(td.m_com,"com4");
	memset(td.m_mac,0,sizeof(td.m_mac));
	strcpy(td.m_mac,"00 00 00");
	arryTeacherdev.Add(td);	*/
	int iCount = com.TestStucom(arryTeacherdev);
//	int iCount = arryTeacherdev.GetSize();
	if(iCount == 0 )
		return  FALSE;
	else
		return TRUE;
}
BOOL CExamApp::Opencom()
{
	m_com1.Closecom();
	m_com2.Closecom();
	if(arryTeacherdev.GetSize() == 1)
	{
		m_com1.Opencom(arryTeacherdev[0].m_com, arryTeacherdev[0].m_mac);
		AfxMessageBox(m_com1.Get_Comstat_text());
	}
		
	else if(arryTeacherdev.GetSize() >=  2)
	{
		m_com1.Opencom(arryTeacherdev[0].m_com, arryTeacherdev[0].m_mac);
		AfxMessageBox(m_com1.Get_Comstat_text());
		m_com2.Opencom(arryTeacherdev[1].m_com, arryTeacherdev[1].m_mac);
		AfxMessageBox(m_com2.Get_Comstat_text());
	}
	else if(arryTeacherdev.GetSize() == 0)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CExamApp::OpenStucom()
{
	m_com3.Closecom();
	m_com3.Opencom(arryTeacherdev[0].m_com, arryTeacherdev[0].m_mac);
//	AfxMessageBox(m_com3.Get_Comstat_text());
	return TRUE;
}
BOOL CExamApp::CloseStucom()
{
	return m_com3.Closecom();
}
BOOL CExamApp::Get_teach_stat(CString strCom)
{
	strCom.TrimRight();
	if(m_com1.m_port == strCom)
	{	
		return m_com1.m_isuse;
	}
	if(m_com2.m_port == strCom)
	{
		return m_com2.m_isuse;
	}
	return FALSE;
}
/************************************************
函数名：Get_teach_count
说明：获取正在使用的教师机数量
************************************************/
int CExamApp::Get_teach_count()
{
	int ret = 0;
	if(m_com1.m_isuse)
		ret++;
	if(m_com2.m_isuse)
		ret++;
	return ret;
}

BOOL CExamApp::Search(CArray<CStusearch, CStusearch> &arryvalid, CStringArray &arryList)
{
	arryvalid.RemoveAll();
	CStringArray m_arry_search;
	m_arry_search.RemoveAll();
	if(!m_com1.Search(m_arry_search))
		return FALSE;

//	int i = m_arry_search.GetSize();
	for (int i = 0; i < m_arry_search.GetSize(); i++)
	{
		CString strMac = m_arry_search[i];
		strMac.TrimLeft();
		strMac.TrimRight();
		strMac.MakeUpper();
		for (int j = 0; j < m_examdeteil.m_arryStuIfo.GetSize(); j++)
		{
			CString strFileMac = m_examdeteil.m_arryStuIfo[j].sStuDev;
			strFileMac.TrimLeft();
			strFileMac.TrimRight();
			strFileMac.MakeUpper();
			if (strMac == strFileMac)
			{
				CString strStuID = m_examdeteil.m_arryStuIfo[j].sStuID;
				for (int k = 0; k < arryList.GetSize(); k++)
				{
					if (arryList[k] == strStuID)
					{
						CStusearch st;
						memset(st.m_mac, 0, sizeof(st.m_mac));
						strcpy(st.m_mac, strMac);
						memset(st.m_id, 0, sizeof(st.m_id));
						strcpy(st.m_id, strStuID);
						arryvalid.Add(st);
						break;
					}
				}
			}
		}
	}
// 	CMyODBC db;
// 	CODBCSet set;
// 	BOOL flag =  db.ConnectDB();
// 	if(flag)
// 	{
// 		for(int i = 0; i < m_arry_search.GetSize(); i++)
// 		{
// 			CString strmac = m_arry_search[i];
// 			strmac.TrimLeft();
// 			strmac.TrimRight();
// 			strmac.MakeUpper();
// 			CString strSql;
// 			strSql.Format("select stuid from ydstuinfo where studevid = '%s';", strmac);
// 			db.PrepareSql(strSql, set);
// 			if(db.FetchData())
// 			{
// 				CString strId = set.m_coldata[0].value;
// 				strId.TrimLeft();
// 				strId.TrimRight();
// 				for(int j = 0; j < arryList.GetSize(); j++)
// 				{
// 					CString SID = arryList[j];
// 					if(arryList[j] == strId)
// 					{
// 						CStusearch st;
// 						memset(st.m_mac, 0, sizeof(st.m_mac));
// 						strcpy(st.m_mac, strmac);
// 						memset(st.m_id, 0, sizeof(st.m_id));
// 						strcpy(st.m_id, strId);
// 						arryvalid.Add(st);
// 						break;
// 					}
// 				}
// 			}
// 			set.Empty();
// 		}
// 		db.DisConnect();
// 	}
	return TRUE;
}


BOOL CExamApp::Rollcall_all(CStringArray &arryvalidID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
// 	CMyODBC db;
// 	CODBCSet set;
// 	BOOL flag =  db.ConnectDB();
// 	if(flag)
// 	{
// 		m_arryStud.RemoveAll();
// 		for(int i = 0; i < arryvalidID.GetSize(); i++)
// 		{
// 			CString strID = arryvalidID[i];
// 			strID.TrimLeft();
// 			strID.TrimRight();
// 			CString strSql;
// 			strSql.Format("select studevid from ydstuinfo where stuid = '%s';", strID);
// 			db.PrepareSql(strSql, set);
// 			if(db.FetchData())
// 			{
// 				CString strMac = set.m_coldata[0].value;
// 				strMac.TrimLeft();
// 				strMac.TrimRight();
// 				strMac.MakeLower();
// 				CStudev sd;
// 				memset(sd.m_stuid,0,sizeof(sd.m_stuid));
// 				strcpy(sd.m_stuid,strID);
// 				memset(sd.m_mac,0,sizeof(sd.m_mac));
// 				strcpy(sd.m_mac,strMac);
// 				sd.m_index = i;
// 				sd.m_rollcall = 0;
// 				sd.m_rolling = 0;
// 				sd.m_start = tStart;
// 				sd.m_end = tEnd;
// 				memset(sd.m_answer,0,sizeof(sd.m_answer));
// 				sd.m_islinkroll = FALSE;
// 				sd.m_islinkroling = FALSE;
// 				m_arryStud.Add(sd);
// 			}
// 			set.Empty();
// 		}
// 		db.DisConnect();
// 	}
	assignmac();
	
	if(m_com1.m_isuse == TRUE)
		m_com1.Rollcall_all(tStart, tEnd, iMaxNO);
	if(m_com2.m_isuse == TRUE)
		m_com2.Rollcall_all(tStart, tEnd, iMaxNO);
	return TRUE;
}
/****************************************************
函数名：Rollcall_all_new(向教师机发送考试信息包)，
参数：CStringArray &arryvalidID有效的学生机学号，
	  COleDateTime tStart考试开考时间，
	  COleDateTime tEnd考试结束时间
      int iMaxNO考试最大题号
*****************************************************/
BOOL CExamApp::Rollcall_all_new(CStringArray &arryvalidID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
	for (int i = 0; i < arryvalidID.GetSize(); i++)
	{
		CString strID = arryvalidID[i];
		strID.TrimLeft();
		strID.TrimRight();
		for (int j = 0; j < m_examdeteil.m_arryStuIfo.GetSize(); j++)
		{
			if (m_examdeteil.m_arryStuIfo[j].sStuID == strID)
			{
				CString strMac = m_examdeteil.m_arryStuIfo[j].sStuDev;
				strMac.TrimLeft();
				strMac.TrimRight();
				strMac.MakeLower();
				CStudev sd;
				memset(sd.m_stuid,0,sizeof(sd.m_stuid));
				strcpy(sd.m_stuid,strID);
				memset(sd.m_mac,0,sizeof(sd.m_mac));
				strcpy(sd.m_mac,strMac);
				sd.m_index = i;
				sd.m_rollcall = 0;
				sd.m_rolling = 0;
				sd.m_start = tStart;
				sd.m_end = tEnd;
				memset(sd.m_answer,0,sizeof(sd.m_answer));
				sd.m_islinkroll = FALSE;
				sd.m_islinkroling = FALSE;
				m_arryStud.Add(sd);

			}
		}

	}

	assignmac();
	
	int iGrade;
	if(m_examdeteil.nExam_quality == 1 )
		iGrade = 0;
	else
		iGrade = 1;

	int iRolling;
	if(m_examdeteil.nRolling_pattern == 1)
		iRolling = 0;
	else
		iRolling = 1;

	int iFm;
	if(m_examdeteil.b_listen)
		iFm = 1;
	else
		iFm = 0;

	if(m_com1.m_isuse == TRUE)
		m_com1.Rollcall_all_new(tStart, tEnd, iMaxNO, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
	if(m_com2.m_isuse == TRUE)
		m_com2.Rollcall_all_new(tStart, tEnd, iMaxNO, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
	return TRUE;
}
BOOL CExamApp::Rollcall_single_new(CString strID, COleDateTime tStart, COleDateTime tEnd)
{
	BOOL flag = FALSE;
	strID.TrimRight();
	if(!Ismac_in(strID))
	{
		AfxMessageBox("该学生机无效！");
		return FALSE;
	}
	
	CString strmac;
	int index;
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString temp = m_arryStud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			strmac = m_arryStud[i].m_mac;
			index = m_arryStud[i].m_index;
			break;
		}
	}

	int iGrade;
	if(m_examdeteil.nExam_quality == 1 )
		iGrade = 0;
	else
		iGrade = 1;
	
	int iRolling;
	if(m_examdeteil.nRolling_pattern == 1)
		iRolling = 0;
	else
		iRolling = 1;
	
	int iFm;
	if(m_examdeteil.b_listen)
		iFm = 1;
	else
		iFm = 0;

	int iMaxNO = m_examdeteil.nEndNO;

	if(Get_teach_count() == 1)
		return	m_com1.Rollcall_single_new(m_arryStud,strID, strmac, tStart, tEnd, iMaxNO, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
	if(Get_teach_count() == 2)
	{
		if(index%2 == 0)
			return m_com1.Rollcall_single_new(m_arryStud,strID, strmac, tStart, tEnd, iMaxNO, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
		if(index%2 == 1)
			return m_com2.Rollcall_single_new(m_arryStud,strID, strmac, tStart, tEnd, iMaxNO, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
	}
	return FALSE;



		
}

/**************************************************
函数名：Rollcall_infra
参数
说明：红外模式下，向教师机发送考试信息包，信息内容在CExamDetail中
**************************************************/
BOOL CExamApp::Rollcall_infra()
{
	COleDateTime tStart, tEnd;
	tStart.ParseDateTime(m_examdeteil.sStart);
	tEnd.ParseDateTime(m_examdeteil.sEnd);
	int maxno = m_examdeteil.nMaxNO;
	int iGrade;
	if(m_examdeteil.nExam_quality == 1 )
		iGrade = 0;
	else
		iGrade = 1;

	int iRolling;
	if(m_examdeteil.nRolling_pattern == 1)
		iRolling = 0;
	else
		iRolling = 1;

	int iFm;
	if(m_examdeteil.b_listen)
		iFm = 1;
	else
		iFm = 0;

	if(m_com1.m_isuse == TRUE)
		m_com1.Rollcall_all_new(tStart, tEnd, maxno, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
	if(m_com2.m_isuse == TRUE)
		m_com2.Rollcall_all_new(tStart, tEnd, maxno, iGrade, iRolling, iFm, m_examdeteil.m_start1, m_examdeteil.m_end1, m_examdeteil.m_start2, m_examdeteil.m_end2, m_examdeteil.m_start3, m_examdeteil.m_end3, m_examdeteil.m_chanal);
	return true;
}
/********************************************
函数名：assignmac
参数：
说明：重新整合学生机的mac地址，如果2台教师机平分学生地址
********************************************/
void CExamApp::assignmac()
{
	int iCount = Get_teach_count();
	if(iCount == 1)
	{
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			m_com1.Setvalidmac(m_arryStud[i].m_stuid, m_arryStud[i].m_mac, m_arryStud[i].m_start, m_arryStud[i].m_end);
		}
	}
	if(iCount == 2)
	{
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			if(i%2 == 0)
			{
				m_com1.Setvalidmac(m_arryStud[i].m_stuid, m_arryStud[i].m_mac, m_arryStud[i].m_start, m_arryStud[i].m_end);
			}
			if(i%2 == 1)
			{
				m_com2.Setvalidmac(m_arryStud[i].m_stuid, m_arryStud[i].m_mac, m_arryStud[i].m_start, m_arryStud[i].m_end);
			}
		}
	}
}
/*****************************************
函数名：Get_stat
参数：CStringArray &arryRoll 点名已经完成的学生学号
	  CStringArray &arryRolling 收卷已经完成的学生学号
	  BOOL &bRoll点名是否结束
	  int &iRolldone点名结束的人数
	  BOOL &bRolling收卷是否结束
	  int &iRollingdone收卷结束的人数
说明：获取教师机中学生的状态
*****************************************/
void CExamApp::Get_stat(CStringArray &arryRoll,CStringArray &arryRolling, BOOL &bRoll,int &iRolldone, BOOL &bRolling, int &iRollingdone)
{
	BOOL bflagroll1,  bflagroll2, bflagrlling1, bflagrlling2;
	if(m_com1.m_isuse)
		m_com1.Get_roll_stat(m_arryStud, bflagroll1, bflagrlling1);
	if(m_com2.m_isuse)
		m_com2.Get_roll_stat(m_arryStud, bflagroll2, bflagrlling2);
	
	if(m_com1.m_isuse && m_com2.m_isuse)
	{
		bRoll = bflagroll1 && bflagroll2;
		bRolling = bflagrlling1 && bflagrlling2;
	}
	else if(m_com1.m_isuse && (!m_com2.m_isuse))
	{
		bRoll = bflagroll1;
		bRolling = bflagrlling1;
	}
	else if((!m_com1.m_isuse) && m_com2.m_isuse)
	{
		bRoll = bflagroll2;
		bRolling = bflagrlling2;
	}
	else
	{
		bRoll = TRUE;
		bRolling = TRUE;
	}


	iRolldone = 0;
	iRollingdone = 0;
	arryRoll.RemoveAll();
	arryRolling.RemoveAll();
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		if(m_arryStud[i].m_rollcall == 1)
		{
			iRolldone++;
			arryRoll.Add(m_arryStud[i].m_stuid);
		}
		if(m_arryStud[i].m_rolling == 1 || m_arryStud[i].m_rolling == 3)
		{
			iRollingdone++;
			arryRolling.Add(m_arryStud[i].m_stuid);
		}
	}
}
/**********************************************
函数名：Get_start_time
参数:CString strID学生学号
说明：获取该学生的开考时间
**********************************************/
COleDateTime CExamApp::Get_start_time(CString strID)
{
	strID.TrimRight();
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString strTemp = m_arryStud[i].m_stuid;
		strTemp.TrimRight();
		if(strTemp == strID)
			return m_arryStud[i].m_start;
	}

	return COleDateTime::GetCurrentTime();
}
/**********************************************
函数名：Get_start_time
参数:CString strID学生学号
说明：获取该学生的收卷时间
**********************************************/
COleDateTime CExamApp::Get_end_time(CString strID)
{
	strID.TrimRight();
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString strTemp = m_arryStud[i].m_stuid;
		strTemp.TrimRight();
		if(strTemp == strID)
			return m_arryStud[i].m_end;
	}

	return COleDateTime::GetCurrentTime();
}
/********************************************
函数名：Get_stud_count
参数：
说明：获取参加考试的学生认识
********************************************/
int CExamApp::Get_stud_count()
{
	return m_arryStud.GetSize();
}
/********************************************
函数明：Rollcall_single
参数：CString strID学生学号
	  COleDateTime tStart开始考试时间
	  COleDateTime tEnd结束考试时间
	  int iMaxNO 最大题号
	  BOOL bLate是否是迟到的学生
说明：对学生单独开考指令
********************************************/
BOOL CExamApp::Rollcall_single(CString strID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO, BOOL bLate)
{
	strID.TrimRight();
	if(!Ismac_in(strID))
		add(strID, tStart, tEnd, bLate,FALSE,FALSE);

	CString strmac;
	int index;
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString temp = m_arryStud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			strmac = m_arryStud[i].m_mac;
			index = m_arryStud[i].m_index;
			break;
		}
	}

	if(Get_teach_count() == 1)
		return	m_com1.Rollcall_single(m_arryStud,strID, strmac, tStart, tEnd, iMaxNO);
	if(Get_teach_count() == 2)
	{
		if(index%2 == 0)
			return m_com1.Rollcall_single(m_arryStud,strID, strmac, tStart, tEnd, iMaxNO);
		if(index%2 == 1)
			return m_com2.Rollcall_single(m_arryStud,strID, strmac, tStart, tEnd, iMaxNO);
	}
	return FALSE;
}
/********************************************
函数名：Roll_link
参数：CString strID学生学号
	  COleDateTime tStart开始考试时间
	  COleDateTime tEnd结束考试时间
	  int iMaxNO 最大题号
说明：联机开考
********************************************/
BOOL CExamApp::Roll_link(CString strID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
	strID.TrimLeft();
	strID.TrimRight();
	if(!Ismac_in(strID))
	{
		COleDateTime tCurrent;
		tCurrent = COleDateTime::GetCurrentTime();
		add(strID, tCurrent, tEnd, FALSE,TRUE,FALSE);
	}
	else
	{
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			if(m_arryStud[i].m_stuid == strID)
			{
				m_arryStud[i].m_islinkroll = TRUE;
				break;
			}
		}
	}
	
	return m_com3.Roll_link(tStart, tEnd, iMaxNO);
}
/********************************************
函数名：LinkRolling
参数：CString strID学生学号
	  COleDateTime tStart开始考试时间
	  COleDateTime tEnd结束考试时间
	  int iMaxNO 最大题号
说明：联机开考
********************************************/
CString CExamApp::LinkRolling(CString strID,int iStart, int iEnd, int paperid, CString strAddr)
{
	strID.TrimLeft();
	strID.TrimRight();
	CString answer = m_com3.LinkRolling(iStart, iEnd);
	if(answer == "")
		return "";

	COleDateTime tEnd;
	tEnd = COleDateTime::GetCurrentTime();
	COleDateTime tStart;

	if(!Ismac_in(strID))
	{
		AfxMessageBox("该学生机不在已开考的学生中，请检测！");
		return "";
	}
	else
	{
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			if(m_arryStud[i].m_stuid == strID)
			{
				m_arryStud[i].m_islinkroling = TRUE;
				m_arryStud[i].m_end = tEnd;
				tStart = m_arryStud[i].m_start;
				memset(m_arryStud[i].m_answer,0,sizeof(m_arryStud[i].m_answer));
				strcpy(m_arryStud[i].m_answer,answer);
				break;
			}
		}
	}


	return answer;
}
/********************************************
函数名：Ismac_in
参数：CString strID学生学号
说明：该学生是否在动态数组之中
********************************************/

BOOL CExamApp::Ismac_in(CString strID)
{
	BOOL ret = FALSE;
	strID.TrimRight();
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString temp = m_arryStud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			ret = TRUE;
			break;
		}
	}
	return ret;
}
/********************************************
函数名：add
参数：CString strID学生学号
	  COleDateTime tStart该考生开始考试的时间
	  COleDateTime tEnd该考生结束考试的时间
	  BOOL bLate该考生是否迟到
	  BOOL bIsLingroll该考生是否联机开考 
	  BOOL bIsLinkrolling该考生是否联机收卷
说明：该学生是否在动态数组之中
********************************************/
void CExamApp::add(CString strID, COleDateTime tStart, COleDateTime tEnd, BOOL bLate, BOOL bIsLingroll, BOOL bIsLinkrolling)
{
	strID.TrimRight();
	strID.TrimLeft();
	CString strmac;

	CStudev sd;
	int index = m_arryStud.GetSize();
	sd.m_index = index;
	memset(sd.m_stuid,0,sizeof(sd.m_stuid));
	strcpy(sd.m_stuid, strID);
	memset(sd.m_mac,0,sizeof(sd.m_mac));
	strcpy(sd.m_mac,strmac);
	sd.m_start = tStart;
	sd.m_end = tEnd;
	sd.m_islate = bLate;
	sd.m_rollcall = 0;
	sd.m_rolling = 0;
	sd.m_islinkroll = bIsLingroll;
	sd.m_islinkroling = bIsLinkrolling;
	memset(sd.m_answer,0,sizeof(sd.m_answer));
	m_arryStud.Add(sd);
}
/********************************************
函数名：Get_singlemac_stat
参数：CString strID学生学号
	  int &iRoll点名方式
	  int &iRolling收卷方式
说明：获取单个学生点名和收卷模式
********************************************/
BOOL CExamApp::Get_singlemac_stat(CString strID, int &iRoll, int &iRolling)
{
	BOOL bflagroll1,  bflagroll2, bflagrlling1, bflagrlling2;
	if(m_com1.m_isuse)
		m_com1.Get_roll_stat(m_arryStud, bflagroll1, bflagrlling1);
	if(m_com2.m_isuse)
		m_com2.Get_roll_stat(m_arryStud, bflagroll2, bflagrlling2);
	
	strID.TrimRight();
	BOOL flag = FALSE;
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString temp = m_arryStud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			flag = TRUE;
			iRoll = m_arryStud[i].m_rollcall;
			iRolling = m_arryStud[i].m_rolling;
		}
	}
	return flag;
}
/****************************************
函数名：Rolling_all
参数：  int iStart开始题号
		int iEnd结束题号
		COleDateTime tEnd收卷时间
说明：  对所以的学生进行收卷
****************************************/
BOOL CExamApp::Rolling_all(int iStart, int iEnd, COleDateTime tEnd)
{
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		if(m_arryStud[i].m_rolling == 0 || m_arryStud[i].m_rolling == 2)
		{
			m_arryStud[i].m_end = tEnd;
		}
	}
	
	BOOL ret = FALSE;
	BOOL ret1 = FALSE;
	BOOL ret2 = FALSE;
	
	if(Get_teach_count() == 1)
	{
		return m_com1.Rolling(iStart, iEnd);
	}
	if(Get_teach_count() == 2)
	{
		ret1 = m_com1.Rolling(iStart, iEnd);
		ret2 = m_com2.Rolling(iStart, iEnd);
		ret = ret1 && ret2;
	}
	return ret;
}
/********************************************
函数名：Get_answer
参数：int iStart开始题号
	  CString strSubject考试科目的名称
	  int paperid试卷的编号
	  CString strAddr学生考试的地点
说明：提取学生的答案，保存至数据库
********************************************/
BOOL CExamApp::Get_answer(int iStart, int iEnd, CString strSubject, int paperid, CString strAddr)
{
	BOOL ret = FALSE;
	BOOL ret1 = FALSE;
	BOOL ret2 = FALSE;
	CString strBacanswer = "";
	if(Get_teach_count() == 1)
	{
		ret = m_com1.Getanswer(m_arryStud, iStart, iEnd, strBacanswer);
	}
	if(Get_teach_count() == 2)
	{
		CString temp1 = "";
		CString temp2 = "";
		ret1 = m_com1.Getanswer(m_arryStud, iStart, iEnd, temp1);
		ret2 = m_com2.Getanswer(m_arryStud, iStart, iEnd, temp2);
		strBacanswer = temp1 + temp2;
		ret = ret1 && ret2;
	}
	if(!ret)
		return ret;
	
//	CBackUp bk;
//	bk.Backup("学生答案", strBacanswer);
	
// 	CMyODBC db;
// 	BOOL flag =  db.ConnectDB();
// 	if(flag)
// 	{
// 		for(int i = 0; i < m_arryStud.GetSize(); i++)
// 		{
// 			if(m_arryStud[i].m_rolling != 3)
// 			{
// 				CString strSql;
// 				strSql.Format("insert into ydresult(stuid, paperid, stustart, stuend, addrid, stuanswer) values \
// 					('%s', %d, to_date('%s', 'yyyy-mm-dd hh24:mi:ss'), to_date('%s', 'yyyy-mm-dd hh24:mi:ss'), (select addrid from ydexamaddr where examaddr = '%s'), '%s');",
// 					m_arryStud[i].m_stuid, paperid, m_arryStud[i].m_start.Format(), m_arryStud[i].m_end.Format(), strAddr, m_arryStud[i].m_answer);
// 
// 				if(!db.ExeSqlDirect(strSql))
// 				{
// 					CString msg;
// 					msg.Format("学号为：%s答案存储失败！");
// 					AfxMessageBox(msg);
// 				}
// 			}
// 		}
// 		db.DisConnect();
// 	}
	return ret;
}

BOOL CExamApp::GreateExamInfo()
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


	for (i = 0; i < m_arryStud.GetSize(); i++)
	{
		CString sTitle = "";
		sTitle.Format("STUINFO%d", i);

		CString strStuID = m_arryStud[i].m_stuid;
		CString strStuAnswer = m_arryStud[i].m_answer;
		bk.Createfile(sTitle, "STUID", strStuID);
		bk.Createfile(sTitle, "STUANSWER", strStuAnswer);
	}


	CJahCoverCapture capture;
	capture.m_bmpFile =getExePath()+"temp.ini";	
	
//	CBackUp bk;
	CString title = bk.Load("PAPERNAME", capture.m_bmpFile);
	CFileDialog dlg(FALSE,"*.ini",title,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"ini Files (*.ini)|*.ini|All Files (*.*)|*.*||");
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
	
	return TRUE;
}

CString CExamApp::getExePath()
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

BOOL CExamApp::Pre_Rolling(CString strID, COleDateTime tEnd)
{
	BOOL ret = FALSE;
	if(m_com1.m_isuse)
	{
		if(m_com1.Ismac_in(strID))
		{
			ret = m_com1.Pre_rolling(strID);
		}
	}
	if(m_com2.m_isuse)
	{
		if(m_com2.Ismac_in(strID))
		{
			ret = m_com2.Pre_rolling(strID);
		}
	}
	if(ret)
	{
		strID.TrimRight();
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			CString temp = m_arryStud[i].m_stuid;
			temp.TrimRight();
			if(temp == strID)
			{
				m_arryStud[i].m_end = tEnd;
				break;
			}
		}		
	}
	return ret;
}
BOOL CExamApp::GetSingleAnswer(CString strID, int iStart, int iEnd,int paperid, CString strAddr)
{
	BOOL ret = FALSE;
	CString strAnswer = "";
	if(m_com1.m_isuse)
	{
		if(m_com1.Ismac_in(strID))
		{
			ret = m_com1.GetSingleAnswer(strID, iStart, iEnd, strAnswer);
		}
	}
	if(m_com2.m_isuse)
	{
		if(m_com2.Ismac_in(strID))
		{
			ret = m_com2.GetSingleAnswer(strID, iStart, iEnd, strAnswer);
		}
	}
	//AfxMessageBox(strAnswer);
	COleDateTime tStart;
	COleDateTime tEnd;
	for(int i = 0; i < m_arryStud.GetSize(); i++)
	{
		strID.TrimLeft();
		strID.TrimRight();
		if(m_arryStud[i].m_stuid == strID)
		{
			tStart = m_arryStud[i].m_start;
			tEnd = m_arryStud[i].m_end;
		}
	}	


	return ret;
}

BOOL CExamApp::SingleRolling(CString strID, COleDateTime tEnd)
{
	BOOL ret = FALSE;
	if(m_com1.m_isuse)
	{
		if(m_com1.Ismac_in(strID))
		{
			ret = m_com1.SingleRolling(strID);
		}
	}
	if(m_com2.m_isuse)
	{
		if(m_com2.Ismac_in(strID))
		{
			ret = m_com2.SingleRolling(strID);
		}
	}
	if(ret)
	{
		strID.TrimRight();
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			CString temp = m_arryStud[i].m_stuid;
			temp.TrimRight();
			if(temp == strID)
			{
				m_arryStud[i].m_end = tEnd;
				break;
			}
		}		
	}
	return ret;
}

BOOL CExamApp::LinkSuccess()
{
	return m_com3.LinkSuccess();
}
BOOL CExamApp::Offexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO)
{
	return m_com3.Offexaminfo(timeStart,timeEnd,iMaxNO,iStartNO,iEndNO);
}
BOOL CExamApp::Offsendmac(CArray<CStudev, CStudev> &arry)
{
	return m_com3.Offsendmac(arry);
}
BOOL CExamApp::Rest()
{
	return m_com1.Rest();
}
BOOL CExamApp::OpenFM()
{
	BOOL ret = FALSE;
	if(m_com1.m_isuse)
		ret = m_com1.OpenFM();
	if(m_com2.m_isuse)
		ret = m_com2.OpenFM();
	return ret;
}
BOOL CExamApp::SearchFM()
{
	BOOL ret = FALSE;
	if(m_com1.m_isuse)
		ret = m_com1.SearchFM();
	if(m_com2.m_isuse)
		ret = m_com2.SearchFM();
	return ret;
}
BOOL CExamApp::CloseFM()
{
	BOOL ret = FALSE;
	if(m_com1.m_isuse)
		ret = m_com1.CloseFM();
	if(m_com2.m_isuse)
		ret = m_com2.CloseFM();
	return ret;
}
BOOL CExamApp::UpFM()
{
	return m_com1.UpFM();
}
BOOL CExamApp::DownFM()
{
	return m_com1.DownFM();
}
BOOL CExamApp::SetFM(CString strFM)
{
	BOOL ret = FALSE;
	if(m_com1.m_isuse)
		ret = m_com1.SetFM(strFM);
	if(m_com2.m_isuse)
		ret = m_com2.SetFM(strFM);
	return ret;
}
CString CExamApp::GetFm()
{
	CString strFM = m_com1.GetFM();
	if(strFM == "")
		return strFM;
	else
	{
		strFM.Remove(' ');
		int  Ten_Dig=(int)strtol(strFM,NULL,16);
		strFM = "";
		strFM.Format("%d",Ten_Dig);
		return strFM;
	}
}

BOOL CExamApp::Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO)
{
	return m_com3.Getoffanswer(arryList,iStartNO,iEndNO);
}
BOOL CExamApp::Getrelationmap(CStringArray &arryrelation)
{
	return m_com3.Getrelationmap(arryrelation);
}

BOOL CExamApp::FetchAnswer(int iStartNO, int iEndNO)
{
//	CArray<CStudev, CStudev> arry;
//	arry.RemoveAll();
	m_arryStud.RemoveAll();

	return m_com1.FetchAnswer(m_arryStud, iStartNO, iEndNO);
}