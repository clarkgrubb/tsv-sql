tsv-sql: sql.tab.c lex.yy.c tsv-sql.c
	gcc lex.yy.c sql.tab.c tsv-sql.c -o tsv-sql

sql.tab.c: sql.y
	bison -d sql.y

sql.tab.h: sql.tab.c

lex.yy.c: sql.l sql.tab.h
	flex sql.l

clobber: clean
	rm tsv-sql

clean:
	rm -f sql.tab.c sql.tab.h lex.yy.c
