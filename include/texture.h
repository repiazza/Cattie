#include <cattie.h>
#include <util.h>
/** 
 * 
 *   texture.h
 * 
 *   Handles surfaces, textures, images, etc.
 * 
*/

#ifndef _TEXTURE_H
  #define _TEXTURE_H

  #define DESTROY_TEXTURES_NONE 0
  #define DESTROY_TEXTURES      1

  typedef struct STRUCT_TEXTURE_LIST{
    SDL_Texture *pSDL_Texture;
    char *pszImgPath;
    struct STRUCT_TEXTURE_LIST* pstNext;
  } STRUCT_TEXTURE_LIST, *PSTRUCT_TEXTURE_LIST;
  
  extern STRUCT_TEXTURE_LIST gstTextureList;

//   SDL_Texture *pSDL_TXTR_RectAddToList(SDL_Renderer *renderer, SDL_Rect *pSDL_Rect){
//     STRUCT_TEXTURE_LIST *pstWrkTxtr;
// 
//     for ( pstWrkTxtr = &gstTextureList; pstWrkTxtr->pstNext != NULL; pstWrkTxtr = pstWrkTxtr->pstNext );
//     
//     if ( pstWrkTxtr == &gstTextureList){
//       pstWrkTxtr->pSDL_Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pSDL_Rect->w, pSDL_Rect->h);
//       pstWrkTxtr->pszImgPath = NULL;
//       pstWrkTxtr->pstNext = NULL;
//       return pstWrkTxtr->pSDL_Texture;
//     }
//     if ( (pstWrkTxtr->pstNext = (PSTRUCT_TEXTURE_LIST) malloc(sizeof(STRUCT_TEXTURE_LIST))) == NULL )
//       return NULL;
//     
//     memset(pstWrkTxtr->pstNext, 0, sizeof(STRUCT_TEXTURE_LIST));
//     pstWrkTxtr = pstWrkTxtr->pstNext;
//     pstWrkTxtr->pSDL_Texture =SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pSDL_Rect->w, pSDL_Rect->h);
//     pstWrkTxtr->pszImgPath = NULL;
//     pstWrkTxtr->pstNext = NULL;
// 
//     return pstWrkTxtr->pSDL_Texture;
//   }
// 
//   SDL_Texture *pSDL_TXTR_ImageAddToList(SDL_Renderer *renderer, char *pszImgPath){
//     STRUCT_TEXTURE_LIST *pstWrkTxtr;
// 
//     for ( pstWrkTxtr = &gstTextureList; pstWrkTxtr->pstNext != NULL; pstWrkTxtr = pstWrkTxtr->pstNext );
//     
//     if ( pstWrkTxtr == &gstTextureList){
//       pstWrkTxtr->pSDL_Texture = IMG_LoadTexture(renderer, pszImgPath);
//       pstWrkTxtr->pszImgPath = pszImgPath;
//       pstWrkTxtr->pstNext = NULL;
//       return pstWrkTxtr->pSDL_Texture;
//     }
//     if ( (pstWrkTxtr->pstNext = (PSTRUCT_TEXTURE_LIST) malloc(sizeof(STRUCT_TEXTURE_LIST))) == NULL )
//       return NULL;
//     
//     memset(pstWrkTxtr->pstNext, 0, sizeof(STRUCT_TEXTURE_LIST));
//     pstWrkTxtr = pstWrkTxtr->pstNext;
//     pstWrkTxtr->pSDL_Texture = IMG_LoadTexture(renderer, pszImgPath);
//     pstWrkTxtr->pszImgPath = pszImgPath;
//     pstWrkTxtr->pstNext = NULL;
// 
//     return pstWrkTxtr->pSDL_Texture;
//   }
  SDL_Texture *pSDL_TXTR_AddToList(SDL_Renderer *renderer, SDL_Rect *pSDL_Rect, char *pszImgPath){
    STRUCT_TEXTURE_LIST *pstWrkTxtr;

    for ( pstWrkTxtr = &gstTextureList; pstWrkTxtr->pstNext != NULL; pstWrkTxtr = pstWrkTxtr->pstNext );
  
    pstWrkTxtr->pszImgPath = NULL;
    if ( pstWrkTxtr != &gstTextureList){
      if ( (pstWrkTxtr->pstNext = (PSTRUCT_TEXTURE_LIST) malloc(sizeof(STRUCT_TEXTURE_LIST))) == NULL )
        return NULL;
        
      pstWrkTxtr = pstWrkTxtr->pstNext; 
    }

    memset(pstWrkTxtr, 0, sizeof(STRUCT_TEXTURE_LIST));
    if ( !bStrIsEmpty(pszImgPath) ){
      pstWrkTxtr->pSDL_Texture = IMG_LoadTexture(renderer, pszImgPath);
      pstWrkTxtr->pszImgPath = pszImgPath;
    }
    else if ( pSDL_Rect != NULL ){
      pstWrkTxtr->pSDL_Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pSDL_Rect->w, pSDL_Rect->h);
    }
    pstWrkTxtr->pstNext = NULL;

    return pstWrkTxtr->pSDL_Texture;
  }

  void vTXTR_FreeList(int iDestroy){
    STRUCT_TEXTURE_LIST *pstWrkTxtr;

    for ( pstWrkTxtr = gstTextureList.pstNext; pstWrkTxtr != NULL; ){ 
      STRUCT_TEXTURE_LIST *pstLastTxtr = pstWrkTxtr;
      pstWrkTxtr = pstWrkTxtr->pstNext;
      if ( iDestroy && pstLastTxtr->pSDL_Texture != NULL ){
        SDL_DestroyTexture(pstLastTxtr->pSDL_Texture);
      }
      free(pstLastTxtr);
    }
  }
  void vTXTR_InitList(){
    memset(&gstTextureList, 0, sizeof(STRUCT_TEXTURE_LIST));
    gstTextureList.pstNext = NULL;
  }
#endif /* _TEXTURE_H */