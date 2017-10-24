#include <stdio.h>
#include <stdlib.h>
#include <camel.h>

enum CAMEL_ERRORS
{
    SUCCESS = 0
};

void camel(void)
{
    printf("Perl is great!\n");
}

void camel_power(int a, int b)
{
    dSP;                            /* initialize stack pointer         */
    ENTER;                          /* everything created after here    */
    SAVETMPS;                       /* ...is a temporary variable.      */
    PUSHMARK(SP);                   /* remember the stack pointer       */
    XPUSHs(sv_2mortal(newSViv(a))); /* push the base onto the stack     */
    XPUSHs(sv_2mortal(newSViv(b))); /* push the exponent onto stack     */
    PUTBACK;                        /* make local stack pointer global  */
    call_pv("expo", G_SCALAR);      /* call the function                */
    SPAGAIN;                        /* refresh stack pointer            */
                                    /* pop the return value from stack  */
    printf("Meat: %d to the %dth power is %d.\n", a, b, POPi);
    PUTBACK;
    FREETMPS;                       /* free that return value           */
    LEAVE;                          /* ...and the XPUSHed "mortal" args */
}

/* Perl Embed */
int camel_test(void)
{
    /* Parameters for the subroutine */
    char *args[] = {NULL};

    /* Call debug subroutine from meat.pl */
    call_argv("debug", G_DISCARD | G_NOARGS, args);

    /* Inline perl commands */
    /* Treat $a as an integer */
    eval_pv("$a = 3; $a **= 2", TRUE);
    printf("a = %d\n", SvIV(get_sv("a", FALSE)));

    /* Treat $a as a float */
    eval_pv("$a = 3.14; $a *= 2", TRUE);
    printf("a = %f\n", SvNV(get_sv("a", FALSE)));

    STRLEN n_a;
    SV *val = eval_pv("reverse 'llaW yrraL'", TRUE);
    printf("%s\n", SvPV(val, n_a));

    return SUCCESS;
}
