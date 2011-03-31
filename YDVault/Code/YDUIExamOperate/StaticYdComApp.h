#pragma once
#include "../YdCom/ExamDetail.h"
#include "../YdCom/ExamApp.h"
#include "../YdCom/Studev.h"
#include "../YdCom/TeacherDev.h"

class BASE_DLL_API CStaticYdComApp
{
public:
	CStaticYdComApp(void);
	~CStaticYdComApp(void);
	static CStaticYdComApp* Instance(){return &m_YdComApp;}
private:
	static CStaticYdComApp m_YdComApp;
public:
	CExamDetail  m_ExamDetail;
	CExamApp m_ExamApp;
	CArray<CStudev, CStudev>  m_arrStudev;
	CArray<CTeacherDev, CTeacherDev>  m_arrTeacherdev;
};
