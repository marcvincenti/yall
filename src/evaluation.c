#include "environment.h"
#include "evaluation.h"
#include "mpc.h"

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
/* Construct a pointer to a function */
lval* lval_fun(lbuiltin func) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_FUN;
  v->data.fun = func;
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

lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_copy(lval* v) {

  lval* x = malloc(sizeof(lval));
  x->type = v->type;

  switch (v->type) {

    /* Copy Functions and Numbers Directly */
    case LVAL_NUM_INT: x->data.num_int = v->data.num_int; break;
    case LVAL_NUM_DEC: x->data.num_dec = v->data.num_dec; break;
    case LVAL_FUN: x->data.fun = v->data.fun; break;

    /* Copy Strings using malloc and strcpy */
    case LVAL_ERR:
      x->data.err = malloc(strlen(v->data.err) + 1);
      strcpy(x->data.err, v->data.err); break;

    case LVAL_SYM:
      x->data.sym = malloc(strlen(v->data.sym) + 1);
      strcpy(x->data.sym, v->data.sym); break;

    /* Copy Lists by copying each sub-expression */
    case LVAL_SEXPR:
    case LVAL_QEXPR:
      x->count = v->count;
      x->cell = malloc(sizeof(lval*) * x->count);
      for (int i = 0; i < x->count; i++) {
        x->cell[i] = lval_copy(v->cell[i]);
      }
    break;
  }

  return x;
}

lval* lval_join(lval* x, lval* y) {
  /* For each cell in 'y' add it to 'x' */
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }
  /* Delete the empty 'y' and return 'x' */
  lval_del(y);
  return x;
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

void lval_del(lval* v) {

  switch (v->type) {
    /* Do nothing special for number type and functions */
    case LVAL_NUM_INT: break;
    case LVAL_NUM_DEC: break;
    case LVAL_FUN: break;

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
    case LVAL_FUN:      printf("<function>"); break;
    case LVAL_ERR:      printf("Error: %s", v->data.err); break;
    case LVAL_SYM:      printf("%s", v->data.sym); break;
    case LVAL_QEXPR:    lval_expr_print(v, "\'(", ")"); break;
    case LVAL_SEXPR:    lval_expr_print(v, "(", ")"); break;
  }
}

void lval_println(lval* v) { lval_print(v); putchar('\n'); }

lval* lval_eval_sexpr(lenv* e, lval* v) {
  /* Evaluate Children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
  }
  /* Error Checking */
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) { return lval_take(v, i); }
  }
  /* Empty Expression */
  if (v->count == 0) { return v; }
  /* Single Expression */
  if (v->count == 1) { return lval_take(v, 0); }
  /* Ensure first element is a function after evaluation */
  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_FUN) {
    lval_del(v); lval_del(f);
    return lval_err("Unknown function!");
  }
  /* If so call function to get result */
  lval* result = f->data.fun(e, v);
  lval_del(f);
  return result;
}

lval* lval_eval(lenv* e, lval* v) {
  /* Evaluate Sexpressions */
  if (v->type == LVAL_SYM) {
    lval* x = lenv_get(e, v);
    lval_del(v);
    return x;
  }
  if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(e, v); }
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
