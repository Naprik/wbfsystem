/************************************************************
类名：CFileGraph
说明：记录要通过OCX显示统计图的各项数据
************************************************************/
#pragma once
#include <afxtempl.h>
#define   FILE_GRAPH_FILE     "graphs\\graph.dat"
#define   MAX_SERIES_ACCOUNT  33
class CFileGraph  
{
public:
	CFileGraph();
	~CFileGraph();
public:	
	void          Clear();                                            //删除数据，初始化
	BOOL          Save(CString file="");                             //保存数据
	BOOL          Load(CString file="");                             //装载数据
	int           GetMaxMem();                                       //得到最大的备注项（有的X可能有多项统计指标）
	BOOL          SetMem(CStringArray &mem);                         //设置备注项目（注意和同一个X对应的最大统计项顺序相同）
	void          SetStep(DWORD min,DWORD max,DWORD step);           //设置Y的最小数，最大数和步长
	void          SetText(CString title,CString xText,CString yText);//设置标题，X，Y的描述
	void          SetType(int type);                                 //设置图形类别，[0,10]
	void          SetLine(BOOL flag);                                //是否显示线条
	void          AddData(CString xText,CUIntArray &yList);          //增加X和Y坐标 一对多
	void          AddData(CString xText, DWORD y);                   //增加X和Y坐标 一对一
public:
	int           m_type;                                            //图形类别
	CString       m_textTitle;                                       //标题文字
	CString       m_textX;                                           //X轴文字 
	CString       m_textY;                                           //Y轴文字	
	CStringArray  m_legList;                                         //注释文字列表
public:
	DWORD         m_pos[3];                                          //最小数，最大数，和步长
	CStringArray  m_xList;                                           //X轴数据列表
	CDWordArray   m_yList[MAX_SERIES_ACCOUNT];                       //Y轴数据列表

	BOOL          m_isLine;
};
