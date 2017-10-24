/**
 * @file camel.h
 * @author Stephen Lorenz
 * @date 23 Oct 2017
 * @brief A wrapper for easily embedding Perl scripts. Dangerous.
 * 
 * This is intended to be used for  prototyping.
 *
 * @see https://github.com/Lorenzsj/meat/wiki 
 */

#ifndef CAMEL_H
#define CAMEL_H

#include <EXTERN.h>
#include <perl.h>

extern PerlInterpreter *my_perl;

void camel(void);

void camel_power(int a, int b);

int camel_test(void);

#endif /* CAMEL_H */
