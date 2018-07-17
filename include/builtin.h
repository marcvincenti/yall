/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#ifndef builtin_h
#define builtin_h

#include "types.h"

/* Functions */
lval* builtin_def(lenv*, lval*);

/* List */
lval* builtin_list(lenv*, lval*);
lval* builtin_eval(lenv*, lval*);
lval* builtin_first(lenv*, lval*);
lval* builtin_rest(lenv*, lval*);
lval* builtin_len(lenv*, lval*);
lval* builtin_join(lenv*, lval*);

/* Math operations */
lval* builtin_add(lenv*, lval*);
lval* builtin_sub(lenv*, lval*);
lval* builtin_mul(lenv*, lval*);
lval* builtin_div(lenv*, lval*);
lval* builtin_mod(lenv*, lval*);
lval* builtin_pow(lenv*, lval*);

#endif
