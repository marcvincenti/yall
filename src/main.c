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

#define BUFFERSIZE 2048

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
     char input[BUFFERSIZE];
     mpc_result_t r;

     fputs("> ", stdout);
     fgets(input, BUFFERSIZE , stdin);

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
