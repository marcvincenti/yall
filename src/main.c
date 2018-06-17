/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#include <stdio.h>
#include "mpc.h"
#include "y_math.h"

#define BUFFERSIZE 2048

/* Use operator string to see which operation to perform */
long eval_op(char* op, long x, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  if (strcmp(op, "%") == 0) { return x % y; }
  if (strcmp(op, "^") == 0) { return y_math_pow(x,y); }
  if (strcmp(op, "max") == 0) { return y_math_max(x,y); }
  if (strcmp(op, "min") == 0) { return y_math_min(x,y); }
  return 0;
}

long eval(mpc_ast_t* t) {

  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  long x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(op, x, eval(t->children[i]));
    i++;
  }

  return x;
}


int main(int argc, char** argv) {

  /* Create Some Parsers */
 mpc_parser_t* Number   = mpc_new("number");
 mpc_parser_t* Operator = mpc_new("operator");
 mpc_parser_t* Function = mpc_new("function");
 mpc_parser_t* Expr     = mpc_new("expr");
 mpc_parser_t* Yall     = mpc_new("yall");

 /* Define them with the following Language */
 mpca_lang(MPCA_LANG_DEFAULT,
   "                                                                    \
     number   : /[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)/ ;                 \
     operator : '+' | '-' | '*' | '/' | '%' | '^' ;                     \
     function : \"min\" | \"max\" | <operator> ;                        \
     expr     : <number> | '(' <function> <expr>+ ')' ;                 \
     yall     : /^/ <function> <expr>+ /$/ ;                            \
   ",
   Number, Operator, Function, Expr, Yall);

   while(1) {
     char input[BUFFERSIZE];
     mpc_result_t r;

     fputs("> ", stdout);
     fgets(input, BUFFERSIZE , stdin);

     if (mpc_parse("<stdin>", input, Yall, &r)) {
       /* Attempt to parse the user input */
       long result = eval(r.output);
       printf("%li\n", result);
       mpc_ast_delete(r.output);
     } else {
       /* Otherwise print and delete the Error */
       mpc_err_print(r.error);
       mpc_err_delete(r.error);
     }

   }

  /* Undefine and delete our parsers */
  mpc_cleanup(5, Number, Operator, Function, Expr, Yall);

  return 0;
}
