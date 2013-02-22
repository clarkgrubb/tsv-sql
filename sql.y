%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYSTYPE char *

char *strdupf1(const char *, char *);
char *strdupf2(const char *, char *, char *);
void yyerror(const char *s);

char *top;
char *yylval;

%}

%token AND
%token ASC
%token BY
%token CASE
%token CURRENT_TIMESTAMP
%token DESC
%token DISTINCT
%token ELSE
%token END_P
%token FALSE_P
%token IS
%token LIKE
%token NOT
%token NULL_P
%token OR
%token ORDER
%token SIMILAR
%token THEN
%token TO
%token TRUE_P
%token WHEN

%token GT
%token LT
%token GE
%token LE
%token EQ
%token NE

%token SCONST
%token ICONST
%token FCONST
%token IDENT


%left '+' '-'
%left '*' '/' '%'
%left '^'

%%

 /*
SelectStmt: simple_select {}
| simple_select sort_clause {}

simple_select:
 */


 /* sort_clause: ORDER BY sortby_list { top = $$ = strdupf1("ORDER BY %s", $2) }

sortby_list: sortby { top = $$ = $1 }
| sortby_list ',' sortby { top = $$ = strdupf2("%s, %s", $1, $3) }

sortby: a_expr opt_asc_desc { top = $$ = strdupf2("%s %s", $1, $2) }

opt_asc_desc: ASC { top = $$ = strdup("ASC") }
| DESC { top = $$ = strdup("DESC") } */

a_expr: c_expr { top = $$ = $1 }
| '+' a_expr { top = $$ = strdupf1("(+ %s)", $2) }
| '-' a_expr { top = $$ = strdupf1("(- %s)", $2) }
| a_expr '+' a_expr { top = $$ = strdupf2("(%s + %s)", $1, $3) }
| a_expr '-' a_expr { top = $$ = strdupf2("(%s - %s)", $1, $3) }
| a_expr '*' a_expr { top = $$ = strdupf2("(%s * %s)", $1, $3) }
| a_expr '/' a_expr { top = $$ = strdupf2("(%s / %s)", $1, $3) }
| a_expr '%' a_expr { top = $$ = strdupf2("(%s % %s)", $1, $3) }
| a_expr '^' a_expr { top = $$ = strdupf2("(%s ^ %s)", $1, $3) }
| a_expr '<' a_expr { top = $$ = strdupf2("(%s < %s)", $1, $3) }
| a_expr '>' a_expr { top = $$ = strdupf2("(%s > %s)", $1, $3) }
| a_expr '=' a_expr { top = $$ = strdupf2("(%s = %s)", $1, $3) }
| a_expr AND a_expr { top = $$ = strdupf2("(%s AND %s)", $1, $3) }
| a_expr OR a_expr { top = $$ = strdupf2("(%s OR %s)", $1, $3) }
| NOT a_expr { top = $$ = strdupf1("(NOT %s)", $2) }
| a_expr LIKE a_expr { top = $$ = strdupf2("(%s LIKE %s)", $1, $3) }
| a_expr NOT LIKE a_expr { top = $$ = strdupf2("(%s NOT LIKE %s)", $1, $4) }
| a_expr SIMILAR TO a_expr { top = $$ = strdupf2("(%s SIMILAR TO %S)", $1, $4) }
| a_expr NOT SIMILAR TO a_expr { top = $$ = strdupf2("(%s NOT SIMILAR TO %s)", $1, $5) }
| a_expr IS NULL_P { top = $$ = strdupf1("(%s IS NULL)", $1) }
| a_expr IS NOT NULL_P { top = $$ = strdupf1("(%s IS NOT NULL)", $1) }

c_expr: columnref { top = $$ = $1 }
| AexprConst { top = $$ = $1 }
| '(' a_expr ')' { top = $$ = $2 }
| case_expr { top = $$ = $1 }
| func_expr { top = $$ = $1 }

columnref: IDENT { top = $$ = strdup(yylval) }

AexprConst: ICONST { top = $$ = $1 }
| FCONST { top = $$ = $1 }
| SCONST { top = $$ = $1 }
| TRUE_P { top = $$ = strdup("TRUE") }
| FALSE_P { top = $$ = strdup("FALSE") }
| NULL_P { top = $$ = strdup("NULL") }

case_expr: CASE case_arg when_clause_list case_default END_P { top = $$ = strdupf2("(CASE %s %s END)", $2, $3) }

when_clause_list: when_clause { top = $$ = $1 }
| when_clause_list when_clause { top = $$ = strdupf2("%s %s", $1, $2) }

when_clause: WHEN a_expr THEN a_expr { top = $$ = strdupf2("WHEN %s THEN %s", $2, $4) }

case_default: ELSE a_expr {}
| /*EMPTY*/ {}

case_arg: a_expr {}
| /*EMPTY*/ {}

func_expr: func_name '(' ')' { top = $$ = strdupf1("(%s())", $1) }
| func_name '(' func_arg_list ')' { top = $$ = strdupf2("(%s(%s))", $1, $3) }
| func_name '(' DISTINCT func_arg_list ')' { top = $$ = strdupf2("(%s(DISTINCT %s))", $1, $4) }
| func_name '(' '*' ')' { top = $$ = strdupf1("(%s(*))", $1) }
| CURRENT_TIMESTAMP { top = $$ = strdup("CURRENT_TIMESTAMP") }

func_name: IDENT {}

func_arg_list: func_arg_expr { top = $$ = $1 }
| func_arg_list ',' func_arg_expr { top = $$ = strdupf2("%s, %s", $1, $3) }

func_arg_expr: a_expr { top = $$ = $1 }


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
