/**
 * cattie.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 *
 * Window
 */

#ifndef _CATTIE_H_INC
  #define _CATTIE_H_INC

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/
  #ifndef GXRF_H_INC
    #include "GXRF/GXRF.h"
  #endif /* GXRF_H_INC */

/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

  #define WINDOW_TITLE "Cattie"
  #define WINDOW_WIDTH  800
  #define WINDOW_HEIGHT 800
  #define WINDOW_RATIO  WINDOW_WIDTH/WINDOW_HEIGHT

  #define REDRAW_IMAGE 1
  #define ERROR_WALKING 10

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
  #define COPYRIGHT     "Copyright (C) 2023"
  #define RFERMI_MAIL   "repiazza@gmail.com"
  #define BACAGINE_MAIL "gustavo.bacagine@protonmail.com"
  #define DESCRIPTION   "cattie logic game"
  
/******************************************************************************
 *                                                                            *
 *                  Typedefs, structures, unions and enums                    *
 *                                                                            *
 ******************************************************************************/

  typedef struct STRUCT_PLAYER {
    int iCurrX;
    int iCurrY;
    int iFacingPos;
    SDL_Rect *pSDL_RECT_Player;
  } STRUCT_PLAYER;


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

/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

  extern char *ppszImagePath[];
  extern STRUCT_PLAYER gstPlayer;

/******************************************************************************
 *                                                                            *
 *                                 Prototypes                                 *
 *                                                                            *
 ******************************************************************************/ 

  int iWalk();
  int iTurn();
  int iFireLaser();  
  
#endif /* _CATTIE_H_INC */

