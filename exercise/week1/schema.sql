-- get DB schema as a sequence of strings
-- each string contains TableName(Attr1Name, Attr2Name, ...)

create or replace function schema() returns setof text
as $$
declare
	rec record;      -- current tuple from query
	rel text := '';  -- name of current table
	att text := '';  -- name of current attribute
	out text := '';  -- output string
begin
	for rec in
		-- get (rel,attr,seq) for all attrs in all tables
		select t.relname, a.attname, a.attnum
	    from   pg_class t
		       join pg_attribute a on (a.attrelid=t.oid)
		       join pg_namespace n on (t.relnamespace=n.oid)
	    where  t.relkind='r' and n.nspname = 'public'
		       and a.attnum > 0
		order by t.relname,a.attnum
	loop
		-- check for table name change
		if (rec.relname <> rel) then
			if (rel <> '') then
				-- "output" next table schema
				out := rel || '(' || att || ')';
				return next out;
			end if;
			-- set up for new table
			rel := rec.relname;
			att := '';
		end if;
		-- build list of attribute names
		-- fiddling around avoids extraneous ","
		if (att <> '') then
			att := att || ', ';
		end if;
		att := att || rec.attname;
	end loop;
	-- deal with last table
	-- if we don't do this, the last table is left out
	if (rel <> '') then
		out := rel || '(' || att || ')';
		return next out;
	end if;
end;
$$ language plpgsql;
