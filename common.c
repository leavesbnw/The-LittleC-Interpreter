#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

pointer mk_string(char *string)
{
        pointer x = get_cell(NULL, NULL);
        type(x) = T_STRING;
        str(x) = string;
        return x;
} 
pointer mk_number(int number)
{
        pointer x = get_cell(NULL, NULL);
        type(x) = T_NUMBER;
        num(x) = number;
        return x;
}
pointer cons_with_flag(pointer car, pointer cdr, int flag)
{
        pointer x = get_cell(NULL, NULL);
        type(x) = flag;
        car(x) = car;
        cdr(x) = cdr;
        return x;
}
pointer cons(pointer car, pointer cdr)
{
        pointer x = get_cell(NULL, NULL);
        type(x) = T_PAIR;
        car(x) = car;
        cdr(x) = cdr;
        return x;
}
pointer mk_symbol(char *string)
{
        pointer x= cons(mk_string(string), NULL);
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
        
int sym_eq(pointer sym1, pointer sym2)
{
        if (strcmp(sym(sym1), sym(sym2)) == 0)
                return 1;
        else
                return 0;
}

pointer lookup_symbol(pointer sym)
{
        pointer iter = env;
        while (iter != NULL) {
                if (sym_eq(sym, caar(iter)))
                        return car(iter);
                iter = cdr(iter);
        }
        return iter;
}

pointer add_new_binding(pointer symbol, pointer binding)
{
        env = cons(cons(symbol, binding), env);
        return binding;
}

void dbg_print(pointer obj)
{
        printf("\n DEBUG: ");
        write(stdout, obj);
        printf("\n");
}
