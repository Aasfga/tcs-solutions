--ZAD10
----


--ZAD1
DROP TABLE IF EXISTS zwierzeta CASCADE;

CREATE TABLE zwierzeta (
  gatunek        VARCHAR(100) NOT NULL,
  jajorodny      CHAR(1)      NOT NULL
    CHECK (jajorodny = 'T' OR jajorodny = 'N'),
  liczba_konczyn NUMERIC(2)   NOT NULL,
  data_odkrycia  DATE         NOT NULL
);
----

--ZAD2
DROP TABLE IF EXISTS klienci CASCADE;

CREATE TABLE klienci (
  pesel         CHAR(11) PRIMARY KEY,
  adres         VARCHAR(100),
  wiek          NUMERIC(2) NOT NULL,
  wspolpraca_od DATE       NOT NULL
);
----

--ZAD3
DROP TABLE IF EXISTS uczelnie CASCADE;
CREATE TABLE uczelnie
(
  id_uczelni NUMERIC(4),
  nazwa      VARCHAR(100),
  adres      VARCHAR(100),
  budzet     NUMERIC(10, 2) NOT NULL,
  zalozona   DATE           NOT NULL,


  UNIQUE (nazwa),
  PRIMARY KEY (id_uczelni)
);
----

--ZAD4
DROP TABLE IF EXISTS ksiazki CASCADE;
CREATE TABLE ksiazki
(
  id_ksiazki   NUMERIC(10),
  tytul        VARCHAR(100) NOT NULL,
  autorzy      VARCHAR(100),
  cena         NUMERIC(6, 2),
  data_wydania DATE,
  PRIMARY KEY (id_ksiazki)
);
----

DROP TABLE IF EXISTS zespoly CASCADE;
CREATE TABLE zespoly
(
  id_zesp NUMERIC(2) PRIMARY KEY
);

--ZAD5
DROP TABLE IF EXISTS pokoje CASCADE;
CREATE TABLE pokoje
(
  numer_pokoju NUMERIC(3),
  id_zesp      NUMERIC(2),
  liczba_okien NUMERIC(1),
  PRIMARY KEY (numer_pokoju),
  FOREIGN KEY (id_zesp) REFERENCES zespoly (id_zesp)
);
----

--ZAD6
DROP TABLE IF EXISTS plyty_cd CASCADE;
CREATE TABLE plyty_cd
(
  kompozytor    CHAR(100) NOT NULL,
  tytul_albumu  CHAR(100) NOT NULL,
  data_nagrania DATE,
  data_wydania  DATE,
  CHECK (data_nagrania < data_wydania),
  czas_trwania  INTERVAL CHECK (czas_trwania < '1:22:00'),

  CONSTRAINT un_ko_ty UNIQUE (kompozytor, tytul_albumu)
);

----

--ZAD7
DROP TABLE IF EXISTS szef_podwladny CASCADE;
CREATE TABLE szef_podwladny AS
  SELECT
    szef.nazwisko       AS szef,
    pracownicy.nazwisko AS podwladny
  FROM pracownicy
    JOIN pracownicy AS szef
      ON pracownicy.id_szefa = szef.id_prac;
----

--ZAD8

DROP TABLE IF EXISTS plyty_cd CASCADE;
CREATE TABLE plyty_cd
(
  kompozytor    CHAR(100) NOT NULL,
  tytul_albumu  CHAR(100) NOT NULL,
  data_nagrania DATE,
  data_wydania  DATE,
  CHECK (data_nagrania < data_wydania),
  czas_trwania  INTERVAL CHECK (czas_trwania < '1:22:00'),

  CONSTRAINT un_ko_ty UNIQUE (kompozytor, tytul_albumu)
);

ALTER TABLE plyty_cd
  DROP CONSTRAINT un_ko_ty;
ALTER TABLE plyty_cd
  ADD CONSTRAINT un_ko_ty PRIMARY KEY (kompozytor, tytul_albumu);
----

--ZAD9
DROP TABLE IF EXISTS plyty_cd CASCADE;
CREATE TABLE plyty_cd
(
  kompozytor    CHAR(100) NOT NULL,
  tytul_albumu  CHAR(100) NOT NULL,
  data_nagrania DATE,
  data_wydania  DATE,
  CHECK (data_nagrania < data_wydania),
  czas_trwania  INTERVAL CHECK (czas_trwania < '1:22:00'),

  CONSTRAINT un_ko_ty UNIQUE (kompozytor, tytul_albumu)
);

ALTER TABLE plyty_cd
  DROP CONSTRAINT un_ko_ty;
ALTER TABLE plyty_cd
  ADD CONSTRAINT un_ko_ty PRIMARY KEY (kompozytor, tytul_albumu);
ALTER TABLE plyty_cd
  DROP CONSTRAINT un_ko_ty;
INSERT INTO plyty_cd VALUES ('a', 'b', NULL, NULL, NULL);
ALTER TABLE plyty_cd
  ADD CONSTRAINT un_ko_ty PRIMARY KEY (kompozytor, tytul_albumu);
