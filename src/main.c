#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <argp.h>
#include <confuse.h>
#include <zlog.h>
#include <meat.h>

const char *argp_program_version =
    "meat 0.0.1";
const char *argp_program_bug_address =
    "<lorenzsj@clarkson.edu>";

/* Program documentation. */
static char doc[] = 
"Meat is a simple interpreted language.\
\vSee 'man meat' for more help with Meat.";

/* A description of the accepted arguments. */
static char args_doc[] = "ARG1 [STRING...]";

/* Keys for options without short-options. */
#define OPT_ABORT 1              /* –abort */

/* Accepted Arguments */
static struct argp_option options[] = 
{
    { "config", 'c', "FILE", 0, "Load configuration file" },
    { "log",    'l', "FILE", 0, "Write output to file"    },

    { "abort",  OPT_ABORT, 0, 0, "Abort before showing any output" },
    { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *arg1;        /* arg1 */
    char **strings;    /* [string…] */
    int  abort;        /* ‘--abort’ */
    char *log_file;    /* file arg to '--log'    */
    char *config_file; /* file arg to '--config_file' */
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'c':
            arguments->config_file = arg;
            break;
        case 'l':
            arguments->log_file = arg;
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

    return 0;
}

/* Argp parser */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv) {
    /* Handle command-line arguments */
    int j;
    struct arguments arguments;

    /* Default values. */
    arguments.abort = 0;
    arguments.log_file = "-";
    arguments.config_file = "-";

    /* Parse our arguments; every option seen by parse_opt will be
       reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Command-line arguments */
    if (arguments.abort)
    {
        error(10, 0, "ABORTED");
    }

    /* Command-line data */
    printf ("ARG1 = %s\n", arguments.arg1);
    printf ("STRINGS = ");
    for (j = 0; arguments.strings[j]; j++)
    {
        printf(j == 0 ? "%s" : ", %s", arguments.strings[j]);
    }
    printf("\n");
    printf("config_file = %s\nlog_file = %s\n",
            arguments.config_file,
            arguments.log_file); 
   
    /* Handle configuration file */
    if (strcmp(arguments.config_file, "-"))
    {
        /* Set default configurations */
        cfg_opt_t opts[] = 
        {
            CFG_STR("name", "Meat", CFGF_NONE),
            CFG_END()
        };
        cfg_t *cfg;
       
        /* Load configuration file */ 
        cfg = cfg_init(opts, CFGF_NONE);
        if (cfg_parse(cfg, arguments.config_file) == CFG_PARSE_ERROR)
        {
            printf("Error loading configuration file.\n");

            return 1;
        }

        /* Configuration data */
        printf("Hello, %s!\n", cfg_getstr(cfg, "name"));
    

        /* Clean up */
        cfg_free(cfg);
    }
    else
    {
        printf("No configuration file specified. Moving on...\n");
    }

    /* Handle logging file */
    if (strcmp(arguments.log_file, "-"))
    {
        int rc;

        zlog_category_t *c;

        rc = zlog_init("zlog.conf");
        
        if (rc)
        {
            printf("zlog init failed\n");

            return 1;
        }
    }
    else
    {
        printf("No logging file specified. Moving on...\n");
    }
 
    /* Begin Meat loop */
    run();

    return 0;
}
