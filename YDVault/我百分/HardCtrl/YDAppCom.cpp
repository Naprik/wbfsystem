// YDAppCom.cpp: implementation of the CYDAppCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hardctrl.h"
#include "YDAppCom.h"
#include "afxtempl.h"
#include "MainFrm.h"

CYDAppCom::CYDAppCom()
{
	isSearch = false;
	CJahFile    m_file;
	m_file.Remove(GetExePath());

	getpath();
}

CYDAppCom::~CYDAppCom()
{

}

CString GetExePath1()
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

void CYDAppCom::getpath()
{
    CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath1()+"temp.txt" ;
	m_path = capture.m_bmpFile;
	m_path.Replace("\\", "\\\\");
}


BOOL CYDAppCom::ResetSYS()
{
	m_com.Read();
	CString strRecv = contrl("a7 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "b7 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::Resetmac()
{
	m_com.Read();
	CString strRecv = contrl("a9 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "b9 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
/******************************************
��������GetTeacherdev
������
����ֵ����ʦ����������ַ
******************************************/
CString CYDAppCom::GetTeacherdev()
{
	m_com.Read();

	CString strRecv = contrl(getteacherdevCmd, 5);
//	CString strRecv = m_bk.Load("��ʦ����ַ",m_path);
//	strRecv += " ";
	if (strRecv != "")
	{
		if(strRecv.GetLength() == 42)
			strRecv = strRecv.Mid(24,8);
		else
			strRecv = "";
		//if(strRecv.GetLength() == 39)
		//	strRecv = strRecv.Mid(21,8);
	}

	return strRecv;
}

CString CYDAppCom::GetStusingledev()
{
	m_com.Read();

	CString strRecv = contrl(getteacherdevCmd, 5);
	if (strRecv != "")
	{
		//if(strRecv.GetLength() == 42)
		//	strRecv = strRecv.Mid(24,8);
		if(strRecv.GetLength() == 39)
			strRecv = strRecv.Mid(21,8);
		else
			strRecv = "";
	}

	return strRecv;
}
/******************************************
��������Reset
������
����ֵ��TRUE    ��λ�ɹ�
		FASLE   ��λʧ��		
*******************************************/
BOOL CYDAppCom::Reset()
{
	m_com.Read();
	
	CString strRecv = contrl(resetCmd, 5);
//	CString strRecv = "91 00 00 00 0d 0d 0a ";
	if(strRecv == recvResetCmd)
		return TRUE;
	else
		return FALSE;
}
/************************************************
��������Initsearch
������
����ֵ��TRUE
		FALSE
˵�������ʦ����������ָ�ʹ��ʦ����ʼ����ѧ����
************************************************/
BOOL CYDAppCom::Initsearch()
{
	m_com.Read();
	CString strRecv = contrl(searchCmd, 5);
//	CString strRecv = "92 00 00 00 0d 0d 0a ";
	if(strRecv == recvSearchCmd)
		return TRUE;
	else
		return FALSE;

}
/*****************************************************
��������GetStudev
���������������ѧ������ַ���ַ�������
����ֵ��FALSE û�еõ��������
		TRUE  �õ��������������û��������ѧ���������
*****************************************************/
BOOL CYDAppCom::GetStudev(CStringArray &arry)
{
	m_com.Read();
	arry.RemoveAll();
	
	CString strRecv = contrl(getaddrCmd, 5);
//	CString strRecv = m_bk.Load("������ѧ������ַ",m_path);
//	strRecv += " ";
	if(strRecv == "")
		return FALSE;
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString strTotalmac = strRecv.Mid(12, iCmdlength-12-9);
		int iMaccount = (strTotalmac.GetLength()+1)/9;
		for(int i=0; i<iMaccount; i++)
		{
			CString strPerMca = strTotalmac.Mid(9*i,9);
			strPerMca.MakeUpper();
			arry.Add(strPerMca);
		}
		return TRUE;
	}

}
/*******************************************
������Sendvalidmac
�������ַ������� arry�� mac��ַ�ַ���
����ֵ��true ���ͺϷ���ַ����ʦ���ɹ�
		false ʧ��
˵����
*******************************************/
BOOL CYDAppCom::Sendvalidmac(CArray<CStudev, CStudev>  &arry)
{
	m_com.Read();
	
	CString strCmd = "84 00 ";
	
	int iCount = arry.GetSize();
	int iStrlength = iCount *3;
	CString  strIndexHexlength = formatlength(DectoHexstr(iStrlength));
	CString strMac = "";
	for(int i = 0; i < iCount; i++)
	{
		//		int index = arry[i].m_index;
		//		CString  strIndexHexlength = formatNO(DectoHexstr(index));
		//		strMac += strIndexHexlength + arry[i].m_mac + " ";
		strMac = strMac + arry[i].m_mac + " ";
	}
	strCmd = strCmd + strIndexHexlength + strMac + "0d 0d 0a";
	
	CString  strRecv = contrl(strCmd, 5);
	//	CString  strRecv = "94 00 00 00 0d 0d 0a ";
	if(strRecv == recvSendlegaladdrCmd)
		return TRUE;
	else
		return FALSE;
}

BOOL CYDAppCom::SendFirstMac(CString strMac)
{
	m_com.Read();
	CString strCmd = "84 00 00 04 00 ";
	strCmd = strCmd + strMac + " 0d 0d 0a";
	
	CString  strRecv = contrl(strCmd, 5);
	if(strRecv == recvSendlegaladdrCmd)
		return TRUE;
	else
		return FALSE;

}
/*******************************************************************
��������Sendsinglemac
������mac ��Ҫ���͵�ѧ������ַ
����ֵ��TRUE
		FALSE
˵�����˺���ֻ��ʹ���ڵ��������У��ұ����ȷ������кϷ��ĵ�ַ����ʦ����
	  �˵�ַ�����������Щ��ַ����
********************************************************************/
int CYDAppCom::Sendsinglemac(CString mac)
{
	CString strCmd = "89 00 00 03 ";
//	CString  strIndexHexlength = formatNO(DectoHexstr(index));
//	strIndexHexlength.TrimRight();
	strCmd += mac + " " + "0d 0d 0a";
	CString  strRecv = contrl(strCmd, 8);
//	CString  strRecv = "99 00 00 00 0d 0d 0a ";
	if(strRecv == recvSinglemacsuccess)
		return 1;
	else if(strRecv == recvSinglemacfail)
		return 2;
	else
		return 0;
}


/**********************************************
��������UpdateStudev
�������õ�ַ�ڽ�ʦ���еı��index��
      Ҫ���µĵ�ַ 		mac		
����ֵ��TRUE
		FALSE
˵��������ѧ���豸
***********************************************/
BOOL CYDAppCom::UpdateStudev(int index, CString mac)
{
	m_com.Read();
	CString strCmd = "87 00 00 04 ";
	CString  strIndexHexlength = DectoHexstr(index);
	strCmd += formatNO(strIndexHexlength) + mac + " " + "0d 0d 0a";
	CString  strRecv = contrl(strCmd, 5);
	if(strRecv == recvUpdatestudev)
		return TRUE;
	else
		return FALSE;
}
/*********************************************************
��������Addmac
������index �����ĵ�ַ�ı��
	  mac  �����ĵ�ַ
����ֵ��TRUE
		FALSE
˵�����������ĵ�ַ�����ʦ���Ϸ��ĵ�ַ��
**********************************************************/
BOOL CYDAppCom::Addmac(int index, CString strMac)
{
	m_com.Read();
/*	CString strCmd = "88 00 ";
	int iCount = arry.GetSize();
	int iStrlength = iCount *4;
	CString  strIndexHexlength = formatlength(DectoHexstr(iStrlength));
	CString strMac = "";
	for(int i = 0; i < iCount; i++)
	{
		//int index = arry[i].m_index;
		CString  strIndexHexlength = formatNO(DectoHexstr(index));
		index++;
		strMac += strIndexHexlength + arry[i] + " ";
	}
	strCmd = strCmd + strIndexHexlength + strMac + "0d 0d 0a";
	CString  strRecv = contrl(strCmd);
	if(strRecv == recvAddmac)
		return TRUE;
	else
		return FALSE;*/
	CString strCmd = "88 00 00 04 ";
	CString  strIndexHexlength = DectoHexstr(index);
	strMac.TrimRight();
	strCmd += formatNO(strIndexHexlength) + strMac + " " + "0d 0d 0a";	
	CString  strRecv = contrl(strCmd, 5);
	if(strRecv == recvAddmac)
		return TRUE;
	else
		return FALSE;
}


/*************************************
��������Sendexaminfo
������	COleDateTime timeStart ���Կ�ʼ��ʱ��
		COleDateTime timeEnd���Խ���ʱ��
		int iMaxNO   ������
����ֵ��true  ���Ϳ�����Ϣ�ɹ�
		false ʧ��
˵����1����������Ϣָ��
	  2����Ҫ���� ��1��������ʱ��2����ʱ��3������
	  3�����Ϳ�����Ϣָ�����ʦ��
*************************************/
BOOL CYDAppCom::Sendexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO)
{
	m_com.Read();
	
	CString strCmd = "85 00 00 18 ";
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan;
	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;
	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <0)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //��������ʱ��
	if(iAlertspan<1)
		iAlertspan = 1;

	int iStartHour = timeStart.GetHour();
	int iStartMin = timeStart.GetMinute();
	int iEndHour = timeEnd.GetHour();
	int iEndMin = timeEnd.GetMinute();

	CString strHexStartspan = formatlength(DectoHexstr(iStartspan));
	CString strHexEndspan = formatlength(DectoHexstr(iEndspan));
	CString strHexAlertspan = formatlength(DectoHexstr(iAlertspan));
	CString  strHexMaxNO = formatNO(DectoHexstr(iMaxNO));
	CString strHexStartHour = formatNO(DectoHexstr(iStartHour));
	CString strHexStartMin = formatNO(DectoHexstr(iStartMin));
	CString strHexEndHour = formatNO(DectoHexstr(iEndHour));
	CString strHexEndMin = formatNO(DectoHexstr(iEndMin));
		
	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxNO +strHexStartHour+strHexStartMin+strHexEndHour+strHexEndMin+ "0d 0d 0a";
	
	CString  strRecv = contrl(strCmd, 8);
//	CString  strRecv = "95 00 00 00 0d 0d 0a ";
	if(strRecv == recvSendexammessageCmd)
		return true;
	else
		return false;
}

BOOL CYDAppCom::Sendsingleinfo_new(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM)
{
	m_com.Read();
	
	CString strCmd = "8a 00 00 18 ";
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan, timeStartfm1span, timeEndfm1span, timeStartfm2span, timeEndfm2span, timeStartfm3span, timeEndfm3span;
	
	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;
	
	timeStartfm1span = timeStartfm1 - currentimt;
	timeEndfm1span = timeEndfm1 - currentimt;
	
	timeStartfm2span = timeStartfm2 - currentimt;
	timeEndfm2span = timeEndfm2 - currentimt;
	
	timeStartfm3span = timeStartfm3 - currentimt;
	timeEndfm3span = timeEndfm3 - currentimt;
	
	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <0)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //��������ʱ��
	if(iAlertspan<1)
		iAlertspan = 1;
	
	int  iStartfm1span = (int)timeStartfm1span.GetTotalSeconds();
	if(iStartfm1span <= 0 || iFm == 0)
		iStartfm1span = 1;
	int  iEndfm1span = (int)timeEndfm1span.GetTotalSeconds();
	if(iEndfm1span < 0 || iFm == 0)
		iEndfm1span = 2;
	
	int  iStartfm2span = (int)timeStartfm2span.GetTotalSeconds();
	if(iStartfm2span <= 0 || iFm == 0)
		iStartfm2span = 1;
	int  iEndfm2span = (int)timeEndfm2span.GetTotalSeconds();
	if(iEndfm2span < 0 || iFm == 0)
		iEndfm2span = 2;
	
	int  iStartfm3span = (int)timeStartfm3span.GetTotalSeconds();
	if(iStartfm3span <= 0 || iFm == 0)
		iStartfm3span = 1;
	int  iEndfm3span = (int)timeEndfm3span.GetTotalSeconds();
	if(iEndfm3span < 0 || iFm == 0)
		iEndfm3span = 2;
	
	int iFMchanal = atoi(strFM);
	
	
	CString strHexStartspan = formatlength(DectoHexstr(iStartspan));
	CString strHexEndspan = formatlength(DectoHexstr(iEndspan));
	CString strHexAlertspan = formatlength(DectoHexstr(iAlertspan));
	CString strHexMaxno = formatNO(DectoHexstr(iMaxNO));
	CString strHexGrade = formatNO(DectoHexstr(iGrade));
	CString strHexRolling = formatNO(DectoHexstr(iRolling));
	CString strHexFm = formatNO(DectoHexstr(iFm));
	CString strHexchanal = formatlength(DectoHexstr(iFMchanal));
	CString strHexStartfm1span = formatlength(DectoHexstr(iStartfm1span));
	CString strHexEndfm1span = formatlength(DectoHexstr(iEndfm1span));
	CString strHexStartfm2span = formatlength(DectoHexstr(iStartfm2span));
	CString strHexEndfm2span = formatlength(DectoHexstr(iEndfm2span));
	CString strHexStartfm3span = formatlength(DectoHexstr(iStartfm3span));
	CString strHexEndfm3span = formatlength(DectoHexstr(iEndfm3span));
	
	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxno +strHexGrade+strHexRolling+strHexFm+strHexchanal+strHexStartfm1span+strHexEndfm1span+strHexStartfm2span+strHexEndfm2span+strHexStartfm3span+strHexEndfm3span+ "0d 0d 0a";
	
	CString  strRecv = contrl(strCmd, 8);
	//	CString  strRecv = "95 00 00 00 0d 0d 0a ";
	if(strRecv == recvSendsingleinfoCmd)
		return true;
	else
		return false;
}

