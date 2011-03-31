#include "stdafx.h"
#include "RQDb.h"
#include "RQWhere.h" 
#define      RQ_DB_MAX_COLS   50              //表中最多的字节

CRQDb::CRQDb()
{	
     m_connect = NULL;
	 m_colMap = new CMapStringToPtr(RQ_DB_MAX_COLS);
	 m_colMap->RemoveAll();	
}
CRQDb::~CRQDb()
{
	m_colMap->RemoveAll();
	delete m_colMap;
}
BOOL CRQDb::Load(CString sql)
{   
	SetFilter("");
	if ( !CRQDb::SetFilter(sql) )
		return FALSE;
	if ( IsEOF())
		return FALSE;
	if ( MoveFirst())
		return UpdateRecord(TRUE) ;
	return FALSE;
}

BOOL CRQDb::Load(DWORD id)
{
	CRQWhere w("YDID",CRQWhere::GetLong(id));
	return Load(w.GetClausePlain());
}
BOOL CRQDb::Commit()
{
	BOOL ret = m_pConnection->BeginTrans();
    if ( ret ) 
		ret = CAdoRecordSet::Update();
    if ( ret )
		ret = m_pConnection->CommitTrans();
	else
		m_pConnection->RollbackTrans();
   	return ret;
}
BOOL CRQDb::UpdateColData(BOOL bLoad, CString colName, void *data)
{
    int type = CRQDb::GetFieldType(colName);
	BOOL ret = FALSE;
	switch( type )
	{	
	case adSingle:
		if ( bLoad )
			ret = GetCollect(colName, (*(double *)data));
		else
			ret = PutCollect(colName, (*(double *)data));
		break;
	case adBigInt:
		if ( bLoad )
			ret = GetCollect(colName, (*(ULONG *)data));
		else
			ret = PutCollect(colName, (*(ULONG *)data));
		break;
	case adUnsignedTinyInt:	
		if ( bLoad )
			ret = GetCollect(colName, (*(BYTE *)data));
		else
			ret = PutCollect(colName, (*(BYTE *)data));
		break;
	case adInteger:	
		if ( bLoad )
			ret = GetCollect(colName, (*(DWORD *)data));
		else
			ret = PutCollect(colName, (*(DWORD *)data));
		break;
    case adWChar:
	case adChar:    
	case adVarWChar:
	case adLongVarWChar:
		if ( bLoad )
		{
			CString tmp="";
			ret = GetCollect(colName, tmp);
			tmp.TrimLeft();
            tmp.TrimRight();
			(*(CString *)data).ReleaseBuffer();
			(*(CString *)data).FreeExtra();
			(*(CString *)data) = tmp;
			tmp.ReleaseBuffer();
            tmp.FreeExtra();			
		}
		else
		{	
			CString tmp="";
			tmp = (*(CString*)data);
			tmp.TrimLeft();
            tmp.TrimRight();
			ret = PutCollect(colName, tmp);	
			tmp.FreeExtra();
			tmp.ReleaseBuffer();
			(*(CString *)data).ReleaseBuffer();
			(*(CString *)data).FreeExtra();
		}
		break;
	case adDate:
	case adDBTimeStamp:
		if ( bLoad )
			ret = GetCollect(colName, (*(COleDateTime *)data));
		else
			ret = PutCollect(colName, (*(COleDateTime *)data));
		break;
	case adDouble:
		if ( bLoad )
			ret = GetCollect(colName, (*(double *)data));
		else
			ret = PutCollect(colName, (*(double *)data));
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
BOOL CRQDb::UpdateRecord(BOOL bLoad)
{
	BOOL ret = TRUE;
	POSITION pos = m_colMap->GetStartPosition();
	while(pos)	
	{
		CString key;
		void *val;
		m_colMap->GetNextAssoc(pos,key,val);
        if ( !UpdateColData(bLoad,key.GetBuffer(0),val))
		{
			ret = FALSE;
			break;
		}
	}
	return ret;
}
BOOL CRQDb::Add()
{
	BOOL ret = CAdoRecordSet::AddNew();
    if ( ret )
		ret = UpdateRecord(FALSE );
    if ( ret )
		ret = Commit();
	if ( ret )
		ret = UpdateRecord(TRUE) ;
	return ret;
}
BOOL CRQDb::Delete(DWORD id)
{
	SetFilter("");
	CRQWhere w("YDID",CRQWhere::GetLong(id));
	SetFilter(w.GetClausePlain());
	if ( CAdoRecordSet::Delete() )
		return Commit();
	return FALSE;
}
BOOL CRQDb::SetQuery(CString sql)
{
	SetFilter("");
	if ( !sql.IsEmpty() )
	{
		if ( !CRQDb::SetFilter(sql) )
			return FALSE;
	}
	return FALSE;
}
BOOL CRQDb::Update(DWORD id)
{
	SetFilter("");
	CRQWhere w("YDID",CRQWhere::GetLong(id));
	SetFilter(w.GetClausePlain());
	BOOL ret = UpdateRecord(FALSE );
	if ( ret )
		ret = Commit() ;
	if ( ret ) 
		ret =  UpdateRecord(TRUE) ;
	return ret;
}
BOOL CRQDb::Move(BOOL isNext)
{
	if ( IsEOF() )
		return FALSE;
	BOOL ret = FALSE;
	if ( isNext )   
		ret = MoveNext();
	else
		ret = MovePrevious();
	if ( ret )
		 ret = UpdateRecord( TRUE );
	return ret;
}

BOOL CRQDb::Open(CString table,CAdoConnection *connect)
{
	m_table = table;
	m_connect = connect;
	SetAdoConnection(connect);
	SetCursorLocation(adUseClient);
	return CAdoRecordSet::Open(table, adCmdTable);
}

void CRQDb::Close()
{
//	SetFilter("");
	if (GetRecordCount() >0)
		MoveFirst();
    CAdoRecordSet::Close();
	if ( m_connect )
		m_connect->Close();
}

BOOL CRQDb::RunSql(CString sql)
{
	BOOL ret = m_connect->Execute(sql);
    if ( ret )
		ret = Commit();
	return ret;
}
