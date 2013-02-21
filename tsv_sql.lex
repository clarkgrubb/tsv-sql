%{
#define YYSTYPE char *
#include "tsv_sql.tab.h"

%}

%option noyywrap

%%

and { return AND; }
or { return OR; }
not { return NOT; }
"<" { return LT; }
">" { return GT; }
"=" { return EQ; }
"!=" { return NE; }
"<>" { return NE; }
"<=" { return LE; }
">=" { return GE; }
"(" { return '('; }
")" { return ')'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"%" { return '%'; }
"^" { return '^'; }
-?[0-9]*\.[0-9]+ { yylval = strdup(yytext); return NUMERIC; }
-?[0-9]+\.[0-9]* { yylval = strdup(yytext); return NUMERIC; }
-?[0-9]+ { yylval = strdup(yytext); return INTEGER; }
[ \t\n]+          /* eat up whitespace */
. { printf( "Unrecognized character: %s\n", yytext ); }

%%

/*
int
main(int argc, char **argv) {
  int token;
  while ((token = yylex())) {

  }
  return (0);
}
*/
