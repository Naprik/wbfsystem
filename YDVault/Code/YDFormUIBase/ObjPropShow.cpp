#include "StdAfx.h"
#include "ObjPropShow.h"

CObjPropShow::CObjPropShow(void)
{
	m_type  = OBJ_PROP_SHOW_TYPE(-1);
	m_pObjWrapper = NULL;
	m_strName = _T("");
	m_pSheet = NULL;
	m_nOp = OPERATION(-1);
}

CObjPropShow::~CObjPropShow(void)
{
}
