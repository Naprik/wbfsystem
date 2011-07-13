#pragma once
#include "../YDFormUIBase/YDQuestionDlg.h"
#include "afxcmn.h"


// CYDReadQuestionDlg dialog
class CYDArticleQuestionRef;
class CYDLinkRef;
class CYDQuestionRef;
class CYDMediaRef;

class CYDReadQuestionDlg : public CYDQuestionDlg
{
	DECLARE_DYNAMIC(CYDReadQuestionDlg)

public:
	CYDReadQuestionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYDReadQuestionDlg();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_READ_QUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CListCtrl m_lstChoiceQuestion;
	CString m_strArticle;
	int m_iHardLevel;
	CString m_strCreator;
	CString m_strCreateDate;
	afx_msg void OnBnClickedButtonCqAdd();
private:
	HRESULT ClearData();
	CListCtrlOperate* m_pCQListOperate;
	HRESULT UpdateQuestionArea();
	HRESULT EnableQuestionArea(BOOL bEnable);
	//��ʼ���б�
	HRESULT InitListChoiceQuest(CYDArticleQuestionRef* _pRef);
	HRESULT InitMedia(CYDArticleQuestionRef* _pRef);//��ʼ��Mdia
	HRESULT CreateQuestionRef(CYDArticleQuestionRef* &_pRef);
	HRESULT CreateLinkRef(CYDLinkRef* &_pLinkRef);
	HRESULT UpdateQuestionRef(CYDArticleQuestionRef* _pRef);
	HRESULT PersistRelateQuestion(CYDObjectRef* _pParentRef);//�������¹�������Ŀ
	HRESULT PersistKnowledge(CYDObjectRef* _pParentRef);//��������֪ʶ��
	HRESULT PersistMedia(CYDObjectRef* _pParentRef);//��������ý���ļ�
	BOOL IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper);
	HRESULT InitKnowledge();//��ʼ��֪ʶ��
	HRESULT GetMediaType(MEDIATYPE* _pMType);
	HRESULT GetFileTypeFilter(MEDIATYPE _MType,CString& _strFiltTypeFilter);
	HRESULT CreateMediaRef(CYDMediaRef* &_pRef);
	HRESULT CreateMediaFile(CYDMediaRef* _pRef);
	HRESULT RemoveMediaFile(CYDMediaRef* _pRef);
	HRESULT GetPhotoData(VARIANT* _pVal);
	HRESULT ReadPhotoBuf(CString _strfile);
	void	DestroyPhoto();
	HBITMAP			BufferToHBITMAP();
	void			DrawUserPhoto(int x,int y,CDC* pDC);
	std::list<CYdObjWrapper*>	m_lstAddKPs;
	std::list<CYdObjWrapper*>	m_lstDeleteKPs;

	std::list<CYdObjWrapper*>	m_lstDeleteQuestions;

	std::list<CYdObjWrapper*>   m_lstMedia;//������Media
	BOOL						m_bModifyMedia;//�Ƿ��޸Ĺ���Ƶ
	//������ͼƬҪ�õı���
	HBITMAP			m_hPhotoBitmap;
	char			*m_pBMPBuffer;
	DWORD			m_dwFileLen;
public:
	afx_msg void OnBnClickedButtonModifyQuestion();
	afx_msg void OnBnClickedButtonDelQuestion();
	afx_msg void OnBnClickedButtonCqModify();
	afx_msg void OnBnClickedButtonAddkp();
	afx_msg void OnBnClickedButtonRemovekp();
	afx_msg void OnNMDblclkListQuestion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNewQuestion();
	CListCtrl m_listCtrlKpAll;
	CListCtrl m_listCtrlKpRelated;
	afx_msg void OnBnClickedButtonSelFile();
	CString m_strMediaFile;

	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedButtonAddPic();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonEmptyPic();
};
