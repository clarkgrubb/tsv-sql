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
%token OR
%token NOT
%token GT
%token LT
%token GE
%token LE
%token EQ
%token NE
%token INTEGER
%token NUMERIC

%left '+' '-'
%left '*' '/' '%'
%left '^'

%%

arithmetic_expr: arithmetic_expr2 { top = $$ = $1 }
| arithmetic_expr2 '+' arithmetic_expr { top = $$ = strdupf2("(%s + %s)", $1, $3) }
| arithmetic_expr2 '-' arithmetic_expr { top = $$ = strdupf2("(%s - %s)", $1, $3) }

arithmetic_expr2: arithmetic_expr3 { top = $$ = $1 }
| arithmetic_expr3 '*' arithmetic_expr2 { top = $$ = strdupf2("(%s * %s)", $1, $3) }
| arithmetic_expr3 '/' arithmetic_expr2 { top = $$ = strdupf2("(%s / %s)", $1, $3) }
| arithmetic_expr3 '%' arithmetic_expr2 { top = $$ = strdupf2("(%s % %s)", $1, $3) }

arithmetic_expr3: arithmetic_expr4 { top = $$ = $1 }
| arithmetic_expr4 '^' arithmetic_expr3 { top = $$ = strdupf2("(%s ^ %s)", $1, $3) }

arithmetic_expr4: INTEGER { top = $$ = yylval }
| '(' arithmetic_expr ')' { top = $$ = strdupf1("(%s)", $2) }

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
