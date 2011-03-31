#pragma once
#include "YDCom.h"
#include "afxtempl.h"
#include "TeacherDev.h"
#include "Studev.h"
#include "JahFile.h"
#include "BackUp.h"

class CYDAppCom  
{
public:
	CYDAppCom();
	 ~CYDAppCom();
public:
	BOOL Close();

	BOOL		ResetSYS();
	BOOL		Resetmac();
	CString		GetTeacherdev();
	CString     GetStusingledev();
	BOOL		Reset();
	BOOL		Initsearch();
	BOOL		GetStudev(CStringArray &arry);
	BOOL		Sendvalidmac(CArray<CStudev, CStudev>  &arry);
	int         Sendsinglemac(CString mac);
	BOOL		UpdateStudev(int index, CString strMac);
	BOOL		Addmac(int index, CString mac);
	BOOL		Sendexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO);
	BOOL        Sendexaminfo_new(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM);
	BOOL        Sendsingleinfo_new(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM);
	BOOL		Sendoffexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO);
	BOOL        Sendsingleinfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO);
	BOOL		Sendstepexaminfo(CArray<CStudev, CStudev>  &arry);
	BOOL		Updatestat(CArray<CStudev, CStudev>  &arry, BOOL &bRoll, BOOL &bRolling);
	BOOL		Rolling(int iStartNO, int iEndNO);
	BOOL	    IsRolldone(CArray<CStudev, CStudev>  &arry);
	BOOL		IsRollingdone(CArray<CStudev, CStudev>  &arry);
	BOOL		PreRolling(int index);
	CString        LinkRolling(int iStartNO, int iEndNO);
	BOOL        Linksuccess();
	BOOL        OpenFM();
	BOOL        SearchFM();
	BOOL		CloseFM();
	BOOL        UpFM();
	BOOL        DownFM();
	BOOL		SetFM(CString strFM);
	CString     GetFM();
	BOOL		SingleRolling(int index);
	BOOL        SendFirstMac(CString strMac);

	BOOL        Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO);
	BOOL		Getrelationmap(CStringArray &arryrelation);
	
	BOOL		Opencom(CString strCOM);
	int			Testcom(CArray<CTeacherDev, CTeacherDev>  &arryTeacherdev);
	int         TestStucom(CArray<CTeacherDev, CTeacherDev>  &arryTeacherdev);

	BOOL		Getanswer(CArray<CStudev, CStudev>  &arry, int iStartNO, int iEndNO, CString &strAnswerout);
	BOOL		GetSingleanswer(int index, int iStartNO, int iEndNO, CString &strAnswerout);

	BOOL        FetchAnswer(CArray<CStudev, CStudev> &arry, int iStartNO, int iEndNO);
private:
	CString		contrl(CString cmd, int iTime);
	CYDCom		m_com;
	BOOL		isSearch;
	CString     DectoHexstr(int iDec);
	CString		formatlength(CString strHexMaclength);
	CString     formatNO(CString strNO);
	void		decode(CString input,BYTE *out,int min,int max);
	int			string2Hex(CString str, char *SendOut);
	char		convertHexData(char ch);
	CString     GetExePath();
	CString     m_path;
	CBackUp     m_bk;
	void        getpath();
};


