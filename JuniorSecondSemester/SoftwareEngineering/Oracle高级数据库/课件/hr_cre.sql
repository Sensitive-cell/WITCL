--ɾ������Լ������
PROMPT
PROMPT ɾ��HRģʽ���������ݿ����......

ALTER TABLE departments DROP CONSTRAINT dept_mgr_fk;
ALTER TABLE employees DROP CONSTRAINT emp_mgr_fk;

DROP TABLE sal_grades;
DROP TABLE job_history;
DROP TABLE employees;
DROP TABLE jobs;
DROP TABLE departments;
DROP TABLE locations;
DROP TABLE countries;
DROP TABLE regions;

DROP SEQUENCE employees_seq;
DROP SEQUENCE departments_seq;
DROP SEQUENCE locations_seq;

--����REGIONS��
PROMPT
PROMPT ����REGIONS��......

CREATE TABLE regions
( region_id NUMBER PRIMARY KEY, 
  region_name VARCHAR2(25)
)
TABLESPACE USERS;

--����COUNTRIES��
PROMPT
PROMPT ����COUNTRIES��......

CREATE TABLE countries
( country_id CHAR(2) PRIMARY KEY, 
  country_name VARCHAR2(40), 
  region_id NUMBER REFERENCES regions(region_id)
)
  ORGANIZATION INDEX
  TABLESPACE USERS;


--����LOCATIONS��
PROMPT
PROMPT ����LOCATIONS��......

CREATE TABLE locations
( location_id NUMBER(4) PRIMARY KEY, 
  street_address VARCHAR2(40), 
  postal_code VARCHAR2(12), 
  city VARCHAR2(30) NOT NULL, 
  state_province VARCHAR2(25), 
  country_id CHAR(2)  REFERENCES countries(country_id)
)
 TABLESPACE USERS;


--����DEPARTMENTS��
PROMPT
PROMPT ����DEPARTMENTS��......

CREATE TABLE departments
( department_id NUMBER(4) PRIMARY KEY, 
  department_name VARCHAR2(30) NOT NULL, 
  manager_id NUMBER(6) , 
  location_id NUMBER(4) REFERENCES locations (location_id)
)
 TABLESPACE USERS;


--����JOBS��
PROMPT
PROMPT ����JOBS��......

CREATE TABLE jobs
( job_id VARCHAR2(10) PRIMARY KEY, 
  job_title VARCHAR2(35) NOT NULL, 
  min_salary NUMBER(6), 
  max_salary NUMBER(6)
)
 TABLESPACE USERS;

--����EMPLOYEES��
PROMPT
PROMPT ����EMPLOYEES��......

CREATE TABLE employees
( employee_id NUMBER(6) PRIMARY KEY, 
  first_name VARCHAR2(20), 
  last_name VARCHAR2(25) NOT NULL, 
  email VARCHAR2(25) NOT NULL UNIQUE, 
  phone_number VARCHAR2(20), 
  hire_date DATE NOT NULL, 
  job_id VARCHAR2(10) NOT NULL REFERENCES jobs (job_id), 
  salary NUMBER(8,2) CHECK (salary > 0),
  commission_pct NUMBER(2,2), 
  manager_id NUMBER(6), 
  department_id NUMBER(4) REFERENCES departments(department_id)
) 
 TABLESPACE USERS;

--����JOB_HISTORY��
PROMPT
PROMPT ����JOB_HISTORY��......

CREATE TABLE job_history
( employee_id NUMBER(6) NOT NULL REFERENCES employees(employee_id), 
  start_date DATE NOT NULL, 
  end_date DATE NOT NULL, 
  job_id VARCHAR2(10) NOT NULL REFERENCES jobs(job_id), 
  department_id NUMBER(4) REFERENCES departments(department_id),
  CONSTRAINT jhist_date_interval CHECK (end_date > start_date),
  CONSTRAINT jhist_emp_id_st_date_pk PRIMARY KEY (employee_id, start_date)
) 
 TABLESPACE USERS;

--����SAL_GRADES��
PROMPT
PROMPT ����SAL_GRADES��......

CREATE TABLE sal_grades
(grade  NUMBER PRIMARY KEY,
min_salary  NUMBER(8,2),
max_salary  NUMBER(8,2)
)
TABLESPACE USERS;

--����MANAGER��
PROMPT
PROMPT ����MANAGER��......

CREATE TABLE managers(
manager_id NUMBER(2)PRIMARY KEY,
manager_name CHAR(20),
password  VARCHAR2(20) NOT NULL
)
TABLESPACE USERS


--Ϊdepartments���manager_id��������Լ��
PROMPT
PROMPT ������Լ��......

ALTER TABLE departments ADD CONSTRAINT dept_mgr_fk FOREIGN KEY(manager_id) references employees(employee_id) DISABLE;
--Ϊemployees���manager_id��������Լ��
ALTER TABLE employees ADD CONSTRAINT emp_mgr_fk FOREIGN KEY(manager_id) references employees(employee_id) DISABLE;


