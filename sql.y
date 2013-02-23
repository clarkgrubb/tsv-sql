%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYSTYPE char *

char *strdupf1(const char *, char *);
char *strdupf2(const char *, char *, char *);
char *strdupf3(const char *, char *, char *, char *);
char *strdupf4(const char *, char *, char *, char *, char *);
char *strdupf6(const char *, char *, char *, char *, char *, char *, char *);
void yyerror(const char *s);

char *top;
char *yylval;

%}

%token AND
%token AS
%token ASC
%token BY
%token CASE
%token CURRENT_TIMESTAMP
%token DESC
%token DISTINCT
%token ELSE
%token END_P
%token FALSE_P
%token FULL
%token FROM
%token GROUP_P
%token HAVING
%token INNER_P
%token IS
%token JOIN
%token LEFT
%token LIKE
%token LIMIT
%token NOT
%token NULL_P
%token OFFSET
%token ON
%token OR
%token ORDER
%token OUTER_P
%token RIGHT
%token SELECT
%token SIMILAR
%token THEN
%token TO
%token TRUE_P
%token WHEN
%token WHERE

%token GE
%token LE
%token NE
%token CONCAT

%token SCONST
%token ICONST
%token FCONST
%token IDENT


%left '+' '-'
%left '*' '/' '%'
%left '^'

%%

SelectStmt: simple_select { top = $$ = $1 }
| simple_select opt_select_limit { $$ = strdupf2("%s %s", $1, $2) }
| simple_select sort_clause opt_select_limit { $$ = strdupf3("%s %s %s", $1, $2, $3) }

simple_select: SELECT opt_distinct target_list from_clause where_clause group_clause having_clause
{ $$ = strdupf6("SELECT%s%s%s%s%s%s", $2, $3, $4, $5, $6, $7) }

opt_distinct: DISTINCT { $$ = strdup(" DISTINCT") }
     | /* EMPTY */ { $$ = strdup("") }

target_list: target_el { $$ = strdupf1(" %s", $1) }
| target_list ',' target_el { $$ = strdupf2(" %s, %s", $1, $3) }

target_el: a_expr AS ColLabel { $$ = strdupf2("%s AS %s", $1, $3) }
| a_expr { $$ = $1 }
| '*' { $$ = strdup("*")}

ColLabel: IDENT {}

from_clause: FROM from_list { $$ = strdupf1(" FROM %s", $2) }
| /* EMPTY */ { $$ = strdup("")}

from_list: table_ref { $$ = $1 }

table_ref: relation_expr { $$ = $1 }
| relation_expr alias_clause { $$ = strdupf2("%s %s", $1, $2) }
| joined_table { $$ = $1 }

joined_table: table_ref join_type JOIN table_ref join_qual { $$ = strdupf4("%s %s JOIN %s %s", $1, $2, $4, $5) }
| table_ref JOIN table_ref join_qual { $$ = strdupf3("%s JOIN %s %s", $1, $3, $4) }

join_type: FULL join_outer { $$ = strdup("FULL") }
| LEFT join_outer { $$ = strdup("LEFT") }
| RIGHT join_outer { $$ = strdup("RIGHT") }
| INNER_P { $$ = strdup("") }

join_outer: OUTER_P {}
| /* EMPTY */ {}

join_qual: ON IDENT '=' IDENT { $$ = strdupf2("ON %s = %s", $2, $4) }
| ON '(' IDENT '=' IDENT ')' { $$ = strdupf2("ON %s = %s", $3, $5) }

relation_expr: IDENT {}

alias_clause: AS IDENT { $$ = strdupf1("AS %s", $2) }

where_clause: WHERE a_expr { $$ = strdupf1(" WHERE %s", $2) }
| /* EMPTY */ { $$ = strdup("") }

/* IMPLEMENT */
group_clause: GROUP_P BY expr_list { $$ = strdupf1(" GROUP BY %s", $3) }
| /* EMPTY */ { $$ = strdup("") }

expr_list: a_expr {}
| expr_list ',' a_expr { }

/* IMPLEMENT */
having_clause: HAVING a_expr { $$ = strdupf1(" HAVING %s", $2) }
| /* EMPTY */ { $$ = strdup("") }

sort_clause: ORDER BY sortby_list { $$ = strdupf1("ORDER BY %s", $3) }

sortby_list: sortby { $$ = $1 }
| sortby_list ',' sortby { $$ = strdupf2("%s, %s", $1, $3) }

sortby: a_expr opt_asc_desc { $$ = strdupf2("%s %s", $1, $2) }

opt_asc_desc: ASC { $$ = strdup("ASC") }
| DESC { $$ = strdup("DESC") }
| /* EMPTY */ { $$ = strdup("") }

opt_select_limit: select_limit { $$ = $1 }
| /* EMPTY */ { $$ = strdup("") }

select_limit: limit_clause offset_clause { $$ = strdupf2("%s%s", $1, $2) }
| offset_clause limit_clause { $$ = strdupf2("%s%s", $1, $2) }
| limit_clause { $$ = $1 }
| offset_clause { $$ = $1 }

limit_clause: LIMIT a_expr { $$ = strdupf1(" LIMIT %s", $2)}

offset_clause: OFFSET a_expr { $$ = strdupf1(" OFFSET %s", $2) }

