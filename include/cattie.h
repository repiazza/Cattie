/**
 * cattie.h
 *
 * Written by Renato Fermi <repizza@gmail.com> 
 *
 * Main source header file
 * 
 */

#ifndef _CATTIE_H_
  #define _CATTIE_H_

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/
  
  #include <GXRF.h>
  
/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

  #define WINDOW_TITLE "Cattie - Logic Game 4 Kids"
  #define INT_WINDOW_WIDTH  atoi( gstCmdLine.szWinWidth )
  #define INT_WINDOW_HEIGHT atoi( gstCmdLine.szWinHeight )
  #define WINDOW_RATIO  INT_WINDOW_WIDTH/INT_WINDOW_HEIGHT

  #define REDRAW_MENU_CLKD -10
  #define REDRAW_ERROR -1
  #define REDRAW_NONE  0          /* Not to redraw the scene **/
  #define REDRAW_IMAGE 1
  #define ERROR_WALKING 10

  #define VSYNC_TIME 16.666666666 /* Time(ms) to ensure 60hz refresh rate **/

  #define BUTTON_UNKNOWN   0
  #define BUTTON_DIRECTION 1
  #define BUTTON_CONFIRM   2
  #define BUTTON_ERASE     3
  #define BUTTON_CONFIGURE 4
  #define BUTTON_CMD_LIST  5

  #define PLAYER_IMG_PATH_IDX  0
  #define FORWARD_IMG_PATH_IDX 1
  #define LASER_IMG_PATH_IDX   2
  #define ROTATE_IMG_PATH_IDX  3
  #define GEAR_IMG_PATH_IDX    4
  #define END_PATH_LIST        5  /* END_ must always be the last **/
 
  #define PREV_IMG_PATH -1
  #define NEXT_IMG_PATH  1

  #define DEVELOPER     "Renato Fermi & Gustavo Bacagine"
  #define VERSION       "1.0"
  #define COPYRIGHT     "Copyright (C) 2023"
  #define RFERMI_MAIL   "repiazza@gmail.com"
  #define BACAGINE_MAIL "gustavo.bacagine@protonmail.com"
  #define DESCRIPTION   "Cattie logic game"

  #define EVEN 0
  #define ODD  1

/******************************************************************************
 *                                                                            *
 *                  Typedefs, structures, unions and enums                    *
 *                                                                            *
 ******************************************************************************/
  typedef enum SquareType {
    INVALID_SQUARE = -1,
    WALL_SQUARE = 0, /* Dead End **/
    PATH_SQUARE,     /* Safe Way **/
    END_SQUARE       /* Finish Line **/
  } eSqType;
  
  typedef enum WindRose {
    NORTH = 1,       
    EAST,
    SOUTH,
    WEST
  } eWndRose;

  typedef struct STRUCT_PLAYER {
    int iCurrX;
    int iCurrY;
    int iFacingPos;   /* Where player img is facing to (where to walk) **/ 
    SDL_Rect *pSDL_RECT_Player;
  } STRUCT_PLAYER;
  
/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

  extern char *ppszImagePath[];
  extern STRUCT_PLAYER gstPlayer;
  extern SDL_Rect *pSDL_RECT_Menu;

/******************************************************************************
 *                                                                            *
 *                                 Prototypes                                 *
 *                                                                            *
 ******************************************************************************/ 

  int iWalk( void );
  int iTurn( void );
  int iFireLaser( void );
  int iHandleClick( SDL_Texture *pSDL_TXTR_CmdListHud );
  int iHandleEventKey( SDL_Event *pSDL_EVENT_Ev );
  int iHandleMouseMotion(SDL_Rect *pSDL_RECT_Menu);
  SDL_Surface *pSDL_SRFC_LoadImage( char *pszImgPath );
  
#endif /* _CATTIE_H_ */

