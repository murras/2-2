### 다음 스키마를 토대로 테이블을 만든다.

- Pitchers (**pid: string**, name: string, salary: integer)
- Teams (**tid: string**, name: string, salary: integer)
- Records (**rid: string**, name: string, salary: integer)
- 굵은 선이 PK이다.
  
#### 3개의 테이블 생성을 한다.
  
```sql
CREATE TABLE Pitchers(
    pid varchar2(10),
    pname varchar2(20),
    salary integer,
    CONSTRAINT P_PK PRIMARY KEY (pid)    
);
```

```sql
CREATE TABLE Teams(
    tid varchar2(10),
    location varchar2(40),
    manager varchar2(20),
    CONSTRAINT T_PK PRIMARY KEY (tid)
);
```

```sql
CREATE TABLE Records(
    rid varchar2(10),
    home_team_id varchar2(10),
    away_team_id varchar2(10),
    starting_pitcher_home varchar2(10),
    starting_pitcher_away varchar2(10), 
    CONSTRAINT R_PK PRIMARY KEY (rid),
    CONSTRAINT R_FK_homeTeamId FOREIGN KEY(home_team_id) REFERENCES Teams(tid),
    CONSTRAINT R_FK_awayTeamId FOREIGN KEY(away_team_id) REFERENCES Teams(tid),
    CONSTRAINT R_FK_homeStartPitcher FOREIGN KEY(starting_pitcher_home) REFERENCES Pitchers(pid),
    CONSTRAINT R_FK_awayStartPitcher FOREIGN KEY(starting_pitcher_away) REFERENCES Pitchers(pid)
);
```

#### Pitchers 데이터 추가

```sql
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p01', 'AA', '51600');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p02', 'BB', '47500');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p03', 'CC', '74000');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p04', 'DD', '62400');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p05', 'EE', '54500');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p06', 'FF', '45700');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p07', 'GG', '45300');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p08', 'HH', '43400');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p09', 'II', '42200');
INSERT INTO Pitchers(pid, pname, salary)
VALUES ('p10', 'JJ', '49000');
```

#### Teams 데이터 추가

```sql
INSERT INTO Teams(tid, location, manager)
VALUES ('T1', 'Seoul', 'Taehyung');
INSERT INTO Teams(tid, location, manager)
VALUES ('T2', 'Seoul', 'Joongil');
INSERT INTO Teams(tid, location, manager)
VALUES ('T3', 'Incheon', 'Hillman');
INSERT INTO Teams(tid, location, manager)
VALUES ('T4', 'Busan', 'Sangmoon');
```

#### Records 데이터 추가

```sql
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('001', 'T1', 'T2', 'p01', 'p06');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('002', 'T2', 'T3', 'p02', 'p06');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('003', 'T3', 'T2', 'p03', 'p06');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('004', 'T4', 'T2', 'p04', 'p06');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('005', 'T1', 'T3', 'p05', 'p02');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('006', 'T4', 'T2', 'p03', 'p09');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('007', 'T4', 'T2', 'p04', 'p07');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('008', 'T1', 'T4', 'p01', 'p07');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('009', 'T2', 'T1', 'p02', 'p07');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('010', 'T3', 'T4', 'p03', 'p07');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('011', 'T4', 'T1', 'p04', 'p07');
```

#### 가 - 1
```sql
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
```

#### 가 - 2

```sql
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
```
