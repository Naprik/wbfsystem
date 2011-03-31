#include "stdafx.h"
#include "hardctrl.h"
#include "TeacherCom.h"


CTeacherCom::CTeacherCom()
{
	m_isuse = FALSE;
	m_is_first_seacher = TRUE;
	m_is_first_sendmac = TRUE;
	m_stud.RemoveAll();
}

CTeacherCom::~CTeacherCom()
{

}

BOOL CTeacherCom::Opencom(CString strCom, CString strMac)
{
	strCom.TrimRight();
	strMac.TrimRight();
	m_port = strCom;
	m_mac = strMac;
	if(m_com.Opencom(m_port))
	{
		m_isuse = TRUE;
		return TRUE;
	}
	else
		return FALSE;

}

BOOL CTeacherCom::Closecom()
{
	BOOL ret = TRUE;
	if(m_isuse == TRUE)
	{
		m_com.Close();
		m_isuse = FALSE;
		ret = TRUE;
	}
	return ret;

	//return m_com.Close();
}
CString CTeacherCom::Get_Comstat_text()
{
	CString ret;
	if(m_isuse == FALSE)
		ret.Format("打开串口 %s 失败！", m_port);
	else
		ret.Format("打开串口 %s 成功！", m_port);
	return ret;

}

BOOL CTeacherCom::Search(CStringArray &arry)
{
	arry.RemoveAll();
	if(true)
	{
		if(m_com.Reset())
		{
			Sleep(500);
			if(m_com.Initsearch())
			{
				m_is_first_seacher = FALSE;
			}
			else
			{
				AfxMessageBox("初始化搜索失败！");
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox("初始化教师机蓝牙失败！");
			return FALSE;
		}
	}
	Sleep(10000);
	if(!m_com.GetStudev(arry))
	{
		AfxMessageBox("获取学生地址失败！");
		return FALSE;
	}
	return TRUE;
}

void CTeacherCom::Setvalidmac(CString strID, CString strMac, COleDateTime tStart, COleDateTime tEnd)
{
	CStudev sd;
	int index = m_stud.GetSize();
	sd.m_index = index;
	memset(sd.m_stuid, 0, sizeof(sd.m_stuid));
	strcpy(sd.m_stuid, strID);
	memset(sd.m_mac,0,sizeof(sd.m_mac));
	strcpy(sd.m_mac, strMac);
	sd.m_start = tStart;
	sd.m_end = tEnd;
	sd.m_rollcall = 0;
	sd.m_rolling = 0;
	memset(sd.m_answer,0,sizeof(sd.m_answer));
	m_stud.Add(sd);
}

BOOL CTeacherCom::Rollcall_all(COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
	m_is_first_seacher = TRUE;
	if(!m_com.Sendvalidmac(m_stud))
	{
		AfxMessageBox("发送合法地址到教师机失败！");
		return FALSE;
	}
	Sleep(2000);
	if(!m_com.Sendexaminfo(tStart, tEnd, iMaxNO))
	{
		AfxMessageBox("发送考试信息包失败！");
		return FALSE;
	}
	return TRUE;
}

BOOL CTeacherCom::Rollcall_all_new(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM)
{
	m_is_first_seacher = TRUE;
	if(!m_com.Sendvalidmac(m_stud))
	{
		AfxMessageBox("发送合法地址到教师机失败！");
		return FALSE;
	}
	Sleep(2000);
	if(!m_com.Sendexaminfo_new(timeStart, timeEnd, iMaxNO, iGrade, iRolling, iFm, timeStartfm1, timeEndfm1, timeStartfm2, timeEndfm2, timeStartfm3, timeEndfm3, strFM))
	{
		AfxMessageBox("发送考试信息包失败！");
		return FALSE;
	}
	return TRUE;
}

BOOL CTeacherCom::Rollcall_single_new(CArray<CStudev, CStudev> &arry,CString strID, CString strMac,COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM)
{
	if(m_com.Sendsinglemac(strMac) != 1)
	{
		AfxMessageBox("向教师机和该学生机建链失败！\n该学生可能电池电量不足或关机！");
		return FALSE;
 	}
	
	Sleep(500);

 	if(!m_com.Sendsingleinfo_new(timeStart, timeEnd, iMaxNO, iGrade, iRolling, iFm, timeStartfm1, timeEndfm1, timeStartfm2, timeEndfm2, timeStartfm3, timeEndfm3, strFM))
 	{
		AfxMessageBox("向学生机发送考试信息失败！");
 		return FALSE;
 	}
 	Sleep(500);
	BOOL bRool, bRolling;
 	Get_roll_stat(arry, bRool, bRolling);
	return TRUE;
}

BOOL CTeacherCom::update_stat(BOOL &bRoll, BOOL &bRolling)
{
	return m_com.Updatestat(m_stud, bRoll, bRolling);
}

void CTeacherCom::Get_roll_stat(CArray<CStudev, CStudev> &arry, BOOL &bRoll, BOOL &bRolling)
{
	if(!update_stat(bRoll, bRolling))
		return;

	for(int i = 0; i < m_stud.GetSize(); i++)
	{
		CString strID = m_stud[i].m_stuid;
		for(int j = 0; j < arry.GetSize(); j++)
		{
			if(arry[j].m_stuid == strID)
			{
				arry[j].m_rollcall = m_stud[i].m_rollcall;
				arry[j].m_rolling = m_stud[i].m_rolling;
				break;
			}
		}
	}
}

BOOL CTeacherCom::Rollcall_single(CArray<CStudev, CStudev> &arry, CString strID, CString strMac,COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
// 	m_is_first_seacher = TRUE;
// 	if(m_is_first_sendmac)
// 	{
// 		//arry.RemoveAll();
// 		Setvalidmac(strID, strMac, tStart, tEnd);
// 		m_com.SendFirstMac(strMac);
// 		m_is_first_sendmac = FALSE;
// 	}
// 	BOOL  flag = FALSE;
// 	strID.TrimRight();
// 	strMac.TrimRight();
// 	if(!Ismac_in(strID))
// 	{
// 		flag = TRUE;
// 		Setvalidmac(strID, strMac, tStart, tEnd);
// 	}
// 
// 	int index;
// 	for(int i = 0; i < m_stud.GetSize(); i++)
// 	{
// 		CString temp = m_stud[i].m_stuid;
// 		temp.TrimRight();
// 		if(strID == temp)
// 		{
// 			index = m_stud[i].m_index;
// 			break;
// 		}
// 	}
// 	if(flag)
// 	{
// 		if(!m_com.Addmac(index, strMac))
// 		{
// 			AfxMessageBox("向教师机添加学生机地址失败！");
// 			return FALSE;
// 		}
// 	}
// 	Sleep(2000);
// 	if(m_com.Sendsinglemac(strMac, index) != 1)
// 	{
// 		AfxMessageBox("向教师机和该学生机建链失败！\n该学生可能电池电量不足或关机！");
// 		return FALSE;
// 	}
// 
// 	Sleep(500);
// 	if(!m_com.Sendsingleinfo(tStart, tEnd, iMaxNO))
// 	{
// 		AfxMessageBox("向学生机发送考试信息失败！");
// 		return FALSE;
// 	}
// 	Sleep(500);
// 	BOOL bRool, bRolling;
// 	Get_roll_stat(arry, bRool, bRolling);
	return TRUE;
}

/******************************************************
函数名：ismac_in
说明：判断此学生是否也存在于数字中
******************************************************/
BOOL CTeacherCom::Ismac_in(CString strID)
{
	BOOL ret = FALSE;
	strID.TrimRight();
	for(int i = 0; i < m_stud.GetSize(); i++)
	{
		CString temp = m_stud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			ret = TRUE;
			break;
		}
	}
	return ret;
}
BOOL CTeacherCom::Roll_link(COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
	return m_com.Sendexaminfo(tStart, tEnd, iMaxNO);
}

