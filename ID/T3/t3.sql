--ZAD1

BEGIN;
UPDATE pracownicy
SET placa_dod = placa_dod + 200
WHERE nazwisko = 'Dolny';

SAVEPOINT S1;

UPDATE pracownicy
SET placa_dod = placa_dod + 100
WHERE nazwisko = 'Nowak';

SAVEPOINT S2;

DELETE FROM pracownicy WHERE nazwisko = 'Makowski';
ROLLBACK To S1;
SELECT * FROM pracownicy;
ROLLBACK TO S2;
ABORT;

COMMIT;
----

--ZAD2
BEGIN;
DELETE FROM pracownicy WHERE nazwisko = 'Dolny';
SAVEPOINT S1;

ALTER TABLE etaty ALTER COLUMN nazwa TYPE VARCHAR(20);
ROLLBACK TO S1;

COMMIT;
----

--ZAD3
DROP TABLE IF EXISTS test;
CREATE TABLE test
(
  id INTEGER PRIMARY KEY DEFERRABLE
);


BEGIN;
SET CONSTRAINTS ALL DEFERRED;
INSERT INTO test VALUES (1);
INSERT INTO test VALUES (1);
INSERT INTO test VALUES (1);
INSERT INTO test VALUES (1);
INSERT INTO test VALUES (1);
COMMIT;
----

--ZAD4
BEGIN;
SELECT * FROM pracownicy WHERE nazwisko = 'Dolny';
SELECT * FROM pracownicy WHERE nazwisko = 'Dolny';
COMMIT;

BEGIN;
UPDATE pracownicy SET placa_dod = 2* placa_dod
WHERE etat = 'ASYSTENT';

COMMIT;
----