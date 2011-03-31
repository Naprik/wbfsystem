#include "stdafx.h"
#include "FileGraph.h"

CFileGraph::CFileGraph()
{
	Clear();
	SetType(3);
}
CFileGraph::~CFileGraph()
{
}
void CFileGraph::Clear()
{
	int account  = m_xList.GetSize();
    m_xList.RemoveAll();
	m_legList.RemoveAll();
	for( int i = 0 ;i < account ;i ++)
        m_yList[i].RemoveAll();
	m_textTitle = "";            
	m_textX     = "";                 
	m_textY     = ""; 
	m_type      = 0;	
	m_pos[0]    = 0;
	m_pos[1]    = 0;
	m_pos[2]    = 0;
	m_isLine    = FALSE;
}
void CFileGraph::SetType(int type)
{
    m_type = type;
}
void CFileGraph::SetText(CString title, CString xText, CString yText)
{
	m_textTitle = title;
	m_textX     = xText; 
	m_textY     = yText;
}
void CFileGraph::SetStep(DWORD min, DWORD max, DWORD step)
{   
	if ( min <=0)
        m_pos[0] = 1;
	else
		m_pos[0] = min;
	m_pos[0] = min;
    if ( max <=0)
        m_pos[1] = 1;
	else
		m_pos[1] = max;
	if ( step <= 0 )
		m_pos[2] = 1;
	else
		m_pos[2] = step;
}
BOOL CFileGraph::SetMem(CStringArray &mem)
{
    if ( mem.GetSize() != GetMaxMem() )
		return FALSE;
	for(int i = 0 ; i < mem.GetSize() ; i ++ )
		m_legList.Add(mem.GetAt(i));
    return TRUE;
}
int CFileGraph::GetMaxMem()
{
    int account = m_xList.GetSize();
	int maxLegend = 0;
	for ( int i = 0 ; i < m_xList.GetSize() ; i ++ )
	{
		if ( m_yList[i].GetSize() > maxLegend )
			maxLegend = m_yList[i].GetSize();
	}
	return maxLegend;
}
void CFileGraph::AddData(CString xText, CUIntArray &yList)
{
	m_xList.Add(xText);
	for(int i = 0 ;i<yList.GetSize();i++ )
	{
        m_yList[m_xList.GetSize()-1].Add(yList.GetAt(i));
	}
}
void CFileGraph::SetLine(BOOL flag)
{
	m_isLine = flag;
}
BOOL CFileGraph::Load(CString file)
{
    return TRUE;
}
BOOL CFileGraph::Save(CString file)
{

    return TRUE;
}

void CFileGraph::AddData(CString xText, DWORD y)
{
	CUIntArray list;
	list.RemoveAll(); 
	list.Add( y );
	AddData(xText,list);
}
