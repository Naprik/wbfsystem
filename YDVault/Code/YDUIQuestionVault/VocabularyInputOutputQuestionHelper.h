#pragma once
#include "inputOutputquestionhelper.h"
#include <list>

//����ѡ����

class CVocabularyQuestion //ѡ����Ľṹ
{
public:
	CString m_strCaption;//��Ŀ
	//ѡ���б�A--->Cat B-->dog...
	std::list<std::pair<CString,CString> >  m_lstOption;//ѡ��
	CString m_strAnswer;
	//ָ���б�ָ����-->ָ��ֵ
	std::list<std::pair<CString,CString> >  m_lstFactor;//ָ��
public:
	HRESULT Load(Paragraphs &_paragraphs,long _index);
};

class CVocabularyInputOutputQuestionHelper :
	public CInputOutputQuestionHelper
{
public:
	CVocabularyInputOutputQuestionHelper(CYDObjectRef* _pVault,CYDObjectRef* _pType);
	~CVocabularyInputOutputQuestionHelper(void);
public:
	virtual HRESULT ExeInputFile(CString _strFile);
	virtual HRESULT ExeOutputFile(CString _strFile,std::list<CYDObjectRef*> *_plstObj);
private:
	//�ж�_strText��ѡ����ı��⣬����ǣ���_strCaption���ر���
	HRESULT IsNewQuestionCaption(CString _strText,BOOL &_IsCaption,CString &_strCaption);
	//�Ƿ���ѡ�����ѡ������,��_lstOption����ѡ����б���Ϊһ�п�����2��ѡ��������б�
	HRESULT IsQuestionOption(CString _strText,BOOL &_IsOption,std::list<std::pair<CString,CString> > &_lstOption);
	//�ж�_strText��ѡ����Ĵ𰸣�����ǣ���_strAnswer���ر���
	HRESULT IsQuestionAnswer(CString _strText,BOOL &_IsAnswer,CString &_strAnswer);
	//�Ƿ���ѡ�����ָ�꣬�����,����_lstFactor�в���һ��ָ��
	HRESULT IsQuestionFactor(CString _strText,BOOL &_IsOption,std::list<std::pair<CString,CString> > &_lstFactor);
	HRESULT ShowPrevDlg();
};

