DROP TABLE IF EXISTS ankiety CASCADE;
DROP TABLE IF EXISTS kursy CASCADE;
DROP TABLE IF EXISTS prowadzacy CASCADE;
DROP TABLE IF EXISTS zajecia CASCADE;
DROP TABLE IF EXISTS studenci CASCADE;
DROP TABLE IF EXISTS zapisani CASCADE;


CREATE TABLE prowadzacy (
    id_prowadzacy integer PRIMARY KEY,
    imie character varying(20) NOT NULL,
    nazwisko character varying(25) NOT NULL
);

CREATE TABLE kursy (
    id_kurs integer PRIMARY KEY,
    nazwa character varying(30) NOT NULL,
    rodzaj character(3) NOT NULL,
    CHECK (rodzaj IN ('wyk', 'lab', 'cw')),
    UNIQUE (nazwa, rodzaj)
);

CREATE TABLE zajecia (
    id_zajecia integer PRIMARY KEY,
    id_kurs integer NOT NULL REFERENCES kursy,
    id_prowadzacy integer NOT NULL REFERENCES prowadzacy,
    nr_grupy integer,
    semestr character(8) NOT NULL
);

CREATE TABLE studenci (
    id_student integer PRIMARY KEY,
    imie character varying(20) NOT NULL,
    nazwisko character varying(25) NOT NULL
);

CREATE TABLE zapisani (
    id_student integer NOT NULL REFERENCES studenci ,
    id_zajecia integer NOT NULL REFERENCES zajecia,
    PRIMARY KEY (id_student, id_zajecia)
);

CREATE TABLE ankiety (
    id_zajecia integer NOT NULL REFERENCES zajecia,
    ocena integer NOT NULL,
    CHECK (ocena IN (1, 2, 3, 4, 5))
);


INSERT INTO prowadzacy VALUES (1, 'Jan', 'Kowalski'), (2, 'Marek', 'Nowak'), (3, 'Anna', 'Wójtowicz');
INSERT INTO kursy VALUES (1, 'Inżynieria danych', 'wyk'), (2, 'Inżynieria danych', 'lab'), (3, 'Metody numeryczne', 'wyk'), (4, 'Metody numeryczne', 'cw');
INSERT INTO zajecia VALUES (1, 1, 1, null, '2015/16L'), (2, 2, 1, 1, '2015/16L'), (3, 2, 2, 2, '2015/16L'), (4, 3, 2, null, '2015/16L'), (5, 4, 2, 1, '2015/16Z');
INSERT INTO studenci VALUES (1, 'Zofia', 'Wiśniewska'), (2, 'Łukasz', 'Jedynak'), (3, 'Romek', 'Wesoły'), (4, 'Michał', 'Rumcajs');
INSERT INTO zapisani VALUES (1, 1), (1, 2), (1,4), (2, 1), (2,3), (2,4), (3, 4), (3, 1), (4, 4), (4, 3);
INSERT INTO ankiety VALUES (4, 5), (1, 4), (4, 3);