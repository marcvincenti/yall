/*
** yall - yet another lisp language
** https://github.com/mvrcrypto/yall
**
** Marc Vincenti
** Licensed under CC BY-SA 4.0
*/

#ifndef y_math_h
#define y_math_h

#include "types.h"

void y_math_add(lval*, lval*);
void y_math_sub(lval*, lval*);
void y_math_mul(lval*, lval*);
void y_math_div(lval*, lval*);
void y_math_mod(lval*, lval*);
void y_math_pow(lval*, lval*);

void y_math_max(lval*, lval*);
void y_math_min(lval*, lval*);

#endif
