--ZAD14
----


--ZAD1
CREATE SEQUENCE seq START 10 INCREMENT 1 MAXVALUE 99;
----

--ZAD2
CREATE SEQUENCE seq START 10 INCREMENT 1 MAXVALUE 99;
INSERT INTO klienci VALUES (nextval('seq'), 'Anna', 'Nowak');
INSERT INTO klienci VALUES (nextval('seq'), 'Jan', 'Kowalski');
----

--ZAD3
CREATE UNIQUE INDEX ON transakcje (z_konta, na_konto, data_zlecenia);
----

--ZAD4
CREATE OR REPLACE VIEW wplaty_wyplaty AS
  SELECT
    konta.nr_konta AS "Konto",
    count(CASE WHEN z_konta = konta.nr_konta
      THEN 1 END)  AS "Ilosc wyplat",
    count(CASE WHEN na_konto = konta.nr_konta
      THEN 1 END)  AS "Ilosc wplat"
  FROM
    konta
    CROSS JOIN transakcje
  GROUP BY nr_konta;
----

--ZAD5
CREATE OR REPLACE VIEW wplaty_wyplaty AS
  SELECT
    konta.nr_konta AS "Konto",
    count(CASE WHEN z_konta = konta.nr_konta
      THEN 1 END)  AS "Ilosc wyplat",
    count(CASE WHEN na_konto = konta.nr_konta
      THEN 1 END)  AS "Ilosc wplat"
  FROM
    konta
    CROSS JOIN transakcje
  GROUP BY nr_konta;


SELECT
  *
FROM wplaty_wyplaty;

INSERT INTO transakcje VALUES
  (200, now(), NULL, 1004, 500.00);

SELECT
  *
FROM wplaty_wyplaty;

DROP VIEW wplaty_wyplaty;
----

--ZAD6
CREATE OR REPLACE FUNCTION oblicz_koszt(x NUMERIC(11, 2))
  RETURNS NUMERIC(11, 2) AS
$$
BEGIN
  RETURN round(x * 0.02, 2);
END
$$
LANGUAGE plpgsql;
----

--ZAD7
CREATE OR REPLACE FUNCTION oblicz_koszt(x NUMERIC(11, 2))
  RETURNS NUMERIC(11, 2) AS
$$
BEGIN
  RETURN round(x * 0.02, 2);
END
$$
LANGUAGE plpgsql;

SELECT
  oblicz_koszt(kwota)
FROM transakcje;
----

--ZAD8
CREATE OR REPLACE FUNCTION bilans_kont()
  RETURNS
    TABLE
    (
      konto       NUMERIC(11),
      suma_wplat  NUMERIC(11, 2),
      suma_wyplat NUMERIC(11, 2)
    ) AS
$$
BEGIN
  RETURN QUERY
  SELECT
    nr_konta,
    coalesce(sum_wyplaty, 0),
    coalesce(sum_wplaty, 0)
  FROM
    konta
    LEFT JOIN (SELECT
                 z_konta,
                 sum(kwota) AS sum_wplaty
               FROM transakcje
               GROUP BY z_konta) AS wyplaty
      ON z_konta = konta.nr_konta
    LEFT JOIN (SELECT
                 na_konto,
                 sum(kwota) AS sum_wyplaty
               FROM transakcje
               GROUP BY na_konto) AS wplaty
      ON na_konto = konta.nr_konta;
END;
$$
LANGUAGE plpgsql;
----

--ZAD9
CREATE OR REPLACE FUNCTION bilans_kont()
  RETURNS
    TABLE
    (
      konto       NUMERIC(11),
      suma_wplat  NUMERIC(11, 2),
      suma_wyplat NUMERIC(11, 2)
    ) AS
$$
BEGIN
  RETURN QUERY
  SELECT
    nr_konta,
    coalesce(sum_wyplaty, 0),
    coalesce(sum_wplaty, 0)
  FROM
    konta
    LEFT JOIN (SELECT
                 z_konta,
                 sum(kwota) AS sum_wplaty
               FROM transakcje
               GROUP BY z_konta) AS wyplaty
      ON z_konta = konta.nr_konta
    LEFT JOIN (SELECT
                 na_konto,
                 sum(kwota) AS sum_wyplaty
               FROM transakcje
               GROUP BY na_konto) AS wplaty
      ON na_konto = konta.nr_konta;
END;
$$
LANGUAGE plpgsql;

SELECT
  konto,
  sum(suma_wplat) - sum(suma_wyplat) AS bilans
FROM bilans_kont()
GROUP BY konto;
----

--ZAD10a
CREATE OR REPLACE FUNCTION silnia(x NUMERIC)
  RETURNS NUMERIC AS
$$
DECLARE
  a NUMERIC;
BEGIN
  a := 1;
  FOR i IN 1..x LOOP
    a := a * i;
  END LOOP;

  RETURN a;
END;
$$
LANGUAGE plpgsql;
----


--ZAD10b
CREATE OR REPLACE FUNCTION silnia(x NUMERIC)
  RETURNS NUMERIC AS
$$
BEGIN
  IF x <= 0
  THEN
    RETURN 1;
  ELSE
    RETURN x * silnia(x - 1);
  END IF;

END;
$$
LANGUAGE plpgsql;
----


