-- Ian Dunkerley's solution

create or replace function schema3() returns setof text
as $$
declare
    foo record;
    relation text;
    result text;
begin
    for foo in select table_name, column_name
               from information_schema.columns
               where table_schema = 'public'
               order by table_name, ordinal_position
    loop
        if relation is null then
            relation := foo.table_name;
            result := relation||'('||foo.column_name;
        elsif relation = foo.table_name then
            result := result||', '||foo.column_name;
        else
            return next result||')';
            relation := foo.table_name;
            result := relation||'('||foo.column_name;
        end if;
    end loop;
    return next result||')';
end;
$$ language plpgsql;
