#pragma once
#include "../YdCom/ExamDetail.h"
#include "../YdCom/ExamApp.h"
#include "../YdCom/Studev.h"
#include <map>

class CYDEAddrUnit;
class CYDStuMark;


#ifdef YDCOM_EXPORTS   
#define YDCOM_DLL_API __declspec(dllexport)   
#else   
#define YDCOM_DLL_API __declspec(dllimport)   
#endif   

enum CONFIG_TEACHER_TYPE
{
	GFROM_TEACHER     =   0 ,//当前是普通的教师机考试
	GFROM_INFRARED	 = 1	//当前是红外考试
};

class YDCOM_DLL_API CInfraredStruct		//红外考试时，用到的结构体
{
public:
	CInfraredStruct()
	{
		m_idPaper = 0;
		m_idSubject = 0;
	}
	OBJID	m_idPaper;
	OBJID   m_idSubject;
	std::map<CYDEAddrUnit*, CYDStuMark*> m_mapUnitToMark;;
};

class YDCOM_DLL_API CGExamStruct //用来存储ExamCom等的全局变量
{
public:
	CGExamStruct();
	~CGExamStruct();
	CExamDetail*					m_pExamDetail;
	//CExamApp*						m_pExamApp;
//	std::list<CStudev*>				m_lstStudev;
	std::list<CYDTeacherAppCom*>	m_lstTacherAppCom; //所有找到的教师机COM端口
	BOOL							m_bExtractOnlyFromTeacher;//True：只从教师机中提取信息
															  //False:会进行考试操作
	BOOL							m_bThreadJump;			  //检测教师机是否连接USB的线程是否退出的标记
	CWnd*							m_pMainWnd;				  //线程中不能得到主窗口，在主窗口初始化将这个变量设置
	CONFIG_TEACHER_TYPE				m_TeacherType;
	CInfraredStruct					m_InfraredStruct;
public:
	CCriticalSection m_cs;			//临界点控制，用于多线程中控制
};

class YDCOM_DLL_API CStaticYdComApp
{
public:
	CStaticYdComApp(void);
	~CStaticYdComApp(void);
public:
	static CStaticYdComApp* Instance(){return &m_staticYdComApp;}
	HRESULT      GetGExamStruct(CGExamStruct* &_pGExamStruct);
	HRESULT		 ClearStudev();
private:
	static CStaticYdComApp m_staticYdComApp;
private:
	CGExamStruct		  m_GExamStruct;
};
