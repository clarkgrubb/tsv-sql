%{
#define YYSTYPE char *
#include "sql.tab.h"

%}

%option noyywrap

%%

[aA][nN][dD] { return AND; }
[aA][sS] { return AS; }
[aA][sS][cC] { return ASC; }
[bB][yY] { return BY; }
[cC][aA][sS][eE] { return CASE; }
[cC][uU][rR][rR][eE][nN][tT]_[tT][iI][mM][eE][sS][tT][aA][mM][pP] { return CURRENT_TIMESTAMP; }
[dD][eE][sS][cC] { return DESC; }
[dD][iI][sS][tT][iI][nN][cC][tT] { return DISTINCT; }
[eE][lL][sS][eE] { return ELSE; }
[eE][nN][dD] { return END_P; }
[fF][aA][lL][sS][eE] { return FALSE_P; }
[fF][uU][lL][lL] { return FULL; }
[fF][rR][oO][mM] { return FROM; }
[gG][rR][oO][uU][pP] { return GROUP_P; }
[hH][aA][vV][iI][nN][gG] { return HAVING; }
[iI][nN][nN][eE][rR] { return INNER_P; }
[iI][sS] { return IS; }
[jJ][oO][iI][nN] { return JOIN; }
[lL][eE][fF][tT] { return LEFT; }
[lL][iI][kK][eE] { return LIKE; }
[lL][iI][mM][iI][tT] { return LIMIT; }
[nN][oO][tT] { return NOT; }
[nN][uU][lL][lL] { return NULL_P; }
[oO][fF][fF][sS][eE][tT] { return OFFSET; }
[oO][nN] { return ON; }
[oO][rR] { return OR; }
[oO][rR][dD][eE][rR] { return ORDER; }
[oO][uU][tT][eE][rR] { return OUTER_P; }
[rR][iI][gG][hH][tT] { return RIGHT; }
[sS][eE][lL][eE][cC][tT] { return SELECT; }
[sS][iI][mM][iI][lL][aA][rR] { return SIMILAR; }
[tT][hH][eE][nN] { return THEN; }
[tT][oO] { return TO; }
[tT][rR][uU][eE] { return TRUE_P; }
[wW][hH][eE][nN] { return WHEN; }
[wW][hH][eE][rR][eE] { return WHERE; }
"<" { return '<'; }
">" { return '>'; }
"=" { return '='; }
"!=" { return NE; }
"<>" { return NE; }
"<=" { return LE; }
">=" { return GE; }
"," { return ','; }
"(" { return '('; }
")" { return ')'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"%" { return '%'; }
"^" { return '^'; }
\"([^\"]|\\.)*\" { yylval = strdup(yytext); return SCONST; }
-?[0-9]*\.[0-9]+ { yylval = strdup(yytext); return FCONST; }
-?[0-9]+\.[0-9]* { yylval = strdup(yytext); return FCONST; }
-?[0-9]+ { yylval = strdup(yytext); return ICONST; }
[a-zA-Z][a-zA-Z0-9_]* {yylval = strdup(yytext); return IDENT; }
[ \t\n]+          /* eat up whitespace */
. { printf( "Unrecognized character: %s\n", yytext ); }

%%
