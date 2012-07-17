#include "interpreter.h"

#define first(list) car(list)
#define second(list) cadr(list)
#define third(list) car(cddr(list))

pointer F;
pointer T;

static pointer _add(void) {return mk_number(num(first(arg)) + num(second(arg)));}
static pointer _sub(void) {return mk_number(num(first(arg)) - num(second(arg)));}
static pointer _mul(void) {return mk_number(num(first(arg)) * num(second(arg)));}
static pointer _div(void) {return mk_number(num(first(arg)) / num(second(arg)));}
static pointer _gt(void) {return num(first(arg)) > num(second(arg)) ? T : F;}
static pointer _lt(void) {return num(first(arg)) < num(second(arg)) ? T : F;}
static pointer _gte(void) {return num(first(arg)) >= num(second(arg)) ? T : F;}
static pointer _lte(void) {return num(first(arg)) <= num(second(arg)) ? T : F;}
static pointer _eq(void) {return num(first(arg)) == num(second(arg)) ? T : F;}
static pointer _stringp(void) {return isstring(first(arg)) ? T : F;}
static pointer _numberp(void) {return isnumber(first(arg)) ? T : F;}
static pointer _symbolp(void) {return issymbol(first(arg)) ? T : F;}
static pointer _extprocp(void) {return isextproc(first(arg)) ? T : F;}
static pointer _bltinprocp(void) {return isbltinproc(first(arg)) ? T : F;}
static pointer _pairp(void) {return ispair(first(arg)) ? T : F;}
static pointer _macrop(void) {return ismacro(first(arg)) ? T : F;}
static pointer _atomp(void) {return isatom(first(arg)) ? T : F;}
static pointer _nullp(void) {return first(arg) == NULL ? T : F;}
static pointer _and(void) {return first(arg) != F && second(arg) != F ? T : F;}
static pointer _or(void) {return first(arg) != F || second(arg) != F ? T : F;}
static pointer _not(void) {return first(arg) != F ? F : T;}
static pointer _car(void) {return car(first(arg));}
static pointer _cdr(void) {return cdr(first(arg));}
static pointer _cons(void) {return cons(first(arg), second(arg));}
static pointer _append(void) {return append(first(arg), second(arg));}
static pointer _if(void) {return eval(first(arg)) != F ? eval(second(arg)) : eval(third(arg));}
static pointer _quote(void) {return first(arg);}
static pointer _lambda(void) {return cons_with_flag(env, arg, T_EXTEND_PROC);}
static pointer _defmacro(void){return add_new_binding(first(arg), cons_with_flag(env, cdr(arg), T_MACRO));}
static pointer __backquote(pointer args);
static pointer _backquote(void) {return __backquote(first(arg));}
static pointer _set(void) 
{
        pointer symbol = first(arg);
        pointer tmp = lookup_symbol(symbol);
        if (tmp != NULL)
                return lookup_val(tmp) = eval(second(arg));
        else {
                add_new_binding(symbol, 0);
                return lookup_val(lookup_symbol(symbol)) = eval(second(arg));
        }
}

static pointer _expand_macro(void)
{
        arg = first(arg);
        op = lookup_val(lookup_symbol(first(arg)));
        arg = cdr(arg);
        env = op_get_macro_env(op);
        pointer farg = op_get_macro_farg(op);
        while (farg != NULL) {
                add_new_binding(car(farg), car(arg));
                farg = cdr(farg);
                arg = cdr(arg);
        }
        return eval(op_get_macro_body(op));
}

static pointer _progn(void)
{
        pointer result, tmp = arg;

        
        while (tmp != NULL) {
                result = eval(first(tmp));
                tmp = cdr(tmp);
        }
        return result;
}

static void register_bltin_proc(char *name, bltin_proc_code_ptr proc, int flag)
{
        add_new_binding(mk_symbol(name), cons_with_flag((pointer) name, (pointer) proc, flag));
}

void init()
{
        F = mk_symbol("#f");
        T = mk_symbol("#t");

        env = arg = op = cont = NULL;
        register_bltin_proc("+", _add, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("-", _sub, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("*", _mul, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("/", _div, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc(">", _gt, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc(">=", _gte, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("<", _lt, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("<=", _lte, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("=", _eq, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("string?", _stringp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("number?", _numberp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("symbol?", _symbolp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("extproc?", _extprocp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("bltinproc?", _bltinprocp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("pair?", _pairp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("macro?", _macrop, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("atom?", _atomp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("null?", _nullp, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("and", _and, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("or", _or, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("not", _not, T_BUILT_IN_REGULAR_PROC);        
        register_bltin_proc("car", _car, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("cdr", _cdr, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("cons", _cons, T_BUILT_IN_REGULAR_PROC);
        register_bltin_proc("append", _append, T_BUILT_IN_REGULAR_PROC);

        register_bltin_proc("if", _if, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("quote", _quote, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("lambda", _lambda, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("defmacro", _defmacro, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("backquote", _backquote, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("set!", _set, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("expand-macro", _expand_macro, T_BUILT_IN_SPECIAL_PROC);
        register_bltin_proc("progn", _progn, T_BUILT_IN_SPECIAL_PROC);

}


/* really ugly! */
static pointer __backquote(pointer args)
{
        if (args == NULL)
                return NULL;
        if (type(args) != T_PAIR)
                return args;
        pointer next, val;
        if (cdr(args) != NULL && type(cadr(args)) == T_PAIR) {
                next = caar(cdr(args));
                if (issymbol(next) && sym_eq(next, mk_symbol("splice"))) {
                        val = __backquote(cons(car(args), NULL));
                        if (!ispair(val))
                                return cons(val, __backquote(cdr(args)));
                        else
                                return append(val, __backquote(cdr(args)));

                }
        }
        pointer cur = car(args);
        if (type(cur) == T_PAIR && issymbol(car(cur))) {
                if (sym_eq(car(cur), mk_symbol("unquote")))
                        return cons(eval(cadr(cur)), __backquote(cdr(args)));
                else if (sym_eq(car(cur), mk_symbol("splice")))  {
                        val = eval(cadr(cur));
                        if (!ispair(val))
                                return val;
                        else
                                return append(val, __backquote(cdr(args)));
                }
        }
        
        return cons(__backquote(car(args)), __backquote(cdr(args)));
        
        /* if (args == NULL) */
        /*         return NULL; */

        /* if (type(args) == T_PAIR) { */
        /*         if (issymbol(car(args))) { */
        /*                 if (sym_eq(car(args), mk_symbol("unquote"))) */
        /*                         return eval(cadr(args)); */
        /*                 if (sym_eq(car(args), mk_symbol("splice"))) { */
        /*                         pointer val = eval(cadr(args)); */
                                
                                
        /*         } */
        /* } else */
        /*         return args; */
        /* return cons(__backquote(car(args)), __backquote(cdr(args))); */
}
