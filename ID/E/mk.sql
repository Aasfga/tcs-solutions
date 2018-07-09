SELECT pro.shortname AS "name", ok1.ok, ans.ans, tle.tle,rte1.rte,rte2.rte,rte3.rte,cmp.cmp,rul.rul,hea.hea,int1.int, al2.all
FROM problems AS pro LEFT OUTER JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "ok"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 8
    GROUP BY p.shortname
) AS ok1 USING(shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "ans"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.kod LIKE 'ANS'
    GROUP BY p.shortname
) AS ans USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "tle"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.kod LIKE 'TLE'
    GROUP BY p.shortname
) AS tle USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "rte"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 4
    GROUP BY p.shortname
) AS rte1 USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "rte"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 14
    GROUP BY p.shortname
) AS rte2 USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "rte"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 3
    GROUP BY p.shortname
) AS rte3 USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "cmp"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 2
    GROUP BY p.shortname
) AS cmp USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "rul"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 10
    GROUP BY p.shortname
) AS rul USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "hea"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 11
    GROUP BY p.shortname
) AS hea USING (shortname) FULL JOIN
(
    SELECT
      p.shortname,
      COUNT(*) AS "int"
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
      LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    WHERE sta.id = 6
    GROUP BY p.shortname
) AS int1 USING (shortname) FULL JOIN
  (
    SELECT
      p.shortname,
      COUNT(*) AS "all",
      p.name
    FROM
      problems AS p LEFT OUTER JOIN submits AS sub ON p.id = sub.problemsid
    LEFT JOIN
      statusy AS sta ON sub.status = sta.id
    GROUP BY p.shortname, p.id
    ORDER BY p.id
) as al2 USING(shortname)
ORDER BY al2.name;