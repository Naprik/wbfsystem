// ExamDetail.h: interface for the CExamDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
#define AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StuInfo.h"

class CExamDetail  
{
public:
	CExamDetail();
	virtual ~CExamDetail();
public:
	int						nRoll_pattern;		//������ʽ��1��������2�����⣬3��USB
	int						nRolling_pattern;	//�վ���ʽ��1��������2�����⣬3��USB
	int                     nExam_quality;      //�������ʣ�1�����濼�� 2��һ���Բ���
	CStringArray			arryClass;			//�༶����
	CString					sAddress;			//���Եص�
	CString					sDepart;			//���Ե�Ժϵ
	int						nPaperid;           //�Ծ��ţ�
	CString					sPapername;			//�Ծ�����
	CString					sSubject;			//���Կ�Ŀ
	CString					sStart;				//����ʱ��
	CString					sEnd;				//�վ�ʱ��
	int						nMaxNO;				//������
	int						nStartNO;			//��ʼ���
	int						nEndNO;				//�������
	BOOL					b_listen;			//�Ƿ�������
	BOOL					b_part1, b_part2, b_part3;	//1��2��3������ʹ�����
	COleDateTime			m_start1;
	COleDateTime			m_end1;
	COleDateTime			m_start2;
	COleDateTime			m_end2;
	COleDateTime			m_start3;
	COleDateTime			m_end3;
	CString                 m_chanal;			//�������Ե�Ƶ��
	int						nSetp;				//���Խ��е��Ĳ���
	BOOL					bIsRollDone;		//�����Ƿ����
	BOOL					bIsRollingDone;		//�վ��Ƿ����
	CArray<CStuInfo, CStuInfo>  m_arryStuIfo;


	void Initdata();
	BOOL IsEmpty();
};

#endif // !defined(AFX_EXAMDETAIL_H__5F47CCE5_348B_4645_8F36_775E5CE8DA14__INCLUDED_)
