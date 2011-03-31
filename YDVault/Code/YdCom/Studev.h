#pragma once
#include <afxmt.h>

class CXmlNode;
class CXmlWriter;

//学生机设备
class AFX_EXT_CLASS CStudev  
{
public:
	CStudev();
	virtual ~CStudev();
private:
	CString				m_com;
	int					m_index;					//编号
	CString				m_mac;					   //学生机地址
	CString				m_stuid;				   //学生学号
	int					m_rollcall;					//是否点名
	int					m_rolling;                  //是否交卷
	BOOL				m_bFetchAnswer;             //是否已经提取答案
	BOOL				m_islate;					//是否迟到
	CString				m_answer;				    //学生答案
	COleDateTime		m_start;					//开始考试时间
	COleDateTime		m_end;						//结束考试时间
	BOOL				m_islinkroll;               //是不是联机点名  就是通过USB点名
	BOOL				m_islinkroling;             //是不是联机收卷，就是通过USB收卷
	BOOL				m_bWriteDB;					//是否已经写入数据库
public:
	HRESULT		LoadXml(CXmlNode& _node);
	HRESULT     SaveXml(CXmlWriter& _writer);
public:
	HRESULT		SetCom(CString _strCom);
	HRESULT		GetCom(CString& _strCom);
	HRESULT		SetIndex(int _index);
	HRESULT		GetIndex(int& _index);
	HRESULT		SetMac(CString _strMac);
	HRESULT		GetMac(CString& _strMac);
	HRESULT		SetStuID(CString _strStuID);
	HRESULT		GetSudID(CString& _strStuID);
	HRESULT		SetRollCall(int _iRollCall);
	HRESULT		GetRollCall(int &_iRollCall);
	HRESULT		SetRolling(int _iRolling);
	HRESULT		GetRolling(int &_iRolling);
	HRESULT		SetFetchAnswer(BOOL _bFetchAnswer);
	HRESULT		GetFetchAnswer(BOOL& _bFetchAnswer);
	HRESULT     SetIsLate(BOOL _bIsLate);
	HRESULT		GetIsLate(BOOL &_bIsLate);
	HRESULT		SetAnswer(CString _strAnswer);
	HRESULT     GetAnswer(CString& _strAnswer);
	HRESULT		SetStart(COleDateTime _tStart);
	HRESULT		GetStart(COleDateTime& _tStart);
	HRESULT		SetEnd(COleDateTime _tEnd);
	HRESULT		GetEnd(COleDateTime &_tEnd);
	HRESULT		SetIsLinkRoll(BOOL _bIsLinkRoll);
	HRESULT		GetIsLinkRoll(BOOL& _bIsLinkRoll);
	HRESULT		SetIsLinkRolling(BOOL _bIsLinkRolling);
	HRESULT		GetIsLinkRolling(BOOL& _bIsLinkRolling);
	HRESULT		SetIsWriteDB(BOOL _bWriteDB);
	HRESULT		GetIsWriteDB(BOOL &_bWriteDB);
public:
	CString  Get_Roll_text(); //获取当前点名状态
	CString  Get_Rolling_text(); //获取当前收卷状态
	//BOOL     Setanswer(CString strAnswer, int start, int end);
private:
	CCriticalSection m_cs; //临界点控制，用于多线程中控制
};

