#include <cattie.h>
 
typedef struct STRUCT_BUTTON_LIST{
  SDL_Rect *pSDL_RECT_Button;
  int iAction;
  struct STRUCT_BUTTON_LIST* pstNext;
} STRUCT_BUTTON_LIST, *PSTRUCT_BUTTON_LIST;

extern STRUCT_BUTTON_LIST gstButtonList;

int iBUTTON_AddToList(SDL_Rect *pSDL_RECT_Btm, int iAction){
  STRUCT_BUTTON_LIST *pstWrkButtonList;

  eMovAction eMvAct = (eMovAction)iAction;

  for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList->pstNext != NULL; pstWrkButtonList = pstWrkButtonList->pstNext );
  
  if ( pstWrkButtonList->iAction == 0 && pstWrkButtonList == &gstButtonList){
      pstWrkButtonList->pSDL_RECT_Button = pSDL_RECT_Btm;
    pstWrkButtonList->iAction = eMvAct;
    pstWrkButtonList->pstNext = NULL;
    return 0;
  }
  if ( (pstWrkButtonList->pstNext = (PSTRUCT_BUTTON_LIST) malloc(sizeof(STRUCT_BUTTON_LIST))) == NULL )
    return -1;
  
  memset(pstWrkButtonList->pstNext, 0, sizeof(STRUCT_BUTTON_LIST));
  pstWrkButtonList = pstWrkButtonList->pstNext;
  pstWrkButtonList->pSDL_RECT_Button = pSDL_RECT_Btm;
  pstWrkButtonList->iAction = eMvAct;
  pstWrkButtonList->pstNext = NULL;

  return 0;
}

void vBUTTON_FreeList(){
  STRUCT_BUTTON_LIST *pstWrkButtonList;

  for ( pstWrkButtonList = gstButtonList.pstNext; pstWrkButtonList != NULL; ){ 
    STRUCT_BUTTON_LIST *pstLastButton = pstWrkButtonList;
    pstWrkButtonList = pstWrkButtonList->pstNext;
    free(pstLastButton);
  }
}

void vBUTTON_InitList(){
  memset(&gstButtonList, 0, sizeof(STRUCT_BUTTON_LIST));
  gstButtonList.pstNext = NULL;
}

