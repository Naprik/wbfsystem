// ExamDetail.h: interface for the CExamDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
#define AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StuInfo.h"

class CExamDetail  
{
public:
	CExamDetail();
	virtual ~CExamDetail();
public:
	int						nRoll_pattern;		//点名样式：1、蓝牙，2、红外，3、USB
	int						nRolling_pattern;	//收卷样式：1、蓝牙，2、红外，3、USB
	int                     nExam_quality;      //考试性质：1、正规考试 2、一般性测试
	CStringArray			arryClass;			//班级数组
	CString					sAddress;			//考试地点
	CString					sDepart;			//考试的院系
	int						nPaperid;           //试卷编号；
	CString					sPapername;			//试卷名称
	CString					sSubject;			//考试科目
	CString					sStart;				//开考时间
	CString					sEnd;				//收卷时间
	int						nMaxNO;				//最大题号
	int						nStartNO;			//开始题号
	int						nEndNO;				//结束题号
	BOOL					b_listen;			//是否有听力
	BOOL					b_part1, b_part2, b_part3;	//1、2、3段听力使用情况
	COleDateTime			m_start1;
	COleDateTime			m_end1;
	COleDateTime			m_start2;
	COleDateTime			m_end2;
	COleDateTime			m_start3;
	COleDateTime			m_end3;
	CString                 m_chanal;			//听力考试的频段
	int						nSetp;				//考试进行到的步骤
	BOOL					bIsRollDone;		//点名是否结束
	BOOL					bIsRollingDone;		//收卷是否结束
	CArray<CStuInfo, CStuInfo>  m_arryStuIfo;


	void Initdata();
	BOOL IsEmpty();
};

#endif // !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
