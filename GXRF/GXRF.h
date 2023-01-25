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
  #include "trace.h"

  #define RENDERIZABLE_EXISTS 1

  typedef enum SDLTypes{
    SDL_RECT = 1,
    SDL_IMAGE,
    SDL_TTF,
    SDL_TEXTURE
  }eSDLT_Renderizable;

  // Creates the generic render callback function
  typedef void (*GXRFCALLBACK)(SDL_Renderer *renderer, ...);

  typedef struct STRUCT_GXRF_RENDER{
    int bEnabled2Render;
    int iSDL_RenderType;
    void *vSDL_ObjToRender;
    GXRFCALLBACK vpfnRenderMethod;
    va_list *vargRenderArgs;
    SDL_Renderer *pSDL_Renderer;
    struct STRUCT_GXRF_RENDER *pNextObj;
  } STRUCT_GXRF_RENDER;

  int iGXRF_Init();
  int iGXRF_End();
  int bGXRF_EnableRenderizable(void *vGXRF_Renderizable);
  int iGXRF_Add2RenderList(
    SDL_Renderer *renderer,
    int bIs2Render, 
    int iSDL_RenderType,
    void *vRenderObject, 
    void *vpfnRenderFnc, 
    va_list* vFncArgs);
  
  int bGXRF_EnableRenderizable(void *vGXRF_Renderizable);
  STRUCT_GXRF_RENDER *pstGXRF_FindRenderizable(void *vGXRF_Renderizable);
  STRUCT_GXRF_RENDER *pstGXRF_FindFirstRenderizableByType(eSDLT_Renderizable eSDLTy);
  STRUCT_GXRF_RENDER *pstGXRF_FindNextRenderizableByType (STRUCT_GXRF_RENDER *pstGXRF_CurrRenderObj, eSDLT_Renderizable eSDLTy);
  
  void vGXRF_RenderObject(void *vGXRF_Renderizable);
  void vGXRF_RenderAll();
  extern STRUCT_GXRF_RENDER *gpstGXRF_RenderList;
  extern STRUCT_GXRF_RENDER *gpstGXRF_FirstRenderizable;
  
#endif // ifndef GXRF_h_
