#ifndef CAMEL_H
#define CAMEL_H

#include <EXTERN.h>
#include <perl.h>

extern PerlInterpreter *my_perl;

void camel(void);

void camel_power(int a, int b);

int camel_test(void);

#endif /* CAMEL_H */
