#ifndef types_h
#define types_h

enum { LVAL_ERR, LVAL_NUM_INT, LVAL_NUM_DEC, LVAL_SYM,
       LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

typedef struct lval lval;
typedef struct lenv lenv;

typedef struct lval* (*lbuiltin) (struct lenv*, struct lval*);

struct lval {
  int type;
  union {
    /* Numbers types */
    long num_int;
    double num_dec;
    /* Error and Symbol types have some string data */
    char* err;
    char* sym;
    lbuiltin fun;
  } data;
  /* Count and Pointer to a list of "lval*"; */
  int count;
  struct lval** cell;
};

struct lenv {
  int count;
  char** syms;
  struct lval** vals;
};

#endif
