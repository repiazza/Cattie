#ifndef GRXF_h_
  #define GRXF_h_
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #include "trace.h"

  typedef void (*GXRFCALLBACK)(SDL_Renderer *renderer, ...);

  typedef struct STRUCT_GXRF_RENDER{
    void *vSDL_ObjToRender;
    int iSDL_RenderType;
    GXRFCALLBACK vpfnRenderMethod;
    SDL_Renderer *pSDL_Renderer;
    va_list *vargRenderArgs;
    struct STRUCT_GXRF_RENDER *pNextObj;
    int bIsObjToRender;
  }STRUCT_GXRF_RENDER;

  extern STRUCT_GXRF_RENDER *gpstGXRF_Render;

#endif // ifndef GRXF_h_