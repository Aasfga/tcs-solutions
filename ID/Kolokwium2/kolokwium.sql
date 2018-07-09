--ZAD4
----


--ZAD1
CREATE OR REPLACE FUNCTION collatzFun(n BIGINT)
  RETURNS BIGINT AS
$$
BEGIN
  IF n % 2 = 0
  THEN
    RETURN n / 2;
  ELSE
    RETURN 3 * n + 1;
  END IF;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION collatz(n INTEGER)
  RETURNS INTEGER AS
$$
DECLARE
  i INTEGER := 0;
  m BIGINT := n :: BIGINT;
BEGIN
  WHILE m != 1
  LOOP
    m := collatzFun(m);
    i = i + 1;
  END LOOP;

  RETURN i;
END;
$$
LANGUAGE plpgsql;
----


--ZAD2
CREATE OR REPLACE VIEW punkty_info AS
  SELECT
    pesel       AS pesel_kierowcy,
    sum(punkty) AS suma_punktow
  FROM
    kierowcy
    LEFT JOIN mandaty
    USING (pesel)
    JOIN wykroczenia
      ON mandaty.id_wykroczenia = wykroczenia.id
  WHERE punkty != 0 AND data > now() - INTERVAL '1 year'
  GROUP BY pesel;
----

--ZAD3
CREATE OR REPLACE FUNCTION wstrzymane_uprawnienia(d DATE)
  RETURNS TABLE(p CHAR(11), i VARCHAR, n VARCHAR, o DATE) AS
$$
BEGIN
  RETURN QUERY SELECT
                 DISTINCT ON (pesel)
                 pesel,
                 imie,
                 nazwisko,
                 od
               FROM
                 kierowcy
                 JOIN ograniczenia_uprawnien
                 USING (pesel)
               WHERE od <= d AND ograniczenia_uprawnien.do >= d
               ORDER BY pesel, ograniczenia_uprawnien."do" DESC;
END;
$$
LANGUAGE plpgsql;
----

--ZAD4
CREATE OR REPLACE FUNCTION punkty()
  RETURNS TRIGGER AS
$punkty$
BEGIN
  IF 24 < (SELECT
             suma_punktow
           FROM punkty_info
           WHERE pesel_kierowcy = new.pesel)
  THEN
    INSERT INTO ograniczenia_uprawnien VALUES (new.pesel, new.data, new.data + INTERVAL '3 months', NULL);
  END IF;

  RETURN new;
END;
$punkty$
LANGUAGE plpgsql;

CREATE TRIGGER punkty
AFTER INSERT ON mandaty
FOR EACH ROW
EXECUTE PROCEDURE punkty();
----



