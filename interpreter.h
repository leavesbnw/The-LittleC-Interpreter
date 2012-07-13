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
extern pointer NIL;

#define T_STRING 1
#define T_NUMBER 2
#define T_SYMBOL 4
#define T_PAIR   8
#define T_ATOM  16
#define T_PROC  32
#define T_MACRO 64

#define type(p) ((p)->flag)
#define str(p) ((p)->_object._string.value)
#define num(p) ((p)->_object._number.value)
#define sym(p) (str(car(p)))
#define car(p) ((p)->_object._cons.car)
#define cdr(p) ((p)->_object._cons.cdr)

#define isstring(p) (type(p) & T_STRING)
#define isnumber(p) (type(p) & T_NUMBER)
#define issymbol(p) (type(p) & T_SYMBOL)
#define isproc(p) (type(p) & T_PROC)
#define ispair(p) (type(p) & T_PAIR)
#define ismacro(p) (type(p) & T_MACRO)
#define isatom(p) (type(p) & T_ATOM)

#define caar(p) car(car(p))
#define cadr(p) car(cdr(p))
#define cdar(p) cdr(car(p))
#define cddr(p) cdr(cdr(p))

pointer mk_string(char *string);
pointer mk_number(int number);
pointer cons(pointer car, pointer cdr);
pointer mk_symbol(char *string);
void *alloc_strseg(int size);
void *alloc_cellseg(int n);
pointer get_cell(pointer car, pointer cdr);
pointer eval(pointer sexpr);

//char *store_string(const char *string);

#define MAX_SYM_NUM_LEN 255
#define MAX_STR_LEN 1024

#include <stdio.h>
typedef FILE port;
pointer read(port *out);
void write(port *out, pointer object);


#endif
