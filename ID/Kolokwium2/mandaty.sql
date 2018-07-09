DROP TABLE IF EXISTS kierowcy CASCADE;
DROP TABLE IF EXISTS mandaty CASCADE;
DROP TABLE IF EXISTS ograniczenia_uprawnien CASCADE;
DROP TABLE IF EXISTS uprawnienia CASCADE;
DROP TABLE IF EXISTS wykroczenia CASCADE;


CREATE TABLE kierowcy (
    pesel character(11) NOT NULL,
    imie character varying NOT NULL,
    nazwisko character varying NOT NULL
);


CREATE TABLE mandaty (
    id integer NOT NULL,
    pesel character(11) NOT NULL,
    id_wykroczenia integer NOT NULL,
    data date NOT NULL
);


CREATE TABLE ograniczenia_uprawnien (
    pesel character(11) NOT NULL,
    od date NOT NULL,
    "do" date NOT NULL,
    podstawa_prawna character varying
);


CREATE TABLE uprawnienia (
    pesel character(11) NOT NULL,
    kategoria character(1) NOT NULL,
    od date NOT NULL,
    "do" date
);


CREATE TABLE wykroczenia (
    id integer NOT NULL,
    opis character varying,
    punkty integer NOT NULL
);


insert into kierowcy values 
(86092478268, 'Jan', 'Kowalski'), (99033168793, 'Marek', 'Nowak'), (91030915521, 'Anna', 'Włodarczyk'),
(58042866539, 'Wojciech', 'Czubaty'), (58041662716, 'Elżbieta', 'Malinowska'), (32051699218, 'Jerzy', 'Balawander'),
(41031629392, 'Zofia', 'Kopecka'), (82021936432, 'Andrzej', 'Firlej'), (96111828983, 'Łukasz', 'Pietrzyk'), (30033096987, 'Łukasz', 'Pietrzyk'),
(48072892491, 'Agnieszka', 'Słupecka');

insert into wykroczenia values 
(1, 'Przekroczenie prędkości: powyżej 50 km/h', 10),
(2, 'Przekroczenie prędkości: od 41 do 50 km/h', 8),
(3, 'Przekroczenie prędkości: od 31 do 40 km/h', 6),
(4, 'Przekroczenie prędkości: od 21 do 30 km/h', 4),
(5, 'Przekroczenie prędkości: od 11 do 20 km/h', 2),
(6, 'Przekroczenie prędkości: do 10 km/h', 0),
(7, 'Jazda z prędkością utrudniającą ruch innym kierującym: ', 2),
(8, 'Nieupewnienie się co do możliwości wyprzedzania', 5),
(9, 'Wyprzedzanie z niewłaściwej strony', 3),
(10, 'Naruszenie zakazu wyprzedzania: na przejazdach rowerowych i bezpośrednio przed nimi', 10),
(11, 'Naruszenie zakazu wyprzedzania: przy dojeżdżaniu do wierzchołka wzniesienia', 5),
(12, 'Naruszenie zakazu wyprzedzania: na zakrętach oznaczonych znakami ostrzegawczymi', 5),
(13, 'Naruszenie zakazu wyprzedzania: na skrzyżowaniach', 5),
(14, 'Naruszenie zakazu wyprzedzania: na przejazdach tramwajowych i bezpośrednio przed nimi', 5),
(15, 'Niestosowanie się do znaku B-25 lub B-26', 5),
(16, 'Zwiększenie prędkości przez kierującego pojazdem wyprzedzanym', 3),
(17, 'Wyprzedzanie pojazdu uprzywilejowanego na obszarze zabudowanym', 3),
(18, 'Niestosowanie się podczas jazdy do obowiązku używania wymaganych przepisami świateł: od zmierzchu do świtu', 4),
(19, 'Niestosowanie się podczas jazdy do obowiązku używania wymaganych przepisami świateł: w warunkach zmniejszonej przejrzystości powietrza', 2),
(20, 'Niestosowanie się podczas jazdy do obowiązku używania wymaganych przepisami świateł: w okresie od świtu do zmierzchu', 2),
(21, 'Niestosowanie się podczas jazdy do obowiązku używania wymaganych przepisami świateł: w tunelu, niezależnie od pory doby', 4),
(22, 'Naruszenie warunków dopuszczalności używania świateł przeciwmgłowych przednich', 2),
(23, 'Naruszenie warunków dopuszczalności używania świateł przeciwmgłowych tylnych', 2),
(24, 'Pozostawienie pojazdu bez wymaganego przepisami oświetlenia', 3),
(25, 'Korzystanie ze świateł drogowych w sposób niezgodny z przepisami', 3);

