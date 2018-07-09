begin;

-- Pola data_od i data_od sa NOT NULL
update rabaty SET data_od = DATE'2000-01-01' where data_od is null;
update rabaty SET data_do = DATE'2015-01-01' where data_do is null;
alter table rabaty alter data_do set not null ;
alter table rabaty alter data_od set not null ;

-- Rabat kliencki przyznany za zamowienie dotyczy zawsze klienta skladajacego zamowienie
alter table rabaty_klienci drop CONSTRAINT rabaty_klienci_pkey;
alter table rabaty_klienci alter id_klienta drop not null ;

update rabaty_klienci rk set 
  id_klienta = (select id_klienta from zamowienia z where rk.id_zamowienia = z.id_zamowienia) 
where id_zamowienia is not null;

-- Pierwsza cena
insert into historia_cen (kod_produktu,data_wprowadzenia, cena_netto)
select kod_produktu, DATE'2000-01-01', 10 from produkty;

commit;
