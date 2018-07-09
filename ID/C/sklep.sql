drop table if exists Produkty cascade;
create table Produkty (
	kod_produktu numeric(6) constraint pk_prod primary key,
	nazwa varchar(250) not null,
	cena_netto numeric(6,2) not null,
	vat numeric(3,1) not null,
	waga numeric(6) not null,
	kategoria varchar(250) not null

	check (vat >= 0),
	check (cena_netto >= 0)
);

drop table if exists Klienci cascade;
create table Klienci (
	id_klienta numeric(6) constraint pk_kli primary key,
	nazwa varchar(250) not null,
	numer_telefonu varchar(25), -- opcjonalny,
	email varchar(100), -- opcjonalny
	nip char(10) -- opcjonalny
	
	check (numer_telefonu is not null or email is not null)
);

drop table if exists Zamowienia cascade;
create table Zamowienia (
	id_zamowienia numeric(8) constraint pk_zam primary key,
	id_klienta numeric(6) constraint fk_zam_kli references Klienci(id_klienta) not null,
	adres_dostawy varchar(500) not null,
	data_zlozenia date not null default now()
);

drop table if exists Produkty_Zamowienia cascade;
create table Produkty_Zamowienia (
	produkt numeric(6) constraint fk_prod_zam references Produkty(kod_produktu) not null,
	id_zamowienia numeric(8) constraint fk_zam_prod references Zamowienia(id_zamowienia) not null,
	ilosc numeric(8) not null

	check (ilosc > 0),
	primary key (produkt, id_zamowienia)
);

-- Bazowa tabela dostępnych rabatów
-- Znizka stanowi procentowy upust na zamowienie.
drop table if exists Rabaty cascade;
create table Rabaty (
	id_rabatu numeric(6) constraint pk_rab primary key,
	znizka numeric(3,1) not null
);

-- Tabela rabatów dla stałych klientów.
-- Rabaty dla stałych klientów są aplikowane do każdego zamówienia klienta.
drop table if exists Rabaty_Klientow cascade;
create table Rabaty_Klientow (
	id_rabatu numeric(6) constraint fk_rabkli_rab references Rabaty(id_rabatu) not null,
	id_klienta numeric(6) constraint fk_zam_kli references Klienci(id_klienta) not null,
	id_polecajacego numeric(6) constraint fk_zam_pol references Klienci(id_klienta),

	check (id_polecajacego is null or id_klienta != id_polecajacego),
	primary key (id_rabatu, id_klienta)
);

-- Tabela rabatów na poszczególne produkty.
-- Rabaty na produkty są aplikowane gdy zamówienie zostanie dokonane w odpowiedniej ilosci lub wadze.
drop table if exists Rabaty_Produktow cascade;
create table Rabaty_Produktow (
	id_rabatu numeric(6) constraint fk_rabprod_rab references Rabaty(id_rabatu) not null,
	produkt numeric(6) constraint fk_rabprod_prod references Produkty(kod_produktu) not null,
	min_ilosc numeric(6),
	min_waga numeric(6)

	check (min_ilosc is null or min_ilosc > 0),
	check (min_waga is null or min_waga > 0),
	check (min_ilosc is not null or min_waga is not null),
	primary key (id_rabatu, produkt)
);

insert into Produkty values
	(101, 'Gilotyna do papieru', 219.99, 18.0, 3500, 'Gilotyny i trymery'),
	(102, 'Dziurkacz metalowy duży', 59.99, 18.0, 1300, 'Dziurkacze'),
	(103, 'Dziurkacz metalowy mały', 29.99, 18.0, 700, 'Dziurkacze'),
	(201, 'Identyfikator personalny', 6.40, 8.0, 350, 'Identyfikatory i pieczatki'),
	(202, 'Identyfikator magnetyczny', 54.99, 8.0, 320, 'Identyfikatory i pieczatki'),
	(501, 'Pieczatka imienna', 12.99, 18.0, 400, 'Identyfikatory i pieczatki'),
	(601, 'Spinacze biurowe 1000 sztuk', 8.90, 0.0, 200, 'Spinacze i zszywacze'),
	(603, 'Zszywacz duży', 69.00, 0.0, 2700, 'Spinacze i zszywacze'),
	(701, 'Piórnik duży', 12.00, 0.0, 500, 'Inne'),
	(801, 'Wizytownik skórzany duży', 36.00, 8.0, 1200, 'Artykuły papiernicze'),
	(802, 'Notatnik A6', 11.0, 18.0, 800, 'Artykuły papiernicze'),
	(803, 'Czarna mata na biurko', 19.0, 22.0, 2500, 'Inne'),
	(901, 'Ołówki z gumką 15 sztuk', 15.99, 0.0, 360, 'Artykuły piśmiennicze'),
	(902, 'Gumki do mazania sztuk 3', 8.99, 18.0, 470, 'Artykuły piśmiennicze'),
	(301, 'Kasetka metalowa mała', 85.99, 18.0, 1000, 'Inne'),
	(401, 'Nożyczki uniwersalne 21cm', 29.99, 0.0, 200, 'Nożyczki'),
	(502, 'Przybornik drewniany na biurko', 109.00, 22.0, 2700, 'Inne'),
	(602, 'Taśma klejąca akrylowa 50 sztuk', 49.68, 8.0, 650, 'Inne'),
	(702, 'Długopis niebieski 3 sztuki', 8.00, 18.0, 30, 'Artykuły piśmiennicze'),
	(1001, 'Papier biurowy A4 ryza', 13.99, 18.0, 3600, 'Artykuły papiernicze');

