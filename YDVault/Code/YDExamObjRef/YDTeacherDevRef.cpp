#include "StdAfx.h"
#include "YDTeacherDevRef.h"

CYDTeacherDevRef::CYDTeacherDevRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDTEACHERDEV;
	AddPropDef(FIELD_YDTEACHERDEV_TEACHID,VT_BSTR);	//教师编号
	AddPropDef(FIELD_YDTEACHERDEV_NAME,VT_BSTR);	//教师姓名
	AddPropDef(FIELD_YDTEACHERDEV_DEVID,VT_BSTR);	//教师机设备号
}

CYDTeacherDevRef::~CYDTeacherDevRef(void)
{
}
