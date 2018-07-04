#include <stdio.h>
#include "evaluation.h"
#include "y_math.h"

/* Construct a pointer to a new integer lval */
lval* lval_num_int(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM_INT;
  v->data.num_int = x;
  return v;
}

/* Construct a pointer to a new double lval */
lval* lval_num_dec(double x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM_DEC;
  v->data.num_dec = x;
  return v;
}

/* Construct a pointer to a new Error lval */
lval* lval_err(char* m) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->data.err = malloc(strlen(m) + 1);
  strcpy(v->data.err, m);
  return v;
}

/* Construct a pointer to a new Symbol lval */
lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->data.sym = malloc(strlen(s) + 1);
  strcpy(v->data.sym, s);
  return v;
}

/* A pointer to a new empty Sexpr lval */
lval* lval_sexpr() {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

/* A pointer to a new empty Qexpr lval */
lval* lval_qexpr() {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lval_del(lval* v) {

  switch (v->type) {
    /* Do nothing special for number type */
    case LVAL_NUM_INT: break;
    case LVAL_NUM_DEC: break;

    /* For Err or Sym free the string data */
    case LVAL_ERR: free(v->data.err); break;
    case LVAL_SYM: free(v->data.sym); break;

    /* If [S/Q]-expr then delete all elements inside */
    case LVAL_QEXPR:
    case LVAL_SEXPR:
      for (int i = 0; i < v->count; i++) {
        lval_del(v->cell[i]);
      }
      /* Also free the memory allocated to contain the pointers */
      free(v->cell);
    break;
  }

  /* Free the memory allocated for the "lval" struct itself */
  free(v);
}

lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_pop(lval* v, int i) {
  /* Find the item at "i" */
  lval* x = v->cell[i];

  /* Shift memory after the item at "i" over the top */
  memmove(&v->cell[i], &v->cell[i+1],
    sizeof(lval*) * (v->count-i-1));

  /* Decrease the count of items in the list */
  v->count--;

  /* Reallocate the memory used */
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  return x;
}

lval* lval_take(lval* v, int i) {
  lval* x = lval_pop(v, i);
  lval_del(v);
  return x;
}

void lval_print(lval* v);

void lval_expr_print(lval* v, char* open, char* close) {
  printf("%s", open);
  for (int i = 0; i < v->count; i++) {

    /* Print Value contained within */
    lval_print(v->cell[i]);

    /* Don't print trailing space if last element */
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  printf("%s", close);
}

void lval_print(lval* v) {
  switch (v->type) {
    case LVAL_NUM_INT:  printf("%li", v->data.num_int); break;
    case LVAL_NUM_DEC:  printf("%f", v->data.num_dec); break;
    case LVAL_ERR:      printf("Error: %s", v->data.err); break;
    case LVAL_SYM:      printf("%s", v->data.sym); break;
    case LVAL_QEXPR:    lval_expr_print(v, "\'(", ")"); break;
    case LVAL_SEXPR:    lval_expr_print(v, "(", ")"); break;
  }
}

void lval_println(lval* v) { lval_print(v); putchar('\n'); }

lval* builtin_op(lval* a, char* op) {

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

lval* lval_eval_sexpr(lval* v) {

  /* Evaluate Children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(v->cell[i]);
  }

  /* Error Checking */
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) { return lval_take(v, i); }
  }

  /* Empty Expression */
  if (v->count == 0) { return v; }

  /* Single Expression */
  if (v->count == 1) { return lval_take(v, 0); }

  /* Ensure First Element is Symbol */
  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_SYM) {
    lval_del(f); lval_del(v);
    return lval_err("S-expression Does not start with symbol.");
  }

  /* Call builtin with operator */
  lval* result = builtin_op(v, f->data.sym);
  lval_del(f);
  return result;
}

lval* lval_eval(lval* v) {
  /* Evaluate Sexpressions */
  if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(v); }
  /* All other lval types remain the same */
  return v;
}

lval* lval_read_num_int(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  if (errno != ERANGE) {
    return lval_num_int(x);
  } else {
    return lval_err("invalid number");
  }
}

lval* lval_read_num_dec(mpc_ast_t* t) {
  errno = 0;
  double x = strtod(t->contents, NULL);
  if (errno != ERANGE) {
    return lval_num_dec(x);
  } else {
    return lval_err("invalid number");
  }
}

lval* lval_read(mpc_ast_t* t) {

  /* If Symbol or Number return conversion to that type */
  if (strstr(t->tag, "integer")) { return lval_read_num_int(t); }
  if (strstr(t->tag, "decimal")) { return lval_read_num_dec(t); }
  if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

  /* If root (>) or sexpr then create empty list */
  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
  if (strstr(t->tag, "sexpr"))  { x = lval_sexpr(); }
  if (strstr(t->tag, "qexpr"))  { x = lval_qexpr(); }

  /* Fill this list with any valid expression contained within */
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "'(") == 0) { continue; }
    if (strcmp(t->children[i]->tag, "regex") == 0) { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}
