#pragma once
#include "YDAppCom.h"
#include "TeacherCom.h"
#include "TeacherDev.h"
#include "Stusearch.h"
#include "Studev.h"

class CExamApp  
{
public:
	CExamApp();
	virtual ~CExamApp();

	BOOL    Test();
	BOOL	TestStu();
	BOOL    Opencom();
	BOOL    OpenStucom();
	BOOL	CloseStucom();
	BOOL	Get_teach_stat(CString strCom);
	int		Get_teach_count();
	BOOL	Search(CArray<CStusearch, CStusearch> &arryvalid, CStringArray &arryList);
	BOOL	Rollcall_all(CStringArray &arryvalidID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO);
	BOOL	Rollcall_all_new(CStringArray &arryvalidID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO);
	BOOL    Rollcall_single_new(CString strID, COleDateTime tStart, COleDateTime tEnd);
	BOOL    Rollcall_infra();
	void	Get_stat(CStringArray &arryRoll,CStringArray &arryRolling, BOOL &bRoll,int &iRolldone, BOOL &bRolling, int &iRollingdone);
	COleDateTime  Get_start_time(CString strID);
	COleDateTime  Get_end_time(CString strID);
	int		Get_stud_count();
	BOOL    Rollcall_single(CString strID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO, BOOL bLate);
	BOOL    Get_singlemac_stat(CString strID, int &iRoll, int &iRolling);
	BOOL    Rolling_all(int iStart, int iEnd, COleDateTime tEnd);
	BOOL	Roll_link(CString strID,COleDateTime tStart, COleDateTime tEnd, int iMaxNO);

	BOOL	Get_answer(int iStart, int iEnd, CString strSubject,int paperid, CString strAddr);
	BOOL    Ismac_in(CString strID);
	BOOL    Pre_Rolling(CString strID, COleDateTime tEnd);
	BOOL    SingleRolling(CString strID, COleDateTime tEnd);
	CString    LinkRolling(CString strID,int iStart, int iEnd, int paperid, CString strAddr);
	BOOL	LinkSuccess();
	BOOL    Offexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO);
	BOOL    Offsendmac(CArray<CStudev, CStudev> &arry);
	BOOL	Rest();
	BOOL    OpenFM();
	BOOL    SearchFM();
	BOOL    CloseFM();
	BOOL    UpFM();
	BOOL	DownFM();
	CString GetFm();
	BOOL	SetFM(CString strFM);
	BOOL    Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO);
	BOOL	Getrelationmap(CStringArray &arryrelation);
	BOOL	GetSingleAnswer(CString strID, int iStart, int iEnd,int paperid, CString strAddr);

	BOOL    FetchAnswer(int iStartNO, int iEndNO); //红外模式提取答案；
	BOOL	GreateExamInfo();
private:
	CTeacherCom    m_com1;
	CTeacherCom	   m_com2;
	CTeacherCom    m_com3;
	void  assignmac();
	void  add(CString strID, COleDateTime tStart, COleDateTime tEnd, BOOL bLate,BOOL bIsLingroll, BOOL bIsLinkrolling);
	CString getExePath();
	
};


