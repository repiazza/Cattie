/**
 * cmdline.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *  
 * My cmdline file
 *
 * Date: 21/10/2023
 */

#include "cmdline.h"
#include "trace.h"

static const char *kszOptStr = "hvt:d:";

/**
 * Command line structure and strings
 */
struct option astCmdOpt[] = {
  { "help"         , no_argument      ,    0, 'h' },
  { "version"      , no_argument      ,    0, 'v' },
  { "trace"        , required_argument,    0, 't' },
  { "debug-level"  , required_argument,    0, 'd' },
  { NULL           , 0                , NULL,  0  }
};

/**
 * Arguments of command line options useds 
 * in usage message of program
 */
const char *pszCmdArguments[] = {
  NULL,
  NULL,
  "file",
  "number",
  NULL,
  "file",
  NULL
};

/**
 * Help messages that showed in usage message
 * of program
 */
const char *pszCmdMessages[] = {
  "Show this message and exit",
  "Show the version and exit",
  "<file> is the path of the debug file",
  "<number> is the level of debug level",
  NULL
};


void vPrintUsage(void)
{
  int ii = 0;
  
  printf("Usage %s [options] <arguments>\n\n"
         "%s\n\n"
         "Options:\n", gkpszProgramName, DESCRIPTION);
  while(astCmdOpt[ii].name)
  {
    if(astCmdOpt[ii].has_arg == required_argument)
    {
      printf("  --%s=<%s>, -%c <%s>\n"
             "    %s\n\n", astCmdOpt[ii].name, pszCmdArguments[ii],
                           astCmdOpt[ii].val, pszCmdArguments[ii],
                           pszCmdMessages[ii]);
    }
    else
    {
      printf("  --%s, -%c\n"
             "    %s\n\n", astCmdOpt[ii].name, astCmdOpt[ii].val,
                           pszCmdMessages[ii]);
    }

    ii++;
  }
}

void vPrintVersion(void)
{
  printf("%s %s\n"
         "Build in %s %s\n"
         "%s %s\n"
         "For reporting bugs, send a email to:\n"
         "<%s>\n" 
         "<%s>\n", gkpszProgramName, 
                   VERSION,
                   __DATE__,
                   __TIME__,
                   COPYRIGHT,
                   DEVELOPER,
                   RFERMI_MAIL,
                   BACAGINE_MAIL
  );
}

bool bCommandLineIsOK(int argc, char **argv)
{
  int iCmdLineOpt = 0;
  
  /**
   * Used to get the final of
   * conversion of strtol
   */
  char *pchEndPtr; 

  while((iCmdLineOpt = getopt_long(argc, argv, kszOptStr, astCmdOpt, NULL )) != -1)
  {
    switch(iCmdLineOpt)
    {
      case 'h':
        vPrintUsage();
        exit(EXIT_SUCCESS);
      case 'v':
        vPrintVersion();
        exit(EXIT_SUCCESS);
      case 't':
        sprintf(stCmdLine.szTraceFile, "%s", optarg);
        break;
      case 'd':
        sprintf(stCmdLine.szDebugLevel, "%s", optarg);

        strtol(stCmdLine.szDebugLevel, &pchEndPtr, 10);

        if(*pchEndPtr != '\0')
        {
          return FALSE;
        }

        break;
      case '?':
      default:
        return FALSE;
    }
  }

  return TRUE;
}

char *szGetProgramName(const char *szPathName)
{
  char *pszProgramName = 0;

  if((pszProgramName = strrchr(szPathName, '/')) != 0)
  {
    ++pszProgramName; /* Skip '/' */
  }
  else
  {
    pszProgramName = (char *) szPathName; /* Nenhum dir. component */
  }

  return pszProgramName;
}
