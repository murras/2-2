SELECT P.pid, S.sname
FROM (
    SELECT C1.sid AS LastSid, C1.pid AS LastPid
    FROM (
        SELECT C.pid AS pGroupId, MAX(C.cost) AS pGroupMaxCost
        FROM Catalog C
        GROUP BY C.pid
    ), Catalog C1
    WHERE C1.pid = pGroupId AND C1.cost = pGroupMaxCost
), Suppliers S, Parts P
WHERE S.sid = LastSid AND P.pid = LastPid;