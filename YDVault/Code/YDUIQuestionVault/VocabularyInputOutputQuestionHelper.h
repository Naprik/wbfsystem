#pragma once
#include "inputOutputquestionhelper.h"
#include <list>

//导入选择题

class CVocabularyQuestion //选择题的结构
{
public:
	CString m_strCaption;//题目
	//选项列表：A--->Cat B-->dog...
	std::list<std::pair<CString,CString> >  m_lstOption;//选项
	CString m_strAnswer;
	//指标列表：指标名-->指标值
	std::list<std::pair<CString,CString> >  m_lstFactor;//指标
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
	//判断_strText是选择题的标题，如果是，则_strCaption返回标题
	HRESULT IsNewQuestionCaption(CString _strText,BOOL &_IsCaption,CString &_strCaption);
	//是否是选择题的选项，如果是,则_lstOption返回选项的列表，因为一行可能有2个选项，所以用列表
	HRESULT IsQuestionOption(CString _strText,BOOL &_IsOption,std::list<std::pair<CString,CString> > &_lstOption);
	//判断_strText是选择题的答案，如果是，则_strAnswer返回标题
	HRESULT IsQuestionAnswer(CString _strText,BOOL &_IsAnswer,CString &_strAnswer);
	//是否是选择题的指标，如果是,则在_lstFactor中插入一个指标
	HRESULT IsQuestionFactor(CString _strText,BOOL &_IsOption,std::list<std::pair<CString,CString> > &_lstFactor);
	HRESULT ShowPrevDlg();
};

