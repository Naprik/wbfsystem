#pragma once
#include "Stuanswer.h"

class CStudev  
{
public:
	CStudev();
	virtual ~CStudev();

	int					m_index;					//编号
	char				m_mac[20];					//学生机地址
	char				m_stuid[20];				//学生学号
	int					m_rollcall;					//是否点名
	int					m_rolling;                  //是否交卷
	BOOL				m_islate;					//是否迟到
	//CArray<CStuanswer, CStuanswer>  m_answer;
	char				m_answer[500];				//学生答案
	COleDateTime		m_start;					//开始考试时间
	COleDateTime		m_end;						//结束考试时间
	BOOL				m_islinkroll;
	BOOL				m_islinkroling;

public:
	CString  Get_Roll_text();
	CString  Get_Rolling_text();
	//BOOL     Setanswer(CString strAnswer, int start, int end);
};

