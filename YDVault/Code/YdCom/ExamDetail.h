// ExamDetail.h: interface for the CExamDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
#define AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include\YDExamComm.h"
#include <list>

class CYDObjectRef;
class CExamQuestionSegment;

class AFX_EXT_CLASS CExamDetail  //ȫ�ֱ���
{
public:
	CExamDetail();
	virtual ~CExamDetail();
public:
	YDEXAM_ROLLCALL_TYPE				m_uRollCallType;	//������ʽ��1��������2�����⣬3��USB
	YDEXAM_ROLLING_TYPE 				m_uRollingType;	    //�վ���ʽ��1��������2�����⣬3��USB
	YDEXAM_KIND							m_uExamKind;      //�������ʣ�1�����濼�� 2��һ���Բ���
	std::list<CYDObjectRef*>			m_lstClass;			//�༶����
	CYDObjectRef*						m_pObjAddress;			//���Եص�
	CYDObjectRef*						m_pDepart;			//���Ե�Ժϵ
	CYDObjectRef*						m_pPaper;           //�Ծ�
	CYDObjectRef*						m_pSubject;			//���Կ�Ŀ
	std::list<CYDObjectRef*>			m_lstUnit;       //�����Ŀ��Ե�Ԫ
	COleDateTime						m_timeStart;		//����ʱ��
	COleDateTime						m_timeEnd;			//�վ�ʱ��
	std::list<CExamQuestionSegment*>	m_lstQSeg;        //��Ŀ����
	BOOL								m_bListen;			//�Ƿ�������
	BOOL								m_bListenPart1, m_bListenPart2, m_bListenPart3;	//1��2��3������ʹ�����
	COleDateTime						m_timeStart1,m_timeEnd1,m_timeStart2,
										m_timeEnd2,m_timeStart3,m_timeEnd3; //�����Ŀ�ʼ������ʱ��
	CString								m_strChanal;			//�������Ե�Ƶ��
	YDEAM_STEP							m_nSetp;				//���Խ��е��Ĳ���
	BOOL								m_bIsRollDone;		//�����Ƿ����
	BOOL								m_bIsRollingDone;		//�վ��Ƿ����
	CString								m_strInvigilateTeacher;   //�࿼��ʦ
	CString								m_strXmlFile;			//��Ӧ�����XML�ļ���
public:
	HRESULT    InitClearData();
	HRESULT	   GetMinMaxNo(int &_iMin,int &_iMax);//�õ���ǰ����С��������
};

#endif // !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