----

--ZAD10
-- DROP TABLE IF EXISTS gatunki CASCADE;
-- DROP TABLE IF EXISTS zwierzeta CASCADE;

CREATE TABLE zwierzeta (
  gatunek        VARCHAR(100) NOT NULL,
  jajorodny      CHAR(1)      NOT NULL
    CHECK (jajorodny = 'T' OR jajorodny = 'N'),
  liczba_konczyn NUMERIC(2)   NOT NULL,
  data_odkrycia  DATE         NOT NULL
);

ALTER TABLE zwierzeta
  RENAME TO gatunki;
DROP TABLE gatunki;
----

--ZAD11
DROP TABLE IF EXISTS projekty CASCADE;

CREATE TABLE projekty
(
  id_projektu      NUMERIC(4),
  opis_projektu    CHAR(20) NOT NULL,
  data_rozpoczecia DATE DEFAULT now(),
  data_zakonczenia DATE CHECK (data_zakonczenia > projekty.data_rozpoczecia),
  fundusz          NUMERIC(7, 2),
  PRIMARY KEY (id_projektu),
  UNIQUE (opis_projektu)
);
----

--ZAD12
DROP TABLE IF EXISTS projekty CASCADE;

CREATE TABLE projekty
(
  id_projektu      NUMERIC(4),
  opis_projektu    CHAR(20) NOT NULL,
  data_rozpoczecia DATE DEFAULT now(),
  data_zakonczenia DATE CHECK (data_zakonczenia > projekty.data_rozpoczecia),
  fundusz          NUMERIC(7, 2),
  PRIMARY KEY (id_projektu),
  UNIQUE (opis_projektu)
);

DROP TABLE IF EXISTS przydzialy CASCADE;
CREATE TABLE przydzialy
(
  id_projektu NUMERIC(4),
  id_prac     NUMERIC(4),
  od          DATE DEFAULT now(),
  "do"        DATE CHECK ("do" > od),
  stawka      NUMERIC(7, 2),
  rola        CHAR(20) CHECK (rola = 'KIERUJACY' OR rola = 'ANALITYK' OR rola = 'PROGRAMISTA'),
  PRIMARY KEY (id_projektu, id_prac),
  FOREIGN KEY (id_projektu) REFERENCES projekty (id_projektu),
  FOREIGN KEY (id_prac) REFERENCES pracownicy (id_prac)
);

----

--ZAD13
DROP TABLE IF EXISTS projekty CASCADE;

CREATE TABLE projekty
(
  id_projektu      NUMERIC(4),
  opis_projektu    CHAR(20) NOT NULL,
  data_rozpoczecia DATE DEFAULT now(),
  data_zakonczenia DATE CHECK (data_zakonczenia > projekty.data_rozpoczecia),
  fundusz          NUMERIC(7, 2),
  PRIMARY KEY (id_projektu),
  UNIQUE (opis_projektu)
);

DROP TABLE IF EXISTS przydzialy CASCADE;
CREATE TABLE przydzialy
(
  id_projektu NUMERIC(4),
  id_prac     NUMERIC(4),
  od          DATE DEFAULT now(),
  "do"        DATE CHECK ("do" > od),
  stawka      NUMERIC(7, 2),
  rola        CHAR(20) CHECK (rola = 'KIERUJACY' OR rola = 'ANALITYK' OR rola = 'PROGRAMISTA'),
  PRIMARY KEY (id_projektu, id_prac),
  FOREIGN KEY (id_projektu) REFERENCES projekty (id_projektu),
  FOREIGN KEY (id_prac) REFERENCES pracownicy (id_prac)
);

ALTER TABLE przydzialy
  ADD COLUMN godziny NUMERIC;
----

--ZAD15
DROP TABLE IF EXISTS projekty CASCADE;

CREATE TABLE projekty
(
  id_projektu      NUMERIC(4),
  opis_projektu    CHAR(20) NOT NULL,
  data_rozpoczecia DATE DEFAULT now(),
  data_zakonczenia DATE CHECK (data_zakonczenia > projekty.data_rozpoczecia),
  fundusz          NUMERIC(7, 2),
  PRIMARY KEY (id_projektu),
  UNIQUE (opis_projektu)
);

ALTER TABLE projekty
  ALTER COLUMN opis_projektu TYPE CHAR(30);
----

--ZAD17
ALTER TABLE pracownicy
  DROP IF EXISTS imie;
----

--ZAD18

DROP TABLE IF EXISTS pracownicy_zespoly;

CREATE TABLE pracownicy_zespoly AS
  SELECT
    nazwisko,
    etat,
    12 * placa_pod AS roczna_placa,
    zespoly.nazwa  AS zespol,
    zespoly.adres  AS adres_pracy
  FROM
    pracownicy
    JOIN zespoly
    USING (id_zesp);
----