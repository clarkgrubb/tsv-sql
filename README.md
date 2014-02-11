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
* relation aliases can only be used once per query (subqueries have their own scope)
* same table can have multiple aliases (does file need to be passed multiple times?)
* column aliases must be unique for the relation
* use file names as table names (what about bad characters?)
* support no input tables and no from clause

TYPES
=====
* casting
* declare with CREATE TABLE
* declare with header notation

FLAGS
========
* -x explain plan
* -D don't delete tmp files
* -f flag to put SQL in a file

TODO
====
* buffer overflow in backend?
* error handling function which reports func name
* tmpfile cleanup

LIMITATIONS
===========
* join conditions must be equality of two columns
* only one from table
* NULL is the empty string
* strings are double quoted (for ease of use on cmd line)
* types: columns are text and must be cast to get a numerical sort
* subqueries in from/join clause only
* no common table expressions
* list of supported functions

COLUMN NAMES
============
* head of TSV file
* AS (if provided)
* head of expression (function name or aggregation function)
* binary operators: add, subtract, multiply, divide, ...
* add number suffixes to disambiguate
* join columns are ALIAS.COLUMN
* query columns are join columns with ALIAS removed; error if name collision
