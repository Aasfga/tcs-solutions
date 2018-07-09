--ZAD22
----


CREATE TABLE test(
  text VARCHAR(20)
);



--ZAD1
SELECT *
FROM zespoly
ORDER BY 1, 2, 3;
----

--ZAD2
SELECT *
FROM pracownicy
ORDER BY 1, 2, 3, 4, 5, 6, 7, 8, 9;
----

--ZAD3
SELECT nazwisko, etat, 12*placa_pod as Roczne_dochody
FROM pracownicy
ORDER BY 1, 2, 3;
----

--ZAD4
SELECT nazwisko, coalesce(placa_dod, 0) + pracownicy.placa_pod
FROM pracownicy
ORDER BY 1, 2;
----

--ZAD5
SELECT *
FROM zespoly
ORDER BY 2;
----

--ZAD6
SELECT etat
FROM pracownicy
GROUP BY etat
ORDER BY 1;
----

--ZAD7
SELECT *
FROM pracownicy
WHERE etat = 'ASYSTENT'
ORDER BY 1, 2, 3, 4, 5;
----

--ZAD8
SELECT *
FROM pracownicy
WHERE id_zesp BETWEEN 20 and 30
ORDER BY placa_pod DESC;
----

--ZAD9
SELECT *
FROM pracownicy
where placa_pod BETWEEN 300 and 1800
ORDER BY 1, 2, 3, 4;
----

--ZAD10
SELECT *
FROM pracownicy
WHERE nazwisko LIKE '%ski'
ORDER BY 1, 2, 3, 4, 5, 6;
----

--ZAD11
SELECT *
FROM pracownicy
WHERE placa_pod >= 2000 AND id_szefa NOTNULL
ORDER BY 1, 2, 3, 4, 5;
----

--ZAD12
SELECT nazwisko, zatrudniony, etat
FROM pracownicy
WHERE extract(YEAR from zatrudniony) BETWEEN 1992 and 1993
ORDER BY 1, 2, 3;
----

--ZAD13
SELECT *
FROM pracownicy
WHERE placa_dod >= 500
ORDER BY etat, nazwisko;
----

--ZAD14
SELECT
  nazwisko ||
  ' pracuje od ' ||
  zatrudniony ||
  ' i zarabia ' ||
  placa_pod AS "PROFESOROWIE"
FROM pracownicy
WHERE etat = 'PROFESOR'
ORDER BY placa_pod DESC;
----

--ZAD15
SELECT nazwisko, round(placa_pod * 1.15)
FROM pracownicy
ORDER BY 1, 2;
----

--ZAD16
SELECT
  nazwisko ||
  repeat('.', 40 - length(nazwisko) - length(etat)) ||
  etat AS "nazwisko i etat"
FROM pracownicy
ORDER BY id_prac;

----

--ZAD17
SELECT substring(etat, 0, 3) || id_prac::TEXT,
  id_prac,
  nazwisko,
  etat
FROM pracownicy
ORDER BY 1, 2, 3, 4;
----

--ZAD18
SELECT nazwisko, regexp_replace(nazwisko, '[KkLlMm]', 'X', 'g')
FROM pracownicy
ORDER BY 1, 2;
----

--ZAD19
SELECT nazwisko,
  initcap(to_char(zatrudniony, 'month')) ||
  repeat(' ', 9 - length(initcap(to_char(zatrudniony, 'month')) )) ||
  ', ' || to_char(zatrudniony, 'DD') || ' ' || to_char(zatrudniony, 'YYYY')
  AS data_zatrudnienia
FROM pracownicy
WHERE id_zesp = 20
ORDER BY id_prac;
----

--ZAD20
SELECT to_char(clock_timestamp(), 'Day') as "dzis";
----

--ZAD21
SELECT id_prac,
  CASE
    WHEN placa_pod < 1850 THEN 'mniej'
    WHEN placa_pod = 1850 THEN 'rowna'
    WHEN  placa_pod > 1850 THEN  'wiecej'
  END
FROM pracownicy
ORDER BY 1, 2;
----

--ZAD22
SELECT imie, nazwisko, placa_dod
FROM pracownicy
ORDER BY placa_dod DESC NULLS LAST , id_prac;
----