/*
Ϊ��������ݵĲ�ѯЧ�ʣ���Ҫ���ʵ�����ʵ����ϴ������������������洢��INDX��ռ��С�

��1����employees ���department_id���ϴ���һ����Ϊ��EMP_DEPARTMENT_INDX����ƽ����������
��2����employees ���job_id���ϴ���һ����Ϊ��EMP_JOB_INDX����ƽ����������
��3����employees ���manager_id���ϴ���һ����Ϊ��EMP_MANAGER_INDX����ƽ����������
��4����employees ���last_name, first_name���ϴ���һ����Ϊ��EMP_NAME_INDX���ĸ���������
��5����departments ���location_id���ϴ���һ����Ϊ��DEPT_LOCATION_INDX����ƽ����������
��6����job_history ���job_id���ϴ���һ����Ϊ��JHIST_JOB_INDX����ƽ����������
��7����job_history���employee_id���ϴ���һ����Ϊ��JHIST_EMP_INDX����ƽ����������
��8����job_history���department_id���ϴ���һ����Ϊ��JHIST_DEPT_INDX����ƽ����������
��9����locations���city���ϴ���һ����Ϊ��LOC_CITY_INDX����ƽ����������
��10����locations���state_province���ϴ���һ����Ϊ��LOC_STATE_PROVINCE_INDX����ƽ����������
��11����locations���country_id���ϴ���һ����Ϊ��LOC_COUNTRY_INDX����ƽ����������
*/

PROMPT
PROMPT ��������......

CREATE INDEX emp_department_INDX ON employees (department_id) TABLESPACE indx;
CREATE INDEX emp_job_INDX ON employees(job_id) TABLESPACE indx;
CREATE INDEX emp_manager_INDX ON employees (manager_id) TABLESPACE indx;
CREATE INDEX emp_name_INDX ON employees (last_name, first_name) TABLESPACE indx;
CREATE INDEX dept_location_INDX ON departments (location_id) TABLESPACE indx;
CREATE INDEX jhist_job_INDX ON job_history (job_id) TABLESPACE indx;
CREATE INDEX jhist_emp_INDX ON job_history (employee_id) TABLESPACE indx;
CREATE INDEX jhist_dept_INDX ON job_history (department_id) TABLESPACE indx;
CREATE INDEX loc_city_INDX ON locations (city) TABLESPACE indx;
CREATE INDEX loc_state_province_INDX ON locations (state_province) TABLESPACE indx;
CREATE INDEX loc_country_INDX ON locations (country_id) TABLESPACE indx;



PROMPT
PROMPT ������ͼ......

--����һ����Ϊ��EMP_DETAILS_VIEW������ͼ����������Ա����Ա���š�ְλ�����źš����Һ�λ����Ϣ��
CREATE OR REPLACE VIEW emp_details_view
(employee_id,job_id,manager_id,department_id,location_id,country_id,first_name,last_name,salary,commission_pct,
department_name,job_title,city,state_province,country_name,region_name)
AS 
SELECT e.employee_id,e.job_id,e.manager_id,e.department_id,d.location_id,l.country_id,e.first_name,e.last_name,
e.salary,e.commission_pct,d.department_name,j.job_title,l.city,l.state_province,c.country_name,r.region_name
FROM employees e,departments d,jobs j,locations l,countries c,regions r
WHERE e.department_id = d.department_id AND d.location_id = l.location_id AND l.country_id = c.country_id
AND c.region_id = r.region_id AND j.job_id = e.job_id 
WITH READ ONLY;

--����һ����Ϊ��DEPT_STAT_VIEW������ͼ���������źš���������������ƽ�����ʡ�������߹��ʡ�������͹����Լ����Ź����ܺ͡�
CREATE OR REPLACE VIEW dept_stat_view
AS
SELECT department_id,count(*) num,avg(salary) avgsal,max(salary) maxsal,min(salary) minsal,sum(salary) totalsal
FROM employees GROUP BY department_id;



PROMPT
PROMPT ��������......

--����һ����Ϊ��EMPLOYEES_SEQ�������У����ڲ���Ա����ţ���ʼֵΪ100������Ϊ1�������棬��ѭ����
CREATE SEQUENCE employees_seq
START WITH 100
INCREMENT BY 1
NOCACHE
NOCYCLE;

--����һ����Ϊ��DEPARTMENTS_SEQ�������У����ڲ������ű�ţ���ʼֵΪ10������Ϊ10�����ֵΪ9990�������棬��ѭ����

CREATE SEQUENCE departments_seq
START WITH 10
INCREMENT BY 10
MAXVALUE 9990
NOCACHE
NOCYCLE;

--����һ����Ϊ��LOCATIONS_SEQ�������У����ڲ���λ�ñ�ţ���ʼֵΪ1000������Ϊ100�����ֵΪ9990�������棬��ѭ����
CREATE SEQUENCE locations_seq
START WITH 1000
INCREMENT BY 100
MAXVALUE 9900
NOCACHE
NOCYCLE;

