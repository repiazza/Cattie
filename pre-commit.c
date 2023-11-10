/**
 * template.h
 *
 * Written by DEV_NAME <email@example.com>
 *  
 * My template file
 *
 * Date: dd/mm/aaaa
 */

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

/**
 * Program information defines
 */
#define VERSION        "1.0"
#define DESCRIPTION    "This software is a template"
#define COPYRIGHT      "Copyright (C) 2023"
#define DEVELOPER      "DEV_NAME"
#define DEV_MAIL       "email@example.com"
#define CONF_FILE_NAME "./template.conf"
#define LOG_FILE_NAME  "./template.log"
#define GITHUB_URL     "https://www.github.com/DEV_NAME/template"

/**
 * Program language defines
 */
#define _(STR) gettext(STR)
#define LOCALE_DIR ""

/**
 * This macro is used to remove
 * unused warnings during the
 * compilation.
 */
#define UNUSED(X) (void) X

/******************************************************************************
 *                                                                            *
 *                           Command Line Options                             *
 *                                                                            *
 ******************************************************************************/

/**
 * Structure that represnts
 * command line arguments
 */
typedef struct STRUCT_COMMAND_LINE
{
  char szLogFileName [LOG_FILE_NAME_LENGTH];
  char szDebugLevel  [256];
  char szRevision    [256];
  char szFIles       [256];
} STRUCT_COMMAND_LINE;

/**
 * Command line structure and strings
 */
static struct option astCmdOpt[] = {
  { "help"         , no_argument      ,    0, 'h' },
  { "version"      , no_argument      ,    0, 'v' },
  { "trace"        , required_argument,    0, 't' },
  { "debug-level"  , required_argument,    0, 'd' },
  { "revision"     , required_argument,    0, 'r' },
  { "files"        , required_argument,    0, 'f' },
  { NULL           , 0                , NULL,  0  }
};

/**
 * Arguments of command line options useds 
 * in usage message of program
 */
static const char *pszCmdArguments[] = {
  NULL,
  NULL,
  "file",
  "number",
  "text",
  "files",
  NULL
};

/**
 * Help messages that showed in usage message
 * of program
 */
static const char *pszCmdMessages[] = {
  "Show this message and exit",
  "Show the version and exit",
  "<file> is the path of the debug file",
  "<number> is the level of debug level",
  "revision used in files",
  "<files> is the name of the files will be edit with git revision",
  NULL
};

/* Receive the name of program */
const char *kpszProgramName;

/**
 * Command line arguments
 */
STRUCT_COMMAND_LINE stCmdLine;

int main(int argc, char **argv)
{
  char szCmd[256];
  char szFiles[] = "Teste";

  memset(&stCmdline, 0, sizeof(stCmdline));
  memset(szCmd     , 0, sizeof(szCmd    ));

  snprintf(szCmd, sizeof(szCmd), "sed \'/\\$Id \\$/c\\ * \\$Id: %s \\$\' %s", argv[1], szFiles);

  puts(szCmd);

  return 0;
}

