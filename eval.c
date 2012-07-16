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
        case T_MACRO:
        case T_BUILT_IN_REGULAR_PROC:
        case T_BUILT_IN_SPECIAL_PROC:
                return sexpr;
        case T_SYMBOL:
//                dbg_print(sexpr);
//                dbg_print(env);
                return lookup_val(lookup_symbol(sexpr));
        case T_PAIR:
                return apply(sexpr);
        default:
                printf("Unknow Object\n");
                exit(1);
        }
}

#define cont_env 1
#define cont_arg 2
#define cont_op  4

static pointer eval_arg_list(pointer sexpr);
void save_continuation(int flag);
void restore_continuation(int flag);

static pointer apply(pointer expr)
{
        pointer tmp_op, result, farg, expanded;
        int flags;
        tmp_op = eval(sexpr_get_op(expr));

        switch (op_type(tmp_op)) {
        case T_BUILT_IN_REGULAR_PROC:
                save_continuation(cont_env | cont_arg | cont_op);
                op = tmp_op;
                arg = eval_arg_list(sexpr_get_arg(expr));
                result = (op_get_bltin_proc_code(op))();
                restore_continuation(cont_env | cont_arg | cont_op);
                break;
        case T_BUILT_IN_SPECIAL_PROC:
                if (strcmp("set!", op_get_bltin_proc_name(tmp_op)) == 0 || 
                    strcmp("defmacro", op_get_bltin_proc_name(tmp_op)) == 0)
                        flags = cont_arg | cont_op;
                else
                        flags = cont_env | cont_arg | cont_op;
                save_continuation(flags);
                op = tmp_op;
                arg = sexpr_get_arg(expr);
                result = (op_get_bltin_proc_code(op))();
                restore_continuation(flags);
                break;
        case T_EXTEND_PROC:
                save_continuation(cont_env | cont_arg | cont_op);
                op = tmp_op;
                arg = eval_arg_list(sexpr_get_arg(expr));
                env = op_get_ext_proc_env(op);
                farg = op_get_ext_proc_farg(op);
                while (farg != NULL) {
                        add_new_binding(car(farg), car(arg));
                        farg = cdr(farg);
                        arg = cdr(arg);
                }
                result = eval(op_get_ext_proc_body(op));
                restore_continuation(cont_env | cont_arg | cont_op);
                break;
        case T_MACRO:
                save_continuation(cont_env | cont_arg | cont_op);
                op = tmp_op;
                arg = sexpr_get_arg(expr);
                env = op_get_macro_env(op);
                farg = op_get_macro_farg(op);
                while (farg != NULL) {
                        add_new_binding(car(farg), car(arg));
                        farg = cdr(farg);
                        arg = cdr(arg);
                }
                expanded = eval(op_get_macro_body(op));
                restore_continuation(cont_env | cont_arg | cont_op);
                result = eval(expanded);

                break;
        }


        return result;
}

void save_continuation(int flag)
{
        pointer tmp = cons(cont, NULL);
        
        if (flag & cont_arg)
                tmp = cons(arg, tmp);
        if (flag & cont_op)
                tmp = cons(op, tmp);
        if (flag & cont_env)
                tmp = cons(env, tmp);
        cont = tmp;
}

void restore_continuation(int flag)
{
        int count = 0;
        if (flag & cont_env) {
                count++;
                env = car(cont);
        }
        if (flag & cont_op) {
                count++;
                op = cadr(cont);
        }
        if (flag & cont_arg) {
                count++;
                arg = car(cddr(cont));
        }
        switch (count) {
        case 3:
                cont = car(cdr(cddr(cont)));
                break;
        case 2:
                cont = car(cddr(cont));
                break;
        case 1:
                cont = car(cdr(cont));
                break;
        case 0:
                cont = car(cont);
                break;
        }
}


        /* if (strcmp("set!", op_get_bltin_proc_name(op)) == 0 || */
        /*     strcmp("defmacro", op_get_bltin_proc_name(op)) == 0) { */
        /*         op = cadr(cont); */
        /*         arg = car(cddr(cont)); */
        /*         cont = car(cdr(cddr(cont))); */
        /* } else { */
        /*         env = car(cont); */
        /*         op = cadr(cont); */
        /*         arg = car(cddr(cont)); */
        /*         cont = car(cdr(cddr(cont))); */
        /* } */
                
//}
        
static pointer eval_arg_list(pointer sexpr)
{
        if (sexpr == NULL)
                return NULL;
        pointer car = eval(car(sexpr));
        pointer cdr = eval_arg_list(cdr(sexpr));
        pointer consed = cons(car, cdr);
        return consed;
}
