// ExamDetail.h: interface for the CExamDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
#define AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include\YDExamComm.h"
#include <list>

class CYDObjectRef;
class CExamQuestionSegment;

class AFX_EXT_CLASS CExamDetail  //全局变量
{
public:
	CExamDetail();
	virtual ~CExamDetail();
public:
	YDEXAM_ROLLCALL_TYPE				m_uRollCallType;	//点名样式：1、蓝牙，2、红外，3、USB
	YDEXAM_ROLLING_TYPE 				m_uRollingType;	    //收卷样式：1、蓝牙，2、红外，3、USB
	YDEXAM_KIND							m_uExamKind;      //考试性质：1、正规考试 2、一般性测试
	std::list<CYDObjectRef*>			m_lstClass;			//班级数组
	CYDObjectRef*						m_pObjAddress;			//考试地点
	CYDObjectRef*						m_pDepart;			//考试的院系
	CYDObjectRef*						m_pPaper;           //试卷；
	CYDObjectRef*						m_pSubject;			//考试科目
	std::list<CYDObjectRef*>			m_lstUnit;       //考场的考试单元
	COleDateTime						m_timeStart;		//开考时间
	COleDateTime						m_timeEnd;			//收卷时间
	std::list<CExamQuestionSegment*>	m_lstQSeg;        //题目段数
	BOOL								m_bListen;			//是否有听力
	BOOL								m_bListenPart1, m_bListenPart2, m_bListenPart3;	//1、2、3段听力使用情况
	COleDateTime						m_timeStart1,m_timeEnd1,m_timeStart2,
										m_timeEnd2,m_timeStart3,m_timeEnd3; //听力的开始，结束时间
	CString								m_strChanal;			//听力考试的频段
	YDEAM_STEP							m_nSetp;				//考试进行到的步骤
	BOOL								m_bIsRollDone;		//点名是否结束
	BOOL								m_bIsRollingDone;		//收卷是否结束
	CString								m_strInvigilateTeacher;   //监考老师
	CString								m_strXmlFile;			//对应保存的XML文件名
public:
	HRESULT    InitClearData();
	HRESULT	   GetMinMaxNo(int &_iMin,int &_iMax);//得到当前的最小和最大题号
};

#endif // !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
