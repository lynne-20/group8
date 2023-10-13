%{
#include <stdio.h>
%}

%token NUMBER
%left '+' '-'
%left '*' '/'

%%

input: /* empty */
     | input line
     ;

line: '\n'
    | exp '\n' { printf("Result: %d\n", $1); }
    ;


exp: exp '+' exp { $$ = $1 + $3; }
   | exp '-' exp { $$ = $1 - $3; }
   | exp '*' exp { $$ = $1 * $3; }
   | exp '/' exp { $$ = $1 / $3; }
   | '(' exp ')' { $$ = $2; }
   | NUMBER { $$ = $1; }
   ;

%%

int main() {
    yyparse(); // Start the parser
    return 0;
}

