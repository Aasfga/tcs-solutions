--ZAD9
ALTER TABLE etaty
  ADD pensja_od NUMERIC(8, 2);
ALTER TABLE etaty
  ADD pensja_do NUMERIC(8, 2);
UPDATE etaty
SET
  pensja_od = CASE WHEN widelki [1] < etaty.widelki [2]
    THEN widelki [1]
              ELSE widelki [2] END,
  pensja_do = CASE WHEN widelki [1] < etaty.widelki [2]
    THEN widelki [2]
              ELSE widelki [1] END;
ALTER TABLE etaty
  ALTER COLUMN pensja_od SET NOT NULL;
ALTER TABLE etaty
  ALTER COLUMN pensja_do SET NOT NULL;
ALTER TABLE etaty
  DROP COLUMN widelki;
ALTER TABLE etaty
  ADD CHECK (pensja_od < pensja_do);

UPDATE pracownicy
SET
  pensja = CASE WHEN pensja_od > pracownicy.pensja
    THEN pensja_od
           ELSE pensja END
FROM etaty
WHERE etaty.etat = pracownicy.etat;


INSERT INTO etaty
  SELECT
    etat || ' starszy',
    pensja_od,
    2 * pensja_do
  FROM
    etaty;

UPDATE pracownicy
SET
  etat = pracownicy.etat || ' starszy'
FROM etaty
WHERE etaty.etat = pracownicy.etat AND pensja > etaty.pensja_do;


ALTER TABLE pracownicy
  DROP CONSTRAINT pracownicy_etat_fkey;
ALTER TABLE etaty
  DROP CONSTRAINT etaty_pkey;
ALTER TABLE etaty
  ADD COLUMN id_etatu SERIAL;
UPDATE etaty
SET
  id_etatu = id_etatu * 10;
ALTER TABLE etaty
  ADD PRIMARY KEY (id_etatu);
ALTER TABLE pracownicy
  ADD COLUMN id_etatu INTEGER;
UPDATE pracownicy
SET
  id_etatu = etaty.id_etatu
FROM etaty
WHERE etaty.etat = pracownicy.etat;


ALTER TABLE pracownicy
  DROP COLUMN etat;
ALTER TABLE pracownicy
  RENAME COLUMN id_etatu TO etat;
ALTER TABLE pracownicy
  ADD FOREIGN KEY (etat) REFERENCES etaty;
ALTER TABLE pracownicy
  ALTER COLUMN etat SET NOT NULL;

ALTER TABLE pracownicy
  ADD COLUMN dodatki NUMERIC(8, 2);
UPDATE pracownicy
SET
  dodatki = -etaty.pensja_do + pracownicy.pensja,
  pensja  = etaty.pensja_do
FROM etaty
WHERE pracownicy.etat = etaty.id_etatu AND pensja > etaty.pensja_do;


----