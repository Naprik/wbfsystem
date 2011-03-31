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
	GFROM_TEACHER     =   0 ,//��ǰ����ͨ�Ľ�ʦ������
	GFROM_INFRARED	 = 1	//��ǰ�Ǻ��⿼��
};

class YDCOM_DLL_API CInfraredStruct		//���⿼��ʱ���õ��Ľṹ��
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

class YDCOM_DLL_API CGExamStruct //�����洢ExamCom�ȵ�ȫ�ֱ���
{
public:
	CGExamStruct();
	~CGExamStruct();
	CExamDetail*					m_pExamDetail;
	//CExamApp*						m_pExamApp;
//	std::list<CStudev*>				m_lstStudev;
	std::list<CYDTeacherAppCom*>	m_lstTacherAppCom; //�����ҵ��Ľ�ʦ��COM�˿�
	BOOL							m_bExtractOnlyFromTeacher;//True��ֻ�ӽ�ʦ������ȡ��Ϣ
															  //False:����п��Բ���
	BOOL							m_bThreadJump;			  //����ʦ���Ƿ�����USB���߳��Ƿ��˳��ı��
	CWnd*							m_pMainWnd;				  //�߳��в��ܵõ������ڣ��������ڳ�ʼ���������������
	CONFIG_TEACHER_TYPE				m_TeacherType;
	CInfraredStruct					m_InfraredStruct;
public:
	CCriticalSection m_cs;			//�ٽ����ƣ����ڶ��߳��п���
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
