SELECT P.*
FROM (
    SELECT P1.pid AS minPid
    FROM Pitchers P1
    WHERE NOT EXISTS(
        SELECT T1.tid
        FROM Teams T1
        WHERE NOT EXISTS(
            SELECT R1.rid
            FROM Records R1
            WHERE T1.tid = R1.home_team_id AND P1.pid = R1.starting_pitcher_away
        )
    )
), (
    SELECT min(P2.salary) AS minsalary
    FROM Pitchers P2
    WHERE NOT EXISTS(
        SELECT T2.tid
        FROM Teams T2
        WHERE NOT EXISTS(
            SELECT R2.rid
            FROM Records R2
            WHERE T2.tid = R2.home_team_id AND P2.pid = R2.starting_pitcher_away
        )
    )
), Pitchers p
WHERE p.pid = minPid AND p.salary = minSalary;