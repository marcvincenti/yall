#include "y_math.h"

long y_math_max(long x, long y) {
  return fmaxl(x, y);
}

long y_math_min(long x, long y) {
  return fminl(x, y);
}

long y_math_pow(long x, long y) {
  return pow(x, y);
}
