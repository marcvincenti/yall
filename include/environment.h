/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#ifndef environment_h
#define environment_h

#include "types.h"

lenv* lenv_new(void);
void lenv_del(lenv*);
lval* lenv_get(lenv*, lval*);
void lenv_put(lenv*, lval*, lval*);

void lenv_add_builtin(lenv*, char*, lbuiltin);
void lenv_add_default_builtins(lenv*);

#endif
