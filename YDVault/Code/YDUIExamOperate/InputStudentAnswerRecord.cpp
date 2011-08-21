#include "StdAfx.h"
#include "InputStudentAnswerRecord.h"
#include "../Base\DataHandler.h"
#include "../ObjRef\PropQueryContidition.h"
#include "../Base\AutoClean.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../ObjRef\YDObjectRef.h"
#include "../YDExamObjRef\YDStuMark.h"

CInputStudentAnswerRecord::CInputStudentAnswerRecord(void)
{
	m_strStuID = _T("");
	m_ExamTime = COleDateTime::GetCurrentTime();
	m_strSubjectID = _T("");
	m_strExamPaperID = _T("");
	m_strStuAnswer = _T("");

	m_pStudent = NULL;
	m_pSubject = NULL;
	m_pPaper   = NULL;
	m_pLinkSubjectPaper = NULL;
}


CInputStudentAnswerRecord::~CInputStudentAnswerRecord(void)
{
	ClearObj();
}

HRESULT CInputStudentAnswerRecord::CreateObj(CDatabaseEx* pDB)
{
	HRESULT hr = E_FAIL;
	hr = ClearObj();
	if(FAILED(hr))
	{
		return hr;
	}
	{
		//判断学生学号是否存在
		CDataHandler::TrimString(m_strSubjectID);
		std::list<CPropQueryContidition*> lstQueryCon;
		CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
		CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
		pPropQueryContidition->m_uFieldType = VT_BSTR;
		pPropQueryContidition->m_uOpType = Q_EQUAL;
		pPropQueryContidition->m_strFiledName  = FIELD_YDSTU_ID;
		pPropQueryContidition->m_strConVal = m_strStuID;
		lstQueryCon.push_back(pPropQueryContidition);
		std::list<CYDObjectRef*> lstStu;
		hr = CStaticObjHelper::GetObjRef(DB_YDSTUINFO,pDB,&lstStu,&lstQueryCon);
		if(FAILED(hr))
		{
			return hr;
		}
		if(lstStu.size() == 1)
		{
			m_pStudent = *lstStu.begin();
		}
		else
		{
			CListAutoClean<CYDObjectRef> clr1(lstStu);
			return S_FALSE;
		}
	}

	{
		//判断科目编号是否存在
		CDataHandler::TrimString(m_strSubjectID);
		std::list<CPropQueryContidition*> lstQueryCon;
		CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
		CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
		pPropQueryContidition->m_uFieldType = VT_BSTR;
		pPropQueryContidition->m_uOpType = Q_EQUAL;
		pPropQueryContidition->m_strFiledName  = FIELD_YDSUBJECT_ID;
		pPropQueryContidition->m_strConVal = m_strSubjectID;
		lstQueryCon.push_back(pPropQueryContidition);
		std::list<CYDObjectRef*> lstSubject;
		hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT,pDB,&lstSubject,&lstQueryCon);
		if(FAILED(hr))
		{
			return hr;
		}
		if(lstSubject.size() == 1)
		{
			m_pSubject = *lstSubject.begin();
		}
		else
		{
			CListAutoClean<CYDObjectRef> clr1(lstSubject);
			return S_FALSE;
		}
	}

	{
		//判断试卷编号是否存在
		CDataHandler::TrimString(m_strExamPaperID);
		std::list<CPropQueryContidition*> lstQueryCon;
		CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
		CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
		pPropQueryContidition->m_uFieldType = VT_BSTR;
		pPropQueryContidition->m_uOpType = Q_EQUAL;
		pPropQueryContidition->m_strFiledName  = FIELD_YDPAPER_CODE;
		pPropQueryContidition->m_strConVal = m_strExamPaperID;
		lstQueryCon.push_back(pPropQueryContidition);
		std::list<CYDObjectRef*> lstPaper;
		hr = CStaticObjHelper::GetObjRef(DB_PAPER,pDB,&lstPaper,&lstQueryCon);
		if(FAILED(hr))
		{
			return hr;
		}
		if(lstPaper.size() == 1)
		{
			m_pPaper = *lstPaper.begin();
		}
		else
		{
			CListAutoClean<CYDObjectRef> clr1(lstPaper);
			return S_FALSE;
		}
	}

	{
		//判断科目下是否有该试卷
		CYDSubjectRef* pSubjectRef = (CYDSubjectRef*)m_pSubject;
		ASSERT(pSubjectRef);
		std::list<CYDObjectRef*> lstPaper;
		std::list<CYDLinkRef*> lstLink;
		CListAutoClean<CYDObjectRef> clrPaper(lstPaper);
		CListAutoClean<CYDLinkRef> clrLink(lstLink);
		hr = pSubjectRef->GetPapers(&lstPaper,&lstLink);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstPaper.size() == lstLink.size());
		std::list<CYDObjectRef*>::iterator itrPaper = lstPaper.begin();
		std::list<CYDLinkRef*>::iterator itrLink = lstLink.begin();
		for(;itrPaper != lstPaper.end()&& itrLink != lstLink.end();++itrPaper,++itrLink)
		{
			VARIANT_BOOL bSame = VARIANT_FALSE;
			if(m_pPaper->IsEqual(*itrPaper))
			{
				{
					CPtrAutoClean<CYDObjectRef> clr(m_pPaper);
				}
				m_pPaper = *itrPaper;
				m_pLinkSubjectPaper = *itrLink;
				lstPaper.erase(itrPaper);
				lstLink.erase(itrLink);
				break;
			}

		}
	}
	return S_OK;
}

