#include "evaluation.h"
#include "y_math.h"

#define LASSERT(args, cond, err) \
  if (!(cond)) { lval_del(args); return lval_err(err); }

lval* builtin_list(lenv* e, lval* a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval* builtin_eval(lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'eval' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'eval' passed incorrect type!");

  lval* x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(e, x);
}

lval* builtin_first(lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'first' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'first' passed incorrect type!");
  LASSERT(a, a->cell[0]->count != 0,
    "Function 'first' passed '()!");

  lval* v = lval_take(a, 0);
  while (v->count > 1) { lval_del(lval_pop(v, 1)); }
  return v;
}

lval* builtin_rest(lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'rest' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'rest' passed incorrect type!");
  LASSERT(a, a->cell[0]->count != 0,
    "Function 'rest' passed '()!");

  lval* v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval* builtin_len(lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'len' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'len' passed incorrect type!");

  lval* x = lval_num_int(a->cell[0]->count);
  lval_del(a);
  return x;
}

lval* builtin_join(lenv* e, lval* a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXPR,
      "Function 'join' passed incorrect type.");
  }

  lval* x = lval_pop(a, 0);
  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }
  lval_del(a);
  return x;
}

lval* builtin_math_op(lenv* e, lval* a, char* op) {
  /* Ensure all arguments are numbers */
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM_INT && a->cell[i]->type != LVAL_NUM_DEC) {
      lval_del(a);
      return lval_err("Cannot operate on non-number!");
    }
  }
  /* Pop the first element */
  lval* x = lval_pop(a, 0);
  /* If no arguments and sub then perform unary negation */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    if (x->type == LVAL_NUM_INT) {
      x->data.num_int = -x->data.num_int;
    } else if (x->type == LVAL_NUM_DEC) {
      x->data.num_dec = -x->data.num_dec;
    }

  }
  /* While there are still elements remaining */
  while (a->count > 0) {
    /* Pop the next element */
    lval* y = lval_pop(a, 0);
    /* Perform operation */
    if (strcmp(op, "+") == 0) { y_math_add(x, y); }
    if (strcmp(op, "-") == 0) { y_math_sub(x, y); }
    if (strcmp(op, "*") == 0) { y_math_mul(x, y); }
    if (strcmp(op, "/") == 0) { y_math_div(x, y); }
    if (strcmp(op, "%") == 0) { y_math_mod(x, y); }
    if (strcmp(op, "^") == 0) { y_math_pow(x, y); }
    /* Delete element now finished with */
    lval_del(y);
  }
  /* Delete input expression and return result */
  lval_del(a);
  return x;
}

lval* builtin_add(lenv* e, lval* a) {
  return builtin_math_op(e, a, "+");
}

lval* builtin_sub(lenv* e, lval* a) {
  return builtin_math_op(e, a, "-");
}

lval* builtin_mul(lenv* e, lval* a) {
  return builtin_math_op(e, a, "*");
}

lval* builtin_div(lenv* e, lval* a) {
  return builtin_math_op(e, a, "/");
}

lval* builtin_mod(lenv* e, lval* a) {
  return builtin_math_op(e, a, "%");
}

lval* builtin_pow(lenv* e, lval* a) {
  return builtin_math_op(e, a, "^");
}
