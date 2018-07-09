--ZAD9
----


--ZAD1
CREATE OR REPLACE FUNCTION pesel_check()
  RETURNS TRIGGER AS $pesel_check$
DECLARE
  q   INTEGER := 9;
  i   INTEGER := 1;
  sum INTEGER := 0;
BEGIN
  IF (length(new.pesel) != 11)
  THEN
    RAISE EXCEPTION 'Niepoprawny PESEL';
  END IF;


  sum := sum + 9 * cast(substr(new.pesel, 1, 1) AS INTEGER);
  sum := sum + 7 * cast(substr(new.pesel, 2, 1) AS INTEGER);
  sum := sum + 3 * cast(substr(new.pesel, 3, 1) AS INTEGER);
  sum := sum + 1 * cast(substr(new.pesel, 4, 1) AS INTEGER);
  sum := sum + 9 * cast(substr(new.pesel, 5, 1) AS INTEGER);
  sum := sum + 7 * cast(substr(new.pesel, 6, 1) AS INTEGER);
  sum := sum + 3 * cast(substr(new.pesel, 7, 1) AS INTEGER);
  sum := sum + 1 * cast(substr(new.pesel, 8, 1) AS INTEGER);
  sum := sum + 9 * cast(substr(new.pesel, 9, 1) AS INTEGER);
  sum := sum + 7 * cast(substr(new.pesel, 10, 1) AS INTEGER);

  IF (cast(substr(new.pesel, 11, 1) AS INTEGER) != sum % 10)
  THEN
    RAISE EXCEPTION 'Niepoprawny PESEL';
  END IF;
  RETURN new;
END;
$pesel_check$ LANGUAGE plpgsql;


CREATE TRIGGER pesel_check
BEFORE INSERT OR UPDATE ON pacjenci
FOR EACH ROW EXECUTE PROCEDURE pesel_check();
----

--ZAD2
CREATE OR REPLACE FUNCTION end_date()
  RETURNS TRIGGER AS
$end_date$
BEGIN
  IF (new.data_zakonczenia ISNULL)
  THEN
    new.data_zakonczenia := new.data_rozpoczecia + INTERVAL '30 minutes';
  END IF;

  RETURN new;
END;
$end_date$
LANGUAGE plpgsql;

CREATE TRIGGER end_date
BEFORE INSERT OR UPDATE ON wizyty
FOR EACH ROW EXECUTE PROCEDURE end_date();
----

--ZAD3
CREATE OR REPLACE FUNCTION add_row()
  RETURNS TRIGGER AS
$del_row$
BEGIN
  IF (old.data_rozpoczecia + INTERVAL '5 years' >= now())
  THEN
    INSERT INTO wizyty VALUES (old.lekarz, old.pacjent, old.data_rozpoczecia, old.data_zakonczenia);
  END IF;

  RETURN old;
END;
$del_row$
LANGUAGE plpgsql;

CREATE TRIGGER too_early
AFTER DELETE ON wizyty
FOR EACH ROW
EXECUTE PROCEDURE add_row();
----

--ZAD4
CREATE OR REPLACE FUNCTION del_row()
  RETURNS TRIGGER AS
$del_row$
DECLARE
  t INTEGER := (SELECT
                  1
                FROM wizyty
                WHERE
                  wizyty.lekarz = new.lekarz AND
                  (new.data_rozpoczecia, new.data_zakonczenia) OVERLAPS
                  (wizyty.data_rozpoczecia, wizyty.data_zakonczenia)
                  AND NOT
                  (
                    wizyty.lekarz = new.lekarz AND
                    wizyty.pacjent = new.pacjent AND
                    wizyty.data_rozpoczecia = new.data_rozpoczecia AND
                    wizyty.data_zakonczenia = new.data_zakonczenia)
                LIMIT 1);
