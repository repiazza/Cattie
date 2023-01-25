#include "GXRF/GXRF.h"
// #include "GXRF/trace.h"
/*
* Window
*/
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

/*
* Board
*/
#define BOARD_ROWS 10
#define BOARD_COLS 10
// #define BOARD_ROWS 20
// #define BOARD_COLS 20
#define BOARD_SIZE BOARD_ROWS * BOARD_COLS

#define ROW_RATIO  WINDOW_HEIGHT / BOARD_ROWS
#define COL_RATIO  WINDOW_WIDTH  / BOARD_COLS

#define VSYNC_TIME 16.666666666 //tempo em ms para atualização em 60 FPS
#define _MAX_MOVEMENT BOARD_COLS*BOARD_ROWS*10

#define BUTTON_DIRECTION 1
#define BUTTON_CONFIRM   2
#define BUTTON_ERASE     3


#define IMG_PATH "img.png"

#define PLAYER_IMG_PATH_IDX  0
#define FORWARD_IMG_PATH_IDX 1
#define LASER_IMG_PATH_IDX   2
#define ROTATE_IMG_PATH_IDX  3

#define _MAX_IMG_PATH 4

#define DEBUG_MSGS 1
#define DEBUG_MORE_MSGS 9

#define MAX_MENU_OPTIONS 6

char *ppszImagePath[] = {
  "img/cat2.png",
  "img/forward.png",
  "img/laser.png",
  "img/rotate2.png"
};

typedef struct STRUCT_BUTTON_LIST{
  SDL_Rect *pSDL_RECT_Button;
  int iAction;
  struct STRUCT_BUTTON_LIST* pstNext;
}STRUCT_BUTTON_LIST;

STRUCT_BUTTON_LIST gstButtonList;

enum SquareColors{
  WALL_SQUARE = 0,
  PATH_SQUARE,
  END_SQUARE
}eSqColors;


enum MovementAction{
  FORWARD = 1,
  TURN,
  FIRE_LASER,
  ERASE,
  CONFIRM
}eMovAction;


enum WindRose{
  NORTH = 1,
  EAST,
  SOUTH,
  WEST
}eWndRose;

typedef struct STRUCT_PLAYER {
  int iCurrX;
  int iCurrY;
  int iFacingPos;
  SDL_Rect *pSDL_RECT_Player;
}STRUCT_PLAYER;

STRUCT_PLAYER gstPlayer;
