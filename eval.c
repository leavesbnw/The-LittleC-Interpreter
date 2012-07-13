#include "interpreter.h"

pointer env;
pointer arg;
pointer op;
pointer cont;

void init()
{
        init_envir();
        init_regs();
}
