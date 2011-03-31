#include "StdAfx.h"
#include "YDExamInfo.h"

CYDExamInfo::CYDExamInfo(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDEXAM_INFO;
	AddPropDef(FIELD_YEEXAMINFO_PAPERID,VT_I4); //�Ծ�OBJID
	AddPropDef(FIELD_YEEXAMINFO_STARTTIME,VT_DATE);//��ʼʱ��
	AddPropDef(FIELD_YEEXAMINFO_ENDTIME,VT_DATE);  //����ʱ��
	AddPropDef(FIELD_YEEXAMINFO_TEACHER,VT_BSTR);  //�࿼��ʦ
	AddPropDef(FIELD_YEEXAMINFO_EXAMXML,VT_BSTR);//������Ϣ�ļ���
}

CYDExamInfo::~CYDExamInfo(void)
{
}
