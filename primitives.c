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
static pointer _gt(void)  {return num(first(arg)) > num(second(arg)) ? T : F;}
static pointer _lt(void)  {return num(first(arg)) < num(second(arg)) ? T : F;}
static pointer _gte(void) {return num(first(arg)) >= num(second(arg)) ? T : F;}
static pointer _lte(void) {return num(first(arg)) <= num(second(arg)) ? T : F;}
static pointer _eq(void)  {return num(first(arg)) == num(second(arg)) ? T : F;}
static pointer _stringp(void)    {return isstring(first(arg)) ? T : F;}
static pointer _numberp(void)    {return isnumber(first(arg)) ? T : F;}
static pointer _symbolp(void)    {return issymbol(first(arg)) ? T : F;}
static pointer _extprocp(void)   {return isextproc(first(arg)) ? T : F;}
static pointer _bltinprocp(void) {return isbltinproc(first(arg)) ? T : F;}
static pointer _pairp(void)  {return ispair(first(arg)) ? T : F;}
static pointer _macrop(void) {return ismacro(first(arg)) ? T : F;}
static pointer _atomp(void)  {return isatom(first(arg)) ? T : F;}
static pointer _and(void)    {return first(arg) != F && second(arg) != F ? T : F;}
static pointer _or(void)     {return first(arg) != F || second(arg) != F ? T : F;}
static pointer _not(void)    {return first(arg) != F ? F : T;}


static void register_bltin_proc(char *name, bltin_proc_code_ptr proc)
{
        add_new_binding(name, cons_with_flag((pointer) proc, NULL, T_BLTIN_PROC));
}

void init()
{
        F = mk_symbol("#f");
        T = mk_symbol("#t");

        env = arg = op = cont = NULL;
        register_bltin_proc("+", _add);
        register_bltin_proc("-", _sub);
        register_bltin_proc("*", _mul);
        register_bltin_proc("/", _div);
        register_bltin_proc(">", _gt);
        register_bltin_proc(">=", _gte);
        register_bltin_proc("<", _lt);
        register_bltin_proc("<=", _lte);
        register_bltin_proc("=", _eq);
        register_bltin_proc("string?", _stringp);
        register_bltin_proc("number?", _numberp);
        register_bltin_proc("symbol?", _symbolp);
        register_bltin_proc("extproc?", _extprocp);
        register_bltin_proc("bltinproc?", _bltinprocp);
        register_bltin_proc("pair?", _pairp);
        register_bltin_proc("macro?", _macrop);
        register_bltin_proc("atom?", _atomp);
        register_bltin_proc("and", _and);
        register_bltin_proc("or", _or);
        register_bltin_proc("not", _not);        
}
