/**
 * button.h
 *
 * Written by Renato Fermi <repizza@gmail.com>
 */

#include <cattie.h>

#ifndef _BUTTON_H_
  #define _BUTTON_H_
  typedef enum ButtonAction {
    FORWARD = 6,
    TURN,
    FIRE_LASER,
    ERASE,
    CONFIRM,
    CONFIGURE
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

  int iBUTTON_CheckInteraction( SDL_Event *pSDL_EVENT_Ev, int iXCursor, int iYCursor ) {
    STRUCT_BUTTON_LIST *pstWrkButtonList = NULL;

    UNUSED( pSDL_EVENT_Ev );

    for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext ) {
      SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;

      if ( pSDL_RECT_Btn == NULL )
        break;
        
      if ( bAreCoordsInSDL_Rect(pSDL_RECT_Btn, iXCursor, iYCursor) ) {
        return pstWrkButtonList->iAction;
      }
    }

    return 0;
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
      pstWrkButton->pSDL_RECT_Button = pSDL_RECT_Btm;
      pstWrkButton->iAction = eBtnAct;
      pstWrkButton->iButtonType = iGetButtonTypebyAction(pstWrkButton->iAction);
      pstWrkButton->pstNext = NULL;
      return 0;
    }

    if ( (pstWrkButton->pstNext = (PSTRUCT_BUTTON_LIST) malloc(sizeof(STRUCT_BUTTON_LIST))) == NULL )
      return -1;
    
    memset(pstWrkButton->pstNext, 0, sizeof(STRUCT_BUTTON_LIST));
    pstWrkButton = pstWrkButton->pstNext;
    pstWrkButton->pSDL_RECT_Button = pSDL_RECT_Btm;
    pstWrkButton->iAction = eBtnAct;
    pstWrkButton->iButtonType = iGetButtonTypebyAction(pstWrkButton->iAction); 
    pstWrkButton->pstNext = NULL;

    return 0;
  } /* iBUTTON_AddToList */

  void vBUTTON_FreeList( void ) {
    STRUCT_BUTTON_LIST *pstWrkButton = NULL;

    if ( DEBUG_MSGS ) vTraceBegin();
    
    for ( pstWrkButton = gstButtonList.pstNext; pstWrkButton != NULL; ) {
      STRUCT_BUTTON_LIST *pstLastButton = pstWrkButton;
      pstWrkButton = pstWrkButton->pstNext;
      free( pstLastButton );
    }
    
    if ( DEBUG_MSGS ) vTraceEnd();
  } /* vBUTTON_FreeList */

  void vBUTTON_InitList( void ) {
    memset( &gstButtonList, 0x00, sizeof(STRUCT_BUTTON_LIST ) );
    gstButtonList.pstNext = NULL;
  } /* vBUTTON_InitList */

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
#endif /* _BUTTON_H_ */