BOOL CYDAppCom::Sendexaminfo_new(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM)
{
	m_com.Read();

	CString strCmd = "85 00 00 18 ";
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan, timeStartfm1span, timeEndfm1span, timeStartfm2span, timeEndfm2span, timeStartfm3span, timeEndfm3span;

	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;

	timeStartfm1span = timeStartfm1 - currentimt;
	timeEndfm1span = timeEndfm1 - currentimt;

	timeStartfm2span = timeStartfm2 - currentimt;
	timeEndfm2span = timeEndfm2 - currentimt;

	timeStartfm3span = timeStartfm3 - currentimt;
	timeEndfm3span = timeEndfm3 - currentimt;

	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <0)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //��������ʱ��
	if(iAlertspan<1)
		iAlertspan = 1;

	int  iStartfm1span = (int)timeStartfm1span.GetTotalSeconds();
	if(iStartfm1span <= 0 || iFm == 0)
		iStartfm1span = 1;
	int  iEndfm1span = (int)timeEndfm1span.GetTotalSeconds();
	if(iEndfm1span < 0 || iFm == 0)
		iEndfm1span = 2;

	int  iStartfm2span = (int)timeStartfm2span.GetTotalSeconds();
	if(iStartfm2span <= 0 || iFm == 0)
		iStartfm2span = 1;
	int  iEndfm2span = (int)timeEndfm2span.GetTotalSeconds();
	if(iEndfm2span < 0 || iFm == 0)
		iEndfm2span = 2;

	int  iStartfm3span = (int)timeStartfm3span.GetTotalSeconds();
	if(iStartfm3span <= 0 || iFm == 0)
		iStartfm3span = 1;
	int  iEndfm3span = (int)timeEndfm3span.GetTotalSeconds();
	if(iEndfm3span < 0 || iFm == 0)
		iEndfm3span = 2;

	int iFMchanal = atoi(strFM);


	CString strHexStartspan = formatlength(DectoHexstr(iStartspan));
	CString strHexEndspan = formatlength(DectoHexstr(iEndspan));
	CString strHexAlertspan = formatlength(DectoHexstr(iAlertspan));
	CString strHexMaxno = formatNO(DectoHexstr(iMaxNO));
	CString strHexGrade = formatNO(DectoHexstr(iGrade));
	CString strHexRolling = formatNO(DectoHexstr(iRolling));
	CString strHexFm = formatNO(DectoHexstr(iFm));
	CString strHexchanal = formatlength(DectoHexstr(iFMchanal));
	CString strHexStartfm1span = formatlength(DectoHexstr(iStartfm1span));
	CString strHexEndfm1span = formatlength(DectoHexstr(iEndfm1span));
	CString strHexStartfm2span = formatlength(DectoHexstr(iStartfm2span));
	CString strHexEndfm2span = formatlength(DectoHexstr(iEndfm2span));
	CString strHexStartfm3span = formatlength(DectoHexstr(iStartfm3span));
	CString strHexEndfm3span = formatlength(DectoHexstr(iEndfm3span));

	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxno +strHexGrade+strHexRolling+strHexFm+strHexchanal+strHexStartfm1span+strHexEndfm1span+strHexStartfm2span+strHexEndfm2span+strHexStartfm3span+strHexEndfm3span+ "0d 0d 0a";

	CString  strRecv = contrl(strCmd, 8);
