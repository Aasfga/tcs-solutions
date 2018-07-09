DROP TABLE IF EXISTS druzyny CASCADE;
DROP TABLE IF EXISTS zawodnicy CASCADE;


CREATE TABLE druzyny (
    id integer NOT NULL,
    id_kapitan integer NOT NULL,
    licznosc integer NOT NULL,
    CONSTRAINT ck_0 CHECK (((licznosc > 0) AND (licznosc <= 11)))
);


CREATE TABLE zawodnicy (
    id integer NOT NULL,
    imie character varying NOT NULL,
    nazwisko character varying NOT NULL,
    id_druzyny integer
);


COPY druzyny (id, id_kapitan, licznosc) FROM stdin;
\.


COPY zawodnicy (id, imie, nazwisko, id_druzyny) FROM stdin;
\.


ALTER TABLE ONLY druzyny
    ADD CONSTRAINT pk_druzyny PRIMARY KEY (id);


ALTER TABLE ONLY zawodnicy
    ADD CONSTRAINT pk_zawodnicy PRIMARY KEY (id);

CREATE INDEX idx_zawodnicy ON zawodnicy USING btree (id_druzyny);

ALTER TABLE ONLY druzyny
    ADD CONSTRAINT fk_druzyny_zawodnicy FOREIGN KEY (id_kapitan) REFERENCES zawodnicy(id);

ALTER TABLE ONLY zawodnicy
    ADD CONSTRAINT fk_zawodnicy_druzyny FOREIGN KEY (id_druzyny) REFERENCES druzyny(id);

