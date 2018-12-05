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

INSERT INTO Teams(tid, location, manager)
VALUES ('T1', 'Seoul', 'Taehyung');
INSERT INTO Teams(tid, location, manager)
VALUES ('T2', 'Seoul', 'Joongil');
INSERT INTO Teams(tid, location, manager)
VALUES ('T3', 'Incheon', 'Hillman');
INSERT INTO Teams(tid, location, manager)
VALUES ('T4', 'Busan', 'Sangmoon');

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
