#pragma once

//���Բ���
#define EXAM_SERACH_TEACHER				_T("������ʦ��")
#define EXAM_FM_REDIO                   _T("FM������")
#define EXAM_BULETOOTH_RECEIVE_PAPER	_T("�����վ�")
#define EXAM_REDOUT_RECEIVE_PAPER		_T("�����վ�")
#define EXAM_USB_RECEIVE_PAPER			_T("USB�վ�")
#define EXAM_EXAM_BACK					_T("���Ա���")
#define EXAM_SPECIAL_DELAY				_T("�ٵ�")
#define EXAM_SPECIAL_BEFORE				_T("��ǰ����")
#define EXAM_SPECIAL_EQUIP_REPLCACE		_T("�豸����")
#define EXAM_INFO_INPUT					_T("��Ϣ����")
#define EXAM_CREATE_PAPER_FILE			_T("���ɿ����ļ�")
#define EXAM_PIC_GATHER					_T("ͼ��ɼ�")
#define EXAM_PAPER_READOVER             _T("�Ծ�����")
#define EXAM_EXAM_OPERATE               _T("���Բ���")
#define EXAM_EXAM                       _T("����")
#define EXAM_MARK_INFO					_T("�ɼ���ѯ")
#define EXAM_PAPER_ANSYS				_T("�Ծ����")
#define EXAM_ERR_REPORT					_T("���ⱨ��")
#define EXAN_FETCH_ANSWER               _T("������ȡ��")
#define EXAM_EXTACT_ANSWER              _T("��ʦ����ȡ��")
#define EXAM_INFRARED                   _T("���⿼��")
#define EXAM_BLUE						_T("��������")

//���ݹ���
#define EXAM_DATAMGR_TREE				_T("����ά��")
#define EXAM_DEPT_INFO					_T("Ժϵ��Ϣ")
#define EXAM_STU_INFO					_T("ѧ����Ϣ")
#define EXAM_STU_DEVICE					_T("ѧ���豸")
#define EXAM_ANSWER_INFO                _T("����Ϣ")
#define EXAM_TEACHER_DEVICE_INFO		_T("��ʦ�豸")
#define EXAM_PAPER_INFO					_T("�Ծ�����")
#define EXAM_SUBJECT_INFO				_T("���Կ�Ŀ")
#define EXAM_EXAMADDR_INFO              _T("������Ϣ")


#define DB_YDSTUINFO                        _T("STUDENT")   //ѧ����Ϣ��
#define FIELD_YDSTU_ID						_T("ID")       //ѧ��
#define FIELD_YDSTU_NAME                    _T("NAME")     //ѧ������
#define FIELD_YDSTU_GENDER					_T("GENDER")   //ѧ���Ա�
#define FIELD_YDSTU_AGE						_T("AGE")	  //ѧ������
#define FIELD_YDSTU_TEL						_T("TEL")	  //ѧ���绰
#define FIELD_YDSTU_MAIL					_T("EMAIL")	  //ѧ��Emai
#define FIELD_YDSTU_PHOTO					_T("PHOTO")	  //ѧ��ͼƬ
#define FIELD_YDSTU_EVID					_T("DEVICEID")	  //�豸ID



#define DB_YDDEPARTMENT                     _T("DEPARTMENT") //������Ϣ
#define DB_YDLINK_DEPARTMENT_CLASS          _T("DEPTCLS") //������༶�Ĺ�ϵ

#define FIELD_YDDEPARTMENT_ID               _T("ID")     //����ID
#define FIELD_YDDEPARTMENT_NAME             _T("NAME")   //��������
#define FIELD_YDDEPARTMENT_PRESIDENT        _T("PRESIDENT")    //Ժ��
#define FIELD_YDDEPARTMENT_PRESIDENT_TEL    _T("TEL") //��ϵ��ʽ

#define DB_YDCLASS                          _T("CLASS")   //�༶
#define DB_YDLINK_CLASS_STUDENT             _T("CLSSTU") //�༶��ѧ���Ĺ�ϵ

#define FIELD_YDCLASS_CLASSID               _T("ID")  //�༶ID
#define FIELD_YDCLASS_NAME                  _T("NAME") //�༶����
#define FIELD_YDCLASS_HEAD					_T("HEAD")//	������
#define FIELD_YDCLASS_HEADTEL               _T("TEL") //	��ϵ��ʽ

#define DB_YDEXAMADDR                        _T("EXAMADDR")   //����
#define DB_YDLINK_EXAMADDR_UNIT          _T("EADDRUNIT") //�����뿼����Ԫ

#define FIELD_YDEXAMADDR_ID						_T("ID")  //����ID
#define FIELD_YDEXAMADDR_ADDR                  _T("ADDR") //�����ص�

