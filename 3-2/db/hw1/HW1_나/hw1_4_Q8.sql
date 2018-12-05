SELECT S.sid
FROM Suppliers S
WHERE S.sid IN ((
        SELECT onlyOneRedSid
        FROM (
            SELECT C1.sid AS onlyOneRedSid
            FROM Catalog C1
            WHERE C1.pid IN (
                SELECT P1.pid
                FROM Parts P1
                WHERE P1.color = 'red'
            )
        )
        GROUP BY onlyOneRedSid
        HAVING count(*)=1
    )
    INTERSECT (
        SELECT onlyOneGreenSid
        FROM (
            SELECT C2.sid AS onlyOneGreenSid
            FROM Catalog C2
            WHERE C2.pid IN (
                SELECT P2.pid
                FROM Parts P2
                WHERE P2.color = 'green'
            )
        )
        GROUP BY onlyOneGreenSid
        HAVING count(*)=1
    )
);