--ZAD11
CREATE OR REPLACE FUNCTION bilans_kont()
  RETURNS
    TABLE
    (
      konto       NUMERIC(11),
      suma_wplat  NUMERIC(11, 2),
      suma_wyplat NUMERIC(11, 2)
    ) AS
$$
BEGIN
  RETURN QUERY
  SELECT
    nr_konta,
    coalesce(sum_wyplaty, 0),
    coalesce(sum_wplaty, 0)
  FROM
    konta
    LEFT JOIN (SELECT
                 z_konta,
                 sum(kwota) AS sum_wplaty
               FROM transakcje
               GROUP BY z_konta) AS wyplaty
      ON z_konta = konta.nr_konta
    LEFT JOIN (SELECT
                 na_konto,
                 sum(kwota) AS sum_wyplaty
               FROM transakcje
               GROUP BY na_konto) AS wplaty
      ON na_konto = konta.nr_konta;
END;
$$
LANGUAGE plpgsql;

CREATE SEQUENCE seq START 1000 INCREMENT 10 MAXVALUE 5000;

CREATE OR REPLACE FUNCTION bonus_swiateczny(p NUMERIC DEFAULT 0.01)
  RETURNS VOID AS
$$
BEGIN
  INSERT INTO transakcje (id, data_zlecenia, z_konta, na_konto, kwota)
    SELECT
      nextval('seq'),
      now(),
      NULL,
      konto,
      p * suma_wyplat
    FROM bilans_kont() AS bilans;
END;
$$
LANGUAGE plpgsql;
----


--ZAD12
CREATE OR REPLACE FUNCTION stan_konta(konto NUMERIC(11), t TIMESTAMP)
  RETURNS NUMERIC(11, 2) AS
$$
DECLARE
  ans NUMERIC := 0;
    sel CURSOR FOR SELECT
                     val
                   FROM
                     (SELECT
                        CASE WHEN transakcje.na_konto = konto
                          THEN kwota
                        ELSE -kwota END AS val
                      FROM transakcje
                      WHERE (konto = transakcje.na_konto OR konto = transakcje.z_konta) AND
                            transakcje.data_zlecenia <= t) AS sel;
BEGIN
  FOR temprow IN sel
  LOOP
    ans := ans + temprow.val;
    IF (ans < 0)
    THEN
      RAISE 'Wykryto ujemny bilans konta';
    END IF;
  END LOOP;
  RETURN ans;
END;
$$
LANGUAGE plpgsql;
----

--ZAD13
CREATE OR REPLACE FUNCTION stan_konta(konto NUMERIC(11), t TIMESTAMP)
  RETURNS NUMERIC(11, 2) AS
$$
DECLARE
  ans NUMERIC := 0;
    sel CURSOR FOR SELECT
                     val
                   FROM
                     (SELECT
                        CASE WHEN transakcje.na_konto = konto
                          THEN kwota
                        ELSE -kwota END AS val
                      FROM transakcje
                      WHERE (konto = transakcje.na_konto OR konto = transakcje.z_konta) AND
                            transakcje.data_zlecenia <= t) AS sel;
BEGIN
  FOR temprow IN sel
  LOOP
    ans := ans + temprow.val;
    IF (ans < 0)
    THEN
      RAISE 'Wykryto ujemny bilans konta';
    END IF;
  END LOOP;
  RETURN ans;
END;
$$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION historia_konta(konto NUMERIC(11))
  RETURNS TABLE
  (
    data TIMESTAMP,
    stan NUMERIC(11)
  ) AS
$$
BEGIN

  RETURN QUERY (
    SELECT
      transakcje.data_zlecenia                      AS data,
      stan_konta(konto, data_zlecenia :: TIMESTAMP) AS stan
    FROM transakcje
    WHERE konto = transakcje.na_konto OR konto = transakcje.z_konta);
END;
$$
LANGUAGE plpgsql;
----


--ZAD14
CREATE OR REPLACE FUNCTION stan_konta(konto NUMERIC(11), t TIMESTAMP)
  RETURNS NUMERIC(11, 2) AS
$$
DECLARE
  ans NUMERIC := 0;
    sel CURSOR FOR SELECT
                     val
                   FROM
                     (SELECT
                        CASE WHEN transakcje.na_konto = konto
                          THEN kwota
                        ELSE -kwota END AS val
                      FROM transakcje
                      WHERE (konto = transakcje.na_konto OR konto = transakcje.z_konta) AND
                            transakcje.data_zlecenia <= t) AS sel;
BEGIN
  FOR temprow IN sel
  LOOP
    ans := ans + temprow.val;
    IF (ans < 0)
    THEN
      RAISE 'Wykryto ujemny bilans konta';
    END IF;
  END LOOP;
  RETURN ans;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION moment_rozspojniajacy()
  RETURNS TIMESTAMP AS
$$
DECLARE
    les CURSOR FOR SELECT
                     data_zlecenia,
                     z_konta
                   FROM transakcje;
  ans  NUMERIC;
  czas TIMESTAMP;
BEGIN
  FOR temprow IN les
  LOOP
    czas := temprow.data_zlecenia :: TIMESTAMP;
    IF temprow.z_konta IS NOT NULL
    THEN
      ans := stan_konta(temprow.z_konta, temprow.data_zlecenia :: TIMESTAMP);
    END IF;
  END LOOP;
  RETURN NULL;
  EXCEPTION
  WHEN OTHERS
    THEN RETURN czas;
END;
$$
LANGUAGE plpgsql;
----