--ZAD11
----

--ZAD1
SELECT
  produkty.nazwa,
  klienci.nazwa
FROM produkty
  CROSS JOIN klienci
WHERE produkty.kategoria LIKE 'Artykuły%' AND klienci.email NOTNULL
ORDER BY 1, 2;
----

--ZAD2
SELECT
  data_zlozenia,
  nazwa
FROM klienci
  NATURAL JOIN zamowienia
WHERE nip NOTNULL
ORDER BY 1, 2;
----

--ZAD3
SELECT DISTINCT
  nazwa
FROM zamowienia
  JOIN produkty_zamowienia
  USING (id_zamowienia)
  JOIN produkty
    ON produkt = produkty.kod_produktu
WHERE adres_dostawy LIKE 'ul. Kubusia Puchatka%'
ORDER BY 1;
----

--ZAD4
SELECT
  klienci.nazwa  AS "nazwa klienta",
  CASE WHEN count(DISTINCT adres_dostawy) > 1
    THEN 'TAK'
  ELSE 'NIE' END AS "rozne adresy"
FROM
  klienci
  JOIN zamowienia
  USING (id_klienta)
GROUP BY id_klienta, klienci.nazwa
ORDER BY 1, 2;
----

--ZAD5
SELECT
  first.nazwa,
  second.nazwa
FROM
  produkty AS first
  CROSS JOIN produkty AS second
WHERE first.cena_netto * (1 + first.vat / 100) > second.cena_netto * (1 + second.vat / 100)
ORDER BY 1, 2;
----

--ZAD6
SELECT DISTINCT
  nazwa,
  email
FROM klienci
  NATURAL JOIN zamowienia
WHERE extract(MONTH FROM data_zlozenia) = 3
ORDER BY 1, 2;
----

--ZAD7a
SELECT
  rabatobiorcy.nazwa AS rabatobiorca,
  klienci.nazwa      AS rabatodawca
FROM
  klienci
  JOIN rabaty_klientow
    ON klienci.id_klienta = rabaty_klientow.id_polecajacego
  JOIN klienci AS rabatobiorcy
    ON rabatobiorcy.id_klienta = rabaty_klientow.id_klienta
ORDER BY 1, 2;
----

--ZAD7b
SELECT
  rabatobiorcy.nazwa AS rabatobiorca,
  klienci.nazwa      AS rabatodawca
FROM
  klienci
  RIGHT JOIN rabaty_klientow
    ON klienci.id_klienta = rabaty_klientow.id_polecajacego
  JOIN klienci AS rabatobiorcy
    ON rabatobiorcy.id_klienta = rabaty_klientow.id_klienta
ORDER BY 1, 2;
----

--ZAD8
SELECT
  klienci.*
FROM
  klienci
  LEFT OUTER JOIN rabaty_klientow
  USING (id_klienta)
WHERE id_rabatu ISNULL
ORDER BY 1, 2, 3;
----

--ZAD9
SELECT
  nazwa
FROM
  produkty
  JOIN rabaty_produktow
    ON produkt = kod_produktu
WHERE min_ilosc = 1
ORDER BY 1;
----

--ZAD10
SELECT
  klienci.*,
  id_zamowienia,
  sum(cena_netto * ilosc) AS wartosc_zamowienia
FROM produkty_zamowienia
  JOIN produkty
    ON kod_produktu = produkt
  JOIN zamowienia
  USING (id_zamowienia)
  JOIN klienci
  USING (id_klienta)
WHERE klienci.nazwa = 'Urząd Skarbowy w Radomiu'
GROUP BY klienci.id_klienta, id_zamowienia
ORDER BY wartosc_zamowienia DESC, id_zamowienia;
----

--ZAD11
SELECT
  DISTINCT
  id_zamowienia
FROM
  zamowienia
  JOIN rabaty_klientow
  USING (id_klienta)
ORDER BY 1;
----

--ZAD12
SELECT
  nazwa               AS klient,
  sum(CASE WHEN ilosc ISNULL
    THEN 0
      ELSE ilosc END) AS "ilosc produtkow"
FROM
  produkty_zamowienia
  JOIN zamowienia
  USING (id_zamowienia)
  RIGHT JOIN klienci
  USING (id_klienta)
GROUP BY klient
ORDER BY 1, 2;
----

--ZAD13
SELECT
  klienci.nazwa                               AS "klient",
  sum(coalesce(produkty_zamowienia.ilosc, 0)) AS "ilosc produktow"
FROM
  produkty_zamowienia
  NATURAL JOIN zamowienia
  JOIN produkty
    ON kod_produktu = produkt AND waga > 1000
  JOIN klienci
  USING (id_klienta)
GROUP BY id_klienta, klient
ORDER BY 1, 2;
----

SELECT
  p.nazwa                     AS "klient",
  sum(coalesce((r.ilosc), 0)) AS "ilosc produktow"
FROM klienci AS p
  JOIN zamowienia AS q
  USING (id_klienta)
  JOIN produkty_zamowienia AS r
  USING (id_zamowienia)
  JOIN produkty
    ON (r.produkt = produkty.kod_produktu)
WHERE produkty.waga > 1000
GROUP BY p.id_klienta
ORDER BY 1;

--ZAD14
SELECT
  klienci.nazwa        AS klient,
  count(id_zamowienia) AS "ilosc zamowien"
FROM
  klienci
  LEFT JOIN zamowienia
  USING (id_klienta)
GROUP BY id_klienta, klient
ORDER BY 2 DESC, 1;
----

--ZAD15
SELECT
  id_rabatu,
  znizka,
  CASE WHEN id_klienta ISNULL
    THEN 'RABAT PRODUKTU'
  ELSE 'RABAT KLIENCKI'
  END AS "typ rabatu"
FROM
  rabaty
  FULL JOIN rabaty_produktow
  USING (id_rabatu)
  FULL JOIN rabaty_klientow
  USING (id_rabatu)
ORDER BY 1, 2, 3;
----

--ZAD16
SELECT
  CASE WHEN bool_or(first.id_klienta = second.id_polecajacego)
    THEN 'TAK'
  ELSE 'NIE'
  END
FROM
  rabaty_klientow AS first
  JOIN rabaty_klientow AS second
    ON first.id_polecajacego = second.id_klienta;
----

--ZAD17
SELECT
  id_zamowienia,
  max(ilosc)
FROM
  zamowienia
  JOIN produkty_zamowienia
  USING (id_zamowienia)
GROUP BY id_zamowienia
ORDER BY 2 DESC, 1;
----

