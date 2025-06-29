/**
 * button.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 * 
 * Button data structures and rendering
 * 
 */

#include <cattie.h>
#include <rect.h>

#ifndef _BUTTON_H_
  #define _BUTTON_H_

  #define BUTTON_SIZE_RATIO 0.06

  typedef enum ButtonAction {
    FORWARD = 6,
    TURN,
    FIRE_LASER,
    ERASE,
    CONFIRM,
    CONFIGURE,
    CMD_LIST
  } eBtnAction;

  typedef struct STRUCT_BUTTON_LIST {
    SDL_Rect *pSDL_RECT_Button;
    int iAction;
    int iButtonType;
    struct STRUCT_BUTTON_LIST* pstNext;
  } STRUCT_BUTTON_LIST, *PSTRUCT_BUTTON_LIST;

  extern STRUCT_BUTTON_LIST gstButtonList;
  
  SDL_Rect *pBUTTON_GetRectByImg( int iImgIx ) {
    STRUCT_BUTTON_LIST *pstWrkButton = NULL;

    for ( pstWrkButton = &gstButtonList; pstWrkButton != NULL; pstWrkButton = pstWrkButton->pstNext ) {
      switch ( iImgIx ) {
        case PLAYER_IMG_PATH_IDX: {
            return gstPlayer.pSDL_RECT_Player;
          break;
        }
        case FORWARD_IMG_PATH_IDX: {
          if ( pstWrkButton->iAction == FORWARD )
            return pstWrkButton->pSDL_RECT_Button;
          break;
        }
        case LASER_IMG_PATH_IDX: {
          if ( pstWrkButton->iAction == FIRE_LASER )
            return pstWrkButton->pSDL_RECT_Button;
          break;
        }
        case GEAR_IMG_PATH_IDX: {
          if ( pstWrkButton->iAction == CONFIGURE )
            return pstWrkButton->pSDL_RECT_Button;
          break;
        }
        case ROTATE_IMG_PATH_IDX: {
          if ( pstWrkButton->iAction == TURN )
            return pstWrkButton->pSDL_RECT_Button;
          break;
        }
        default: break;
      } /* switch */
    } /* for */
    return NULL;
  } /* pBUTTON_GetRectByImg */

  int iBUTTON_CheckInteraction( int iXCursor, int iYCursor ) {
    STRUCT_BUTTON_LIST *pstWrkButtonList = NULL;

    for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext ) {
      SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;

      if ( pSDL_RECT_Btn == NULL )
        break;
        
      if ( bAreCoordsInSDL_Rect(pSDL_RECT_Btn, iXCursor, iYCursor) ) {
        return pstWrkButtonList->iAction;
      }
    }

    return REDRAW_NONE;
  } /* iBUTTON_CheckInteraction */

  int iGetButtonTypebyAction( int iAction ) {
    int iType = BUTTON_UNKNOWN;

    switch ( iAction ) {
      case FORWARD:
      case TURN:
      case FIRE_LASER: {
        iType = BUTTON_DIRECTION;
        break;
      }
      case ERASE: {
        iType = BUTTON_ERASE;
        break;
      }
      case CONFIRM: {
        iType = BUTTON_CONFIRM;
        break;
      }
      case CONFIGURE: {
        iType = BUTTON_CONFIGURE;
        break;
      }
      default: break;
    } /* switch */
    return iType;
  } /* iGetButtonTypebyAction */

  int iBUTTON_AddToList( SDL_Rect *pSDL_RECT_Btm, int iAction ) {
    STRUCT_BUTTON_LIST *pstWrkButton = NULL;

    eBtnAction eBtnAct = (eBtnAction)iAction;

    for ( pstWrkButton = &gstButtonList; pstWrkButton->pstNext != NULL; pstWrkButton = pstWrkButton->pstNext );
    
    if ( pstWrkButton->iAction == 0 && pstWrkButton == &gstButtonList){
      pstWrkButton->pSDL_RECT_Button = (SDL_Rect *) malloc(sizeof(SDL_Rect));
      if ( pstWrkButton->pSDL_RECT_Button == NULL )
        return -1;

      memset(pstWrkButton->pSDL_RECT_Button, 0, sizeof(SDL_Rect));
      memcpy(pstWrkButton->pSDL_RECT_Button, pSDL_RECT_Btm, sizeof(SDL_Rect));
      pstWrkButton->iAction = eBtnAct;
      pstWrkButton->iButtonType = iGetButtonTypebyAction(pstWrkButton->iAction);
      pstWrkButton->pstNext = NULL;
      return 0;
    }

    if ( (pstWrkButton->pstNext = (PSTRUCT_BUTTON_LIST) malloc(sizeof(STRUCT_BUTTON_LIST))) == NULL )
      return -1;
    
    pstWrkButton = pstWrkButton->pstNext;
    pstWrkButton->pSDL_RECT_Button = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    if ( pstWrkButton->pSDL_RECT_Button == NULL )
      return -1;
    memset(pstWrkButton->pSDL_RECT_Button, 0, sizeof(SDL_Rect));
    memcpy(pstWrkButton->pSDL_RECT_Button, pSDL_RECT_Btm, sizeof(SDL_Rect));
    pstWrkButton->iAction = eBtnAct;
    pstWrkButton->iButtonType = iGetButtonTypebyAction(pstWrkButton->iAction); 
    pstWrkButton->pstNext = NULL;

    return 0;
  } /* iBUTTON_AddToList */

  void vBUTTON_InitList( void ) {
    memset( &gstButtonList, 0x00, sizeof(STRUCT_BUTTON_LIST ) );
    gstButtonList.pstNext = NULL;
  } /* vBUTTON_InitList */
  
  void vBUTTON_FreeList( void ) {
    STRUCT_BUTTON_LIST *pstWrkButton = NULL;

    if ( DEBUG_MSGS ) vTraceBegin();
    
    for ( pstWrkButton = gstButtonList.pstNext; pstWrkButton != NULL; ) {
      STRUCT_BUTTON_LIST *pstLastButton = pstWrkButton;
      pstWrkButton = pstWrkButton->pstNext;
      free( pstLastButton->pSDL_RECT_Button);
      free( pstLastButton );
    }
    
    vBUTTON_InitList();

    if ( DEBUG_MSGS ) vTraceEnd();
  } /* vBUTTON_FreeList */

  void vBUTTON_Draw( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Button, int iButtonType ) {
    if ( DEBUG_MSGS ) vTraceBegin();

    if ( iButtonType == BUTTON_DIRECTION || iButtonType == BUTTON_CONFIGURE ) {
      if( DEBUG_MORE_MSGS ) vTraceVarArgs("iButtonType == BUTTON_DIRECTION");
      
      SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    }
    else if ( iButtonType == BUTTON_CONFIRM ) {
      if( DEBUG_MORE_MSGS ) vTraceVarArgs("iButtonType == BUTTON_CONFIRM");
      
      SDL_SetRenderDrawColor(renderer, 0, 200, 50, 255);
    }
    else if ( iButtonType == BUTTON_ERASE ) {
      if( DEBUG_MORE_MSGS ) vTraceVarArgs("iButtonType == BUTTON_ERASE");

      SDL_SetRenderDrawColor( renderer, 255, 255, 200, 255 );
    }

    SDL_RenderFillRect( renderer, pSDL_RECT_Button );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, pSDL_RECT_Button );

    if( DEBUG_MSGS ) vTraceEnd();
  } /* vBUTTON_Draw */

  void vBUTTON_DrawList( SDL_Renderer *renderer ) {
    STRUCT_BUTTON_LIST *pstWrkButton = NULL;

    for ( pstWrkButton = &gstButtonList; pstWrkButton != NULL; pstWrkButton = pstWrkButton->pstNext ) {
      vBUTTON_Draw( renderer, pstWrkButton->pSDL_RECT_Button, pstWrkButton->iButtonType );
    }
  } /* vBUTTON_DrawList */
  void vBUTTON_SetDimensions( SDL_Rect *pSDL_RECT_Btn, int iXTrslt, int iYTrslt ) {
    if ( DEBUG_MSGS ) vTraceBegin();

    pSDL_RECT_Btn->w = BUTTON_SIZE_RATIO * INT_WINDOW_WIDTH;
    pSDL_RECT_Btn->h = BUTTON_SIZE_RATIO * INT_WINDOW_HEIGHT;
    pSDL_RECT_Btn->x = pSDL_RECT_Btn->w + iXTrslt;
    pSDL_RECT_Btn->y = INT_WINDOW_HEIGHT - pSDL_RECT_Btn->h - iYTrslt;

    vRECT_Trace(pSDL_RECT_Btn);

    if ( DEBUG_MSGS ) vTraceEnd();
  } /* vBUTTON_SetDimensions */
  /**
   *  ---------------------------------
   * |  __   __   __      __      __   | 
   * | |Wl| |Tr| |Fr|    |BS|    |OK|  |
   * | |__| |__| |__|    |__|    |__|  |
   * |_________________________________|
  */
  void vBUTTON_InitButtonHUDRect(){
    SDL_Rect SDL_RECT_ButtonArrowRight;
    SDL_Rect SDL_RECT_ButtonTurnArrow;
    SDL_Rect SDL_RECT_ButtonFireLaser;
    SDL_Rect SDL_RECT_ButtonUndoLast;
    SDL_Rect SDL_RECT_ButtonConfirm;
    SDL_Rect SDL_RECT_ButtonConfigure;
    int iXTranslation = 0;
    int iYTranslation = 20;

    /* Set button size and position [x,y] and width + height **/
    vBUTTON_SetDimensions( &SDL_RECT_ButtonArrowRight, iXTranslation , iYTranslation);
    iXTranslation += SDL_RECT_ButtonArrowRight.w + 10;
    vBUTTON_SetDimensions( &SDL_RECT_ButtonTurnArrow,  iXTranslation, iYTranslation);
    iXTranslation += SDL_RECT_ButtonTurnArrow.w  + 10;
    vBUTTON_SetDimensions( &SDL_RECT_ButtonFireLaser,  iXTranslation, iYTranslation);
    iXTranslation += SDL_RECT_ButtonFireLaser.w  + 50;
    vBUTTON_SetDimensions( &SDL_RECT_ButtonUndoLast,   iXTranslation, iYTranslation);
    iXTranslation += SDL_RECT_ButtonUndoLast.w   + 50;
    vBUTTON_SetDimensions( &SDL_RECT_ButtonConfirm,    iXTranslation, iYTranslation);
    iXTranslation += SDL_RECT_ButtonConfirm.w    + 100;
    vBUTTON_SetDimensions( &SDL_RECT_ButtonConfigure,  iXTranslation, iYTranslation);

    // iXTranslation += SDL_RECT_ButtonArrowRight.w;
    // vBUTTON_SetDimensions( &SDL_RECT_ButtonArrowRight, iXTranslation, iYTranslation);
    // iXTranslation += SDL_RECT_ButtonArrowRight.w + 30;
    // vBUTTON_SetDimensions( &SDL_RECT_ButtonTurnArrow,  iXTranslation, iYTranslation);
    // iXTranslation += SDL_RECT_ButtonTurnArrow.w + 30;
    // vBUTTON_SetDimensions( &SDL_RECT_ButtonFireLaser,  iXTranslation, iYTranslation);
    // iXTranslation += SDL_RECT_ButtonFireLaser.w + 30;
    // vBUTTON_SetDimensions( &SDL_RECT_ButtonUndoLast,   iXTranslation, iYTranslation);
    // iXTranslation += SDL_RECT_ButtonUndoLast.w + 30;
    // vBUTTON_SetDimensions( &SDL_RECT_ButtonConfirm,    iXTranslation, iYTranslation);
    
  
    /* Add them to list **/
    iBUTTON_AddToList( &SDL_RECT_ButtonArrowRight   , FORWARD    );
    iBUTTON_AddToList( &SDL_RECT_ButtonTurnArrow    , TURN       );
    iBUTTON_AddToList( &SDL_RECT_ButtonFireLaser    , FIRE_LASER );
    iBUTTON_AddToList( &SDL_RECT_ButtonUndoLast     , ERASE      );
    iBUTTON_AddToList( &SDL_RECT_ButtonConfirm      , CONFIRM    );
    iBUTTON_AddToList( &SDL_RECT_ButtonConfigure    , CONFIGURE  );
  }
  
#endif /* _BUTTON_H_ */
