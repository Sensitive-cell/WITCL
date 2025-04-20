



CREATE TABLE departments (
	department_id NUMBER(4)  PRIMARY KEY ,
	department_name VARCHAR2(30)   not NULL ,
	manager_id NUMBER(6)
        ) tablespace users;


CREATE TABLE employees (
	employee_id NUMBER(6)  PRIMARY KEY ,
        employee_name VARCHAR2(10),
        email VARCHAR2(30) NOT NULL UNIQUE,
	phone_number VARCHAR2(20),
        hire_date DATE NOT NULL,
        salary NUMBER(8,2)CONSTRAINT e_ck CHECK(salary>0),
        manager_id NUMBER(6),
        department_id NUMBER(4) REFERENCES departments(department_id)
       )tablespace users;

INSERT INTO departments VALUES(1000,'������',100001);
INSERT INTO departments VALUES(1001,'�����',100101);
INSERT INTO departments VALUES(1002,'���ۿ�',100201);
INSERT INTO departments VALUES(1003,'���¿�',100301);
INSERT INTO departments VALUES(1004,'��Ӧ��',100401);

INSERT INTO employees VALUES('100001','������','glf@126.com','1234567890',TO_DATE('1984-4-4','yyyy-mm-dd'),8000,100001,1000);
INSERT INTO employees VALUES('100002','�ֽ���','ljy@126.com','1234567890',TO_DATE('1985-5-5','yyyy-mm-dd'),6000,100001,1000);
INSERT INTO employees VALUES('100003','������','wlh@126.com','1234567890',TO_DATE('1986-8-6','yyyy-mm-dd'),7000,100001,1000);
INSERT INTO employees VALUES('100101','������','lxw@126.com','1234567890',TO_DATE('1984-4-4','yyyy-mm-dd'),8000,100101,1001);
INSERT INTO employees VALUES('100102','����','hy@126.com','1234567890',TO_DATE('1984-4-4','yyyy-mm-dd'),6000,100101,1001);
INSERT INTO employees VALUES('100103','������','llj@126.com','1234567890',TO_DATE('1984-4-4','yyyy-mm-dd'),5000,100101,1001);
INSERT INTO employees VALUES('100201','֣���','zyj@126.com','1234567890',TO_DATE('1983-11-9','yyyy-mm-dd'),8000,100201,1002);
INSERT INTO employees VALUES('100202','�»���','chb@126.com','1234567890',TO_DATE('1984-4-4','yyyy-mm-dd'),5000,100201,1002);
INSERT INTO employees VALUES('100301','����ƽ','fyp@126.com','1234567890',TO_DATE('1984-4-4','yyyy-mm-dd'),8000,100301,1003);
INSERT INTO employees VALUES('100302','�����','wzf@126.com','1234567890',TO_DATE('1983-11-5','yyyy-mm-dd'),5000,100301,1003);
INSERT INTO employees VALUES('100401','����ƽ','wsp@126.com','1234567890',TO_DATE('1985-5-5','yyyy-mm-dd'),8000,100401,1004);
INSERT INTO employees VALUES('100402','������','wlk@126.com','1234567890',TO_DATE('1986-8-6','yyyy-mm-dd'),7000,100401,1004);



