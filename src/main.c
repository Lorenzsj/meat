#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <argp.h>
#include <confuse.h>
#include <zlog.h>
#include <meat.h>

const char *argp_program_version =
    "meat 0.0.2";
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
    int  logging;    /* file arg to '--log'    */
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

    return 0;
}

/* Argp parser */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv) {
    /* Handle command-line arguments */
    struct arguments arguments;

    /* Default argument values */
    arguments.abort = 0;
    arguments.logging = 0;
    arguments.config_file = "-";

    /* Parse our arguments; every option seen by parse_opt will be
       reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Command-line data */
    /* Handle abort flag */
    if (arguments.abort)
    {
        /* This flag is currently useless */
        error(10, 0, "ABORTED");
    }

    /* Handle logging flag */
    /* The if below should kill main if debug is not defined */
    zlog_category_t *debug;
    if (arguments.logging)
    {
        int rc;

        /* Load zlog configuration file */
        rc = zlog_init("zlog.conf");
        
        if (rc)
        {
            /* Fail if configuration is not found */
            printf("zlog configuration failed.\n");

            return 1;
        }

        /* Select category from file */
	debug = zlog_get_category("meat");

        /* Fail if category is not found */
	if (!debug)
	{
	    printf("Get meat category failed.\n");
            zlog_fini();

            return 2;    
	}

        printf("Started logging\n"); // debug

        /* Write data to file */
        zlog_info(debug, "hello, zlog");
    }

    /* Command-line data */
    /* Debug output */
    printf ("ARG1 = %s\n", arguments.arg1);
    printf ("STRINGS = ");

    int i;
    for (i = 0; arguments.strings[i]; i++)
    {
        printf(i == 0 ? "%s" : ", %s", arguments.strings[i]);
    }
    printf("\n");
    printf("config_file = %s\nlogging = %s\n",
            arguments.config_file,
            arguments.logging ? "yes" : "no"); 
   
    /* Handle configuration file */
    if (strcmp(arguments.config_file, "-"))
    {
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
        if (cfg_parse(cfg, arguments.config_file) == CFG_PARSE_ERROR)
        {
            printf("Error loading configuration file\n");

            return 1;
        }

        /* Configuration data */
        printf("Hello, %s!\n", cfg_getstr(cfg, "name")); 

        /* Clean up */
        cfg_free(cfg);

        printf("Successfully loaded configuration file\n"); // debug
    }
 
    /* Begin Meat loop */
    run();

    if (arguments.logging)
    {
        /* Close logging file */
        zlog_fini();

        printf("Stopped logging\n"); // debug
    }

    return 0;
}
