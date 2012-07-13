#include <stdlib.h>
#include "interpreter.h"

pointer mk_string(char *string)
{
        pointer x = get_cell(NIL, NIL);
        type(x) = T_STRING | T_ATOM;
        str(x) = string;
        return x;
} 
pointer mk_number(int number)
{
        pointer x = get_cell(NIL, NIL);
        type(x) = T_NUMBER | T_ATOM;
        num(x) = number;
        return x;
}
pointer cons(pointer car, pointer cdr)
{
        pointer x = get_cell(NIL, NIL);
        type(x) = T_PAIR;
        car(x) = car;
        cdr(x) = cdr;
        return x;
}
pointer mk_symbol(char *string)
{
        pointer x= cons(mk_string(string), NIL);
        type(x) = T_SYMBOL;
        return x;
}
void *alloc_strseg(int size)
{
        return malloc(size);
}
void *alloc_cellseg(int n)
{
        return malloc(sizeof(struct cell) * n);
}
pointer get_cell(pointer car, pointer cdr)
{
        pointer x = alloc_cellseg(1);
        return x;
}
        
