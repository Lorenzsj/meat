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
#include <camel.h>

enum MEAT_ERRORS
{
    SUCCESS = 0
};

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

    meat_camel();

    printf("Running Meat!\n"); // debug

    meat_shutdown();

    return SUCCESS;
}

void meat_camel(void)
{
    camel();
    camel_power(3, 4);
    camel_test();
}
