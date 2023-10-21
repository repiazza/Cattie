/*
 *
 * GXRF Framework header file for SDL2
 *
 * Written by repiazza@gmail.com in January 2023 
 * 
 * $Id $
 */
 
#ifndef GXRF_H_INC
  #define GXRF_H_INC
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
#ifdef LINUX
  #include <SDL2/SDL_ttf.h>
#else
  #include <SDL2/SDL_TTF.h>
#endif /* LINUX */
  // #include <stdarg.h>
  #include <stdarg.h>
  #include <trace.h>
  #define SDL_NONE 0
  #define GRX_SDL_TYPES 4

  #define RENDERIZABLE_EXISTS 1

  typedef enum SDLTypes{
    SDL_RECT = 1,
    SDL_SURFACE,
    SDL_TTF,
    SDL_TEXTURE,
  } eSDLT_Renderizable;
// 
//   typedef int (GXRFTYPES)[GRX_SDL_TYPES];
//   
//   GXRFTYPES szSDLTypes = {
//     sizeof(SDL_Rect),       
//     sizeof(SDL_Surface),    
//     sizeof(TTF_Font *),      
//     sizeof(SDL_Texture *)   
//   };
  // Creates the generic render callback function
  typedef void (*GXRFCALLBACK)(SDL_Renderer *renderer, ...);

  // // Renderizable Object List
  typedef struct STRUCT_GXRF_RENDER_LIST{
    struct STRUCT_GXRF_RENDER *pstGXRF_FirstRenderizable;
    struct STRUCT_GXRF_RENDER *pstGXRF_LastRenderizable;
  }STRUCT_GXRF_RENDER_LIST, *PSTRUCT_GXRF_RENDER_LIST;
// 
  typedef struct STRUCT_GXRF_OBJFNC_ARG_LIST {
    struct STRUCT_GXRF_OBJFNC_ARG *pstGXRF_FirstObjFncArg;
    struct STRUCT_GXRF_OBJFNC_ARG *pstGXRF_LastObjFncArg;
  } STRUCT_GXRF_OBJFNC_ARG_LIST, *PSTRUCT_GXRF_OBJFNC_ARG_LIST;

  typedef struct STRUCT_GXRF_OBJFNC_ARG {
    void *vArg;
    struct STRUCT_GXRF_OBJFNC_ARG *pNextArg;
  } STRUCT_GXRF_OBJFNC_ARG, *PSTRUCT_GXRF_OBJFNC_ARG;

  // Renderizable Objects.
  //  bEnabled2Render - 1 Render | 0 Do nothing
  //  eSDLTy SDL type for vSDL_ObjToRender
  //  vSDL_ObjToRender ptr to SDL structure (Object)
  //  vlstRenderArgs - Arg list of this SDL Object 
  typedef struct STRUCT_GXRF_RENDER{
    int bEnabled2Render;
    eSDLT_Renderizable eSDLTy;
    SDL_Renderer *pSDL_Renderer;
    void *vSDL_ObjToRender;
    GXRFCALLBACK vpfnRenderMethod;
    va_list vlstRenderArgs;
    STRUCT_GXRF_OBJFNC_ARG *pstArgList;
    struct STRUCT_GXRF_RENDER *pNextObj;
  } STRUCT_GXRF_RENDER, *PSTRUCT_GXRF_RENDER;

  // Tipos de dados dos prms, para construcao do va_list
  typedef struct STRUCT_GXRF_FNCLIST{
    GXRFCALLBACK vpfnRenderMethod;
    eSDLT_Renderizable *peSDLTypes;
    struct STRUCT_GXRF_FNCLIST *pNextFnc;
  } STRUCT_GXRF_FNCLIST;
  
  void vGXRF_AttachValues2Fnc(STRUCT_GXRF_FNCLIST *pstFnctList, eSDLT_Renderizable *peSDLTypes, void* vpfnRenderMethod);
  
  extern va_list gpvlstGXRF_ArgList;
  int iGXRF_Init();
  int iGXRF_End();
  int iGXRF_Add2RenderList(
    SDL_Renderer *renderer,
    int bIs2Render,
    eSDLT_Renderizable eSDLTy,
    void *vRenderObject, 
    void *vpfnRenderFnc,
    int iVArgsCt,
  ...);
  STRUCT_GXRF_RENDER *pstGXRF_FindFirstRenderizableByType(eSDLT_Renderizable eSDLTy);
  STRUCT_GXRF_RENDER *pstGXRF_FindNextRenderizableByType (STRUCT_GXRF_RENDER *pstGXRF_CurrRenderObj, eSDLT_Renderizable eSDLTy);

  int bGXRF_EnableRenderizable(void *vGXRF_Renderizable);
  void vGXRF_RenderObject(void *vGXRF_Renderizable);
  void vGXRF_RenderAll();

  extern PSTRUCT_GXRF_RENDER_LIST gpstGXRF_RenderList;
  
#endif // ifndef GXRF_h_
