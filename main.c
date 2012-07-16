#include "interpreter.h"

int main()
{
        init();
        for (;;) {
                pointer sexpr = read(stdin);
                printf("*evaluating ");
                write(stdout, sexpr);
                printf("\n   ");
                pointer result = eval(sexpr);
                write(stdout, result);
                printf("\n\n");
        }
        return 0;
}
