-- get DB schema as a sequence of strings
-- each string contains TableName(Attr1Name, Attr2Name, ...)

create or replace function schema2() returns setof text
as $$
	select t.relname||'('||string_agg(a.attname::text,', ')||')'
    from   pg_class t
           join pg_attribute a on (a.attrelid=t.oid)
           join pg_namespace n on (t.relnamespace=n.oid)
    where  t.relkind='r' and n.nspname = 'public' and a.attnum > 0
    group by t.relname
    order by t.relname;
$$
language sql;
