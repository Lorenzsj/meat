#ifndef MEAT_H
#define MEAT_H

#include <EXTERN.h>
#include <perl.h>

void 
meat_perl_power(PerlInterpreter *my_perl,
                int a, int b);

int meat_perl(PerlInterpreter *my_perl);

int meat_boot(void);

int meat_shutdown(void);

int meat_run(void);

#endif /* MEAT_H */
