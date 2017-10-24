#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <EXTERN.h>
#include <perl.h>
#include <argp.h>
#include <confuse.h>
#include <zlog.h>
#include <meat.h>
#include <camel.h>

/* See https://github.com/Lorenzsj/meat/wiki for more
   information. */

enum MAIN_ERRORS
{
    /* General */
    SUCCESS = 0,

    /* zlog */
    ZLOG_ERR_CONF = 1,
    ZLOG_ERR_CATEGORY = 2,

    /* libConfuse */
    CONFUSE_ERR_CONF = 3 
};

/* Argp */
/* Keys for options without short-options. */
#define OPT_ABORT 1              /* –abort */

const char *argp_program_version =
    "meat 0.0.3";
const char *argp_program_bug_address =
    "<lorenzsj@clarkson.edu>";

/* Program documentation. */
static char doc[] = 
"Meat is a simple interpreted language.\
\vSee 'man meat' for more help with Meat.";

/* A description of the accepted arguments. */
static char args_doc[] = "FILE.pl [STRING...]";

/* Accepted Arguments */
static struct argp_option options[] = 
{
    {"config", 'c', "FILE", 0, "Load configuration file"},
    {"log",    'l', 0,      0, "Write output to file"   },

    {"abort",  OPT_ABORT, 0, 0, "Abort before showing any output"},
    {0}
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *arg1;        /* arg1 */
    char **strings;    /* [string…] */
    int  abort;        /* ‘--abort’ */
    int  logging;      /* file arg to '--log' */
    char *config_file; /* file arg to '--config_file' */
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key) {
        case 'c':
            arguments->config_file = arg;
            break;
        case 'l':
            arguments->logging = 1;
            break;
        case OPT_ABORT:
            arguments->abort = 1;
            break;

        case ARGP_KEY_NO_ARGS:
            argp_usage(state);

        case ARGP_KEY_ARG:
            /* Here we know that state->arg_num == 0, since we
              force argument parsing to end before any more arguments can
              get here. */
            arguments->arg1 = arg;
            /* Now we consume all the rest of the arguments.
              state->next is the index in state->argv of the
              next argument to be parsed, which is the first string
              we’re interested in, so we can just use
              &state->argv[state->next] as the value for
              arguments->strings.

              In addition, by setting state->next to the end
              of the arguments, we can force argp to stop parsing here and
              return. */
            arguments->strings = &state->argv[state->next];
            state->next = state->argc;
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return SUCCESS;
}

/* Argp */
static struct argp argp = {options, parse_opt, args_doc, doc};

/* Perl */
PerlInterpreter *my_perl;

int main(int argc, char **argv, char **env)
{
    /* Perl */
    char *perl_argv[] = {"meat", "meat.pl"};
    int perl_argc = 2;

    /* Initialize interpreter */
    PERL_SYS_INIT3(&argc, &argv, &env);

    /* Create interpreter object */
    /* camel.c has an extern my_perl */
    my_perl = perl_alloc();
    perl_construct(my_perl);
 
    perl_parse(my_perl, NULL, perl_argc, perl_argv, NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;

    /* Argp */
    /* Handle command-line arguments */
    struct arguments arguments;

    /* Default argument values */
    arguments.abort = 0;
    arguments.logging = 0;
    arguments.config_file = "meat.conf";

    /* Parse our arguments; every option seen by parse_opt will be
       reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    
    /* Command-line argument handlers */
    /* zlog */
    zlog_category_t *debug;

    /* Handle logging flag */
    if (arguments.logging) {
        int rc;

        /* Load zlog configuration file */
        rc = zlog_init("zlog.conf");
        
        if (rc) {
            printf("zlog: Unable to load config file\n");

            return ZLOG_ERR_CONF;
        }

        /* Select category from file */
	    debug = zlog_get_category("meat");

        /* Fail if category is not found */
	    if (!debug) {
	        printf("zlog: Unable to get category meat\n");
            zlog_fini();

            return ZLOG_ERR_CATEGORY; 
        }

        printf("Started logging\n"); // debug

        /* Write data to log file */
        zlog_info(debug, "hello, zlog");
    }

    /* Argp debug output */
    printf ("ARG1 = %s\n", arguments.arg1);
    printf ("STRINGS = ");

    int i;
    for (i = 0; arguments.strings[i]; i++) {
        printf(i == 0 ? "%s" : ", %s", arguments.strings[i]);
    }
    printf("\n");
    printf("config_file = %s\nlogging = %s\n",
            arguments.config_file,
            arguments.logging ? "yes" : "no"); 
   
    /* libConfuse */
    /* Handle configuration file */
    if (arguments.config_file) {
        printf("Loading configuration file\n"); // debug

        /* Set default configurations */
        cfg_opt_t opts[] = 
        {
            CFG_STR("name", "Meat", CFGF_NONE),
            CFG_END()
        };
        cfg_t *cfg;
       
        /* Load configuration file */ 
        cfg = cfg_init(opts, CFGF_NONE);
        if (cfg_parse(cfg, arguments.config_file) == CFG_PARSE_ERROR) {
            printf("Error loading configuration file\n");

            return CONFUSE_ERR_CONF;
        }

        /* Configuration data */
        printf("Hello, %s!\n", cfg_getstr(cfg, "name")); 

        /* Clean up */
        cfg_free(cfg);

        printf("Loaded configuration file\n"); // debug
    }

    /* Argp */
    /* Handle abort flag */
    if (arguments.abort) {
        /* This flag is currently useless */
        error(10, 0, "ABORTED");
    }
 
    /* Use perl interpreter in a seperate file */
    printf("\nCalling Perl from camel.c\n");
    camel();
    camel_power(3, 4);
    camel_test();
    printf("Finished calling Perl from camel.c\n\n");

    /* Begin Meat loop */
    meat_run();

    if (arguments.logging) {
        /* Close logging file */
        zlog_fini();

        printf("Stopped logging\n"); // debug
    }

    /* Perl */
    /* Shutdown interpreter */
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();

    return SUCCESS;
}
