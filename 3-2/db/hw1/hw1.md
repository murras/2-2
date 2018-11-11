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
    psalary integer,
    CONSTRAINT P_PK PRIMARY KEY (pid)    
);
```

```sql
CREATE TABLE Teams(
    tid varchar2(10),
    tlocation varchar2(40),
    tmanager varchar2(20),
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
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('432', '브리검', '51600');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('624', '양현종', '47500');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('111', '소사', '74000');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('212', '윌슨', '62400');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('467', '린드블럼', '54500');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('662', '니퍼트', '45700');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('224', '켈리', '45300');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('923', '김광현', '43400');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('100', '레일리', '42200');
INSERT INTO Pitchers(pid, pname, psalary)
VALUES ('513', '샘슨', '49000');
```

#### Teams 데이터 추가

```sql
INSERT INTO Teams(tid, tlocation, tmanager)
VALUES ('1', '서울', '김태형');
INSERT INTO Teams(tid, tlocation, tmanager)
VALUES ('2', '서울', '류중일');
INSERT INTO Teams(tid, tlocation, tmanager)
VALUES ('3', '인천', '힐만');
INSERT INTO Teams(tid, tlocation, tmanager)
VALUES ('4', '부산', '양상문');
INSERT INTO Teams(tid, tlocation, tmanager)
VALUES ('5', '창원', '이동욱');
INSERT INTO Teams(tid, tlocation, tmanager)
VALUES ('6', '대구', '김한수');
```

#### Records 데이터 추가

```sql
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('001', '1', '2', '432', '111');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('002', '2', '3', '212', '111');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('003', '3', '2', '467', '111');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('004', '4', '2', '513', '111');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('005', '5', '2', '624', '111');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('006', '6', '2', '662', '111');
INSERT INTO Records(rid, home_team_id, away_team_id, starting_pitcher_home, starting_pitcher_away)
VALUES ('007', '5', '6', '100', '662');
```

#### 나번 문제.
```sql
SELECT p4.*
FROM Pitchers p4
WHERE p4.psalary = (
    SELECT MAX(p2.psalary)
    FROM Pitchers p2
    WHERE p2.pid NOT IN (
        SELECT p.pid
        FROM Records r, Pitchers p
        WHERE p.pid = r.starting_pitcher_home OR p.pid = r.starting_pitcher_away
    )
)        
```