#define DB_YDTEACHERDEV		                _T("TEACHERDEVICE") //��ʦ����ַ��Ϣ
#define FIELD_YDTEACHERDEV_TEACHID          _T("ID")   //	��ʦ���
#define FIELD_YDTEACHERDEV_NAME             _T("NAME") //	��ʦ����
#define FIELD_YDTEACHERDEV_DEVID			_T("DEVICEID")//��ʦ���豸��

#define DB_YDSUBJECT                        _T("SUBJECT")  //���Կ�Ŀ��Ϣ��
#define FIELD_YDSUBJECT_ID                  _T("ID")	 //��Ŀ���
#define FIELD_YDSUBJECT_NAME                 _T("NAME")	//��Ŀ������

#define DB_YDLINK_SUBJECT_PAPER             _T("SUBJECTPAPER") //���Կ�Ŀ���Ծ�Ĺ�ϵ

#define DB_YDEXAMADDRUNIT                   _T("EXAMADDRUNIT")  
#define FIELD_YDEXAMADDRUNIT_STUID          _T("STUID")	
#define FIELD_YDEXAMADDRUNIT_DEVICEID       _T("DEVICEID")	
#define FIELD_YDEXAMADDRUNIT_EXAMID			_T("EXAMID")

#define DB_YDSTUMARK                   _T("STUMARK")  
#define FIELD_YDSTUMARK_STUID          _T("STUID")	
#define FIELD_YDSTUMARK_DEVICEID       _T("DEVICEID")	
#define FIELD_YDSTUMARK_EXAMINFOID     _T("EXAMINFOID")	
#define FIELD_YDSTUMARK_EXAMID			_T("EXAMID")
#define FIELD_YDSTUMARK_ANSWER			_T("ANSWER")
#define FIELD_YDSTUMARK_PAPERID			_T("PAPERID")
#define FIELD_YDSTUMARK_MARK			_T("MARK")
#define FIELD_YDSTUMARK_SUBJECTID		_T("SUBJECTID")
#define FIELD_YDSTUMARK_EXAMDATE		_T("EXAMDATE")

#define DB_YDRESULT                                 _T("ydresult")   //ѧ������Ϣ��
#define FIELD_YDRESULT_STUDID						_T("StudID")      //ѧ����Ϣ��OBJID
#define FIELD_YDRESULT_PAPERID                      _T("PaperID")     //�Ծ���
#define FIELD_YDRESULT_START						_T("Studstart")	  //����ʱ��
#define FIELD_YDRESULT_END							_T("Studend")	  //����ʱ��
#define FIELD_YDRESULT_ASNSWER						_T("Studanswer")  //ѧ����
#define FIELD_YDRESULT_ADDRID                       _T("AddrID")	  //�������

#define DB_YDDEVICE							_T("DEVICE")   //�豸��

#define FIELD_YDDEVICE_ID					_T("ID")  //�豸���
#define FIELD_YDDEVICE_MAC					_T("MAC") //�豸������ַ

#define DB_YDEXAM_INFO						_T("EXAMINFO")  //�࿼��Ϣ
#define FIELD_YEEXAMINFO_PAPERID            _T("PAPERID")   //�Ծ�OBJID
#define FIELD_YEEXAMINFO_STARTTIME          _T("StartTime")   //��ʼʱ��
#define FIELD_YEEXAMINFO_ENDTIME            _T("EndTime")   //����ʱ��
#define FIELD_YEEXAMINFO_TEACHER            _T("TEACHER")   //�࿼��ʦ
#define FIELD_YEEXAMINFO_EXAMXML            _T("EXAMXML")   //������Ϣ�ļ���

#define DB_YDLINK_EADDREINFO                _T("EADDREINFO") //�����뿼����Ϣ��ϵ

#define IDC_EXAM_DM_QUERY_STU_CONN		100000

enum YDEXAM_ROLLCALL_TYPE //������ʽ
{
	ROLLCALL_TYPE_UNKNOW = -1, //δ֪
	ROLLCALL_TYPE_BLUE = 0,       //����
	ROLLCALL_TYPE_INFRAR = 1,     //����
	ROLLCALL_TYPE_USB = 2          //USB
};

enum YDEXAM_ROLLING_TYPE //�վ�ʽ
{
	ROLLING_TYPE_UNKNOW = -1, //δ֪
	ROLLING_TYPE_BLUE = 0,       //����
	ROLLING_TYPE_INFRAR = 1,     //����
	ROLLING_TYPE_USB = 2          //USB
};

enum YDEXAM_KIND //��������
{
	YDEXAM_KIND_NORMAL = 0,//��ʽ����
	YDEXAM_KIND_TEST  = 1 //һ�����
};

enum YDEAM_STEP //���ԵĲ���
{
	YDEXAM_STEP_NOT = -1,//ʲôҲû�н���
	YDEXAM_STEP_ROOLCALL = 0,//�Ѿ�������
	YDEXAM_STEP_BGGINEXAM = 1,//�Ѿ�������
	YDEXAM_STEP_ROLLING  = 2//�Ѿ�����
};

