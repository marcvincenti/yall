/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#include <stdio.h>
#include "evaluation.h"
#include "mpc.h"

#ifdef _WIN32 //Windows
#include <string.h>
static char buffer[2048];
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}
void add_history(char* unused) {}
#elif __APPLE__ //Mac OS
#include <editline/readline.h>
#include "TargetConditionals.h"
#elif __linux // linux
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

  /* Create Some Parsers */
  mpc_parser_t* Integer = mpc_new("integer");
  mpc_parser_t* Decimal = mpc_new("decimal");
  mpc_parser_t* Symbol  = mpc_new("symbol");
  mpc_parser_t* Sexpr   = mpc_new("sexpr");
  mpc_parser_t* Expr    = mpc_new("expr");
  mpc_parser_t* Yall    = mpc_new("yall");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                       \
      integer : /[+-]?[0-9]+/ ;                             \
      decimal : /[+-]?(([0-9]+\\.[0-9]*)|(\\.[0-9]+))/ ;    \
      symbol  : '+' | '-' | '*' | '/' | '%' | '^' ;         \
      sexpr   : '(' <expr>* ')' ;                           \
      expr    : <integer> | <decimal> | <symbol> | <sexpr> ;\
      yall    : /^/ <expr>* /$/ ;                           \
    ",
    Integer, Decimal, Symbol, Sexpr, Expr, Yall);

   while(1) {
     char* input = readline("> ");
     add_history(input);

     mpc_result_t r;

     if (mpc_parse("<stdin>", input, Yall, &r)) {
       /* Attempt to parse the user input */
       lval* x = lval_eval(lval_read(r.output));
       lval_println(x);
       lval_del(x);
       mpc_ast_delete(r.output);
     } else {
       /* Otherwise print and delete the Error */
       mpc_err_print(r.error);
       mpc_err_delete(r.error);
     }

   }

  /* Undefine and delete our parsers */
  mpc_cleanup(6, Integer, Decimal, Symbol, Sexpr, Expr, Yall);

  return 0;
}
