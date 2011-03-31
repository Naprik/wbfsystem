delete from qvaultquestion
where id_type = 1
and ID_B NOT in (select OBJID from choicequestion)

delete from qvaultquestion
where id_type = 2
and ID_B NOT in (select OBJID from articlequestion)


select * from qvaultquestion
where id_type = 2
and ID_B NOT in (select OBJID from articlequestion)


