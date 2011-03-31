#include "StdAfx.h"
#include "ThreadTeachComParam.h"

CThreadTeachComParam::CThreadTeachComParam(void)
{
	m_pAppCom = NULL;
	m_pForm = NULL;
	m_iStart = -1;
	m_iEnd = -1;
	m_pYDProgressControl = NULL;
}

CThreadTeachComParam::~CThreadTeachComParam(void)
{
	m_pAppCom = NULL;
	m_pForm = NULL;
}
