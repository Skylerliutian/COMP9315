create table People (
	id          integer,
	ptype       varchar(10),
	title       varchar(5),
	family      varchar(40),
	given       varchar(50),
	street      varchar(40),
	suburb      varchar(30),
	pcode       integer,
	gender      char(1) check (gender in ('m','f')),
	birthday    date,
	country     char(3),
	primary key (id)
);

create table Courses (
	id          integer,
	code        char(8) check (code ~ '[A-Z]{4}[0-9]{4}'),
	title       varchar(50),
	uoc         integer check (uoc > 0),
	convenor    integer references People(id),
	primary key (id)
);

create table Enrolments (
	course      integer references Courses(id),
	student     integer references People(id),
	mark        integer check (mark between 0 and 100),
	grade       char(2) check (grade in ('HD','DN','CR','PS','FL')),
	primary key (course,student)
);

create table Items (
	id          integer,
	course      integer references Courses(id),
	name        varchar(20),
	maxMark     integer check (maxMark > 0),
	primary key (id)
);

create table Assessments (
	item        integer references Items(id),
	student     integer references People(id),
	mark        integer check (mark >= 0),
	primary key (item,student)
);