//	CString  strRecv = "95 00 00 00 0d 0d 0a ";
	if(strRecv == recvSendexammessageCmd)
		return true;
	else
		return false;
}


BOOL CYDAppCom::Sendoffexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO)
{
	m_com.Read();
	
	CString strCmd = "ab 00 00 0c ";
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan;
	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;
	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <= 1)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //��������ʱ��
	if(iAlertspan<1)
		iAlertspan = 1;

	int iStartHour = timeStart.GetHour();
	int iStartMin = timeStart.GetMinute();
	int iEndHour = timeEnd.GetHour();
	int iEndMin = timeEnd.GetMinute();

	CString strHexStartspan = formatlength(DectoHexstr(iStartspan));
	CString strHexEndspan = formatlength(DectoHexstr(iEndspan));
	CString strHexAlertspan = formatlength(DectoHexstr(iAlertspan));
	CString  strHexMaxNO = formatNO(DectoHexstr(iMaxNO));
	CString strHexStartHour = formatNO(DectoHexstr(iStartHour));
	CString strHexStartMin = formatNO(DectoHexstr(iStartMin));
	CString strHexEndHour = formatNO(DectoHexstr(iEndHour));
	CString strHexEndMin = formatNO(DectoHexstr(iEndMin));
	CString strHexStartNO = formatNO(DectoHexstr(iStartNO));
	CString strHexEndNO = formatNO(DectoHexstr(iEndNO));
		
	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxNO +strHexStartHour+strHexStartMin+strHexEndHour+strHexEndMin+strHexStartNO+strHexEndNO+ "0d 0d 0a";
	
	CString  strRecv = contrl(strCmd, 8);
