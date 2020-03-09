
create table Course (
	id      integer,  -- 4 bytes
	code    char(8),  -- 8 bytes, e.g. COMP9315
	uoc     integer,  -- 4 bytes
    term    char(4),  -- 4 bytes e.g. "18s2" "19T1" ...
    avgmark float,    -- 4/8? bytes
	primary key (id)
);
