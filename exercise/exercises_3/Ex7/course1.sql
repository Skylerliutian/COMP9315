create table Course (
	id      integer,  -- 4 bytes
	code    char(8),  -- 8 bytes, e.g. COMP9315
	title   varchar(100),  -- ??? bytes
	uoc     integer,  -- 4 bytes
    avgmark float,    -- 4/8? bytes
	primary key (id)
);
