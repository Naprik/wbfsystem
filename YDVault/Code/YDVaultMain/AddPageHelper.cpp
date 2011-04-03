#include "StdAfx.h"  
#include "AddPageHelper.h"  
#include "../YDFormUIBase\ObjPropertySheet.h"
#include "../YDFormUIBase\ObjPropShow.h"
#include "../YDUIQuestionVault\QuestionVaultClassificationFormView.h"
#include "../YDUIQuestionVault\QuestionVaultBaseFormView.h"
#include "../\YDUIQuestionVault\QuestionVaultKnowledgeFormView.h"
#include "../YDUIQuestionVault\QuestionVaultQTypeFormView.h"
#include "../YDUIQuestionVault\GeneratePaperFormView.h"
#include "../YDUIQuestionVault\PaperFolderFormView.h"
#include "../\YDUIQuestionVault\PaperFormView.h"
#include "../YDUIQuestionVault\QuestionTypeFormView.h"
#include "../YDUIQuestionVault\QuestionListFormView.h"
#include "../YDUIQuestionVault\QuestionKnowledgeListFormView.h"
#include "../YDUIQuestionVault\AtricleQuestionFormView.h"
#include "../Include\YDExamComm.h"
#include "../YDUIExamOperate\FormFM.h"
#include "../YDUIExamOperate\FormExamMode.h"
#include "../YDUIExamDataMaintain\FormDepartment.h"
#include "../YDUIExamDataMaintain\FormStudent.h"
#include "../YDUIExamDataMaintain\FormSubject.h"
#include "../YDUIExamDataMaintain\FormTeacherDevice.h"
#include "../YDUIExamDataMaintain\FormPaper.h"
#include "../YDUIExamDataMaintain\FormExamAddress.h"
#include "../YDUIExamDataMaintain\FormStudentDevice.h"
#include "../YDUIExamOperate/FormQueryMark.h"
#include "../YDUIExamOperate/FormPaperAnsys.h"
#include "../YDUIExamOperate/FormErrorReport.h"
#include "../YDUIExamOperate\FormExamInfrar.h"
#include "../YDUIExamOperate\FormExamUSB.h"
#include "../YDUIExamOperate\FormExamInfoBack.h"
#include "../YDUIExamOperate\FormImage.h"
#include "..\YDUIExamOperate\FormImageRead.h"
#include "../YdCom\StaticYdComApp.h"
#include "../Include\YDVaultError.h"
#include "../YDUIExamOperate\FormExamRollingAnswer.h"
#include "../YDUIExamOperate\FormExamInfraRed.h"
#include "../YDUIExamOperate\FormExamBlue.h"

CAddPageHelper::CAddPageHelper()
{
	
}

CAddPageHelper::~CAddPageHelper(void)
{
}

