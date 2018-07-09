--ZAD18
----

--ZAD1
SELECT
  round(min(cena_netto * (1 + vat / 100)), 2) AS min,
  round(max(cena_netto * (1 + vat / 100)), 2) AS max
FROM produkty
ORDER BY 1;
----

--ZAD2
SELECT
  id_zamowienia,
  sum(ilosc)
FROM produkty_zamowienia
GROUP BY id_zamowienia
ORDER BY 1, 2;
----

--ZAD3
SELECT produkt
FROM produkty_zamowienia
GROUP BY produkt
HAVING sum(ilosc) >= 50
ORDER BY 1;
----

--ZAD4
SELECT
  kategoria,
  round(avg(waga), 2)
FROM produkty
GROUP BY kategoria
HAVING count(kod_produktu) >= 2
ORDER BY 2 DESC, 1;
----

--ZAD5
SELECT
  kategoria,
  min(cena_netto)                   AS min,
  max(cena_netto)                   AS max,
  max(cena_netto) - min(cena_netto) AS roznica
FROM produkty
GROUP BY kategoria
ORDER BY 1, 2, 3;
----

--ZAD6
SELECT
  kategoria,
  sum(CASE WHEN vat != 18
    THEN 1
      ELSE 0 END) AS "ilosc produktow"
FROM produkty
GROUP BY kategoria
ORDER BY 1, 2;
----

--ZAD7
SELECT count(kod_produktu)
FROM produkty
WHERE waga < 1000;
----

--ZAD8
SELECT sum(cena_netto) AS sum
FROM produkty
HAVING max(cena_netto) >= 300;
----

--ZAD9
SELECT CASE WHEN count(adres_dostawy) > count(DISTINCT adres_dostawy)
  THEN 'NIE'
       ELSE 'TAK' END
FROM zamowienia;
----

--ZAD10
SELECT id_klienta
FROM zamowienia
GROUP BY id_klienta
HAVING count(id_zamowienia) > 1
ORDER BY 1;
----

--ZAD11
SELECT kategoria
FROM produkty
GROUP BY kategoria
HAVING every(rabat NOTNULL)
ORDER BY 1;
----

--ZAD12
SELECT kategoria
FROM produkty
WHERE vat = 8
GROUP BY kategoria
HAVING every(rabat NOTNULL)
ORDER BY 1;
----

--ZAD13
SELECT
  kategoria,
  round(coalesce(variance(cena_netto), 0), 2) AS wariancja,
  round(coalesce(stddev(cena_netto), 0), 2)   AS "standardowe odchylenie"
FROM produkty
GROUP BY kategoria
ORDER BY 1, 2, 3;
----

--ZAD14
SELECT count(kod_produktu)
FROM produkty
WHERE kategoria = 'Artykuły papiernicze';
----

--ZAD15a
SELECT '{' || string_agg('"' || numer_telefonu || '"', ', ') || '}'
FROM klienci;
----

--ZAD15b
SELECT '{' || string_agg('"' || numer_telefonu || '"', ', '
ORDER BY numer_telefonu) || '}'
FROM klienci;
----

--ZAD16
SELECT
  produkt,
  count(id_zamowienia)
FROM produkty_zamowienia
GROUP BY produkt
ORDER BY 2 DESC, 1;
----

--ZAD17
SELECT
  CASE WHEN nip NOTNULL
    THEN 'Podany'
  ELSE 'Brak' END   AS "NIP",
  count(id_klienta) AS "ilosc klientow"
FROM klienci
GROUP BY nip NOTNULL;
----


--ZAD18
SELECT count(id_klienta)
FROM klienci
WHERE email LIKE '%@gmail.com';
----

--ZAD19
SELECT to_char(data_zlozenia, 'Month') as "Miesiac"
  , count(id_zamowienia) as "Ilosc zamowien"
FROM zamowienia
GROUP BY 1
ORDER BY 1, 2;

----