BEGIN
  IF (new.data_zakonczenia - new.data_rozpoczecia > INTERVAL '1 hour' OR t NOTNULL)
  THEN
    DELETE FROM wizyty
    WHERE
      wizyty.lekarz = new.lekarz AND
      wizyty.pacjent = new.pacjent AND
      wizyty.data_rozpoczecia = new.data_rozpoczecia AND
      wizyty.data_zakonczenia = new.data_zakonczenia;
  END IF;
  RETURN new;
END;
$del_row$
LANGUAGE plpgsql;


CREATE TRIGGER del_row
AFTER INSERT ON wizyty
FOR EACH ROW EXECUTE PROCEDURE del_row();
----


--ZAD5
DROP TABLE IF EXISTS lekarze_prowadzacy;
CREATE TABLE lekarze_prowadzacy
(
  pesel  CHAR(11) NOT NULL REFERENCES pacjenci (pesel),
  lekarz INTEGER  NOT NULL REFERENCES lekarze (id),
  PRIMARY KEY (pesel, lekarz)
);


CREATE OR REPLACE FUNCTION add_doc()
  RETURNS TRIGGER AS
$add_doc$

DECLARE
  doc_id INTEGER := (SELECT
                       id
                     FROM lekarze_prowadzacy
                       RIGHT JOIN lekarze
                         ON lekarz = lekarze.id
                       RIGHT JOIN specjalizacje
                         ON id = specjalizacje.id_lekarza
                     WHERE specjalizacja = 'Medycyna rodzinna'
                     GROUP BY id
                     ORDER BY count(pesel), id
                     LIMIT 1);
BEGIN
  INSERT INTO lekarze_prowadzacy VALUES (new.pesel, doc_id);
  RETURN new;
END;
$add_doc$
LANGUAGE plpgsql;


CREATE TRIGGER add_doc
AFTER INSERT ON pacjenci
FOR EACH ROW EXECUTE PROCEDURE add_doc();
----


--ZAD6
CREATE VIEW pediatrzy AS
  SELECT
    lekarze.*
  FROM
    lekarze
    JOIN specjalizacje
      ON id = id_lekarza
  WHERE specjalizacja = 'Pediatria';

CREATE OR REPLACE RULE add_Pediatra AS ON INSERT TO pediatrzy
DO INSTEAD
( INSERT INTO lekarze VALUES (new.id, new.imie, new.nazwisko);
  INSERT INTO specjalizacje VALUES (new.id, 'Pediatria'));
----


--ZAD7
CREATE VIEW chirurdzy AS
  SELECT
    lekarze.*
  FROM
    lekarze
    JOIN specjalizacje
      ON (id = id_lekarza)
  WHERE specjalizacja = 'Chirurgia';

CREATE OR REPLACE RULE del_chirurg AS ON DELETE TO chirurdzy
DO INSTEAD NOTHING;
----

--ZAD8

CREATE FUNCTION del_chir2(id INTEGER)
  RETURNS VOID AS
$$
DECLARE
  checker INTEGER := (SELECT
                        chirurdzy.id
                      FROM chirurdzy
                      WHERE id = chirurdzy.id);
BEGIN
  IF (checker ISNULL)
  THEN
    DELETE FROM specjalizacje
    WHERE id_lekarza = id;
    DELETE FROM lekarze
    WHERE lekarze.id = id;
  END IF;
END;
$$
LANGUAGE plpgsql;


CREATE OR REPLACE RULE del_chir2 AS ON DELETE TO lekarze
  WHERE (SELECT
           id
         FROM lekarze
           JOIN specjalizacje
             ON lekarze.id = specjalizacje.id_lekarza
         WHERE id = old.id AND specjalizacja = 'Chirurgia') NOTNULL
DO INSTEAD NOTHING;

----

--ZAD9
CREATE OR REPLACE RULE del_pac AS ON DELETE To pacjenci
DO
(
  DELETE FROM wizyty WHERE wizyty.pacjent = old.pesel;
);
----

Delete FROM pacjenci WHERE pesel = '96122707811';



