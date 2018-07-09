--ZAD9
----


--ZAD1
SELECT
  nazwa
FROM produkty
WHERE id_kategoria = (SELECT
                        id_kategoria
                      FROM produkty
                      WHERE nazwa = 'Piórnik duży');
----

--ZAD2
SELECT
  nazwa
FROM produkty
WHERE (id_kategoria) IN (SELECT
                           id_kategoria
                         FROM kategorie
                         WHERE nadkategoria NOTNULL);
----

--ZAD3a
SELECT
  nazwa
FROM kategorie AS first
WHERE (SELECT
         count(kod_produktu)
       FROM produkty
       WHERE produkty.id_kategoria = first.id_kategoria) >= 3;
----

--ZAD3b
SELECT
  (SELECT
     kategorie.nazwa
   FROM
     produkty
     JOIN kategorie
     USING (id_kategoria)
   GROUP BY id_kategoria, kategorie.nazwa
   HAVING count(kod_produktu) >= 3);
;
----

--ZAD3c
SELECT
  kategorie.nazwa
FROM
  produkty
  JOIN kategorie
  USING (id_kategoria)
GROUP BY id_kategoria, kategorie.nazwa
HAVING count(kod_produktu) >= 3;

----

--ZAD4
SELECT
  *
FROM rabaty
WHERE data_do - rabaty.data_od = (SELECT
                                    max(data_do - data_od)
                                  FROM rabaty);
----

--ZAD5
SELECT
  *
FROM kategorie AS first
WHERE id_kategoria = ANY (SELECT
                            nadkategoria
                          FROM kategorie
                          WHERE vat > first.vat);
----

--ZAD6
SELECT
  id_zamowienia
FROM
  produkty_zamowienia
GROUP BY id_zamowienia
HAVING sum(ilosc) >= ALL (SELECT
                            sum(ilosc)
                          FROM produkty_zamowienia
                          GROUP BY id_zamowienia);
----

--ZAD7
SELECT
  nazwa,
  howManyChanges
FROM
  produkty
  JOIN (SELECT
          kod_produktu,
          count(data_wprowadzenia) AS howManyChanges
        FROM historia_cen
        WHERE extract(MONTH FROM data_wprowadzenia) = 4
        GROUP BY kod_produktu) AS countChanges
  USING (kod_produktu)
WHERE howManyChanges >= 2;
----

--ZAD9
SELECT
  produkty.nazwa, cena_netto * (1 + vat/100)
FROM
  produkty
  JOIN historia_cen
    USING (kod_produktu)
  JOIN kategorie
    USING (id_kategoria)
  JOIN (SELECT
          kod_produktu,
          max(data_wprowadzenia) as maxData
        FROM historia_cen
        GROUP BY kod_produktu) AS max_data
    ON max_data.kod_produktu = produkty.kod_produktu AND data_wprowadzenia = max_data.maxData
----