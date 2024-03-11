/*
 * Board
 */

#include <stdlib.h>
#include <cmdline.h>
#include <cattie.h>

#ifndef _BOARD_H
  #define _BOARD_H
    
  #define BOARD_ROWS 10
  #define BOARD_COLS 10

  #define BOARD_SIZE BOARD_ROWS * BOARD_COLS
  
  #define ROW_RATIO  atoi(gstCmdLine.szWinHeight) / BOARD_ROWS
  #define COL_RATIO  atoi(gstCmdLine.szWinWidth) / BOARD_COLS

  extern int giBOARD_Main[BOARD_ROWS][BOARD_COLS];

  void vBOARD_Init(){
    int iX;
    int iY;
    vTraceBegin();
    for (iX = 0; iX < BOARD_ROWS; iX++)
      for (iY = 0; iY < BOARD_COLS; iY++)
        giBOARD_Main[iX][iY] = WALL_SQUARE;

    vTraceEnd();
  }

  void vBOARD_Trace(){
    int ii;
    int jj;
    
    vTraceMsg("========================\n");
    for (ii = 0; ii < BOARD_ROWS; ii++) {
      for (jj = 0; jj < BOARD_COLS; jj++) {
        char szWrk[8];
        memset(szWrk, 0, sizeof(szWrk));
        sprintf(szWrk, "[%d] ", giBOARD_Main[ii][jj]);
        vTraceMsgNoNL(szWrk);
      }
      vTraceMsg("\n");
    }
    vTraceMsg("========================\n");
  }

  int iBOARD_Colorfy(SDL_Renderer *renderer){
    int ii;
    int jj;

    for (ii = 0; ii < BOARD_ROWS; ii++) {
      for (jj = 0; jj < BOARD_COLS; jj++) {
        const int iSquare = giBOARD_Main[ii][jj];
        switch(iSquare){
          // Cinza claro
          case WALL_SQUARE:
            SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
            break;
          // amarelo claro
          case PATH_SQUARE:
            SDL_SetRenderDrawColor(renderer, 255, 255, 224, 255); 
            break;
          // vermelhão
          case END_SQUARE:
            SDL_SetRenderDrawColor(renderer, 235, 150, 70, 255);
            break;
          default:
            // Se não é nada, branco...
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
        }

        SDL_Rect SDLRECT_Square = {jj * COL_RATIO, ii * COL_RATIO, COL_RATIO, COL_RATIO};
        
        SDL_RenderFillRect(renderer, &SDLRECT_Square);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
        SDL_Rect SDLRECT_Edges = {jj * COL_RATIO, ii * COL_RATIO, COL_RATIO, COL_RATIO};
        SDL_RenderDrawRect(renderer, &SDLRECT_Edges);
      }
    }

    return 0;
  }

  int iBOARD_IsSquareInBounds(int iNextX, int iNextY){
    if ( iNextX < 0 || iNextY < 0  )
      return FALSE;

    if ( iNextX >= BOARD_COLS || iNextY >= BOARD_ROWS )
      return FALSE;

    return TRUE;
  }

  int iBOARD_IsValidSquare(int iNextX, int iNextY){
    eSqType eSquareType;

    if ( iBOARD_IsSquareInBounds(iNextX,  iNextY) == FALSE )
      return eSquareType = INVALID_SQUARE;
    
    eSquareType = giBOARD_Main[iNextX][iNextY];

    return (eSquareType > WALL_SQUARE);
  }

  void vInitPlayerAttr(int iDirection){
    gstPlayer.iCurrX = 0;
    gstPlayer.iCurrY = 0;
    if ( iDirection == EVEN ){
      gstPlayer.iFacingPos = SOUTH;
    }
    else{
      gstPlayer.iFacingPos = EAST;
    }
  }
  
  int iBOARD_GenerateRandomPath(){
    int iRow = 0;
    int iCol = 0;
    int bFirstSq = TRUE;
    int iLastRow = 0;
    int iLastCol = 0;
    int iDirection;
    time_t lTime;

    if(DEBUG_MSGS) vTraceBegin();
    
    vBOARD_Init();
    
    time(&lTime);
    srand(lTime);
    
    while (iRow < BOARD_ROWS && iCol < BOARD_COLS) {
      giBOARD_Main[iRow][iCol] = PATH_SQUARE;
      iLastRow = iRow;
      iLastCol = iCol;
      iDirection = rand() % 2; // Generate a random number (0 or 1)
      if (iDirection == EVEN) {
        // EVEN, Go down
        iRow++;
      } else {
        // ODD, Go right
        iCol++;
      }
      if ( bFirstSq ) {
        vInitPlayerAttr(iDirection);
        // gstPlayer.iCurrX = 0;
        // gstPlayer.iCurrY = 0;
        // if ( iDirection == 0 ){
        //   gstPlayer.iFacingPos = SOUTH;
        // }
        // else{
        //   gstPlayer.iFacingPos = EAST;
        // }
        bFirstSq = FALSE;
      }
    }

    giBOARD_Main[iLastRow][iLastCol] = END_SQUARE;
    
    if ( DEBUG_MSGS ) vBOARD_Trace();
    
    if ( DEBUG_MSGS ) vTraceEnd();
    
    return 0;
  }

#endif /* _BOARD_H */
