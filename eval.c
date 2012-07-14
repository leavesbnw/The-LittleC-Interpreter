#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

pointer env;
pointer arg;
pointer op;
pointer cont;

static pointer apply(pointer expr);

pointer eval(pointer sexpr)
{
        if (sexpr == NULL)
                return NULL;

        switch(type(sexpr)) {
        case T_NUMBER:
        case T_STRING:
                return sexpr;
        case T_SYMBOL:
                return lookup_val(lookup_symbol(sexpr));
        case T_PAIR:
                return apply(sexpr);
        default:
                printf("Unknow Object\n");
                exit(1);
        }
}

static pointer eval_arg_list(pointer sexpr);
void save_continuation();
void restore_continuation();
static pointer apply(pointer expr)
{
        pointer result, farg;
        save_continuation();
        op = eval(sexpr_get_op(expr));



        switch (op_type(op)) {
        case T_BUILT_IN_REGULAR_PROC:
                arg = eval_arg_list(sexpr_get_arg(expr));
                result = (op_get_bltin_proc_code(op))();
                break;
        case T_BUILT_IN_SPECIAL_PROC:
                arg = sexpr_get_arg(expr);
                result = (op_get_bltin_proc_code(op))();
                break;
        case T_EXTEND_PROC:
                arg = eval_arg_list(sexpr_get_arg(expr));
                env = op_get_ext_proc_env(op);
                farg = op_get_ext_proc_farg(op);
                while (farg != NULL) {
                        add_new_binding(car(farg), car(arg));
                        farg = cdr(farg);
                        arg = cdr(arg);
                }
                result = eval(op_get_ext_proc_body(op));
                break;
        case T_MACRO:
                break;
        }

        restore_continuation();

        return result;
}
void save_continuation(void)
{
        cont = cons(env, cons(op, cons(arg, cons(cont, NULL))));
}

void restore_continuation(void)
{
        if (strcmp("set!", op_get_bltin_proc_name(op)) == 0) {
                op = cadr(cont);
                arg = car(cddr(cont));
                cont = car(cdr(cddr(cont)));
        } else {
                env = car(cont);
                op = cadr(cont);
                arg = car(cddr(cont));
                cont = car(cdr(cddr(cont)));
        }
                
}
        
static pointer eval_arg_list(pointer sexpr)
{
        if (sexpr == NULL)
                return NULL;
        pointer car = eval(car(sexpr));
        pointer cdr = eval_arg_list(cdr(sexpr));
        pointer consed = cons(car, cdr);
        return consed;
}
