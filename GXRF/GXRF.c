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
#include <consts.h>

#ifndef GXRF_H_INC
  #include "GXRF.h"
#endif

#ifdef _WIN32
  #include <windows.h>
#elif LINUX
  #include <unistd.h>
#endif

// globals
PSTRUCT_GXRF_RENDER_LIST gpstGXRF_RenderList;
PSTRUCT_GXRF_OBJFNC_ARG_LIST gpstGXRF_FncArgList;

//
// Finds some specific object
//
STRUCT_GXRF_RENDER *pstGXRF_FindRenderizable(void *vGXRF_Renderizable){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender = NULL;
  // Looking for the Object
  // Running All of'em, until we get vGXRF_Renderizable
  for ( 
       pstGXRF_WrkRender = gpstGXRF_RenderList->pstGXRF_FirstRenderizable; // Init.
       ( 
        pstGXRF_WrkRender != NULL                                          //
        && pstGXRF_WrkRender->vSDL_ObjToRender != vGXRF_Renderizable       // Cond.
       );                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj                     // Incr.
  );

  return pstGXRF_WrkRender;
}

STRUCT_GXRF_RENDER *pstGXRF_FindLastRenderizable(__attribute__((unused)) void *vpPtr){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  if ( gpstGXRF_RenderList->pstGXRF_FirstRenderizable == NULL )
    return NULL;

  for ( 
       pstGXRF_WrkRender = gpstGXRF_RenderList->pstGXRF_FirstRenderizable;// Init.
       pstGXRF_WrkRender && pstGXRF_WrkRender->pNextObj != NULL;          // Cond.                                                        
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj                    // Incr.
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
       pstGXRF_WrkRender = gpstGXRF_RenderList->pstGXRF_FirstRenderizable;      // Init.
       ( 
        pstGXRF_WrkRender != NULL                          //
        && pstGXRF_WrkRender->eSDLTy != eSDLTy             // Cond.
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
        && pstGXRF_WrkRender->eSDLTy != eSDLTy               // Cond.
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

int iGXRF_AllocList(STRUCT_GXRF_RENDER_LIST **ppstGXRF_RenderizableList){
  ppstGXRF_RenderizableList = (STRUCT_GXRF_RENDER_LIST **) malloc(sizeof(STRUCT_GXRF_RENDER_LIST *) * sizeof(STRUCT_GXRF_RENDER_LIST));
  if ( ppstGXRF_RenderizableList == NULL ) // gg 
    return -1;

  (*ppstGXRF_RenderizableList) = (STRUCT_GXRF_RENDER_LIST *) malloc(sizeof(STRUCT_GXRF_RENDER_LIST));
  (*ppstGXRF_RenderizableList)->pstGXRF_FirstRenderizable = NULL;
  (*ppstGXRF_RenderizableList)->pstGXRF_LastRenderizable = NULL;

  return 0;
}

// 0: OK
//-1: Out of memory :( 
//
int iGXRF_AllocFncArgList(STRUCT_GXRF_OBJFNC_ARG_LIST **ppstGXRF_FncArgList){

   ppstGXRF_FncArgList = (STRUCT_GXRF_OBJFNC_ARG_LIST **) malloc(sizeof(STRUCT_GXRF_OBJFNC_ARG_LIST));
  if ( ppstGXRF_FncArgList == NULL ) // gg 
    return -1;

  (*ppstGXRF_FncArgList) = (STRUCT_GXRF_OBJFNC_ARG_LIST *) malloc(sizeof(STRUCT_GXRF_RENDER_LIST));
  (*ppstGXRF_FncArgList)->pstGXRF_FirstObjFncArg = NULL;
  (*ppstGXRF_FncArgList)->pstGXRF_LastObjFncArg = NULL;
  
  return 0;
}

int iGXRF_ADD2ArgList(void *vGXRF_Renderizable, void *vArgType, void *vArg){
  STRUCT_GXRF_OBJFNC_ARG *pstWrkObjArg;
  STRUCT_GXRF_OBJFNC_ARG_LIST *pstWrkArgList;
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vGXRF_Renderizable)) != NULL )
    return RENDERIZABLE_EXISTS; // Already Exists 

  pstWrkArgList = pstGXRF_WrkRender->pstArgList;
  if ( pstWrkArgList == NULL ) // gg 
    return -1;
  
  if ( (pstWrkObjArg = pstWrkArgList->pstGXRF_FirstObjFncArg) != NULL ){
    for ( ; pstWrkObjArg->pNextArg != NULL ; pstWrkObjArg = pstWrkObjArg->pNextArg );

    pstWrkObjArg->pNextArg = (STRUCT_GXRF_OBJFNC_ARG *) malloc(sizeof(STRUCT_GXRF_OBJFNC_ARG));
    pstWrkObjArg = pstWrkObjArg->pNextArg;
  }
  else {
     (*pstWrkArgList).pstGXRF_FirstObjFncArg = (STRUCT_GXRF_OBJFNC_ARG *) malloc(sizeof(STRUCT_GXRF_OBJFNC_ARG));
     pstWrkObjArg = (*pstWrkArgList).pstGXRF_FirstObjFncArg;
  }
  pstWrkObjArg->vArgType = vArgType;
  pstWrkObjArg->vArg = vArg;
  pstWrkObjArg->pNextArg = NULL;
  pstWrkArgList->pstGXRF_LastObjFncArg = pstWrkObjArg;
  return 0;
} 

