#include <cattie.h>

#ifndef _BUTTON_H
  #define _BUTTON_H
  typedef enum ButtonAction{
    FORWARD = 1,
    TURN,
    FIRE_LASER,
    ERASE,
    CONFIRM,
    CONFIGURE
  } eBtnAction;


  typedef struct STRUCT_BUTTON_LIST{
    SDL_Rect *pSDL_RECT_Button;
    int iAction;
    int iButtonType;
    struct STRUCT_BUTTON_LIST* pstNext;
  } STRUCT_BUTTON_LIST, *PSTRUCT_BUTTON_LIST;

  extern STRUCT_BUTTON_LIST gstButtonList;

  int iBUTTON_CheckInteraction(SDL_Event *pSDL_EVENT_Ev, int iXCursor, int iYCursor){
    STRUCT_BUTTON_LIST *pstWrkButtonList;

    UNUSED(pSDL_EVENT_Ev);

    for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext){
      SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;
      if ( pSDL_RECT_Btn == NULL )
        break;
        
      if (iXCursor >= pSDL_RECT_Btn->x 
        && iXCursor < pSDL_RECT_Btn->x + pSDL_RECT_Btn->w 
        && iYCursor >= pSDL_RECT_Btn->y 
        && iYCursor < pSDL_RECT_Btn->y + pSDL_RECT_Btn->h) {
        return pstWrkButtonList->iAction;
      }
    }
    return 0;
  }

  int iGetButtonTypebyAction(int iAction){
    int iType = BUTTON_UNKNOWN;
    switch ( iAction ){
      case FORWARD:
      case TURN:
      case FIRE_LASER:
        iType = BUTTON_DIRECTION;
        break;
      case ERASE:
        iType = BUTTON_ERASE;
        break;
      case CONFIRM:
        iType = BUTTON_CONFIRM;
        break;
      case CONFIGURE:
        iType = BUTTON_CONFIGURE;
        break;
      default:
        break;
    }
    return iType;
  }

  int iBUTTON_AddToList(SDL_Rect *pSDL_RECT_Btm, int iAction){
    STRUCT_BUTTON_LIST *pstWrkButton;

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
  }

  void vBUTTON_FreeList(){
    STRUCT_BUTTON_LIST *pstWrkButton;

    for ( pstWrkButton = gstButtonList.pstNext; pstWrkButton != NULL; ){ 
      STRUCT_BUTTON_LIST *pstLastButton = pstWrkButton;
      pstWrkButton = pstWrkButton->pstNext;
      free(pstLastButton);
    }
  }

  void vBUTTON_InitList(){
    memset(&gstButtonList, 0, sizeof(STRUCT_BUTTON_LIST));
    gstButtonList.pstNext = NULL;
  }

  void vBUTTON_Draw(SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Button, int iButtonType){
    if(DEBUG_MSGS) vTraceBegin();

    if ( iButtonType == BUTTON_DIRECTION )
    {
      if(DEBUG_MORE_MSGS) vTraceVarArgs("iButtonType == BUTTON_DIRECTION");
      
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    }
    else if ( iButtonType == BUTTON_CONFIRM )
    {
      if(DEBUG_MORE_MSGS) vTraceVarArgs("iButtonType == BUTTON_CONFIRM");
      
      SDL_SetRenderDrawColor(renderer, 0, 200, 50, 255);
    }
    else if ( iButtonType == BUTTON_ERASE )
    {
      if(DEBUG_MORE_MSGS) vTraceVarArgs("iButtonType == BUTTON_ERASE");

      SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    }

    SDL_RenderFillRect(renderer, pSDL_RECT_Button);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, pSDL_RECT_Button);

    if(DEBUG_MSGS) vTraceEnd();
  }

  void vBUTTON_DrawList(SDL_Renderer *renderer){
    STRUCT_BUTTON_LIST *pstWrkButton;

    for ( pstWrkButton = &gstButtonList; pstWrkButton != NULL; pstWrkButton = pstWrkButton->pstNext ){
      vBUTTON_Draw(renderer, pstWrkButton->pSDL_RECT_Button, pstWrkButton->iButtonType);
    }
  }
#endif /* _BUTTON_H */