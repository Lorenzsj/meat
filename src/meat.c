/**
 * @file meat.c
 * @author Stephen Lorenz
 * @date 17 Oct 2017
 * @brief A simple interpreter.
 * @see https://github.com/Lorenzsj/meat/wiki 
 */

#include <stdio.h>

enum ERROR_CODES
{
    SUCCESS = 0
};

/** 
 * @brief Handle command-line arguments.
 */
int boot()
{
    printf("Booting Meat...\n"); // debug

    return SUCCESS;
}

/** 
 * @brief Start runtime loop.
 */
int run(void)
{
    boot();

    printf("Running Meat!\n"); // debug

    return SUCCESS;
}
