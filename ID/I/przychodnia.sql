-- Zestaw I : Wyzwalacze i reguły
-- Przychodnia.sql

DROP TABLE IF EXISTS lekarze CASCADE;
CREATE TABLE lekarze (
    id integer NOT NULL,
    imie character varying(100) NOT NULL,
    nazwisko character varying(100) NOT NULL
);

DROP TABLE IF EXISTS specjalizacje CASCADE;
CREATE TABLE specjalizacje (
    id_lekarza integer NOT NULL,
    specjalizacja character varying(80) NOT NULL
);

DROP TABLE IF EXISTS pacjenci CASCADE;
CREATE TABLE pacjenci (
    pesel character(11) NOT NULL,
    imie character varying(100) NOT NULL,
    nazwisko character varying(100) NOT NULL
);

DROP TABLE IF EXISTS wizyty CASCADE;
CREATE TABLE wizyty (
    lekarz integer NOT NULL,
    pacjent character(11) NOT NULL,
    data_rozpoczecia timestamp without time zone NOT NULL,
    data_zakonczenia timestamp without time zone NOT NULL,
    CONSTRAINT ck_data CHECK ((data_rozpoczecia < data_zakonczenia))
);

COPY lekarze (id, imie, nazwisko) FROM stdin;
0	Abel	Spencer
1	Erick	Leonard
2	Janice	Montes
3	Gretchen	Proctor
4	Lawanda	Velazquez
5	Robbie	Wilkins
6	Carla	Randall
7	Heath	Dickson
8	Kendra	Rodgers
9	Brandie	Finley
\.


COPY pacjenci (pesel, imie, nazwisko) FROM stdin;
55121400927	Keri	Brennan
67042513037	Cameron	Bass
46062718514	Roberta	Morse
68122519338	Colby	Boone
56062118986	Cornelius	Herring
92060614291	Teddy	Martin
39051200886	Lillian	Huerta
30092104661	Dianna	Chapman
72123101403	Geoffrey	Pacheco
74030811158	Melissa	Calderon
18091210888	Moses	Patel
24092005633	Franklin	Mills
78082506945	Arlene	Mcgrath
81121615959	Kathleen	Huynh
97091307785	Sheldon	Lamb
60122611096	Ivan	Edwards
93061202003	Rose	Escobar
41042504097	Ernest	Robinson
43080817599	Randal	Larsen
46052712258	Ismael	Dickerson
\.


COPY specjalizacje (id_lekarza, specjalizacja) FROM stdin;
0	Dermatologia
0	Alergologia
1	Ginekologia
4	Ginekologia
2	Chirurgia
9	Chirurgia
3	Psychologia
3	Psychiatria
5	Kardiologia
6	Okulistyka
7	Pediatria
7	Alergologia
8	Pediatria
9	Reumatologia
0	Medycyna rodzinna
5	Medycyna rodzinna
8	Medycyna rodzinna
\.


COPY wizyty (lekarz, pacjent, data_rozpoczecia, data_zakonczenia) FROM stdin;
7	97091307785	2008-07-24 11:00:00	2008-07-24 11:30:00
8	46062718514	2008-07-22 14:00:00	2008-07-22 14:30:00
2	72123101403	2008-07-25 08:30:00	2008-07-25 09:00:00
6	72123101403	2008-07-21 08:00:00	2008-07-21 08:30:00
6	56062118986	2008-07-23 11:00:00	2008-07-23 11:30:00
3	55121400927	2008-07-25 12:00:00	2008-07-25 12:30:00
5	39051200886	2008-07-25 13:30:00	2008-07-25 14:00:00
1	81121615959	2008-07-21 11:30:00	2008-07-21 12:00:00
5	46052712258	2008-07-25 09:30:00	2008-07-25 10:00:00
7	97091307785	2008-07-25 08:00:00	2008-07-25 08:30:00
3	55121400927	2008-07-25 13:00:00	2008-07-25 13:30:00
2	68122519338	2008-07-23 13:30:00	2008-07-23 14:00:00
\.


ALTER TABLE ONLY specjalizacje
    ADD CONSTRAINT idx_specjalizacje PRIMARY KEY (id_lekarza, specjalizacja);


ALTER TABLE ONLY wizyty
    ADD CONSTRAINT idx_wizyty UNIQUE (lekarz, pacjent, data_rozpoczecia, data_zakonczenia);


ALTER TABLE ONLY lekarze
    ADD CONSTRAINT pk_lekarze PRIMARY KEY (id);


ALTER TABLE ONLY pacjenci
    ADD CONSTRAINT pk_pacjenci PRIMARY KEY (pesel);


ALTER TABLE ONLY specjalizacje
    ADD CONSTRAINT fk_specjalizacje_lekarze FOREIGN KEY (id_lekarza) REFERENCES lekarze(id);


ALTER TABLE ONLY wizyty
    ADD CONSTRAINT fk_wizyty_lekarze FOREIGN KEY (lekarz) REFERENCES lekarze(id);


ALTER TABLE ONLY wizyty
    ADD CONSTRAINT fk_wizyty_pacjenci FOREIGN KEY (pacjent) REFERENCES pacjenci(pesel);
