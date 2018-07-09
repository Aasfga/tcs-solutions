DROP TABLE if exists PRACOWNICY;
DROP TABLE if exists ZESPOLY;
DROP TABLE if exists ETATY;
 
CREATE TABLE ZESPOLY
	(ID_ZESP NUMERIC(2) CONSTRAINT PK_ZESP PRIMARY KEY,
	NAZWA CHARACTER VARYING(20),
	ADRES CHARACTER VARYING(20) );

CREATE TABLE ETATY
      ( NAZWA CHARACTER VARYING(15) CONSTRAINT PK_ETAT PRIMARY KEY,
	PLACA_OD NUMERIC(6,2),
	PLACA_DO NUMERIC(6,2));

CREATE TABLE PRACOWNICY
       (ID_PRAC NUMERIC(4) CONSTRAINT PK_PRAC PRIMARY KEY,
	NAZWISKO CHARACTER VARYING(15),
	IMIE CHARACTER VARYING(15),
	ETAT CHARACTER VARYING(15) CONSTRAINT FK_ETAT REFERENCES ETATY(NAZWA),
	ID_SZEFA NUMERIC(4) CONSTRAINT FK_ID_SZEFA REFERENCES PRACOWNICY(ID_PRAC), 
	ZATRUDNIONY DATE,
	PLACA_POD NUMERIC(6,2) CONSTRAINT MIN_PLACA_POD CHECK(PLACA_POD>100),
	PLACA_DOD NUMERIC(6,2),
	ID_ZESP NUMERIC(2) CONSTRAINT FK_ID_ZESP REFERENCES ZESPOLY(ID_ZESP));
  
INSERT INTO ZESPOLY VALUES (10,'ADMINISTRACJA',      'PIOTROWO 2');
INSERT INTO ZESPOLY VALUES (20,'SYSTEMY ROZPROSZONE','PIOTROWO 3A');
INSERT INTO ZESPOLY VALUES (30,'SYSTEMY EKSPERCKIE', 'STRZELECKA 14');
INSERT INTO ZESPOLY VALUES (40,'ALGORYTMY',          'WIENIAWSKIEGO 16');
INSERT INTO ZESPOLY VALUES (50,'BADANIA OPERACYJNE', 'MIELZYNSKIEGO 30');

INSERT INTO ETATY VALUES ('PROFESOR'  ,3000.00, 4000.00);
INSERT INTO ETATY VALUES ('ADIUNKT'   ,2510.00, 3000.00);
INSERT INTO ETATY VALUES ('ASYSTENT'  ,1500.00, 2100.00);
INSERT INTO ETATY VALUES ('DOKTORANT'  ,800.00, 1000.00);
INSERT INTO ETATY VALUES ('SEKRETARKA',1470.00, 1650.00);
INSERT INTO ETATY VALUES ('DYREKTOR' ,4280.00,5100.00);
 
INSERT INTO PRACOWNICY VALUES (100,'Marecki','Jan'    ,'DYREKTOR'  ,NULL,to_date('01-01-1968','DD-MM-YYYY'),4730.00,980.50,10);
INSERT INTO PRACOWNICY VALUES (110,'Janicki','Karol'  ,'PROFESOR'  ,100 ,to_date('01-05-1973','DD-MM-YYYY'),3350.00,610.00,40);
INSERT INTO PRACOWNICY VALUES (120,'Nowicki','Pawel'  ,'PROFESOR'  ,100 ,to_date('01-09-1977','DD-MM-YYYY'),3070.00,  NULL,30);
INSERT INTO PRACOWNICY VALUES (130,'Nowak','Piotr' ,'PROFESOR'  ,100 ,to_date('01-07-1968','DD-MM-YYYY'), 3960.00,  NULL,20);
INSERT INTO PRACOWNICY VALUES (140,'Kowalski','Krzysztof','PROFESOR'  ,130 ,to_date('15-09-1975','DD-MM-YYYY'), 3230.00,805.00,20);
INSERT INTO PRACOWNICY VALUES (150,'Grzybowska','Maria','ADIUNKT'   ,130 ,to_date('01-09-1977','DD-MM-YYYY'), 2845.50,  NULL,20);
INSERT INTO PRACOWNICY VALUES (160,'Krakowska','Joanna', 'SEKRETARKA'   ,130 ,to_date('01-03-1985','DD-MM-YYYY'), 1590.00,  NULL,20);
INSERT INTO PRACOWNICY VALUES (170,'Opolski','Roman'  ,'ASYSTENT'  ,130 ,to_date('01-10-1992','DD-MM-YYYY'), 1839.70, 480.50,20);
INSERT INTO PRACOWNICY VALUES (190,'Kotarski','Konrad', 'ASYSTENT'  ,140 ,to_date('01-09-1993','DD-MM-YYYY'), 1971.00,  NULL,20);
INSERT INTO PRACOWNICY VALUES (180,'Makowski', 'Marek', 'ADIUNKT',100 ,to_date('20-02-1985','DD-MM-YYYY'), 2610.20,  NULL,10);
INSERT INTO PRACOWNICY VALUES (200,'Przywarek','Leon' ,'DOKTORANT'  ,140 ,to_date('15-07-1994','DD-MM-YYYY'), 900.00,  NULL,30);
INSERT INTO PRACOWNICY VALUES (210,'Kotlarczyk','Stefan','DOKTORANT'  ,130 ,to_date('15-10-1993','DD-MM-YYYY'), 900.00,570.60,30);
INSERT INTO PRACOWNICY VALUES (220,'Siekierski', 'Mateusz','ASYSTENT'  ,110 ,to_date('01-10-1993','DD-MM-YYYY'), 1889.00,  NULL,20);
INSERT INTO PRACOWNICY VALUES (230,'Dolny', 'Tomasz' ,'ASYSTENT'  ,120 ,to_date('01-09-1992','DD-MM-YYYY'), 1850.00, 390.00,NULL);
 

COMMIT;

