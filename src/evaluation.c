#include <stdio.h>
#include "evaluation.h"
#include "y_math.h"

/* Create a new number type lval */
lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.val.l = x;
  return v;
}

lval lval_dec(double x) {
  lval v;
  v.type = LVAL_DEC;
  v.val.d = x;
  return v;
}

/* Create a new error type lval */
lval lval_err(enum e_lval_error x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

/* Print an "lval" */
void lval_print(lval v) {
  switch (v.type) {

    case LVAL_NUM:
      printf("%li", v.val.l);
      break;

    case LVAL_DEC:
      printf("%f", v.val.d);
      break;

    /* In the case the type is an error */
    case LVAL_ERR:
      switch (v.err) {
        case LERR_DIV_ZERO:
          printf("Error: Division By Zero!");
          break;
        case LERR_BAD_OP:
          printf("Error: Invalid Operator!");
          break;
        case LERR_BAD_NUM:
          printf("Error: Invalid Number!");
          break;
      }
    break;
  }
}

/* Print an "lval" followed by a newline */
void lval_println(lval v) { lval_print(v); putchar('\n'); }

/* Use operator string to see which operation to perform */
lval eval_op(char* op, lval x, lval y) {

  /* If either value is an error return it */
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }

  if (strcmp(op, "+") == 0) { return y_math_add(x, y); }
  if (strcmp(op, "-") == 0) { return y_math_sub(x, y); }
  if (strcmp(op, "*") == 0) { return y_math_mul(x, y); }
  if (strcmp(op, "/") == 0) { return y_math_div(x, y); }
  if (strcmp(op, "%") == 0) { return y_math_mod(x, y); }
  if (strcmp(op, "^") == 0) { return y_math_pow(x, y); }
  if (strcmp(op, "max") == 0) { return y_math_max(x, y); }
  if (strcmp(op, "min") == 0) { return y_math_min(x, y); }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {

  if (strstr(t->tag, "number")) {
    /* Check if there is some error in conversion */
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    if (errno != ERANGE) { return lval_num(x); }
    else { return lval_err(LERR_BAD_NUM); }
  }

  if (strstr(t->tag, "decimal")) {
    /* Check if there is some error in conversion */
    errno = 0;
    double x = strtod(t->contents, NULL);
    if (errno != ERANGE) { return lval_dec(x); }
    else { return lval_err(LERR_BAD_NUM); }
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  lval x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(op, x, eval(t->children[i]));
    i++;
  }

  return x;
}
