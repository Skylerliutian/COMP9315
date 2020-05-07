create table Students (
	sid     integer,
	name    text,
	degree  text,
	primary key (sid)
);
create table Courses (
	cid     integer,
	code    char(8),
	term    char(4),
	title   text,
	primary key (cid)
);
create table Enrolments (
	sid     integer references Students(sid),
	cid     integer references Courses(cid),
	mark    integer,
	grade   char(2)
);

copy Students(sid,name,degree) from stdin;
3322111	John Smith	MEngSc
3335555	Joe Blow	BSc
5012345	John Smith	MIT
5123456	Jack Black	MEngSc
5176543	Jane Smith	BSc
\.

copy Courses(cid, code, term, title) from stdin;
1622	COMP9315	16s1	DBMS Implementation
1732	COMP9311	18s1	Database Systems
1733	COMP3311	18s1	Database Systems
1844	COMP9315	18s2	DBMS Implementation
\.

copy Enrolments(sid, cid, mark, grade) from stdin;
3335555	1622	90	HD
5123456	1622	45	FL
5012345	1622	30	FL
3322111	1732	50	PS
5012345	1732	50	PS
5123456	1732	80	DN
5176543	1733	99	HD
5012345	1844	49	FL
5123456	1844	75	DN
5176543	1844	90	HD
