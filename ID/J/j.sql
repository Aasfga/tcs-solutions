--ZAD10
----

--ZAD1
CREATE OR REPLACE FUNCTION cast_int(x VARCHAR)
  RETURNS INTEGER AS
$$
BEGIN

  RETURN cast(x AS INTEGER);

  EXCEPTION
  WHEN OTHERS
    THEN RETURN NULL;

END;
$$
LANGUAGE plpgsql;
----


--ZAD2
SELECT
  string_to_array(regexp_replace(a.con, '\((.+)\)', '\1'), ',') AS wynik
FROM
  (SELECT
     concat(tab.*) AS con
   FROM tab) AS a;
----


--ZAD3
CREATE OR REPLACE FUNCTION nulls(VARIADIC x ANYARRAY)
  RETURNS INTEGER AS
$$
BEGIN
  RETURN (SELECT
            sum(CASE WHEN b ISNULL
              THEN 1
                ELSE 0 END)
          FROM unnest(x) AS a(b));
END;
$$
LANGUAGE plpgsql;

--ZAD5
CREATE OR REPLACE FUNCTION array_intersect(ANYARRAY, ANYARRAY)
  RETURNS ANYARRAY
LANGUAGE SQL
AS $FUNCTION$
SELECT
  ARRAY(
      SELECT
        UNNEST($1)
      INTERSECT
      SELECT
        UNNEST($2)
  );
$FUNCTION$;
----

--ZAD6
CREATE OR REPLACE FUNCTION array_sort(ANYARRAY)
  RETURNS ANYARRAY AS
$$
BEGIN
  RETURN (SELECT
            ARRAY
            (
                SELECT
                  *
                FROM
                    unnest($1)
                ORDER BY 1
            ));
END
$$
LANGUAGE plpgsql;
----

--ZAD7
SELECT
  *
FROM (
       SELECT
         *,
         row_number()
         OVER (
           ORDER BY CTID ASC ) AS row
       FROM tab
     ) AS new_tab
WHERE (row - 1) % 3 = 0
----


--ZAD8
SELECT
  *
FROM zwierzeta
ORDER BY
  CASE WHEN rodzaj = 'pies'
    THEN 1
  ELSE CASE WHEN rodzaj = 'kot'
    THEN 2
       ELSE 3 END END, nazwa;
----


--ZAD9
ALTER TABLE tab
  DROP CONSTRAINT tab_b_check,
  ALTER COLUMN b TYPE BOOLEAN
  USING
  CASE WHEN b = 'tak'
    THEN TRUE
  ELSE FALSE END;
----


--ZAD10
SELECT
  tablename
FROM pg_tables
WHERE schemaname = 'public'
----


--ZAD11
CREATE OR REPLACE FUNCTION remove_all()
  RETURNS VOID AS

$$
DECLARE
  r RECORD;
BEGIN

  FOR r IN SELECT
             tablename
           FROM pg_tables
           WHERE schemaname = 'public'
  LOOP
    EXECUTE 'Drop table if exists ' || quote_ident(r.tablename) || ' Cascade';
  END LOOP;
END;
$$
LANGUAGE plpgsql;

SELECT
  remove_all();
----

--ZAD12

SELECT
  pg_database.datname,
  pg_size_pretty(pg_database_size(pg_database.datname)) AS size
FROM pg_database
----
