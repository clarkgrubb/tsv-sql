tsv-sql: tsv_sql.tab.c lex.yy.c
	gcc lex.yy.c tsv_sql.tab.c -o tsv-sql

tsv_sql.tab.c: tsv_sql.y
	bison -d tsv_sql.y

tsv_sql.tab.h: tsv_sql.tab.c

lex.yy.c: tsv_sql.lex tsv_sql.tab.h
	flex tsv_sql.lex

clobber: clean
	rm tsv-sql

clean:
	rm -f tsv_sql.tab.c tsv_sql.tab.h lex.yy.c
