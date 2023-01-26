/*
*
* GXRF Framework source for SDL2
*
* Written by repiazza@gmail.com in January 2023
*
*/
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include "GXRF.h"

#ifdef _WIN32
  #include <windows.h>
  #define getpid _getpid 
#elif LINUX
  #include <unistd.h>
  #define SDL_main main
#endif

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE  1
#endif

STRUCT_GXRF_RENDER *gpstGXRF_RenderList;      // pointer to renderizable object list
STRUCT_GXRF_RENDER *gpstGXRF_FirstRenderizable; // pointer to first renderizable obj

//
// Finds some specific object
//
STRUCT_GXRF_RENDER *pstGXRF_FindRenderizable(void *vGXRF_Renderizable){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // Looking for the Object
  // Running All of'em, until we get vGXRF_Renderizable
  for ( 
       pstGXRF_WrkRender = gpstGXRF_FirstRenderizable;               // Init.
       ( 
        pstGXRF_WrkRender != NULL                                    //
        && pstGXRF_WrkRender->vSDL_ObjToRender != vGXRF_Renderizable // Cond.
       );                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj               // Incr.
  );

  return pstGXRF_WrkRender;
}

STRUCT_GXRF_RENDER *pstGXRF_FindLastRenderizable(){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;

  for ( 
       pstGXRF_WrkRender = gpstGXRF_FirstRenderizable;    // Init.
       pstGXRF_WrkRender ->pNextObj != NULL;              // Cond.                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj    // Incr.
  );

  return pstGXRF_WrkRender;
}

//
// Finds first object using Renderizable types
//
STRUCT_GXRF_RENDER *pstGXRF_FindFirstRenderizableByType(eSDLT_Renderizable eSDLTy){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // Looking for the Object
  // Running All of'em, until we get vRenderObject
  //
  for ( 
       pstGXRF_WrkRender = gpstGXRF_FirstRenderizable;     // Init.
       ( 
        pstGXRF_WrkRender != NULL                          //
        && pstGXRF_WrkRender->eSDLTy != eSDLTy    // Cond.
       );                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj     // Incr.
  );

  return pstGXRF_WrkRender;
}

STRUCT_GXRF_RENDER *pstGXRF_FindNextRenderizableByType(STRUCT_GXRF_RENDER *pstGXRF_CurrRenderObj, eSDLT_Renderizable eSDLTy){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // Looking for the next object of eSDLTy type 
  // begins from the last one found, pstGXRF_CurrRenderObj
  //
  for ( 
       pstGXRF_WrkRender = pstGXRF_CurrRenderObj;            // Init.
       ( 
        pstGXRF_WrkRender != NULL                            //
        && pstGXRF_WrkRender->eSDLTy != eSDLTy      // Cond.
       );                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj       // Incr.
  );

  // Found someone else, or NULL
  pstGXRF_CurrRenderObj = pstGXRF_WrkRender;
  return pstGXRF_CurrRenderObj;
}

int bGXRF_EnableRenderizable(void *vGXRF_Renderizable){
   STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vGXRF_Renderizable)) == NULL )
    return FALSE; // Not Found

  pstGXRF_WrkRender->bEnabled2Render = TRUE;
  // Thats our guy...
  return TRUE;
}

int iGXRF_AllocList(STRUCT_GXRF_RENDER **ppstGXRF_RenderizableList){
  *ppstGXRF_RenderizableList = (STRUCT_GXRF_RENDER *) malloc(sizeof(STRUCT_GXRF_RENDER));
    if ( *ppstGXRF_RenderizableList == NULL ) 
      return -1;

  return 0;
}

int iGXRF_Init(){
  if ( gpstGXRF_FirstRenderizable == NULL ){
    if ( iGXRF_AllocList(&gpstGXRF_RenderList) < 0 ) 
      return -1;
  }

  gpstGXRF_FirstRenderizable = gpstGXRF_RenderList;
  memset(gpstGXRF_FirstRenderizable, 0, sizeof(STRUCT_GXRF_RENDER));
  gpstGXRF_FirstRenderizable->pNextObj = NULL;
  gpstGXRF_FirstRenderizable->vSDL_ObjToRender = NULL;
  return 0;
}

