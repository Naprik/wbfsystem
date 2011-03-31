/************************************************************
������CFileGraph
˵������¼Ҫͨ��OCX��ʾͳ��ͼ�ĸ�������
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
	void          Clear();                                            //ɾ�����ݣ���ʼ��
	BOOL          Save(CString file="");                             //��������
	BOOL          Load(CString file="");                             //װ������
	int           GetMaxMem();                                       //�õ����ı�ע��е�X�����ж���ͳ��ָ�꣩
	BOOL          SetMem(CStringArray &mem);                         //���ñ�ע��Ŀ��ע���ͬһ��X��Ӧ�����ͳ����˳����ͬ��
	void          SetStep(DWORD min,DWORD max,DWORD step);           //����Y����С����������Ͳ���
	void          SetText(CString title,CString xText,CString yText);//���ñ��⣬X��Y������
	void          SetType(int type);                                 //����ͼ�����[0,10]
	void          SetLine(BOOL flag);                                //�Ƿ���ʾ����
	void          AddData(CString xText,CUIntArray &yList);          //����X��Y���� һ�Զ�
	void          AddData(CString xText, DWORD y);                   //����X��Y���� һ��һ
public:
	int           m_type;                                            //ͼ�����
	CString       m_textTitle;                                       //��������
	CString       m_textX;                                           //X������ 
	CString       m_textY;                                           //Y������	
	CStringArray  m_legList;                                         //ע�������б�
public:
	DWORD         m_pos[3];                                          //��С������������Ͳ���
	CStringArray  m_xList;                                           //X�������б�
	CDWordArray   m_yList[MAX_SERIES_ACCOUNT];                       //Y�������б�

	BOOL          m_isLine;
};
