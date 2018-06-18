/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#ifndef evaluation_h
#define evaluation_h

#include "mpc.h"

union u_lval_value {
   long l;
   double d;
 };

/* Create Enumeration of Possible lval Types */
enum e_lval_type { LVAL_NUM, LVAL_DEC, LVAL_ERR };

/* Create Enumeration of Possible Error Types */
enum e_lval_error { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* Declare New lval Struct */
typedef struct {
  union u_lval_value val;
  enum e_lval_type type;
  enum e_lval_error err;
} lval;


lval lval_num(long);
lval lval_dec(double);
lval lval_err(enum e_lval_error);

void lval_print(lval);
void lval_println(lval);

lval eval_op(char*, lval, lval);

lval eval(mpc_ast_t*);

#endif