HRESULT CInputStudentAnswerRecord::ClearObj()
{
	HRESULT hr = E_FAIL;
	CPtrAutoClean<CYDObjectRef> clrStu(m_pStudent);
	CPtrAutoClean<CYDObjectRef> clrSub(m_pSubject);
	CPtrAutoClean<CYDObjectRef> clrPaper(m_pPaper);
	CPtrAutoClean<CYDLinkRef> clrLink(m_pLinkSubjectPaper);
	return S_OK;
}

HRESULT CInputStudentAnswerRecord::InsertToDB(CDatabaseEx* pDB)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pStudent);
	ASSERT(m_pSubject);
	ASSERT(m_pPaper);
	CYDStuMark* pStuMak = new CYDStuMark(pDB);
	CPtrAutoClean<CYDStuMark> clr(pStuMak);
	{
		//学生信息表OBJID
		OBJID stuID = 0;
		hr = m_pStudent->GetID(&stuID);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valStuID((long)stuID);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_STUID,&valStuID);
		if(FAILED(hr))
		{
			return hr;
		}
	} 
	/*{
		//设备objid
		CYDDevice* pDev = NULL;
		hr = _pUint->GetDevice(pDev);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDDevice> clr(pDev);
		ASSERT(pDev);
		OBJID idDev = 0;
		hr = pDev->GetID(&idDev);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valDevID((long)idDev);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_DEVICEID,&valDevID);
		if(FAILED(hr))
		{
			return hr;
		}
	}*/
	/*{
		//准考证号
		CComVariant valExamID;
		hr = _pUint->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,&valExamID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_EXAMID,&valExamID);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	*/
	/*{
		//考试信息ID
		OBJID idExamInfo = 0;
		hr = m_pExamInfo->GetID(&idExamInfo);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valIDExamInfo((long)idExamInfo);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_EXAMINFOID,&valIDExamInfo);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	*/
	{
		//试卷ID
		OBJID idPaper = 0;
		hr = m_pPaper->GetID(&idPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valIDPaper((long)idPaper);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_PAPERID,&valIDPaper);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//科目ID
		OBJID idSubject = 0;
		hr = m_pSubject->GetID(&idSubject);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valIDSubject((long)idSubject);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_SUBJECTID,&valIDSubject);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//学生答案
		CComVariant valAnswer(m_strStuAnswer);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_ANSWER,&valAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//分数
		double dbMark = 0;
		hr = pStuMak->CalMark(&dbMark);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valMark(dbMark);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_MARK,&valMark);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//考试时间
		CComVariant valTime;
		CDataHandler::OleDateTimeToVariant(m_ExamTime,&valTime);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_EXAMDATE,&valTime);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	hr = pStuMak->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}