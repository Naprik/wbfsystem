#include "stdafx.h"
#include "DbST.h"
//#include "RQWhere.h" 
/************************************************************************
以下四个方法需要根据具体的应用进行修改
************************************************************************/
//CDbST 控制类
CDbST::CDbST()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("DMID",&m_id);	
	m_colMap->SetAt("PKName",&m_name);
	m_colMap->SetAt("PKKind",&m_kind);	
	m_colMap->SetAt("PKMark",&m_mark);

}
CDbST::~CDbST()
{
	Close();
}
BOOL CDbST::LoadByUser(CString user)
{	
	CRQWhere w("name", user);
	return Load(w.GetClausePlain());
}
BOOL CDbST::Connect()
{
	BOOL ret;
	CString dbPath= CJahCommand::GetExePath()+"database\\skychaogu.mdb";
	ret =  m_connect.ConnectAccess(dbPath,"1978222");
	if ( ret  )
		ret = CRQDb::Open("chaogu", &m_connect);	
	return ret;  
}

void CDbST::Close()
{
	CRQDb::Close();
}
BOOL CDbST::LoadByAll(int kind)
{
	CRQWhere w;
	if ( kind != 0 )
		w1.Add("PKKind",CRQWhere::GetLong(kind));
	tmp =  w.GetClausePlain();
	return Load(tmp);
}

BOOL CDbST::ChangeKind(CString domain, int kind)
{
	BOOL ret = FALSE;
	ret = LoadByUser(domain);
	if ( ret )
	{

	}
	return ret;
}

BOOL CDbST::Delete(CString domain)
{
	BOOL ret = FALSE;
	ret = LoadByUser(domain);
	if ( ret )
	{	
		DWORD id = m_id;
		m_id = -1;
		ret = CRQDb::Delete( id );
	}
	return ret;
}

BOOL CDbST::LoadToCheck()
{
	CRQWhere w;
	w.Add("DmKind",CRQWhere::GetLong(CConst::WEB_KIND_NEW));
	return Load(w.GetClausePlain());
}

int CDbST::GetAccount()
{
    return GetRecordCount();
}

BOOL CDbST::Move(BOOL isNext)
{
    return CRQDb::Move(isNext);
}

BOOL CDbST::Update(DWORD id)
{
	m_id = -1;
	return CRQDb::Update(id );
}

BOOL CDbST::Add()
{
	m_id = -1;
	return CRQDb::Add( );
}