HRESULT CAddPageHelper::AddPage(CObjPropShow* pObjPropShow)
{
	HRESULT hr = E_FAIL;
	CObjPropertySheet* pSheet = pObjPropShow->m_pSheet;
	ASSERT(pSheet);
	if(pObjPropShow->m_type == SHOW_OBJECT)
	{
		ASSERT(pObjPropShow->m_pObjWrapper);
		CString strDBName;
		hr = pObjPropShow->m_pObjWrapper->m_pObjRef->GetDBName(strDBName);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strViewLabel;
		if(strDBName.CompareNoCase(DB_QVAULTCLASSIFICATION) == 0)
		{
			strViewLabel = _T("������");
			pSheet->AddPage(RUNTIME_CLASS (CQuestionVaultClassificationFormView), strViewLabel);
		}
		else if(strDBName.CompareNoCase(DB_QUESTIONVAULT) == 0)
		{
			strViewLabel = _T("���");
			pSheet->AddPage(RUNTIME_CLASS (CQuestionVaultBaseFormView), strViewLabel);
			//�½���ʱ��Ҫ��֪ʶ��ҳ�������ҳ��
			if(pObjPropShow->m_nOp != OP_NEW)
			{
				pSheet->AddPage(RUNTIME_CLASS (CQuestionVaultKnowledgeFormView), _T("֪ʶ��"));
				pSheet->AddPage(RUNTIME_CLASS (CQuestionVaultQTypeFormView), _T("����"));
				pSheet->AddPage(RUNTIME_CLASS (CGeneratePaperFormView), _T("����"));
			}
		}
		else if (strDBName.CompareNoCase(DB_PAPERFOLDER) == 0)
		{
			strViewLabel = _T("�Ծ��");
			pSheet->AddPage(RUNTIME_CLASS (CPaperFolderFormView), strViewLabel);
		}
		else if (strDBName.CompareNoCase(DB_PAPER) == 0)
		{
			strViewLabel = _T("�Ծ�");
			pSheet->AddPage(RUNTIME_CLASS (CPaperFormView), strViewLabel);
		}
		else if (strDBName.CompareNoCase(DB_QUESTIONTYPE) == 0) //����
		{
			strViewLabel = _T("����");
			pSheet->AddPage(RUNTIME_CLASS (CQuestionTypeFormView), strViewLabel);
			pSheet->AddPage(RUNTIME_CLASS (CQuestionListFormView), _T("����ά��"));
		}
		else if (strDBName.CompareNoCase(DB_CHOICEQUESTION) == 0) //ѡ����������
		{
			strViewLabel = _T("����");
			//		AddPage(RUNTIME_CLASS (CChoiceQuestionFormView), strViewLabel);
			pSheet->AddPage(RUNTIME_CLASS (CQuestionKnowledgeListFormView), _T("֪ʶ��"));

		}
		else if (strDBName.CompareNoCase(DB_ARTICLEQUESTION) == 0) //�Ķ��������
		{
			strViewLabel = _T("����");
			pSheet->AddPage(RUNTIME_CLASS (CAtricleQuestionFormView), strViewLabel);
			pSheet->AddPage(RUNTIME_CLASS (CQuestionKnowledgeListFormView), _T("֪ʶ��"));
		}
	}
	else if(pObjPropShow->m_type == SHOW_STRING)
	{
		if(pObjPropShow->m_strName.CompareNoCase(EXAM_BLUE) == 0)
		{
			//��������
			pSheet->AddPage(RUNTIME_CLASS (CFormExamBlue), pObjPropShow->m_strName);
			CGExamStruct* pGExamStruct = NULL;
			hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
			if(FAILED(hr))
			{
				return hr;
			}
			ASSERT(pGExamStruct);
			pGExamStruct->m_bExtractOnlyFromTeacher = FALSE;
		}
		
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_EXTACT_ANSWER) == 0)
		{
			//�����ӽ�ʦ����ȡ��
			pSheet->AddPage(RUNTIME_CLASS (CFormExamBlue), pObjPropShow->m_strName);
			CGExamStruct* pGExamStruct = NULL;
			hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
			if(FAILED(hr))
			{
				return hr;
			}
			ASSERT(pGExamStruct);
			pGExamStruct->m_bExtractOnlyFromTeacher = TRUE;
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAN_FETCH_ANSWER) == 0)
		{
			//������ȡ���Դ�
			pSheet->AddPage(RUNTIME_CLASS (CFormExamRollingAnswer), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_INFRARED) == 0)
		{
			//���⿼��
			pSheet->AddPage(RUNTIME_CLASS (CFormExamInfraRed), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_DEPT_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormDepartment), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_STU_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormStudent), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_STU_DEVICE) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormStudentDevice), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_SUBJECT_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormSubject), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_TEACHER_DEVICE_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormTeacherDevice), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_PAPER_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormPaper), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_EXAMADDR_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormExamAddress), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_MARK_INFO) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormQueryMark), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_PAPER_ANSYS) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormPaperAnsys), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(EXAM_ERR_REPORT) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CFormErrorReport), pObjPropShow->m_strName);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}
