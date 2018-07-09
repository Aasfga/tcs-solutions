--ZAD2
----


--ZAD1
SELECT
  jezyk.name        AS language,
  count(submits.id) AS submits
FROM
  submits
  JOIN jezyk
    ON language = jezyk.id
GROUP BY jezyk.name
ORDER BY 2 DESC, 1;
----

--ZAD2
SELECT
  shortname as name,

  case when sum(Case WHEN status = 8 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 8 THEN 1 else 0 END ) end as ok,
  case when sum(Case WHEN status = 7 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 7 THEN 1 else 0 END ) end as ans,
  case when sum(Case WHEN status = 5 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 5 THEN 1 else 0 END ) end as tle,
  case when sum(Case WHEN status = 4 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 4 THEN 1 else 0 END ) end as rte,
  case when sum(Case WHEN status = 14 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 14 THEN 1 else 0 END ) end as rte,
  case when sum(Case WHEN status = 3 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 3 THEN 1 else 0 END ) end as rte,
  case when sum(Case WHEN status = 2 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 2 THEN 1 else 0 END ) end as cmp,
  case when sum(Case WHEN status = 10 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 10 THEN 1 else 0 END ) end as rul,
  case when sum(Case WHEN status = 11 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 11 THEN 1 else 0 END ) end as hea,
  case when sum(Case WHEN status = 6 THEN 1 else 0 END ) = 0 then null else sum(Case WHEN status = 6 THEN 1 else 0 END ) end as int,
  count(*) as "all"

FROM
  problems
  JOIN submits
    ON problems.id = submits.problemsid
GROUP BY shortname
ORDER BY substr(shortname, 1, 1), CASE WHEN substr(shortname, 2, 2) = '' THEN 0 ELSE substr(shortname, 2, 2)::INTEGER END ;
----

--ZAD3
SELECT

FROM
  (SELECT DISTINCT ON (usersid, problemsid) *
   FROM submits
   ORDER BY usersid, problemsid, ocena DESC) as bestResults;
----