int iGXRF_Init(){
  if ( DEBUG_MSGS ) vTraceMsg("iGXRF_Init --- Ok\n");

  if ( gpstGXRF_RenderList == NULL ){
    if ( iGXRF_AllocList(&gpstGXRF_RenderList) < 0 ) 
      return -1;
  }
  
  return 0;
}

int iGXRF_Add2RenderList(
  SDL_Renderer *renderer,
  int bIs2Render,
  eSDLT_Renderizable eSDLTy,
  void *vRenderObject, 
  void *vpfnRenderFnc){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;

  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vRenderObject)) != NULL )
    return RENDERIZABLE_EXISTS; // Already Exists 

  if ( (pstGXRF_WrkRender = pstGXRF_FindLastRenderizable(vRenderObject)) == NULL )
    return -1; // Error 
  else {
    if ( (pstGXRF_WrkRender = (STRUCT_GXRF_RENDER *) malloc(sizeof(STRUCT_GXRF_RENDER))) == NULL )
      return -2; // Error Malloc
    
    gpstGXRF_RenderList->pstGXRF_FirstRenderizable = pstGXRF_WrkRender;
    memset (pstGXRF_WrkRender, 0, sizeof(STRUCT_GXRF_RENDER));
  }

  iGXRF_AllocFncArgList(&pstGXRF_WrkRender->pstArgList);

  pstGXRF_WrkRender->bEnabled2Render  = bIs2Render;
  pstGXRF_WrkRender->eSDLTy           = eSDLTy;
  pstGXRF_WrkRender->vSDL_ObjToRender = vRenderObject;
  pstGXRF_WrkRender->vpfnRenderMethod = vpfnRenderFnc;
  pstGXRF_WrkRender->pSDL_Renderer    = renderer;
  pstGXRF_WrkRender->pNextObj = NULL;
  
  return 0;
}

void vGXRF_RenderObject(void *vGXRF_Renderizable){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;

  if ( (pstGXRF_WrkRender = pstGXRF_FindRenderizable(vGXRF_Renderizable)) == NULL )
    return; // Not Found

  if (pstGXRF_WrkRender->bEnabled2Render == FALSE )
    return;
  
  
  // pstGXRF_WrkRender->vpfnRenderMethod(
  //   pstGXRF_WrkRender->pSDL_Renderer,
  //   &pstGXRF_WrkRender->vlstRenderArgs
  // );
}

void vGXRF_RenderAll(){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  for ( 
       pstGXRF_WrkRender = gpstGXRF_RenderList->pstGXRF_FirstRenderizable;               // Init.
       pstGXRF_WrkRender != NULL ;                                   // Cond.
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj               // Incr.
  ){
    vGXRF_RenderObject(pstGXRF_WrkRender->vSDL_ObjToRender);
  }
}

void vGXRF_FreeRenderList(){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  
  for ( pstGXRF_WrkRender = gpstGXRF_RenderList->pstGXRF_FirstRenderizable;
        pstGXRF_WrkRender != NULL;
        ){

        pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj;
        free(gpstGXRF_RenderList->pstGXRF_FirstRenderizable);
        gpstGXRF_RenderList->pstGXRF_FirstRenderizable = pstGXRF_WrkRender;
  }
  gpstGXRF_RenderList = NULL;
}

int iGXRF_End(){
  vGXRF_FreeRenderList();
  return 0;
}

int iGXRF_InitStack(){
  return 0;
}

// int iGXRF_Push();



