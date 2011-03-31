#include "StdAfx.h"
#include "YDResultRef.h"

CYDResultRef::CYDResultRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName =  DB_YDRESULT ;//学生答案信息表
	AddPropDef(FIELD_YDRESULT_STUDID,VT_I4);//学生信息表OBJID
	AddPropDef(FIELD_YDRESULT_PAPERID,VT_I4);//试卷编号
	AddPropDef(FIELD_YDRESULT_START,VT_DATE);//开考时间
	AddPropDef(FIELD_YDRESULT_END,VT_DATE);//交卷时间
	AddPropDef(FIELD_YDRESULT_ASNSWER,VT_BSTR); //学生答案
	AddPropDef(FIELD_YDRESULT_ADDRID,VT_BSTR); //考场编号

}

CYDResultRef::~CYDResultRef(void)
{
}
