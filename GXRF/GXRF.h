/*
*
* GXRF Framework header file for SDL2
*
* Written by repiazza@gmail.com in January 2023 
*
*/

#ifndef GXRF_h_
  #define GXRF_h_
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #include <stdarg.h>
  #include "trace.h"

  #define RENDERIZABLE_EXISTS 1

  typedef enum SDLTypes{
    SDL_RECT = 1,
    SDL_IMAGE,
    SDL_TTF,
    SDL_TEXTURE,
  } eSDLT_Renderizable;

  // Creates the generic render callback function
  typedef void (*GXRFCALLBACK)(SDL_Renderer *renderer, ...);
  
  // Renderizable Objects.
  //  bEnabled2Render - 1 Render | 0 Do nothing
  //  eSDLTy SDL type for vSDL_ObjToRender
  //  vSDL_ObjToRender ptr to SDL structure (Object)
  //  vlstRenderArgs - Arg list of this SDL Object 
  typedef struct STRUCT_GXRF_RENDER{
    int bEnabled2Render;
    eSDLT_Renderizable eSDLTy;
    void *vSDL_ObjToRender;
    GXRFCALLBACK vpfnRenderMethod;
    va_list vlstRenderArgs;
    SDL_Renderer *pSDL_Renderer;
    struct STRUCT_GXRF_RENDER *pNextObj;
  } STRUCT_GXRF_RENDER;

  // Tipos de dados dos prms, para construcao do 
  // va_list
  typedef struct STRUCT_GXRF_FNCLIST{
    GXRFCALLBACK vpfnRenderMethod;
    eSDLT_Renderizable *peSDLTypes;
    struct STRUCT_GXRF_FNCLIST *pNextFnc;
  } STRUCT_GXRF_FNCLIST;
  
  void vGXRF_AttachValues2Fnc(STRUCT_GXRF_FNCLIST *pstFnctList, eSDLT_Renderizable *peSDLTypes, void* vpfnRenderMethod);
  int iGXRF_Init();
  int iGXRF_End();
  int iGXRF_Add2RenderList(
    SDL_Renderer *renderer,
    int bIs2Render,
    eSDLT_Renderizable eSDLTy,
    void *vRenderObject, 
    void *vpfnRenderFnc,
    int iVArgsCt,
    va_list *pvlstFnArgList);
    
  STRUCT_GXRF_RENDER *pstGXRF_FindFirstRenderizableByType(eSDLT_Renderizable eSDLTy);
  STRUCT_GXRF_RENDER *pstGXRF_FindNextRenderizableByType (STRUCT_GXRF_RENDER *pstGXRF_CurrRenderObj, eSDLT_Renderizable eSDLTy);

  int bGXRF_EnableRenderizable(void *vGXRF_Renderizable);
  void vGXRF_RenderObject(void *vGXRF_Renderizable);
  void vGXRF_RenderAll();
  extern STRUCT_GXRF_RENDER *gpstGXRF_RenderList;
  extern STRUCT_GXRF_RENDER *gpstGXRF_FirstRenderizable;
  
#endif // ifndef GXRF_h_
