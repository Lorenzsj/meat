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
 * @brief Assemble Meat. Allocate memory. 
 *
 * Handle command-line argument data. 
 */
int boot()
{
    printf("Booting Meat...\n"); // debug

    return SUCCESS;
}

/**
 * @brief Disassemble Meat. Free memory.
 */
int shutdown(void)
{
    printf("Shutting down Meat...\n"); // debug

    return SUCCESS;
}

/** 
 * @brief Start runtime loop.
 */
int run(void)
{
    boot();

    printf("Running Meat!\n"); // debug

    shutdown();

    return SUCCESS;
}