int iGXRF_Add2RenderList(
    SDL_Renderer *renderer,
    int bIs2Render, 
    eSDLT_Renderizable eSDLTy,
    void *vRenderObject, 
    void *vpfnRenderFnc,
    int iVArgsCt,
    ...){

  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // va_list vFncArgs;
  // va_list vWrkArgs;
  int iVaCtr =0;
  // SDL_Renderer *pSDL_WrkRenderer;
  SDL_Rect *pSDL_Rect_test;
    
  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vRenderObject)) != NULL )
    return RENDERIZABLE_EXISTS; // Already Exists 

  if ( (pstGXRF_WrkRender = pstGXRF_FindLastRenderizable(vRenderObject)) == NULL )
    return -1; // Error 
  
  if ( gpstGXRF_FirstRenderizable != pstGXRF_WrkRender 
       || gpstGXRF_FirstRenderizable->vSDL_ObjToRender != NULL ){
    if ( (pstGXRF_WrkRender->pNextObj = (STRUCT_GXRF_RENDER *) malloc(sizeof(STRUCT_GXRF_RENDER))) == NULL )
      return -2; // Erro Malloc
  
    pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj;
  }
  
  //  va_start(vWrkArgs, iVArgsCt);

  pstGXRF_WrkRender->bEnabled2Render  = bIs2Render;
  pstGXRF_WrkRender->eSDLTy  = eSDLTy;
  pstGXRF_WrkRender->vSDL_ObjToRender = vRenderObject;
  pstGXRF_WrkRender->vpfnRenderMethod = vpfnRenderFnc;
  pstGXRF_WrkRender->pSDL_Renderer    = renderer;
  va_start(pstGXRF_WrkRender->vargRenderArgs, iVArgsCt); 
  // va_copy(pstGXRF_WrkRender->vargRenderArgs, va_arg(vWrkArgs, va_list));
  
  // pSDL_WrkRenderer = va_arg(pstGXRF_WrkRender->vargRenderArgs, SDL_Renderer *);
  pSDL_Rect_test = va_arg(pstGXRF_WrkRender->vargRenderArgs, SDL_Rect *);
  iVaCtr  = va_arg(pstGXRF_WrkRender->vargRenderArgs, int );
  va_end(pstGXRF_WrkRender->vargRenderArgs);
  pstGXRF_WrkRender->pNextObj = NULL;

  return 0;
}

void vGXRF_RenderObject(void *vGXRF_Renderizable){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // SDL_Renderer *pSDL_WrkRenderer;
  SDL_Rect *pSDL_Rect_test;
  int iVaCtr =0;
  char szMsg[32];

  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vGXRF_Renderizable)) == NULL )
    return; // Not Found

  if (pstGXRF_WrkRender->bEnabled2Render == FALSE )
    return;
  
  // SDL_SetRenderDrawColor(pstGXRF_WrkRender->pSDL_Renderer, 255, 0, 0, 255); 
  // pSDL_WrkRenderer = va_arg(pstGXRF_WrkRender->vargRenderArgs, SDL_Renderer *);
  pSDL_Rect_test = va_arg(pstGXRF_WrkRender->vargRenderArgs, SDL_Rect *);
  sprintf(szMsg, "%p", pSDL_Rect_test);
  iVaCtr = va_arg(pstGXRF_WrkRender->vargRenderArgs, int );
  pstGXRF_WrkRender->vargRenderArgs += (int)(pSDL_Rect_test) * sizeof(int) - sizeof(pstGXRF_WrkRender->vargRenderArgs);
  
  pstGXRF_WrkRender->vpfnRenderMethod(
    pstGXRF_WrkRender->pSDL_Renderer,
    pstGXRF_WrkRender->vargRenderArgs
  );
}

void vGXRF_RenderAll(){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  for ( 
       pstGXRF_WrkRender = gpstGXRF_FirstRenderizable;               // Init.
       pstGXRF_WrkRender != NULL ;                                   // Cond.
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj               // Incr.
  ){
    vGXRF_RenderObject(pstGXRF_WrkRender->vSDL_ObjToRender);
  }
}

void vGXRF_FreeRenderList(){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  for ( pstGXRF_WrkRender = gpstGXRF_FirstRenderizable;
        pstGXRF_WrkRender != NULL;
        ){

        pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj;
        va_end(gpstGXRF_FirstRenderizable->vargRenderArgs);
        free(gpstGXRF_FirstRenderizable);
        gpstGXRF_FirstRenderizable = pstGXRF_WrkRender;
  }
  gpstGXRF_RenderList = NULL;
  gpstGXRF_FirstRenderizable = NULL;
}

int iGXRF_End(){
  vGXRF_FreeRenderList();
  return 0;
}





