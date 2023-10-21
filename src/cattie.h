/*
* Window
*/
#ifndef _CATTIE_H_INC
  #define _CATTIE_H_INC

  #include <getopt.h>

  #ifndef GXRF_H_INC
    #include "GXRF/GXRF.h"
  #endif
  #define WINDOW_TITLE "Cattie"
  #define WINDOW_WIDTH  800
  #define WINDOW_HEIGHT 800
  #define WINDOW_RATIO  WINDOW_WIDTH/WINDOW_HEIGHT

  #define REDRAW_IMAGE 1
  #define ERROR_WALKING 10

  #ifndef FALSE
    #define FALSE 0
  #endif

  #ifndef TRUE
    #define TRUE  1
  #endif

  #define VSYNC_TIME 16.666666666 //tempo em ms para atualização em 60 FPS

  #define BUTTON_DIRECTION 1
  #define BUTTON_CONFIRM   2
  #define BUTTON_ERASE     3

  #define PLAYER_IMG_PATH_IDX  0
  #define FORWARD_IMG_PATH_IDX 1
  #define LASER_IMG_PATH_IDX   2
  #define ROTATE_IMG_PATH_IDX  3

  #define _MAX_IMG_PATH 4

  #define DEVELOPER     "Renato Fermi & Gustavo Bacagine"
  #define VERSION       "1.0"
  #define COPYRIGHT     "2023 (C)"
  #define RFERMI_MAIL   "repiazza@gmail.com"
  #define BACAGINE_MAIL "gustavo.bacagine@protonmail.com"
  #define DESCRIPTION   "cattie logic game"
  
  char *ppszImagePath[] = {
    "img/cat2.png",
    "img/forward.png",
    "img/laser.png",
    "img/rotate2.png"
  };

  typedef enum SquareType{
    INVALID_SQUARE = -1,
    WALL_SQUARE = 0,
    PATH_SQUARE,
    END_SQUARE
  } eSqType;

  typedef enum MovementAction{
    FORWARD = 1,
    TURN,
    FIRE_LASER,
    ERASE,
    CONFIRM
  } eMovAction;

  typedef enum WindRose{
    NORTH = 1,
    EAST,
    SOUTH,
    WEST
  } eWndRose;

  typedef struct STRUCT_PLAYER {
    int iCurrX;
    int iCurrY;
    int iFacingPos;
    SDL_Rect *pSDL_RECT_Player;
  } STRUCT_PLAYER;

  STRUCT_PLAYER gstPlayer;

  int iWalk();
  int iTurn();
  int iFireLaser();  

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
    char szLogFileName  [_MAX_PATH];
    char szDebugLevel   [256];
  } STRUCT_COMMAND_LINE;

  /**
   * Command line structure and strings
   */
  static struct option astCmdOpt[] = {
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
  static const char *pszCmdArguments[] = {
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
  static const char *pszCmdMessages[] = {
    "Show this message and exit",
    "Show the version and exit",
    "<file> is the path of the debug file",
    "<number> is the level of debug level",
    NULL
  };

#endif /* _CATTIE_H_INC */

