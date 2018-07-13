/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#ifndef evaluation_h
#define evaluation_h

#include "types.h"
#include "mpc.h"

lval* lval_num_int(long);
lval* lval_num_dec(double);
lval* lval_fun(lbuiltin);
lval* lval_err(char*);
lval* lval_sym(char*);
lval* lval_sexpr();
lval* lval_qexpr();

lval* lval_add(lval*, lval*);
lval* lval_copy(lval*);
lval* lval_join(lval*, lval*);
lval* lval_pop(lval*, int);
lval* lval_take(lval*, int);
void lval_del(lval*);

void lval_print(lval*);
void lval_expr_print(lval*, char*, char*);
void lval_println(lval*);

lval* lval_eval(lenv*, lval*);
lval* lval_eval_sexpr(lenv*, lval*);
lval* lval_read_num(mpc_ast_t*) ;
lval* lval_read(mpc_ast_t*);

#endif
