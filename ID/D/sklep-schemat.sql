begin;

drop table if exists historia_cen  cascade;
drop table if exists kategorie  cascade;
drop table if exists klienci  cascade;
drop table if exists produkty  cascade;
drop table if exists produkty_zamowienia  cascade;
drop table if exists rabaty  cascade;
drop table if exists zamowienia  cascade;
drop table if exists rabaty_klienci cascade;
drop table if exists rabaty_produkty cascade;


CREATE TABLE klienci (
    id_klienta numeric(6,0) PRIMARY KEY,
    nazwa character varying(250) NOT NULL,
    numer_telefonu character varying(25),
    email character varying(100),
    nip character(10),
    
    CHECK (((numer_telefonu IS NOT NULL) OR (email IS NOT NULL)))
);


CREATE TABLE kategorie (
    id_kategoria numeric(6,0) PRIMARY KEY,
    nazwa character varying(250) NOT NULL,
    vat numeric(3,1) NOT NULL,
    nadkategoria numeric(6,0) REFERENCES kategorie
);


CREATE TABLE produkty (
    kod_produktu numeric(6,0)  PRIMARY KEY,
    nazwa character varying(250) NOT NULL,
    id_kategoria numeric(6,0) NOT NULL REFERENCES kategorie,
    jednostka_miary character varying(10) NOT NULL,
    
    CHECK (jednostka_miary IN  ('szt', 'g', 'l'))
);

CREATE TABLE historia_cen (
    cena_netto numeric(6,2) NOT NULL,
    kod_produktu numeric(6,0) NOT NULL REFERENCES produkty,
    data_wprowadzenia timestamp without time zone DEFAULT now() NOT NULL,
    
    PRIMARY KEY (kod_produktu, data_wprowadzenia)
);



CREATE TABLE zamowienia (
    id_zamowienia numeric(8,0) PRIMARY KEY,
    id_klienta numeric(6,0) NOT NULL REFERENCES klienci,
    adres_dostawy character varying(500) NOT NULL,
    data_zlozenia date DEFAULT now() NOT NULL
);

CREATE TABLE produkty_zamowienia (
    produkt numeric(6,0) NOT NULL REFERENCES produkty,
    id_zamowienia numeric(8,0) NOT NULL REFERENCES zamowienia,
    ilosc numeric(8,0) NOT NULL,
    
    CONSTRAINT produkty_zamowienia_ilosc_check CHECK ((ilosc > (0)::numeric)),
    PRIMARY KEY (produkt, id_zamowienia)
);


CREATE TABLE rabaty (
    id_rabatu numeric(6,0) PRIMARY KEY,
    znizka numeric(3,1) NOT NULL,
    nazwa character varying,
    data_od timestamp without time zone,
    data_do timestamp without time zone
);


CREATE TABLE rabaty_klienci(
    id_klienta numeric(6,0) REFERENCES klienci,
    id_rabatu numeric(6,0) NOT NULL REFERENCES rabaty,
    id_zamowienia numeric(6,0) REFERENCES zamowienia,
    
    PRIMARY KEY(id_klienta,id_rabatu)
);

CREATE TABLE rabaty_produkty(
    id_rabatu numeric(6,0) NOT NULL references rabaty,
    id_produktu numeric(6,0) NOT NULL references produkty,
    min_ilosc integer NOT NULL DEFAULT 1,
    
    PRIMARY KEY (id_rabatu, id_produktu)
);


commit;