BOOL CTeacherCom::Rolling(int iStart, int iEnd)
{
	return m_com.Rolling(iStart, iEnd);
}

BOOL CTeacherCom::Pre_rolling(CString strID)
{
	m_is_first_seacher = TRUE;
	strID.TrimRight();
	int index = -1;
	for(int i = 0; i < m_stud.GetSize(); i++)
	{
		CString temp = m_stud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			index = m_stud[i].m_index;
			break;
		}
	}
	if(index == -1)
		return FALSE;
	return m_com.PreRolling(index);
}

BOOL CTeacherCom::GetSingleAnswer(CString strID, int iStartNO, int iEndNO, CString &strAnswerout)
{
	strID.TrimLeft();
	strID.TrimRight();
	int index = -1;
	for(int i = 0; i < m_stud.GetSize(); i++)
	{
		CString temp = m_stud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			index = m_stud[i].m_index;
			break;
		}
	}
	if(index == -1)
		return FALSE;
	return m_com.GetSingleanswer(index, iStartNO, iEndNO, strAnswerout);
}
BOOL CTeacherCom::SingleRolling(CString strID)
{
	m_is_first_seacher = TRUE;
	strID.TrimRight();
	int index = -1;
	for(int i = 0; i < m_stud.GetSize(); i++)
	{
		CString temp = m_stud[i].m_stuid;
		temp.TrimRight();
		if(temp == strID)
		{
			index = m_stud[i].m_index;
			break;
		}
	}
	if(index == -1)
		return FALSE;
	return m_com.SingleRolling(index);
}

