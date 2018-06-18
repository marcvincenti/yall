#include "y_math.h"

lval y_math_add(lval x, lval y) {
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(x.val.l + y.val.l);
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(x.val.l + y.val.d);
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(x.val.d + y.val.l);
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(x.val.d + y.val.d);
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

lval y_math_sub(lval x, lval y) {
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(x.val.l - y.val.l);
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(x.val.l - y.val.d);
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(x.val.d - y.val.l);
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(x.val.d - y.val.d);
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

lval y_math_mul(lval x, lval y) {
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(x.val.l * y.val.l);
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(x.val.l * y.val.d);
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(x.val.d * y.val.l);
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(x.val.d * y.val.d);
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

lval y_math_div(lval x, lval y) {
  if ((y.type == LVAL_NUM && y.val.l == 0) ||
      (y.type == LVAL_DEC && y.val.d == 0)) {
    return lval_err(LERR_DIV_ZERO);
  } // assert y != 0
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(x.val.l / y.val.l);
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(x.val.l / y.val.d);
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(x.val.d / y.val.l);
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(x.val.d / y.val.d);
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

lval y_math_mod(lval x, lval y) {
  if ((y.type == LVAL_NUM && y.val.l == 0) ||
      (y.type == LVAL_DEC && y.val.d == 0)) {
    return lval_err(LERR_DIV_ZERO);
  } // assert y != 0
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(x.val.l % y.val.l);
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(fmod(x.val.l, y.val.d));
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(fmod(x.val.d, y.val.l));
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(fmod(x.val.d, y.val.d));
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

lval y_math_pow(lval x, lval y) {
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(pow(x.val.l, y.val.l));
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(pow(x.val.l, y.val.d));
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(pow(x.val.d, y.val.l));
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(pow(x.val.d, y.val.d));
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

double _max(double x, double y) {
  if (x >= y) {
    return x;
  } else {
    return y;
  }
}

lval y_math_max(lval x, lval y) {
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(fmaxl(x.val.l, y.val.l));
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(_max(x.val.l, y.val.d));
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(_max(x.val.d, y.val.l));
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(_max(x.val.d, y.val.d));
  } else {
    return lval_err(LERR_BAD_OP);
  }
}

double _min(double x, double y) {
  if (x <= y) {
    return x;
  } else {
    return y;
  }
}

lval y_math_min(lval x, lval y) {
  if (x.type == LVAL_NUM && y.type == LVAL_NUM) {
    return lval_num(fminl(x.val.l, y.val.l));
  } else if (x.type == LVAL_NUM && y.type == LVAL_DEC) {
    return lval_dec(_min(x.val.l, y.val.d));
  } else if (x.type == LVAL_DEC && y.type == LVAL_NUM) {
    return lval_dec(_min(x.val.d, y.val.l));
  } else if (x.type == LVAL_DEC && y.type == LVAL_DEC) {
    return lval_dec(_min(x.val.d, y.val.d));
  } else {
    return lval_err(LERR_BAD_OP);
  }
}
