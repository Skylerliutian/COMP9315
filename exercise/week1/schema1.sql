drop type if exists SchemaTuple cascade;
create type SchemaTuple as ("table" text, "attributes" text);

create or replace function schema1() returns setof SchemaTuple
as $$
declare
        rec   record;
        rel   text := '';
	attr  text := '';
        attrs text := '';
        out   SchemaTuple;
	len   integer := 0;
begin
	for rec in
		select r.relname, a.attname, t.typname, a.atttypmod
		from   pg_class r
			join pg_namespace n on (r.relnamespace = n.oid)
			join pg_attribute a on (a.attrelid = r.oid)
			join pg_type t on (a.atttypid = t.oid)
		where  r.relkind='r'
			and n.nspname = 'public'
			and attnum > 0
		order by relname,attnum
	loop
		if (rec.relname <> rel) then
			if (rel <> '') then
				out."table" := rel;
				out.attributes := attrs;
				return next out;
			end if;
			rel := rec.relname;
			attrs := '';
			len := 0;
		end if;
		if (attrs <> '') then
			attrs := attrs || ', ';
			len := len + 2;
		end if;
		if (rec.typname = 'varchar') then
			rec.typname := 'varchar('||(rec.atttypmod-4)||')';
		elsif (rec.typname = 'bpchar') then
			rec.typname := 'char('||(rec.atttypmod-4)||')';
		elsif (rec.typname = 'int4') then
			rec.typname := 'integer';
		elsif (rec.typname = 'float8') then
			rec.typname := 'float';
		end if;
		attr := rec.attname||':'||rec.typname;
		if (len + length(attr) > 50) then
			attrs := attrs || E'\n';
			len := 0;
		end if;
		attrs := attrs || attr;
		len := len + length(attr);
	end loop;
	-- deal with last table
	if (rel <> '') then
		out."table" := rel;
		out.attributes := attrs;
		return next out;
	end if;
end;
$$ language plpgsql;
