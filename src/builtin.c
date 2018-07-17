#include "environment.h"
#include "evaluation.h"
#include "y_math.h"

#define LASSERT(args, cond, fmt, ...) \
  if (!(cond)) { \
    lval* err = lval_err(fmt, __VA_ARGS__); \
    lval_del(args); \
    return err; \
  }

#define LASSERT_TYPE(func, args, index, expect) \
  LASSERT(args, args->cell[index]->type == expect, \
    "Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
    func, index, ltype_name(args->cell[index]->type), ltype_name(expect))

#define LASSERT_NUM(func, args, num) \
  LASSERT(args, args->count == num, \
    "Function '%s' passed incorrect number of arguments. Got %i, Expected %i.", \
    func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index) \
  LASSERT(args, args->cell[index]->count != 0, \
    "Function '%s' passed {} for argument %i.", func, index);

lval* builtin_def(lenv* e, lval* a) {
  LASSERT_TYPE("def", a, 0, LVAL_QEXPR);
  /* First argument is symbol list */
  lval* syms = a->cell[0];
  /* Ensure all elements of first list are symbols */
  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
      "Function 'def' cannot define non-symbol. "
      "Got %s, Expected %s.",
      ltype_name(syms->cell[i]->type), ltype_name(LVAL_SYM));
  }
  /* Check correct number of symbols and values */
  LASSERT(a, (syms->count == a->count-1),
    "Function 'def' passed too many arguments for symbols. "
    "Got %i, Expected %i.",
    syms->count, a->count-1);
  /* Assign copies of values to symbols */
  for (int i = 0; i < syms->count; i++) {
    lenv_put(e, syms->cell[i], a->cell[i+1]);
  }
  lval_del(a);
  return lval_sexpr();
}

lval* builtin_list(lenv* e, lval* a) {
  a->type = LVAL_QEXPR;
  return a;
}

char* ltype_name(int t) {
  switch(t) {
    case LVAL_FUN: return "Function";
    case LVAL_NUM_INT: return "Integer";
    case LVAL_NUM_DEC: return "Decimal";
    case LVAL_ERR: return "Error";
    case LVAL_SYM: return "Symbol";
    case LVAL_SEXPR: return "S-Expression";
    case LVAL_QEXPR: return "Q-Expression";
    default: return "Unknown";
  }
}

lval* builtin_eval(lenv* e, lval* a) {
  LASSERT_NUM("eval", a, 1);
  LASSERT_TYPE("eval", a, 0, LVAL_QEXPR);
  lval* x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(e, x);
}

lval* builtin_first(lenv* e, lval* a) {
  LASSERT_NUM("first", a, 1);
  LASSERT_TYPE("first", a, 0, LVAL_QEXPR);
  LASSERT_NOT_EMPTY("first", a, 0);
  lval* v = lval_take(a, 0);
  while (v->count > 1) { lval_del(lval_pop(v, 1)); }
  return v;
}

lval* builtin_rest(lenv* e, lval* a) {
  LASSERT_NUM("rest", a, 1);
  LASSERT_TYPE("rest", a, 0, LVAL_QEXPR);
  LASSERT_NOT_EMPTY("rest", a, 0);
  lval* v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval* builtin_len(lenv* e, lval* a) {
  LASSERT_NUM("rest", a, 1);
  LASSERT_TYPE("rest", a, 0, LVAL_QEXPR);
  lval* x = lval_num_int(a->cell[0]->count);
  lval_del(a);
  return x;
}

lval* builtin_join(lenv* e, lval* a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT_TYPE("join", a, i, LVAL_QEXPR);
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
    LASSERT(a, a->cell[i]->type == LVAL_NUM_INT || a->cell[i]->type == LVAL_NUM_DEC, \
      "Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
      op, i, ltype_name(a->cell[i]->type), "Number");
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
