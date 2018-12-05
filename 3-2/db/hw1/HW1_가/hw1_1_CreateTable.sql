CREATE TABLE Pitchers(
    pid varchar2(10),
    pname varchar2(20),
    salary integer,
    CONSTRAINT P_PK PRIMARY KEY (pid)    
);
CREATE TABLE Teams(
    tid varchar2(10),
    location varchar2(40),
    manager varchar2(20),
    CONSTRAINT T_PK PRIMARY KEY (tid)
);
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