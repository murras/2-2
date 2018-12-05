CREATE TABLE Suppliers(
    sid integer,
    sname varchar(20),
    address varchar(20),
    CONSTRAINT S_PK PRIMARY KEY (sid)    
);

CREATE TABLE Parts(
    pid integer,
    pname varchar2(20),
    color varchar2(20),
    CONSTRAINT Parts_PK PRIMARY KEY (pid)
);

CREATE TABLE Catalog(
    sid integer,
    pid integer,
    cost real,
    CONSTRAINT Cata_PK PRIMARY KEY (sid, pid),
    CONSTRAINT Cat_FK_sid FOREIGN KEY(sid) REFERENCES Suppliers(sid),
    CONSTRAINT Cat_FK_pid FOREIGN KEY(pid) REFERENCES Parts(pid)
);
