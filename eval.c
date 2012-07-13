#include <stdlib.h>
#include "interpreter.h"

pointer env;
pointer arg;
pointer op;
pointer cont;

static pointer apply(pointer expr);
static pointer eval_list(pointer sexpr);

pointer eval(pointer sexpr)
{
        if (sexpr == NULL)
                return NULL;

        switch(type(sexpr)) {
        case T_NUMBER:
        case T_STRING:
                return sexpr;
        case T_SYMBOL:
                return lookup_symbol(sexpr);
        case T_PAIR:
                
                return apply(eval_list(sexpr));
        default:
                printf("Unknow Object\n");
                exit(1);
        }
}

static pointer eval_list(pointer sexpr)
{
        if (sexpr == NULL)
                return NULL;
        pointer car = eval(car(sexpr));
        pointer cdr = eval_list(cdr(sexpr));
        pointer consed = cons(car, cdr);
        return consed;
}

static pointer apply(pointer expr)
{
        op = sexpr_get_op(expr);
        arg = sexpr_get_arg(expr);

        switch (op_type(op)) {
        case T_BLTIN_PROC:
                return (op_get_bltin_proc_code(op))();
        case T_EXT_PROC:
                break;
        case T_MACRO:
                break;
        }
        return NULL;

}
                
        
