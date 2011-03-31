#include "StdAfx.h"
#include "YDTeacherDevRef.h"

CYDTeacherDevRef::CYDTeacherDevRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDTEACHERDEV;
	AddPropDef(FIELD_YDTEACHERDEV_TEACHID,VT_BSTR);	//��ʦ���
	AddPropDef(FIELD_YDTEACHERDEV_NAME,VT_BSTR);	//��ʦ����
	AddPropDef(FIELD_YDTEACHERDEV_DEVID,VT_BSTR);	//��ʦ���豸��
}

CYDTeacherDevRef::~CYDTeacherDevRef(void)
{
}