BOOL CTeacherCom::Getanswer(CArray<CStudev, CStudev> &arry, int iStart, int iEnd, CString &strAnswer)
{
	BOOL ret = m_com.Getanswer(m_stud, iStart, iEnd, strAnswer);
//	AfxMessageBox(strAnswer);
	if(!ret)
		return FALSE;
	for(int i = 0; i < m_stud.GetSize(); i++)
	{
		CString strID = m_stud[i].m_stuid;
		for(int j = 0; j < arry.GetSize(); j++)
		{
			if(arry[j].m_stuid == strID)
			{
				memset(arry[j].m_answer,0,sizeof(arry[j].m_answer));
				strcpy(arry[j].m_answer, m_stud[i].m_answer);
				//AfxMessageBox(arry[j].m_answer);
				break;
			}
		}
	}
	strAnswer = strAnswer.Mid(18,strAnswer.GetLength()-18-9);
	int t1 = strAnswer.GetLength();
	int t2 = m_stud.GetSize();
	if(strAnswer.GetLength()/303 == m_stud.GetSize())
	{
		for(int t = 0; t < strAnswer.GetLength()/303; t++)
		{
			strAnswer.Delete(309*t,2);
			strAnswer.Insert(309*t,m_stud[t].m_mac);
		}
	}
	//AfxMessageBox(strAnswer);
	return ret;
}
CString CTeacherCom::BackAnwer()
{
	return "";
}
CString CTeacherCom::LinkRolling(int iStart, int iEnd)
{
	return m_com.LinkRolling(iStart, iEnd);
}
BOOL CTeacherCom::LinkSuccess()
{
	return m_com.Linksuccess();
}
BOOL CTeacherCom::Offexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO)
{
	return m_com.Sendoffexaminfo(timeStart,timeEnd,iMaxNO,iStartNO,iEndNO);
}

BOOL CTeacherCom::Offsendmac(CArray<CStudev, CStudev> &arry)
{
	return m_com.Sendvalidmac(arry);
}
BOOL CTeacherCom::Rest()
{
	return m_com.Reset();
}
BOOL CTeacherCom::OpenFM()
{
	return m_com.OpenFM();
}
BOOL CTeacherCom::SearchFM()
{
	return m_com.SearchFM();
}
BOOL CTeacherCom::CloseFM()
{
	return m_com.CloseFM();
}
BOOL CTeacherCom::UpFM()
{
	return m_com.UpFM();
}
BOOL CTeacherCom::DownFM()
{
	return m_com.DownFM();
}
BOOL CTeacherCom::SetFM(CString strFM)
{
	return m_com.SetFM(strFM);
}
CString CTeacherCom::GetFM()
{
	return m_com.GetFM();
}
BOOL CTeacherCom::Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO)
{
	return m_com.Getoffanswer(arryList, iStartNO, iEndNO);
}
BOOL CTeacherCom::Getrelationmap(CStringArray &arryrelation)
{
	return m_com.Getrelationmap(arryrelation);
}

BOOL CTeacherCom::FetchAnswer(CArray<CStudev, CStudev> &arry, int iStartNO, int iEndNO)
{
	return m_com.FetchAnswer(arry, iStartNO, iEndNO);
}

