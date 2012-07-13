#include "interpreter.h"

pointer NIL;

int main()
{
        NIL = mk_symbol("nil");
        for (;;) {
                pointer sexpr = read(stdin);
                pointer result = eval(sexpr);
                write(stdout, result);
                printf("\n");
        }
        return 0;
}
