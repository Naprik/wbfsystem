#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Studev.h"
#include "YDAppCom.h"

class CTeacherCom  
{
public:
	CTeacherCom();
	virtual ~CTeacherCom();

	BOOL          m_isuse;           //教师机是否使用
	CString		  m_port;			//教师机的串口
	BOOL          Opencom(CString  strCOM, CString strMac);
	BOOL		  Closecom();
	CString		  Get_Comstat_text();
	BOOL		  Search(CStringArray &arry);
	void		  Setvalidmac(CString strID, CString strMac, COleDateTime tStart, COleDateTime tEnd);
	BOOL		  Rollcall_all(COleDateTime tStart, COleDateTime tEnd, int iMaxNO);
	BOOL		  Rollcall_all_new(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM);
	BOOL          Rollcall_single_new(CArray<CStudev, CStudev> &arry,CString strID, CString strMac,COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO, int iGrade, int iRolling, int iFm, COleDateTime timeStartfm1, COleDateTime timeEndfm1, COleDateTime timeStartfm2, COleDateTime timeEndfm2, COleDateTime timeStartfm3, COleDateTime timeEndfm3, CString strFM);
	void		  Get_roll_stat(CArray<CStudev, CStudev> &arry, BOOL &bRoll, BOOL &bRolling);
	BOOL		  Rollcall_single(CArray<CStudev, CStudev> &arry,CString strID, CString strMac,COleDateTime tStart, COleDateTime tEnd, int iMaxNO);
	BOOL	      Rolling(int iStart, int iEnd);
	BOOL		  Getanswer(CArray<CStudev, CStudev> &arry, int iStart, int iEnd, CString &strAnswer);
	BOOL		  Ismac_in(CString strID);
	BOOL		  Pre_rolling(CString strID);
	BOOL		  SingleRolling(CString strID);
	BOOL		  Roll_link(COleDateTime tStart, COleDateTime tEnd, int iMaxNO);
	CString		  LinkRolling(int iStart, int iEnd);
	BOOL		  LinkSuccess();
	BOOL          Offexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO);
	BOOL		  Offsendmac(CArray<CStudev, CStudev> &arry);
	BOOL          Rest();
	BOOL          OpenFM();
	BOOL          SearchFM();
	BOOL          CloseFM();
	BOOL          UpFM();
	BOOL		  DownFM();
	BOOL		  SetFM(CString strFM);
	CString       GetFM();
	CString		  BackAnwer();
	BOOL          Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO);
	BOOL		  Getrelationmap(CStringArray &arryrelation);
	BOOL		  GetSingleAnswer(CString strID, int iStartNO, int iEndNO, CString &strAnswerout);

	BOOL		  FetchAnswer(CArray<CStudev, CStudev> &arry, int iStartNO, int iEndNO);

private:
	CString       m_mac;			//教师机蓝牙地址
	CArray<CStudev, CStudev>     m_stud;     //学生机动态数组;
	CYDAppCom					m_com;
	BOOL		m_is_first_seacher;
	BOOL		m_is_first_sendmac;
	BOOL		update_stat(BOOL &bRoll, BOOL &bRolling);
}; 
