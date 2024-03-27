/**
 * cattie.h
 *
 * Written by Renato Fermi <repizza@gmail.com> 
 *
 * Window
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

  #define WINDOW_TITLE "Cattie"
//  #define gstCmdLine.szWinWidth  800
//  #define gstCmdLine.szWinHeight 800
  #define WINDOW_RATIO  gstCmdLine.szWinWidth/gstCmdLine.szWinHeight

  #define REDRAW_MENU_CLKD -10
  #define REDRAW_NONE  0
  #define REDRAW_IMAGE 1
  #define ERROR_WALKING 10

  #define VSYNC_TIME 16.666666666 //tempo em ms para atualização em 60 FPS

  #define BUTTON_UNKNOWN   0
  #define BUTTON_DIRECTION 1
  #define BUTTON_CONFIRM   2
  #define BUTTON_ERASE     3
  #define BUTTON_CONFIGURE 4

  #define PLAYER_IMG_PATH_IDX  0
  #define FORWARD_IMG_PATH_IDX 1
  #define LASER_IMG_PATH_IDX   2
  #define ROTATE_IMG_PATH_IDX  3
  #define GEAR_IMG_PATH_IDX    4
  #define END_PATH_LIST        5  // Must be the last
 
  #define NEXT_IMG_PATH 1

  #define DEVELOPER     "Renato Fermi & Gustavo Bacagine"
  #define VERSION       "1.0"
  #define COPYRIGHT     "Copyright (C) 2023"
  #define RFERMI_MAIL   "repiazza@gmail.com"
  #define BACAGINE_MAIL "gustavo.bacagine@protonmail.com"
  #define DESCRIPTION   "cattie logic game"

  #define EVEN 0
  #define ODD  1  
  

/******************************************************************************
 *                                                                            *
 *                  Typedefs, structures, unions and enums                    *
 *                                                                            *
 ******************************************************************************/
  typedef enum SquareType {
    INVALID_SQUARE = -1,
    WALL_SQUARE = 0,
    PATH_SQUARE,
    END_SQUARE
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
    int iFacingPos;
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
  void vUpdateCmdTmpList( int iAct, SDL_Rect *pSDL_Rect, SDL_Texture *pSDL_TXTR_CmdListHud, SDL_Renderer *pSDL_Rndr ); 

#endif /* _CATTIE_H_ */

