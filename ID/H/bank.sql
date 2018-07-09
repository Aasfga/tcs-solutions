DROP TABLE IF EXISTS klienci CASCADE;
DROP TABLE IF EXISTS konta CASCADE;
DROP TABLE IF EXISTS transakcje CASCADE;

CREATE TABLE klienci (
    id numeric(11,0) NOT NULL,
    imie character varying(150) NOT NULL,
    nazwisko character varying(200) NOT NULL
);

CREATE TABLE konta (
    nr_konta numeric(11,0) NOT NULL,
    wlasciciel numeric(11,0) NOT NULL,
    data_otwarcia timestamp without time zone DEFAULT now() NOT NULL
);

CREATE TABLE transakcje (
    id numeric(11,0) NOT NULL,
    data_zlecenia timestamp without time zone DEFAULT now() NOT NULL,
    z_konta numeric(11,0),
    na_konto numeric(11,0),
    kwota numeric(11,2) NOT NULL,
    CONSTRAINT transaction_ch CHECK (((z_konta IS NOT NULL) OR (na_konto IS NOT NULL)) AND (z_konta <> na_konto))
);

COPY klienci (id, imie, nazwisko) FROM stdin;
100	Abel	Spencer
101	Erick	Leonard
102	Janice	Montes
104	Lawanda	Velazquez
105	Robbie	Wilkins
106	Carla	Randall
\.


COPY konta (nr_konta, wlasciciel, data_otwarcia) FROM stdin;
1000	105	2014-11-14 14:07:48.864
1001	100	2014-10-18 21:47:47.712
1002	102	2014-11-24 23:37:14.368
1003	102	2014-10-01 06:05:19.024
1004	101	2014-10-31 06:50:41.92
1005	100	2014-11-22 03:06:27.072
1007	104	2014-10-03 00:45:46.768
1006	106	2014-11-30 03:18:34.624
\.


COPY transakcje (id, data_zlecenia, z_konta, na_konto, kwota) FROM stdin;
101	2014-12-01 12:22:23.084991	\N	1000	2500.00
102	2014-12-01 15:23:03.317561	\N	1001	1500.00
103	2014-12-01 16:23:24.275342	\N	1002	500.00
104	2014-12-02 09:23:37.619493	\N	1002	1650.00
105	2014-12-02 10:24:32.723885	1001	\N	250.00
106	2014-12-02 12:24:59.196042	\N	1003	5000.00
107	2014-12-03 11:25:12.072206	\N	1004	3500.00
108	2014-12-03 12:25:34.158276	\N	1005	1750.00
109	2014-12-03 15:25:48.866346	\N	1006	12000.00
110	2014-12-04 12:26:05.335593	1006	\N	3300.00
111	2014-12-05 12:26:16.544764	1005	\N	50.00
112	2014-12-05 14:31:51.698744	\N	1007	1000.00
113	2014-12-05 12:27:56.573163	1000	1007	2000.00
114	2014-12-06 12:28:22.22531	1006	1000	5500.00
115	2014-12-06 13:29:12.205655	1002	1003	150.00
116	2014-12-06 14:29:38.023802	1005	1006	50.00
117	2014-12-06 17:30:03.465956	1003	1002	250.00
\.


ALTER TABLE ONLY klienci
    ADD CONSTRAINT pk_klienci PRIMARY KEY (id);


ALTER TABLE ONLY konta
    ADD CONSTRAINT pk_konta PRIMARY KEY (nr_konta);


ALTER TABLE ONLY transakcje
    ADD CONSTRAINT pk_transakcje PRIMARY KEY (id);


ALTER TABLE ONLY konta
    ADD CONSTRAINT fk_konta_klienci FOREIGN KEY (wlasciciel) REFERENCES klienci(id);


ALTER TABLE ONLY transakcje
    ADD CONSTRAINT fk_transakcje_konta_na FOREIGN KEY (na_konto) REFERENCES konta(nr_konta);


ALTER TABLE ONLY transakcje
    ADD CONSTRAINT fk_transakcje_konta_z FOREIGN KEY (z_konta) REFERENCES konta(nr_konta);
