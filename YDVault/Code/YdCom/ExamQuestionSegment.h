 #pragma once
// XX XX XX ��һ����Ŀ�������� 
// ��һ��XX�����һ����ŵĿ�ʼ��ţ�
// �ڶ���XX�����һ����ŵĽ�����š�
// ������XX����4λ�����һ�ε���Ŀ�Ƿ��ѡ���磺0000 �����ѡ 0001 ����ѡ��
// ����λ����ѡ��ĸ������磺0011 ��������ѡ��ABC  1111 ����15��ѡ��A-O����Ŀ��

class AFX_EXT_CLASS CExamQuestionSegment //��Ŀ����������
{
public:
	CExamQuestionSegment(void);
	~CExamQuestionSegment(void);
public:
	int		m_iStart;//��ʼ���
	int     m_iEnd;  //�������
	int     m_iMultiType; // 0 :�����ѡ 1:����ѡ
	int		m_iOptionNum;//ѡ��ĸ���
	HRESULT CreateRollcallCmd(CString &_strCmd);//���ɵ�������Ϣ
};
