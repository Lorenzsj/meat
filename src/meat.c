/**
 * @file meat.c
 * @author Stephen Lorenz
 * @date 17 Oct 2017
 * @brief A simple interpreter.
 * @see https://github.com/Lorenzsj/meat/wiki 
 */

#include <stdio.h>
#include <stdlib.h>
#include <meat.h>

enum MEAT_ERROR_CODES
{
    SUCCESS = 0
};

void
meat_perl_power(PerlInterpreter *my_perl, int a, int b)
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
    printf ("Meat: %d to the %dth power is %d.\n", a, b, POPi);
    PUTBACK;
    FREETMPS;                       /* free that return value           */
    LEAVE;                          /* ...and the XPUSHed "mortal" args */
}

/* Perl Embed */
int meat_perl(PerlInterpreter *my_perl)
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

/** 
 * @brief Assemble Meat. Allocate memory. 
 *
 * Handle command-line argument data. 
 */
int meat_boot(void)
{ 
    printf("Booting Meat\n"); // debug

    return SUCCESS;
}

/**
 * @brief Disassemble Meat. Free memory.
 */
int meat_shutdown(void)
{
    printf("Shutting down Meat\n"); // debug

    return SUCCESS;
}

/** 
 * @brief Start runtime loop.
 */
int meat_run(void)
{
    meat_boot();

    printf("Running Meat!\n"); // debug

    meat_shutdown();

    return SUCCESS;
}