//	CString  strRecv = "95 00 00 00 0d 0d 0a ";
	if(strRecv == "bb 00 00 00 0d 0d 0a ")
		return true;
	else
		return false;
}

BOOL CYDAppCom::Sendsingleinfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO)
{
	m_com.Read();
	
	CString strCmd = "8a 00 00 0b ";
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan;
	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;
	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <= 1)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //��������ʱ��
	if(iAlertspan<1)
		iAlertspan = 1;

	int iStartHour = timeStart.GetHour();
	int iStartMin = timeStart.GetMinute();
	int iEndHour = timeEnd.GetHour();
	int iEndMin = timeEnd.GetMinute();

	CString strHexStartspan = formatlength(DectoHexstr(iStartspan));
	CString strHexEndspan = formatlength(DectoHexstr(iEndspan));
	CString strHexAlertspan = formatlength(DectoHexstr(iAlertspan));
	CString  strHexMaxNO = formatNO(DectoHexstr(iMaxNO));
	CString strHexStartHour = formatNO(DectoHexstr(iStartHour));
	CString strHexStartMin = formatNO(DectoHexstr(iStartMin));
	CString strHexEndHour = formatNO(DectoHexstr(iEndHour));
	CString strHexEndMin = formatNO(DectoHexstr(iEndMin));
		
	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxNO +strHexStartHour+strHexStartMin+strHexEndHour+strHexEndMin+ "0d 0d 0a";
	
	CString  strRecv = contrl(strCmd, 8);
//	CString  strRecv = "9a 00 00 00 0d 0d 0a ";
	if(strRecv == recvSendsingleinfoCmd)
		return true;
	else
		return false;
}
/**************************************************************************
��������Sendstepexaminfo
������CArray<CStudev, CStudev>  arryѧ����Ϣ�б�
����ֵ��TRUE  ���·��ͳɹ�
		FALSE ʧ��
˵������ʧ�ܵĿ������·��Ϳ�����Ϣ����ֻҪ����δ�ɹ���ѧ�������б��е����
***************************************************************************/

BOOL CYDAppCom::Sendstepexaminfo(CArray<CStudev, CStudev>  &arry)
{

	m_com.Read();
	
	CString strCmd = "86 00 ";
	int iCount = arry.GetSize();
	int iLength = 0;
	CString strIndex = "";
	for(int i = 0; i < iCount; i++)
	{
		if(arry[i].m_rollcall == 2)
		{
			int index = arry[i].m_index;
			CString strIndexHexlength = formatNO(DectoHexstr(index));
			strIndex += strIndexHexlength;
			iLength++;
		}
	}

	CString strHexlength = formatlength(DectoHexstr(iLength));
	strCmd += strHexlength+ strIndex + "0d 0d 0a";

	CString  strRecv = contrl(strCmd, 8);
	if(strRecv == recvSendstepexammessageCmd)
		return true;
	else
		return false;

}

