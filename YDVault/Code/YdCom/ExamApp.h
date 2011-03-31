#pragma once
#include "YDTeacherAppCom.h"
#include "Stusearch.h"
#include "Studev.h"
#include "../DBBase/DatabaseEx.h"

class CYDObjectRef;
class CStudentCom;
//-wym 暂时不用了
class AFX_EXT_CLASS CExamApp  //全局变量，用来存储已经找到的教师机列表和学生机列表
{
public:
	CExamApp();
	virtual ~CExamApp();

	HRESULT    FindTeacherDev(); //查找端口，得到所有的教师机
	HRESULT    TestStu();
	HRESULT    OpenStucom();
	HRESULT    Get_teach_stat(CString strCom,BOOL& _bState);
	HRESULT    Get_teach_count(int& _iCount);
	HRESULT    Search(std::list<CStusearch*> &_lstFindStu,
						std::list<CYDObjectRef*> &_lstUnit,
						BOOL &_bSearch);
	HRESULT    Rollcall_all(std::list<CYDObjectRef*> &_lstStuRef, COleDateTime tStart, COleDateTime tEnd, int iMaxNO);
	HRESULT    Rollcall_all_new(std::list<CYDObjectRef*> &_lstStuRef, 
								COleDateTime tStart, COleDateTime tEnd);
	HRESULT    Rollcall_infra();
	HRESULT    Get_stat(CStringArray &arryRoll,CStringArray &arryRolling, BOOL &bRoll,int &iRolldone, BOOL &bRolling, int &iRollingdone);
	HRESULT    Get_start_time(CString strID,COleDateTime& _strart_time);
	HRESULT    Get_end_time(CString strID,COleDateTime& _end_time);
	HRESULT    Get_stud_count(int &_iCount);
	HRESULT    Rollcall_single(CString strID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO, BOOL bLate);
	HRESULT    Get_singlemac_stat(CString strID, int &iRoll, int &iRolling,BOOL &_bFind);
	HRESULT    Rolling_all(std::list<CExamQuestionSegment*> &_lstQSeg, COleDateTime tEnd);
	HRESULT    Roll_link(CString strID,COleDateTime tStart, COleDateTime tEnd, int iMaxNO);

	HRESULT    Get_answer(std::list<CExamQuestionSegment*> &_lstQSeg, CString strSubject,int paperid, CString strAddr);
	HRESULT    Ismac_in(CString strID,BOOL &_bIn);
	HRESULT    Pre_Rolling(CString strID, COleDateTime tEnd);
	HRESULT    SingleRolling(CString strID, COleDateTime tEnd);
//	HRESULT    LinkRolling(CString strID,int iStart, int iEnd, int paperid, CString strAddr,CString& _strRolling);
//	HRESULT    LinkSuccess();
	HRESULT    Offexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO);
	HRESULT    Offsendmac(std::list<CStudev*> &_lstStuDev);
	HRESULT    Rest();
	HRESULT    OpenFM();
	HRESULT    SearchFM();
	HRESULT    CloseFM();
	HRESULT    UpFM();
	HRESULT    DownFM();
	HRESULT    GetFm(CString& _str);
	HRESULT    SetFM(CString strFM);
	HRESULT    Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO);
	HRESULT    Getrelationmap(CStringArray &arryrelation);
	HRESULT    GetSingleAnswer(CString strID, int iStart, int iEnd,int paperid, CString strAddr);

	HRESULT    FetchAnswer(); //红外模式提取答案；

private:
	std::list<CStudentCom*> m_lstStuCom;
	HRESULT    Assignmac();
	HRESULT    Add(CString strID, COleDateTime tStart, COleDateTime tEnd, BOOL bLate,BOOL bIsLingroll, BOOL bIsLinkrolling);
private:
	HRESULT ClearStuCom();
};


