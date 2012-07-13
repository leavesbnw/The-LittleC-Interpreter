#ifndef H_INTERPRETER
#define H_INTERPRETER

struct cell {
        int flag;
        union {
                struct {
                        char *value;
                } _string;
                struct {
                        int value;
                } _number;
                struct {
                        struct cell *car;
                        struct cell *cdr;
                } _cons;
        } _object;
};
typedef struct cell* pointer;
typedef pointer (*bltin_proc_code_ptr)(void);
extern pointer F;
extern pointer T;
extern pointer env;
extern pointer arg;
extern pointer op;
extern pointer cont;
#define T_STRING 1
#define T_NUMBER 2
#define T_SYMBOL 4
#define T_PAIR   8
#define T_EXT_PROC  32
#define T_BLTIN_PROC 64
#define T_MACRO 128

#define type(p) ((p)->flag)
#define str(p) ((p)->_object._string.value)
#define num(p) ((p)->_object._number.value)
#define sym(p) (str(car(p)))
#define car(p) ((p)->_object._cons.car)
#define cdr(p) ((p)->_object._cons.cdr)
#define op_type(p) ((p)->flag)
#define op_get_bltin_proc_code(p) ((bltin_proc_code_ptr) car(p))
#define sexpr_get_op(sexpr) car(sexpr)
#define sexpr_get_arg(sexpr) cdr(sexpr)

#define isstring(p) (type(p) & T_STRING)
#define isnumber(p) (type(p) & T_NUMBER)
#define issymbol(p) (type(p) & T_SYMBOL)
#define isextproc(p) (type(p) & T_EXT_PROC)
#define isbltinproc(p) (type(p) & T_BLTIN_PROC)
#define ispair(p) (type(p) & T_PAIR)
#define ismacro(p) (type(p) & T_MACRO)
#define isatom(p) (isstring(p) || isnumber(p))

#define caar(p) car(car(p))
#define cadr(p) car(cdr(p))
#define cdar(p) cdr(car(p))
#define cddr(p) cdr(cdr(p))

pointer mk_string(char *string);
pointer mk_number(int number);
pointer cons(pointer car, pointer cdr);
pointer cons_with_flag(pointer car, pointer cdr, int flag);
pointer mk_symbol(char *string);
void *alloc_strseg(int size);
void *alloc_cellseg(int n);
pointer get_cell(pointer car, pointer cdr);
pointer eval(pointer sexpr);
void init(void);
int sym_eq(pointer sym1, pointer sym2);
pointer lookup_symbol(pointer sym);
void add_new_binding(char *name, pointer binding);
//char *store_string(const char *string);

#define MAX_SYM_NUM_LEN 255
#define MAX_STR_LEN 1024

#include <stdio.h>
typedef FILE port;
pointer read(port *out);
void write(port *out, pointer object);


#endif
