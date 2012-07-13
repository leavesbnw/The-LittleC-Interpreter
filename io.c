#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interpreter.h"

enum TOK_T {LIST_START, LIST_END, STRING, SYMBOL, NUMBER, QUOTE, BACKQUOTE, UNQUOTE, SPLICE};


static int pair_count;

pointer _read(port *in);
pointer read(port *in)
{
        pair_count = 0;
        return _read(in);
}

static enum TOK_T get_next_token(port *in, pointer *token);
static pointer read_list(port *in);
static pointer cons_with_symbol(char *string, pointer object);

pointer _read(port *in)
{
        pointer token;
        switch (get_next_token(in, &token) & ~T_ATOM) {
        case STRING:
        case SYMBOL:
        case NUMBER:
                return token;
        case LIST_START:
                return read_list(in);
        case LIST_END:
                pair_count--;
                return NULL;
        case QUOTE:
                return cons_with_symbol("quote", cons(_read(in), NULL));
        case BACKQUOTE:
                return cons_with_symbol("backquote", cons(_read(in), NULL));
        case UNQUOTE:
                return cons_with_symbol("unquote", cons(_read(in), NULL));
        case SPLICE:
                return cons_with_symbol("splice", cons(_read(in), NULL));
        default:
                printf("Unknown token %s\n", str(token));
                exit(1);
                break;
        }
}

/* this is really ugly! */
static pointer read_list(port *in)
{
        int saved_pair_count = pair_count++;
        pointer pre, cur, subsexpr, sexpr = NULL;
        
        if ((subsexpr = _read(in)) == NULL && pair_count == saved_pair_count)
                return NULL;
        pre = sexpr = cons(subsexpr, NULL);
        for(;;) {
                subsexpr = _read(in);
                if (saved_pair_count == pair_count)
                        break;
                cur = cons(subsexpr, NULL);
                cdr(pre) = cur;
                pre = cur;
        }

        return sexpr;

}

static pointer cons_with_symbol(char *string, pointer object)
{
        return cons(mk_symbol(string), object);
}

        

static int get_next_char(port *in);
static void push_back_char(port *in, int ch);
static void skip_white_spc(port *in);
static char *read_string(port *in);
static char *get_char_until_delim(port *in);
static int test_number(const char *string);

static enum TOK_T get_next_token(port *in, pointer *token)
{
        char ch, *t;

        skip_white_spc(in);
        switch (ch = get_next_char(in)) {
        case EOF:
                exit(1);
        case '(':
                return LIST_START;
        case ')':
                return LIST_END;
        case '"':
                *token = mk_string(read_string(in));
                return STRING;
        case '\'':
                return QUOTE;
        case '`':
                return BACKQUOTE;
        case ',':
                if ((ch = get_next_char(in)) == '@')
                        return SPLICE;
                else {
                        push_back_char(in, ch);
                        return UNQUOTE;
                }
        default:
                push_back_char(in, ch);
                t = get_char_until_delim(in);
                if (test_number(t)) {
                        *token = mk_number(atoi(t));
                        return NUMBER;
                } else {
                        *token = mk_symbol(t);
                        return SYMBOL;
                }
        }
}
                
static int get_next_char(port *in)
{
        return fgetc(in);
}
static void push_back_char(port *in, int ch)
{
        ungetc(ch, in);
}
static int is_white_space(char ch) 
{
        return ch == '\n' || ch == '\t' || ch == ' ';
}

static void skip_white_spc(port *in)
{
        int ch;
        while ((ch = get_next_char(in)) != EOF && is_white_space(ch))
                ;
        if (ch != EOF)
                push_back_char(in, ch);
}
static char *read_string(port *in)
{
        int ch, i = 0;
        char str[MAX_STR_LEN], *dst;

        if ((ch = get_next_char(in)) != '"') 
                do {
                        str[i++] = ch;
                } while ((ch = get_next_char(in)) != '"' && str[i - 1] != '\\');
                        
        str[i] = '\0';
        memcpy((dst = alloc_strseg(i + 1)), str, i + 1);

        return dst;
}
                
        
static char *get_char_until_delim(port *in)
{
        int i = 0, ch;
        char str[MAX_SYM_NUM_LEN], *dst;
        
        str[0] = '\0';
        while ((ch = get_next_char(in)) != EOF && !is_white_space(ch) && ch != '(' && ch != ')')
                str[i++] = ch;
        str[i] = '\0';
        
        memcpy((dst = alloc_strseg(i + 1)), str, i + 1);
        if (ch != EOF)
                push_back_char(in, ch);
        return dst;
}

static int test_number(const char *string)
{
        int i = 0, len = strlen(string);

        if (string[0] == '+' || string[0] == '-') {
                if (len == 1)
                        return 0;
                else
                        i++;
        }
        while (string[i] != '\0')
                if (!isdigit(string[i++]))
                        return 0;
        return 1;
}
static void write_string(port *out, pointer object);
static void write_number(port *out, pointer object);
static void write_symbol(port *out, pointer object);
static void write_pair(port *out, pointer object);
static void write_proc(port *out, pointer object);
static void write_macro(port *out, pointer object);
static void wirte_unknown(port *out, pointer object);

void write(port *out, pointer object)
{
        if (object == NULL) {
                printf("()");
                return;
        }
        switch (type(object) & ~T_ATOM) {
        case T_STRING:
                write_string(out, object);
                break;
        case T_NUMBER:
                write_number(out, object);
                break;
        case T_SYMBOL:
                write_symbol(out, object);
                break;
        case T_PAIR:
                write_pair(out, object);
                break;
        case T_PROC:
                write_proc(out, object);
                break;
        case T_MACRO:
                write_macro(out, object);
                break;
        default:
                wirte_unknown(out, object);
                break;
        }
}

static void write_string(port *out, pointer object)
{
        fprintf(out, "\"%s\"", str(object));
}
                
static void write_number(port *out, pointer object)
{
        fprintf(out, "%d", num(object));
}
static void write_symbol(port *out, pointer object)
{
        fprintf(out, "%s", sym(object));
}
static void write_pair(port *out, pointer object)
{
        fprintf(out, "(");
        if (cdr(object) == NULL) {
                write(out, car(object));
                fprintf(out, ")");
                return;
        }

        if (!ispair(cdr(object))) {
                write(out, car(object));
                fprintf(out, " . ");
                write(out, cdr(object));
        } else
                while (object != NULL) {
                        write(out, car(object));
                        fprintf(out, " ");
                        object = cdr(object);
                }
        fprintf(out, "\b)");
}
static void write_proc(port *out, pointer object)
{
        fprintf(out, "procedure %p", object);
}
static void write_macro(port *out, pointer object)
{
        fprintf(out, "macro %p", object);
}
static void wirte_unknown(port *out, pointer object)
{
        fprintf(out, "unknown object %p", object);
}
