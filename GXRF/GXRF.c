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
#include <stdarg.h>
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
        && pstGXRF_WrkRender->iSDL_RenderType != eSDLTy    // Cond.
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
        && pstGXRF_WrkRender->iSDL_RenderType != eSDLTy      // Cond.
       );                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj       // Incr.
  );

  // Found someone else, or NULL
  return (pstGXRF_CurrRenderObj = pstGXRF_WrkRender);
}

int bGXRF_SetRenderizable2Render(void *vRenderObject){
   STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vRenderObject)) == NULL )
    return FALSE; // Not Found

  // Thats our guy...
  return (pstGXRF_WrkRender->bEnabled2Render = TRUE);
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
  return 0;
}

int iGXRF_Add2RenderList(
    SDL_Renderer *renderer,
    int bIs2Render, 
    int iSDL_RenderType,
    void *vRenderObject, 
    void *vpfnRenderFnc, 
    va_list* vFncArgs){

  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
    
  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vRenderObject)) != NULL )
    return RENDERIZABLE_EXISTS; // Already Exists 

  if ( (pstGXRF_WrkRender = pstGXRF_FindLastRenderizable(vRenderObject)) == NULL )
    return -1; // Error 
  
  if ( (pstGXRF_WrkRender->pNextObj = (STRUCT_GXRF_RENDER *) malloc(sizeof(STRUCT_GXRF_RENDER))) == NULL )
    return -2; // Erro Malloc

  pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj;
  pstGXRF_WrkRender->bEnabled2Render  = bIs2Render;
  pstGXRF_WrkRender->iSDL_RenderType  = iSDL_RenderType;
  pstGXRF_WrkRender->vSDL_ObjToRender = vRenderObject;
  pstGXRF_WrkRender->vpfnRenderMethod = vpfnRenderFnc;
  pstGXRF_WrkRender->pSDL_Renderer    = renderer;
  pstGXRF_WrkRender->vargRenderArgs   = vFncArgs;
  pstGXRF_WrkRender->pNextObj = NULL;

  return 0;
}

void vGXRF_RenderObject(void *vRenderObject){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;

  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vRenderObject)) == NULL )
    return; // Not Found

  // SDL_SetRenderDrawColor(gstSDLRender.pSDL_Renderer, 255, 0, 0, 255); 
  pstGXRF_WrkRender->vpfnRenderMethod(
    pstGXRF_WrkRender->pSDL_Renderer,
    pstGXRF_WrkRender->vargRenderArgs
  );
}

void vGXRF_FreeRenderList(){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  for ( pstGXRF_WrkRender = gpstGXRF_FirstRenderizable;
        pstGXRF_WrkRender != NULL;
        ){

        pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj;
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