insert into Klienci (id_klienta, nazwa, numer_telefonu, email) values
	(101, 'Mariusz Nowak', '(+48) 505 23 22 12', 'mnowak@gmail.com');

insert into Klienci (id_klienta, nazwa, numer_telefonu) values
	(102, 'Joanna Stępień', '(+48) 32 256 66 97');

insert into Klienci (id_klienta, nazwa, email) values
	(103, 'Wiesław Kowalski', 'wiesioo@onet.eu'),
	(104, 'Stanisława Kopeć', 'stanislawa.kopec@gmail.com');

insert into Klienci (id_klienta, nazwa, numer_telefonu, nip) values
	(201, 'II Liceum Ogólnokształcące im. Tadeusza Kościuszki w Pszczółkach', '(+48) 22 253 33 89', 8743407200),
	(202, 'Biurex Polska z.o.o', '(+48) 516 88 12 32', 6885475742);

insert into Klienci (id_klienta, nazwa, numer_telefonu, email, nip) values
	(203, 'TaxMax z.o.o', '(+48) 11 768 23 99', 'biuro@taxmax.com', 1421697789),
	(204, 'Urząd Skarbowy w Radomiu', '(+48) 786 98 81 22', 'us@us.radom.pl', 8235014581),
	(205, 'BetonExp Industrial', '(+48) 723 88 97 11', 'kontakt@betonexp.pl', 7142321628);

insert into Zamowienia (id_zamowienia, id_klienta, adres_dostawy, data_zlozenia) values
	(101, 101, 'ul. Kubusia Puchatka 1/13, Warszawa', to_date('10-01-2001', 'DD-MM-YYYY')),
	(102, 103, 'ul. Adama Mickiewicza 17/21, Katowice', to_date('07-01-2001', 'DD-MM-YYYY')),
	(103, 103, 'ul. Adama Mickiewicza 17/21, Katowice', to_date('22-02-2001', 'DD-MM-YYYY')),
	(104, 102, 'ul. Mieczysława Karłowicza 13, Gdynia', to_date('14-03-2001', 'DD-MM-YYYY')),
	(105, 104, 'ul. Miarki 2, Bytom', to_date('21-02-2001', 'DD-MM-YYYY')),
	(106, 201, 'ul. Wesoła 7, Pszczółki', to_date('14-03-2001', 'DD-MM-YYYY')),
	(107, 201, 'ul. Wesoła 7, Pszczółki', to_date('14-04-2001', 'DD-MM-YYYY')),
	(108, 201, 'ul. Wesoła 7, Pszczółki', to_date('07-08-2001', 'DD-MM-YYYY')),
	(109, 204, 'ul. Krakowska 8, Radom', to_date('18-03-2001', 'DD-MM-YYYY')),
	(110, 204, 'ul. Krakowska 8, Radom', to_date('26-03-2001', 'DD-MM-YYYY')),
	(111, 203, 'ul. Bracka 1, Kraków', to_date('01-02-2001', 'DD-MM-YYYY')),
	(112, 203, 'ul. Nowohucka 19, Kraków', to_date('03-01-2001', 'DD-MM-YYYY')),
	(113, 202, 'ul. Studencka 7, Poznań', to_date('17-03-2001', 'DD-MM-YYYY')),
	(114, 205, 'ul. Starogardzka 7, Łódź', to_date('18-03-2001', 'DD-MM-YYYY')),
	(115, 205, 'ul. Starogardzka 7, Łódź', to_date('19-03-2001', 'DD-MM-YYYY')),
	(116, 205, 'ul. Długa 2, Łódź', to_date('20-04-2001', 'DD-MM-YYYY')),
	(117, 205, 'ul. Długa 2, Łódź', to_date('27-05-2001', 'DD-MM-YYYY'));

insert into Produkty_Zamowienia (produkt, id_zamowienia, ilosc) values 
	(701, 101, 1), (802, 101, 2), (702, 101, 1),
	(501, 102, 10), (602, 103, 15), (603, 103, 1),
	(702, 104, 20), (803, 104, 1), (601, 104, 1), (902, 104, 2),
	(102, 105, 3), (603, 106, 5), (102, 106, 10), (1001, 106, 40),
	(1001, 107, 40), (902, 107, 20), (501, 107, 35),
	(801, 108, 3), (101, 108, 1), (1001, 109, 50), (702, 109, 100),
	(602, 109, 60), (301, 109, 3), (802, 110, 45), (601, 110, 25),
	(802, 111, 75), (602, 112, 10), (701, 112, 3), (201, 112, 250),
	(202, 113, 350), (201, 113, 350), (902, 114, 20), (702, 114, 20),
	(501, 114, 35), (101, 115, 5), (102, 115, 5), (1001, 116, 45),
	(901, 117, 250), (902, 117, 250), (702, 117, 1000), (603, 117, 15);

insert into Rabaty values 
	(101, 10), (102, 25), (103, 15), (104, 5), 
	(105, 5), (106, 75), (107, 25),
	(108, 10), (109, 10), (201, 15);

insert into Rabaty_Klientow (id_rabatu, id_klienta) values
	(109, 201);

insert into Rabaty_Klientow (id_rabatu, id_klienta, id_polecajacego) values
	(105, 204, 205), (108, 205, 104);

insert into Rabaty_Produktow (id_rabatu, produkt, min_ilosc) values
	(101, 101, 1), (102, 701, 3), (103, 602, 15), (104, 1001, 3), (201, 501, 5);

insert into Rabaty_Produktow (id_rabatu, produkt, min_waga) values
	(106, 601, 5000), (107, 702, 1500);