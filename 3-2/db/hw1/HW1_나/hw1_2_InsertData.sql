INSERT INTO Suppliers(sid, sname, address)
VALUES ('1', 'SupA', 'Seoul');
INSERT INTO Suppliers(sid, sname, address)
VALUES ('2', 'SupB', 'Jeju');
INSERT INTO Suppliers(sid, sname, address)
VALUES ('3', 'SupC', 'Daejeon');
INSERT INTO Suppliers(sid, sname, address)
VALUES ('4', 'SupD', 'Seoul');

INSERT INTO Parts(pid, pname, color)
VALUES ('100', 'p1', 'red');
INSERT INTO Parts(pid, pname, color)
VALUES ('101', 'p2', 'red');
INSERT INTO Parts(pid, pname, color)
VALUES ('200', 'p3', 'green');
INSERT INTO Parts(pid, pname, color)
VALUES ('201', 'p4', 'green');

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
