/**
 * @file meat.c
 * @author Stephen Lorenz
 * @date 17 Oct 2017
 * @brief A simple interpreter.
 * @see https://github.com/Lorenzsj/meat/wiki 
 */

#include <stdio.h>
#include <stdlib.h>
// #include <EXTERN.h>
// #include <perl.h>

enum ERROR_CODES
{
    SUCCESS = 0
};

/* Perl Embed */


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
