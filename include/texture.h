#include <cattie.h>
/** 
 * 
 *   texture.h
 * 
 *   Handles surfaces, textures, images, etc.
 * 
*/

#ifndef _TEXTURE_H
  #define _TEXTURE_H

  typedef struct STRUCT_TEXTURE_LIST{
    SDL_Texture *pSDL_Texture;
    char *pszImgPath;
    struct STRUCT_TEXTURE_LIST* pstNext;
  } STRUCT_TEXTURE_LIST, *PSTRUCT_TEXTURE_LIST;
  
  extern STRUCT_TEXTURE_LIST gstTextureList;

  SDL_Texture *pSDL_TXTR_AddToList(SDL_Renderer *renderer, char *pszImgPath){
    STRUCT_TEXTURE_LIST *pstWrkTxtr;

    for ( pstWrkTxtr = &gstTextureList; pstWrkTxtr->pstNext != NULL; pstWrkTxtr = pstWrkTxtr->pstNext );
    
    if ( pstWrkTxtr == &gstTextureList){
      pstWrkTxtr->pSDL_Texture = IMG_LoadTexture(renderer, pszImgPath);
      pstWrkTxtr->pszImgPath = pszImgPath;
      pstWrkTxtr->pstNext = NULL;
      return pstWrkTxtr->pSDL_Texture;
    }
    if ( (pstWrkTxtr->pstNext = (PSTRUCT_TEXTURE_LIST) malloc(sizeof(STRUCT_TEXTURE_LIST))) == NULL )
      return NULL;
    
    memset(pstWrkTxtr->pstNext, 0, sizeof(STRUCT_TEXTURE_LIST));
    pstWrkTxtr = pstWrkTxtr->pstNext;
    pstWrkTxtr->pSDL_Texture = IMG_LoadTexture(renderer, pszImgPath);
    pstWrkTxtr->pszImgPath = pszImgPath;
    pstWrkTxtr->pstNext = NULL;

    return pstWrkTxtr->pSDL_Texture;
  }
  void vTXTR_FreeList(){
    STRUCT_TEXTURE_LIST *pstWrkTxtr;

    for ( pstWrkTxtr = gstTextureList.pstNext; pstWrkTxtr != NULL; ){ 
      STRUCT_TEXTURE_LIST *pstLastTxtr = pstWrkTxtr;
      pstWrkTxtr = pstWrkTxtr->pstNext;
      free(pstLastTxtr);
    }
  }
  void vTXTR_InitList(){
    memset(&gstTextureList, 0, sizeof(STRUCT_TEXTURE_LIST));
    gstTextureList.pstNext = NULL;
  }
#endif /* _TEXTURE_H */