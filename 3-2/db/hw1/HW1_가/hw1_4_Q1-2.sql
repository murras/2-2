SELECT P.*
FROM (
    SELECT MAX(P1.salary) AS maxSalary
    FROM Pitchers P1
    WHERE P1.pid NOT IN (
        SELECT P12.pid
        FROM Records R1, Pitchers P12
        WHERE P12.pid = R1.starting_pitcher_home OR P12.pid = R1.starting_pitcher_away
    )
), (
    SELECT P2.pid AS wantPid
    FROM Pitchers P2
    WHERE P2.pid NOT IN (
        SELECT P21.pid
        FROM Records R2, Pitchers P21
        WHERE P21.pid = R2.starting_pitcher_home OR P21.pid = R2.starting_pitcher_away
    )
), Pitchers P
WHERE P.pid = wantPid AND P.salary = maxSalary;
