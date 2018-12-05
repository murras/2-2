### 책에 나온대로 테이블을 만든다.
```sql
CREATE TABLE Suppliers(
    sid integer,
    sname varchar(20),
    address varchar(20),
    CONSTRAINT S_PK PRIMARY KEY (sid)    
);
```

```sql
CREATE TABLE Parts(
    pid integer,
    pname varchar2(20),
    color varchar2(20),
    CONSTRAINT Parts_PK PRIMARY KEY (pid)
);
```

```sql
CREATE TABLE Catalog(
    sid integer,
    pid integer,
    cost real,
    CONSTRAINT Cata_PK PRIMARY KEY (sid, pid),
    CONSTRAINT Cat_FK_sid FOREIGN KEY(sid) REFERENCES Suppliers(sid),
    CONSTRAINT Cat_FK_pid FOREIGN KEY(pid) REFERENCES Parts(pid)
);
```

#### Suppliers Data 추가
```sql
INSERT INTO Suppliers(sid, sname, address)
VALUES ('1', 'SupA', 'Seoul');
INSERT INTO Suppliers(sid, sname, address)
VALUES ('2', 'SupB', 'Jeju');
INSERT INTO Suppliers(sid, sname, address)
VALUES ('3', 'SupC', 'Daejeon');
INSERT INTO Suppliers(sid, sname, address)
VALUES ('4', 'SupD', 'Seoul');
```

#### Parts Data 추가
```sql
INSERT INTO Parts(pid, pname, color)
VALUES ('100', 'p1', 'red');
INSERT INTO Parts(pid, pname, color)
VALUES ('101', 'p2', 'red');
INSERT INTO Parts(pid, pname, color)
VALUES ('200', 'p3', 'green');
INSERT INTO Parts(pid, pname, color)
VALUES ('201', 'p4', 'green');
```

#### Catalog Data 추가
```sql
INSERT INTO Catalog(sid, pid, cost)
VALUES ('1', '100', 500);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('1', '101', 5000);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('1', '200', 100);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('1', '201', 300);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('2', '100', 600);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('2', '201', 250);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('3', '101', 4300);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('3', '200', 600);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('4', '100', 700);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('4', '101', 4700);
INSERT INTO Catalog(sid, pid, cost)
VALUES ('4', '200', 50);
```

#### 6번 문제
```sql
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
WHERE S.sid = LastSid AND P.pid = LastPid
```

#### 8번 문제
```sql
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
)
```