a_expr: c_expr { $$ = $1 }
| '+' a_expr { $$ = strdupf1("(+ %s)", $2) }
| '-' a_expr { $$ = strdupf1("(- %s)", $2) }
| a_expr '+' a_expr { $$ = strdupf2("(%s + %s)", $1, $3) }
| a_expr '-' a_expr { $$ = strdupf2("(%s - %s)", $1, $3) }
| a_expr '*' a_expr { $$ = strdupf2("(%s * %s)", $1, $3) }
| a_expr '/' a_expr { $$ = strdupf2("(%s / %s)", $1, $3) }
| a_expr '%' a_expr { $$ = strdupf2("(%s % %s)", $1, $3) }
| a_expr '^' a_expr { $$ = strdupf2("(%s ^ %s)", $1, $3) }
| a_expr '<' a_expr { $$ = strdupf2("(%s < %s)", $1, $3) }
| a_expr '>' a_expr { $$ = strdupf2("(%s > %s)", $1, $3) }
| a_expr '=' a_expr { $$ = strdupf2("(%s = %s)", $1, $3) }
| a_expr CONCAT a_expr { $$ = strdupf2("(%s || %s)", $1, $3) }
| a_expr AND a_expr { $$ = strdupf2("(%s AND %s)", $1, $3) }
| a_expr OR a_expr { $$ = strdupf2("(%s OR %s)", $1, $3) }
| NOT a_expr { $$ = strdupf1("(NOT %s)", $2) }
| a_expr LIKE a_expr { $$ = strdupf2("(%s LIKE %s)", $1, $3) }
| a_expr NOT LIKE a_expr { $$ = strdupf2("(%s NOT LIKE %s)", $1, $4) }
| a_expr SIMILAR TO a_expr { $$ = strdupf2("(%s SIMILAR TO %S)", $1, $4) }
| a_expr NOT SIMILAR TO a_expr { $$ = strdupf2("(%s NOT SIMILAR TO %s)", $1, $5) }
| a_expr IS NULL_P { $$ = strdupf1("(%s IS NULL)", $1) }
| a_expr IS NOT NULL_P { $$ = strdupf1("(%s IS NOT NULL)", $1) }

c_expr: columnref { $$ = $1 }
| AexprConst { $$ = $1 }
| '(' a_expr ')' { $$ = $2 }
| case_expr { $$ = $1 }
| func_expr { $$ = $1 }

columnref: IDENT { $$ = strdup(yylval) }

AexprConst: ICONST { $$ = $1 }
| FCONST { $$ = $1 }
| SCONST { $$ = $1 }
| TRUE_P { $$ = strdup("TRUE") }
| FALSE_P { $$ = strdup("FALSE") }
| NULL_P { $$ = strdup("NULL") }

case_expr: CASE case_arg when_clause_list case_default END_P { $$ = strdupf2("(CASE %s %s END)", $2, $3) }

when_clause_list: when_clause { $$ = $1 }
| when_clause_list when_clause { $$ = strdupf2("%s %s", $1, $2) }

when_clause: WHEN a_expr THEN a_expr { $$ = strdupf2("WHEN %s THEN %s", $2, $4) }

case_default: ELSE a_expr {}
| /*EMPTY*/ {}

case_arg: a_expr {}
| /*EMPTY*/ {}

func_expr: func_name '(' ')' { $$ = strdupf1("(%s())", $1) }
| func_name '(' func_arg_list ')' { $$ = strdupf2("(%s(%s))", $1, $3) }
| func_name '(' DISTINCT func_arg_list ')' { $$ = strdupf2("(%s(DISTINCT %s))", $1, $4) }
| func_name '(' '*' ')' { $$ = strdupf1("(%s(*))", $1) }
| CURRENT_TIMESTAMP { $$ = strdup("CURRENT_TIMESTAMP") }

func_name: IDENT {}

func_arg_list: func_arg_expr { $$ = $1 }
| func_arg_list ',' func_arg_expr { $$ = strdupf2("%s, %s", $1, $3) }

func_arg_expr: a_expr { $$ = $1 }


%%

int
main(int argc, char **argv) {
  char *awk;
  if ( 0 == yyparse() ) {
    printf("awk: %s\n", top);
  } else {
    fprintf(stderr, "parse failed\n");
  }
}

void
yyerror (const char *s)
{
  fprintf (stderr, "%s\n", s);
}

char *
strdupf1(const char *fmt, char *a) {
  char *c;
  int ret;
  ret = asprintf(&c, fmt, a);
  if (ret < 0) {
    fprintf(stderr, "could not allocate memory\n");
    exit(-1);
  }
  free(a);
  return (c);
}

char *
strdupf2(const char *fmt, char *a, char *b) {
  char *c;
  int ret;
  ret = asprintf(&c, fmt, a, b);
  if (ret < 0) {
    fprintf(stderr, "could not allocate memory\n");
    exit(-1);
  }
  free(a);
  free(b);
  return (c);
}

char *
strdupf3(const char *fmt,
         char *s1,
         char *s2,
         char *s3) {
  char *c;
  int ret;
  ret = asprintf(&c, fmt, s1, s2, s3);
  if (ret < 0) {
    fprintf(stderr, "could not allocate memory\n");
    exit(-1);
  }
  free(s1);
  free(s2);
  free(s3);
  return (c);
}

char *
strdupf4(const char *fmt,
         char *s1,
         char *s2,
         char *s3,
         char *s4) {
  char *c;
  int ret;
  ret = asprintf(&c, fmt, s1, s2, s3, s4);
  if (ret < 0) {
    fprintf(stderr, "could not allocate memory\n");
    exit(-1);
  }
  free(s1);
  free(s2);
  free(s3);
  free(s4);
  return (c);
}

char *
strdupf6(const char *fmt,
         char *s1,
         char *s2,
         char *s3,
         char *s4,
         char *s5,
         char *s6) {
  char *c;
  int ret;
  ret = asprintf(&c, fmt, s1, s2, s3, s4, s5, s6);
  if (ret < 0) {
    fprintf(stderr, "could not allocate memory\n");
    exit(-1);
  }
  free(s1);
  free(s2);
  free(s3);
  free(s4);
  free(s5);
  free(s6);
  return (c);
}
