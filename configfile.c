/* configfile.c
 * A small and simple library wrote to handle simple config files
 * wrote by Francois Bernard-Cardascia <cardascia.f {AT} gmail.com>
 */

/* History:
 * 19/01/2011: Creation
 * 20/01/2011: first usable version
 */

/* TODOs:
 * - Variable type other than integers
 * - saving config
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>

#include "configfile.h"

/* Public data ****************************************************************/

   /* None */

/* Private constants **********************************************************/

   /* None */

/* Private structures *********************************************************/

typedef struct config_pair
{
    /* the key name in the config file */
    const char*  key_name;
    /* the values its can take */
    KEY_VALUES*  key_values;
    /* a pointer to the modified value in the program */
    int*         var_dst_int;

    struct config_pair* nxt;

} CONFIG_PAIR;

/* Private data ***************************************************************/

/* The path to the config file */
static char cfg_path[256] = {'\0'};

/* a chained list of values to be read in the config file */
static CONFIG_PAIR* keylist = NULL;

/* Private functions **********************************************************/

/** find the config key with the name val
 *  return : NULL if key not found
 */
static CONFIG_PAIR* configfile_parse_options__find_key (const char* val)
{
    CONFIG_PAIR* cur = keylist;
    CONFIG_PAIR* found = NULL;

    while (cur != NULL && found == NULL)
    {
        if (!strcmp (cur->key_name, val))
        {
            found = cur;
        }
        else
        {
            cur = cur->nxt;
        }
    }

    return found;
}

/** apply the selected setting
 *  key : the key name
 *  val : the key value
 */
static void configfile_parse_options__set (const char* key, const char* val)
{
    CONFIG_PAIR* pair = configfile_parse_options__find_key (key);
    int i = 0;

    if (pair != NULL)
    {
        while (i < 128)
        {
            if (strcmp (pair->key_values->key_values[i], val))
            {
                i++;
            }
            else
            {
                *pair->var_dst_int = pair->key_values->prg_values[i];
                i = 999;
            }

        }

        if (i == 128)
        {
            fprintf (stderr, "[WW] invalid argument %s\n", val);
        }
    }
    else
    {
        fprintf (stderr, "[WW] %s is not a supported option\n", key);
    }
}

/** Get a valid line of the config file and send it to the function
 *  configfile_parse_options__set
 *  file : the config file stream
 *  line : a buffer where the line will be wrote
 */
static char* configfile_parse_options__parse_line (FILE* file, char* line)
{
    char*  ret = fgets (line, 127, file);
    int    pass = 0;
    char   key[64] = {'\0'};
    char   value[64] = {'\0'};

    /* go to the first non blank char */
    while (line[0] == ' ' || line[0] == '\0')
    {
        line++;
    }

    /* pass blank lines and comments */
    if (line[0] == '\n' || line[0] == '#')
    {
        pass = 1;
    }

    if (ret && pass == 0)
    {
        if (sscanf (line, "%[^=]=%s", key, value) == 2)
        {
            configfile_parse_options__set (key, value);
        }
        else
        {
            fprintf (stderr, "[WW] Can't parse line: %s", line);
        }
    }

    return ret;
}


/** Create the default config file
 *  default_cfg_str : a string of what the default config file will contain
 */
static void configfile_create_default_file (const char* default_cfg_str)
{
    FILE* file = fopen (cfg_path, "w");

    if (file)
    {
        fprintf (stderr, "[**] Config file created: %s\n", cfg_path);
        fputs (default_cfg_str, file);
        fclose (file);
    }
    else
    {
        WARNING ("Can't create config file. Using default options.");
    }
}


/** return 1 if filename exist, 0 if not */
static int configfile_check_file (const char* filename)
{
    FILE* cfg_path = fopen (filename, "r");
    void* ret = cfg_path;

    if (cfg_path)
    {
        fclose(cfg_path);
    }

    return ret? 1 : 0;
}


/** check/create the config directory */
static void configfile_check_dir (const char* prog_name)
{
    struct passwd *pw = getpwuid(getuid());

    strncpy (cfg_path, pw->pw_dir, 127);
    strncat (cfg_path, "/.config/",127);
    mkdir (cfg_path, S_IRWXU | S_IRWXG);
    strncat (cfg_path, prog_name,127);
    mkdir (cfg_path, S_IRWXU | S_IRWXG);
    strncat (cfg_path, "/",127);
    strncat (cfg_path, prog_name,127);
    strncat (cfg_path, ".conf",127);
}


/* Public functions ***********************************************************/

void configfile_add_key (const char* name, KEY_VALUES* values, void* var_dst)
{
    CONFIG_PAIR* cur  = keylist;
    CONFIG_PAIR* last = NULL;

    if (cur == NULL)
    {
        keylist = malloc (sizeof (CONFIG_PAIR));
        keylist->key_name = name;
        keylist->key_values = values;
        keylist->var_dst_int = var_dst;
        keylist->nxt = NULL;
    }
    else
    {
        while (cur != NULL)
        {
            last = cur;
            cur = cur->nxt;
        }
        cur = malloc (sizeof (CONFIG_PAIR));
        cur->key_name = name;
        cur->key_values = values;
        cur->var_dst_int = var_dst;
        cur->nxt = NULL;
        last->nxt = cur;
    }
}


void configfile_parse_options (void)
{
    char  line[128] = {'\0'};
    FILE* cfg_file = fopen (cfg_path, "r");

    if (cfg_file != NULL)
    {
        while ((configfile_parse_options__parse_line (cfg_file, line)) != NULL);
        fclose (cfg_file);
    }
    else
    {
        WARNING ("Can't open config file. Using Default options");
    }
}


void configfile_init (const char* default_cfg_str, const char* prog_name)
{
    configfile_check_dir (prog_name);

    if (!configfile_check_file (cfg_path))
    {
        configfile_create_default_file (default_cfg_str);
    }
}
