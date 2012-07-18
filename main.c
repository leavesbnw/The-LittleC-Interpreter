#include "interpreter.h"

void load_init_file(char *file, int flags)
{
        FILE *f = fopen(file, "r");
        pointer sexpr, result;
        printf("**************INIT***************\n");
        while ((sexpr = read(f)) != E_O_F) {
                result = eval(sexpr);
                if (flags) {
                        printf("*installing ");
                        write(stdout, result);
                        printf("...\n");
                }
        }
        fclose(f);
        printf("**************DONE***************\n");        
}

int main()
{
        init();
        pointer sexpr, result;

        load_init_file("init.sch", 1);
        printf("\n Welcome to LittleC!\nLC> ");
        while ((sexpr = read(stdin)) != E_O_F) {
                printf("*evaluating ");
                write(stdout, sexpr);
                printf("\n   ");
                result = eval(sexpr);
                write(stdout, result);
                printf("\n");
                printf("LC> ");
                fflush(stdout);
        }
        return 0;
}
