--ZAD2
----


--ZAD1
SELECT
  coalesce(status, '') AS status,
  count(task_id)
FROM checks
GROUP BY status
ORDER BY
  (status LIKE ''),
  1, 2;
----


SELECT
  DISTINCT ON (task_id)
  task_id,
  cost,
  substring(cost, '([0-9]+)-') :: INTEGER   AS firstPart,
  substring(cost, '.*-([0-9]+)') :: INTEGER AS secondPart
FROM checks
WHERE status = 'OK'
ORDER BY 1, 3 DESC NULLS LAST, 4 DESC;

--ZAD2
SELECT
  short_name,
  task_name,
  coalesce(maxCost.cost, '') as max_cost,
  sum(CASE WHEN checks.status = 'OK' AND maxCost.cost = checks.cost
    THEN 1
      ELSE 0 END) as liczba_rozwiazan
FROM
  checks
  RIGHT JOIN tasks
  USING (task_id)
  JOIN problems
  USING (problem_id)
  LEFT JOIN
  (SELECT
     DISTINCT ON (task_id)
     task_id,
     cost,
     substring(cost, '([0-9.]+)-') :: FLOAT  AS firstPart,
     substring(cost, '.*-([0-9.]+)') :: FLOAT AS secondPart
   FROM checks
   WHERE status = 'OK'
   ORDER BY 1, 3 DESC NULLS LAST, 4 DESC) AS maxCost
  USING (task_id)
GROUP BY 1, 2, 3
ORDER BY 1, 2;

----