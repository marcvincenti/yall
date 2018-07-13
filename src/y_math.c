#include "evaluation.h"

void cast_numbers(lval* x, lval* y) {
  if (x->type != y->type) {
    /* type cast integer to decimal for operation on numbers */
    if (x->type == LVAL_NUM_INT) {
      x->data.num_dec = (double) x->data.num_int;
      x->type = LVAL_NUM_DEC;
    }
    if (y->type == LVAL_NUM_INT) {
      y->data.num_dec = (double) y->data.num_int;
      y->type = LVAL_NUM_DEC;
    }
  }
}

void y_math_add(lval* x, lval* y) {
  cast_numbers(x, y);
  if (x->type == LVAL_NUM_INT && y->type == LVAL_NUM_INT) {
    x->data.num_int += y->data.num_int;
  } else if (x->type == LVAL_NUM_DEC && y->type == LVAL_NUM_DEC) {
    x->data.num_dec += y->data.num_dec;
  }
}

void y_math_sub(lval* x, lval* y) {
  cast_numbers(x, y);
  if (x->type == LVAL_NUM_INT && y->type == LVAL_NUM_INT) {
    x->data.num_int -= y->data.num_int;
  } else if (x->type == LVAL_NUM_DEC && y->type == LVAL_NUM_DEC) {
    x->data.num_dec -= y->data.num_dec;
  }
}

void y_math_mul(lval* x, lval* y) {
  cast_numbers(x, y);
  if (x->type == LVAL_NUM_INT && y->type == LVAL_NUM_INT) {
    x->data.num_int *= y->data.num_int;
  } else if (x->type == LVAL_NUM_DEC && y->type == LVAL_NUM_DEC) {
    x->data.num_dec *= y->data.num_dec;
  }
}

void y_math_div(lval* x, lval* y) {
  cast_numbers(x, y);
  if ((y->type == LVAL_NUM_INT && y->data.num_int == 0) ||
      (y->type == LVAL_NUM_DEC && y->data.num_dec == 0)) {
    lval_del(x);
    x = lval_err("Division By Zero.");
  } else if (x->type == LVAL_NUM_INT && y->type == LVAL_NUM_INT) {
    x->data.num_int /= y->data.num_int;
  } else if (x->type == LVAL_NUM_DEC && y->type == LVAL_NUM_DEC) {
    x->data.num_dec /= y->data.num_dec;
  }
}

void y_math_mod(lval* x, lval* y) {
  cast_numbers(x, y);
  if ((y->type == LVAL_NUM_INT && y->data.num_int == 0) ||
      (y->type == LVAL_NUM_DEC && y->data.num_dec == 0)) {
    lval_del(x);
    x = lval_err("Division By Zero.");
  } else if (x->type == LVAL_NUM_INT && y->type == LVAL_NUM_INT) {
    x->data.num_int %= y->data.num_int;
  } else if (x->type == LVAL_NUM_DEC && y->type == LVAL_NUM_DEC) {
    x->data.num_dec = fmod(x->data.num_dec, y->data.num_dec);
  }
}

void y_math_pow(lval* x, lval* y) {
  cast_numbers(x, y);
  if (x->type == LVAL_NUM_INT && y->type == LVAL_NUM_INT) {
    x->data.num_int = pow(x->data.num_int, y->data.num_int);
  } else if (x->type == LVAL_NUM_DEC && y->type == LVAL_NUM_DEC) {
    x->data.num_dec = pow(x->data.num_dec, y->data.num_dec);
  }
}
