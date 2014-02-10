tsv-sql
=======

Execute a SQL SELECT statement at the command line on TSV files

IR ISSUES
=========
* how to we convert table alias to table
* table vs relation vs join
* aggregation_type is not used
* nested queries

ALIASES
=======
* do they have scope?

  create table foo (one text, two text, three text);
  create table bar (one text, two text, three text);

  select b.one, a.three
  from foo a
  join (
    select one, two
    from bar
  ) as b
  on b.two = a.two;


TODO
====
* buffer overflow in backend?
* error handling function which reports func name
* tmpfile cleanup


====
query
relation
column
target
expression