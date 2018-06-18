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
 mpc_parser_t* Number   = mpc_new("number");
 mpc_parser_t* Decimal  = mpc_new("decimal");
 mpc_parser_t* Operator = mpc_new("operator");
 mpc_parser_t* Function = mpc_new("function");
 mpc_parser_t* Expr     = mpc_new("expr");
 mpc_parser_t* Yall     = mpc_new("yall");

 /* Define them with the following Language */
 mpca_lang(MPCA_LANG_DEFAULT,
   "                                                                    \
     number   : /[+-]?[0-9]+/ ;                                         \
     decimal  : /[+-]?(([0-9]+\\.[0-9]*)|(\\.[0-9]+))/ ;                \
     operator : '+' | '-' | '*' | '/' | '%' | '^' ;                     \
     function : \"min\" | \"max\" | <operator> ;                        \
     expr     : <number> | <decimal> | '(' <function> <expr>+ ')' ;     \
     yall     : /^/ <function> <expr>+ /$/ ;                            \
   ",
   Number, Decimal, Operator, Function, Expr, Yall);

   while(1) {
     char input[BUFFERSIZE];
     mpc_result_t r;

     fputs("> ", stdout);
     fgets(input, BUFFERSIZE , stdin);

     if (mpc_parse("<stdin>", input, Yall, &r)) {
       /* Attempt to parse the user input */
       lval result = eval(r.output);
       lval_println(result);
       mpc_ast_delete(r.output);
     } else {
       /* Otherwise print and delete the Error */
       mpc_err_print(r.error);
       mpc_err_delete(r.error);
     }

   }

  /* Undefine and delete our parsers */
  mpc_cleanup(6, Number, Decimal, Operator, Function, Expr, Yall);

  return 0;
}