insert into mandaty values
(1, 86092478268, 3, '2014-09-17'), 
(2, 86092478268, 5, '2016-08-31'),
(3, 96111828983, 8, '2016-12-31'),
(4, 86092478268, 24, '2017-01-05'),
(5, 96111828983, 15, '2017-02-28'), 
(6, 91030915521, 13, '2017-03-27'),
(7, 91030915521, 6, '2017-03-28'), 
(8, 91030915521, 9, '2017-03-29'),
(9, 30033096987, 6, '2017-04-01'),
(10, 30033096987, 6, '2017-04-02'),
(11, 41031629392, 1, '2017-05-24'), 
(12, 41031629392, 10, '2017-06-01'), 
(13, 41031629392, 3, '2017-06-10');

insert into uprawnienia values
(86092478268, 'B', '2010-08-17', null),
(86092478268, 'C', '2012-10-03', null),
(99033168793, 'B', '2017-04-19', '2022-04-18'),
(91030915521, 'B', '2014-07-31', '2024-07-30'),
(58042866539, 'A', '1980-11-04', null),
(58041662716, 'B', '1989-12-19', null),
(32051699218, 'B', '1950-08-26', null),
(41031629392, 'A', '1965-11-10', null),
(41031629392, 'B', '1979-03-23', null),
(41031629392, 'C', '2005-06-05', '2018-12-04'),
(82021936432, 'B', '2010-08-17', null),
(96111828983, 'B', '2005-08-17', '2018-08-16'),
(30033096987, 'B', '1982-05-28', null),
(48072892491, 'B', '1967-07-15', null);

insert into ograniczenia_uprawnien values
(48072892491, '2008-11-01', '2011-10-31', 'Jazda pod wpływem alkoholu.'),
(82021936432, '2014-07-13', '2017-07-12', 'Przekroczenie dozwolonego limitu punktów.'),
(32051699218, '2017-05-14', '2017-08-13', 'Kierowanie pojazdem z prędkością przekraczającą dopuszczalną o więcej niż 50 km/h na obszarze zabudowanym');


ALTER TABLE ONLY uprawnienia
    ADD CONSTRAINT idx_uprawnienia PRIMARY KEY (pesel, kategoria);


ALTER TABLE ONLY kierowcy
    ADD CONSTRAINT pk_kierowcy PRIMARY KEY (pesel);


ALTER TABLE ONLY mandaty
    ADD CONSTRAINT pk_mandaty PRIMARY KEY (id);


ALTER TABLE ONLY wykroczenia
    ADD CONSTRAINT pk_wykroczenia PRIMARY KEY (id);


CREATE INDEX idx_mandaty ON mandaty USING btree (id_wykroczenia);


CREATE INDEX idx_mandaty_0 ON mandaty USING btree (pesel);


ALTER TABLE ONLY mandaty
    ADD CONSTRAINT fk_mandaty_kierowcy FOREIGN KEY (pesel) REFERENCES kierowcy(pesel);


ALTER TABLE ONLY mandaty
    ADD CONSTRAINT fk_mandaty_wykroczenia FOREIGN KEY (id_wykroczenia) REFERENCES wykroczenia(id);


ALTER TABLE ONLY ograniczenia_uprawnien
    ADD CONSTRAINT fk_ograniczenia_uprawnien FOREIGN KEY (pesel) REFERENCES kierowcy(pesel);


ALTER TABLE ONLY uprawnienia
    ADD CONSTRAINT fk_uprawnienia_kierowcy FOREIGN KEY (pesel) REFERENCES kierowcy(pesel);