/**********************************************************
��������Updatestat
������CArray<CStudev, CStudev>  &arryѧ����ַ�б�
����ֵ��
˵���������б���ѧ��״̬��־λ
	  ��׼λ��Ϊ2����
	  ǰ����Ϊ ������Ϣ���ͱ�־λ00 ����ʧ�� 01 ������Ϣ���ͳɹ� 02 ʧ��
	  �󲿷�Ϊ �վ���Ϣ��׼λ00 ����ʧ�� 01 �վ�ɹ� 02 ʧ�� 03 ��ǰ����
**********************************************************/
BOOL  CYDAppCom::Updatestat(CArray<CStudev, CStudev>  &arry, BOOL &bRoll, BOOL &bRolling)
{
	m_com.Read();
	
	CString strRecv = contrl(statCmd, 2);


//	CString strRecv = "9b 00 00 34 00 01 01 01 02 01 03 01 04 01 05 01 06 01 07 01 08 01 09 01 0a 01 0b 01 0c 01 00 01 01 01 02 01 03 01 04 01 05 01 06 01 07 01 08 01 09 01 0a 01 0b 01 0c 01 0d 0d 0a ";
//	CString strRecv = m_bk.Load("״̬��־λ",m_path);
//	strRecv += " ";
	if(strRecv == "")
		return FALSE;
	else
	{
		int iCount = arry.GetSize();
		CString strRollStat = strRecv.Mid(12, 2);
		if(strRollStat == "00")
			bRoll = FALSE;
		else if(strRollStat == "01")
			bRoll = TRUE;
		else
			bRoll = FALSE;

/*		CString strRollingStat = strRecv.Mid(15+iCount*6,2);
		if(strRollingStat == "00")
			bRolling = FALSE;
		else if(strRollingStat == "01")
			bRolling = TRUE;
		else
			bRolling = FALSE;*/

		CString strRollingStat = strRecv.Mid(15,2);
		if(strRollingStat == "00")
			bRolling = FALSE;
		else if(strRollingStat == "01")
			bRolling = TRUE;
		else
			bRolling = FALSE;


		int iLength = strRecv.GetLength();
		CString strStat = strRecv.Mid(18, iLength-18);
		for(int i = 0; i<iCount; i++)
		{
			CString strRecvMac = strStat.Mid(i*15+0, 8);
			strRecvMac.MakeUpper();
			strRecvMac.TrimLeft();
			strRecvMac.TrimRight();
			for (int j = 0; j < arry.GetSize(); j++)
			{
				CString strMac = arry[j].m_mac;
				strMac.MakeUpper();
				strMac.TrimLeft();
				strMac.TrimRight();
				if (strMac == strRecvMac)
				{
					CString strRollcall = strStat.Mid(i*15+9,2);
					if(strRollcall == "00")
	 					arry[j].m_rollcall = 0;
		 			else if(strRollcall == "01")
						arry[j].m_rollcall = 1;
					else
						arry[j].m_rollcall = 2;
					 
					CString strRolling = strStat.Mid(i*15+12,2);
					if(strRolling == "00")
						arry[j].m_rolling = 0;
					else if(strRolling == "01")
						arry[j].m_rolling = 1;
					else if(strRolling == "02")
						arry[j].m_rolling = 2;
					else
					arry[j].m_rolling = 3;
				}

			}
// 			CString strRollcall = strStat.Mid(i*15+9,2);
// 			if(strRollcall == "00")
// 				arry[i].m_rollcall = 0;
// 			else if(strRollcall == "01")
// 				arry[i].m_rollcall = 1;
// 			else
// 				arry[i].m_rollcall = 2;
// 
// 			CString strRolling = strStat.Mid(i*15+12,2);
// 			if(strRolling == "00")
// 				arry[i].m_rolling = 0;
// 			else if(strRolling == "01")
// 				arry[i].m_rolling = 1;
// 			else if(strRolling == "02")
// 				arry[i].m_rolling = 2;
// 			else
// 				arry[i].m_rolling = 3;
		}
		return TRUE;
	}
}
BOOL CYDAppCom::IsRolldone(CArray<CStudev, CStudev>  &arry)
{
	m_com.Read();
	CString strRecv = contrl(statCmd, 5);
	if(strRecv == "")
		return FALSE;
	else
	{
		CString strStat = strRecv.Mid(12, 2);
		if(strStat == "00")
		{
			return FALSE;
		}
		else if(strStat == "01")
		{
			return TRUE;
		}
		else
			return FALSE;
	}
}
BOOL CYDAppCom::IsRollingdone(CArray<CStudev, CStudev>  &arry)
{
	m_com.Read();
	CString strRecv = contrl(statCmd, 5);
	if(strRecv == "")
		return FALSE;
	else
	{
		int iCount = arry.GetSize();
		CString strStat = strRecv.Mid(15+iCount*6,2);
		if(strStat == "00")
		{
			return FALSE;
		}
		else if(strStat == "01")
		{
			return TRUE;
		}
		else
			return FALSE;
	}

}
/*****************************************
��������Rolling
������iStartNO ��ʼ���  
	  iEndNO   �������
����ֵ��true 
        false
******************************************/
BOOL CYDAppCom::Rolling(int iStartNO, int iEndNO)
{
	m_com.Read();
	
	CString strCMD = "8c 00 00 02 ";
	CString strStartNO = formatNO(DectoHexstr(iStartNO));
	CString strEndNO = formatNO(DectoHexstr(iEndNO));
	strCMD = strCMD + strStartNO + strEndNO + "0d 0d 0a";
	
	CString strRecv = contrl(strCMD, 5);
//	CString strRecv = "9c 00 00 00 0d 0d 0a ";
	if(strRecv == recvRollingCmd)
		return true;
	else
		return false;	
}



