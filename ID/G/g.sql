--ZAD2
----

--ZAD1
CREATE TABLE kategorie
(
  id_kategoria SERIAL PRIMARY KEY,
  nazwa        VARCHAR(250) UNIQUE NOT NULL,
  vat          NUMERIC(3, 1)       NOT NULL
);

INSERT INTO kategorie (nazwa, vat)
  SELECT DISTINCT ON (nazwa)
    kategoria AS nazwa,
    vat
  FROM produkty;

ALTER TABLE produkty
  ADD COLUMN id_kategoria INTEGER;
UPDATE produkty
SET id_kategoria = kategorie.id_kategoria FROM kategorie
WHERE produkty.kategoria = kategorie.nazwa;
ALTER TABLE produkty
  DROP COLUMN IF EXISTS kategoria;
ALTER TABLE produkty
  DROP COLUMN IF EXISTS vat;
ALTER TABLE produkty
  ADD CONSTRAINT id_kategoria FOREIGN KEY (id_kategoria) REFERENCES kategorie;
ALTER TABLE produkty
  ALTER COLUMN id_kategoria SET NOT NULL;
----

--ZAD2
CREATE TABLE historia_cen (kod_produktu, cena_netto, data_wprowadzenia) AS
  SELECT
    kod_produktu,
    cena_netto,
    '2000-01-01' :: DATE
  FROM produkty;

ALTER TABLE historia_cen
  ADD PRIMARY KEY (kod_produktu, data_wprowadzenia);
ALTER TABLE historia_cen
  ALTER COLUMN cena_netto SET NOT NULL;
ALTER TABLE historia_cen
  ADD FOREIGN KEY (kod_produktu) REFERENCES produkty;
ALTER TABLE produkty
  DROP COLUMN IF EXISTS cena_netto;
----

