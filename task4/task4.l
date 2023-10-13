%{
#include "y.tab.h" 
int yylex(void); 
%}

%%

[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[-+*/\n()]  { return yytext[0]; }
.           { }

%%

