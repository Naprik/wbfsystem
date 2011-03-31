#include "StdAfx.h"
#include "YDExamInfo.h"

CYDExamInfo::CYDExamInfo(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDEXAM_INFO;
	AddPropDef(FIELD_YEEXAMINFO_PAPERID,VT_I4); //试卷OBJID
	AddPropDef(FIELD_YEEXAMINFO_STARTTIME,VT_DATE);//开始时间
	AddPropDef(FIELD_YEEXAMINFO_ENDTIME,VT_DATE);  //结束时间
	AddPropDef(FIELD_YEEXAMINFO_TEACHER,VT_BSTR);  //监考老师
	AddPropDef(FIELD_YEEXAMINFO_EXAMXML,VT_BSTR);//考试信息文件名
}

CYDExamInfo::~CYDExamInfo(void)
{
}
