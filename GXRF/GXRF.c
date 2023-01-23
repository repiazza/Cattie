/*
*
* GXRF Framework for SDL2 in C lang
*
*/

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdarg.h>
#include <GXRF/GXRF.h>

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

STRUCT_GXRF_RENDER *gpstGXRF_Render;

enum SDLTypes{
  SDL_RECT = 1,
  SDL_IMAGE,
  SDL_TTF,
  SDL_TEXTURE
}eSDLT;


int bGXRF_SetObjectRenderable(void *vRenderObject){
   STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // Looking for the Object
  for( pstGXRF_WrkRender = gpstGXRF_Render;
       ( 
        pstGXRF_WrkRender != NULL
        && pstGXRF_WrkRender->vSDL_ObjToRender != vRenderObject
       );
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj
  );
  
  if ( pstGXRF_WrkRender == NULL )
    return; // Not Found

  // Thats our guy...
  pstGXRF_WrkRender->bIsObjToRender = TRUE;
}

int iGXRF_Init(){
  return ( gpstGXRF_Render == NULL ) ? -1 : 0;
}

void vGXRF_InitRenderList(){
  memset(gpstGXRF_Render, 0, sizeof(STRUCT_GXRF_RENDER));
  gpstGXRF_Render->pNextObj = NULL;
}

void vGXRF_AssignObj2RenderList(SDL_Renderer *renderer, void *vRenderObject, int iSDL_RenderType, void* vpfnRenderFnc, va_list* vFncArgs){
  gpstGXRF_Render->iSDL_RenderType  = iSDL_RenderType;
  gpstGXRF_Render->vSDL_ObjToRender = vRenderObject;
  gpstGXRF_Render->vpfnRenderMethod = vpfnRenderFnc;
  gpstGXRF_Render->pSDL_Renderer    = renderer;
  gpstGXRF_Render->vargRenderArgs   = vFncArgs;
}

void vRenderObjectFromList(void *vRenderObject, ...){
  STRUCT_GXRF_RENDER *pstGXRF_WrkRender;
  // Looking for the Object
  for( pstGXRF_WrkRender = gpstGXRF_Render;
       ( 
        pstGXRF_WrkRender != NULL
        && pstGXRF_WrkRender->vSDL_ObjToRender != vRenderObject
       );
       pstGXRF_WrkRender = pstGXRF_WrkRender->pNextObj
  );

  if ( pstGXRF_WrkRender == NULL )
    return; // Not Found

  // SDL_SetRenderDrawColor(gstSDLRender.pSDL_Renderer, 255, 0, 0, 255); 
  gpstGXRF_Render->vpfnRenderMethod(
    gpstGXRF_Render->pSDL_Renderer,
    gpstGXRF_Render->vargRenderArgs
  );
}
// 
// #define VSYNC_TIME 16.666666666 //tempo em ms para atualização em 60 FPS
// #define WINDOW_WIDTH  800
// #define WINDOW_HEIGHT 800
// #define DEBUG_MSGS 1
// #define DEBUG_MORE_MSGS 9
// int SDL_main(int argc, char *argv[]){
//   int iXTranslation = 0;
//   uint64_t ui64ElapsedTime;
//   SDL_Window* window;
//   SDL_Renderer* renderer;
//   char *pTok;
//   
//   sprintf(gszLogTitle, "%s", argv[0]);
//   if ( (pTok = strstr(gszLogTitle, ".exe")) != NULL ){
//     *pTok = 0;
//   }
//   strcat(gszLogTitle, ".log");
// 
//   vTraceMsg("SDL_Main --- Init\n");
// 
//   SDL_SetMainReady();
//   
//   // Initialize SDL
//   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//     if ( DEBUG_MSGS ) { 
//       char szMsg[256];
//       sprintf(szMsg,
//     "Couldn't initialize SDL: %s\n", 
//          SDL_GetError()
//       );
//       sMsg(szMsg);
//     }
//     return 1;
//   }
//   
//   // Create a window
//   window = SDL_CreateWindow(
//     "SDLFW",
//     SDL_WINDOWPOS_UNDEFINED,
//     SDL_WINDOWPOS_UNDEFINED, 
//     WINDOW_WIDTH, 
//     WINDOW_HEIGHT, 
//     SDL_WINDOW_SHOWN
//   );

  // Create a renderer
  // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // SDL_RenderPresent(renderer);
 
  // vInitRenderList(renderer, &SDL_RECT_Hud, SDL_RECT, SDL_RenderDrawRect, &SDL_RECT_Hud);
 
  // SDL_Event event;
  // while (gbRunning) {
  //   while ( SDL_PollEvent(&event) ){
  //     if (event.type == SDL_QUIT )
  //       gbRunning = FALSE;
  //       
  //     break;
  //   }
  //   ui64ElapsedTime = SDL_GetTicks64();
  //   
  //   SDL_SetRenderDrawColor(gpstGXRF_Render->pSDL_Renderer, 255, 255, 255, 255); 
  //   SDL_RenderClear(renderer);
    
    // vRenderObjectFromList(&SDL_RECT_Hud);
    // Update the screen
//     SDL_RenderPresent(renderer);
//     ui64ElapsedTime -= SDL_GetTicks64();
//     if (ui64ElapsedTime <= 20) SDL_Delay(20 - ui64ElapsedTime);
// 
//   }
  
  // Don't forget to destroy the texture when you're done with it
  // SDL_DestroyTexture(pSDL_TXTR_Hud);

  // SDL_DestroyRenderer(renderer);
  // SDL_DestroyWindow(window);
  // SDL_Quit();

  // return 0;
// }
