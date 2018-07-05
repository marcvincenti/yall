/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#ifndef builtin_h
#define builtin_h

#include "evaluation.h"

lval* builtin_list(lval*);
lval* builtin_eval(lval*);
lval* builtin_first(lval*);
lval* builtin_rest(lval*);
lval* builtin_len(lval*);
lval* builtin_join(lval*);

#endif
