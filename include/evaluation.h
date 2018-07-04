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

/* Add SYM and SEXPR as possible lval types */
enum { LVAL_ERR, LVAL_NUM_INT, LVAL_NUM_DEC, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR };

typedef struct lval {
  int type;
  union {
    /* Numbers types */
    long num_int;
    double num_dec;
    /* Error and Symbol types have some string data */
    char* err;
    char* sym;
  } data;
  /* Count and Pointer to a list of "lval*"; */
  int count;
  struct lval** cell;
} lval;


lval* lval_num_int(long);
lval* lval_num_dec(double);
lval* lval_err(char*);
lval* lval_sym(char*);
lval* lval_sexpr();
lval* lval_qexpr();

void lval_del(lval*);
lval* lval_add(lval*, lval*);
lval* lval_pop(lval*, int);
lval* lval_take(lval*, int);

void lval_print(lval*);
void lval_expr_print(lval*, char*, char*);
void lval_println(lval*);

lval* builtin_op(lval*, char*);
lval* lval_eval(lval*);
lval* lval_eval_sexpr(lval*);
lval* lval_read_num(mpc_ast_t*) ;
lval* lval_read(mpc_ast_t*);

#endif