BOOL CYDAppCom::Linksuccess()
{
	m_com.Read();
	CString strCMD = "a3 00 00 00 0d 0d 0a";
	CString strRecv = contrl(strCMD, 5);
	if(strRecv == "b3 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
/******************************************************
��������PreRolling
������index    ѧ�����ڽ�ʦ���е����
����ֵ
˵������ѧ������ǰ����ģ����е����վ�
******************************************************/
BOOL CYDAppCom::PreRolling(int index)
{
	m_com.Read();

	CString strCMD = "a9 00 00 01 ";
	CString  strIndexHexlength = formatNO(DectoHexstr(index));
	strCMD += strIndexHexlength + "0d 0d 0a";

	CString strRecv = contrl(strCMD, 5);
	if(strRecv == "b9 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::SingleRolling(int index)
{
	m_com.Read();

	CString strCMD = "a2 00 00 01 ";
	CString  strIndexHexlength = formatNO(DectoHexstr(index));
	strCMD += strIndexHexlength + "0d 0d 0a";

	CString strRecv = contrl(strCMD, 5);
	if(strRecv == "b2 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}


BOOL CYDAppCom::Opencom(CString strCOM)
{
	return  m_com.OpenComm(strCOM);
}
BOOL CYDAppCom::Close()
{
	return m_com.Close();
}
/*****************************************************************************
������Testcom()
������CArray<CTeacherDev, CTeacherDev>  &arryTeacherdev    ��ʦ���������
����ֵ�������н�ʦ����������
˵��������ÿ���˿ڣ�����˿ڴ��ڽ�ʦ�������ȡ��ʦ����mac��ַ��������ַ�Ͷ˿ڼ�¼��������
******************************************************************************/
int CYDAppCom::Testcom(CArray<CTeacherDev, CTeacherDev>  &arryTeacherdev)
{
	m_com.Read();

	for(int i = 1; i < 15; i++)
	{
		CString strTemp;
		strTemp.Format("com%d",i);
		if(Opencom(strTemp))
		{
			CString strRecv = GetTeacherdev();
		//	CString strRecv = "00 00 00 00 00 22";
			if(strRecv != "")
			{
				CTeacherDev td;
				memset(td.m_com,0,sizeof(td.m_com));
				strcpy(td.m_com,strTemp);
				memset(td.m_mac,0,sizeof(td.m_mac));
				strcpy(td.m_mac,strRecv);
				arryTeacherdev.Add(td);
			}
			Close();
		}
	}
	return arryTeacherdev.GetSize();
}

int  CYDAppCom::TestStucom(CArray<CTeacherDev, CTeacherDev>  &arryTeacherdev)
{
	m_com.Read();

	for(int i = 1; i < 15; i++)
	{
		CString strTemp;
		strTemp.Format("com%d",i);
		if(Opencom(strTemp))
		{
			CString strRecv = GetStusingledev();
		//	CString strRecv = "00 00 00 00 00 22";
			if(strRecv != "")
			{
				CTeacherDev td;
				memset(td.m_com,0,sizeof(td.m_com));
				strcpy(td.m_com,strTemp);
				memset(td.m_mac,0,sizeof(td.m_mac));
				strcpy(td.m_mac,strRecv);
				arryTeacherdev.Add(td);
			}
			Close();
		}
	}
	return arryTeacherdev.GetSize();
}


BOOL CYDAppCom::OpenFM()
{
	m_com.Read();
	CString strRecv = contrl("aa 00 00 07 01 00 00 00 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "ba 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::SearchFM()
{
	m_com.Read();
	CString strRecv = contrl("aa 00 00 07 00 00 01 00 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "ba 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::CloseFM()
{
	m_com.Read();
	CString strRecv = contrl("aa 00 00 07 00 01 00 00 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "ba 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::UpFM()
{
	m_com.Read();
	CString strRecv = contrl("aa 00 00 07 00 00 00 01 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "ba 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::DownFM()
{
	m_com.Read();
	CString strRecv = contrl("aa 00 00 07 00 00 00 00 01 00 00 0d 0d 0a ", 5);
	if(strRecv == "ba 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
BOOL CYDAppCom::SetFM(CString strFM)
{
	m_com.Read();
	int iFM = atoi(strFM);
	
	CString strCMD = "aa 00 00 07 00 00 00 00 00 ";
	strCMD = strCMD + formatlength(DectoHexstr(iFM)) + "0d 0d 0a";
	CString strRecv = contrl(strCMD, 5);
	if(strRecv == "ba 00 00 00 0d 0d 0a ")
		return TRUE;
	else
		return FALSE;
}
CString CYDAppCom::GetFM()
{
	m_com.Read();
	CString strRecv = contrl("ac 00 00 00 0d 0d 0a ", 5);
	if(strRecv == "")
		return "";
	else
		return strRecv.Mid(12,5);
}


/********************************************************
��������Getanswer
������CArray<CStudev, CStudev>  &arry ѧ����̬����
	iStartNO          ��ʼ�����
	iEndNO            ���������
����ֵ��TRUE    ��ȡ��ʧ��
		FALSE   �ɹ�
˵�����Եõ��Ĵ𰸽��н��룬��ŵ�ѧ����̬������
********************************************************/

CString CYDAppCom::LinkRolling(int iStartNO, int iEndNO)
{
	m_com.Read();
	
	CString strCMD = "8c 00 00 02 ";
	CString strStartNO = formatNO(DectoHexstr(iStartNO));
	CString strEndNO = formatNO(DectoHexstr(iEndNO));
	strCMD = strCMD + strStartNO + strEndNO + "0d 0d 0a";
	
	CString strRecv = contrl(strCMD, 5);
	if(strRecv == "")
	{
		AfxMessageBox("û�н��յ���");
		return "";
	}
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString strTotalanswer = strRecv.Mid(12, iCmdlength-12-15);

		BYTE out[1000];
		decode(strTotalanswer,out,0,199);
		CString answerstr = "";
		for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
		{
			CString tmp;
			if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
				tmp = "0";
			else
				tmp.Format("%d",out[j]);
			answerstr = answerstr+tmp;
		}	
		CString strsutanswer = ""; 
		for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
		{
			CString test = answerstr.Mid(strlength,1);
			if(answerstr.Mid(strlength,1) == "1")
				strsutanswer += "A";
			else if(answerstr.Mid(strlength,1) == "2")
				strsutanswer += "B";
			else if(answerstr.Mid(strlength,1) == "3")
				strsutanswer += "C";
			else if(answerstr.Mid(strlength,1) == "4")
				strsutanswer += "D";
			else
				strsutanswer += "0";
		}
		return strsutanswer;
	}
}

BOOL CYDAppCom::GetSingleanswer(int index, int iStartNO, int iEndNO, CString &strAnswerout)
{
	m_com.Read();
	CString strRecv = contrl(getanswerCmd, 5);
	if(strRecv == "")
	{
		AfxMessageBox("û�н��յ���");
		return FALSE;
	}
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString strTotalanswer = strRecv.Mid(18, iCmdlength-18-9);
		int iPeranswer = 303;
	
		CString strAnswer = strTotalanswer.Mid(index*iPeranswer+3,iPeranswer-3);
		BYTE out[1000];
		decode(strAnswer,out,0,199);
		CString answerstr = "";
		for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
		{
			CString tmp;
			if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
				tmp = "0";
			else
				tmp.Format("%d",out[j]);
			answerstr = answerstr+tmp;
		}	
		CString strsutanswer = ""; 
		for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
		{
			CString test = answerstr.Mid(strlength,1);
			if(answerstr.Mid(strlength,1) == "1")
				strsutanswer += "A";
			else if(answerstr.Mid(strlength,1) == "2")
				strsutanswer += "B";
			else if(answerstr.Mid(strlength,1) == "3")
				strsutanswer += "C";
			else if(answerstr.Mid(strlength,1) == "4")
				strsutanswer += "D";
			else
				strsutanswer += "0";
		}
		strAnswerout = strsutanswer;
	}
	return TRUE;
}
BOOL CYDAppCom::Getanswer(CArray<CStudev, CStudev>  &arry, int iStartNO, int iEndNO, CString &strAnswerout)
{
	m_com.Read();

	CString strRecv = contrl(getanswerCmd, 5);
//	CString strRecv = m_bk.Load("ѧ����",m_path);
//	strRecv += " ";
//	CString strRecv ="9d 00 00 c3 01 0a 00 45 19 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 59 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 28 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 43 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 53 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 32 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 34 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 57 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 2c 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 4f 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 3a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 2a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 55 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0d 0d 0a ";
	strAnswerout = strRecv;
//	AfxMessageBox(strAnswerout);
	if(strRecv == "")
	{
		AfxMessageBox("û�н��յ���");
		return FALSE;
	}
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString strTotalanswer = strRecv.Mid(18, iCmdlength-18-9);
		int iPeranswer = strTotalanswer.GetLength()/(arry.GetSize());
		for(int i = 0; i < arry.GetSize(); i++)
		{
			CString strAnswer = strTotalanswer.Mid(i*iPeranswer+15,iPeranswer-15);
			BYTE out[1000];
			decode(strAnswer,out,0,199);
			CString answerstr = "";
			for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
			{
				CString tmp;
				if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
					tmp = "0";
				else
					tmp.Format("%d",out[j]);
				answerstr = answerstr+tmp;
			}	
			CString strsutanswer = ""; 
			for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
			{
				CString test = answerstr.Mid(strlength,1);
				if(answerstr.Mid(strlength,1) == "1")
					strsutanswer += "A";
				else if(answerstr.Mid(strlength,1) == "2")
					strsutanswer += "B";
				else if(answerstr.Mid(strlength,1) == "3")
					strsutanswer += "C";
				else if(answerstr.Mid(strlength,1) == "4")
					strsutanswer += "D";
				else
					strsutanswer += "0";
			}
			CString strMac = strTotalanswer.Mid(i*iPeranswer+6, 8);
			strMac.MakeUpper();
			for(int k = 0; k < arry.GetSize(); k++)
			{
				CString strarryMac = arry[k].m_mac;
				strarryMac.MakeUpper();
				strarryMac.TrimLeft();
				strarryMac.TrimRight();
				if(strarryMac == strMac)
				{
					memset(arry[k].m_answer, 0, sizeof(arry[k].m_answer));
					strcpy(arry[k].m_answer, strsutanswer);
					arry.SetAt(k,arry[k]);
				}
			}
		}
	}
	
	return TRUE;
}

BOOL CYDAppCom::Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO)
{
	arryList.RemoveAll();
	m_com.Read();
	CString strRecv = contrl(getanswerCmd, 5);
	if(strRecv == "")
		return FALSE;
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString strTotalanswer = strRecv.Mid(18, iCmdlength-18-9);
		int count = strTotalanswer.GetLength()/303;
		for(int i = 0; i < count; i++)
		{
			CString strAnswer = strTotalanswer.Mid(i*303+3,300);
			BYTE out[1000];
			decode(strAnswer,out,0,199);
			CString answerstr = "";
			for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
			{
				CString tmp;
				if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
					tmp = "0";
				else
					tmp.Format("%d",out[j]);
				answerstr = answerstr+tmp;
			}
			CString strsutanswer = ""; 
			for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
			{
				CString test = answerstr.Mid(strlength,1);
				if(answerstr.Mid(strlength,1) == "1")
					strsutanswer += "A";
				else if(answerstr.Mid(strlength,1) == "2")
					strsutanswer += "B";
				else if(answerstr.Mid(strlength,1) == "3")
					strsutanswer += "C";
				else if(answerstr.Mid(strlength,1) == "4")
					strsutanswer += "D";
				else
					strsutanswer += "0";
			}
			arryList.Add(strsutanswer);
		}
		return TRUE;
	}
}

BOOL CYDAppCom::FetchAnswer(CArray<CStudev, CStudev> &arry, int iStartNO, int iEndNO)
{
	CString strRecv = contrl("8d 00 00 00 0d 0d 0a", 5);
	if(strRecv == "")
		return FALSE;
	else
	{
		//AfxMessageBox(strRecv);
		int iLen = strRecv.GetLength();
		CString sTotalAnswer = strRecv.Mid(18, iLen-18-9);
		int tt = sTotalAnswer.GetLength();
		int iStuCount = sTotalAnswer.GetLength()/(105*3);
		int itemp = sTotalAnswer.GetLength()%(105*3);
		if (itemp != 0)
		{
			return FALSE;
		}
		for(int i = 0; i < iStuCount; i++)
		{
			CString sStuIndex = sTotalAnswer.Mid(0+i*105*3, 5);
			sStuIndex.Replace(" ", "");
			int  iStuIndex = (int)strtol(sStuIndex,NULL,16);
			CString sStuMac = sTotalAnswer.Mid(6+i*105*3, 8);
			sStuMac.TrimRight();
			sStuMac.TrimLeft();
			sStuMac.MakeUpper();
			CString sStuAnswer = sTotalAnswer.Mid(15+i*105*3,300);
			BYTE out[1000];
			decode(sStuAnswer,out,0,199);
			CString answerstr = "";
			for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
			{
				CString tmp;
				if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
					tmp = "0";
				else
					tmp.Format("%d",out[j]);
				answerstr = answerstr+tmp;
			}
			CString strsutanswer = ""; 
			for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
			{
				CString test = answerstr.Mid(strlength,1);
				if(answerstr.Mid(strlength,1) == "1")
					strsutanswer += "A";
				else if(answerstr.Mid(strlength,1) == "2")
					strsutanswer += "B";
				else if(answerstr.Mid(strlength,1) == "3")
					strsutanswer += "C";
				else if(answerstr.Mid(strlength,1) == "4")
					strsutanswer += "D";
				else
					strsutanswer += "0";
			}
			
			CStudev  sd;
			sd.m_index = iStuIndex;
			memset(sd.m_mac, 0, sizeof(sd.m_mac));
			strcpy(sd.m_mac, sStuMac);
			memset(sd.m_answer, 0, sizeof(sd.m_answer));
			strcpy(sd.m_answer, strsutanswer);
			arry.Add(sd);
			
		}
	}
	return TRUE;
}


BOOL CYDAppCom::Getrelationmap(CStringArray &arryrelation)
{
	arryrelation.RemoveAll();
	m_com.Read();
	CString strRecv = contrl("a3 00 00 00 0d 0d 0a", 5);
	if(strRecv == "")
		return FALSE;
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString total = strRecv.Mid(12, iCmdlength-12-9);
		int count = total.GetLength()/12;
		for(int i = 0; i < count; i++)
		{
			CString mac = total.Mid(3+i*12, 8);
			arryrelation.Add(mac);
		}
		return TRUE;
	}
}
/***********************************************
������contrl
������CString cmd���͸���ʦ����ָ��
����ֵ��CString ��ʦ�����ص�ָ��
˵����������ָ������ʦ��3�����޻ظ����򷵻ؿգ�
***********************************************/
CString CYDAppCom::GetExePath()
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
	CString temp = file + "syslog.txt";
	return temp;
}
CString CYDAppCom::contrl(CString cmd, int iTime)
{	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CJahFile    m_file;
	COleDateTime tm;
	tm = COleDateTime::GetCurrentTime();
	CString temp = tm.Format()+" SEND "+" "+cmd+"\r\n";
	CString file = GetExePath();
	m_file.Open(file,FALSE);
	m_file.WriteString(temp);
	fram->m_dlgMsg.ShowMsg(temp);

	if(!m_com.Send(cmd))
		return "";
	CString recv;
	COleDateTime t1, t2;
	t1 = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeSpan; 
	int  span=0;
	do
	{
		recv = m_com.Read();
		t2 = COleDateTime::GetCurrentTime();
		timeSpan = t2 - t1;
		span = (int)timeSpan.GetTotalSeconds();
	}while((recv == "")&&(span <= iTime));
	tm = COleDateTime::GetCurrentTime();
	temp = tm.Format()+" RECV "+" "+recv+"\r\n";
	m_file.WriteString(temp);
	fram->m_dlgMsg.ShowMsg(temp);
	m_file.Close();
	return recv;
}

CString CYDAppCom::DectoHexstr(int iDec)
{
	CString temp = "";
	char hex[4];
	if(iDec >= 65535)
		iDec = 0;
	sprintf(hex, "%x", iDec);
	temp = hex;
	return temp;
}
/**********************************
��������formatlength
������strHexMaclength(16���Ƶ�ַ������)
����ֵ��ָ���б�׼�ĵ�ַ����
˵����ָ���е�ַ���ȸ�ʽΪ"xx xx" ͨ��ת�������ĵ�ַ���ȣ����б�׼��
***********************************/
CString CYDAppCom::formatlength(CString strHexMaclength)
{
	CString ret = "";
	if(strHexMaclength == "0")
		return "00 00 ";
	int i = strHexMaclength.GetLength();
	switch(i)
	{
	case 0:
		ret = "00 00 ";
		return ret;
		break;
	case 1:
		ret = "00 0" + strHexMaclength + " ";
		return ret;
		break;
	case 2:
		ret = "00 " + strHexMaclength + " ";
		return ret;
	case 3:
		strHexMaclength.Insert(0, "0");
		strHexMaclength.Insert(2, " ");
		strHexMaclength.Insert(5, " ");
		ret = strHexMaclength;
		return ret;
		break;
	case 4:
		strHexMaclength.Insert(2, " ");
		strHexMaclength.Insert(5, " ");
		ret = strHexMaclength;
		return ret;
		break;
	default:
		return ret;
		break;
	}
}
/************************************
��������formatNO
������strNO(16������ų����ַ���)
����ֵ��ָ���б�׼����ų���
˵����
************************************/
CString CYDAppCom::formatNO(CString strNO)
{
	int iLength = strNO.GetLength();
	if(iLength == 2)
	{
		strNO = strNO+" ";
	}
	else if(iLength == 1)
	{
		//if(strNO == "0")
		//	strNO = "ff ";
	//	else
			strNO = "0"+strNO+" ";
	}
	else 
		strNO = "ff ";

	return strNO;
}



void CYDAppCom::decode(CString input,BYTE *out,int min,int max)
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

int CYDAppCom::string2Hex(CString str, char *SendOut)
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
char CYDAppCom::convertHexData(char ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
    else return(-1);
}

