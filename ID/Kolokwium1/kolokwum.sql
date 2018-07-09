--ZAD5
----


--ZAD1
SELECT DISTINCT ON (semestr, nazwa)
  semestr,
  nazwa
FROM
  zajecia
  JOIN kursy
  USING (id_kurs);
----


--ZAD2
SELECT
  imie,
  nazwisko,
  count(id_zajecia)
FROM
  studenci
  JOIN zapisani
  USING (id_student)
  JOIN zajecia
  USING (id_zajecia)
  JOIN kursy
  USING (id_kurs)
WHERE kursy.rodzaj = 'wyk'
GROUP BY imie, nazwisko
ORDER BY 1, 2, 3;
----

--ZAD3
SELECT
  id_zajecia,
  (count(ocena) :: FLOAT / ile :: FLOAT) :: NUMERIC(3, 2) AS srednia
FROM
  ankiety
  RIGHT JOIN zajecia
  USING (id_zajecia)
  JOIN (SELECT
          id_zajecia,
          count(id_student) AS ile
        FROM zapisani
        GROUP BY id_zajecia) AS howMany
  USING (id_zajecia)
GROUP BY id_zajecia, ile
HAVING count(ocena) * 2 < ile
ORDER BY 1, 2;
----

--ZAD4
SELECT
  imie || ' ' || nazwisko,
  avg(coalesce(ocena, 0)) :: NUMERIC(3, 2) AS srednia
FROM
  zajecia
  JOIN ankiety
  USING (id_zajecia)
  RIGHT JOIN prowadzacy
  USING (id_prowadzacy)
GROUP BY imie, nazwisko
ORDER BY srednia DESC, nazwisko, imie;
----

--ZAD5
SELECT
  string_agg(imie || ' ' || nazwisko, ', '),
  srednia
FROM
  (SELECT
  imie,
  nazwisko,
  avg(coalesce(ocena, 0)) :: NUMERIC(3, 2) AS srednia
FROM
  zajecia
  JOIN ankiety
  USING (id_zajecia)
  RIGHT JOIN prowadzacy
  USING (id_prowadzacy)
GROUP BY imie, nazwisko
ORDER BY srednia DESC, nazwisko, imie) as subSelect
GROUP BY subSelect.srednia
ORDER BY 1 DESC ;
----

