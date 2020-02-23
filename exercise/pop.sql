-- Assumes: create type TableInfo as ("table" text, ntuples int);

drop type if exists TableInfo cascade;
create type TableInfo as ( "table" text, ntuples integer);

create or replace
	function pop() returns setof TableInfo
as $$
declare
   r record;      -- result tuples from out query
   nr integer;    -- count of tuples
   res TableInfo; -- output tuple
begin
   for r in select tablename
       from pg_tables
       where schemaname = 'public'
       order by tablename
   loop
				-- create a query via string concatentation and execute it
				-- store result of query in a local variable (nr)
      execute 'select count(*) from ' ||quote_ident(r.tablename)
				into nr;
				-- set values of attributesin output tuple
      res."table" := r.tablename; res.ntuples := nr;
				-- add output tuple to results
      return next res;
   end loop;
end;
$$ language plpgsql;

-- Execute this function as: select * from pop();
